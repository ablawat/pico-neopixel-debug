cmake_minimum_required(VERSION 3.13)

include(../pico-sdk/external/pico_sdk_import.cmake)

project(myproject C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# target sources
add_executable(neopixel main.c debug-1.c ws2812b.c)

# target dependencies
target_link_libraries(neopixel pico_stdlib hardware_gpio hardware_pwm hardware_dma)

# create bin/dis/hex/uf2 files
pico_add_extra_outputs(neopixel)
