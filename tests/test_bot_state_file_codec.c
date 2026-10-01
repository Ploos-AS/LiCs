#include "../src/bot_state.h"
#include "../src/bot_state_file_codec.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static int eq(const char*s,const char*k,const char*v,void*ctx){bot_state*t=ctx;return strcmp(bot_state_get(t,s,k),v)==0;}
int main(void){bot_state*a=bot_state_create(),*b=bot_state_create();FILE*f=tmpfile();assert(a&&b&&f);assert(bot_state_set(a,"user:alice","visits","42"));assert(bot_state_set(a,"channel:#ploos","topic","test"));assert(bot_state_file_write(f,a));rewind(f);assert(bot_state_file_read(f,b));assert(bot_state_foreach(a,eq,b));assert(bot_state_count(a)==bot_state_count(b));fclose(f);bot_state_destroy(a);bot_state_destroy(b);return 0;}