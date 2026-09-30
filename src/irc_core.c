#include "irc_core.h"

#include <stdio.h>
#include <string.h>

static void copy_span(char *dst, size_t dst_size, const char *start, size_t len) {
    if (dst_size == 0) return;
    if (len >= dst_size) len = dst_size - 1;
    memcpy(dst, start, len);
    dst[len] = '\0';
}

static size_t line_len(const char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\r' || s[n - 1] == '\n')) --n;
    return n;
}

int irc_parse_line(const char *line, irc_event *event) {
    const char *p;
    const char *space;
    const char *bang;
    size_t n;

    if (!line || !event) return -1;
    memset(event, 0, sizeof(*event));
    n = line_len(line);

    if (n >= 4 && strncmp(line, "PING", 4) == 0) {
        p = line + 4;
        while ((size_t)(p - line) < n && (*p == ' ' || *p == ':')) ++p;
        copy_span(event->token, sizeof(event->token), p, n - (size_t)(p - line));
        event->type = IRC_EVENT_PING;
        return 1;
    }

    p = line;
    if (*p != ':') return 0;
    ++p;
    space = strchr(p, ' ');
    if (!space) return 0;
    copy_span(event->prefix, sizeof(event->prefix), p, (size_t)(space - p));
    bang = memchr(p, '!', (size_t)(space - p));
    copy_span(event->nick, sizeof(event->nick), p,
              bang ? (size_t)(bang - p) : (size_t)(space - p));

    p = space + 1;
    if (strncmp(p, "PRIVMSG ", 8) != 0) return 0;
    p += 8;
    space = strchr(p, ' ');
    if (!space) return 0;
    copy_span(event->target, sizeof(event->target), p, (size_t)(space - p));
    p = space + 1;
    if (*p == ':') ++p;
    copy_span(event->text, sizeof(event->text), p,
              n - (size_t)(p - line));
    event->type = IRC_EVENT_PRIVMSG;
    return 1;
}

int irc_format_pong(const irc_event *event, char *out, size_t out_size) {
    int written;
    if (!event || !out || out_size == 0 || event->type != IRC_EVENT_PING) return -1;
    written = snprintf(out, out_size, "PONG :%s\r\n", event->token);
    if (written < 0 || (size_t)written >= out_size) return -1;
    return written;
}
