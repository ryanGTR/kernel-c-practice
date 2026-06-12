PROBLEM = 30_atomic_counter
LDFLAGS = -lpthread

include ../../Makefile.common

# 覆蓋預設規則加上 -lpthread
$(PROBLEM): starter.c
	$(CC) $(CFLAGS) starter.c -o $(PROBLEM) $(LDFLAGS)

$(PROBLEM)_asan: starter.c
	$(CC) $(CFLAGS) $(ASAN) starter.c -o $@ $(LDFLAGS)

$(PROBLEM)_solution: solution.c
	$(CC) $(CFLAGS) solution.c -o $@ $(LDFLAGS)
