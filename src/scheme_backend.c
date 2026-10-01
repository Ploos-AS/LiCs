#include "scheme_backend.h"
#ifdef LICS_WITH_TINYSCHEME
#include "scheme.h"
#include <stdio.h>
#include <string.h>
static scheme *sc;
static irc_output_sink lics_sink;
void scheme_backend_set_output_sink(const irc_output_sink *sink){if(sink)lics_sink=*sink;else memset(&lics_sink,0,sizeof(lics_sink));}
static char *active_reply; static size_t active_reply_size;
static pointer arg_string(scheme *r,pointer a,int i){while(i--&&a!=r->NIL)a=cdr(a);if(a==r->NIL)return NULL;pointer p=pair_car(a);return is_string(p)?p:NULL;}
static pointer irc_say(scheme *r,pointer a){pointer t=arg_string(r,a,0),x=arg_string(r,a,1);return t&&x&&irc_send_privmsg(&lics_sink,string_value(t),string_value(x))?r->T:r->F;}
static pointer irc_notice(scheme *r,pointer a){pointer t=arg_string(r,a,0),x=arg_string(r,a,1);return t&&x&&irc_send_notice(&lics_sink,string_value(t),string_value(x))?r->T:r->F;}
static pointer irc_join(scheme *r,pointer a){pointer c=arg_string(r,a,0);return c&&irc_send_join(&lics_sink,string_value(c))?r->T:r->F;}
static pointer irc_part(scheme *r,pointer a){pointer c=arg_string(r,a,0),x=arg_string(r,a,1);return c&&x&&irc_send_part(&lics_sink,string_value(c),string_value(x))?r->T:r->F;}
static pointer irc_reply(scheme *runtime,pointer args){pointer arg;if(!active_reply||!active_reply_size||args==runtime->NIL)return runtime->NIL;arg=pair_car(args);if(!is_string(arg))return runtime->NIL;snprintf(active_reply,active_reply_size,"%s",string_value(arg));return runtime->T;}
int scheme_backend_init(void){sc=scheme_init_new();if(!sc)return -1;scheme_define(sc,sc->global_env,mk_symbol(sc,"irc-reply"),mk_foreign_func(sc,irc_reply));scheme_define(sc,sc->global_env,mk_symbol(sc,"irc-say"),mk_foreign_func(sc,irc_say));scheme_define(sc,sc->global_env,mk_symbol(sc,"irc-notice"),mk_foreign_func(sc,irc_notice));scheme_define(sc,sc->global_env,mk_symbol(sc,"irc-join"),mk_foreign_func(sc,irc_join));scheme_define(sc,sc->global_env,mk_symbol(sc,"irc-part"),mk_foreign_func(sc,irc_part));if(scheme_load_string(sc,"(define (hello nick) (irc-reply "Hello from LiCs"))\n(define (on-command name nick) (if (string=? name "hello") (hello nick) #f))\n(define (on-event name nick target) (if (string=? name "join") (irc-reply "Welcome from LiCs") #f))\n")!=0){scheme_deinit(sc);sc=NULL;return -1;}return 0;}
void scheme_backend_shutdown(void){if(sc)scheme_deinit(sc);sc=NULL;}
int scheme_backend_command(const char *symbol,const irc_event *event,char *reply,size_t rs){char expr[256];int rc;if(!sc||!symbol||!event||!reply||!rs)return 0;active_reply=reply;active_reply_size=rs;reply[0]='\0';snprintf(expr,sizeof(expr),"(on-command "%s" "%s")",symbol,event->nick);rc=scheme_load_string(sc,expr);active_reply=NULL;active_reply_size=0;return rc==0&&reply[0]!='\0';}
int scheme_backend_event(const char *name,const irc_event *event,char *reply,size_t rs){char expr[512];int rc;if(!sc||!name||!event||!reply||!rs)return 0;active_reply=reply;active_reply_size=rs;reply[0]='\0';snprintf(expr,sizeof(expr),"(on-event "%s" "%s" "%s")",name,event->nick,event->target);rc=scheme_load_string(sc,expr);active_reply=NULL;active_reply_size=0;return rc==0&&reply[0]!='\0';}
#else
void scheme_backend_set_output_sink(const irc_output_sink *sink){(void)sink;}
int scheme_backend_init(void){return -1;} void scheme_backend_shutdown(void){}
int scheme_backend_command(const char *s,const irc_event *e,char *r,size_t n){(void)s;(void)e;(void)r;(void)n;return 0;}
int scheme_backend_event(const char *s,const irc_event *e,char *r,size_t n){(void)s;(void)e;(void)r;(void)n;return 0;}
#endif
