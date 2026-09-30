#ifndef LICS_DISPATCHER_H
#define LICS_DISPATCHER_H
#include "irc_core.h"
#include <stddef.h>
#define BOT_MAX_COMMANDS 32
typedef int (*script_command_fn)(const irc_event *event, char *reply, size_t reply_size);
typedef struct { const char *command; script_command_fn handler; } command_binding;
typedef struct { command_binding entries[BOT_MAX_COMMANDS]; size_t count; } command_registry;
void command_registry_init(command_registry *registry);
int command_registry_register(command_registry *registry, const char *command, script_command_fn handler);
int command_registry_unregister(command_registry *registry, const char *command);
int dispatch_privmsg(const irc_event *event, const command_registry *registry, char *reply, size_t reply_size);
#endif
