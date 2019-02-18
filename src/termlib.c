#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#include "termlib_types.h"
#include "screen.h"
#include "cursor.h"

termlib_context *termlib_init(void)
{
    system("/bin/stty raw"); // TODO state restoration
    termlib_context *context = (termlib_context *)malloc(sizeof(termlib_context));
    // Getting windows nb col/rows
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    screen_init(context, w.ws_col-1, w.ws_row-1);

    // Edges of the screen
    fill_rectangle(context->screen, 0, 0, context->screen->width,1,'-');
    fill_rectangle(context->screen, 0, 0, 1, context->screen->height,'|');
    fill_rectangle(context->screen, context->screen->width - 1, 0, 1, context->screen->height,'|');
    fill_rectangle(context->screen, 0, context->screen->height - 1 ,context->screen->width, 1,'-');
    
    cursor_init(context, 5, 5, '*');
    display_cursor(context->screen, &context->cursor);     

    context->exit = 0;
    return context;
}

void termlib_event_loop(termlib_context *ctx)
{
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
        display_cursor(ctx->screen, &ctx->cursor);     
    }
    system("clear");
    ctx->exit = 1;

}

void termlib_end(termlib_context *ctx)
{
    // TODO FREE!
}
