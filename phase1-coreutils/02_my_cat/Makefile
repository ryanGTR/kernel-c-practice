# SPDX-License-Identifier: GPL-2.0
SHELL := /bin/bash
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g

TARGET = my_cat

# Absolute path: a shell alias must never shadow the reference implementation.
CAT = /usr/bin/cat

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $<

test: $(TARGET)
	@echo "=== compare with system cat ==="
	@echo "--- single file ---"
	diff <(./$(TARGET) test.txt) <($(CAT) test.txt)
	@echo "--- line numbers ---"
	diff <(./$(TARGET) -n test.txt) <($(CAT) -n test.txt)
	@echo "--- stdin ---"
	diff <(./$(TARGET) < test.txt) <($(CAT) < test.txt)
	@echo "--- multiple files ---"
	diff <(./$(TARGET) test.txt test.txt) <($(CAT) test.txt test.txt)
	@echo "--- multiple files with -n ---"
	diff <(./$(TARGET) -n test.txt test.txt) <($(CAT) -n test.txt test.txt)
	@echo "--- dash means stdin ---"
	diff <(./$(TARGET) -n test.txt - < test.txt) <($(CAT) -n test.txt - < test.txt)
	@echo "--- missing file: stderr message ---"
	diff <(./$(TARGET) nonexist.txt 2>&1 >/dev/null | sed 's/^$(TARGET):/cat:/') \
	     <($(CAT) nonexist.txt 2>&1 >/dev/null)
	@echo "--- missing file: keep going, stdout intact ---"
	diff <(./$(TARGET) nonexist.txt test.txt 2>/dev/null) <($(CAT) test.txt)
	@echo "--- missing file: exit code ---"
	@./$(TARGET) nonexist.txt test.txt >/dev/null 2>&1; got=$$?; \
	 $(CAT) nonexist.txt test.txt >/dev/null 2>&1; want=$$?; \
	 if [ "$$got" != "$$want" ]; then \
		echo "FAIL: exit code $$got, want $$want"; exit 1; fi
	@echo "--- file with no trailing newline ---"
	@printf 'a' > .nonl1.txt && printf 'b\n' > .nonl2.txt
	diff <(./$(TARGET) .nonl1.txt .nonl2.txt) <($(CAT) .nonl1.txt .nonl2.txt)
	diff <(./$(TARGET) -n .nonl1.txt .nonl2.txt) <($(CAT) -n .nonl1.txt .nonl2.txt)
	@rm -f .nonl1.txt .nonl2.txt
	@echo "ALL TESTS PASSED"

valgrind: $(TARGET)
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET) test.txt

# valgrind can't run on this Arch box (stripped ld.so, no glibc debuginfo),
# so use the compiler's ASan/LSan/UBSan instead -- same memory guarantees,
# no debug symbols needed. A clean run prints no sanitizer report.
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

asan: $(TARGET).c
	$(CC) -Wall -Wextra -std=c99 -g $(SANFLAGS) -o $(TARGET)-asan $<
	@echo "=== ASan/LSan/UBSan: single file + -n ==="
	./$(TARGET)-asan -n test.txt
	@echo "=== ASan/LSan/UBSan: stdin via dash ==="
	printf 'z\n' | ./$(TARGET)-asan -n test.txt - test.txt
	@echo "=== ASan/LSan/UBSan: multiple + missing file ==="
	-./$(TARGET)-asan test.txt nonexistent test.txt
	@echo "ALL SANITIZER CHECKS CLEAN"

# gcc and checkpatch are two different judges: `make` says whether it RUNS,
# `make style` says whether it LOOKS like kernel code. Passing one proves
# nothing about the other.
CHECKPATCH = ../../scripts/checkpatch.pl

style: $(TARGET).c
	perl $(CHECKPATCH) --no-tree --no-signoff -f $<

clean:
	rm -f $(TARGET) $(TARGET)-asan .nonl1.txt .nonl2.txt

.PHONY: clean test valgrind asan style
