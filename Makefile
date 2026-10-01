CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
BIN := build/lics
TEST_IRC := build/test_irc
TEST_DISPATCHER := build/test_dispatcher
TEST_EVENTS := build/test_events
TEST_RUNTIME := build/test_runtime
TEST_BOT_STATE := build/test_bot_state
TEST_BOT_CAPS := build/test_bot_caps
TEST_BOT_RUNTIME := build/test_bot_runtime
TEST_RUNTIME_DISPATCH := build/test_runtime_dispatch
TEST_TIMERS := build/test_timers
TEST_TIMER_HANDLERS := build/test_timer_handlers
TEST_RUNTIME_TIMERS := build/test_runtime_timers
SRC := src/bot_state.c src/bot_caps.c src/bot_runtime.c src/main.c src/irc_core.c src/dispatcher.c src/events.c src/runtime_adapter.c src/runtime_dispatch.c src/timers.c src/timer_handlers.c src/scheme_backend.c
TINYSCHEME_DIR ?= vendor/tinyscheme-1.42
.PHONY: all test test-tinyscheme clean
all: $(BIN)
$(BIN): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)
$(TEST_BOT_STATE): tests/test_bot_state.c src/bot_state.c src/bot_state.h src/irc_core.c src/irc_core.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_bot_state.c src/bot_state.c src/irc_core.c -o $(TEST_BOT_STATE)
$(TEST_BOT_CAPS): tests/test_bot_caps.c src/bot_caps.c src/bot_caps.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_bot_caps.c src/bot_caps.c -o $(TEST_BOT_CAPS)
$(TEST_BOT_RUNTIME): tests/test_bot_runtime.c src/bot_runtime.c src/bot_runtime.h src/bot_state.c src/bot_caps.c
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_bot_runtime.c src/bot_runtime.c src/bot_state.c src/bot_caps.c -o $(TEST_BOT_RUNTIME)
$(TEST_IRC): tests/test_irc.c src/irc_core.c src/irc_core.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_irc.c src/irc_core.c -o $(TEST_IRC)
$(TEST_DISPATCHER): tests/test_dispatcher.c src/irc_core.c src/dispatcher.c src/irc_core.h src/dispatcher.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_dispatcher.c src/irc_core.c src/dispatcher.c -o $(TEST_DISPATCHER)
$(TEST_EVENTS): tests/test_events.c src/irc_core.c src/events.c src/irc_core.h src/events.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_events.c src/irc_core.c src/events.c -o $(TEST_EVENTS)
$(TEST_RUNTIME): tests/test_runtime.c src/runtime_adapter.c src/scheme_backend.c src/runtime_adapter.h src/scheme_backend.h src/irc_core.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_runtime.c src/runtime_adapter.c src/scheme_backend.c -o $(TEST_RUNTIME)
$(TEST_RUNTIME_DISPATCH) $(TEST_TIMERS) $(TEST_TIMER_HANDLERS) $(TEST_RUNTIME_TIMERS): tests/test_runtime_dispatch.c src/irc_core.c src/runtime_adapter.c src/runtime_dispatch.c src/scheme_backend.c
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_runtime_dispatch.c src/irc_core.c src/runtime_adapter.c src/runtime_dispatch.c src/scheme_backend.c -o $(TEST_RUNTIME_DISPATCH)
$(TEST_TIMERS): tests/test_timers.c src/timers.c src/timers.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_timers.c src/timers.c -o $(TEST_TIMERS)
$(TEST_TIMER_HANDLERS): tests/test_timer_handlers.c src/timer_handlers.c src/timer_handlers.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_timer_handlers.c src/timer_handlers.c -o $(TEST_TIMER_HANDLERS)
$(TEST_RUNTIME_TIMERS): tests/test_runtime_timers.c src/runtime_adapter.c src/timers.c src/timer_handlers.c
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_runtime_timers.c src/runtime_adapter.c src/timers.c src/timer_handlers.c -o $(TEST_RUNTIME_TIMERS)

test: $(BIN) $(TEST_IRC) $(TEST_DISPATCHER) $(TEST_EVENTS) $(TEST_RUNTIME) $(TEST_RUNTIME_DISPATCH) $(TEST_BOT_STATE) $(TEST_BOT_CAPS) $(TEST_BOT_RUNTIME)
	@./$(BIN) | grep -q "LiCs M0"
	@./$(TEST_IRC)
	@./$(TEST_DISPATCHER)
	@./$(TEST_EVENTS)
	@./$(TEST_RUNTIME)
	@./$(TEST_RUNTIME_DISPATCH)
	@./$(TEST_BOT_STATE)
	@./$(TEST_BOT_CAPS)
	@./$(TEST_BOT_RUNTIME)
	@./$(TEST_TIMERS)
	@./$(TEST_TIMER_HANDLERS)
	@./$(TEST_RUNTIME_TIMERS)
	@echo "LiCs M1 tests: PASS"
test-tinyscheme:
	@test -f $(TINYSCHEME_DIR)/scheme.c || (echo "TinyScheme source missing at $(TINYSCHEME_DIR)"; exit 1)
	@mkdir -p build
	$(CC) $(CFLAGS) -DLICS_WITH_TINYSCHEME -DUSE_DL=0 -I$(TINYSCHEME_DIR) tests/test_runtime_dispatch.c src/irc_core.c src/runtime_adapter.c src/runtime_dispatch.c src/scheme_backend.c $(TINYSCHEME_DIR)/scheme.c -lm -o build/test_runtime_dispatch_scheme
	@./build/test_runtime_dispatch_scheme
	@echo "LiCs TinyScheme timer integration: PASS"
	@echo "LiCs TinyScheme IRC VM test: PASS"
clean:
	rm -rf build
