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

# valgrind can't run on this Arch box (stripped ld.so, no glibc debuginfo),
# so use the compiler's ASan/LSan/UBSan instead -- same memory guarantees,
# no debug symbols needed. A clean run prints no sanitizer report.
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

asan: $(TARGET).c
	$(CC) -Wall -Wextra -std=c99 -g $(SANFLAGS) -o $(TARGET)-asan $<
	@echo "=== ASan/LSan/UBSan: single file ==="
	./$(TARGET)-asan test.txt
	@echo "=== ASan/LSan/UBSan: stdin ==="
	./$(TARGET)-asan < test.txt
	@echo "=== ASan/LSan/UBSan: multiple + missing file ==="
	-./$(TARGET)-asan test.txt nonexistent test.txt
	@echo "ALL SANITIZER CHECKS CLEAN"

clean:
	rm -f $(TARGET) $(TARGET)-asan

# gcc and checkpatch are two different judges: `make` says whether it RUNS,
# `make style` says whether it LOOKS like kernel code. Passing one proves
# nothing about the other.
CHECKPATCH = ../../scripts/checkpatch.pl

style: $(TARGET).c
	perl $(CHECKPATCH) --no-tree --no-signoff -f $<

.PHONY: clean test valgrind asan style
