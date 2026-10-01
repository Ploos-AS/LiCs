#include "scheme_backend.h"
#ifdef LICS_WITH_TINYSCHEME
#include "scheme.h"
#include "runtime_adapter.h"
#include <stdio.h>
#include <string.h>
static scheme *sc;
static irc_output_sink lics_sink;
static char *active_reply;
static size_t active_reply_size;
#define LICS_TIMER_NAME_MAX 64
typedef struct { char name[LICS_TIMER_NAME_MAX]; bot_timer_id id; int active; } scheme_timer_context;
static scheme_timer_context timer_contexts[BOT_MAX_TIMERS];
static char event_handlers[8][64];
static char command_names[32][64];
static char command_handlers[32][64];

static pointer arg_string(scheme *r,pointer a,int i){while(i--&&a!=r->NIL)a=cdr(a);if(a==r->NIL)return NULL;pointer p=pair_car(a);return is_string(p)?p:NULL;}
static int valid_timer_name(const char *s){size_t i;if(!s||!*s)return 0;for(i=0;s[i];++i)if(!((s[i]>='a'&&s[i]<='z')||(s[i]>='A'&&s[i]<='Z')||(s[i]>='0'&&s[i]<='9')||s[i]=='_'||s[i]=='-'))return 0;return i<LICS_TIMER_NAME_MAX;}
static int lics_scheme_timer_cb(bot_timer_id id,void *user){char expr[96];scheme_timer_context *ctx=(scheme_timer_context *)user;(void)id;if(!sc||!ctx||!ctx->active||!valid_timer_name(ctx->name))return -1;snprintf(expr,sizeof(expr),"(%s)",ctx->name);return scheme_load_string(sc,expr)==0?0:-1;}

static int event_type_from_name(const char *name){if(!name)return 0;if(strcmp(name,"join")==0)return IRC_EVENT_JOIN;if(strcmp(name,"part")==0)return IRC_EVENT_PART;if(strcmp(name,"nick")==0)return IRC_EVENT_NICK;if(strcmp(name,"quit")==0)return IRC_EVENT_QUIT;if(strcmp(name,"notice")==0)return IRC_EVENT_NOTICE;if(strcmp(name,"privmsg")==0)return IRC_EVENT_PRIVMSG;return 0;}
static pointer command_on(scheme *r,pointer a){pointer n=arg_string(r,a,0),h=arg_string(r,a,1);size_t i;if(!n||!h)return r->F;if(strlen(string_value(n))>=64||strlen(string_value(h))>=64)return r->F;for(i=0;i<32;i++){if(!command_names[i][0]||strcmp(command_names[i],string_value(n))==0){snprintf(command_names[i],64,"%s",string_value(n));snprintf(command_handlers[i],64,"%s",string_value(h));return r->T;}}return r->F;}
static pointer event_on(scheme *r,pointer a){pointer n=arg_string(r,a,0),h=arg_string(r,a,1);int t;if(!n||!h)return r->F;t=event_type_from_name(string_value(n));if(!t||strlen(string_value(h))>=sizeof(event_handlers[t]))return r->F;snprintf(event_handlers[t],sizeof(event_handlers[t]),"%s",string_value(h));return r->T;}
static pointer timer_after(scheme *r,pointer a){pointer d,n;long ms;bot_timer_id id;size_t slot;scheme_timer_context *ctx=NULL;d=arg_string(r,a,0);n=arg_string(r,a,1);if(!d||!n||!valid_timer_name(string_value(n))||!is_number(d))return r->F;ms=ivalue(d);if(ms<0)return r->F;for(slot=0;slot<BOT_MAX_TIMERS;slot++)if(!timer_contexts[slot].active){ctx=&timer_contexts[slot];break;}if(!ctx)return r->F;snprintf(ctx->name,LICS_TIMER_NAME_MAX,"%s",string_value(n));ctx->active=1;id=lics_runtime_timer_after((uint64_t)ms,lics_scheme_timer_cb,ctx);if(!id){ctx->active=0;return r->F;}ctx->id=id;return mk_integer(r,(long)id);}
static pointer timer_every(scheme *r,pointer a){pointer d,n;long ms;bot_timer_id id;size_t slot;scheme_timer_context *ctx=NULL;d=arg_string(r,a,0);n=arg_string(r,a,1);if(!d||!n||!valid_timer_name(string_value(n))||!is_number(d))return r->F;ms=ivalue(d);if(ms<=0)return r->F;for(slot=0;slot<BOT_MAX_TIMERS;slot++)if(!timer_contexts[slot].active){ctx=&timer_contexts[slot];break;}if(!ctx)return r->F;snprintf(ctx->name,LICS_TIMER_NAME_MAX,"%s",string_value(n));ctx->active=1;id=lics_runtime_timer_every((uint64_t)ms,lics_scheme_timer_cb,ctx);if(!id){ctx->active=0;return r->F;}ctx->id=id;return mk_integer(r,(long)id);}
static pointer timer_cancel(scheme *r,pointer a){pointer p;bot_timer_id id;size_t i;if(a==r->NIL)return r->F;p=pair_car(a);if(!is_number(p))return r->F;id=(bot_timer_id)ivalue(p);if(!lics_runtime_timer_cancel(id))return r->F;for(i=0;i<BOT_MAX_TIMERS;i++)if(timer_contexts[i].active&&timer_contexts[i].id==id){timer_contexts[i].active=0;break;}return r->T;}
static pointer irc_say(scheme *r,pointer a){pointer t=arg_string(r,a,0),x=arg_string(r,a,1);return t&&x&&irc_send_privmsg(&lics_sink,string_value(t),string_value(x))?r->T:r->F;}
static pointer irc_notice(scheme *r,pointer a){pointer t=arg_string(r,a,0),x=arg_string(r,a,1);return t&&x&&irc_send_notice(&lics_sink,string_value(t),string_value(x))?r->T:r->F;}
static pointer irc_join(scheme *r,pointer a){pointer c=arg_string(r,a,0);return c&&irc_send_join(&lics_sink,string_value(c))?r->T:r->F;}
static pointer irc_part(scheme *r,pointer a){pointer c=arg_string(r,a,0),x=arg_string(r,a,1);return c&&x&&irc_send_part(&lics_sink,string_value(c),string_value(x))?r->T:r->F;}
static pointer irc_reply(scheme *r,pointer args){pointer arg;if(!active_reply||!active_reply_size||args==r->NIL)return r->NIL;arg=pair_car(args);if(!is_string(arg))return r->NIL;snprintf(active_reply,active_reply_size,"%s",string_value(arg));return r->T;}

int scheme_backend_init(void){memset(timer_contexts,0,sizeof(timer_contexts));memset(event_handlers,0,sizeof(event_handlers));memset(command_names,0,sizeof(command_names));memset(command_handlers,0,sizeof(command_handlers));sc=scheme_init_new();if(!sc)return -1;scheme_define(sc,sc->global_env,mk_symbol(sc,"irc-reply"),mk_foreign_func(sc,irc_reply));scheme_define(sc,sc->global_env,mk_symbol(sc,"irc-say"),mk_foreign_func(sc,irc_say));scheme_define(sc,sc->global_env,mk_symbol(sc,"irc-notice"),mk_foreign_func(sc,irc_notice));scheme_define(sc,sc->global_env,mk_symbol(sc,"irc-join"),mk_foreign_func(sc,irc_join));scheme_define(sc,sc->global_env,mk_symbol(sc,"irc-part"),mk_foreign_func(sc,irc_part));scheme_define(sc,sc->global_env,mk_symbol(sc,"timer-after"),mk_foreign_func(sc,timer_after));scheme_define(sc,sc->global_env,mk_symbol(sc,"timer-every"),mk_foreign_func(sc,timer_every));scheme_define(sc,sc->global_env,mk_symbol(sc,"timer-cancel"),mk_foreign_func(sc,timer_cancel));scheme_define(sc,sc->global_env,mk_symbol(sc,"event-on"),mk_foreign_func(sc,event_on));scheme_define(sc,sc->global_env,mk_symbol(sc,"command-on"),mk_foreign_func(sc,command_on));if(scheme_load_string(sc,"(define (hello nick) (irc-reply \"Hello from LiCs\"))\n(define (on-command name nick) (if (string=? name \"hello\") (hello nick) #f))\n(define (on-event name nick target) (if (string=? name \"join\") (begin (irc-say target \"Welcome from LiCs\") (irc-reply \"Welcome from LiCs\")) #f))\n")!=0){scheme_deinit(sc);sc=NULL;return -1;}return 0;}
void scheme_backend_shutdown(void){if(sc)scheme_deinit(sc);sc=NULL;}
int scheme_backend_eval(const char *expr){if(!sc||!expr)return -1;return scheme_load_string(sc,expr);}
int scheme_backend_command(const char *symbol,const irc_event *event,char *reply,size_t rs){char expr[256];int rc;size_t i;if(!sc||!symbol||!event||!reply||!rs)return 0;active_reply=reply;active_reply_size=rs;reply[0]='\0';for(i=0;i<32;i++)if(command_names[i][0]&&strcmp(command_names[i],symbol)==0){snprintf(expr,sizeof(expr),"(%s \\"%s\\")",command_handlers[i],event->nick);rc=scheme_load_string(sc,expr);active_reply=NULL;active_reply_size=0;return rc==0&&reply[0]!='\0';}snprintf(expr,sizeof(expr),"(on-command \\"%s\\" \\"%s\\")",symbol,event->nick);rc=scheme_load_string(sc,expr);active_reply=NULL;active_reply_size=0;return rc==0&&reply[0]!='\0';}
int scheme_backend_event(const char *name,const irc_event *event,char *reply,size_t rs){char expr[512];int rc,t;if(!sc||!name||!event||!reply||!rs)return 0;active_reply=reply;active_reply_size=rs;reply[0]='\\0';t=event_type_from_name(name);if(t&&event_handlers[t][0])snprintf(expr,sizeof(expr),"(%s)",event_handlers[t]);else snprintf(expr,sizeof(expr),"(on-event \\"%s\\" \\"%s\\" \\"%s\\")",name,event->nick,event->target);rc=scheme_load_string(sc,expr);active_reply=NULL;active_reply_size=0;return rc==0&&reply[0]!='\\0';}
#else
void scheme_backend_set_output_sink(const irc_output_sink *sink){(void)sink;}
int scheme_backend_init(void){return -1;} void scheme_backend_shutdown(void){}
int scheme_backend_eval(const char *expr){(void)expr;return -1;}
int scheme_backend_command(const char *s,const irc_event *e,char *r,size_t n){(void)s;(void)e;(void)r;(void)n;return 0;}
int scheme_backend_event(const char *s,const irc_event *e,char *r,size_t n){(void)s;(void)e;(void)r;(void)n;return 0;}
#endif
