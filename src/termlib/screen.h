/**
 * termlib version 0.2 (c) Julien LE THENO
 * (c) MIT LICENSE
 */
#ifndef SCREEN_H
#define SCREEN_H
#include "termlib_types.h"

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
 *  @param ctx    : a pointer to the temrlib context data structure
 *  @param posX   : the X coordinate of the 'pixel' to get, in cols
 *  @param posY   : the Y coordinate of the 'pixel' to get, in rows
 *  @returns char : the character at the given position
 */
char get_pixel(termlib_screen * ctx, int posX, int posY);

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

#endif
