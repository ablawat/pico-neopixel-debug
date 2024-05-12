#ifndef WS2812B_H
#define WS2812B_H

/**************************************************************************************************/
/**                      Include Files                                                           **/
/**************************************************************************************************/

#include <stdint.h>

/**************************************************************************************************/
/**                      Global Types                                                            **/
/**************************************************************************************************/

void ws2812b_init      (void);
void ws2812b_set_pixel (uint8_t red, uint8_t green, uint8_t blue);
void ws2812b_send      (void);

#endif