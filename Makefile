obj-m := nf_df.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -fv *.[kmo]* ; find . -type f -iname ".*" -delete -print ; ls -al
