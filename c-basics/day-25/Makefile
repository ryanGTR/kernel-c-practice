CC      = gcc
CFLAGS  = -g -O0 -Wall -Wextra
PROGS   = fib

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROGS)
