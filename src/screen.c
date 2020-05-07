#include "termlib.h"
#include "log_system.h"
#include <resman.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

void screen_init(termlib_context *ctx, int width, int height)
{
    DEBUG_TRACE("screen init");
    int i;
    DEBUG_TRACE("screen init : server initialisation");
    termlib_server_init(ctx);

    termlib_screen *screen = resman_alloc("termlib_screen", sizeof(termlib_screen));
    screen->pixels = resman_alloc("termlib screen->pixels", sizeof(pixel) * width * height);
    screen->ready_pixels = resman_alloc("termlib screen->ready_pixels", sizeof(pixel) *  width * height);
    screen->filter_pixels = resman_alloc("termlib screen->filter_pixels", sizeof(int) *  width * height);
    screen->width = width;
    screen->height = height;
    sem_init(&(screen->display_semaphore), 0, 1);
    fill_rectangle(screen, 0,0, width, height,' ', FG_DEFAULT, BG_DEFAULT);
    rectangle_filter(screen, 0, 0, width, height, 1); // enabling all screen to be drawn on
    screen_frame_ready(screen);
    screen->stop = 0;
    screen->draw_screen = 0;
    pthread_create(&screen->display_thread, NULL, (void *)screen_display_thread, (void *)ctx);
    ctx->screen = screen;
    DEBUG_TRACE("screen init done!");
}

void display(termlib_screen *screen, int socket)
{
    char* buffer = malloc (2048);
    DEBUG_TRACE("display");
    sem_wait(&(screen->display_semaphore));
    int i, j;
    system("clear");
    pixel *p = screen->ready_pixels; // getting first pixel
    sprintf(buffer,"\e[%dm(\e[%dm",p->bg, p->fg); // Setting color the first time
    for (j = 0; j < screen->height; j++)
    {
        for (i = 0; i < screen->width; i++)
        {
            pixel *new = (screen->ready_pixels) + i * screen->height + j;
            if (p->bg != new->bg || p->fg != new->fg) { // change color only when it changes
                sprintf(buffer+strlen(buffer),"\e[%dm\e[%dm",new->bg, new->fg);
            }
            sprintf(buffer+strlen(buffer),"%c", new->rep);
            p = new;
        }
        sprintf(buffer+strlen(buffer),"\n\r");
    }
    send(socket, buffer, strlen(buffer), 0);
    free(buffer);
    sem_post(&(screen->display_semaphore));
    screen->draw_screen = 0;
}

void *screen_display_thread(void *ctx_arg)
{
    DEBUG_TRACE("screen_display_thread");
    termlib_context *ctx = (termlib_context *)ctx_arg;

    while (!ctx->screen->stop)
    {
        usleep(100000);
        if (ctx->screen->draw_screen)
            display(ctx->screen, ctx->socket);
    }
    WARNING_TRACE("exiting screen_display_thread");

}

void screen_frame_ready(termlib_screen *ctx)
{
    DEBUG_TRACE("screen_frame_ready");
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
    ctx->draw_screen = 1;
    DEBUG_TRACE("screen_frame_ready done!");
}

void draw_rectangle(termlib_screen *ctx, int posX, int posY, int width, int height, char rep, color_enum_fg fg, color_enum_bg bg)
{
    DEBUG_TRACE("draw rectangle!");
    draw_line(ctx, posX, posY, posX+width, posY, rep, fg, bg);
    draw_line(ctx, posX, posY, posX, posY+height, rep, fg, bg);
    draw_line(ctx, posX+width, posY+height, posX+width, posY, rep, fg, bg);
    draw_line(ctx, posX+width, posY+height, posX, posY+height, rep, fg, bg);
    DEBUG_TRACE("draw rectangle done");
}

void fill_rectangle(termlib_screen *ctx, int posX, int posY, int width, int height, char rep, color_enum_fg fg, color_enum_bg bg)
{
    DEBUG_TRACE("fill rectangle");
    int i,j;
    for (i = 0; i < width; i++)
    {
        for ( j = 0; j < height; j++ ) 
        {
            set_pixel(ctx,posX+i, posY+j, rep, fg, bg);
        }
    }
    DEBUG_TRACE("fill rectangle done");
}

void draw_line(termlib_screen *ctx, int posX, int posY, int posX2, int posY2, char rep, color_enum_fg fg, color_enum_bg bg)
{
    DEBUG_TRACE("draw line");
    float diffX = posX2 - posX;
    float diffY = posY2 - posY;
    float i;
    int max = (abs(diffX) > abs(diffY))? abs(diffX) : abs(diffY);
      
    for (i = 0; i < max; i++) 
    {
        set_pixel(ctx,(int) (posX+(i/max)*diffX),(int) (posY+(i/max)*diffY), rep, fg, bg);
    }
    DEBUG_TRACE("draw line done");
}

void draw_circle(termlib_screen *ctx, int posX, int posY, int radius, char rep, color_enum_fg fg, color_enum_bg bg)
{
    DEBUG_TRACE("draw circle");
    radius = abs(radius);
    float perimeter = 10*radius;
    float i;
    for (i=0; i < perimeter; i++)
    {
        set_pixel(ctx, posX + radius*cos(i/radius), posY + radius*sin(i/radius), rep, fg, bg);
    }
    DEBUG_TRACE("draw circle done");
}

void fill_circle(termlib_screen *ctx, int posX, int posY, int radius, char rep, color_enum_fg fg, color_enum_bg bg)
{
    DEBUG_TRACE("fill circle");
    int i = 0;
    for (i = 0; i < radius; i++) 
    {
        draw_circle(ctx, posX, posY, i, rep, fg, bg);
    }
    DEBUG_TRACE("fill circle done");
    
}


void set_pixel(termlib_screen *ctx, int posX, int posY, char c, color_enum_fg fg, color_enum_bg bg)
{
    if (!(*(ctx->filter_pixels + posX * ctx->height + posY)))
        return; // if the filter doesnt allow modification here

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

void rectangle_filter(termlib_screen *ctx, int posX, int posY, int width, int height, int value)
{
    int i,j;
    DEBUG_TRACE("rectangle filter %d from %d, %d to %d, %d", 
                value, posX, posY, width, height);
    for (i = posX; i < posX + width; i++)
    {
        for ( j = posY; j < posY + height; j++ ) 
        {
            int* p = ctx->filter_pixels + i * ctx->height + j;
            *p = value;
        }
    }
}

void screen_end(termlib_screen *ctx)
{
    WARNING_TRACE("SCREEN END");
    ctx->stop = 1;
    pthread_join(ctx->display_thread, NULL);
    resman_free(ctx->ready_pixels); 
    resman_free(ctx->pixels);
    resman_free(ctx);
    WARNING_TRACE("SCREEN END ENDED");
}