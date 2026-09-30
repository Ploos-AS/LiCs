#include "runtime_dispatch.h"
#include "runtime_adapter.h"
#include <string.h>
int lics_dispatch_runtime(const irc_event *event,char *reply,size_t rs){const char *command;if(!event||event->type!=IRC_EVENT_PRIVMSG||!reply||!rs)return 0;command=event->text;if(*command=='!')command++;if(strcmp(command,"hello")!=0)return 0;return lics_runtime_command("hello",event,reply,rs);}
static const char *event_name(irc_event_type t){switch(t){case IRC_EVENT_JOIN:return "join";case IRC_EVENT_PART:return "part";case IRC_EVENT_NICK:return "nick";case IRC_EVENT_QUIT:return "quit";case IRC_EVENT_NOTICE:return "notice";default:return NULL;}}
int lics_dispatch_event_runtime(const irc_event *event,char *reply,size_t rs){const char *name=event_name(event?event->type:IRC_EVENT_NONE);if(!name)return 0;return lics_runtime_event(name,event,reply,rs);}
