# Bot Runtime Contract

LiCs implements the common standalone bot-runtime contract using Scheme-native bindings.

## M1 qualification

**M1 status: COMPLETE — runtime contract frozen.**

The qualification covers parsing, generic command dispatch, event registration, deterministic timers, command/event context, semantic output (`SAY`, `NOTICE`, `REPLY`), and CI coverage. New M2 features must not change these semantics without an explicit contract revision.

## M1 qualification

| Capability | Contract | LiCs |
|---|---|---|
| IRC commands | C dispatcher -> language callback | PASS |
| IRC events | C event dispatcher -> language callback | PASS |
| One-shot timer | timer-after | PASS |
| Repeating timer | timer-every | PASS |
| Timer cancellation | timer-cancel | PASS |
| Named callback | Scheme procedure name | PASS |
| Deterministic polling | explicit runtime clock | PASS |
| CI qualification | make test | PASS |

## Scheme timer API

    (define (heartbeat)
      (irc-say "#ploos" "I'm alive"))
    (define timer-id (timer-every 60000 "heartbeat"))
    (timer-cancel timer-id)

Timer IDs are returned as Scheme integers. Timer callback contexts are owned by the runtime and released on cancellation.

The C timer registry is language-neutral; Scheme only supplies the callback binding.

## Contract rule

Future language features should add a deterministic test at the language-binding layer and retain the same runtime semantics.


## Command and event context

The common contract exposes the same IRC context to every language binding:

- command: command name, sender nick, target, and original message text
- event: event name, sender nick, target, and event text/payload

The exact call syntax remains language-native. Bindings must not invent different semantics for these fields. A handler may ignore fields it does not need.

The runtime keeps the parsed `irc_event` as the canonical source of truth; language adapters translate that context without changing its meaning.


## Context access

M1 command/event handlers receive a canonical IRC context. Bindings expose these fields using language-native conventions:

| Field | Meaning |
|---|---|
| command | registered command without the leading `!` |
| event | normalized event name |
| nick | sender nickname |
| target | channel or recipient |
| text | original message/event text |
| token | protocol token when present |

A binding may expose the context as arguments or as a read-only context object, but the values must retain these meanings. The leading `!` is transport syntax and is not part of the registered command name.

Command arguments are derived from the original message text after the command token. They are not silently discarded by the runtime.


## IRC output contract

Language bindings expose a minimal, language-native output surface:

| Operation | Meaning |
|---|---|
| SAY | send a PRIVMSG to a target/channel |
| NOTICE | send an IRC NOTICE to a target |
| REPLY | reply to the current command/event target |

The runtime owns IRC formatting, escaping, connection state, and transmission. Scripts provide semantic arguments only. A failed transmission is reported by the binding; scripts must not construct raw IRC protocol lines.


## M2.1 State

The runtime provides a language-neutral in-memory state store keyed by `scope + key`. State is bot-owned runtime state; persistence is deliberately outside M2.1. Missing keys return no value, setting an existing key replaces its value, and scopes are opaque strings interpreted by the host/bot policy.


## M2.3 Runtime policy

A bot runtime owns exactly one state store and one capability store. Capability policy is default-deny: a newly created runtime has no capabilities. Host/PBMP policy explicitly grants capabilities. Language bindings must not maintain independent policy stores.


## M2.3 Capability set

| Capability | Operation |
|---|---|
| irc.say | send PRIVMSG |
| irc.notice | send NOTICE |
| irc.join | JOIN a channel |
| irc.part | PART a channel |
| state.read | read bot state |
| state.write | create or update bot state |
| state.delete | delete bot state |

All capabilities are default-deny. There is no wildcard capability in M2.3. Host/PBMP policy grants capabilities explicitly.
