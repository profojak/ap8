/********************************************************************
  LCD utilities for MicroZed based MZ_APO board.
********************************************************************/

#define _POSIX_C_SOURCE 200112L
#define BLKW 60
#define BLKH 32

#include "lcd.h"

/* frame buffer init */
fbuffer_t *fb_init(int w, int h)
{
  fbuffer_t *fb = (fbuffer_t *)malloc(sizeof(fbuffer_t));
  fb->w = w;
  fb->h = h;
  fb->r = (char *)malloc(w * h);
  fb->g = (char *)malloc(w * h);
  fb->b = (char *)malloc(w * h);
  memset(fb->r, '0', w * h);
  memset(fb->g, '0', w * h);
  memset(fb->b, '0', w * h);
  return fb;
}

/* fill area of frame buffer */
void fb_fill(int x, int y, int w, int h, char *rgb, fbuffer_t *fb)
{
  int i, j;
  for (i = x; i < x + w; i++) {
    for (j = y; j < y + h; j++) {
      fb->r[i + j * fb->w] = rgb[0];
      fb->g[i + j * fb->w] = rgb[1];
      fb->b[i + j * fb->w] = rgb[2];
    }
  }
}

/* outline area of frame buffer */
void fb_outl(int x, int y, int w, int h, char *rgb, fbuffer_t *fb)
{
  int i, j;
  for (i = x; i < x + w; i++) {
    for (j = y; j < y + h; j++) {
      if ((i < x + 5) || (i > x + w - 5) ||
        (j < y + 5) || (j > y + h - 5)) {
        fb->r[i + j * fb->w] = rgb[0];
        fb->g[i + j * fb->w] = rgb[1];
        fb->b[i + j * fb->w] = rgb[2];
      }
    }
  }
}

/* block to frame buffer */
void fb_block(int x, int y, bool outline, char *rgb, fbuffer_t *fb)
{
  x = x * (BLKW + 20) + 10; // index of music line 0-5
  y = y * BLKH;             // index of block y
  if (outline) {
    fb_outl(x, y, BLKW, BLKH, rgb, fb);
  } else {
    fb_fill(x, y, BLKW, BLKH, rgb, fb);
  }
}

/* 2 option menu to frame buffer */
void fb_menu2(menu_t *menu, char *rgb, fbuffer_t *fb)
{
  fb_fill(30, 20, fb->w - 60, fb->h - 40, "000", fb);
  fb_outl(50, 40, fb->w - 100, fb->h - 80, rgb, fb);
  if (menu->active == 0) {
    fb_fill(52, 42, fb->w - 104, (fb->h - 84) / 2, rgb, fb);
    fb_text14x16(fb->w / 2, 60, menu->options[0], 6, "000", fb);
    fb_text14x16(fb->w / 2, 176, menu->options[1], 6, rgb, fb);
  } else {
    fb_fill(52, 42 + (fb->h - 84) / 2, fb->w - 104, (fb->h - 84) / 2,
      rgb, fb);
    fb_text14x16(fb->w / 2, 60, menu->options[0], 6, rgb, fb);
    fb_text14x16(fb->w / 2, 176, menu->options[1], 6, "000", fb);
  }
}

/* text to frame buffer */
void fb_text14x16(int x, int y, char *text, int size, char *rgb,
  fbuffer_t *fb)
{
  int ln, ch, i, j, w, width, n = strlen(text), ptr, offset = 0;
  for (ch = 0; ch < n; ch++) {
    offset += font_winFreeSystem14x16.width[text[ch] - 0x20];
  }
  offset = -(offset * 0.5) * size + x + y * fb->w;
  unsigned short mask = 1 << 15; // mask to get bit
  for (ln = 0; ln < font_winFreeSystem14x16.height; ln++) {
    ptr = ln * fb->w * size;
    for (ch = 0; ch < n; ch++) {
      width = font_winFreeSystem14x16.width[text[ch] - 0x20];
      for (w = 0; w < width; w++) {
        // check for 1 bit with mask
        if (font_winFreeSystem14x16.bits[16 * (text[ch] - 0x20) + ln]
          & (short)(mask >> w)) {
          if (offset + ptr + size * fb->w + size < fb->w * fb->h) {
            for (i = 0; i < size; i++) {
              for (j = 0; j < size; j++) {
                fb->r[offset + ptr + i + j * fb->w] = rgb[0];
                fb->g[offset + ptr + i + j * fb->w] = rgb[1];
                fb->b[offset + ptr + i + j * fb->w] = rgb[2];
              }
            }
          }
        }
        ptr += size;
      }
    }
  }
}

/* LCD init */
void lcd_init(unsigned char *lcd_mem_base, unsigned int size)
{
  parlcd_hx8357_init(lcd_mem_base);
  parlcd_write_cmd(lcd_mem_base, 0x2c);
  int i;
  uint16_t c;
  for (i = 0; i < size; i++) {
    c = 0;
    parlcd_write_data(lcd_mem_base, c);
  }
}

/* draw frame buffer to LCD */
void lcd_draw(unsigned char *lcd_mem_base, fbuffer_t *fb)
{
  int i, j, ptr = 0;
  char r, g, b;
  uint16_t c;
  parlcd_write_cmd(lcd_mem_base, 0x2c);
  for (i = 0; i < fb->w; i++) {
    for (j = 0; j < fb->h; j++) {
      r = fb->r[ptr] > 57 ? fb->r[ptr] - 88 : fb->r[ptr] - 48;
      g = fb->g[ptr] > 57 ? fb->g[ptr] - 88 : fb->g[ptr] - 48;
      b = fb->b[ptr] > 57 ? fb->b[ptr] - 88 : fb->b[ptr] - 48;
      c = 0x0 + ((r * 2) << 11) + ((g * 2) << 5) + b * 2;
      ptr++;
      parlcd_write_data(lcd_mem_base, c);
    }
  }
}

/* LCD color test */
void lcd_test(unsigned char *lcd_mem_base, fbuffer_t *fb)
{
  // green
  fb_block(0, 0, 0, "f00", fb);
  fb_block(0, 1, 0, "e00", fb);
  fb_block(0, 2, 0, "d00", fb);
  fb_block(0, 3, 0, "b00", fb);
  fb_block(0, 4, 0, "900", fb);
  fb_block(0, 5, 0, "700", fb);
  fb_block(0, 6, 0, "500", fb);
  fb_block(0, 7, 0, "300", fb);
  fb_block(0, 8, 0, "100", fb);
  lcd_draw(lcd_mem_base, fb);
  parlcd_delay(100);

  // yellow
  fb_block(1, 0, 0, "ff0", fb);
  fb_block(1, 1, 0, "ee0", fb);
  fb_block(1, 2, 0, "dd0", fb);
  fb_block(1, 3, 0, "bb0", fb);
  fb_block(1, 4, 0, "990", fb);
  fb_block(1, 5, 0, "770", fb);
  fb_block(1, 6, 0, "550", fb);
  fb_block(1, 7, 0, "330", fb);
  fb_block(1, 8, 0, "110", fb);
  lcd_draw(lcd_mem_base, fb);
  parlcd_delay(100);

  // green
  fb_block(2, 0, 0, "0f0", fb);
  fb_block(2, 1, 0, "0e0", fb);
  fb_block(2, 2, 0, "0d0", fb);
  fb_block(2, 3, 0, "0b0", fb);
  fb_block(2, 4, 0, "090", fb);
  fb_block(2, 5, 0, "070", fb);
  fb_block(2, 6, 0, "050", fb);
  fb_block(2, 7, 0, "030", fb);
  fb_block(2, 8, 0, "010", fb);
  lcd_draw(lcd_mem_base, fb);
  parlcd_delay(100);

  // cyan
  fb_block(3, 0, 0, "0ff", fb);
  fb_block(3, 1, 0, "0ee", fb);
  fb_block(3, 2, 0, "0dd", fb);
  fb_block(3, 3, 0, "0bb", fb);
  fb_block(3, 4, 0, "099", fb);
  fb_block(3, 5, 0, "077", fb);
  fb_block(3, 6, 0, "055", fb);
  fb_block(3, 7, 0, "033", fb);
  fb_block(3, 8, 0, "011", fb);
  lcd_draw(lcd_mem_base, fb);
  parlcd_delay(100);

  // blue
  fb_block(4, 0, 0, "00f", fb);
  fb_block(4, 1, 0, "00e", fb);
  fb_block(4, 2, 0, "00d", fb);
  fb_block(4, 3, 0, "00b", fb);
  fb_block(4, 4, 0, "009", fb);
  fb_block(4, 5, 0, "007", fb);
  fb_block(4, 6, 0, "005", fb);
  fb_block(4, 7, 0, "003", fb);
  fb_block(4, 8, 0, "001", fb);
  lcd_draw(lcd_mem_base, fb);
  parlcd_delay(100);

  // magenta
  fb_block(5, 0, 0, "f0f", fb);
  fb_block(5, 1, 0, "e0e", fb);
  fb_block(5, 2, 0, "d0d", fb);
  fb_block(5, 3, 0, "b0b", fb);
  fb_block(5, 4, 0, "909", fb);
  fb_block(5, 5, 0, "707", fb);
  fb_block(5, 6, 0, "505", fb);
  fb_block(5, 7, 0, "303", fb);
  fb_block(5, 8, 0, "101", fb);
  lcd_draw(lcd_mem_base, fb);
}
