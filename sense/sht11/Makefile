#Makefile used for SHT11 chip demo


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
	$(CC) $(INCLUDES) -lm $(BCM2836_LOCATION)bcm2835.c RPi_SHT1x.c  sht11_read.c -o sht11_read

test:
	$(CC) $(CCFLAGS_TEST) -lm  $(BCM2836_LOCATION)bcm2835.c RPi_SHT1x.c sht11_read.c -o sht11_read

debug:
	$(CC) $(CCFLAGS) -lm $(BCM2836_LOCATION)bcm2835.c RPi_SHT1x.c sht11_read.c -o sht11_read

clean:
	rm -rf sht11_read

distclean:
	rm -rf sht11_read
	tar czvf sht11.tar.gz RPi_SHT1x.c sht11_read.c RPi_SHT1x.h bcm2835.c bcm2835.h Makefile
