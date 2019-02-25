#include "screen.h"
#include "termlib_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void screen_init(termlib_context *ctx, int width, int height)
{
    int i;
    termlib_screen *screen = (termlib_screen *)malloc(sizeof(termlib_screen));
    screen->pixels = (pixel *)malloc(sizeof(pixel) * width * height);
    screen->ready_pixels = (pixel *)malloc(sizeof(pixel) *  width * height);
    screen->width = width;
    screen->height = height;
    sem_init(&(screen->display_semaphore), 0, 1);
    fill_rectangle(screen, 0,0, width, height,' ', FG_DEFAULT, BG_DEFAULT);
    screen_frame_ready(screen);
    ctx->screen = screen;
    pthread_create(&screen->display_thread, NULL, (void *)screen_display_thread, (void *)ctx);
}

void display(termlib_screen *screen)
{
    sem_wait(&(screen->display_semaphore));
    int i, j;
    system("clear");
    pixel *p = screen->ready_pixels; // getting first pixel
    printf("\e[%dm(\e[%dm",p->bg, p->fg); // Setting color the first time
    for (j = 0; j < screen->height; j++)
    {
        for (i = 0; i < screen->width; i++)
        {
            pixel *new = (screen->ready_pixels) + i * screen->height + j;
            if (p->bg != new->bg || p->fg != new->fg) { // change color only when it changes
                printf("\e[%dm\e[%dm",new->bg, new->fg);
            }
            printf("%c", new->rep);
            p = new;
        }
        printf("\n\r");
    }
    sem_post(&(screen->display_semaphore));

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

void screen_frame_ready(termlib_screen * ctx) 
{
    sem_wait(&(ctx->display_semaphore));
    int j;
    for (j = 0; j < ctx->height *  ctx->width; j++)
    {
            pixel *source = ctx->pixels + j;
            pixel *dest   = ctx->ready_pixels + j;
            dest->rep = source->rep;
            dest->bg = source->bg;
            dest->fg = source->fg;
            
    }
    sem_post(&(ctx->display_semaphore));
}

void draw_rectangle(termlib_screen *ctx, int posX, int posY, int width, int height, char rep, color_enum_fg fg, color_enum_bg bg)
{
    draw_line(ctx, posX, posY, posX+width, posY, rep, fg, bg);
    draw_line(ctx, posX, posY, posX, posY+height, rep, fg, bg);
    draw_line(ctx, posX+width, posY+height, posX+width, posY, rep, fg, bg);
    draw_line(ctx, posX+width, posY+height, posX, posY+height, rep, fg, bg);
    
}

void fill_rectangle(termlib_screen *ctx, int posX, int posY, int width, int height, char rep, color_enum_fg fg, color_enum_bg bg)
{
    int i,j;
    for (i = 0; i < width; i++)
    {
        for ( j = 0; j < height; j++ ) 
        {
            set_pixel(ctx,posX+i, posY+j, rep, fg, bg);
        }
    }
}

void draw_line(termlib_screen *ctx, int posX, int posY, int posX2, int posY2, char rep, color_enum_fg fg, color_enum_bg bg)
{

    float diffX = posX2 - posX;
    float diffY = posY2 - posY;
    float i;
    int max = (abs(diffX) > abs(diffY))? abs(diffX) : abs(diffY);
      
    for (i = 0; i < max; i++) 
    {
        set_pixel(ctx,(int) (posX+(i/max)*diffX),(int) (posY+(i/max)*diffY), rep, fg, bg);
    }
}

void draw_circle(termlib_screen *ctx, int posX, int posY, int radius, char rep, color_enum_fg fg, color_enum_bg bg)
{
    radius = abs(radius);
    float perimeter = 10*radius;
    float i;
    for (i=0; i < perimeter; i++)
    {
        set_pixel(ctx, posX + radius*cos(i/radius), posY + radius*sin(i/radius), rep, fg, bg);
    }
}

void fill_circle(termlib_screen *ctx, int posX, int posY, int radius, char rep, color_enum_fg fg, color_enum_bg bg)
{
    int i = 0;
    for (i = 0; i < radius; i++) 
    {
        draw_circle(ctx, posX, posY, i, rep, fg, bg);
    }
    
}


void set_pixel(termlib_screen *ctx, int posX, int posY, char c, color_enum_fg fg, color_enum_bg bg)
{
    pixel *p = ctx->pixels + posX * ctx->height + posY;
    p->rep = c;
    p->fg = fg;
    p->bg = bg;
}

void write_text(termlib_screen *ctx, int posX, int posY, char* string, color_enum_fg fg, color_enum_bg bg)
{
    int i = 0;
    for (i = 0; i < strlen(string); i++) 
    {
        set_pixel(ctx, posX + i, posY, *(string+i), fg, bg);
    }

}

pixel* get_pixel(termlib_screen *ctx, int posX, int posY)
{
    return ctx->pixels + posX * ctx->height + posY;
}
