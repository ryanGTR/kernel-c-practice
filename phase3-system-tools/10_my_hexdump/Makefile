# SPDX-License-Identifier: GPL-2.0
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g

TARGET = my_hexdump

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $<

test.bin:
	@printf 'Hello, world!\n\x00\x01\x02\x03\xff\xfe' > test.bin

test: $(TARGET) test.bin
	@echo "=== compare with system hexdump -C ==="
	diff <(./$(TARGET) test.bin) <(hexdump -C test.bin)
	@echo "--- stdin ---"
	diff <(./$(TARGET) < test.bin) <(hexdump -C < test.bin)
	@echo "ALL TESTS PASSED"

valgrind: $(TARGET) test.bin
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET) test.bin

clean:
	rm -f $(TARGET) test.bin

# gcc and checkpatch are two different judges: `make` says whether it RUNS,
# `make style` says whether it LOOKS like kernel code. Passing one proves
# nothing about the other.
CHECKPATCH = ../../scripts/checkpatch.pl

style: $(TARGET).c
	perl $(CHECKPATCH) --no-tree --no-signoff -f $<

.PHONY: clean test valgrind style
