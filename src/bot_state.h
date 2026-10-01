#ifndef BOT_STATE_H
#define BOT_STATE_H
#include <stddef.h>
#include "irc_core.h"
typedef struct bot_state bot_state;
bot_state *bot_state_create(void);
void bot_state_destroy(bot_state *s);
int bot_state_set(bot_state *s,const char *scope,const char *key,const char *value);
const char *bot_state_get(const bot_state *s,const char *scope,const char *key);
int bot_state_delete(bot_state *s,const char *scope,const char *key);
void bot_state_clear(bot_state *s);
size_t bot_state_count(const bot_state *s);
int bot_state_scope_user(const char *nick,char *out,size_t out_size);
int bot_state_scope_channel(const char *channel,char *out,size_t out_size);
int bot_state_scope_event_user(const void *event,char *out,size_t out_size);
int bot_state_scope_event_target(const void *event,char *out,size_t out_size);
#endif
