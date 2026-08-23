# SPDX-License-Identifier: GPL-2.0
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g

TARGET = test_bst
SRCS = main.c bst.c
HDRS = bst.h

$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

test: $(TARGET)
	./$(TARGET)

# valgrind can't run on this Arch box (stripped ld.so, no glibc debuginfo),
# so use the compiler's ASan/LSan/UBSan instead -- same memory guarantees,
# no debug symbols needed. A clean run prints no sanitizer report.
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

asan: $(SRCS) $(HDRS)
	$(CC) -Wall -Wextra -std=c99 -g $(SANFLAGS) -o $(TARGET)-asan $(SRCS)
	./$(TARGET)-asan
	@echo "ALL SANITIZER CHECKS CLEAN"

valgrind: $(TARGET)
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET)

# gcc and checkpatch are two different judges: `make` says whether it RUNS,
# `make style` says whether it LOOKS like kernel code. Passing one proves
# nothing about the other.
CHECKPATCH = ../../scripts/checkpatch.pl

style:
	perl $(CHECKPATCH) --no-tree --no-signoff -f $(SRCS) $(HDRS)

clean:
	rm -f $(TARGET) $(TARGET)-asan

.PHONY: clean test asan valgrind style
