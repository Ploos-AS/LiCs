CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
BIN := build/lics
TEST_IRC := build/test_irc
TEST_DISPATCHER := build/test_dispatcher
TEST_RUNTIME := build/test_runtime
SRC := src/main.c src/irc_core.c src/dispatcher.c src/runtime_adapter.c

.PHONY: all test clean

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

$(TEST_RUNTIME): tests/test_runtime.c src/runtime_adapter.c src/runtime_adapter.h src/irc_core.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_runtime.c src/runtime_adapter.c -o $(TEST_RUNTIME)

test: $(BIN) $(TEST_IRC) $(TEST_DISPATCHER) $(TEST_RUNTIME)
	@./$(BIN) | grep -q "LiCs M0"
	@./$(TEST_IRC)
	@./$(TEST_DISPATCHER)
	@./$(TEST_RUNTIME)
	@echo "LiCs M0 tests: PASS"

clean:
	rm -rf build
