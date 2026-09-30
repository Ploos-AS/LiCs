#include "../src/irc_output.h"
#include <stdio.h>
#include <string.h>
int main(void){
 char b[128];
 if(irc_format_privmsg("#ploos","hello",b,sizeof(b))<0||strcmp(b,"PRIVMSG #ploos :hello\r\n"))return 1;
 if(irc_format_notice("alice","notice",b,sizeof(b))<0||strcmp(b,"NOTICE alice :notice\r\n"))return 2;
 if(irc_format_join("#ploos",b,sizeof(b))<0||strcmp(b,"JOIN #ploos\r\n"))return 3;
 if(irc_format_part("#ploos","bye",b,sizeof(b))<0||strcmp(b,"PART #ploos :bye\r\n"))return 4;
 puts("IRC output API: PASS");return 0;
}