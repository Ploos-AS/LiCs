#ifndef LICS_DISPATCHER_H
#define LICS_DISPATCHER_H

#include "irc_core.h"
#include <stddef.h>

typedef int (*script_command_fn)(const irc_event *event, char *reply, size_t reply_size);

typedef struct {
    const char *command;
    script_command_fn handler;
} command_binding;

int dispatch_privmsg(const irc_event *event, const command_binding *bindings, size_t binding_count, char *reply, size_t reply_size);

#endif
