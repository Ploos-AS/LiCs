#ifndef LICS_EVENTS_H
#define LICS_EVENTS_H
#include "irc_core.h"
#include <stddef.h>
typedef int (*bot_event_fn)(const irc_event *event, void *user);
typedef struct { irc_event_type type; bot_event_fn handler; void *user; } bot_event_binding;
int bot_dispatch_event(const irc_event *event, const bot_event_binding *bindings, size_t binding_count);
#endif
