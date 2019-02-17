#ifndef TERMLIB_TYPES_H
#define TERMLIB_TYPES_H
#include <pthread.h>

typedef struct pixel_t {
    char rep;
} pixel;

typedef struct termlib_screen_t {
    unsigned int width;
    unsigned int height;
    pthread_t display_thread;
    pixel**  pixels;
} termlib_screen;

typedef struct termlib_input_t {
    pthread_t input_thread;
} termlib_input;


typedef struct termlib_context_t {
    termlib_screen* screen;
    termlib_input* input;
    volatile int exit;
} termlib_context;

#endif