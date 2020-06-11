/********************************************************************
  Menu interface for MicroZed based MZ_APO board.
********************************************************************/

#ifndef MENU_H
#define MENU_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
  char **options;
  char *rgb;
  short size;
  short active;
} menu_t;

menu_t *menu2_init(char *text1, char *text2, char *rgb);

menu_t *menu1_init(char *text, char *rgb);

#endif /*MENU_H*/
