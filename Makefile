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
PHASE1    := $(sort $(dir $(wildcard phase1-coreutils/*/Makefile)))
PHASE2    := $(sort $(dir $(wildcard phase2-data-structures/*/Makefile)))
PHASE3    := $(sort $(dir $(wildcard phase3-system-tools/*/Makefile)))

ALL_USERSPACE := $(EXERCISES) $(PHASE1) $(PHASE2) $(PHASE3)

.PHONY: all clean check test exercises phases phase1 phase2 phase3

all:
	@for dir in $(ALL_USERSPACE); do \
		echo "=== Building $$dir ==="; \
		$(MAKE) -C $$dir || exit 1; \
	done

exercises:
	@for dir in $(EXERCISES); do \
		echo "=== Building $$dir ==="; \
		$(MAKE) -C $$dir || exit 1; \
	done

phases: phase1 phase2 phase3

phase1:
	@for dir in $(PHASE1); do \
		echo "=== Building $$dir ==="; \
		$(MAKE) -C $$dir || exit 1; \
	done

phase2:
	@for dir in $(PHASE2); do \
		echo "=== Building $$dir ==="; \
		$(MAKE) -C $$dir || exit 1; \
	done

phase3:
	@for dir in $(PHASE3); do \
		echo "=== Building $$dir ==="; \
		$(MAKE) -C $$dir || exit 1; \
	done

test: all
	@for dir in $(ALL_USERSPACE); do \
		echo "=== Testing $$dir ==="; \
		$(MAKE) -C $$dir test || exit 1; \
	done

check:
	@echo "=== Running checkpatch ==="
	@for f in $$(find exercises phase1-coreutils phase2-data-structures \
		phase3-system-tools -name '*.c' -o -name '*.h'); do \
		echo "--- $$f ---"; \
		perl scripts/checkpatch.pl --no-tree --no-signoff -f "$$f"; \
	done

clean:
	@for dir in $(ALL_USERSPACE); do \
		$(MAKE) -C $$dir clean; \
	done
