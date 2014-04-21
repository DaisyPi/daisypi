/***************************** FILE HEADER ***********************************/
/*!
*  \file    led.c
*
*  \brief   <b>Demo application blinks the status LED.</b>\n
*
*  \note    Some special notes
*
*  \author    Vaduva Jan Alexandru\n
*             Copyright 2013 Daisy Pi
*
*  \version 1.0 2013-08-11 Vaduva Jan Alexandru    created
*
*//**************************** FILE HEADER **********************************/

/******************************************************************************
* Includes
******************************************************************************/
#include "led.h"

/******************************************************************************
 * Defines
 *****************************************************************************/
/* GPIO setup macros. Always use INP_GPIO(x) before
 using OUT_GPIO(x) or SET_GPIO_ALT(x,y) */
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

/* sets   bits which are 1 ignores bits which are 0 */
#define GPIO_SET *(gpio+7)
/* clears bits which are 1 ignores bits which are 0 */
#define GPIO_CLR *(gpio+10)

//#define DEMO_ENABLED

/******************************************************************************
 * Local variables
 *****************************************************************************/
#ifdef DEMO_ENABLED
  char pins[] = { 1, 4, 7, 8, 9, 10, 11, 17, 18, 21, 22, 23, 24, 25 };
  //char pins[] = {25};
#else
  char pin = 4;
#endif

/******************************************************************************
* Local functions
******************************************************************************/
/*!
 *******************************************************************************
 *  setup_io
 *******************************************************************************
 *
 *  \brief      <b>Set up a memory regions to access GPIO.\n</b>
 *
 *  \author         Vaduva Jan Alexandru
 *
 *  \date           2013-08-13
 ******************************************************************************/
 void setup_io()
 {
  /* open /dev/mem */
  if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) 
  {
    printf("can't open /dev/mem \n");
    exit(-1);
  }

  /* mmap GPIO */
  gpio_map = mmap(
      NULL,                 // Any adddress in our space will do
      BLOCK_SIZE,           // Map length
      PROT_READ|PROT_WRITE, // Enable reading & writting to mapped memory
      MAP_SHARED,           // Shared with other processes
      mem_fd,               // File to map
      GPIO_BASE             // Offset to GPIO peripheral
      );

  close(mem_fd);

  if (gpio_map == MAP_FAILED) 
  {
    printf("mmap error %d\n", (int)gpio_map);
    exit(-1);
  }

  gpio = (volatile unsigned *)gpio_map;
}


int main(int argc, char **argv)
{
#ifdef DEMO_ENABLED
  int g,rep;
#endif

  /* Set up gpi pointer for direct register access */
  setup_io();
#ifndef DEMO_ENABLED  
  /* Set GPIO pin 4 to output */
  INP_GPIO(pin); 
  OUT_GPIO(pin);
#else
  /* Set all exposed GPIO pins to output */
  for (g = 0; g <= 32; g++)
  {
    if( ((1<<g) & 0x3e6cf93) != 0)
    {
      INP_GPIO(g);
      OUT_GPIO(g);
    }
  }
  GPIO_CLR = 0x3e6cf93; //clear all output pins
#endif

#ifdef DEMO_ENABLED
  while (1)
  {
    for (rep = 0; rep < 14; rep++)
    { 
      GPIO_SET = 1<<pins[rep];
      printf("Set pin: %d\n", pins[rep]);
      sleep(1);
      GPIO_CLR = 1<<pins[rep];
      printf("Clear pin: %d\n", pins[rep]);
      sleep(1);
    }
  }
#else
  while (1)
  {
    GPIO_SET = 1<<pin;
    printf("Set pin: %d\n", pin);
    sleep(1);
    GPIO_CLR = 1<<pin;
    printf("Clear pin: %d\n", pin);
    sleep(1);
  }
#endif

  return 0;
}
