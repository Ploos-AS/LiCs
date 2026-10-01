#include "../src/scheme_backend.h"
#include <stdio.h>
#include <string.h>
static char line[1024]; static int sink(const char*s,void*u){(void)u;snprintf(line,sizeof(line),"%s",s);return 1;}
int main(void){irc_output_sink out={sink,NULL};if(scheme_backend_init()!=0)return 1;scheme_backend_set_output_sink(&out);if(!irc_send_privmsg(&out,"#ploos","hello")||strcmp(line,"PRIVMSG #ploos :hello\r\n"))return 2;puts("LiCs language output bridge: PASS");scheme_backend_shutdown();return 0;}