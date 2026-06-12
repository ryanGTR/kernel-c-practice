CC	= gcc
CFLAGS	= -Wall -Wextra -g
PROGS	= bad_strcpy bad_strncpy my_strscpy my_strlen

.PHONY: all clean valgrind asan

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

# bad_strcpy / bad_strncpy 故意越界，valgrind 會抓到 stack write。
# my_strscpy / my_strlen 實作正確時應 0 errors。
valgrind: all
	@for p in $(PROGS); do \
		echo "=== valgrind ./$$p ==="; \
		valgrind --leak-check=full --show-leak-kinds=all ./$$p; \
	done

# ASan 對 stack overflow 的抓力比 valgrind 強，bad_strcpy 用這個更明顯。
asan:
	$(CC) -Wall -Wextra -g -fsanitize=address bad_strcpy.c -o bad_strcpy_asan
	$(CC) -Wall -Wextra -g -fsanitize=address bad_strncpy.c -o bad_strncpy_asan

clean:
	rm -f $(PROGS) bad_strcpy_asan bad_strncpy_asan
