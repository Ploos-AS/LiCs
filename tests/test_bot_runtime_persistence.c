#include "../src/bot_runtime.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(void){const char*p="/tmp/bot-runtime-e2e.db";remove(p);bot_runtime*a=bot_runtime_create();assert(a);assert(bot_runtime_attach_file_state(a,p));assert(bot_state_set(a->state,"user:alice","visits","42"));assert(strcmp(bot_state_get(a->state,"user:alice","visits"),"42")==0);bot_runtime_destroy(a);bot_runtime*b=bot_runtime_create();assert(b);assert(bot_runtime_attach_file_state(b,p));assert(strcmp(bot_state_get(b->state,"user:alice","visits"),"42")==0);bot_runtime_destroy(b);remove(p);return 0;}