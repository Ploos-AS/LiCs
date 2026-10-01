#include "../src/bot_state.h"
#include "../src/bot_state_file_save.h"
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
int main(void){char p[256];snprintf(p,sizeof(p),"/tmp/bstate-durable-%ld.db",(long)getpid());bot_state*s=bot_state_create();assert(s);assert(bot_state_set(s,"user:alice","visits","42"));assert(bot_state_file_save(p,s));assert(access(p,F_OK)==0);remove(p);bot_state_destroy(s);return 0;}