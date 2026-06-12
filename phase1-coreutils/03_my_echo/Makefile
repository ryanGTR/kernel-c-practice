# SPDX-License-Identifier: GPL-2.0
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g

TARGET = my_echo

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $<

test: $(TARGET)
	@echo "=== compare with system echo ==="
	@echo "--- basic ---"
	diff <(./$(TARGET) hello world) <(echo hello world)
	@echo "--- no newline ---"
	diff <(./$(TARGET) -n hello) <(echo -n hello)
	@echo "--- no args ---"
	diff <(./$(TARGET)) <(echo)
	@echo "--- single arg ---"
	diff <(./$(TARGET) foo) <(echo foo)
	@echo "--- -n in middle is not flag ---"
	diff <(./$(TARGET) foo -n bar) <(echo foo -n bar)
	@echo "ALL TESTS PASSED"

clean:
	rm -f $(TARGET)

.PHONY: clean test
