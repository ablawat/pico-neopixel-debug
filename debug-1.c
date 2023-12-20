/**************************************************************************************************/
/**                      Include Files                                                           **/
/**************************************************************************************************/

#include "debug-1.h"
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
#define RESET_BITS_NUM          UINT16_C(90)

/*
** @brief   Bit Offsets for Red, Green and Blue Subpixels
*/
#define SUBPIXEL_OFFSET_RED     (UINT8_C(1) * SUBPIXEL_BITS_NUM)
#define SUBPIXEL_OFFSET_GREEN   (UINT8_C(0) * SUBPIXEL_BITS_NUM)
#define SUBPIXEL_OFFSET_BLUE    (UINT8_C(2) * SUBPIXEL_BITS_NUM)

/*
** @brief   PWM Wrap Value to Generate 1 MHz Output Frequency
*/
#define PWM_WRAP_1MHZ           UINT16_C(124)

/*
** @brief   PWM Counter Compare Value for Complete Low Pulse
*/
#define PWM_CC_LOW              UINT16_C(0)

/*
** @brief   PWM Counter Compare Value for 
*/
#define PWM_CC_BIT_0            UINT16_C(40)

/*
** @brief   PWM Counter Compare Value for 
*/
#define PWM_CC_BIT_1            UINT16_C(90)

/*
** @brief   
*/
#define PWM_CC_BIT_SHIFT        (DEBUG_NEOPIXEL_PWM_CHANNEL_NUM * UINT8_C(16))

/*
** @brief   
*/
#define PIXEL_BIT_0             ((uint32_t)(PWM_CC_BIT_0) << PWM_CC_BIT_SHIFT)
/*
** @brief   
*/
#define PIXEL_BIT_1             ((uint32_t)(PWM_CC_BIT_1) << PWM_CC_BIT_SHIFT)

/*
** @brief   
*/
#define PWM_CC_ADDR_OFFSET      UINT8_C(0x0C)

/*
** @brief   
*/
#define PWM_SLICE_SIZE          UINT8_C(0x14)

/*
** @brief   
*/
#define PWM_SLICE_ADDR          (DEBUG_NEOPIXEL_PWM_SLICE_NUM * PWM_SLICE_SIZE)

/*
** @brief   
*/
#define ADDR_SET    (volatile uint32_t *)(PWM_BASE + PWM_SLICE_ADDR + PWM_CC_ADDR_OFFSET)

/*
** @brief   Output Pixel Data Encoded for PWM
*/
struct neopixel_led_data
{
    uint32_t m_pixel[PIXEL_BITS_NUM];
    uint32_t m_reset[RESET_BITS_NUM];
};

/*
** @brief   LED Color Setting
*/
struct neopixel_led_color
{
    enum debug_color_e   m_color;
    bool                 m_subpixels[SUBPIXELS_NUM];
};

/*
** @brief   Buffer for Send and Receive Data on I2C Bus
*/
static struct neopixel_led_data buffer;

/*
** @brief   Debug NeoPixel LED Defined Colors
*/
static struct neopixel_led_color colors[6] =
{
    { DEBUG_COLOR_RED,     {  true, false, false } },
    { DEBUG_COLOR_GREEN,   { false,  true, false } },
    { DEBUG_COLOR_BLUE,    { false, false,  true } },
    { DEBUG_COLOR_YELLOW,  {  true,  true, false } },
    { DEBUG_COLOR_CYAN,    { false,  true,  true } },
    { DEBUG_COLOR_MAGENTA, {  true, false,  true } },
};

static uint neopixel_led_dma;

static void set_subpixels (bool red, bool green, bool blue);

/**************************************************************************************************/
/**                      Global Functions                                                        **/
/**************************************************************************************************/

/*
** @brief   Function
*/
void debug1_init (void)
{
    /* Initialize pixel color data */
    for (uint32_t i = UINT32_C(0); i < PIXEL_BITS_NUM; i++)
    {
        buffer.m_pixel[i] = PIXEL_BIT_0;
    }

    /* Initialize reset pulse data */
    for (uint32_t i = UINT32_C(0); i < RESET_BITS_NUM; i++)
    {
        buffer.m_reset[i] = PWM_CC_LOW;
    }

    /* Configure PWM slice and channel for NeoPixel output pin */
    gpio_set_function(DEBUG_NEOPIXEL_OUTPUT_PIN, GPIO_FUNC_PWM);

    pwm_set_wrap       (DEBUG_NEOPIXEL_PWM_SLICE_NUM, PWM_WRAP_1MHZ);
    pwm_set_chan_level (DEBUG_NEOPIXEL_PWM_SLICE_NUM, DEBUG_NEOPIXEL_PWM_CHANNEL_NUM, PWM_CC_LOW);
    pwm_set_enabled    (DEBUG_NEOPIXEL_PWM_SLICE_NUM, true);

    /* Configure DMA channel for PWM slice */
    int pwm_dma_channel = dma_claim_unused_channel(true);

    neopixel_led_dma = pwm_dma_channel;

    /* check result */

    dma_channel_config pwm_dma_channel_config = dma_channel_get_default_config(pwm_dma_channel);

    channel_config_set_transfer_data_size (&pwm_dma_channel_config, DMA_SIZE_32);
    channel_config_set_read_increment     (&pwm_dma_channel_config, true);
    channel_config_set_write_increment    (&pwm_dma_channel_config, false);
    channel_config_set_dreq               (&pwm_dma_channel_config, DREQ_PWM_WRAP0 + DEBUG_NEOPIXEL_PWM_SLICE_NUM);

    /* Prepare  */
    dma_channel_configure(pwm_dma_channel,
                          &pwm_dma_channel_config,
                          ADDR_SET,
                          &buffer,
                          PIXEL_BITS_NUM + RESET_BITS_NUM,
                          false);
}

/*
** @brief   Function
*/
void debug1_set_color (enum debug_color_e color)
{
    /* Wait for previous transfer complete */
    dma_channel_wait_for_finish_blocking(neopixel_led_dma);

    uint_fast8_t i = UINT8_C(0);

    for (i = UINT8_C(0); i < 6; i++)
    {
        if (colors[i].m_color == color)
        {
            set_subpixels(colors[i].m_subpixels[0],
                          colors[i].m_subpixels[1],
                          colors[i].m_subpixels[2]);

            break;
        }
    }

    if (i == 6)
    {
        set_subpixels(false, false, false);
    }

    /* Trigger next transfer */
    dma_channel_set_read_addr(neopixel_led_dma, &buffer, true);
}

/*
** @brief   Function
*/
void debug1_turn_off (void)
{
    /* Wait for previous transfer complete */
    dma_channel_wait_for_finish_blocking(neopixel_led_dma);

    set_subpixels(false, false, false);

    /* Trigger next transfer */
    dma_channel_set_read_addr(neopixel_led_dma, &buffer, true);
}

/**************************************************************************************************/
/**                      Local Functions                                                         **/
/**************************************************************************************************/

/*
** @brief   Function
*/
static void set_subpixels (bool red, bool green, bool blue)
{
    /* Setup green subpixel data */
    for (uint_fast8_t i = UINT8_C(0); i < SUBPIXEL_BITS_NUM; i++)
    {
        if (green == true)
        {
            buffer.m_pixel[i + SUBPIXEL_OFFSET_GREEN] = PIXEL_BIT_1;
        }
        else
        {
            buffer.m_pixel[i + SUBPIXEL_OFFSET_GREEN] = PIXEL_BIT_0;
        }
    }

    /* Setup red subpixel data */
    for (uint_fast8_t i = UINT8_C(0); i < SUBPIXEL_BITS_NUM; i++)
    {
        if (red == true)
        {
            buffer.m_pixel[i + SUBPIXEL_OFFSET_RED] = PIXEL_BIT_1;
        }
        else
        {
            buffer.m_pixel[i + SUBPIXEL_OFFSET_RED] = PIXEL_BIT_0;
        }
    }

    /* Setup blue subpixel data */
    for (uint_fast8_t i = UINT8_C(0); i < SUBPIXEL_BITS_NUM; i++)
    {
        if (blue == true)
        {
            buffer.m_pixel[i + SUBPIXEL_OFFSET_BLUE] = PIXEL_BIT_1;
        }
        else
        {
            buffer.m_pixel[i + SUBPIXEL_OFFSET_BLUE] = PIXEL_BIT_0;
        }
    }
}
