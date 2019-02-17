#include <stdio.h>
#include <stdlib.h>
#include "screen.h"
#include "termlib.h"

int main(int argc, char *argv[])
{
    termlib_context* ctx =  termlib_init();
    termlib_event_loop(ctx);

    return 0;
}
