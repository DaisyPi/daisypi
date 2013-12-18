# Makefile for BMP085 pressure sensor binaries ( using native I2C ) for version 1 and version 2 of the Rapsberry Pi board.

    all:
	gcc -lm -Wall -o pres_temp_v2.o /new_push/daisypi/3rdparty/smbus/smbus.c ./pres_temp_v2.c
	gcc -lm -Wall -o pres_temp_v1.o /new_push/daisypi/3rdparty/smbus/smbus.c ./pres_temp_v1.c
    clean:
	rm -rf *o