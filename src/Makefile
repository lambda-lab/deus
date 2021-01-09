obj-m := deus.o 
deus-objs := driver.o deus.o
CONFIG_MODULE_SIG=n


CFLAGS_deus.o := -DDEBUG

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
