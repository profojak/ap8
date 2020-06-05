/********************************************************************
  LCD utilities for MicroZed based MZ_APO board.
********************************************************************/

#define _POSIX_C_SOURCE 200112L
#define WIDTH 480
#define HEIGHT 320
#define BLKW 60
#define BLKH 32

#include <stdint.h>
#include <stdlib.h>
#include <string.h> 
#include "lcd.h"
#include "../mzapo/mzapo_parlcd.h"
#include "../fonts/font_types.h"

/* LCD init */
void lcd_init(unsigned char *lcd_mem_base)
{
  parlcd_hx8357_init(lcd_mem_base);
  parlcd_write_cmd(lcd_mem_base, 0x2c);
  int i, j;
  uint16_t c;
  for (i = 0; i < WIDTH; i++) {
    for (j = 0; j < HEIGHT; j++) {
      c = 0;
      parlcd_write_data(lcd_mem_base, c);
    }
  }
}

/* draw frame buffer to LCD */
void lcd_draw(unsigned char *lcd_mem_base, fbuffer_t *fb)
{
  int i, j, ptr = 0;
  char r, g, b;
  uint16_t c;
  parlcd_write_cmd(lcd_mem_base, 0x2c);
  for (i = 0; i < WIDTH; i++) {
    for (j = 0; j < HEIGHT; j++) {
      r = fb->r[ptr] > 57 ? fb->r[ptr] - 88 : fb->r[ptr] - 48;
      g = fb->g[ptr] > 57 ? fb->g[ptr] - 88 : fb->g[ptr] - 48;
      b = fb->b[ptr] > 57 ? fb->b[ptr] - 88 : fb->b[ptr] - 48;
      c = 0x0 + ((r * 2) << 11) + ((g * 2) << 5) + b * 2;
      ptr++;
      parlcd_write_data(lcd_mem_base, c);
    }
  }
}

/* frame buffer init */
fbuffer_t *fb_init(int size)
{
  fbuffer_t *fb = (fbuffer_t *)malloc(sizeof(fbuffer_t));
  fb->r = (char *)malloc(size);
  fb->g = (char *)malloc(size);
  fb->b = (char *)malloc(size);
  memset(fb->r, '0', size);
  memset(fb->g, '0', size);
  memset(fb->b, '0', size);
  return fb;
}

/* block to frame buffer */
void fb_block(int x, int y, char *rgb, fbuffer_t *fb)
{
  x = x * (BLKW + 20) + 10; // index of music line 0-5
  y = y * BLKH;             // index of block y
  int offset = x + y * WIDTH;

  int i, j;
  for (i = 0; i < BLKW; i++) {
    for (j = 0; j < BLKH; j++) {
      fb->r[offset + i + j * WIDTH] = rgb[0];
      fb->g[offset + i + j * WIDTH] = rgb[1];
      fb->b[offset + i + j * WIDTH] = rgb[2];
    }
  }
}

/* text to frame buffer */
void fb_text(int x, int y, char *text, int num, char *rgb,
  fbuffer_t *fb)
{
  int i, j, k, l, m, w, ptr, offset = x + y *  WIDTH;
  unsigned short mask = 1 << 15; // mask to get bit
  for (i = 0; i < font_winFreeSystem14x16.height; i++) {
    ptr = i * 480 * 4;
    for (j = 0; j < num; j++) {
      w = font_winFreeSystem14x16.width[text[j] - 0x20];
      for (k = 0; k < w; k++) {
        // check for 1 bit with mask
        if (font_winFreeSystem14x16.bits[16 * (text[j] - 0x20) + i] &
          (short)(mask >> k)) {
          if (offset + ptr + 4 * WIDTH + 4 < WIDTH * HEIGHT) {
            for (l = 0; l < 4; l++) {
              for (m = 0; m < 4; m++) {
                fb->r[offset + ptr + l + m * WIDTH] = rgb[0];
                fb->g[offset + ptr + l + m * WIDTH] = rgb[1];
                fb->b[offset + ptr + l + m * WIDTH] = rgb[2];
              }
            }
          }
        }
        ptr += 4;
      }
    }
  }
}

/* LCD color test */
void lcd_test(unsigned char *lcd_mem_base, fbuffer_t *fb)
{
  // green
  fb_block(0, 0, "f00", fb);
  fb_block(0, 1, "e00", fb);
  fb_block(0, 2, "d00", fb);
  fb_block(0, 3, "b00", fb);
  fb_block(0, 4, "900", fb);
  fb_block(0, 5, "700", fb);
  fb_block(0, 6, "500", fb);
  fb_block(0, 7, "300", fb);
  fb_block(0, 8, "100", fb);
  lcd_draw(lcd_mem_base, fb);
  parlcd_delay(100);

  // yellow
  fb_block(1, 0, "ff0", fb);
  fb_block(1, 1, "ee0", fb);
  fb_block(1, 2, "dd0", fb);
  fb_block(1, 3, "bb0", fb);
  fb_block(1, 4, "990", fb);
  fb_block(1, 5, "770", fb);
  fb_block(1, 6, "550", fb);
  fb_block(1, 7, "330", fb);
  fb_block(1, 8, "110", fb);
  lcd_draw(lcd_mem_base, fb);
  parlcd_delay(100);

  // green
  fb_block(2, 0, "0f0", fb);
  fb_block(2, 1, "0e0", fb);
  fb_block(2, 2, "0d0", fb);
  fb_block(2, 3, "0b0", fb);
  fb_block(2, 4, "090", fb);
  fb_block(2, 5, "070", fb);
  fb_block(2, 6, "050", fb);
  fb_block(2, 7, "030", fb);
  fb_block(2, 8, "010", fb);
  lcd_draw(lcd_mem_base, fb);
  parlcd_delay(100);

  // cyan
  fb_block(3, 0, "0ff", fb);
  fb_block(3, 1, "0ee", fb);
  fb_block(3, 2, "0dd", fb);
  fb_block(3, 3, "0bb", fb);
  fb_block(3, 4, "099", fb);
  fb_block(3, 5, "077", fb);
  fb_block(3, 6, "055", fb);
  fb_block(3, 7, "033", fb);
  fb_block(3, 8, "011", fb);
  lcd_draw(lcd_mem_base, fb);
  parlcd_delay(100);

  // blue
  fb_block(4, 0, "00f", fb);
  fb_block(4, 1, "00e", fb);
  fb_block(4, 2, "00d", fb);
  fb_block(4, 3, "00b", fb);
  fb_block(4, 4, "009", fb);
  fb_block(4, 5, "007", fb);
  fb_block(4, 6, "005", fb);
  fb_block(4, 7, "003", fb);
  fb_block(4, 8, "001", fb);
  lcd_draw(lcd_mem_base, fb);
  parlcd_delay(100);

  // magenta
  fb_block(5, 0, "f0f", fb);
  fb_block(5, 1, "e0e", fb);
  fb_block(5, 2, "d0d", fb);
  fb_block(5, 3, "b0b", fb);
  fb_block(5, 4, "909", fb);
  fb_block(5, 5, "707", fb);
  fb_block(5, 6, "505", fb);
  fb_block(5, 7, "303", fb);
  fb_block(5, 8, "101", fb);
  lcd_draw(lcd_mem_base, fb);
}
