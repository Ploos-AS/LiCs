#include "../src/bot_runtime.h"
#include <assert.h>
int main(void){bot_runtime*r=bot_runtime_create();assert(r);assert(!bot_runtime_has(r,"state.read"));assert(bot_runtime_grant(r,"state.read"));assert(bot_runtime_has(r,"state.read"));assert(bot_runtime_revoke(r,"state.read"));assert(!bot_runtime_has(r,"state.read"));bot_runtime_destroy(r);return 0;}