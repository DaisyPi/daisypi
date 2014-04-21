#Makefile used for status LED demo


UNAME := $(shell uname -m)

ifeq ($(UNAME), x86_64)
	CC := arm-none-linux-gnueabi-gcc
	INCLUDES := -I ../../RaspberryPi_LinuxKernel/include
else
	CC := gcc
	INCLUDES :=
endif
ifeq ($(UNAME), x86)
	CC := arm-none-linux-gnueabi-gcc
	INCLUDES := -I ../../RaspberryPi_LinuxKernel/include
endif

CCFLAGS := -g $(INCLUDES)
CCFLAGS_TEST := -Wall $(INCLUDES)


all:
	$(CC) $(INCLUDES) led.c -o led

test:
	$(CC) $(CCFLAGS_TEST) led.c -o led
	
debug:
	$(CC) $(CCFLAGS) led.c -o led
	
clean:
	rm -rf led

distclean:
	rm -rf led
	tar czvf led.tar.gz led.c led.h Makefile