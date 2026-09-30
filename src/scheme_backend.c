#include "scheme_backend.h"

#ifdef LICS_WITH_TINYSCHEME
#include "scheme.h"
#include <stdio.h>
#include <string.h>

static scheme *sc;
static char *active_reply;
static size_t active_reply_size;

static pointer irc_reply(scheme *runtime, pointer args) {
    pointer arg;
    if (!active_reply || active_reply_size == 0 || args == runtime->NIL) return runtime->NIL;
    arg = pair_car(args);
    if (!is_string(arg)) return runtime->NIL;
    snprintf(active_reply, active_reply_size, "%s", string_value(arg));
    return runtime->T;
}

int scheme_backend_init(void) {
    sc = scheme_init_new();
    if (!sc) return -1;
    scheme_define(sc, sc->global_env, mk_symbol(sc, "irc-reply"), mk_foreign_func(sc, irc_reply));
    if (scheme_load_string(sc,
        "(define (hello nick) (irc-reply \"Hello from LiCs\"))\n"
        "(define (on-command name nick) (if (string=? name \"hello\") (hello nick) #f))\n") != 0) {
        scheme_deinit(sc);
        sc = NULL;
        return -1;
    }
    return 0;
}

void scheme_backend_shutdown(void) {
    if (sc) scheme_deinit(sc);
    sc = NULL;
}

int scheme_backend_command(const char *symbol, const irc_event *event, char *reply, size_t reply_size) {
    char expr[256];
    int rc;
    if (!sc || !symbol || !event || !reply || reply_size == 0) return 0;
    active_reply = reply;
    active_reply_size = reply_size;
    reply[0] = '\0';
    snprintf(expr, sizeof(expr), "(on-command \"%s\" \"%s\")", symbol, event->nick);
    rc = scheme_load_string(sc, expr);
    active_reply = NULL;
    active_reply_size = 0;
    return rc == 0 && reply[0] != '\0';
}
#else
int scheme_backend_init(void) { return -1; }
void scheme_backend_shutdown(void) {}
int scheme_backend_command(const char *symbol, const irc_event *event, char *reply, size_t reply_size) {
    (void)symbol; (void)event; (void)reply; (void)reply_size;
    return 0;
}
#endif
