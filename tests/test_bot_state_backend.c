#include "../src/bot_state.h"
#include "../src/bot_state_backend.h"
#include <assert.h>
#include <string.h>
typedef struct { char value[64]; int sets,gets,dels,clears; } mock_ctx;
static int mset(void*c,const char*s,const char*k,const char*v){mock_ctx*m=c;(void)s;(void)k;strncpy(m->value,v,sizeof(m->value)-1);m->sets++;return 1;}
static const char*mget(void*c,const char*s,const char*k){mock_ctx*m=c;(void)s;(void)k;m->gets++;return m->value[0]?m->value:NULL;}
static int mdel(void*c,const char*s,const char*k){mock_ctx*m=c;(void)s;(void)k;m->value[0]=0;m->dels++;return 1;}
static void mclear(void*c){mock_ctx*m=c;m->value[0]=0;m->clears++;}
int main(void){mock_ctx m={0};bot_state_backend b={mset,mget,mdel,mclear,NULL};bot_state*s=bot_state_create();assert(s);assert(bot_state_backend_attach(s,&b,&m));assert(bot_state_set(s,"user:alice","x","42"));assert(m.sets==1);assert(strcmp(bot_state_get(s,"user:alice","x"),"42")==0);assert(m.gets==1);assert(bot_state_delete(s,"user:alice","x"));assert(m.dels==1);bot_state_clear(s);assert(m.clears==1);bot_state_destroy(s);return 0;}