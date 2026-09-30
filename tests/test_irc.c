#include "../src/irc_core.h"

#include <assert.h>
#include <string.h>

int main(void) {
    irc_event event;
    char out[320];

    assert(irc_parse_line("PING :irc.example.org\r\n", &event) == 1);
    assert(event.type == IRC_EVENT_PING);
    assert(strcmp(event.token, "irc.example.org") == 0);
    assert(irc_format_pong(&event, out, sizeof(out)) > 0);
    assert(strcmp(out, "PONG :irc.example.org\r\n") == 0);

    assert(irc_parse_line(":alice!u@host PRIVMSG #ploos :hello LiCs\r\n", &event) == 1);
    assert(event.type == IRC_EVENT_PRIVMSG);
    assert(strcmp(event.nick, "alice") == 0);
    assert(strcmp(event.target, "#ploos") == 0);
    assert(strcmp(event.text, "hello LiCs") == 0);

    assert(irc_parse_line(":server 001 bot :welcome\r\n", &event) == 0);
    return 0;
}
