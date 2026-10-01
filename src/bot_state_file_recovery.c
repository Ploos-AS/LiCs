#include "bot_state_file_recovery.h"
#include "bot_state_file_codec.h"
#include <stdio.h>
#include <string.h>
int bot_state_file_recover(const char*path,bot_state*s){char tmp[4096];FILE*f;if(!path||!s||snprintf(tmp,sizeof(tmp),"%s.tmp",path)<=0)return 0;f=fopen(path,"rb");if(f){int ok=bot_state_file_read(f,s);fclose(f);if(ok)return 1;}f=fopen(tmp,"rb");if(!f)return 0;int ok=bot_state_file_read(f,s);fclose(f);return ok;}
