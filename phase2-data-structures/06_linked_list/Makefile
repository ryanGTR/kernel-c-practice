# SPDX-License-Identifier: GPL-2.0
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g

TARGET = test_linked_list
SRCS = main.c linked_list.c

$(TARGET): $(SRCS) linked_list.h
	$(CC) $(CFLAGS) -o $@ $(SRCS)

test: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET)

sanitize: $(SRCS) linked_list.h
	$(CC) $(CFLAGS) -fsanitize=address -o $(TARGET)_asan $(SRCS)
	./$(TARGET)_asan

clean:
	rm -f $(TARGET) $(TARGET)_asan

# gcc and checkpatch are two different judges: `make` says whether it RUNS,
# `make style` says whether it LOOKS like kernel code. Passing one proves
# nothing about the other.
CHECKPATCH = ../../scripts/checkpatch.pl

style: $(TARGET).c
	perl $(CHECKPATCH) --no-tree --no-signoff -f $<

.PHONY: clean test valgrind sanitize style
