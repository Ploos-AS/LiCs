#include "../src/irc_core.h"
#include "../src/events.h"
#include <stdio.h>
#include <string.h>

static int seen = 0;
static int on_message(const irc_event *event, void *user) {
    (void)user;
    if (event->type == IRC_EVENT_PRIVMSG && strcmp(event->text, "hello") == 0) seen = 1;
    return 1;
}

int main(void) {
    irc_event event;
    bot_event_binding bindings[] = {{IRC_EVENT_PRIVMSG, on_message, NULL}};
    if (irc_parse_line(":alice!u@h PRIVMSG #ploos :hello\\r\\n", &event) != 1) return 1;
    if (!bot_dispatch_event(&event, bindings, 1)) return 2;
    if (!seen) return 3;
    puts("events: PASS");
    return 0;
}
