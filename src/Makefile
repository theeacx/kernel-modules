C_FILES := $(wildcard *.c)
obj-m := $(C_FILES:.c=.o)
KDIR ?= $(HOME)/rpi-linux-6.12

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- KBUILD_MODPOST_WARN=1 modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- clean
