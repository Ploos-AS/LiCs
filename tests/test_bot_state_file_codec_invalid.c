#include "../src/bot_state.h"
#include "../src/bot_state_file_codec.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static void bad(const char*x){bot_state*s=bot_state_create();FILE*f=tmpfile();assert(s&&f);fwrite(x,1,strlen(x),f);rewind(f);assert(!bot_state_file_read(f,s));assert(bot_state_count(s)==0);fclose(f);bot_state_destroy(s);}
int main(void){bad("NOPE\n");bad("BSTATE1\n4:user");bad("BSTATE1\n3:user3:key");bad("BSTATE1\n4:user3:key3:valX");return 0;}