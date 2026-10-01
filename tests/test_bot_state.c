#include "../src/bot_state.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "../src/irc_core.h"
int main(void){
 char scope[256]; irc_event ev={0}; snprintf(ev.nick,sizeof(ev.nick),"alice"); snprintf(ev.target,sizeof(ev.target),"#ploos"); char escope[256]; assert(bot_state_scope_event_user(&ev,escope,sizeof(escope))); assert(strcmp(escope,"user:alice")==0); assert(bot_state_scope_event_target(&ev,escope,sizeof(escope))); assert(strcmp(escope,"channel:#ploos")==0); bot_state *s=bot_state_create(); assert(bot_state_scope_user("alice",scope,sizeof(scope))); assert(strcmp(scope,"user:alice")==0); assert(bot_state_scope_channel("#ploos",scope,sizeof(scope))); assert(strcmp(scope,"channel:#ploos")==0); assert(s); assert(bot_state_count(s)==0);
 assert(bot_state_set(s,"user:alice","visits","1")); assert(strcmp(bot_state_get(s,"user:alice","visits"),"1")==0);
 assert(bot_state_set(s,"user:alice","visits","2")); assert(strcmp(bot_state_get(s,"user:alice","visits"),"2")==0);
 assert(bot_state_set(s,"channel:#ploos","topic","retro")); assert(strcmp(bot_state_get(s,"channel:#ploos","topic"),"retro")==0);
 assert(bot_state_get(s,"user:bob","visits")==NULL); assert(bot_state_count(s)==2);
 assert(bot_state_delete(s,"user:alice","visits")); assert(bot_state_get(s,"user:alice","visits")==NULL); assert(bot_state_count(s)==1);
 bot_state_clear(s); assert(bot_state_count(s)==0); assert(bot_state_get(s,"channel:#ploos","topic")==NULL);
 bot_state_destroy(s); return 0;
}