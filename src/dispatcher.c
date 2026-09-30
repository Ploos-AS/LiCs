#include "dispatcher.h"
#include <string.h>
void command_registry_init(command_registry *r){if(r)memset(r,0,sizeof(*r));}
int command_registry_register(command_registry *r,const char *cmd,script_command_fn fn){size_t i;if(!r||!cmd||!*cmd||!fn)return 0;for(i=0;i<r->count;i++)if(strcmp(r->entries[i].command,cmd)==0){r->entries[i].handler=fn;return 1;}if(r->count>=BOT_MAX_COMMANDS)return 0;r->entries[r->count].command=cmd;r->entries[r->count].handler=fn;r->count++;return 1;}
int command_registry_unregister(command_registry *r,const char *cmd){size_t i;if(!r||!cmd)return 0;for(i=0;i<r->count;i++)if(strcmp(r->entries[i].command,cmd)==0){if(i+1<r->count)memmove(&r->entries[i],&r->entries[i+1],(r->count-i-1)*sizeof(r->entries[0]));r->count--;return 1;}return 0;}
int dispatch_privmsg(const irc_event *e,const command_registry *r,char *reply,size_t rs){const char *text;size_t i;if(!e||e->type!=IRC_EVENT_PRIVMSG||!r||!reply||!rs)return 0;text=e->text;if(*text=='!')text++;for(i=0;i<r->count;i++)if(strcmp(text,r->entries[i].command)==0)return r->entries[i].handler(e,reply,rs);return 0;}
