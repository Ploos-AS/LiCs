#include "../src/runtime_adapter.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    irc_event event = {0};
    char reply[128] = {0};
    event.type = IRC_EVENT_PRIVMSG;
    strcpy(event.nick, "alice");
    strcpy(event.target, "#ploos");
    strcpy(event.text, "!hello");

    if (lics_runtime_init() != 0) return 1;
    if (!lics_runtime_command("hello", &event, reply, sizeof(reply))) return 2;
    if (strcmp(reply, "Hello from LiCs") != 0) return 3;
    lics_runtime_shutdown();
    puts("runtime adapter: PASS");
    return 0;
}
