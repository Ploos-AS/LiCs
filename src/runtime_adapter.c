#include "runtime_adapter.h"
#include "scheme_backend.h"
#include <stdio.h>
#include <string.h>
static int initialized, scheme_active;
int lics_runtime_init(void) {
#ifdef LICS_WITH_TINYSCHEME
    scheme_active=(scheme_backend_init()==0);
#else
    scheme_active=0;
#endif
    initialized=1; return 0;
}
void lics_runtime_shutdown(void){if(scheme_active)scheme_backend_shutdown();scheme_active=0;initialized=0;}
int lics_runtime_command(const char *symbol,const irc_event *event,char *reply,size_t rs){if(!initialized||!symbol||!reply||!rs)return 0;if(scheme_active&&scheme_backend_command(symbol,event,reply,rs))return 1;if(strcmp(symbol,"hello")==0){snprintf(reply,rs,"Hello from LiCs");return 1;}return 0;}
int lics_runtime_event(const char *name,const irc_event *event,char *reply,size_t rs){if(!initialized||!name||!event||!reply||!rs)return 0;if(scheme_active&&scheme_backend_event(name,event,reply,rs))return 1;return 0;}

static int lics_event_bridge(const irc_event *event, void *user) {
    char reply[512];
    const char *name = (const char *)user;
    if (!event || !name) return 0;
    return lics_runtime_event(name, event, reply, sizeof(reply));
}

int lics_runtime_bind_events(event_registry *registry) {
    static const char *names[] = {"join","part","nick","quit","notice"};
    static const irc_event_type types[] = {
        IRC_EVENT_JOIN, IRC_EVENT_PART, IRC_EVENT_NICK,
        IRC_EVENT_QUIT, IRC_EVENT_NOTICE
    };
    size_t i;
    if (!registry) return 0;
    for (i = 0; i < 5; ++i) {
        if (!event_registry_register(registry, types[i], lics_event_bridge,
                                      (void *)names[i])) return 0;
    }
    return 1;
}

static irc_output_sink lics_sink;
void lics_runtime_set_output_sink(const irc_output_sink *sink){if(sink) lics_sink=*sink; else memset(&lics_sink,0,sizeof(lics_sink));}
int lics_runtime_say(const char *target,const char *text){return irc_send_privmsg(&lics_sink,target,text);}
int lics_runtime_notice(const char *target,const char *text){return irc_send_notice(&lics_sink,target,text);}
int lics_runtime_join(const char *channel){return irc_send_join(&lics_sink,channel);}
int lics_runtime_part(const char *channel,const char *reason){return irc_send_part(&lics_sink,channel,reason);}
