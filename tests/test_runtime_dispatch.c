#include "../src/irc_core.h"
#include "../src/runtime_adapter.h"
#include "../src/runtime_dispatch.h"
#include <stdio.h>
#include <string.h>
int main(void){
 irc_event e; char reply[128]={0};
 if (lics_runtime_init()!=0) return 1;
 if (irc_parse_line(":alice!u@h PRIVMSG #ploos :!hello\\r\\n",&e)!=1) return 2;
 if (!lics_dispatch_runtime(&e,reply,sizeof(reply))) return 3;
 if (strcmp(reply,"Hello from LiCs")!=0) return 4;
 lics_runtime_shutdown(); puts("runtime IRC command: PASS"); return 0;
}
