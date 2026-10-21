CC	= gcc
CFLAGS	= -Wall -Wextra -Werror -g -O0
# valgrind 在 Arch 壞掉，改用 ASan + UBSan 驗記憶體（含 leak）
SAN	= -fsanitize=address,undefined -fno-sanitize-recover=undefined

.PHONY: test clean

test: test_remove_nth
	./test_remove_nth

test_remove_nth: test_remove_nth.c remove_nth.c remove_nth.h
	$(CC) $(CFLAGS) $(SAN) test_remove_nth.c remove_nth.c -o $@

clean:
	rm -f test_remove_nth
