#ifndef BOT_TIMER_HANDLERS_H
#define BOT_TIMER_HANDLERS_H
#include "timers.h"
#define BOT_TIMER_HANDLER_NAME_MAX 64
typedef int (*bot_timer_dispatch_fn)(const char *handler,bot_timer_id id,void *user);
typedef struct { bot_timer_dispatch_fn dispatch; void *user; } bot_timer_dispatcher;
typedef struct { char name[BOT_TIMER_HANDLER_NAME_MAX]; bot_timer_dispatcher dispatcher; } bot_timer_binding;
int bot_timer_bind(bot_timer_binding *binding,const char *name,const bot_timer_dispatcher *dispatcher);
int bot_timer_dispatch(const bot_timer_binding *binding,bot_timer_id id);
#endif
