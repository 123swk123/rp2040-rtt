/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <errno.h>
// #include "pico/stdlib.h"
#include "SEGGER_RTT.h"

#include "pico/divider.h"

#include "hardware/vreg.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"

void measure_freqs(void) {
  uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
  uint f_pll_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
  uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
  uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
  uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
  uint f_clk_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
  uint f_clk_adc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);
  uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);

  puts(RTT_CTRL_TEXT_BRIGHT_GREEN"Measured Clock speed:\n");
  printf("pll_sys  = %d kHz\n", f_pll_sys);
  printf("pll_usb  = %d kHz\n", f_pll_usb);
  printf("rosc     = %d kHz\n", f_rosc);
  printf("clk_sys  = %d kHz\n", f_clk_sys);
  printf("clk_peri = %d kHz\n", f_clk_peri);
  printf("clk_usb  = %d kHz\n", f_clk_usb);
  printf("clk_adc  = %d kHz\n", f_clk_adc);
  printf("clk_rtc  = %d kHz\n", f_clk_rtc);

  // Can't measure clk_ref / xosc as it is the ref
}

void reported_freqs(void)
{
  puts(RTT_CTRL_TEXT_YELLOW"Reported Clock speed\n");
  printf("clk_sys  = %d MHz\n", clock_get_hz(clk_sys) / MHZ);
  printf("clk_peri = %d MHz\n", clock_get_hz(clk_peri) / MHZ);
  printf("clk_usb  = %d MHz\n", clock_get_hz(clk_usb) / MHZ);
  printf("clk_ref  = %d MHz\n", clock_get_hz(clk_ref) / MHZ);
}

static inline void set_clock_sys_250MHz(void)
{
  // raise core voltage to support overclock
  vreg_set_voltage(VREG_VOLTAGE_MAX);

  // temporary set SYS clock to PLL USB
  clock_configure(clk_sys,
                  CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                  CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
                  48 * MHZ,
                  48 * MHZ);

  // pll sys now set to produce 250MHz <== 15000 / (6*1)
  pll_init(pll_sys, 1, 1500 * MHZ, 6, 1);

  // set system clock back to PLL SYS
  clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
                    250 * MHZ,
                    250 * MHZ);
  // peripheral clock is set to clk_sys in runtime_init->clocks_init
  clock_set_reported_hz(clk_peri, 250*MHZ);
}

int main()
{
  set_clock_sys_250MHz();

  SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);

  puts(RTT_CTRL_TEXT_BRIGHT_RED"SEGGER Real-Time-Terminal\r\n");

  measure_freqs();
  reported_freqs();

  return 0;
}
