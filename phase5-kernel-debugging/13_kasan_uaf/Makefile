# SPDX-License-Identifier: GPL-2.0
obj-m += kasan_uaf.o

KDIR := /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

checkpatch:
	$(KDIR)/../../scripts/checkpatch.pl -f kasan_uaf.c || true

.PHONY: all clean checkpatch
