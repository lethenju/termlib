#ifndef TERMLIB_H
#define TERMLIB_H
#include "termlib_types.h"
#include "cursor.h"
#include "screen.h"

termlib_context* termlib_init(void* init_func);
termlib_context* termlib_init2();

void termlib_event_loop(termlib_context *ctx, void* event_loop);

void termlib_end(void);

#endif