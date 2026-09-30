#ifndef LICS_EVENTS_H
#define LICS_EVENTS_H
#include "irc_core.h"
#include <stddef.h>
#define BOT_MAX_EVENT_HANDLERS 32
typedef int (*bot_event_fn)(const irc_event *event, void *user);
typedef struct { irc_event_type type; bot_event_fn handler; void *user; } bot_event_binding;
typedef struct { bot_event_binding entries[BOT_MAX_EVENT_HANDLERS]; size_t count; } event_registry;
void event_registry_init(event_registry *registry);
int event_registry_register(event_registry *registry, irc_event_type type, bot_event_fn handler, void *user);
int event_registry_unregister(event_registry *registry, irc_event_type type, bot_event_fn handler);
int bot_dispatch_event(const irc_event *event, const event_registry *registry);
#endif
