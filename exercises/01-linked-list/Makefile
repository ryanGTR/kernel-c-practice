# SPDX-License-Identifier: GPL-2.0

TARGET	= main
SRCS	= main.c

$(TARGET): $(SRCS) list.h
	$(CC) $(CFLAGS) -o $@ $(SRCS)

.PHONY: test clean

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o
