#include "bot_runtime.h"
#include "bot_state_file_save.h"
#include "bot_state_file.h"
#include <stdlib.h>
bot_runtime *bot_runtime_create(void){bot_runtime*r=calloc(1,sizeof(*r));if(!r)return NULL;r->state=bot_state_create();r->caps=bot_caps_create();if(!r->state||!r->caps){bot_runtime_destroy(r);return NULL;}return r;}
void bot_runtime_destroy(bot_runtime*r){if(!r)return;bot_state_destroy(r->state);bot_caps_destroy(r->caps);free(r);}
int bot_runtime_grant(bot_runtime*r,const char*cap){return r&&r->caps&&bot_caps_grant(r->caps,cap);}
int bot_runtime_revoke(bot_runtime*r,const char*cap){return r&&r->caps&&bot_caps_revoke(r->caps,cap);}
int bot_runtime_has(const bot_runtime*r,const char*cap){return r&&r->caps&&bot_caps_require(r->caps,cap);}

int bot_runtime_attach_state_backend(bot_runtime*r,const bot_state_backend*b,void*ctx){if(!r||!r->state||!b)return 0;r->state_backend=*b;r->state_backend_ctx=ctx;if(!bot_state_backend_attach(r->state,&r->state_backend,ctx))return 0;r->state_backend_attached=1;return 1;}
int bot_runtime_load_state_file(bot_runtime*r,const char*p){return r&&r->state&&bot_state_file_load(p,r->state);}
int bot_runtime_save_state_file(const bot_runtime*r,const char*p){return r&&r->state&&bot_state_file_save(p,r->state);}
int bot_runtime_attach_file_state(bot_runtime*r,const char*p){bot_state_backend b;void*ctx=NULL;if(!r||!r->state||!p)return 0;if(!bot_state_file_open(&b,&ctx,p))return 0;if(!bot_state_backend_attach(r->state,&b,ctx)){if(b.destroy)b.destroy(ctx);return 0;}return 1;}
