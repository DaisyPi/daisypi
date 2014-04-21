/***************************** FILE HEADER ***********************************/
/*!
*  \file		bmp_read.c
*
*  \brief		<b>BMP085 breakout board. This comes with pull up resistors 
*				already on the i2c lines. BMP085 pins below are as marked on 
*				the Sparkfun BMP085 Breakout board</b>\n
*
*				SDA		- 	P1-03 / IC20-SDA
*				XCLR	- 	Not Connected
*				EOC		-	Not Connected
*				GND		-	P1-06 / GND
*				VCC		- 	P1-01 / 3.3V
*
*  \note		Make sure you use P1-01 / 3.3V NOT the 5V pin.s
*				License: CC BY-SA v3.0 - 
*				http://creativecommons.org/licenses/by-sa/3.0/
*
*  \author		John Burns (www.john.geek.nz)
*				Jim Lindblom(SparkFun Electronics)
*
*  \version	1.0	2013-08-13 Vaduva Jan Alexandru    updated
*
*//**************************** FILE HEADER **********************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
 
#include "smbus.h" 

/******************************************************************************
 * Defines
 *****************************************************************************/
#define BMP085_I2C_ADDRESS 0x77

/******************************************************************************
 * Local variables
 *****************************************************************************/
 const unsigned char BMP085_OVERSAMPLING_SETTING = 3;

/* Calibration values - These are stored in the BMP085 */
 short int ac1;
 short int ac2; 
 short int ac3; 
 unsigned short int ac4;
 unsigned short int ac5;
 unsigned short int ac6;
 short int b1; 
 short int b2;
 short int mb;
 short int mc;
 short int md;

 int b5; 

 unsigned int temperature, pressure;

/******************************************************************************
* Local functions
******************************************************************************/
/*!
 *******************************************************************************
 *  bmp085_i2c_Begin
 *******************************************************************************
 *
 *  \brief      <b>Opens a connection to the bmp085.\n</b>
 *
 *	\return 		file descriptor
 *
 *  \author         
 *
 *  \date           2013-08-13
 ******************************************************************************/
 int bmp085_i2c_Begin()
 {
 	int fd;
 	char *fileName = "/dev/i2c-0";
 	
	/* Open port for reading and writing */
 	if ((fd = open(fileName, O_RDWR)) < 0)
 		exit(1);
 	
	/* Set the port options and set the address of the device */
 	if (ioctl(fd, I2C_SLAVE, BMP085_I2C_ADDRESS) < 0) 
 	{					
 		close(fd);
 		exit(1);
 	}

 	return fd;
 }

/*!
 *******************************************************************************
 *	bmp085_i2c_Read_Int
 *******************************************************************************
 *
 *  \brief		<b>Read two words from the BMP085 and supply it as a 
 *				16 bit integer.\n</b>
 *
 *  \param[in]		int			 file descriptor
 *  \param[in]		__u8     	 address from where to read data
 *
 *	\return 		two words read(32 bits in total)
 *
 *  \author         
 *
 *  \date           2013-08-13
 ******************************************************************************/
 __s32 bmp085_i2c_Read_Int(int fd, __u8 address)
 {
 	__s32 res = i2c_smbus_read_word_data(fd, address);
 	if (res < 0) 
 	{
 		close(fd);
 		exit(1);
 	}

	/* Convert result to 16 bits and swap bytes */
 	res = ((res<<8) & 0xFF00) | ((res>>8) & 0xFF);

 	return res;
 }

/*!
 *******************************************************************************
 *	bmp085_i2c_Write_Byte
 *******************************************************************************
 *
 *  \brief		<b>Write a byte to the BMP085.\n</b>
 *
 *  \param[in]		int			 file descriptor
 *  \param[in]		__u8     	 address where to write
 *  \param[in]		__u8     	 value to write
 *
 *  \author         
 *
 *  \date           2013-08-13
 ******************************************************************************/
 void bmp085_i2c_Write_Byte(int fd, __u8 address, __u8 value)
 {
 	if (i2c_smbus_write_byte_data(fd, address, value) < 0) 
 	{
 		close(fd);
 		exit(1);
 	}
 }

/*!
 *******************************************************************************
 *	bmp085_i2c_Read_Block
 *******************************************************************************
 *
 *  \brief		<b>Read a block of data BMP085.\n</b>
 *
 *  \param[in]		int			 file descriptor
 *  \param[in]		__u8     	 address from where to read
 *  \param[in]		__u8     	 length of the block that is read
 *  \param[out]		__u8*     	 output result read
 *
 *  \author         
 *
 *  \date           2013-08-13
 ******************************************************************************/
 void bmp085_i2c_Read_Block(int fd, __u8 address, __u8 length, __u8 *values)
 {
 	if(i2c_smbus_read_i2c_block_data(fd, address,length,values) < 0) 
 	{
 		close(fd);
 		exit(1);
 	}
 }

/*!
 *******************************************************************************
 *  bmp085_Calibration
 *******************************************************************************
 *
 *  \brief      <b>Opens and calibrates the BMP085 connection.\n</b>
 *
 *  \author         
 *
 *  \date           2013-08-13
 ******************************************************************************/
 void bmp085_Calibration()
 {
 	int fd = bmp085_i2c_Begin();

 	ac1 = bmp085_i2c_Read_Int(fd, 0xAA);
 	ac2 = bmp085_i2c_Read_Int(fd, 0xAC);
 	ac3 = bmp085_i2c_Read_Int(fd, 0xAE);
 	ac4 = bmp085_i2c_Read_Int(fd, 0xB0);
 	ac5 = bmp085_i2c_Read_Int(fd, 0xB2);
 	ac6 = bmp085_i2c_Read_Int(fd, 0xB4);

 	b1 = bmp085_i2c_Read_Int(fd, 0xB6);
 	b2 = bmp085_i2c_Read_Int(fd, 0xB8);

 	mb = bmp085_i2c_Read_Int(fd, 0xBA);
 	mc = bmp085_i2c_Read_Int(fd, 0xBC);
 	md = bmp085_i2c_Read_Int(fd, 0xBE);

 	close(fd);
 }

/*!
 *******************************************************************************
 *  bmp085_ReadUT
 *******************************************************************************
 *
 *  \brief      <b>Read the uncompensated temperature value.\n</b>
 *
 *	\return 		temperature value
 *
 *  \author         
 *
 *  \date           2013-08-13
 ******************************************************************************/
 unsigned int bmp085_ReadUT()
 {
 	unsigned int ut = 0;
 	int fd = bmp085_i2c_Begin();

	/* Write 0x2E into Register 0xF4. This requests a temperature reading */
 	bmp085_i2c_Write_Byte(fd, 0xF4, 0x2E);	
	/* Wait at least 4.5ms */
 	usleep(5000);
	/* Read the two byte result from address 0xF6 */
 	ut = bmp085_i2c_Read_Int(fd, 0xF6);

	/* Close the i2c file */
 	close (fd);
 	
 	return ut;
 }

/*!
 *******************************************************************************
 *  bmp085_ReadUP
 *******************************************************************************
 *
 *  \brief      <b>Read the uncompensated pressure value.\n</b>
 *
 *	\return 		pressure value
 *
 *  \author         
 *
 *  \date           2013-08-13
 ******************************************************************************/
 unsigned int bmp085_ReadUP()
 {
 	unsigned int up = 0;
 	int fd = bmp085_i2c_Begin();

	/* Write 0x34+(BMP085_OVERSAMPLING_SETTING<<6) into register 0xF4
		 Request a pressure reading w/ oversampling setting */
 	bmp085_i2c_Write_Byte(fd,0xF4,0x34 + (BMP085_OVERSAMPLING_SETTING<<6));
	/* Wait for conversion, delay time dependent on oversampling setting */
 	usleep((2 + (3<<BMP085_OVERSAMPLING_SETTING)) * 1000);
	/* Read the three byte result from 0xF6.
		 0xF6 = MSB, 0xF7 = LSB and 0xF8 = XLSB */
 	__u8 values[3];
 	bmp085_i2c_Read_Block(fd, 0xF6, 3, values);
 	up = (((unsigned int) values[0] << 16) | ((unsigned int) values[1] << 8) 
 		| (unsigned int) values[2]) >> (8-BMP085_OVERSAMPLING_SETTING);

	/* Close the i2c file */
 	close (fd);
 	
 	return up;
 }

/*!
 *******************************************************************************
 *  bbmp085_GetPressure
 *******************************************************************************
 *
 *  \brief      <b>Calculate pressure given uncalibrated pressure.
 *				Value returned will be in units of Pa.\n</b>
 *
 *  \param[in]		unsigned int 		uncompensated pressure
 *
 *	\return 		pressure value
 *
 *  \author         
 *
 *  \date           2013-08-13
 ******************************************************************************/
 unsigned int bmp085_GetPressure(unsigned int up)
 {
 	int x1, x2, x3, b3, b6, p;
 	unsigned int b4, b7;
 	
 	b6 = b5 - 4000;
	/* Calculate B3 */
 	x1 = (b2 * (b6 * b6)>>12)>>11;
 	x2 = (ac2 * b6)>>11;
 	x3 = x1 + x2;
 	b3 = (((((int)ac1)*4 + x3)<<BMP085_OVERSAMPLING_SETTING) + 2)>>2;
 	
	/* Calculate B4 */
 	x1 = (ac3 * b6)>>13;
 	x2 = (b1 * ((b6 * b6)>>12))>>16;
 	x3 = ((x1 + x2) + 2)>>2;
 	b4 = (ac4 * (unsigned int)(x3 + 32768))>>15;
 	
 	b7 = ((unsigned int)(up - b3) * (50000>>BMP085_OVERSAMPLING_SETTING));
 	if (b7 < 0x80000000)
 		p = (b7<<1)/b4;
 	else
 		p = (b7/b4)<<1;
 	
 	x1 = (p>>8) * (p>>8);
 	x1 = (x1 * 3038)>>16;
 	x2 = (-7357 * p)>>16;
 	p += (x1 + x2 + 3791)>>4;
 	
 	return p;
 }

/*!
 *******************************************************************************
 *  bbmp085_GetPressure
 *******************************************************************************
 *
 *  \brief      <b>Calculate temperature given uncalibrated temperature.
 *				Value returned will be in units of 0.1 deg C.\n</b>
 *
 *  \param[in]		unsigned int 		uncompensated temperature
 *
 *	\return 		temperature value
 *
 *  \author         
 *
 *  \date           2013-08-13
 ******************************************************************************/
 unsigned int bmp085_GetTemperature(unsigned int ut)
 {
 	int x1, x2;
 	unsigned int result;
 	
 	x1 = (((int)ut - (int)ac6)*(int)ac5) >> 15;
 	x2 = ((int)mc << 11)/(x1 + md);
 	b5 = x1 + x2;

 	result = ((b5 + 8)>>4);
 	return result;
 }


 int main(int argc, char **argv)
 {
 	double alt, alt_anal;
 	double pr;

 	bmp085_Calibration();
 	temperature = bmp085_GetTemperature(bmp085_ReadUT());
 	pressure = bmp085_GetPressure(bmp085_ReadUP());
 	pr=(double)pressure/100-0.3;
 	alt = 44330 * (1-pow((double)(pressure/100+6)/1011.25, 0.190294957));
 	alt_anal = 44330 * (1-pow((double)pressure/101325, 0.190294957));

 	printf("TEMP_PRESS_ALTC_ALTA %0.4f %0.4f %0.4f %0.4f \n",((double)temperature)/10,
 		((double)pressure)/100,alt,alt_anal);

 	return 0;

 }