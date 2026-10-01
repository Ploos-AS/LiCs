#ifndef BOT_TIMERS_H
#define BOT_TIMERS_H
#include <stddef.h>
#include <stdint.h>
#define BOT_MAX_TIMERS 64
typedef uint64_t bot_timer_id;
typedef int (*bot_timer_fn)(bot_timer_id id, void *user);
typedef struct { bot_timer_id id; uint64_t due_ms; uint64_t interval_ms; int repeat; bot_timer_fn handler; void *user; int active; } bot_timer;
typedef struct { bot_timer timers[BOT_MAX_TIMERS]; size_t count; bot_timer_id next_id; uint64_t now_ms; } bot_timer_registry;
void bot_timer_registry_init(bot_timer_registry *r);
bot_timer_id bot_timer_add(bot_timer_registry *r,uint64_t delay_ms,uint64_t interval_ms,int repeat,bot_timer_fn handler,void *user);
int bot_timer_cancel(bot_timer_registry *r,bot_timer_id id);
size_t bot_timer_poll(bot_timer_registry *r,uint64_t now_ms);
size_t bot_timer_count(const bot_timer_registry *r);
#endif
