# SPDX-License-Identifier: GPL-2.0
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g

TARGET = my_cat

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $<

test: $(TARGET)
	@echo "=== compare with system cat ==="
	@echo "--- single file ---"
	diff <(./$(TARGET) test.txt) <(cat test.txt)
	@echo "--- line numbers ---"
	diff <(./$(TARGET) -n test.txt) <(cat -n test.txt)
	@echo "--- stdin ---"
	diff <(./$(TARGET) < test.txt) <(cat < test.txt)
	@echo "--- multiple files ---"
	diff <(./$(TARGET) test.txt test.txt) <(cat test.txt test.txt)
	@echo "--- multiple files with -n ---"
	diff <(./$(TARGET) -n test.txt test.txt) <(cat -n test.txt test.txt)
	@echo "ALL TESTS PASSED"

valgrind: $(TARGET)
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET) test.txt

clean:
	rm -f $(TARGET)

.PHONY: clean test valgrind
