# SPDX-License-Identifier: GPL-2.0
#
# Kata: one part, five minutes, red -> green.
#
#   make N=01          compile + run (this is your red/green loop)
#   make gdb N=01      drop into gdb, break on the function under test
#   make asan N=01     ASan/LSan/UBSan (valgrind is broken on this box)
#   make style N=01    checkpatch -- kernel coding style
#   make check N=01    all three judges in a row -- the "done" gate
#   make list          show the katas
#   make clean
#
# The test is already written. Run it FIRST and watch it fail.
# A test you never saw fail is indistinguishable from assertTrue(true).

SHELL := /bin/bash
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g

N ?= 01
SRC = $(wildcard $(N)_*.c)
BIN = $(basename $(SRC))

SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer
CHECKPATCH = ../scripts/checkpatch.pl

# The function the kata is about -- gdb breaks here.
FN = $(shell sed -n 's/^ \* @fn: //p' $(SRC))

.DEFAULT_GOAL := run

guard:
	@test -n "$(SRC)" || { echo "no kata $(N). try: make list"; exit 1; }

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

run: guard
	@$(MAKE) --no-print-directory $(BIN)
	@./$(BIN)

gdb: guard
	@$(MAKE) --no-print-directory $(BIN)
	gdb -q -ex "break $(FN)" -ex run ./$(BIN)

asan: guard
	$(CC) -Wall -Wextra -std=c99 -g $(SANFLAGS) -o $(BIN)-asan $(SRC)
	./$(BIN)-asan
	@echo "SANITIZERS CLEAN"

style: guard
	perl $(CHECKPATCH) --no-tree --no-signoff -f $(SRC)

# The "done" gate: all three judges, in order, stop on the first failure.
# gcc/assert first (does it work), then asan (memory), then style (cosmetics).
check: guard
	@$(MAKE) --no-print-directory run   N=$(N)
	@$(MAKE) --no-print-directory asan  N=$(N)
	@$(MAKE) --no-print-directory style N=$(N)
	@echo "==> kata $(N): ALL GREEN (gcc + asan + style)"

list:
	@grep -h '^ \* @kata:' *.c | sed 's/^ \* @kata: //'

clean:
	rm -f $(basename $(wildcard *_*.c)) *-asan

.PHONY: guard run gdb asan style check list clean
