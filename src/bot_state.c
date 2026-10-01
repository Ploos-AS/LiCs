#include "bot_state.h"
#include <stdlib.h>
#include <string.h>
typedef struct bot_state_entry{char *scope;char *key;char *value;struct bot_state_entry *next;} bot_state_entry;
struct bot_state{bot_state_entry *head;size_t count;};
static char *dupstr(const char *s){size_t n;if(!s)return NULL;n=strlen(s)+1;char *p=(char*)malloc(n);if(p)memcpy(p,s,n);return p;}
static bot_state_entry *find(bot_state *s,const char *scope,const char *key){bot_state_entry *e;if(!s||!scope||!key)return NULL;for(e=s->head;e;e=e->next)if(strcmp(e->scope,scope)==0&&strcmp(e->key,key)==0)return e;return NULL;}
bot_state *bot_state_create(void){return (bot_state*)calloc(1,sizeof(bot_state));}
void bot_state_destroy(bot_state *s){bot_state_entry *e,*n;if(!s)return;for(e=s->head;e;e=n){n=e->next;free(e->scope);free(e->key);free(e->value);free(e);}free(s);}
int bot_state_set(bot_state *s,const char *scope,const char *key,const char *value){bot_state_entry *e;if(!s||!scope||!key||!value)return 0;e=find(s,scope,key);if(e){char *v=dupstr(value);if(!v)return 0;free(e->value);e->value=v;return 1;}e=(bot_state_entry*)calloc(1,sizeof(*e));if(!e)return 0;e->scope=dupstr(scope);e->key=dupstr(key);e->value=dupstr(value);if(!e->scope||!e->key||!e->value){free(e->scope);free(e->key);free(e->value);free(e);return 0;}e->next=s->head;s->head=e;s->count++;return 1;}
const char *bot_state_get(const bot_state *s,const char *scope,const char *key){bot_state_entry *e;if(!s||!scope||!key)return NULL;e=find((bot_state*)s,scope,key);return e?e->value:NULL;}
int bot_state_delete(bot_state *s,const char *scope,const char *key){bot_state_entry **p;if(!s||!scope||!key)return 0;for(p=&s->head;*p;p=&(*p)->next)if(strcmp((*p)->scope,scope)==0&&strcmp((*p)->key,key)==0){bot_state_entry *e=*p;*p=e->next;free(e->scope);free(e->key);free(e->value);free(e);s->count--;return 1;}return 0;}
void bot_state_clear(bot_state *s){bot_state_entry *e,*n;if(!s)return;for(e=s->head;e;e=n){n=e->next;free(e->scope);free(e->key);free(e->value);free(e);}s->head=NULL;s->count=0;}
size_t bot_state_count(const bot_state *s){return s?s->count:0;}
