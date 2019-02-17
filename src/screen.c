#include "screen.h"
#include "termlib_types.h"
#include <stdio.h>
#include <stdlib.h>

void screen_init(termlib_context *ctx, int width, int height)
{
    int i;
    termlib_screen *screen = (termlib_screen *)malloc(sizeof(termlib_screen));
    screen->pixels = (pixel **)malloc(sizeof(pixel*));
    screen->width = width;
    screen->height = height;
    for (i = 0; i < width * height; i++)
    {
        pixel *p = malloc(sizeof(pixel));
        p->rep = ' ';
        *(screen->pixels + i) = p;
    }
    ctx->screen = screen;
    pthread_create(&screen->display_thread, NULL, (void *)screen_display_thread, (void *)ctx);
}

void display(termlib_screen *screen)
{
    int i, j;
    for (i = 0; i < screen->width; i++)
    {
        for (j = 0; j < screen->height; j++)
        {
            pixel *p = (pixel *)(((void *)screen->pixels) + i * screen->height + j);
            printf("%c", p->rep);
        }
        printf("\n\r");
    }
}

void *screen_display_thread(void *ctx_arg)
{
    termlib_context *ctx = (termlib_context *)ctx_arg;

    while (!ctx->exit)
    {
        display(ctx->screen);
        // TODO WAIT FOR EVENT
    }
}

void set_pixel(termlib_screen *ctx, int posX, int posY, char c)
{
    pixel *p = (pixel *)(((void *)ctx->pixels) + posX * ctx->height + posY);
    p->rep = c;
}

char get_pixel(termlib_screen *ctx, int posX, int posY)
{
}
