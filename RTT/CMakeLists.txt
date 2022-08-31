add_library(rtt INTERFACE)

target_sources(rtt INTERFACE SEGGER_RTT.c SEGGER_RTT_printf.c stdio.c SEGGER_RTT.h)

target_include_directories(rtt INTERFACE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/../Config)

target_link_options(rtt INTERFACE "LINKER:--wrap=printf")
target_link_options(rtt INTERFACE "LINKER:--wrap=vprintf")
target_link_options(rtt INTERFACE "LINKER:--wrap=puts")
target_link_options(rtt INTERFACE "LINKER:--wrap=putchar")
target_link_options(rtt INTERFACE "LINKER:--wrap=getchar")
