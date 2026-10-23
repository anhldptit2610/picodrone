# Add executable. Default name is the project name, version 0.1

add_executable(picodrone 
    picodrone.cpp
    imu.cpp)

pico_set_program_name(picodrone "picodrone")
pico_set_program_version(picodrone "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(picodrone 1)

# Add the standard library to the build
target_link_libraries(picodrone
        pico_stdlib
        FreeRTOS-Kernel-Heap4
        pico_async_context_freertos
        pico_cyw43_arch_none
        hardware_i2c)

# Add the standard include files to the build
target_include_directories(picodrone PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(picodrone)