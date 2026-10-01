# Bot Runtime Contract

LiCs implements the common standalone bot-runtime contract using Scheme-native bindings.

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
