#include "dispatcher.h"
#include <string.h>

int dispatch_privmsg(const irc_event *event, const command_binding *bindings, size_t binding_count, char *reply, size_t reply_size) {
    if (!event || event->type != IRC_EVENT_PRIVMSG || !bindings || !reply || reply_size == 0) return 0;
    for (size_t i = 0; i < binding_count; ++i) {
        if (strcmp(event->text, bindings[i].command) == 0) {
            return bindings[i].handler ? bindings[i].handler(event, reply, reply_size) : 0;
        }
    }
    return 0;
}
