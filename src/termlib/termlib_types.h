/**
 * termlib version 0.2 (c) Julien LE THENO
 * (c) MIT LICENSE
 */
#ifndef TERMLIB_TYPES_H
#define TERMLIB_TYPES_H
#include <pthread.h>

/** represents a simple 'pixel' : one display unit of the screen
 */
typedef struct {
    char rep; // representation of the pixel
} pixel; // TODO add color fg/bg as pixel fields

/** represents the screen : the terminal area used by termlib
 */
typedef struct {
    unsigned int width;       // width of the 'screen' in columns
    unsigned int height;      // height of the 'screen' in rows
    pthread_t display_thread; // identifier of the tread that draws the screen
    pixel*  pixels;           // pointer to the pixel array
} termlib_screen;

/** input structure, that links to input-related data
 */ 
typedef struct {
    pthread_t input_thread; // identifier of the thread that manages input
} termlib_input; // TODO Add a input stack to better manage the input data

/** represents the cursor object. 
 */
typedef struct {
    int posX; // X position of the cursor
    int posY; // Y position of the cursor
    char rep; // character representation of the cursor
} termlib_cursor; // TODO Should be moved to winman instead

/** whole termlib context pointer that gather all data about the current app, termlib-wise
 */
typedef struct {
    termlib_screen* screen; // pointer to the screen structure
    termlib_input* input;   // pointer to the input structure
    termlib_cursor cursor;  // pointer to the cursor structure
    volatile int exit;      // exit flag. When it is turned to 1, the threads will exit and the application will stop.
} termlib_context;

#endif
