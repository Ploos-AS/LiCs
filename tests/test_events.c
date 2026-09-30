#include "../src/irc_core.h"
#include "../src/events.h"
#include <stdio.h>
#include <string.h>
static int joins=0, parts=0;
static int on_join(const irc_event *e, void *u){(void)u;if(e->type==IRC_EVENT_JOIN&&strcmp(e->target,"#ploos")==0)joins++;return 1;}
static int on_part(const irc_event *e, void *u){(void)u;if(e->type==IRC_EVENT_PART&&strcmp(e->target,"#ploos")==0)parts++;return 1;}
int main(void){
 irc_event e; event_registry r; event_registry_init(&r);
 if(!event_registry_register(&r,IRC_EVENT_JOIN,on_join,NULL))return 1;
 if(!event_registry_register(&r,IRC_EVENT_PART,on_part,NULL))return 2;
 if(irc_parse_line(":alice!u@h JOIN #ploos\r\n",&e)!=1)return 3;
 if(!bot_dispatch_event(&e,&r))return 4;
 if(irc_parse_line(":alice!u@h PART #ploos :bye\r\n",&e)!=1)return 5;
 if(!bot_dispatch_event(&e,&r))return 6;
 if(joins!=1||parts!=1)return 7;
 if(!event_registry_unregister(&r,IRC_EVENT_JOIN,on_join))return 8;
 if(irc_parse_line(":bob!u@h JOIN #ploos\r\n",&e)!=1)return 9;
 if(bot_dispatch_event(&e,&r)!=0)return 10;
 puts("event registry: PASS"); return 0;
}
