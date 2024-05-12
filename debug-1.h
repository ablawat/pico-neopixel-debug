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
** @brief   Debug NeoPixel LED Colors
*/
typedef enum debug_color_e
{
    DEBUG_COLOR_RED,
    DEBUG_COLOR_GREEN,
    DEBUG_COLOR_BLUE,
    DEBUG_COLOR_YELLOW,
    DEBUG_COLOR_CYAN,
    DEBUG_COLOR_MAGENTA,
}
debug_color_e;

/**************************************************************************************************/
/**                      Global Functions                                                        **/
/**************************************************************************************************/

void debug1_init (void);

void debug1_set_color (debug_color_e color);
void debug1_turn_off  (void);

#endif
