#include "termlib_types.h"

void cursor_init(termlib_context *ctx, int posX, int posY, char rep) {
    ctx->cursor.posX = posX;
    ctx->cursor.posY = posY;
    ctx->cursor.rep = rep;
}

void display_cursor(termlib_screen *screen, termlib_cursor *cursor) {
    set_pixel(screen, cursor->posX, cursor->posY, cursor->rep, FG_DEFAULT, BG_DEFAULT);
}