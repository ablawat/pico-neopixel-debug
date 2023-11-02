#include <stdio.h>
#include <inttypes.h>
#include "pico/stdlib.h"
#include "debug-1.h"

int main()
{
    stdio_init_all();

    debug1_init();

    busy_wait_ms(UINT32_C(4));

    debug1_set_color(DEBUG_COLOR_RED);

    busy_wait_ms(UINT32_C(300));

    debug1_set_color(DEBUG_COLOR_GREEN);

    busy_wait_ms(UINT32_C(4000));

    debug1_set_color(DEBUG_COLOR_RED);

    busy_wait_ms(UINT32_C(2000));

    debug1_set_color(DEBUG_COLOR_BLUE);

    busy_wait_ms(UINT32_C(2000));

    debug1_turn_off();

    while (true)
    {
        busy_wait_ms(UINT32_C(4000));

        debug1_set_color(DEBUG_COLOR_RED);
        busy_wait_ms(UINT32_C(100));
        debug1_set_color(DEBUG_COLOR_GREEN);
        busy_wait_ms(UINT32_C(100));
        debug1_set_color(DEBUG_COLOR_BLUE);
        busy_wait_ms(UINT32_C(100));
        debug1_turn_off();
    }

    return 0;
}
