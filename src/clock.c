#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "termlib/screen.h"
#include "termlib/termlib.h"

void* init(termlib_context* ctx){
    // Implement your high level code from here
    
    // Edges of the screen
    fill_rectangle(ctx->screen, 0, 0, ctx->screen->width,1,'-');
    fill_rectangle(ctx->screen, 0, 0, 1, ctx->screen->height,'|');
    fill_rectangle(ctx->screen, ctx->screen->width - 1, 0, 1, ctx->screen->height,'|');
    fill_rectangle(ctx->screen, 0, ctx->screen->height - 1 ,ctx->screen->width, 1,'-');
    

    cursor_init(ctx, ctx->screen->width/2, (int)ctx->screen->height/2, '*');
    draw_line(ctx->screen, (int)ctx->screen->width/2,(int)ctx->screen->height/2,ctx->cursor.posX, ctx->cursor.posY,'o');
    display_cursor(ctx->screen, &ctx->cursor);   
}

void* event_loop(termlib_context* ctx) {
    char c;
    float angle = 0;
    while(1) {  
        ctx->cursor.posX= ctx->screen->width/2 + cos(angle)*ctx->screen->width/3;
        ctx->cursor.posY= ctx->screen->height/2 + sin(angle)*ctx->screen->height/3;
        angle+=0.01f;
        usleep(10000);
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
