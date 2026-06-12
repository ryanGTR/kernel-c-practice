CC	= gcc
CFLAGS	= -Wall -Wextra -g
PROGS	= step1 step2 list_demo

.PHONY: all clean valgrind

all: $(PROGS)

# 每個 step 的 main + 共用 student.c
step1: step1.c student.c student.h
	$(CC) $(CFLAGS) step1.c student.c -o step1

step2: step2.c student.c student.h
	$(CC) $(CFLAGS) step2.c student.c -o step2

list_demo: list_demo.c student.c student.h
	$(CC) $(CFLAGS) list_demo.c student.c -o list_demo

# valgrind 跑全部。step2 / list_demo 必須 0 leaks。
# step1 是 stack only 不會配 heap，valgrind 也應該乾淨。
valgrind: all
	@for p in $(PROGS); do \
		echo "=== valgrind ./$$p ==="; \
		valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$$p; \
	done

clean:
	rm -f $(PROGS)
