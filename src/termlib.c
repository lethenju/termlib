#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#include "termlib_types.h"
#include "screen.h"
#include "cursor.h"


// when no specific callback init 
termlib_context *termlib_init2() {
    system("/bin/stty raw"); // TODO state restoration
    termlib_context *context = (termlib_context *)malloc(sizeof(termlib_context));
    // Getting windows nb col/rows
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    screen_init(context, w.ws_col-1, w.ws_row-1);
    context->exit = 0;
    return context;
}
// With callback
termlib_context *termlib_init(void (*init_func)(termlib_context*))
{
    termlib_context* context = termlib_init2(init_func);
    (*init_func)(context);
    return context;
}

void termlib_end(termlib_context *ctx)
{
    ctx->exit = 1;
    screen_end(ctx->screen);
    free(ctx->input);
    free(ctx);
}

void termlib_event_loop(termlib_context *ctx, void(*event_loop)(termlib_context*))
{
    (*event_loop)(ctx);
    system("clear");
    termlib_end(ctx);
}
