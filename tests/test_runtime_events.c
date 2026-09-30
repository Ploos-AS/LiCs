#include "../src/irc_core.h"
#include "../src/runtime_adapter.h"
#include "../src/runtime_dispatch.h"
#include "../src/events.h"
#include <stdio.h>
#include <string.h>
int main(void){
 irc_event e; char reply[128]; int ok;
 if (lics_runtime_init()!=0) return 1;
 const struct {const char *line; const char *event; const char *want;} cases[]={
  {":alice!u@h JOIN #ploos\r\n","join","Welcome from LICS"},
  {":alice!u@h PART #ploos :bye\r\n","part",""},
  {":alice!u@h NICK :ally\r\n","nick",""},
  {":alice!u@h QUIT :gone\r\n","quit",""},
  {":alice!u@h NOTICE bob :notice\r\n","notice",""}
 };
 for(size_t i=0;i<sizeof(cases)/sizeof(cases[0]);++i){
  memset(&e,0,sizeof(e)); reply[0]='\0';
  if(irc_parse_line(cases[i].line,&e)!=1)return 2;
  ok=lics_dispatch_event_runtime(&e,reply,sizeof(reply));
  if(i==0 && !bot_dispatch_event(&e,&registry)) return 5;
  if(i==0){if(!ok||strcmp(reply,cases[i].want)!=0)return 3;}
  else if(ok)return 4;
 }
 lics_runtime_shutdown();
 puts("LICS runtime events: PASS"); return 0;
}
