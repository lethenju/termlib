#include <stdio.h>
#include <stdlib.h>
#include "../src/screen.h"
#include "../src/termlib.h"


float centerX;
float centerY;
void* init(termlib_context* ctx){
    // Implement your high level code from here
    
    // Edges of the screen
    fill_rectangle(ctx->screen, 0, 0, ctx->screen->width,1,'-', FG_DEFAULT, BG_DEFAULT);
    fill_rectangle(ctx->screen, 0, 0, 1, ctx->screen->height,'|', FG_DEFAULT, BG_DEFAULT);
    fill_rectangle(ctx->screen, ctx->screen->width - 1, 0, 1, ctx->screen->height,'|', FG_DEFAULT, BG_DEFAULT);
    fill_rectangle(ctx->screen, 0, ctx->screen->height - 1 ,ctx->screen->width, 1,'-', FG_DEFAULT, BG_DEFAULT);
    

    cursor_init(ctx, (int)ctx->screen->width/2 + 5,(int)ctx->screen->height/2, '*', FG_DEFAULT, BG_DEFAULT);
    draw_line(ctx->screen, (int)ctx->screen->width/2,(int)ctx->screen->height/2,ctx->cursor.posX, ctx->cursor.posY,'o', FG_DEFAULT, BG_DEFAULT);
    draw_circle(ctx->screen, (int)ctx->screen->width/2,(int)ctx->screen->height/2, (int)ctx->screen->height/4, '°', FG_DEFAULT, BG_DEFAULT);

    fill_circle(ctx->screen, (int)ctx->screen->width/2,(int)ctx->screen->height/2, (int)ctx->screen->height/5, '.', FG_DEFAULT, BG_DEFAULT);
    display_cursor(ctx->screen, &ctx->cursor);   
}

int cord_dist(termlib_context* ctx){
    return sqrt((centerX-ctx->cursor.posX)*(centerX-ctx->cursor.posX) + 
                (centerY-ctx->cursor.posY)*(centerY-ctx->cursor.posY));
}

void* phys_loop(void* context) {
    termlib_context* ctx = (termlib_context*) context;
    centerX = (float)ctx->screen->width/2;
    centerY = (float)ctx->screen->height/2;
    float accelX = 0;
    float accelY = 1;
    float vitX = 0;
    float vitY = 0;
    char buf1[64];
    char buf2[64];
    float diffX, diffY;
 
    while(1) {

        accelX *= 0.9f;
        accelY *= 0.9f;
        accelY += 1;
        usleep(100000);
        vitX += accelX;
        vitY += accelY;
        
        ctx->cursor.posX+=(int)vitX;        
        ctx->cursor.posY+=(int)vitY;
        diffX = centerX - (float)ctx->cursor.posX;
        diffY = centerY - (float)ctx->cursor.posY;

        fill_rectangle(ctx->screen, 1,1, ctx->screen->width-2, ctx->screen->height-2,' ', FG_DEFAULT, BG_DEFAULT);

        if (cord_dist(ctx) > 20){
            write_text(ctx->screen, 10, 9, "cor ! ", FG_DEFAULT, BG_DEFAULT);
            accelX = diffX/(float)10.0f;
            accelY = diffY/(float)10.0f;
            ctx->cursor.posX += accelX;
            ctx->cursor.posY += accelY;
        }

        sprintf(&buf1, "acX : %f",accelX);
        sprintf(&buf2, "acY : %f",accelY);
        
        write_text(ctx->screen, 10, 10, &buf1);
        write_text(ctx->screen, 10, 11, &buf2);


        
        fill_circle(ctx->screen, (int)centerX,(int)centerY, (int)ctx->screen->height/5, '.', FG_DEFAULT, BG_DEFAULT);
        draw_circle(ctx->screen, (int)centerX,(int)centerY, (int)ctx->screen->height/4, '°', FG_DEFAULT, BG_DEFAULT);
        draw_line(ctx->screen, (int)centerX,(int)centerY,ctx->cursor.posX, ctx->cursor.posY,'o', FG_DEFAULT, BG_DEFAULT);
        display_cursor(ctx->screen, &ctx->cursor);     
    }
}

void* event_loop(termlib_context* ctx) {
    pthread_t p_phys_loop;
    pthread_create(&p_phys_loop, NULL, phys_loop, (void*) ctx);
    char c;
    while((c=getchar())!= '.') {
        switch (c){
            case 'z':
                    centerY-=2;
                    break;
            case 'q':
                    centerX-=2;
                    break;
            case 's':
                    centerY+=2;
                    break;
            case 'd':
                    centerX+=2;
                    break;
        }       
    }
    

   
}


// DO NOT TOUCH MAIN
int main(int argc, char *argv[])
{
    termlib_context* ctx = termlib_init((void*)init);
    termlib_event_loop(ctx, (void*)event_loop);
    return 0;
}
