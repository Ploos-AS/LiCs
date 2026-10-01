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
 lics_runtime_shutdown(); puts("LiCs Scheme timer integration: PASS"); return 0;
}
