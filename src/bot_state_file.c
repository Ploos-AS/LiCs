#include "bot_state_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct { char *path; bot_state *shadow; } file_ctx;
static char *dupstr(const char*s){size_t n;if(!s)return NULL;n=strlen(s)+1;char*p=malloc(n);if(p)memcpy(p,s,n);return p;}
static int fset(void*vp,const char*s,const char*k,const char*v){file_ctx*c=vp;return bot_state_set(c->shadow,s,k,v)&&1;}
static const char*fget(void*vp,const char*s,const char*k){return bot_state_get(((file_ctx*)vp)->shadow,s,k);}
static int fdel(void*vp,const char*s,const char*k){return bot_state_delete(((file_ctx*)vp)->shadow,s,k);}
static void fclear(void*vp){bot_state_clear(((file_ctx*)vp)->shadow);}
static void fdestroy(void*vp){file_ctx*c=vp;if(!c)return;bot_state_destroy(c->shadow);free(c->path);free(c);}
static int save(file_ctx*c){FILE*f=fopen(c->path,"wb");if(!f)return 0;/* serialization is added after format qualification */fputs("BSTATE1\n",f);fclose(f);return 1;}
static int load(file_ctx*c){FILE*f=fopen(c->path,"rb");if(!f)return 1;char hdr[9]={0};fread(hdr,1,8,f);fclose(f);return strcmp(hdr,"BSTATE1\n")==0;}
int bot_state_file_open(bot_state_backend*b,void**ctx,const char*path){file_ctx*c;if(!b||!ctx||!path)return 0;c=calloc(1,sizeof(*c));if(!c)return 0;c->path=dupstr(path);c->shadow=bot_state_create();if(!c->path||!c->shadow||!load(c)){fdestroy(c);return 0;}b->set=fset;b->get=fget;b->del=fdel;b->clear=fclear;b->destroy=fdestroy;*ctx=c;return 1;}
void bot_state_file_close(void*ctx){fdestroy(ctx);}
