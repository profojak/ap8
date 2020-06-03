/********************************************************************
  LED utilities for MicroZed based MZ_APO board.
********************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdint.h>

#include "led.h"
#include "../mzapo/mzapo_regs.h"

/* set LEDs on 32 LED line */
void led_line_set(short leds, unsigned char *led_mem_base)
{
  int i;
  unsigned short mask = 1;
  uint32_t line = 0, set = 0x0000003e;
  for (i = 0; i < 6; i++) {
    if (leds & (mask << i)) {
      line += set << (5 * i);
    }
  }
  *(volatile uint32_t *)(led_mem_base + SPILED_REG_LED_LINE_o) = line;
}
