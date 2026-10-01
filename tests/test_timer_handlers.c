#include "../src/timer_handlers.h"
#include <stdio.h>
#include <string.h>
static int hits; static char got[64];
static int dispatch(const char *name,bot_timer_id id,void *u){(void)id;(void)u;snprintf(got,sizeof(got),"%s",name);hits++;return 0;}
int main(void){bot_timer_binding b;bot_timer_dispatcher d={dispatch,NULL};if(!bot_timer_bind(&b,"heartbeat",&d))return 1;if(!bot_timer_dispatch(&b,7)||hits!=1||strcmp(got,"heartbeat"))return 2;puts("timer handler binding: PASS");return 0;}
