#include "irc_output.h"
#include <stdio.h>\n#include <string.h>
static int fmt(char *o,size_t n,const char *f,const char *a,const char *b){int w;if(!o||!n||!a)return -1;w=b?snprintf(o,n,f,a,b):snprintf(o,n,f,a);return w<0||(size_t)w>=n?-1:w;}
int irc_format_privmsg(const char *t,const char *x,char *o,size_t n){return fmt(o,n,"PRIVMSG %s :%s\r\n",t,x);}
int irc_format_notice(const char *t,const char *x,char *o,size_t n){return fmt(o,n,"NOTICE %s :%s\r\n",t,x);}
int irc_format_join(const char *c,char *o,size_t n){return fmt(o,n,"JOIN %s\r\n",c,NULL);}
int irc_format_part(const char *c,const char *r,char *o,size_t n){return r&&*r?fmt(o,n,"PART %s :%s\r\n",c,r):fmt(o,n,"PART %s\r\n",c,NULL);}

int irc_reply_target(const irc_event *e,char *o,size_t n){const char *t;if(!e||!o||!n)return -1;t=(e->target[0]&&((e->target[0]=='#')||(e->target[0]=='&')||(e->target[0]=='+')||(e->target[0]=='!')))?e->target:e->nick;if(!t||!*t)return -1;if(snprintf(o,n,"%s",t)<0||(strlen(t)+1)>n)return -1;return (int)strlen(t);}
