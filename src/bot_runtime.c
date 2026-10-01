#include "bot_runtime.h"
#include <stdlib.h>
bot_runtime *bot_runtime_create(void){bot_runtime*r=calloc(1,sizeof(*r));if(!r)return NULL;r->state=bot_state_create();r->caps=bot_caps_create();if(!r->state||!r->caps){bot_runtime_destroy(r);return NULL;}return r;}
void bot_runtime_destroy(bot_runtime*r){if(!r)return;bot_state_destroy(r->state);bot_caps_destroy(r->caps);free(r);}
int bot_runtime_grant(bot_runtime*r,const char*cap){return r&&r->caps&&bot_caps_grant(r->caps,cap);}
int bot_runtime_revoke(bot_runtime*r,const char*cap){return r&&r->caps&&bot_caps_revoke(r->caps,cap);}
int bot_runtime_has(const bot_runtime*r,const char*cap){return r&&r->caps&&bot_caps_require(r->caps,cap);}
