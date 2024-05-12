#ifndef PWM_H
#define PWM_H

/**************************************************************************************************/
/**                      Include Files                                                           **/
/**************************************************************************************************/

#include <stdint.h>
#include "cfg.h"

/**************************************************************************************************/
/**                      Global Types                                                            **/
/**************************************************************************************************/

/*
** @brief   PWM Wrap Value to Generate 1 MHz Output Frequency
*/
#define PWM_WRAP_1MHZ           UINT16_C(124)

/*
** @brief   PWM Counter Compare Value for Complete Low Pulse
*/
#define PWM_CC_LOW              UINT16_C(0)

/*
** @brief   PWM Counter Compare Value for Generating Data Bit 0
*/
#define PWM_CC_BIT_0            UINT16_C(40)

/*
** @brief   PWM Counter Compare Value for Generating Data Bit 1
*/
#define PWM_CC_BIT_1            UINT16_C(90)

/*
** @brief   PWM Counter Compare Bit Shift
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
** @brief   PWM Counter Compare Register Address Offset
*/
#define PWM_CC_ADDR_OFFSET      UINT8_C(0x0C)

/*
** @brief   Size of Registers Area for one PWM Slice (2-channel)
*/
#define PWM_SLICE_SIZE          UINT8_C(0x14)

/*
** @brief   PWM Slice Address
*/
#define PWM_SLICE_ADDR          (DEBUG_NEOPIXEL_PWM_SLICE_NUM * PWM_SLICE_SIZE)

/*
** @brief   
*/
#define ADDR_SET    (volatile uint32_t *)(PWM_BASE + PWM_SLICE_ADDR + PWM_CC_ADDR_OFFSET)

#endif
