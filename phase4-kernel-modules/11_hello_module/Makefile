# SPDX-License-Identifier: GPL-2.0
obj-m += hello.o

KDIR := /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

checkpatch:
	$(KDIR)/../../scripts/checkpatch.pl -f hello.c || true

.PHONY: all clean checkpatch
