#ifndef CURSOR_H
#define CURSOR_H
#include "termlib_types.h"

void cursor_init(termlib_context *ctx, int posX, int posY, char rep);

void display_cursor(termlib_screen *screen, termlib_cursor *cursor);
#endif