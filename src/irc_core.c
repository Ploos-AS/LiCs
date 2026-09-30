#include "irc_core.h"
#include <stdio.h>
#include <string.h>
static void copy_span(char *d,size_t ds,const char *s,size_t n){if(!ds)return;if(n>=ds)n=ds-1;memcpy(d,s,n);d[n]='\0';}
static size_t llen(const char *s){size_t n=strlen(s);while(n&&(s[n-1]=='\r'||s[n-1]=='\n'))--n;return n;}
int irc_parse_line(const char *line,irc_event *e){
 const char *p,*sp,*bang,*arg,*end; size_t n;
 if(!line||!e)return -1; memset(e,0,sizeof(*e)); n=llen(line); end=line+n;
 if(n>=4&&!strncmp(line,"PING",4)){p=line+4;while(p<end&&(*p==' '||*p==':'))p++;copy_span(e->token,sizeof(e->token),p,(size_t)(end-p));e->type=IRC_EVENT_PING;return 1;}
 p=line;if(*p!=':')return 0;p++;sp=strchr(p,' ');if(!sp||sp>=end)return 0;
 copy_span(e->prefix,sizeof(e->prefix),p,(size_t)(sp-p));bang=memchr(p,'!',(size_t)(sp-p));copy_span(e->nick,sizeof(e->nick),p,bang?(size_t)(bang-p):(size_t)(sp-p));
 p=sp+1;
 if((size_t)(end-p)>=8&&!strncmp(p,"PRIVMSG ",8)){p+=8;sp=memchr(p,' ',(size_t)(end-p));if(!sp)return 0;copy_span(e->target,sizeof(e->target),p,(size_t)(sp-p));arg=sp+1;if(arg<end&&*arg==':')arg++;copy_span(e->text,sizeof(e->text),arg,(size_t)(end-arg));e->type=IRC_EVENT_PRIVMSG;return 1;}
 if((size_t)(end-p)>=7&&!strncmp(p,"NOTICE ",7)){p+=7;sp=memchr(p,' ',(size_t)(end-p));if(!sp)return 0;copy_span(e->target,sizeof(e->target),p,(size_t)(sp-p));arg=sp+1;if(arg<end&&*arg==':')arg++;copy_span(e->text,sizeof(e->text),arg,(size_t)(end-arg));e->type=IRC_EVENT_NOTICE;return 1;}
 if((size_t)(end-p)>=5&&!strncmp(p,"JOIN ",5)){arg=p+5;if(arg<end&&*arg==':')arg++;copy_span(e->target,sizeof(e->target),arg,(size_t)(end-arg));e->type=IRC_EVENT_JOIN;return 1;}
 if((size_t)(end-p)>=5&&!strncmp(p,"PART ",5)){arg=p+5;sp=memchr(arg,' ',(size_t)(end-arg));if(sp){copy_span(e->target,sizeof(e->target),arg,(size_t)(sp-arg));arg=sp+1;if(arg<end&&*arg==':')arg++;copy_span(e->text,sizeof(e->text),arg,(size_t)(end-arg));}else copy_span(e->target,sizeof(e->target),arg,(size_t)(end-arg));e->type=IRC_EVENT_PART;return 1;}
 if((size_t)(end-p)>=5&&!strncmp(p,"NICK ",5)){arg=p+5;if(arg<end&&*arg==':')arg++;copy_span(e->text,sizeof(e->text),arg,(size_t)(end-arg));e->type=IRC_EVENT_NICK;return 1;}
 if((size_t)(end-p)>=5&&!strncmp(p,"QUIT ",5)){arg=p+5;if(arg<end&&*arg==':')arg++;copy_span(e->text,sizeof(e->text),arg,(size_t)(end-arg));e->type=IRC_EVENT_QUIT;return 1;}
 return 0;
}
int irc_format_pong(const irc_event *e,char *out,size_t os){int w;if(!e||!out||!os||e->type!=IRC_EVENT_PING)return -1;w=snprintf(out,os,"PONG :%s\r\n",e->token);if(w<0||(size_t)w>=os)return -1;return w;}
