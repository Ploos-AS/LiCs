#include "../src/irc_core.h"
#include "../src/runtime_adapter.h"
#include "../src/runtime_dispatch.h"
#include "../src/scheme_backend.h"
#include <stdio.h>
#include <string.h>
int main(void){
 irc_event e; char reply[128]={0};
 if(lics_runtime_init()!=0) return 1;
 if(scheme_backend_eval("(define (tick) (irc-reply \"tick\"))")!=0) return 2;
 if(scheme_backend_eval("(define timer-id (timer-every 20 \"tick\"))")!=0) return 3;
 if(lics_runtime_timer_poll(19)!=0) return 4;
 if(lics_runtime_timer_poll(20)!=1) return 5;
 if(lics_runtime_timer_poll(40)!=1) return 6;
 if(scheme_backend_eval("(timer-cancel timer-id)")!=0) return 7;
 if(lics_runtime_timer_poll(60)!=0) return 8;
 if(irc_parse_line(":alice!u@h PRIVMSG #ploos :!hello\r\n",&e)!=1) return 9;
 if(!lics_dispatch_runtime(&e,reply,sizeof(reply))) return 10;
 if(strcmp(reply,"Hello from LiCs")!=0) return 11;
 if(scheme_backend_eval("(define (argcmd) (irc-reply (string-append (irc-command) \":\" (irc-args))))")!=0) return 26;
 if(scheme_backend_eval("(command-on \"!args\" \"argcmd\")")!=0) return 27;
 memset(reply,0,sizeof(reply)); memset(&e,0,sizeof(e)); if(irc_parse_line(":alice!u@h PRIVMSG #ploos :!args Per Ola\\r\\n",&e)!=1)return 28; if(!lics_dispatch_runtime(&e,reply,sizeof(reply)))return 29; if(strcmp(reply,"args:Per Ola")!=0)return 30;
 if(scheme_backend_eval("(define (cmdhello nick) (irc-reply \"Registered COMMAND\"))")!=0) return 16;
 if(scheme_backend_eval("(command-on \"!hello2\" \"cmdhello\")")!=0) return 17;
 memset(reply,0,sizeof(reply)); memset(&e,0,sizeof(e)); if(irc_parse_line(":alice!u@h PRIVMSG #ploos :!hello2\\r\\n",&e)!=1) return 18; if(!lics_dispatch_runtime(&e,reply,sizeof(reply))) return 19; if(strcmp(reply,"Registered COMMAND")!=0) return 20;
 if(scheme_backend_eval("(define (ctxcmd) (irc-reply (irc-text)))")!=0) return 21;
 if(scheme_backend_eval("(command-on \"!ctx\" \"ctxcmd\")")!=0) return 22;
 memset(reply,0,sizeof(reply)); if(irc_parse_line(":alice!u@h PRIVMSG #ploos :!ctx Per Ola\\r\\n",&e)!=1)return 23; if(!lics_dispatch_runtime(&e,reply,sizeof(reply)))return 24; if(strcmp(reply,"!ctx Per Ola")!=0)return 25;
 if(scheme_backend_eval("(event-on \"join\" \"welcome\")")!=0) return 12;
 if(scheme_backend_eval("(define (welcome) (irc-reply \"Registered JOIN\"))")!=0) return 13;
 memset(&e,0,sizeof(e)); e.type=IRC_EVENT_JOIN; snprintf(e.nick,sizeof(e.nick),"alice"); snprintf(e.target,sizeof(e.target),"#ploos");
 memset(reply,0,sizeof(reply)); if(!lics_dispatch_event_runtime(&e,reply,sizeof(reply))) return 14; if(strcmp(reply,"Registered JOIN")!=0) return 15;
 lics_runtime_shutdown(); puts("LiCs Scheme timer/event integration: PASS"); return 0;
}
