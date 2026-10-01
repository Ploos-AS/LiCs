#ifndef LICS_IRC_CORE_H
#define LICS_IRC_CORE_H
#include <stddef.h>
typedef enum { IRC_EVENT_NONE=0, IRC_EVENT_PING, IRC_EVENT_PRIVMSG, IRC_EVENT_NOTICE, IRC_EVENT_JOIN, IRC_EVENT_PART, IRC_EVENT_NICK, IRC_EVENT_QUIT } irc_event_type;
typedef struct { irc_event_type type; char prefix[128]; char nick[64]; char target[128]; char text[512]; char token[256]; char command[64]; char args[448]; } irc_event;
int irc_parse_line(const char *line, irc_event *event);
int irc_format_pong(const irc_event *event, char *out, size_t out_size);
#endif
