#include "bot_state.h"
#include "bot_state_backend.h"
#include <stdlib.h>
#include <string.h>\n#include <stdio.h>
typedef struct bot_state_entry{char *scope;char *key;char *value;struct bot_state_entry *next;} bot_state_entry;
struct bot_state{bot_state_entry *head;size_t count;const bot_state_backend *backend;void *backend_ctx;};
static char *dupstr(const char *s){size_t n;if(!s)return NULL;n=strlen(s)+1;char *p=(char*)malloc(n);if(p)memcpy(p,s,n);return p;}
static bot_state_entry *find(bot_state *s,const char *scope,const char *key){bot_state_entry *e;if(!s||!scope||!key)return NULL;for(e=s->head;e;e=e->next)if(strcmp(e->scope,scope)==0&&strcmp(e->key,key)==0)return e;return NULL;}
bot_state *bot_state_create(void){return (bot_state*)calloc(1,sizeof(bot_state));}
void bot_state_destroy(bot_state *s){bot_state_entry *e,*n;if(!s)return;for(e=s->head;e;e=n){n=e->next;free(e->scope);free(e->key);free(e->value);free(e);}free(s);}
int bot_state_set(bot_state *s,const char *scope,const char *key,const char *value){if(s&&s->backend&&s->backend->set)return s->backend->set(s->backend_ctx,scope,key,value);bot_state_entry *e;if(!s||!scope||!key||!value)return 0;e=find(s,scope,key);if(e){char *v=dupstr(value);if(!v)return 0;free(e->value);e->value=v;return 1;}e=(bot_state_entry*)calloc(1,sizeof(*e));if(!e)return 0;e->scope=dupstr(scope);e->key=dupstr(key);e->value=dupstr(value);if(!e->scope||!e->key||!e->value){free(e->scope);free(e->key);free(e->value);free(e);return 0;}e->next=s->head;s->head=e;s->count++;return 1;}
const char *bot_state_get(const bot_state *s,const char *scope,const char *key){if(s&&s->backend&&s->backend->get)return s->backend->get(s->backend_ctx,scope,key);bot_state_entry *e;if(!s||!scope||!key)return NULL;e=find((bot_state*)s,scope,key);return e?e->value:NULL;}
int bot_state_delete(bot_state *s,const char *scope,const char *key){if(s&&s->backend&&s->backend->del)return s->backend->del(s->backend_ctx,scope,key);bot_state_entry **p;if(!s||!scope||!key)return 0;for(p=&s->head;*p;p=&(*p)->next)if(strcmp((*p)->scope,scope)==0&&strcmp((*p)->key,key)==0){bot_state_entry *e=*p;*p=e->next;free(e->scope);free(e->key);free(e->value);free(e);s->count--;return 1;}return 0;}
void bot_state_clear(bot_state *s){if(s&&s->backend&&s->backend->clear){s->backend->clear(s->backend_ctx);return;}bot_state_entry *e,*n;if(!s)return;for(e=s->head;e;e=n){n=e->next;free(e->scope);free(e->key);free(e->value);free(e);}s->head=NULL;s->count=0;}
size_t bot_state_count(const bot_state *s){return s?s->count:0;}
int bot_state_backend_attach(bot_state *s,const bot_state_backend *b,void *ctx){if(!s||!b||!b->set||!b->get||!b->del||!b->clear)return 0;s->backend=b;s->backend_ctx=ctx;return 1;}

int bot_state_scope_user(const char *nick,char *out,size_t n){int m;if(!nick||!out||!n)return 0;m=snprintf(out,n,"user:%s",nick);return m>=0&&(size_t)m<n;}
int bot_state_scope_channel(const char *channel,char *out,size_t n){int m;if(!channel||!out||!n)return 0;m=snprintf(out,n,"channel:%s",channel);return m>=0&&(size_t)m<n;}

int bot_state_scope_event_user(const irc_event *e,char *o,size_t n){return e?bot_state_scope_user(e->nick,o,n):0;}
int bot_state_scope_event_target(const irc_event *e,char *o,size_t n){if(!e)return 0; if(e->target[0]=='#'||e->target[0]=='&'||e->target[0]=='+'||e->target[0]=='!') return bot_state_scope_channel(e->target,o,n); return bot_state_scope_user(e->nick,o,n);}

int bot_state_foreach(const bot_state*s,bot_state_iter_fn fn,void*ctx){bot_state_entry*e;if(!s||!fn)return 0;for(e=s->head;e;e=e->next)if(!fn(e->scope,e->key,e->value,ctx))return 0;return 1;}
