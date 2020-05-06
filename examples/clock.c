#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "termlib.h"
#include <log_system.h>
#include <resman.h>
#include <unistd.h>

void* init(termlib_context* ctx){
    // Implement your high level code from here
    // Edges of the screen
    fill_rectangle(ctx->screen, 0, 0, ctx->screen->width,1,'-', FG_DEFAULT, BG_DEFAULT);
    fill_rectangle(ctx->screen, 0, 0, 1, ctx->screen->height,'|', FG_DEFAULT, BG_DEFAULT);
    fill_rectangle(ctx->screen, ctx->screen->width - 1, 0, 1, ctx->screen->height,'|', FG_DEFAULT, BG_DEFAULT);
    fill_rectangle(ctx->screen, 0, ctx->screen->height - 1 ,ctx->screen->width, 1,'-', FG_DEFAULT, BG_DEFAULT);
    

    cursor_init(ctx, ctx->screen->width/2, (int)ctx->screen->height/2, '*');
    draw_line(ctx->screen, (int)ctx->screen->width/2,(int)ctx->screen->height/2,ctx->cursor.posX, ctx->cursor.posY,'o', FG_DEFAULT, BG_DEFAULT);
    display_cursor(ctx->screen, &ctx->cursor);   
}

void* event_loop(termlib_context* ctx) {
    float angle = 0;
    while(1) {  
        ctx->cursor.posX= ctx->screen->width/2 + cos(angle)*ctx->screen->width/3;
        ctx->cursor.posY= ctx->screen->height/2 + sin(angle)*ctx->screen->height/3;
        angle+=0.01f;
        usleep(10000);
        fill_rectangle(ctx->screen, 1,1, ctx->screen->width-2, ctx->screen->height-2,' ', FG_DEFAULT, BG_DEFAULT);
        draw_line(ctx->screen, (int)ctx->screen->width/2,(int)ctx->screen->height/2,ctx->cursor.posX, ctx->cursor.posY,'o', FG_DEFAULT, BG_DEFAULT);
        display_cursor(ctx->screen, &ctx->cursor);     
    }
}


// DO NOT TOUCH MAIN
int main(int argc, char *argv[])
{
    resman_init();
    log_init();
    termlib_context* ctx = termlib_init((void*)init);
    termlib_event_loop(ctx, (void*)event_loop);
    return 0;
    log_end();
    resman_end();
}
