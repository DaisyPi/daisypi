#Makefile used for adxl345 chip demo


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
#BCM2836_LOCATION = ../../daisypi/old_code/shtnew/bcm2835-1.25/src/
BCM2836_LOCATION = 

all:
	$(CC) $(INCLUDES) $(BCM2836_LOCATION)bcm2835.c adxl345.c communication.c adxl_read.c -o adxl345_read

test:
	$(CC) $(CCFLAGS_TEST) $(BCM2836_LOCATION)bcm2835.c adxl345.c communication.c adxl_read.c -o adxl345_read
	
debug:
	$(CC) $(CCFLAGS) $(BCM2836_LOCATION)bcm2835.c adxl345.c communication.c adxl_read.c -o adxl345_read
	
clean:
	rm -rf adxl345_read

distclean:
	rm -rf adxl345_read
	tar czvf adxl345.tar.gz adxl_read.c adxl345.c adxl345.h communication.c communication.h bcm2835.c bcm2835.h Makefile
