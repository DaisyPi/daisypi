/***************************** FILE HEADER ***********************************/
/*!
*  \file		pres_temp.c
*
*  \brief		<b>Sensirion SHT11 Temperature and Humidity Sensor 
*				interfaced to Raspberry Pi GPIO port</b>\n
*
*				SHT pins (modified for DaisyPi own connections)
*				GND  - Connected to GPIO Port P1-06 (Ground)
*				DATA - Connected via a 10k pullup resistor 
*							to GPIO Port P1-01 (3V3 Power)
*				DATA - Connected to GPIO Port P1-11 (GPIO 17)
*				SCK  - Connected to GPIO Port P1-15 (GPIO 22)
*				VDD  - Connected to 3.3 V rail (3V3 Power)
*
*  \note		Make sure you use P1-01 / 3.3V NOT the 5V pin.s
*				License: CC BY-SA v3.0 - 
*				http://creativecommons.org/licenses/by-sa/3.0/
*
*  \author		Rares Plescan
*				John Burns (www.john.geek.nz)
*				Daesung Kim
*
*  \version	1.0	2013-08-13 Vaduva Jan Alexandru    updated
*
*//**************************** FILE HEADER **********************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdio.h>
#include "RPi_SHT1x.h"

/******************************************************************************
* Local functions
******************************************************************************/
/*!
 *******************************************************************************
 *  printTempAndHumidity
 *******************************************************************************
 *
 *  \brief      <b>Print temperature, dew point and humidity.\n</b>
 *
 *  \author         Rares Plescan
 *
 *  \date           2013-08-12
 ******************************************************************************/
void printTempAndHumidity(void)
{
	unsigned char noError = 1;  
	value humi_val, temp_val;
	float fDewPoint;
	
	/* Wait at least 11ms after power-up (chapter 3.1) */
	delay(20); 	
	/* Set up the SHT1x Data and Clock Pins */
	SHT1x_InitPins();	
	/* Reset the SHT1x */
	SHT1x_Reset();
	
	/* Request Temperature measurement */
	noError = SHT1x_Measure_Start( SHT1xMeaT );
	if (!noError)
		return;		
	/* Read Temperature measurement */
	noError = SHT1x_Get_Measure_Value( (unsigned short int*) &temp_val.i );
	if (!noError)
		return;
	/* Request Humidity Measurement */
	noError = SHT1x_Measure_Start( SHT1xMeaRh );
	if (!noError)
		return;		
	/* Read Humidity measurement */
	noError = SHT1x_Get_Measure_Value( (unsigned short int*) &humi_val.i );
	if (!noError)
		return;

	/* Convert intergers to float and calculate true values */
	temp_val.f = (float)temp_val.i;
	humi_val.f = (float)humi_val.i;
	
	/* Calculate Temperature, the Dew Point and Humidity */
	SHT1x_Calc(&humi_val.f, &temp_val.f);
	SHT1x_CalcDewpoint(humi_val.f ,temp_val.f, &fDewPoint);
	/* Print the Dew Point, Temperature and Humidity */
	printf("TEMP_HUMID_DEW %0.4f %0.4f %0.4f\n",temp_val.f,humi_val.f,fDewPoint);

}


int main ()
{
	/* Initialise the Raspberry Pi GPIO */
	if(!bcm2835_init())
		return 1;
	
	printTempAndHumidity();

	return 1;
}
