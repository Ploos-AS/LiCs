CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
BIN := build/lics
TEST_IRC := build/test_irc
TEST_DISPATCHER := build/test_dispatcher
TEST_EVENTS := build/test_events
TEST_RUNTIME := build/test_runtime
SRC := src/main.c src/irc_core.c src/dispatcher.c src/events.c src/runtime_adapter.c src/scheme_backend.c
TINYSCHEME_DIR ?= vendor/tinyscheme-1.42

.PHONY: all test test-tinyscheme clean
all: $(BIN)
$(BIN): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)
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
test: $(BIN) $(TEST_IRC) $(TEST_DISPATCHER) $(TEST_EVENTS) $(TEST_RUNTIME)
	@./$(BIN) | grep -q "LiCs M0"
	@./$(TEST_IRC)
	@./$(TEST_DISPATCHER)
	@./$(TEST_EVENTS)
	@./$(TEST_RUNTIME)
	@echo "LiCs M1 tests: PASS"
test-tinyscheme:
	@test -f $(TINYSCHEME_DIR)/scheme.c || (echo "TinyScheme source missing at $(TINYSCHEME_DIR)"; exit 1)
	@mkdir -p build
	$(CC) $(CFLAGS) -DLICS_WITH_TINYSCHEME -DUSE_DL=0 -I$(TINYSCHEME_DIR) tests/test_runtime.c src/runtime_adapter.c src/scheme_backend.c $(TINYSCHEME_DIR)/scheme.c -lm -o build/test_runtime_scheme
	@./build/test_runtime_scheme
	@echo "LiCs TinyScheme VM test: PASS"
clean:
	rm -rf build
