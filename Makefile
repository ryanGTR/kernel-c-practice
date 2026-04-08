# SPDX-License-Identifier: GPL-2.0
#
# Kernel C Practice - Top-level Makefile
# Mimics kernel build style with strict warnings

CC	= gcc
CFLAGS	= -Wall -Wextra -Werror -Wshadow -Wstrict-prototypes \
	  -Wold-style-definition -Wmissing-prototypes \
	  -Wmissing-declarations -Wdeclaration-after-statement \
	  -std=gnu11 -g -O2

export CC CFLAGS

EXERCISES := $(sort $(dir $(wildcard exercises/*/Makefile)))

.PHONY: all clean check test

all:
	@for dir in $(EXERCISES); do \
		echo "=== Building $$dir ==="; \
		$(MAKE) -C $$dir || exit 1; \
	done

test: all
	@for dir in $(EXERCISES); do \
		echo "=== Testing $$dir ==="; \
		$(MAKE) -C $$dir test || exit 1; \
	done

check:
	@echo "=== Running checkpatch ==="
	@for f in $$(find exercises -name '*.c' -o -name '*.h'); do \
		echo "--- $$f ---"; \
		perl scripts/checkpatch.pl --no-tree --no-signoff -f "$$f"; \
	done

clean:
	@for dir in $(EXERCISES); do \
		$(MAKE) -C $$dir clean; \
	done
