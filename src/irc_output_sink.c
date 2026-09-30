#include "irc_output_sink.h"
#include <stdio.h>
static int send(const irc_output_sink *s,int n,char *b){
 if(!s||!s->send_line||n<0)return 0;
 return s->send_line(b,s->user)!=0;
}
int irc_send_privmsg(const irc_output_sink *s,const char *t,const char *x){char b[1024];return send(s,irc_format_privmsg(t,x,b,sizeof(b)),b);}
int irc_send_notice(const irc_output_sink *s,const char *t,const char *x){char b[1024];return send(s,irc_format_notice(t,x,b,sizeof(b)),b);}
int irc_send_join(const irc_output_sink *s,const char *c){char b[512];return send(s,irc_format_join(c,b,sizeof(b)),b);}
int irc_send_part(const irc_output_sink *s,const char *c,const char *r){char b[768];return send(s,irc_format_part(c,r,b,sizeof(b)),b);}
