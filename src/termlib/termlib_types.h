/**
 * termlib version 0.2 (c) Julien LE THENO
 * (c) MIT LICENSE
 */
#ifndef TERMLIB_TYPES_H
#define TERMLIB_TYPES_H
#include <pthread.h>
#include <semaphore.h> 

/** Foreground color enum
 */ 
typedef enum {
    FG_DEFAULT = 39,
    FG_BLACK = 30,  // more info here (https://misc.flogisoft.com/bash/tip_colors_and_formatting)
    FG_RED,
    FG_GREEN,
    FG_YELLOW,
    FG_BLUE,
    FG_MAGENTA,
    FG_CYAN,
    FG_LIGHT_GRAY,
    FG_DARK_GRAY = 90,
    FG_LIGHT_RED,
    FG_LIGHT_GREEN,
    FG_LIGHT_YELLOW,
    FG_LIGHT_BLUE,
    FG_LIGHT_MAGENTA,
    FG_LIGHT_CYAN,
    FG_WHITE,
} color_enum_fg;

/** Background color enum
 */ 
typedef enum {
    BG_DEFAULT = 49,
    BG_BLACK = 40,
    BG_RED,
    BG_GREEN,
    BG_YELLOW,
    BG_BLUE,
    BG_MAGENTA,
    BG_CYAN,
    BG_LIGHT_GRAY,
    BG_DARK_GRAY = 100,
    BG_LIGHT_RED,
    BG_LIGHT_GREEN,
    BG_LIGHT_YELLOW,
    BG_LIGHT_BLUE,
    BG_LIGHT_MAGENTA,
    BG_LIGHT_CYAN,
    BG_WHITE,
} color_enum_bg;


/** represents a simple 'pixel' : one display unit of the screen
 */
typedef struct {
    char rep; // representation of the pixel
    color_enum_fg fg;
    color_enum_bg bg;
} pixel; // TODO add color fg/bg as pixel fields

/** represents the screen : the terminal area used by termlib
 */
typedef struct {
    unsigned int width;       // width of the 'screen' in columns
    unsigned int height;      // height of the 'screen' in rows
    pthread_t display_thread; // identifier of the tread that draws the screen
    sem_t display_semaphore;  // semaphore blocked during display to avoid artefacts
    pixel*  pixels;           // pointer to the pixel buffer array
    pixel*  ready_pixels;     // pointer to the screen buffer array
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
