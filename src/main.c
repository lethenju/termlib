#include <stdio.h>
#include <stdlib.h>
#include "termlib/screen.h"
#include "termlib/termlib.h"

void* init(termlib_context* ctx){
    // Implement your high level code from here
    
    // Edges of the screen
    fill_rectangle(ctx->screen, 0, 0, ctx->screen->width,1,'-');
    fill_rectangle(ctx->screen, 0, 0, 1, ctx->screen->height,'|');
    fill_rectangle(ctx->screen, ctx->screen->width - 1, 0, 1, ctx->screen->height,'|');
    fill_rectangle(ctx->screen, 0, ctx->screen->height - 1 ,ctx->screen->width, 1,'-');
    

    cursor_init(ctx, 5, 5, '*');
    draw_line(ctx->screen, (int)ctx->screen->width/2,(int)ctx->screen->height/2,ctx->cursor.posX, ctx->cursor.posY,'o');
    display_cursor(ctx->screen, &ctx->cursor);   
}

void* event_loop(termlib_context* ctx) {
    char c;
    while((c=getchar())!= '.') {
        switch (c){
            case 'z':
                    if (ctx->cursor.posY > 1)
                        ctx->cursor.posY--;
                    break;
            case 'q':
                    if (ctx->cursor.posX > 1)
                        ctx->cursor.posX--;
                    break;
            case 's':
                    if (ctx->cursor.posY < ctx->screen->height - 2)
                        ctx->cursor.posY++;
                    break;
            case 'd':
                    if (ctx->cursor.posX < ctx->screen->width - 2)
                        ctx->cursor.posX++;
                    break;
        }       
        fill_rectangle(ctx->screen, 1,1, ctx->screen->width-2, ctx->screen->height-2,' ');
        draw_line(ctx->screen, (int)ctx->screen->width/2,(int)ctx->screen->height/2,ctx->cursor.posX, ctx->cursor.posY,'o');
        display_cursor(ctx->screen, &ctx->cursor);     
    }
}


// DO NOT TOUCH MAIN
int main(int argc, char *argv[])
{
    termlib_context* ctx = termlib_init((void*)init);
    termlib_event_loop(ctx, (void*)event_loop);
    return 0;
}
