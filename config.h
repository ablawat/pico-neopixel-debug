#ifndef DEBUG1_CONFIG_H
#define DEBUG1_CONFIG_H

/**************************************************************************************************/
/**                      Include Files                                                           **/
/**************************************************************************************************/

#include <stdint.h>

/**************************************************************************************************/
/**                      Global Types                                                            **/
/**************************************************************************************************/

/*
** @brief   Debug NeoPixel LED PWM Slice Number
**
** @value   @MIN: 0, @MAX: 7, @Unit: PWM Slice Number
*/
#define DEBUG_NEOPIXEL_PWM_SLICE_NUM        UINT8_C(0)

/*
** @brief   Debug NeoPixel LED PWM Channel Number
**
** @value   @VAL: 0 - PWM Channel A
** @value   @VAL: 1 - PWM Channel B
*/
#define DEBUG_NEOPIXEL_PWM_CHANNEL_NUM      UINT8_C(1)

/*
** @brief   Debug NeoPixel LED Output Pin
**
** @value   @MIN: 0, @MAX: 29, @Unit: GPIO Number
*/
#define DEBUG_NEOPIXEL_OUTPUT_PIN           UINT8_C(17)

#endif
