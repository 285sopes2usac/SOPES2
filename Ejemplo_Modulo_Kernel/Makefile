obj-m = vmouse.o
KVERSION = $(shell uname -r)

all:
	make -C /lib/modules/${KVERSION}/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean