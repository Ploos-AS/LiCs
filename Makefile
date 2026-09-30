CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
BIN := build/lics
TEST_IRC := build/test_irc
TEST_DISPATCHER := build/test_dispatcher
SRC := src/main.c src/irc_core.c src/dispatcher.c

.PHONY: all test clean

all: $(BIN)

$(BIN): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

$(TEST_IRC): tests/test_irc.c src/irc_core.c src/irc_core.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_irc.c src/irc_core.c -o $(TEST_IRC)

$(TEST_DISPATCHER): tests/test_dispatcher.c src/irc_core.c src/irc_core.h src/dispatcher.c src/dispatcher.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_dispatcher.c src/irc_core.c src/dispatcher.c -o $(TEST_DISPATCHER)

test: $(BIN) $(TEST_IRC) $(TEST_DISPATCHER)
	@./$(BIN) | grep -q "LiCs M0"
	@./$(TEST_IRC)
	@./$(TEST_DISPATCHER)
	@echo "LiCs M0 tests: PASS"

clean:
	rm -rf build
