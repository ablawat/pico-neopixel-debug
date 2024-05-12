/**************************************************************************************************/
/**                      Include Files                                                           **/
/**************************************************************************************************/

#include "debug-1.h"
#include "ws2812b.h"

/**************************************************************************************************/
/**                      Local Macros                                                            **/
/**************************************************************************************************/

/*
** @brief   Number of Subpixels Used in LED Pixel
*/
#define LED_SUBPIXELS_NUM           UINT8_C(3)

/*
** @brief   LED Color Setting
*/
struct neopixel_led_color
{
    debug_color_e   m_color;
    uint8_t         m_subpixels[LED_SUBPIXELS_NUM];
};

/*
** @brief   Debug NeoPixel LED Defined Colors
*/
static struct neopixel_led_color colors[6] =
{
    { DEBUG_COLOR_RED,     { UINT8_C(0xFF), UINT8_C(0x00), UINT8_C(0x00) } },
    { DEBUG_COLOR_GREEN,   { UINT8_C(0x00), UINT8_C(0xFF), UINT8_C(0x00) } },
    { DEBUG_COLOR_BLUE,    { UINT8_C(0x00), UINT8_C(0x00), UINT8_C(0xFF) } },
    { DEBUG_COLOR_YELLOW,  { UINT8_C(0xFF), UINT8_C(0xFF), UINT8_C(0x00) } },
    { DEBUG_COLOR_CYAN,    { UINT8_C(0x00), UINT8_C(0xFF), UINT8_C(0xFF) } },
    { DEBUG_COLOR_MAGENTA, { UINT8_C(0xFF), UINT8_C(0x00), UINT8_C(0xFF) } },
};

/**************************************************************************************************/
/**                      Global Functions                                                        **/
/**************************************************************************************************/

/*
** @brief   Function
*/
void debug1_init (void)
{
    /* initialize pixel color data */
    ws2812b_init();
}

/*
** @brief   Function
*/
void debug1_set_color (enum debug_color_e color)
{
    for (uint_fast8_t i = UINT8_C(0); i < 6; i++)
    {
        if (colors[i].m_color == color)
        {
            ws2812b_set_pixel(colors[i].m_subpixels[0],
                              colors[i].m_subpixels[1],
                              colors[i].m_subpixels[2]);

            break;
        }
    }

    ws2812b_send();
}

/*
** @brief   Function
*/
void debug1_turn_off (void)
{
    ws2812b_set_pixel(UINT8_C(0x00), UINT8_C(0x00), UINT8_C(0x00));

    /* trigger next transfer */
    ws2812b_send();
}

/**************************************************************************************************/
/**                      Local Functions                                                         **/
/**************************************************************************************************/
