#ifndef SCREEN_H
#define SCREEN_H
#include "termlib_types.h"

void screen_init(termlib_context * ctx, int width, int height);

void *screen_display_thread(void * ctx);

void set_pixel(termlib_screen * ctx, int posX, int posY, char c);

char get_pixel(termlib_screen * ctx, int posX, int posY);


void draw_rectangle(termlib_screen *ctx, int posX, int posY, int width, int height, char rep);
void fill_rectangle(termlib_screen *ctx, int posX, int posY, int width, int height, char rep);
void draw_line(termlib_screen *ctx, int posX, int posY, int posX2, int posY2, char rep);
void draw_circle(termlib_screen *ctx, int posX, int posY, int radius, char rep);
void fill_circle(termlib_screen *ctx, int posX, int posY, int radius, char rep);
#endif