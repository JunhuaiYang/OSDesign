ifneq ($(KERNELRELEASE),)    
#kbuild syntax.
mymodule-objs :=MyDriver.o
obj-m :=MyDriver.o
# mymodule-objs :=try.o
# obj-m :=try.o
else
PWD :=$(shell pwd)
KVER :=$(shell uname -r)
KDIR :=/lib/modules/$(KVER)/build
all:
	$(MAKE) -C $(KDIR) M=$(PWD)
clean:
	rm -f *.cmd *.o *.mod *.ko *.cmd
endif