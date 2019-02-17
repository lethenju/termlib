#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "termlib_types.h"
#include "screen.h"

termlib_context *termlib_init(void)
{
    termlib_context *context = (termlib_context *)malloc(sizeof(termlib_context));
    screen_init(context, 10, 10);
    context->exit = 0;
    return context;
}

void termlib_event_loop(termlib_context *ctx)
{
    //set_pixel(ctx->screen, 5, 5, 'c');
    while (!ctx->exit)
    {
        //printf("main loop");
        usleep(100000);
    }
}

void termlib_end(termlib_context *ctx)
{
}