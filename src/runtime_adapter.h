#ifndef LICS_RUNTIME_ADAPTER_H
#define LICS_RUNTIME_ADAPTER_H

#include "irc_core.h"
#include <stddef.h>

int lics_runtime_init(void);
void lics_runtime_shutdown(void);
int lics_runtime_command(const char *symbol, const irc_event *event, char *reply, size_t reply_size);

#endif
