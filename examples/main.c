#include <stdio.h>
#include <stdlib.h>
#include "screen.h"
#include "termlib.h"
#include "cursor.h"
#include "log_system.h"
#include "resman.h"

void* init(termlib_context* ctx){
    INFO_TRACE("Init system");
    // Edges of the screen
    fill_rectangle(ctx->screen, 0, 0, ctx->screen->width -1 , ctx->screen->height - 1,'-', FG_DEFAULT, BG_DEFAULT);
    DEBUG_TRACE("Rectangle filled");

    cursor_init(ctx, 5, 5, '*');
    
    draw_line(ctx->screen, (int)ctx->screen->width/2,(int)ctx->screen->height/2,ctx->cursor.posX, ctx->cursor.posY,'o', FG_DEFAULT, BG_DEFAULT);
    display_cursor(ctx->screen, &ctx->cursor);   
    DEBUG_TRACE("Cursor init");
    INFO_TRACE("End init");

}

void* event_loop(termlib_context* ctx) {
    INFO_TRACE("Event loop begin");
    char c;
    while((c=getchar())!= '.') {
        DEBUG_TRACE("char got : %c", c);
        switch (c){
            case 'z':
                    DEBUG_TRACE("ctx->cursor.posY--");
                    if (ctx->cursor.posY > 1)
                        ctx->cursor.posY--;
                    break;
            case 'q':
                    DEBUG_TRACE("ctx->cursor.posX++");
                    if (ctx->cursor.posX > 1)
                        ctx->cursor.posX--;
                    break;
            case 's':
                    DEBUG_TRACE("ctx->cursor.posY++");
                    if (ctx->cursor.posY < ctx->screen->height - 2)
                        ctx->cursor.posY++;
                    break;
            case 'd':
                    DEBUG_TRACE("ctx->cursor.posX++");
                    if (ctx->cursor.posX < ctx->screen->width - 2)
                        ctx->cursor.posX++;
                    break;
        }       
        fill_rectangle(ctx->screen, 1,1, ctx->screen->width-2, ctx->screen->height-2,' ', FG_DEFAULT, BG_DEFAULT);
        draw_line(ctx->screen, (int)ctx->screen->width/2,(int)ctx->screen->height/2,ctx->cursor.posX, ctx->cursor.posY,'o', FG_DEFAULT, BG_DEFAULT);
        display_cursor(ctx->screen, &ctx->cursor);     
        screen_frame_ready(ctx->screen);
        DEBUG_TRACE("Called screen frame ready");

    }
}


// DO NOT TOUCH MAIN
int main(int argc, char *argv[])
{
    resman_init();
    log_init();
    termlib_context* ctx = termlib_init((void*)init);
    termlib_event_loop(ctx, (void*)event_loop);
    log_end();
    resman_end();
    return 0;
}
