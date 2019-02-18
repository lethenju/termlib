#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "termlib_types.h"
#include "screen.h"
#include "cursor.h"

termlib_context *termlib_init(void)
{
    system("/bin/stty raw"); // TODO state restoration
    termlib_context *context = (termlib_context *)malloc(sizeof(termlib_context));
    screen_init(context, 50, 20);
    cursor_init(context, 5, 5, '*');
    context->exit = 0;
    return context;
}

void termlib_event_loop(termlib_context *ctx)
{
    char c;
    while((c=getchar())!= '.') {
        switch (c){
            case 'z':
                    if (ctx->cursor.posY > 0)
                        ctx->cursor.posY--;
                    break;
            case 'q':
                    if (ctx->cursor.posX > 0)
                        ctx->cursor.posX--;
                    break;
            case 's':
                    if (ctx->cursor.posY < ctx->screen->height - 1)
                        ctx->cursor.posY++;
                    break;
            case 'd':
                    if (ctx->cursor.posX < ctx->screen->width - 1)
                        ctx->cursor.posX++;
                    break;
        }       
        fill_rectangle(ctx->screen, 0,0, ctx->screen->width, ctx->screen->height,' ');
        display_cursor(ctx->screen, &ctx->cursor);     
    }
    ctx->exit = 1;

}

void termlib_end(termlib_context *ctx)
{
    // TODO FREE!
}