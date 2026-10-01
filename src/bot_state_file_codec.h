#ifndef BOT_STATE_FILE_CODEC_H
#define BOT_STATE_FILE_CODEC_H
#include "bot_state.h"
#include <stdio.h>
int bot_state_file_write(FILE *f,const bot_state *s);
int bot_state_file_read(FILE *f,bot_state *s);
#endif
