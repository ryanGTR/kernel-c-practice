CC      = gcc
CFLAGS  = -g -O0 -Wall -Wextra
PROGS   = swap addr divmod

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROGS)
