# Module Makefile
# See Documentation/kbuild/makefiles.txt
# and Documentation/kbuild/modules.txt

MODULE_NAME := sysmon

#=================
#Project Variables
#=================
#source files
SOURCE := interposer.c log.c uid.c toggle.c sysmon.c

#main module object
MODULE_OBJ := $(MODULE_NAME).o

#================
#Kernel Variables
#================
#$(MODULE_NAME)-objs are additional source objects, not including $(MODULE_NAME).o
$(MODULE_NAME)-objs := $(SOURCE:.c=.o)
$(MODULE_NAME)-objs -= $(MODULE_OBJ)

#linked module object file
KERNEL_OBJ := $(MODULE_NAME).ko
obj-m += $(MODULE_OBJ)

#===========
#Directories
#===========
KDIR := /lib/modules/`uname -r`/build
PWD := `pwd`

#always carry out these tasks -- don't do dependency timestamping
.PHONY: all clean load unload

#=======
#Targets
#=======
#default target
all: clean $(SOURCE)
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean: unload
	$(MAKE) -C $(KDIR) M=$(PWD) clean

load: unload all
	/sbin/insmod $(KERNEL_OBJ)

unload:
	-/sbin/rmmod $(MODULE_NAME)
