#include "../src/bot_state.h"
#include "../src/bot_state_file_save.h"
#include "../src/bot_state_file_recovery.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(void){const char*p="/tmp/bstate-recovery.db",*t="/tmp/bstate-recovery.db.tmp";bot_state*a=bot_state_create(),*b=bot_state_create();assert(a&&b);assert(bot_state_set(a,"user:alice","visits","42"));assert(bot_state_file_save(p,a));FILE*f=fopen(t,"wb");assert(f);assert(bot_state_file_write(f,a));fclose(f);f=fopen(p,"wb");assert(f);fputs("BROKEN",f);fclose(f);assert(bot_state_file_recover(p,b));assert(strcmp(bot_state_get(b,"user:alice","visits"),"42")==0);remove(p);remove(t);bot_state_destroy(a);bot_state_destroy(b);return 0;}