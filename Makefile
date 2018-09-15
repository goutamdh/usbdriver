TARGET:= go_driver
PWD:= $(shell pwd)
KERNDIR:= "/lib/modules/$(shell uname -r)/build/"
CC:=gcc

obj-m:= ${TARGET}.o

all:
	make -C ${KERNDIR} M=${PWD}

clean:
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symvers
