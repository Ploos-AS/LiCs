#include "runtime_adapter.h"
#include "scheme_backend.h"
#include <stdio.h>
#include <string.h>
static int initialized, scheme_active;
int lics_runtime_init(void){#ifdef LICS_WITH_TINYSCHEME
 scheme_active=(scheme_backend_init()==0);#else
 scheme_active=0;#endif
 initialized=1;return 0;}
void lics_runtime_shutdown(void){if(scheme_active)scheme_backend_shutdown();scheme_active=0;initialized=0;}
int lics_runtime_command(const char *symbol,const irc_event *event,char *reply,size_t rs){if(!initialized||!symbol||!reply||!rs)return 0;if(scheme_active&&scheme_backend_command(symbol,event,reply,rs))return 1;if(strcmp(symbol,"hello")==0){snprintf(reply,rs,"Hello from LiCs");return 1;}return 0;}
int lics_runtime_event(const char *name,const irc_event *event,char *reply,size_t rs){if(!initialized||!name||!event||!reply||!rs)return 0;if(scheme_active&&scheme_backend_event(name,event,reply,rs))return 1;return 0;}
