#include "../src/irc_output_sink.h"
#include <stdio.h>
#include <string.h>
static char line[1024];
static int sink(const char *s,void *u){(void)u;snprintf(line,sizeof(line),"%s",s);return 1;}
int main(void){
 irc_output_sink s={sink,NULL};
 if(!irc_send_privmsg(&s,"#ploos","hello")||strcmp(line,"PRIVMSG #ploos :hello\r\n"))return 1;
 if(!irc_send_notice(&s,"alice","hi")||strcmp(line,"NOTICE alice :hi\r\n"))return 2;
 if(!irc_send_join(&s,"#ploos")||strcmp(line,"JOIN #ploos\r\n"))return 3;
 if(!irc_send_part(&s,"#ploos","bye")||strcmp(line,"PART #ploos :bye\r\n"))return 4;
 puts("IRC output sink: PASS"); return 0;
}
