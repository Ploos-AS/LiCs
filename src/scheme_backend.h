#ifndef LICS_SCHEME_BACKEND_H
#define LICS_SCHEME_BACKEND_H
#include "irc_core.h"
#include "irc_output_sink.h"
#include <stddef.h>
int scheme_backend_init(void);
void scheme_backend_shutdown(void);
void scheme_backend_set_output_sink(const irc_output_sink *sink);
int scheme_backend_command(const char *symbol,const irc_event *event,char *reply,size_t reply_size);
int scheme_backend_event(const char *name,const irc_event *event,char *reply,size_t reply_size);
#endif
