#ifndef SCREEN_H
#define SCREEN_H
#include "termlib_types.h"

void screen_init(termlib_context * ctx, int width, int height);

void *screen_display_thread(void * ctx);

void set_pixel(termlib_screen * ctx, int posX, int posY, char c);

char get_pixel(termlib_screen * ctx, int posX, int posY);
#endif