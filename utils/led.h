/********************************************************************
  LED utilities for MicroZed based MZ_APO board.
********************************************************************/

#ifndef LED_H
#define LED_H

#include <stdint.h>

void led_line_set(short leds, unsigned char *led_mem_base);

#endif /*LED_H*/
