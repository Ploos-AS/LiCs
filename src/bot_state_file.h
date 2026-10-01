#ifndef BOT_STATE_FILE_H
#define BOT_STATE_FILE_H
#include "bot_state_backend.h"
int bot_state_file_open(bot_state_backend *backend, void **ctx, const char *path);
void bot_state_file_close(void *ctx);
#endif
