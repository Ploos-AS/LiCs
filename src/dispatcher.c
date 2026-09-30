#include "dispatcher.h"
#include <string.h>
static const char *command_text(const irc_event *event) {
    if (!event || event->type != IRC_EVENT_PRIVMSG) return NULL;
    return event->text;
}
int dispatch_privmsg(const irc_event *event, const command_binding *bindings, size_t binding_count, char *reply, size_t reply_size) {
    const char *text = command_text(event);
    if (!text || !bindings || !reply || reply_size == 0) return 0;
    for (size_t i = 0; i < binding_count; ++i) {
        if (!bindings[i].command || !bindings[i].handler) continue;
        if (strcmp(text, bindings[i].command) == 0) return bindings[i].handler(event, reply, reply_size);
    }
    return 0;
}
