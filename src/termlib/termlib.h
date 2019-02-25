/**
 * termlib version 0.2 (c) Julien LE THENO
 * (c) MIT LICENSE
 */
#ifndef TERMLIB_H
#define TERMLIB_H
#include "termlib_types.h"

/** initiliases Termlib, and return the context data structure
 *  @param init_func : the custom init function to be called as a callback
 *  @returns         : the context data structure
 */
termlib_context* termlib_init(void* init_func);

/** launches the event loop function given as a callback
 *  @param event_loop : the event loop function 
 *  @param ctx        : a pointer to the termlib context data structure
 */
void termlib_event_loop(termlib_context *ctx, void* event_loop);

/** ends the termlib instance and frees data structure 
 *  @param ctx  : a pointer to the termlib context data structure
 */
void termlib_end(termlib_context *ctx);

#endif
