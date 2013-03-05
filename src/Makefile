MODULE=sysmon
SOURCE=$(wildcard *.c)
KERNEL_OBJ=$(MODULE).ko
OUT=$(MODULE).o
obj-m=$(SOURCE:.c=.o)
KDIR=/lib/modules/$(shell uname -r)/build
PWD=$(shell pwd)

.PHONY: all clean load unload

#default target
all: $(OUT)

$(MODULE).o: $(SOURCE) clean
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean: unload
	$(MAKE) -C $(KDIR) M=$(PWD) clean

load: unload $(MODULE).o
	/sbin/insmod $(MODULE).ko

unload:
	-/sbin/rmmod $(MODULE)
