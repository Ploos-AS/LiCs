#include "../src/bot_state.h"
#include <assert.h>
#include <string.h>
int main(void){
 bot_state *s=bot_state_create(); assert(s); assert(bot_state_count(s)==0);
 assert(bot_state_set(s,"user:alice","visits","1")); assert(strcmp(bot_state_get(s,"user:alice","visits"),"1")==0);
 assert(bot_state_set(s,"user:alice","visits","2")); assert(strcmp(bot_state_get(s,"user:alice","visits"),"2")==0);
 assert(bot_state_set(s,"channel:#ploos","topic","retro")); assert(strcmp(bot_state_get(s,"channel:#ploos","topic"),"retro")==0);
 assert(bot_state_get(s,"user:bob","visits")==NULL); assert(bot_state_count(s)==2);
 assert(bot_state_delete(s,"user:alice","visits")); assert(bot_state_get(s,"user:alice","visits")==NULL); assert(bot_state_count(s)==1);
 bot_state_clear(s); assert(bot_state_count(s)==0); assert(bot_state_get(s,"channel:#ploos","topic")==NULL);
 bot_state_destroy(s); return 0;
}