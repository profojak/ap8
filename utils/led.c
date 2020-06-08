/********************************************************************
  LED utilities for MicroZed based MZ_APO board.
********************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdint.h>
#include <stdlib.h>

#include "led.h"

/* set LEDs on 32 LED line */
void led_line_set(unsigned char *led_mem_base, short leds)
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

/* color LEDs set */
void led_rgb_set(unsigned char *led_mem_base, char *rgb)
{
  char r, g, b;
  uint32_t c;
  r = rgb[0] > 57 ? rgb[0] - 88 : rgb[0] - 48;
  g = rgb[1] > 57 ? rgb[1] - 88 : rgb[1] - 48;
  b = rgb[2] > 57 ? rgb[2] - 88 : rgb[2] - 48;
  c = 0x0 + ((r * 16) << 16) + ((g * 16) << 8) + (b * 16);
  *(volatile uint32_t *)(led_mem_base + SPILED_REG_LED_RGB1_o) = c;
  *(volatile uint32_t *)(led_mem_base + SPILED_REG_LED_RGB2_o) = c;
}

/* servo LEDs set */
void led_servo_set(unsigned char *servo_mem_base, short num)
{
  uint32_t c = num > 0 ? 0x1 << (num - 1) : 0;
  *(volatile uint32_t *)(servo_mem_base + SERVOPS2_REG_CR_o) = c;
}

/* test 32 LED line */
void led_line_test(unsigned char *led_mem_base) {
  int i;
  uint32_t line = 1;
  for (i = 0; i < 44; i++) {
    line = (line << 1);
    if (i < 6) {
      line++;
    }  
    *(volatile uint32_t *)(led_mem_base + SPILED_REG_LED_LINE_o) = line;
    parlcd_delay(30);
  }
}

/* test color LEDs */
void led_rgb_test(unsigned char *led_mem_base)
{
  int i;
  char rgb[3] = "f00";
  for (i = 0; i < 15; i++) {
    led_rgb_set(led_mem_base, rgb);
    parlcd_delay(40);
    rgb[0]--;
  }
  parlcd_delay(300);

  rgb[0] = '0';
  rgb[1] = 'f';
  for (i = 0; i < 15; i++) {
    led_rgb_set(led_mem_base, rgb);
    parlcd_delay(40);
    rgb[1]--;
  }
  parlcd_delay(300);

  rgb[1] = '0';
  rgb[2] = 'f';
  for (i = 0; i < 15; i++) {
    led_rgb_set(led_mem_base, rgb);
    parlcd_delay(40);
    rgb[2]--;
  }
}

/* test servo LEDs */
void led_servo_test(unsigned char *servo_mem_base)
{
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 4; j > 0; j--) {
      led_servo_set(servo_mem_base, j);
      parlcd_delay(150);
    }
  }
  led_servo_set(servo_mem_base, 0);
}
