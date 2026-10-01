#ifndef BOT_STATE_BACKEND_H
#define BOT_STATE_BACKEND_H
#include "bot_state.h"
typedef struct bot_state_backend {
 int (*set)(void *ctx,const char *scope,const char *key,const char *value);
 const char *(*get)(void *ctx,const char *scope,const char *key);
 int (*del)(void *ctx,const char *scope,const char *key);
 void (*clear)(void *ctx);
 void (*destroy)(void *ctx);
} bot_state_backend;
int bot_state_backend_attach(bot_state *state,const bot_state_backend *backend,void *ctx);
#endif
