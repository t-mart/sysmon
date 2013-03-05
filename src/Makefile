# Module Makefile
# See Documentation/kbuild/makefiles.txt
# and Documentation/kbuild/modules.txt

MODULE_NAME := sysmon

#source and object files
SOURCE := interposer.c log.c uid.c toggle.c sysmon.c
#use this line when we have multiple source files
$(MODULE_NAME)-objs := interposer.o log.o uid.o toggle.o

#linked module object file
KERNEL_OBJ := $(MODULE_NAME).ko
obj-m += $(MODULE_NAME).o

#dirs
KDIR := /lib/modules/`uname -r`/build
PWD := `pwd`

.PHONY: all clean load unload

#default target
all: unload clean $(SOURCE)
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean: unload
	$(MAKE) -C $(KDIR) M=$(PWD) clean

load: unload all
	/sbin/insmod $(KERNEL_OBJ)

unload:
	-/sbin/rmmod $(MODULE_NAME)
