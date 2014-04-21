#Master Makefile used for DaisyPi sensors demo

# put common definitions in here
MKDIR  = mkdir -p
MOVE   = mv
REMOVE = rm -rf
SHELL  = /bin/bash
ECHO   = echo
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

#Add modules here.
MODULES = adxl345 bmp085 led mcp3208 sht11 tsl235r 
.PHONY: $(MODULES)
.SILENT:


all: $(MODULES)

$(MODULES):
	$(MAKE) -C $@ all
	$(ECHO) Building module "$@"...

#test: 
#	for module in $(MODULES); do   \
#       (cd $$module; $(MAKE) test); \
#   done;

#debug:
#	for module in $(MODULES); do   \
#       (cd $$module; $(MAKE) test); \
#   done;
	
clean:
	for module in $(MODULES); do   \
       (cd $$module; $(ECHO) Cleanining module "$$module"...; $(MAKE) clean); \
    done;

distclean:
	for module in $(MODULES); do   \
       (cd $$module; $(ECHO) Distclean module "$$module"...; $(MAKE) distclean); \
    done;

archivate:
	$(MKDIR) archives
	for module in $(MODULES); do   \
       (cd $$module; $(ECHO) Archivate module "$$module"...; $(MOVE) *.tar.gz ../archives); \
    done;
	$(ECHO)
	$(ECHO) Distclean module archives were moved into archives directory.

delarchive:
	$(ECHO) Remove archives directory.
	$(REMOVE) archives

