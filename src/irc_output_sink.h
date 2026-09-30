#ifndef IRC_OUTPUT_SINK_H
#define IRC_OUTPUT_SINK_H
#include "irc_output.h"
typedef int (*irc_send_line_fn)(const char *line, void *user);
typedef struct { irc_send_line_fn send_line; void *user; } irc_output_sink;
int irc_send_privmsg(const irc_output_sink *sink,const char *target,const char *text);
int irc_send_notice(const irc_output_sink *sink,const char *target,const char *text);
int irc_send_join(const irc_output_sink *sink,const char *channel);
int irc_send_part(const irc_output_sink *sink,const char *channel,const char *reason);
#endif
