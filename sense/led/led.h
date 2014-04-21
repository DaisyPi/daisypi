/***************************** FILE HEADER ***********************************/
/*!
*  \file    led.h
*
*  \brief   <b>Header used by the demo application that uses the LED.</b>\n
*
*  \note    Some special notes
*
*  \author    Vaduva Jan Alexandru\n
*             Copyright 2013 Daisy Pi
*
*  \version 1.0 2013-08-11 Vaduva Jan Alexandru    created
*
*//**************************** FILE HEADER **********************************/
#ifndef __LedHeader
#define __LedHeader

/******************************************************************************
* Includes
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

/******************************************************************************
 * Defines
 *****************************************************************************/
#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000)

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

/******************************************************************************
 * Local variables
 *****************************************************************************/
 int  mem_fd;
 void *gpio_map;

/* I/O access */
 volatile unsigned *gpio;

/******************************************************************************
 * Forward declaration
 *****************************************************************************/
 void setup_io();


#endif /* __LedHeader */