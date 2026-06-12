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

.PHONY: clean test valgrind sanitize
