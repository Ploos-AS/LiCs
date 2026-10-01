#include "bot_caps.h"
#include <stdlib.h>
#include <string.h>
typedef struct cap_entry{char *name;struct cap_entry *next;} cap_entry;
struct bot_caps{cap_entry *head;};
static char *dupstr(const char*s){size_t n;if(!s)return NULL;n=strlen(s)+1;char*p=malloc(n);if(p)memcpy(p,s,n);return p;}
bot_caps *bot_caps_create(void){return calloc(1,sizeof(bot_caps));}
void bot_caps_destroy(bot_caps*c){cap_entry*e,*n;if(!c)return;for(e=c->head;e;e=n){n=e->next;free(e->name);free(e);}free(c);}
int bot_caps_has(const bot_caps*c,const char*cap){cap_entry*e;if(!c||!cap)return 0;for(e=c->head;e;e=e->next)if(strcmp(e->name,cap)==0)return 1;return 0;}
int bot_caps_grant(bot_caps*c,const char*cap){cap_entry*e;if(!c||!cap||!*cap)return 0;if(bot_caps_has(c,cap))return 1;e=calloc(1,sizeof(*e));if(!e)return 0;e->name=dupstr(cap);if(!e->name){free(e);return 0;}e->next=c->head;c->head=e;return 1;}
int bot_caps_revoke(bot_caps*c,const char*cap){cap_entry**p;if(!c||!cap)return 0;for(p=&c->head;*p;p=&(*p)->next)if(strcmp((*p)->name,cap)==0){cap_entry*e=*p;*p=e->next;free(e->name);free(e);return 1;}return 0;}
void bot_caps_clear(bot_caps*c){cap_entry*e,*n;if(!c)return;for(e=c->head;e;e=n){n=e->next;free(e->name);free(e);}c->head=NULL;}
