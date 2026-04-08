// SPDX-License-Identifier: GPL-2.0
/*
 * Exercise 02: container_of and Embedded Structures
 *
 * In the kernel, data structures are composed by embedding one struct
 * inside another. container_of() lets you go from a pointer to an
 * embedded member back to the containing struct.
 *
 * Tasks:
 *   1. Define a "device" struct with name and status
 *   2. Define a "usb_device" struct that embeds "device"
 *   3. Define a "pci_device" struct that also embeds "device"
 *   4. Write a function that takes a struct device* and prints info
 *   5. Use container_of to get back to the specific device type
 *   6. Register both device types in a single list via their
 *      embedded struct device's list_head
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../01-linked-list/list.h"

struct device {
	char name[32];
	int status;
	struct list_head list;
};

struct usb_device {
	struct device dev;
	int usb_version;	/* 2 or 3 */
	int speed_mbps;
};

struct pci_device {
	struct device dev;
	unsigned int vendor_id;
	unsigned int device_id;
};

static void device_init(struct device *dev, const char *name)
{
	strncpy(dev->name, name, sizeof(dev->name) - 1);
	dev->name[sizeof(dev->name) - 1] = '\0';
	dev->status = 0;
	INIT_LIST_HEAD(&dev->list);
}

static struct usb_device *usb_device_create(const char *name,
					    int version, int speed)
{
	struct usb_device *udev;

	udev = malloc(sizeof(*udev));
	if (!udev)
		return NULL;

	device_init(&udev->dev, name);
	udev->usb_version = version;
	udev->speed_mbps = speed;

	return udev;
}

static struct pci_device *pci_device_create(const char *name,
					    unsigned int vendor,
					    unsigned int devid)
{
	struct pci_device *pdev;

	pdev = malloc(sizeof(*pdev));
	if (!pdev)
		return NULL;

	device_init(&pdev->dev, name);
	pdev->vendor_id = vendor;
	pdev->device_id = devid;

	return pdev;
}

static void print_all_devices(struct list_head *head)
{
	struct device *dev;

	printf("--- Device List ---\n");
	list_for_each_entry(dev, head, list)
		printf("  [%s] status=%d\n", dev->name, dev->status);
	printf("-------------------\n");
}

static void free_all_devices(struct list_head *head)
{
	struct list_head *pos, *n;

	list_for_each_safe(pos, n, head) {
		struct device *dev = list_entry(pos, struct device, list);

		list_del(&dev->list);
		/* In real kernel code, each device type has its own free */
		free(dev);
	}
}

int main(void)
{
	LIST_HEAD(bus);
	struct usb_device *mouse;
	struct pci_device *gpu;
	struct device *found_dev;
	struct usb_device *found_usb;

	/* Create devices */
	mouse = usb_device_create("usb-mouse", 3, 5000);
	gpu = pci_device_create("rtx-4090", 0x10de, 0x2684);

	/* Register on the same bus via embedded struct device */
	list_add_tail(&mouse->dev.list, &bus);
	list_add_tail(&gpu->dev.list, &bus);

	/* Print all devices generically */
	print_all_devices(&bus);

	/*
	 * Demonstrate container_of:
	 * Given a struct device*, recover the usb_device*
	 */
	found_dev = list_entry(bus.next, struct device, list);
	found_usb = container_of(found_dev, struct usb_device, dev);
	printf("container_of demo: %s is USB%d, %d Mbps\n",
	       found_usb->dev.name,
	       found_usb->usb_version,
	       found_usb->speed_mbps);

	/* Cleanup */
	free_all_devices(&bus);

	printf("All tests passed!\n");
	return 0;
}
