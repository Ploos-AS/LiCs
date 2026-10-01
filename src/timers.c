#include "timers.h"
#include <string.h>
void bot_timer_registry_init(bot_timer_registry *r){if(r)memset(r,0,sizeof(*r));if(r)r->next_id=1;}
bot_timer_id bot_timer_add(bot_timer_registry *r,uint64_t delay,uint64_t interval,int repeat,bot_timer_fn fn,void *user){bot_timer *t;if(!r||!fn||r->count>=BOT_MAX_TIMERS)return 0;if(!repeat)interval=0;t=&r->timers[r->count++];t->id=r->next_id++;if(!t->id)t->id=r->next_id++;t->due_ms=r->now_ms+delay;t->interval_ms=interval;t->repeat=repeat!=0;t->handler=fn;t->user=user;t->active=1;return t->id;}
int bot_timer_cancel(bot_timer_registry *r,bot_timer_id id){size_t i;if(!r||!id)return 0;for(i=0;i<r->count;i++)if(r->timers[i].active&&r->timers[i].id==id){r->timers[i].active=0;return 1;}return 0;}
size_t bot_timer_poll(bot_timer_registry *r,uint64_t now){size_t i,fired=0;if(!r)return 0;r->now_ms=now;for(i=0;i<r->count;i++){bot_timer *t=&r->timers[i];if(!t->active||t->due_ms>now)continue;fired++;if(t->handler(t->id,t->user)!=0){t->active=0;continue;}if(t->repeat&&t->interval_ms){do{t->due_ms+=t->interval_ms;}while(t->due_ms<=now);}else t->active=0;}return fired;}
size_t bot_timer_count(const bot_timer_registry *r){size_t i,n=0;if(!r)return 0;for(i=0;i<r->count;i++)if(r->timers[i].active)n++;return n;}
