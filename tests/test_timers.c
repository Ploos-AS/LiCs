#include "../src/timers.h"
#include <stdio.h>
static int hits;
static int cb(bot_timer_id id,void *u){(void)id;(void)u;hits++;return 0;}
int main(void){bot_timer_registry r;bot_timer_registry_init(&r);bot_timer_id a=bot_timer_add(&r,1000,0,0,cb,0);bot_timer_id b=bot_timer_add(&r,500,500,1,cb,0);if(!a||!b)return 1;if(bot_timer_poll(&r,499)!=0)return 2;if(bot_timer_poll(&r,500)!=1||hits!=1)return 3;if(bot_timer_poll(&r,1000)!=1||hits!=2)return 4;if(bot_timer_poll(&r,1500)!=1||hits!=3)return 5;if(bot_timer_cancel(&r,b)!=1)return 6;if(bot_timer_poll(&r,2000)!=0)return 7;if(bot_timer_count(&r)!=0)return 8;puts("timer registry: PASS");return 0;}