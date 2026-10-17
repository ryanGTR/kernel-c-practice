CC	= gcc
CFLAGS	= -Wall -Wextra -Werror -g -O0
# valgrind 在 Arch 壞掉，改用 ASan + UBSan 驗記憶體
SAN	= -fsanitize=address,undefined -fno-sanitize-recover=undefined

.PHONY: test clean

test: test_three_sum
	./test_three_sum

test_three_sum: test_three_sum.c three_sum.c three_sum.h
	$(CC) $(CFLAGS) $(SAN) test_three_sum.c three_sum.c -o $@

clean:
	rm -f test_three_sum
