#include "runtime_adapter.h"
#include <stdio.h>
#include <string.h>

static int initialized;

int lics_runtime_init(void) {
    initialized = 1;
    return 0;
}

void lics_runtime_shutdown(void) {
    initialized = 0;
}

int lics_runtime_command(const char *symbol, const irc_event *event, char *reply, size_t reply_size) {
    (void)event;
    if (!initialized || !symbol || !reply || reply_size == 0) return 0;

    /* M0 adapter contract. The Scheme VM backend replaces this built-in
       fallback in the next step without changing dispatcher or IRC core. */
    if (strcmp(symbol, "hello") == 0) {
        snprintf(reply, reply_size, "Hello from LiCs");
        return 1;
    }
    return 0;
}
