#include <stdio.h>
#include <inttypes.h>
#include "pico/stdlib.h"
#include "debug-1.h"

int main()
{
    stdio_init_all();

    debug1_pixel_init();

    busy_wait_ms(UINT32_C(4));

    debug1_pixel_color_set(DEBUG_COLOR_RED);

    busy_wait_ms(UINT32_C(300));

    debug1_pixel_color_set(DEBUG_COLOR_GREEN);

    busy_wait_ms(UINT32_C(4000));

    debug1_pixel_color_set(DEBUG_COLOR_RED);

    busy_wait_ms(UINT32_C(2000));

    debug1_pixel_color_set(DEBUG_COLOR_CYAN);

    busy_wait_ms(UINT32_C(2000));

    debug1_pixel_color_set(DEBUG_COLOR_BLUE);

    busy_wait_ms(UINT32_C(2000));

    debug1_pixel_color_set(DEBUG_COLOR_MAGENTA);

    busy_wait_ms(UINT32_C(2000));

    debug1_pixel_color_set(DEBUG_COLOR_GREEN);

    busy_wait_ms(UINT32_C(2000));

    debug1_pixel_color_set(DEBUG_COLOR_YELLOW);

    busy_wait_ms(UINT32_C(2000));

    debug1_pixel_turn_off();

    while (true)
    {
        busy_wait_ms(UINT32_C(4000));

        debug1_pixel_color_set(DEBUG_COLOR_RED);
        debug1_pixel_color_set(DEBUG_COLOR_GREEN);
        debug1_pixel_color_set(DEBUG_COLOR_BLUE);

        busy_wait_ms(UINT32_C(200));
        debug1_pixel_color_set(DEBUG_COLOR_RED);

        busy_wait_ms(UINT32_C(200));
        debug1_pixel_color_set(DEBUG_COLOR_GREEN);

        busy_wait_ms(UINT32_C(200));
        debug1_pixel_color_set(DEBUG_COLOR_BLUE);

        busy_wait_ms(UINT32_C(200));
        debug1_pixel_turn_off();
    }

    return 0;
}
