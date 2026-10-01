#include "../src/bot_state.h"
#include "../src/bot_state_file_save.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(void){const char*p="/tmp/bstate-test.db";bot_state*a=bot_state_create(),*b=bot_state_create();assert(a&&b);assert(bot_state_set(a,"user:alice","visits","42"));assert(bot_state_file_save(p,a));assert(bot_state_file_load(p,b));assert(strcmp(bot_state_get(b,"user:alice","visits"),"42")==0);FILE*f=fopen(p,"wb");assert(f);fputs("BROKEN",f);fclose(f);assert(!bot_state_file_load(p,b));remove(p);bot_state_destroy(a);bot_state_destroy(b);return 0;}