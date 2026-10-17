# SPDX-License-Identifier: GPL-2.0
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

TARGET = test_pp
SRCS = test_pp.c pp.c
HDRS = pp.h

$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

test: $(TARGET)
	./$(TARGET)

asan: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(SANFLAGS) -o $(TARGET)-asan $(SRCS)
	./$(TARGET)-asan
	@echo "ALL SANITIZER CHECKS CLEAN"

style:
	perl ../../../scripts/checkpatch.pl --no-tree --no-signoff -f pp.c pp.h

clean:
	rm -f $(TARGET) $(TARGET)-asan

.PHONY: test asan style clean
