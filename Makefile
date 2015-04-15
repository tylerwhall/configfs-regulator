obj-m := userspace-regulator.o

SRC := $(shell pwd)

all:
	make -C $(KERNEL_SRC) M=$(SRC)

clean:
	make -C $(KERNEL_SRC) M=$(SRC) clean
