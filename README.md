# LiCs

LiCs is a C-based IRC bot with an embedded Lisp/Scheme scripting environment.

## M1 status

The standalone runtime now has a qualified language-neutral contract for IRC commands/events and deterministic timers. See docs/BOT_RUNTIME_CONTRACT.md.

- IRC command/event dispatch: qualified
- Timer registry and named handlers: qualified
- Scheme timer bindings: qualified
- AFTER / EVERY / CANCEL semantics: qualified
- CI regression coverage: enabled

## M0 goals

- Small, portable C IRC core
- Embedded Lisp/Scheme runtime
- Event-driven scripting for IRC commands and messages
- PBMP integration boundary defined from the start
- Hooks for BotWeb and BotAI
- Standalone-first operation
- Standalone qualification with PBMP, BotWeb, BotAI and BotLogic disabled
- Deterministic tests for parser, dispatch and script bindings

## Initial architecture

```text
IRC network
    |
    v
C IRC core
    |
    +-- parser/state
    +-- command/event dispatcher
    +-- PBMP adapter
    +-- BotWeb/BotAI hooks
    |
    `-- Lisp/Scheme VM
         +-- commands
         +-- events
         +-- timers
         `-- modules
```

## M0 scripting surface

The first scripting API should expose a deliberately small set of primitives:

- `irc-say`
- `irc-notice`
- `irc-join`
- `irc-part`
- `irc-nick`
- event registration
- command registration
- timers
- basic bot state access

## Design principles

1. C first: the bot core remains understandable and portable.
2. The Lisp environment must be useful, not decorative.
3. Scripts must not be able to crash the IRC core.
4. PBMP integration is optional at runtime, but first-class in the architecture.
5. BotWeb, BotAI and BotLogic remain optional components.
6. No malware or offensive payloads are stored in the repository.


Standalone qualification requires the complete bot, including its Lisp/Scheme runtime, to build and pass its test suite with PBMP, BotWeb, BotAI and BotLogic disabled. The language/runtime is a required part of LiCs, not an optional integration. PBMP, BotWeb, BotAI and BotLogic must remain optional build/runtime dependencies.

## License

Software: MIT.
