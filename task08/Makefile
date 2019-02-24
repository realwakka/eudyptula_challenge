obj-m += helloworld.o
#KDIR := /usr/src/linux-headers-(shell uname -r)/build
KDIR := /usr/src/linux-headers-4.14.98-v7+

all:
		$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
		$(MAKE) -C $(KDIR) M=$(PWD) clean
