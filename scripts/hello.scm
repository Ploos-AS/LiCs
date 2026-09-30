;; LiCs M0 scripting sketch.
;; The concrete Scheme runtime/API will be selected in the next milestone step.

(define (on-hello nick channel)
  (irc-say channel (string-append "Hello " nick " from LiCs!")))
