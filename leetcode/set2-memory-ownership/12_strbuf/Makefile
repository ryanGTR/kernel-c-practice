CC      = gcc
CFLAGS  = -Wall -Wextra -g -O0 -std=c11
ASAN    = -fsanitize=address,undefined
TARGET  = 12_strbuf
SOL     = 12_solution

.PHONY: all run valgrind asan check solution clean

all: $(TARGET)

$(TARGET): starter.c
	$(CC) $(CFLAGS) starter.c -o $(TARGET)

$(TARGET)_asan: starter.c
	$(CC) $(CFLAGS) $(ASAN) starter.c -o $@

$(SOL): solution.c
	$(CC) $(CFLAGS) solution.c -o $(SOL)

run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
	         --error-exitcode=1 ./$(TARGET)

asan: $(TARGET)_asan
	./$(TARGET)_asan

check: valgrind asan
	@echo "=== ✅ ALL CHECKS PASSED ==="

solution: $(SOL)
	./$(SOL)

clean:
	rm -f $(TARGET) $(TARGET)_asan $(SOL)
