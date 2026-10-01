#ifndef BOT_CAPS_H
#define BOT_CAPS_H
typedef struct bot_caps bot_caps;
bot_caps *bot_caps_create(void);
void bot_caps_destroy(bot_caps *c);
int bot_caps_grant(bot_caps *c,const char *cap);
int bot_caps_revoke(bot_caps *c,const char *cap);
int bot_caps_has(const bot_caps *c,const char *cap);
void bot_caps_clear(bot_caps *c);
#endif
