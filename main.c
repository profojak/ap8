/********************************************************************
  ap8 - actually perfect 8bit music sequencer

  This is semestral work for B0B35APO course: Computer Architecture
  at FEE CTU.

  Made by Jakub Profota and Jenda Kucera in 2020 summer semester.
********************************************************************/

#define _POSIX_C_SOURCE 200112L
#define WIDTH 480
#define HEIGHT 320

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "mzapo/mzapo_phys.h"
#include "mzapo/mzapo_regs.h"
#include "utils/lcd.h"
#include "utils/led.h"

/* main */
int main(int argc, char *argv[])
{
  unsigned char *led_mem_base = (unsigned char *)map_phys_address(
    SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  unsigned char *lcd_mem_base = (unsigned char *)map_phys_address(
    PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
  unsigned char *serv_mem_base = (unsigned char *)map_phys_address(
    SERVOPS2_REG_BASE_PHYS, SERVOPS2_REG_SIZE, 0);
  unsigned char *audio_mem_base = (unsigned char *)map_phys_address(
    AUDIOPWM_REG_BASE_PHYS, AUDIOPWM_REG_SIZE, 0);

  // reset device
  lcd_init(lcd_mem_base);
  led_line_set(0x0,led_mem_base);

  // frame buffer
  fbuffer_t *fb = (fbuffer_t *)malloc(sizeof(fbuffer_t));
  fb->w = WIDTH;
  fb->h = HEIGHT;
  fb->r = (unsigned char *)malloc(WIDTH * HEIGHT);
  fb->g = (unsigned char *)malloc(WIDTH * HEIGHT);
  fb->b = (unsigned char *)malloc(WIDTH * HEIGHT);

  *(volatile uint32_t *)(serv_mem_base + SERVOPS2_REG_CR_o) = 0x0;
  *(volatile uint32_t *)(led_mem_base + SPILED_REG_LED_RGB1_o) = 0x000000;
  *(volatile uint32_t *)(led_mem_base + SPILED_REG_LED_RGB2_o) = 0x000000;

  return 0;
}
