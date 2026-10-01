#ifndef LICS_IRC_OUTPUT_H
#define LICS_IRC_OUTPUT_H
#include <stddef.h>\n#include "irc_core.h"
int irc_format_privmsg(const char *target,const char *text,char *out,size_t out_size);
int irc_format_notice(const char *target,const char *text,char *out,size_t out_size);
int irc_format_join(const char *channel,char *out,size_t out_size);
int irc_format_part(const char *channel,const char *reason,char *out,size_t out_size);
int irc_reply_target(const irc_event *event,char *out,size_t out_size);
#endif
