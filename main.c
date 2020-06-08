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
#include <unistd.h>

#include "mzapo/mzapo_phys.h"
#include "mzapo/mzapo_parlcd.h"
#include "mzapo/mzapo_regs.h"
#include "utils/lcd.h"
#include "utils/led.h"
#include "gui/menu.h"

/* main */
int main(int argc, char *argv[])
{
  unsigned char *led_mem_base = (unsigned char *)map_phys_address(
    SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  unsigned char *lcd_mem_base = (unsigned char *)map_phys_address(
    PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
  unsigned char *servo_mem_base = (unsigned char *)map_phys_address(
    SERVOPS2_REG_BASE_PHYS, SERVOPS2_REG_SIZE, 0);
  //unsigned char *audio_mem_base = (unsigned char *)map_phys_address(
    //AUDIOPWM_REG_BASE_PHYS, AUDIOPWM_REG_SIZE, 0);

  // init device
  lcd_init(lcd_mem_base, WIDTH * HEIGHT);
  led_line_set(led_mem_base, 0x0);
  led_rgb_set(led_mem_base, "000");
  led_servo_set(servo_mem_base, 0x0);
  fbuffer_t *fb = fb_init(WIDTH, HEIGHT);

  /*
  lcd_test(lcd_mem_base, fb);
  led_line_test(led_mem_base);
  led_rgb_test(led_mem_base);
  led_servo_test(servo_mem_base);
  */

  /*
  menu_t *menu = menu2_init("Continue", "Exit");
  fb_menu2(menu, "808", fb);
  lcd_draw(lcd_mem_base, fb);
  */

  free(fb->r);
  free(fb->g);
  free(fb->b);
  free(fb);

  return 0;
}
