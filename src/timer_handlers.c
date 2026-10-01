#include "timer_handlers.h"
#include <string.h>
int bot_timer_bind(bot_timer_binding *b,const char *name,const bot_timer_dispatcher *d){if(!b||!name||!d||!d->dispatch||strlen(name)>=BOT_TIMER_HANDLER_NAME_MAX)return 0;memset(b,0,sizeof(*b));strcpy(b->name,name);b->dispatcher=*d;return 1;}
int bot_timer_dispatch(const bot_timer_binding *b,bot_timer_id id){if(!b||!b->dispatcher.dispatch)return 0;return b->dispatcher.dispatch(b->name,id,b->dispatcher.user);}
