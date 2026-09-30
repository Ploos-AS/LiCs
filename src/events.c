#include "events.h"
#include <string.h>
void event_registry_init(event_registry *r){if(r)memset(r,0,sizeof(*r));}
int event_registry_register(event_registry *r,irc_event_type type,bot_event_fn fn,void *user){
 if(!r||type==IRC_EVENT_NONE||!fn||r->count>=BOT_MAX_EVENT_HANDLERS)return 0;
 r->entries[r->count].type=type;r->entries[r->count].handler=fn;r->entries[r->count].user=user;r->count++;return 1;
}
int event_registry_unregister(event_registry *r,irc_event_type type,bot_event_fn fn){
 size_t i;if(!r||!fn)return 0;
 for(i=0;i<r->count;i++)if(r->entries[i].type==type&&r->entries[i].handler==fn){
  if(i+1<r->count)memmove(&r->entries[i],&r->entries[i+1],(r->count-i-1)*sizeof(r->entries[0]));
  r->count--;return 1;
 }return 0;
}
int bot_dispatch_event(const irc_event *e,const event_registry *r){
 size_t i;int handled=0;if(!e||!r)return -1;
 for(i=0;i<r->count;i++)if(r->entries[i].type==e->type&&r->entries[i].handler)
   if(r->entries[i].handler(e,r->entries[i].user))handled=1;
 return handled;
}
