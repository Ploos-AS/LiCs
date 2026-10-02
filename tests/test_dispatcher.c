#include "../src/irc_core.h"
#include "../src/dispatcher.h"
#include <stdio.h>
#include <string.h>

static int hello(const irc_event *event, char *reply, size_t reply_size) {
    (void)event;
    snprintf(reply, reply_size, "Hello from LiCs");
    return 1;
}

int main(void) {
    irc_event event;
    char reply[128] = {0};
    command_registry registry;
    command_registry_init(&registry);
    if (!command_registry_register(&registry, "hello", hello)) return 1;

    if (irc_parse_line(":alice!u@h PRIVMSG #ploos :!hello\r\n", &event) != 1) return 2;
    if (!dispatch_privmsg(&event, &registry, reply, sizeof(reply))) return 3;
    if (strcmp(reply, "Hello from LiCs") != 0) return 4;

    if (irc_parse_line(":alice!u@h PRIVMSG #ploos :!unknown\r\n", &event) != 1) return 5;
    if (dispatch_privmsg(&event, &registry, reply, sizeof(reply)) != 0) return 6;

    puts("dispatcher: PASS");
    return 0;
}
