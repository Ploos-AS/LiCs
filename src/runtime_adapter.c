#include "runtime_adapter.h"
#include "scheme_backend.h"
#include <stdio.h>
#include <string.h>

static int initialized;
static int scheme_active;

int lics_runtime_init(void) {
#ifdef LICS_WITH_TINYSCHEME
    scheme_active = (scheme_backend_init() == 0);
#else
    scheme_active = 0;
#endif
    initialized = 1;
    return 0;
}

void lics_runtime_shutdown(void) {
    if (scheme_active) scheme_backend_shutdown();
    scheme_active = 0;
    initialized = 0;
}

int lics_runtime_command(const char *symbol, const irc_event *event, char *reply, size_t reply_size) {
    if (!initialized || !symbol || !reply || reply_size == 0) return 0;

    if (scheme_active && scheme_backend_command(symbol, event, reply, reply_size)) return 1;

    /* Dependency-free M0 fallback keeps the core buildable without TinyScheme. */
    if (strcmp(symbol, "hello") == 0) {
        snprintf(reply, reply_size, "Hello from LiCs");
        return 1;
    }
    return 0;
}
