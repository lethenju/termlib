#include "screen.h"
#include "termlib_types.h"
#include <stdio.h>
#include <stdlib.h>

void screen_init(termlib_context *ctx, int width, int height)
{
    int i;
    termlib_screen *screen = (termlib_screen *)malloc(sizeof(termlib_screen));
    screen->pixels = (pixel *)malloc(sizeof(pixel)+ width * height);
    screen->width = width;
    screen->height = height;
    fill_rectangle(screen, 0,0, width, height,' ');
    ctx->screen = screen;
    pthread_create(&screen->display_thread, NULL, (void *)screen_display_thread, (void *)ctx);
}

void display(termlib_screen *screen)
{
    int i, j;
    system("clear");
    for (j = 0; j < screen->height; j++)
    {
        for (i = 0; i < screen->width; i++)
        {
            pixel *p = (screen->pixels) + i * screen->height + j;
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
        usleep(100000);
        // TODO WAIT FOR EVENT
    }
}

void draw_rectangle(termlib_screen *ctx, int posX, int posY, int width, int height, char rep) {

}

void fill_rectangle(termlib_screen *ctx, int posX, int posY, int width, int height, char rep) {
    int i,j;
    for (i = 0; i < width; i++)
    {
        for ( j = 0; j < height; j++ ) 
        {
            set_pixel(ctx,posX+i, posY+j, rep);
        }
    }
}

void draw_line(termlib_screen *ctx, int posX, int posY, int posX2, int posY2, char rep) {

    float diffX = posX2 - posX;
    float diffY = posY2 - posY;
    float i;
    int max = (abs(diffX) > abs(diffY))? abs(diffX) : abs(diffY);
      
    for (i = 0; i < max; i++) 
    {
        set_pixel(ctx,(int) (posX+(i/max)*diffX),(int) (posY+(i/max)*diffY), rep);
    }
}

void draw_circle(termlib_screen *ctx, int posX, int posY, int radius, char rep) {

}

void fill_circle(termlib_screen *ctx, int posX, int posY, int radius, char rep) {

}


void set_pixel(termlib_screen *ctx, int posX, int posY, char c)
{
    pixel *p = ctx->pixels + posX * ctx->height + posY;
    p->rep = c;
}

char get_pixel(termlib_screen *ctx, int posX, int posY)
{
}
