CC      = gcc
CFLAGS  = -g -O0 -Wall -Wextra
PROGS   = segfault loop

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROGS) core core.*
