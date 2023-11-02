#ifndef DEBUG1_H
#define DEBUG1_H

/**************************************************************************************************/
/**                      Include Files                                                           **/
/**************************************************************************************************/

#include <stdint.h>
#include <stddef.h>

/**************************************************************************************************/
/**                      Global Types                                                            **/
/**************************************************************************************************/

/*
** @brief   Debug NeoPixel LED PWM Slice Number
*/
#define DEBUG_NEOPIXEL_PWM_SLICE_NUM        UINT8_C(0)

/*
** @brief   Debug NeoPixel LED PWM Slice Channel Number
*/
#define DEBUG_NEOPIXEL_PWM_CHANNEL_NUM      UINT8_C(1)

/*
** @brief   Debug NeoPixel LED Output Pin
*/
#define DEBUG_NEOPIXEL_OUTPUT_PIN           UINT8_C(17)

/*
** @brief   Debug NeoPixel LED Color
*/
enum debug_color_e
{
    DEBUG_COLOR_RED,
    DEBUG_COLOR_GREEN,
    DEBUG_COLOR_BLUE,
};

/**************************************************************************************************/
/**                      Global Functions                                                        **/
/**************************************************************************************************/

void debug1_init (void);

void debug1_set_color (enum debug_color_e color);
void debug1_turn_off  (void);

#endif
