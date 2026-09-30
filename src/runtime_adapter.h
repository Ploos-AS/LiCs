#ifndef LICS_RUNTIME_ADAPTER_H
#define LICS_RUNTIME_ADAPTER_H
#include "irc_core.h"
#include "events.h"
#include <stddef.h>
int lics_runtime_init(void);
void lics_runtime_shutdown(void);
int lics_runtime_command(const char *name,const irc_event *event,char *reply,size_t reply_size);
int lics_runtime_event(const char *name,const irc_event *event,char *reply,size_t reply_size);
int lics_runtime_bind_events(event_registry *registry);
#endif
