# SPDX-License-Identifier: GPL-2.0
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g

TARGET = my_grep

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $<

test: $(TARGET)
	@echo "=== compare with system grep ==="
	@echo "--- basic match ---"
	diff <(./$(TARGET) fox test.txt) <(grep -F fox test.txt)
	@echo "--- case insensitive ---"
	diff <(./$(TARGET) -i FOX test.txt) <(grep -Fi FOX test.txt)
	@echo "--- line numbers ---"
	diff <(./$(TARGET) -n the test.txt) <(grep -Fn the test.txt)
	@echo "--- count ---"
	diff <(./$(TARGET) -c the test.txt) <(grep -Fc the test.txt)
	@echo "--- no match exits 1 ---"
	./$(TARGET) zzzznotfound test.txt; test $$? -eq 1
	@echo "--- stdin ---"
	diff <(echo "hello world" | ./$(TARGET) hello) <(echo "hello world" | grep -F hello)
	@echo "ALL TESTS PASSED"

valgrind: $(TARGET)
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET) fox test.txt

clean:
	rm -f $(TARGET)

.PHONY: clean test valgrind
