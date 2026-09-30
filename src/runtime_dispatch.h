#ifndef LICS_RUNTIME_DISPATCH_H
#define LICS_RUNTIME_DISPATCH_H
#include "irc_core.h"
#include <stddef.h>
int lics_dispatch_runtime(const irc_event *event,char *reply,size_t reply_size);
int lics_dispatch_event_runtime(const irc_event *event,char *reply,size_t reply_size);
#endif
