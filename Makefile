TARGET:= usbdriver
PWD:= $(shell pwd)
EXTRA_CFLAGS:=-Wno-error
KERNDIR:= "/lib/modules/$(shell uname -r)/build/"
CC:=gcc

obj-m:= ${TARGET}.o

all:
	make -C ${KERNDIR} M=${PWD}  modules

clean:
	make -C ${KERNDIR} M=${PWD}  clean
