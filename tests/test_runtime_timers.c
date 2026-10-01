#include "../src/runtime_adapter.h"
#include <stdio.h>
static int fired;
static int cb(bot_timer_id id,void *u){(void)id;(void)u;fired++;return 0;}
int main(void){char reply[128]={0};if(lics_runtime_init()!=0)return 1;/* Native registry lifecycle is qualified here; Scheme binding is qualified by the backend when enabled. */if(lics_runtime_timer_after(10,cb,NULL)==0)return 2;if(lics_runtime_timer_poll(9)!=0)return 3;if(lics_runtime_timer_poll(10)!=1||fired!=1)return 4;if(lics_runtime_timer_every(20,cb,NULL)==0)return 5;if(lics_runtime_timer_poll(29)!=0)return 6;if(lics_runtime_timer_poll(30)!=1||fired!=2)return 7;if(lics_runtime_timer_poll(50)!=1||fired!=3)return 8;lics_runtime_shutdown();puts("LiCs runtime timers: PASS");return 0;}