/**************************************************************************************************/
/**                      Include Files                                                           **/
/**************************************************************************************************/

#include "ws2812b.h"
#include "pwm.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/dma.h"

/**************************************************************************************************/
/**                      Local Macros                                                            **/
/**************************************************************************************************/

/*
** @brief   Number of Bits Used for LED Subpixel
*/
#define SUBPIXEL_BITS_NUM       UINT8_C(8)

/*
** @brief   Number of Subpixels Used in LED Pixel
*/
#define SUBPIXELS_NUM           UINT8_C(3)

/*
** @brief   Number of Bits Used for LED Pixel
*/
#define PIXEL_BITS_NUM          (SUBPIXELS_NUM * SUBPIXEL_BITS_NUM)

/*
** @brief   Number of Bits Used for LED Reset Pulse
*/
#define RESET_BITS_NUM          UINT16_C(280)

/*
** @brief   Bit Offsets for Red, Green and Blue Subpixels
*/
#define SUBPIXEL_OFFSET_RED     (UINT8_C(1) * SUBPIXEL_BITS_NUM)
#define SUBPIXEL_OFFSET_GREEN   (UINT8_C(0) * SUBPIXEL_BITS_NUM)
#define SUBPIXEL_OFFSET_BLUE    (UINT8_C(2) * SUBPIXEL_BITS_NUM)


/*
** @brief   Output Pixel Data Encoded for PWM
*/
typedef struct neopixel_led_data
{
    /*
    ** @brief   Encoded Pixel Color
    */
    uint32_t m_pixel[PIXEL_BITS_NUM];

    /*
    ** @brief   Encoded Reset Pulse
    */
    uint32_t m_reset;
}
neopixel_led_data;

/*
** @brief   Buffer for Send Data Through PWM
*/
static neopixel_led_data    neopixel_output;

/*
** @brief   DMA Channel Used for PWM
*/
static uint                 neopixel_dma;

/*
** @brief   Time Counter Used for Delay
*/
static uint64_t             neopixel_time;

/**************************************************************************************************/
/**                      Local Function Declarations                                             **/
/**************************************************************************************************/

static void set_subpixels (uint8_t red, uint8_t green, uint8_t blue);

/**************************************************************************************************/
/**                      Global Functions                                                        **/
/**************************************************************************************************/

/*
** @brief   Function
*/
void ws2812b_init (void)
{
    /* initialize pixel color data */
    for (uint_fast8_t i = UINT8_C(0); i < PIXEL_BITS_NUM; i++)
    {
        neopixel_output.m_pixel[i] = PIXEL_BIT_0;
    }

    /* initialize reset pulse data */
    neopixel_output.m_reset = PWM_CC_LOW;

    /* configure PWM slice and channel for NeoPixel output */
    gpio_set_function(DEBUG_NEOPIXEL_OUTPUT_PIN, GPIO_FUNC_PWM);
    //gpio_set_function(1, GPIO_FUNC_PWM);

    pwm_set_wrap       (DEBUG_NEOPIXEL_PWM_SLICE_NUM, PWM_WRAP_1MHZ);
    pwm_set_chan_level (DEBUG_NEOPIXEL_PWM_SLICE_NUM, DEBUG_NEOPIXEL_PWM_CHANNEL_NUM, PWM_CC_LOW);
    pwm_set_enabled    (DEBUG_NEOPIXEL_PWM_SLICE_NUM, true);

    /* configure DMA channel for PWM slice */
    int pwm_dma_channel = dma_claim_unused_channel(true);

    /* check result */
    if (pwm_dma_channel == -1)
    {

    }

    neopixel_dma = pwm_dma_channel;

    /* configure DMA channel for NeoPixel output */
    dma_channel_config pwm_dma_channel_config = dma_channel_get_default_config(pwm_dma_channel);

    channel_config_set_transfer_data_size (&pwm_dma_channel_config, DMA_SIZE_32);
    channel_config_set_read_increment     (&pwm_dma_channel_config, true);
    channel_config_set_write_increment    (&pwm_dma_channel_config, false);
    channel_config_set_dreq               (&pwm_dma_channel_config, DREQ_PWM_WRAP0 + DEBUG_NEOPIXEL_PWM_SLICE_NUM);

    /* prepare transmission from buffer to PWM */
    dma_channel_configure(pwm_dma_channel,
                          &pwm_dma_channel_config,
                          ADDR_SET,
                          &neopixel_output,
                          PIXEL_BITS_NUM + UINT8_C(1),
                          false);
}

/*
** @brief   Function
*/
void ws2812b_set_pixel (uint8_t red, uint8_t green, uint8_t blue)
{
    /* wait until previous transfer is completed */
    dma_channel_wait_for_finish_blocking(neopixel_dma);

    /* write new color data */
    set_subpixels(red, green, blue);
}

/*
** @brief   Function
*/
void ws2812b_send (void)
{
    /* wait until previous transfer is completed */
    while (true)
    {
        /* get current system time counter */
        uint64_t time_current = time_us_64();

        /* get duration from start of previous transfer */
        time_current -= neopixel_time;

        if (time_current > (PIXEL_BITS_NUM + RESET_BITS_NUM))
        {
            /* get current system time counter */
            neopixel_time = time_us_64();

            /* transfer has completed */
            break;
        }
    }

    /* trigger next transfer */
    dma_channel_set_read_addr(neopixel_dma, &neopixel_output, true);
}

/*
** @brief   Function
*/
static void set_subpixels (uint8_t red, uint8_t green, uint8_t blue)
{
    uint_fast8_t i = UINT8_C(0);
    uint8_t mask = UINT8_C(0x80);

    /* setup green subpixel data */
    while (mask > UINT8_C(0))
    {
        if ((green && mask) > UINT8_C(0))
        {
            neopixel_output.m_pixel[SUBPIXEL_OFFSET_GREEN + i] = PIXEL_BIT_1;
        }
        else
        {
            neopixel_output.m_pixel[SUBPIXEL_OFFSET_GREEN + i] = PIXEL_BIT_0;
        }

        mask = mask >> UINT8_C(1);
        i++;
    }

    i = UINT8_C(0);
    mask = UINT8_C(0x80);

    /* setup red subpixel data */
    while (mask > UINT8_C(0))
    {
        if ((red && mask) > UINT8_C(0))
        {
            neopixel_output.m_pixel[SUBPIXEL_OFFSET_RED + i] = PIXEL_BIT_1;
        }
        else
        {
            neopixel_output.m_pixel[SUBPIXEL_OFFSET_RED + i] = PIXEL_BIT_0;
        }

        mask = mask >> UINT8_C(1);
        i++;
    }

    i = UINT8_C(0);
    mask = UINT8_C(0x80);

    /* setup blue subpixel data */
    while (mask > UINT8_C(0))
    {
        if ((blue && mask) > UINT8_C(0))
        {
            neopixel_output.m_pixel[SUBPIXEL_OFFSET_BLUE + i] = PIXEL_BIT_1;
        }
        else
        {
            neopixel_output.m_pixel[SUBPIXEL_OFFSET_BLUE + i] = PIXEL_BIT_0;
        }

        mask = mask >> UINT8_C(1);
        i++;
    }
}
