# SPDX-License-Identifier: GPL-2.0

TARGET		= main
SRCS		= main.c

PRACTICE_TARGET	= main_practice
PRACTICE_SRCS	= main_practice.c

.PHONY: all test clean

all: $(TARGET) $(PRACTICE_TARGET)

$(TARGET): $(SRCS) list.h
	$(CC) $(CFLAGS) -o $@ $(SRCS)

$(PRACTICE_TARGET): $(PRACTICE_SRCS) list.h
	$(CC) $(CFLAGS) -o $@ $(PRACTICE_SRCS)

test: $(TARGET) $(PRACTICE_TARGET)
	./$(TARGET)
	./$(PRACTICE_TARGET)

clean:
	rm -f $(TARGET) $(PRACTICE_TARGET) *.o
