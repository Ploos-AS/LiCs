#ifndef BOT_STATE_FILE_SAVE_H
#define BOT_STATE_FILE_SAVE_H
#include "bot_state.h"
int bot_state_file_save(const char *path,const bot_state *state);
int bot_state_file_load(const char *path,bot_state *state);
#endif
