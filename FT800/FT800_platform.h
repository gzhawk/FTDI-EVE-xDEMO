/* 
    Platform related file all should be defined here
    Author: Hawk
    Email : hawk.gao@ftdichip.com	
    Date  : 2013/Oct
*/

#ifndef _FT800_PLATFORM_H_
#define _FT800_PLATFORM_H_

/*---------------------------------------------------------All platform define*/
/*
 * Version: A.B.C.D
 * A: Basic version number, increase it when modify happen on core function
      (FT800, APP, HAL, etc.), or add a new MCU platform, 
	  leave 'B' part unchange and clear the 'C' part when 'A' part change.
 * B: significant structure change version number, 
      increase it when significant changed
 * C: Demo version number, increase it when new demo be added in
 * D: Modify tracking number, each modify in code should increase the number, 
      it should be clear to 0 when 'B' change
 * 
 *    for example: 3.X.11.5, 
                   3  - support 3 MCU platform
				   X  - from A ~ Z, software structure change
				   11 - it has 11 demos
				   5  - 5 small changes/modify
 *
 */
#define FT800_VER "5.A.27.0"

#define BYTES4ALIGN(l) ((l)%4)?((l)+(4-(l)%4)):(l)

/*!!! define this length base on your own system mem !!!*/
#define USE_STATIC_MEM_LEN 512 

/* 
 * in order to enhance the effective
 * during command transfering
 * we may buffer the command
 * then send it when buffer full
 * or command list needs to be sent by user
#define USED_CMD_BUF //put it in each platform define, only Arduino not use it
 */

/* enable it when using FT81X */
#define DEF_81X

/* 
 * too limit resource on Arduino platform, 
 * may disable the print for resource saving
 */
#define FT800_PRINT

/* 
 * when not using external OSC
 * you may do the clock trimming if needed
 */
#define TRIM_NEEDED

/* 
 * you may bypass the calibration phase
 */
#define CAL_NEEDED

/* 
 * enable or disable this define for:
 * different LCD size: WQVGA, QVGA, WVGA
 * different EVE     : FT80X, FT81X
 * Cap or Res touch  : CAP_MULTI/CAP_NONMULTI/RES
#define DEF_CAP_MULTI
#define DEF_CAP_NONMULTI
*/

/* set the LCD screen based on your real connected LCD
LCD_WQVGA: 480x272
LCD_QVGA : 320x240
LCD_HVGA : 320x480
LCD_WVGA : 800x480
*/
//#define LCD_WQVGA 
//#define LCD_QVGA 
//#define LCD_HVGA //for FTDI ME8XXA_HV35R module
#define LCD_WVGA //only FT81X have WVGA

/* SPI, DSPI or QSPI supported
   only FT81X can set to 2 or 4*/
#define EVE_SPI_TYPE 4

/*-------------------------------------------------------------MSVC2010EXPRESS*/
#ifdef MSVC2010EXPRESS
#include "vc2010_xdemo.h"
/*-----------------------------------------------------------------MSVC2012EMU*/
#elif defined(MSVC2012EMU)
#include "vc2012emu_xdemo.h"
/*---------------------------------------------------------------------STM32F4*/
#elif defined(STM32F4)
#include "stm32_xdemo.h"
/*---------------------------------------------------------------------FT9XXEV*/
#elif defined(FT9XXEV)
#include "ft900_xdemo.h"
/*---------------------------------------------------------------------Arduino*/
#else
#include "arduino_xdemo.h"
#endif
/*-------------------------------------------------------------End of platform*/
#endif

