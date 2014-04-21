/***************************** FILE HEADER ***********************************/
/*!
*  \file    tsl_read.c
*
*  \brief   <b>Read TSL235R sensor connected to GPIO pin by measuring 
*             average time for a defined number of cycles.</b>\n
*
*       
*
*  \note    Output is calculated as kHz or according to docs, in uW/cm2
*
*  \author  Rares Plescan
*
*  \version 1.0 2013-08-13 Vaduva Jan Alexandru    updated
*
*//**************************** FILE HEADER **********************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
//#include <wiringPi.h>

/******************************************************************************
 * Defines
 *****************************************************************************/
#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000)

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

/* GPIO setup macros. Always use INP_GPIO(x) before
 using OUT_GPIO(x) or SET_GPIO_ALT(x,y) */
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

/* sets   bits which are 1 ignores bits which are 0 */
#define GPIO_SET *(gpio+7)
/* clears bits which are 1 ignores bits which are 0 */
#define GPIO_CLR *(gpio+10)
 /* reads pin g value */
#define GPIO_READ(g)    *(gpio + 13) &= (1<<(g))

/******************************************************************************
 * Local variables
 *****************************************************************************/
struct timespec tp1, tp2;
const int tsl_pin = 1 ;     // GPIO18, pin 12
int  mem_fd;
void *gpio_map;

/* I/O access */
 volatile unsigned *gpio;

/******************************************************************************
* Local functions
******************************************************************************/
/*!
 *******************************************************************************
 *  time_diff
 *******************************************************************************
 *
 *  \brief        <b>Returns the time estimated between events.\n</b>
 *
 *  \param[in]    struct timespec   start time
 *  \param[in]    struct timespec   finish time
 *
 *  \return       long int          event duration
 *
 *  \author       Rares Plescan
 *
 *  \date         2013-08-12
 ******************************************************************************/
unsigned long int time_diff(struct timespec start, struct timespec stop)
{
  unsigned long int r1;
 
  r1 = (unsigned long int) ((stop.tv_sec - start.tv_sec)*1000000000 + stop.tv_nsec - start.tv_nsec );
 
  return r1;
}

/*!
 *******************************************************************************
 *  count_wait_tsl
 *******************************************************************************
 *
 *  \brief        <b>Offers a delay for the selected TSL235R pin.\n</b>
 *
 *  \param[in]    int   number of cycles
 *
 *  \author       Rares Plescan
 *
 *  \date         2013-08-12
 ******************************************************************************/
void count_wait_tsl(unsigned int cycles)
{
  int count = 0;

  while ( count < cycles ) 
  {
    while ( GPIO_READ(tsl_pin) == 0 ) {}
    count++;
    while ( GPIO_READ(tsl_pin) == 1 ) {}
    count++;
  }
}

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


int main(int argc, char* argv[])
{

  long timp1, timp2, timp3;
  int poll_time, itterations, stat;
  long double s2;
  float sum, rb, ra;
  float value[1000];
  unsigned int cycle1, it;
  unsigned int autoscale;

  /* init phase */  
  ////TODO: remove - the next 3 commented lines
  ///* just use the setup_oi from led implementation */
  //wiringPiSetup () ;
  //pinMode (tsl_pin, INPUT) ;
  setup_io();
  INP_GPIO(tsl_pin);

  ////TODO: remove - test
  printf(".........\n");
  printf("%s\n", GPIO_READ(tsl_pin));
  printf(".........\n");

  if (argc > 1)
  {
    poll_time = atoi(argv[1]);
    itterations = atoi(argv[2]);
  }
  else
  {
    poll_time = 150000;
    itterations = 5;
  }
   
  rb = 0;
  clock_gettime(CLOCK_REALTIME, &tp1);
  while (rb<1 && ra<100000)
  {
    if (GPIO_READ(tsl_pin) == 1) 
      rb = 1;
    clock_gettime(CLOCK_REALTIME, &tp2);
    ra = time_diff(tp1, tp2)/1000;
  }
  
  if ( rb == 0 ) 
  {
    printf("Error! No TSL235 found on wiringpi_pin(%d)\n", tsl_pin);
    exit(-1);
  }

  autoscale = 2;
  ra = 1;
  
  /* autoscale till time is more than 300 ms then perform real readout */
  while (ra < poll_time)
  {
    autoscale = autoscale*2;
    clock_gettime(CLOCK_REALTIME, &tp1);
    count_wait_tsl(autoscale);
    clock_gettime(CLOCK_REALTIME, &tp2);
    ra = time_diff(tp1, tp2)/1000;
  }

  /* end of autoscaling, now performing real readouts */
  cycle1 = autoscale;  
  sum = 0;
  for (it = 0; it < itterations; it++)
  {
    clock_gettime(CLOCK_REALTIME, &tp1);
    count_wait_tsl(cycle1);
    clock_gettime(CLOCK_REALTIME, &tp2);
    value[it] = (float)cycle1*(float)(1000000)/(float)time_diff(tp1, tp2);
    sum = sum + value[it];
  }

  printf("TSL235READ--poll_time--itterations--avg_value--autoscale %d %d %.3f %d\n", 
    poll_time, itterations, sum/(float)itterations, autoscale);
 
  return 0; 
}