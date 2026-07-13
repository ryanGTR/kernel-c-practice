# SPDX-License-Identifier: GPL-2.0
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g

TARGET = my_sort

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $<

test: $(TARGET)
	@echo "=== compare with system sort ==="
	@echo "--- alphabetical ---"
	diff <(./$(TARGET) test.txt) <(sort test.txt)
	@echo "--- reverse ---"
	diff <(./$(TARGET) -r test.txt) <(sort -r test.txt)
	@echo "--- numeric ---"
	diff <(./$(TARGET) -n test.txt) <(sort -n test.txt)
	@echo "--- stdin ---"
	diff <(./$(TARGET) < test.txt) <(sort < test.txt)
	@echo "--- reverse numeric ---"
	diff <(./$(TARGET) -r -n test.txt) <(sort -r -n test.txt)
	@echo "ALL TESTS PASSED"

valgrind: $(TARGET)
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET) test.txt

clean:
	rm -f $(TARGET)

# gcc and checkpatch are two different judges: `make` says whether it RUNS,
# `make style` says whether it LOOKS like kernel code. Passing one proves
# nothing about the other.
CHECKPATCH = ../../scripts/checkpatch.pl

style: $(TARGET).c
	perl $(CHECKPATCH) --no-tree --no-signoff -f $<

.PHONY: clean test valgrind style
