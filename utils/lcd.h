/********************************************************************
  LCD utilities for MicroZed based MZ_APO board.
********************************************************************/

#ifndef LCD_H
#define LCD_H

#include <stdint.h>

/* frame buffer structure for better color manipulation */
typedef struct {
  unsigned int w, h;        /* width and height */
  unsigned char *r, *g, *b; /* arrays for red, green and blue */
} fbuffer_t;

void lcd_init(unsigned char *lcd_mem_base);

void lcd_draw(unsigned char *lcd_mem_base, fbuffer_t *fb);

void fb_block(int x, int y, char *rgb, fbuffer_t *fb);

void fb_bar(char *name, int num, fbuffer_t *fb);

void fb_text(int x, int y, char *text, int num, char *rgb,
  fbuffer_t *fb);

#endif /*LCD_H*/
