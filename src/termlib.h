#ifndef TERMLIB_H
#define TERMLIB_H
#include "termlib_types.h"

termlib_context* termlib_init(void);
void termlib_event_loop(termlib_context *ctx);

void termlib_end(void);

#endif