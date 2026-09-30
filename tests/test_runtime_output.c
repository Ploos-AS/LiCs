#include "../src/runtime_adapter.h"
#include <stdio.h>
#include <string.h>
static char line[1024];
static int sink(const char *s, void *u){(void)u;snprintf(line,sizeof(line),"%s",s);return 1;}
int main(void){
 irc_output_sink out={sink,NULL};
 if(lics_runtime_init()!=0)return 1;
 lics_runtime_set_output_sink(&out);
 if(!lics_runtime_say("#ploos","hello")||strcmp(line,"PRIVMSG #ploos :hello\r\n"))return 2;
 if(!lics_runtime_notice("alice","hi")||strcmp(line,"NOTICE alice :hi\r\n"))return 3;
 if(!lics_runtime_join("#ploos")||strcmp(line,"JOIN #ploos\r\n"))return 4;
 if(!lics_runtime_part("#ploos","bye")||strcmp(line,"PART #ploos :bye\r\n"))return 5;
 lics_runtime_set_output_sink(NULL); lics_runtime_shutdown();
 puts("runtime IRC output: PASS"); return 0;
}
