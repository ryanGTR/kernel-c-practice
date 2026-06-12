CC	= gcc
CFLAGS	= -Wall -Wextra -g -O0
PROGS	= basics sizeof_trap sum max_of oob
ASAN	= oob_asan

.PHONY: all clean valgrind asan

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

valgrind: all
	@for p in $(PROGS); do \
		echo "=== valgrind ./$$p ==="; \
		valgrind --error-exitcode=1 --track-origins=yes ./$$p; \
	done

# AddressSanitizer：抓 stack/heap 越界，valgrind 看不到 stack 越界，要靠它。
# ASan 偵測到錯誤會 exit != 0，這正常，前綴 - 讓 make 不要中斷。
asan: $(ASAN)
	@echo "=== ASan ./$(ASAN) ==="
	-./$(ASAN)

$(ASAN): oob.c
	$(CC) $(CFLAGS) -fsanitize=address $< -o $@

clean:
	rm -f $(PROGS) $(ASAN)
