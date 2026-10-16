CC	= gcc
CFLAGS	= -Wall -Wextra -Werror -g -O0
# valgrind 在 Arch 壞掉，改用 ASan + UBSan 驗記憶體
SAN	= -fsanitize=address,undefined -fno-sanitize-recover=undefined

.PHONY: test clean

test: test_two_sum
	./test_two_sum

test_two_sum: test_two_sum.c two_sum.c two_sum.h
	$(CC) $(CFLAGS) $(SAN) test_two_sum.c two_sum.c -o $@

clean:
	rm -f test_two_sum
