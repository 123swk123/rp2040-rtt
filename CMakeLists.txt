cmake_minimum_required(VERSION 3.24)

# Pull in SDK (must be before project)
# include(pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(pico_rtt C CXX ASM)
set(exe_name test_rtt)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_COMPILER_LAUNCHER "ccache.exe")
set(CMAKE_CXX_COMPILER_LAUNCHER "ccache.exe")

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

add_executable(${exe_name}
                hello.c)

add_subdirectory(RTT)
# target_link_libraries(${exe_name} pico_stdlib rtt hardware_clocks)
target_link_libraries(${exe_name} pico_standard_link pico_runtime rtt pico_divider hardware_clocks)
                                    # pico_runtime)
                                    # pico_platform
                                    # pico_time)

target_compile_definitions(${exe_name} PRIVATE PICO_FLOAT_SUPPORT_ROM_V1=0 PICO_DOUBLE_SUPPORT_ROM_V1=0)
# target_compile_options(${exe_name} PUBLIC -flto)
target_link_options(${exe_name} PRIVATE "LINKER:--print-memory-usage")
    
# disable usb output, disable uart output
pico_enable_stdio_usb(${exe_name} 0)
pico_enable_stdio_uart(${exe_name} 0)
pico_enable_stdio_semihosting(${exe_name} 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${exe_name})
