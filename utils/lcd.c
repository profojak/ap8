/********************************************************************
  LCD utilities for MicroZed based MZ_APO board.
********************************************************************/

#define _POSIX_C_SOURCE 200112L
#define WIDTH 480
#define HEIGHT 320
#define BLKW 60
#define BLKH 32

#include <stdint.h>

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
  uint16_t c;
  parlcd_write_cmd(lcd_mem_base, 0x2c);
  for (i = 0; i < WIDTH; i++) {
    for (j = 0; j < HEIGHT; j++) {
      c = 0x0;
      // RED                        c = 0000 0|000 000|0 0000
      // example: fb->r = 0x8  +fb->r = 1000
      //                                    +1
      if (fb->r[ptr] != 0) {
        c += (((uint16_t)(fb->r[ptr]) << 1) + 0x1) << 11;
      }
      // GREEN                      c = 0000 0|000 000|0 0000
      // example: fb->g = 0xf         +fb->g = 111 1
      //                                           +11
      if (fb->g[ptr] != 0) {
        c += (((uint16_t)(fb->g[ptr]) << 2) + 0x3) << 5;
      }
      // BLUE                       c = 0000 0|000 000|0 0000
      // example: fb->b = 0x0                 +fb->b = 0 000
      //                                                   +0
      if (fb->b[ptr] != 0) {
        c += (((uint16_t)(fb->b[ptr]) << 1) + 0x1);
      }
      ptr++;
      parlcd_write_data(lcd_mem_base, c);
    }
  }
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

/* bar with song name to frame buffer */
void fb_bar(char *name, int num, fbuffer_t *fb)
{
  int i, j;
  for (i = 0; i < WIDTH; i++) {
    for (j = HEIGHT - BLKH * 2 - 10; j < HEIGHT; j++) {
      fb->r[i + j * WIDTH] = 0x0;
      fb->g[i + j * WIDTH] = 0x0;
      fb->b[i + j * WIDTH] = 0x0;
    }
  }
  fb_text(5, HEIGHT - BLKH * 2 + 10, name, num, "444", fb);
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
