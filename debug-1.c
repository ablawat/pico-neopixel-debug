/**************************************************************************************************/
/**                      Include Files                                                           **/
/**************************************************************************************************/

#include "debug-1.h"
#include "ws2812b.h"

/**************************************************************************************************/
/**                      Local Macros                                                            **/
/**************************************************************************************************/

/*
** @brief   Number of LED Pixel Subpixels
*/
#define LED_SUBPIXELS_NUM           UINT8_C(3)

/*
** @brief   Number of LED Pixel Colors
*/
#define LED_COLORS_NUM              UINT8_C(7)

/**************************************************************************************************/
/**                      Local Types                                                             **/
/**************************************************************************************************/

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
static struct neopixel_led_color colors[LED_COLORS_NUM] =
{
    { DEBUG_COLOR_NONE,    { UINT8_C(0x00), UINT8_C(0x00), UINT8_C(0x00) } },
    { DEBUG_COLOR_RED,     { UINT8_C(0xFF), UINT8_C(0x00), UINT8_C(0x00) } },
    { DEBUG_COLOR_GREEN,   { UINT8_C(0x00), UINT8_C(0xFF), UINT8_C(0x00) } },
    { DEBUG_COLOR_BLUE,    { UINT8_C(0x00), UINT8_C(0x00), UINT8_C(0xFF) } },
    { DEBUG_COLOR_YELLOW,  { UINT8_C(0xFF), UINT8_C(0xFF), UINT8_C(0x00) } },
    { DEBUG_COLOR_CYAN,    { UINT8_C(0x00), UINT8_C(0xFF), UINT8_C(0xFF) } },
    { DEBUG_COLOR_MAGENTA, { UINT8_C(0xFF), UINT8_C(0x00), UINT8_C(0xFF) } },
};

/*
** @brief   Debug NeoPixel LED Displayed Color
*/
static debug_color_e    color_current;

/**************************************************************************************************/
/**                      Global Functions                                                        **/
/**************************************************************************************************/

/*
** @brief   Function
*/
void debug1_init (void)
{
    /* set pixel color */
    color_current = DEBUG_COLOR_NONE;

    /* initialize pixel color data */
    ws2812b_init();
}

/*
** @brief   Function
*/
void debug1_set_color (enum debug_color_e color)
{
    if (color_current != color)
    {
        for (uint_fast8_t i = UINT8_C(0); i < LED_COLORS_NUM; i++)
        {
            if (colors[i].m_color == color)
            {
                ws2812b_set_pixel(colors[i].m_subpixels[0],
                                  colors[i].m_subpixels[1],
                                  colors[i].m_subpixels[2]);

                color_current = color;

                break;
            }
        }

        /* apply new color */
        ws2812b_send();
    }
}

/*
** @brief   Function
*/
void debug1_turn_off (void)
{
    if (color_current != DEBUG_COLOR_NONE)
    {
        for (uint_fast8_t i = UINT8_C(0); i < LED_COLORS_NUM; i++)
        {
            if (colors[i].m_color == DEBUG_COLOR_NONE)
            {
                ws2812b_set_pixel(colors[i].m_subpixels[0],
                                  colors[i].m_subpixels[1],
                                  colors[i].m_subpixels[2]);

                color_current = DEBUG_COLOR_NONE;

                break;
            }
        }

        /* apply new color */
        ws2812b_send();
    }
}

/**************************************************************************************************/
/**                      Local Functions                                                         **/
/**************************************************************************************************/
