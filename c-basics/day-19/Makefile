CC	= gcc
CFLAGS	= -Wall -Wextra -g
PROGS	= basic arrays calloc_demo strbuf errors_leak errors_uaf errors_dfree

.PHONY: all clean valgrind

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

# 跑全部 valgrind。
# errors_leak 會故意噴 "definitely lost"。
# errors_uaf / errors_dfree 預設無 UB（你要拿掉註解後才會觸發）。
# --error-exitcode=1 讓 valgrind 抓到問題時回傳非零，方便 CI 模式。
valgrind: all
	@for p in $(PROGS); do \
		echo "=== valgrind ./$$p ==="; \
		valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$$p; \
	done

clean:
	rm -f $(PROGS)
