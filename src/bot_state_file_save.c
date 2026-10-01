#include "bot_state_file_save.h"
#include "bot_state_file_codec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
static int join_tmp(const char*p,char*out,size_t n){int m=snprintf(out,n,"%s.tmp",p);return m>0&&(size_t)m<n;}
int bot_state_file_save(const char*path,const bot_state*s){char tmp[4096];FILE*f;if(!path||!s||!join_tmp(path,tmp,sizeof(tmp)))return 0;f=fopen(tmp,"wb");if(!f)return 0;if(!bot_state_file_write(f,s)||fflush(f)!=0){fclose(f);unlink(tmp);return 0;}if(fsync(fileno(f))!=0){fclose(f);unlink(tmp);return 0;}if(fclose(f)!=0){unlink(tmp);return 0;}if(rename(tmp,path)!=0){unlink(tmp);return 0;}return 1;}
int bot_state_file_load(const char*path,bot_state*s){FILE*f;if(!path||!s)return 0;f=fopen(path,"rb");if(!f)return 0;int ok=bot_state_file_read(f,s);fclose(f);return ok;}
