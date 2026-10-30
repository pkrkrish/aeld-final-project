obj-m += rpi_chardev.o

BUILDROOT_DIR ?= $(HOME)/buildroot
KERNEL_DIR ?= $(BUILDROOT_DIR)/output/build/linux-custom
ARCH ?= arm64
CROSS_COMPILE ?= $(BUILDROOT_DIR)/output/host/bin/aarch64-linux-

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean