#ifndef BOT_RUNTIME_H
#define BOT_RUNTIME_H
#include "bot_state.h"
#include "bot_caps.h"
#include "bot_state_backend.h"
typedef struct bot_runtime { bot_state *state; bot_caps *caps; } bot_runtime;
bot_runtime *bot_runtime_create(void);
void bot_runtime_destroy(bot_runtime *r);
int bot_runtime_grant(bot_runtime *r,const char *cap);
int bot_runtime_revoke(bot_runtime *r,const char *cap);
int bot_runtime_has(const bot_runtime *r,const char *cap);
int bot_runtime_attach_state_backend(bot_runtime *r,const bot_state_backend *backend,void *ctx);
int bot_runtime_load_state_file(bot_runtime *r,const char *path);
int bot_runtime_save_state_file(const bot_runtime *r,const char *path);
int bot_runtime_attach_file_state(bot_runtime *r,const char *path);
#endif
