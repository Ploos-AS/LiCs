#include "events.h"

int bot_dispatch_event(const irc_event *event, const bot_event_binding *bindings, size_t binding_count) {
    size_t i;
    int handled = 0;
    if (!event || !bindings) return -1;
    for (i = 0; i < binding_count; ++i) {
        if (bindings[i].type != event->type || !bindings[i].handler) continue;
        if (bindings[i].handler(event, bindings[i].user)) handled = 1;
    }
    return handled;
}
