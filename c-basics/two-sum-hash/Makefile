CC	= gcc
CFLAGS	= -Wall -Wextra -Werror -g -O0
# valgrind 在 Arch 壞掉，改用 ASan + UBSan 驗記憶體
SAN	= -fsanitize=address,undefined -fno-sanitize-recover=undefined

.PHONY: test clean

test: test_two_sum_hash
	./test_two_sum_hash

test_two_sum_hash: test_two_sum_hash.c two_sum_hash.c two_sum_hash.h
	$(CC) $(CFLAGS) $(SAN) test_two_sum_hash.c two_sum_hash.c -o $@

clean:
	rm -f test_two_sum_hash
