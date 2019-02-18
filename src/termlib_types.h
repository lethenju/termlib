#ifndef TERMLIB_TYPES_H
#define TERMLIB_TYPES_H
#include <pthread.h>

typedef struct {
    char rep;
} pixel;

typedef struct {
    unsigned int width;
    unsigned int height;
    pthread_t display_thread;
    pixel*  pixels;
} termlib_screen;

typedef struct {
    pthread_t input_thread;
} termlib_input;

typedef struct {
    int posX;
    int posY;
    char rep;
} termlib_cursor;

typedef struct {
    termlib_screen* screen;
    termlib_input* input;
    termlib_cursor cursor;
    volatile int exit;
} termlib_context;


#endif