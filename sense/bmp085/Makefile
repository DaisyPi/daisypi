#Makefile used for BMP085 chip demo


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

CCFLAGS = -g $(INCLUDES)
CCFLAGS_TEST = -Wall $(INCLUDES)
LDFLAGS = -lm


all:
	$(CC) $(INCLUDES) $(LDFLAGS) smbus.c bmp_read.c -o bmp_read

test:
	$(CC) $(CCFLAGS_TEST) $(LDFLAGS) smbus.c bmp_read.c -o bmp_read

debug:
	$(CC) $(CCFLAGS) $(LDFLAGS) smbus.c bmp_read.c -o bmp_read

clean:
	rm -rf bmp_read

distclean:
	rm -rf bmp_read
	tar czvf bmp085.tar.gz bmp_read.c smbus.c smbus.h Makefile
