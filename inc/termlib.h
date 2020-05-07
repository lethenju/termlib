#ifndef TERMLIB_H
#define TERMLIB_H
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
    color_enum_fg fg; // foreground (char) color
    color_enum_bg bg; // background color
} pixel;

/** represents the screen : the terminal area used by termlib
 */
typedef struct {
    unsigned int width;       // width of the 'screen' in columns
    unsigned int height;      // height of the 'screen' in rows
    pthread_t display_thread; // identifier of the tread that draws the screen
    sem_t display_semaphore;  // semaphore blocked during display to avoid artefacts
    pixel*  pixels;           // pointer to the pixel buffer array
    pixel*  ready_pixels;     // pointer to the screen buffer array
    int*    filter_pixels;    // pointer to the filter screen
    volatile int stop;        // flag to stop the thread that draws the screen
    volatile int draw_screen; // flag to update and draw the screen
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
    int socket;             // socket of the client
} termlib_context;




/** initialises the screen, filling ctx->screen data structure
 *  @param ctx    : a pointer to the termlib context data structure
 *  @param width  : the width of the screen, in cols
 *  @param height : the height of the screen, in rows
 */
void screen_init(termlib_context * ctx, int width, int height);

/** the thread that loops in the screen display procedure
 *  @param ctx : a pointer to the termlib context data structure
 */
void *screen_display_thread(void * ctx);

/** called when the frame is ready to draw : will copy the frame buffer to the screen buffer
 *  @param ctx  : a pointer to the termlib_screen context data structure
 */  
void screen_frame_ready(termlib_screen * ctx);

/** changes a 'pixel' (char) in the screen with its coordinates
 *  @param ctx  : a pointer to the temrlib context data structure
 *  @param posX : the X coordinate of the 'pixel' to change, in cols
 *  @param posY : the Y coordinate of the 'pixel' to change, in rows
 *  @param c    : character to be setted at the given position
 *  @param fg   : foreground (char) color
 *  @param bg   : background color
 */
void set_pixel(termlib_screen * ctx, int posX, int posY, char c, color_enum_fg fg, color_enum_bg bg);

/** returns the chararacter of a given 'pixel' in the screen with its coordinates
 *  @param ctx      : a pointer to the temrlib context data structure
 *  @param posX     : the X coordinate of the 'pixel' to get, in cols
 *  @param posY     : the Y coordinate of the 'pixel' to get, in rows
 *  @returns pixel* : a pointer to the pixel
 */
pixel* get_pixel(termlib_screen * ctx, int posX, int posY);

/** 'draws' a rectangle on the screen with the given characteristics 
 *  @param ctx    : a pointer to the screen context data structure (termlib_ctx->screen)
 *  @param posX   : the X coordinate of the top left corner, in cols
 *  @param posY   : the Y coordinate of the top left corner, in rows
 *  @param width  : the width of the rectangle, in cols
 *  @param height : the height of the rectangle, in rows
 *  @param rep    : the character representation of the rectangle to be drawned (will be on each edge)
 *  @param fg     : foreground (char) color
 *  @param bg     : background color
 */
void draw_rectangle(termlib_screen *ctx, int posX, int posY, int width, int height, char rep, color_enum_fg fg, color_enum_bg bg);


/** 'draws' and 'fills' a rectangle on the screen with the given characteristics 
 *  @param ctx    : a pointer to the screen context data structure (termlib_ctx->screen)
 *  @param posX   : the X coordinate of the top left corner, in cols
 *  @param posY   : the Y coordinate of the top left corner, in rows
 *  @param width  : the width of the rectangle, in cols
 *  @param height : the height of the rectangle, in rows
 *  @param rep    : the character representation of the rectangle to be drawned (will be on each edge, and inside)
 *  @param fg     : foreground (char) color
 *  @param bg     : background color
 */
void fill_rectangle(termlib_screen *ctx, int posX, int posY, int width, int height, char rep, color_enum_fg fg, color_enum_bg bg);

/** 'draws' a line on the screen with the given characteristics 
 *  @param ctx   : a pointer to the screen context data structure (termlib_ctx->screen)
 *  @param posX  : the X coordinate of the first point, in cols
 *  @param posY  : the Y coordinate of the first point, in rows
 *  @param posX2 : the X coordinate of the second point, in cols
 *  @param posY2 : the Y coordinate of the second point, in rows
 *  @param rep   : the character representation of the line to be drawned (will be on each pixel)
 *  @param fg     : foreground (char) color
 *  @param bg     : background color
 */
void draw_line(termlib_screen *ctx, int posX, int posY, int posX2, int posY2, char rep, color_enum_fg fg, color_enum_bg bg);

/** 'draws' a circle on the screen with the given characteristics 
 *  @param ctx    : a pointer to the screen context data structure (termlib_ctx->screen)
 *  @param posX   : the X coordinate of the center, in cols
 *  @param posY   : the Y coordinate of the center, in rows
 *  @param radius : the radius of the circle, in cols
 *  @param rep    : the character representation of the circle to be drawned (will be along the radius)
 *  @param fg     : foreground (char) color
 *  @param bg     : background color
 */
void draw_circle(termlib_screen *ctx, int posX, int posY, int radius, char rep, color_enum_fg fg, color_enum_bg bg);

/** 'draws' and 'fills' a circle on the screen with the given characteristics 
 *  @param ctx    : a pointer to the screen context data structure (termlib_ctx->screen)
 *  @param posX   : the X coordinate of the center, in cols
 *  @param posY   : the Y coordinate of the center, in rows
 *  @param radius : the radius of the circle, in cols
 *  @param rep    : the character representation of the circle to be drawned (will be along the radius and inside)
 *  @param fg     : foreground (char) color
 *  @param bg     : background color
 */
void fill_circle(termlib_screen *ctx, int posX, int posY, int radius, char rep, color_enum_fg fg, color_enum_bg bg);

/** writes text at some given coordinates in the screen
 *  @param ctx    : a pointer to the screen context data structure (termlib_ctx->screen)
 *  @param posX   : the X coordinate of the first character, in cols
 *  @param posY   : the Y coordinate of the first character, in rows
 *  @param string : the text string to be displayed
 *  @param fg     : foreground (char) color
 *  @param bg     : background color
 */
void write_text(termlib_screen *ctx, int posX, int posY, char* string, color_enum_fg fg, color_enum_bg bg);

/**TODO ADD DOC
 */ 
void rectangle_filter(termlib_screen *ctx, int posX, int posY, int width, int height, int value);

/** stops the display threads and free the data structures
 *  @param ctx    : a pointer to the screen context data structure (termlib_ctx->screen)
 */ 
void screen_end(termlib_screen *ctx);


void cursor_init(termlib_context *ctx, int posX, int posY, char rep);

void display_cursor(termlib_screen *screen, termlib_cursor *cursor);

termlib_context* termlib_init(void (*init_func)(termlib_context*));
termlib_context* termlib_init2();

void termlib_event_loop(termlib_context *ctx, void(*event_loop)(termlib_context*));

void termlib_end(termlib_context *ctx);

void termlib_server_init(termlib_context *ctx);

#endif