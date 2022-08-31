#include <stdarg.h>
#include <stdio.h>
#include "SEGGER_RTT.h"

int __wrap_getchar(void)
{
  return SEGGER_RTT_GetKey();
}

int __wrap_putchar(int c)
{
  return SEGGER_RTT_PutChar(0, c)?c:-1;
}

int __wrap_puts(const char *s)
{
  return SEGGER_RTT_WriteString(0, s);
}

int __printflike(1, 0) __wrap_printf(const char* format, ...)
{
  va_list va;
  va_start(va, format);
  int ret = vprintf(format, va);
  va_end(va);
  return ret;
}

int __wrap_vprintf(const char *format, va_list va)
{
  return SEGGER_RTT_vprintf(0, format, &va);
}
