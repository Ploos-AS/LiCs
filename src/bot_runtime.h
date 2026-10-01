#ifndef BOT_RUNTIME_H
#define BOT_RUNTIME_H
#include "bot_state.h"
#include "bot_caps.h"
typedef struct bot_runtime { bot_state *state; bot_caps *caps; } bot_runtime;
bot_runtime *bot_runtime_create(void);
void bot_runtime_destroy(bot_runtime *r);
int bot_runtime_grant(bot_runtime *r,const char *cap);
int bot_runtime_revoke(bot_runtime *r,const char *cap);
int bot_runtime_has(const bot_runtime *r,const char *cap);
#endif
