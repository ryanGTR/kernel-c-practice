# SPDX-License-Identifier: GPL-2.0
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g

TARGET = my_wc

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $<

test: $(TARGET)
	@echo "=== compare with system wc ==="
	@echo "--- single file ---"
	diff <(./$(TARGET) test.txt) <(wc test.txt)
	@echo "--- stdin ---"
	diff <(./$(TARGET) < test.txt) <(wc < test.txt)
	@echo "--- multiple files ---"
	diff <(./$(TARGET) test.txt test.txt) <(wc test.txt test.txt)
	@echo "--- missing file ---"
	./$(TARGET) nonexistent 2>/dev/null; test $$? -ne 0
	@echo "ALL TESTS PASSED"

valgrind: $(TARGET)
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET) test.txt

clean:
	rm -f $(TARGET)

.PHONY: clean test valgrind
