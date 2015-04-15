configfs-regulator-y := configfs.o regulator.o

obj-m := configfs-regulator.o

SRC := $(shell pwd)

all:
	make -C $(KERNEL_SRC) M=$(SRC)

clean:
	make -C $(KERNEL_SRC) M=$(SRC) clean
