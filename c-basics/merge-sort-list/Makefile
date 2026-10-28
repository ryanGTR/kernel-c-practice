CC	= gcc
CFLAGS	= -Wall -Wextra -Werror -g -O0
# valgrind 在 Arch 壞掉，改用 ASan + UBSan 驗記憶體（含 leak）
SAN	= -fsanitize=address,undefined -fno-sanitize-recover=undefined

.PHONY: test clean

test: test_merge_sort
	./test_merge_sort

test_merge_sort: test_merge_sort.c merge_sort.c merge_sort.h
	$(CC) $(CFLAGS) $(SAN) test_merge_sort.c merge_sort.c -o $@

clean:
	rm -f test_merge_sort
