#ifndef LICS_RUNTIME_ADAPTER_H
#define LICS_RUNTIME_ADAPTER_H
#include "irc_core.h"
#include "events.h"
#include "irc_output_sink.h"
#include <stddef.h>
int lics_runtime_init(void);
void lics_runtime_shutdown(void);
int lics_runtime_command(const char *name,const irc_event *event,char *reply,size_t reply_size);
int lics_runtime_event(const char *name,const irc_event *event,char *reply,size_t reply_size);
int lics_runtime_bind_events(event_registry *registry);
void lics_runtime_set_output_sink(const irc_output_sink *sink);
int lics_runtime_say(const char *target,const char *text);
int lics_runtime_notice(const char *target,const char *text);
int lics_runtime_join(const char *channel);
int lics_runtime_part(const char *channel,const char *reason);
#endif
