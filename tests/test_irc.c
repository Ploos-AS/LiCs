#include "../src/irc_core.h"
#include "../src/irc_output.h"
#include <assert.h>
#include <string.h>
int main(void){
 irc_event e; char out[320];
 assert(irc_parse_line("PING :irc.example.org\r\n",&e)==1); assert(e.type==IRC_EVENT_PING); assert(strcmp(e.token,"irc.example.org")==0); assert(irc_format_pong(&e,out,sizeof(out))>0); assert(strcmp(out,"PONG :irc.example.org\r\n")==0);
 assert(irc_parse_line(":alice!u@host PRIVMSG #ploos :hello\r\n",&e)==1); assert(e.command[0]=='\0'); assert(e.type==IRC_EVENT_PRIVMSG); assert(strcmp(e.nick,"alice")==0); assert(strcmp(e.target,"#ploos")==0); assert(strcmp(e.text,"hello")==0);
 assert(irc_parse_line(":alice!u@host PRIVMSG #ploos :!ban Per 10m\r\n",&e)==1); assert(strcmp(e.command,"ban")==0); assert(strcmp(e.args,"Per 10m")==0); assert(irc_parse_line(":alice!u@host NOTICE bob :notice\r\n",&e)==1); assert(e.type==IRC_EVENT_NOTICE); assert(strcmp(e.target,"bob")==0); assert(strcmp(e.text,"notice")==0);
 assert(irc_parse_line(":alice!u@host JOIN #ploos\r\n",&e)==1); assert(e.type==IRC_EVENT_JOIN); assert(strcmp(e.target,"#ploos")==0);
 assert(irc_parse_line(":alice!u@host PART #ploos :bye\r\n",&e)==1); assert(e.type==IRC_EVENT_PART); assert(strcmp(e.target,"#ploos")==0); assert(strcmp(e.text,"bye")==0);
 assert(irc_parse_line(":alice!u@host NICK :ally\r\n",&e)==1); assert(e.type==IRC_EVENT_NICK); assert(strcmp(e.text,"ally")==0);
 assert(irc_parse_line(":alice!u@host QUIT :gone\r\n",&e)==1); assert(e.type==IRC_EVENT_QUIT); assert(strcmp(e.text,"gone")==0);
 assert(irc_parse_line(":server 001 bot :welcome\r\n",&e)==0);
 assert(irc_format_privmsg("#ploos","hello",out,sizeof(out))>0); assert(strcmp(out,"PRIVMSG #ploos :hello\r\n")==0); assert(irc_format_notice("#ploos","hello",out,sizeof(out))>0); assert(strcmp(out,"NOTICE #ploos :hello\r\n")==0); irc_event re={0}; snprintf(re.target,sizeof(re.target),"#ploos"); snprintf(re.nick,sizeof(re.nick),"alice"); assert(irc_reply_target(&re,out,sizeof(out))>0); assert(strcmp(out,"#ploos")==0); snprintf(re.target,sizeof(re.target),"alice"); assert(irc_reply_target(&re,out,sizeof(out))>0); assert(strcmp(out,"alice")==0);
 return 0;
}
