/* 
    Platform related file all should be defined here
    Author: Hawk
    Email : hawk.gao@ftdichip.com	
    Date  : 2013/Oct
 */

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

/*TODO: define your UI_XXX here */
#define UI_BITMAP
/* 
    actually, nowhere using ARDUINO define
    only for demo platform compatable check
*/
#if (!defined(FT9XXEV) && \
     !defined(STM32F4) && \
     !defined(VC_MPSSE) && \
     !defined(VC_EMULATOR))
#define ARDUINO
#endif
/*-------------------------------------------------Demo Related hardware setting
 * DEF_80X          when using FT80X 
 * DEF_81X          when using FT81X
 * DEF_BT81X        when using BT81X
 * DEF_CAP_MULTI    Cap touch with multiple touch control, or DEMO run as res TCP 
 * DEF_CAP_NONMULTI Cap touch with single touch control, or DEMO run as res TCP 
 * LCD_QVGA         320x240
 * LCD_HVGA         320x480 for FTDI ME8XXA_HV35R module
 * LCD_WQVGA        480x272
 * LCD_WVGA         800x480 only FT81X have WVGA
 * EVE_SPI_TYPE     SPI, DSPI or QSPI supported, only FT81X can set to 2 or 4
 *                  also, need to know if your MCU use what kind of SPI
 */

/*------------------------------------------demo setting, platform limitation */
#if defined(UI_FLASH_ANIM) || \
    defined(UI_FLASH_PROG)
#define DEF_BT81X
#define LCD_WVGA
#define DEF_CAP_NONMULTI
#define EVE_SPI_TYPE 4

#if (defined(ARDUINO) || \
     defined(STM32F4) || \
     defined(DEF_80X) || \
     defined(DEF_81X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/
#if (defined(UI_DIAL) || \
    defined(UI_BSH) || \
    defined(UI_GRG) || \
    defined(UI_PUZZLE) || \
    defined(UI_MIDEA) || \
    defined(UI_FVIDEO_INVENSYS) || \
    defined(UI_AMICON) || \
    defined(UI_XIZI_BKGND) || \
    defined(UI_FVIDEO) || \
    defined(UI_BKGND) || \
    defined(UI_DXT1) || \
    defined(UI_DISPRAW) || \
    defined(UI_FONT) || \
    defined(UI_WELLING) || \
    defined(UI_COOBOT))
#define DEF_81X
#define LCD_WQVGA
#define DEF_CAP_NONMULTI
#define EVE_SPI_TYPE 1

#if defined(STM32F4)
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/
#if defined(UI_LITTLESWAN)
#define DEF_81X
#define DEF_CAP_NONMULTI
#define LCD_WVGA
#define EVE_SPI_TYPE 1

#if (defined(ARDUINO) || \
     defined(STM32F4) || \
     defined(DEF_80X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/

#if (defined(UI_AVI) || \
    defined(UI_BITMAP) || \
    defined(UI_LIERDA)) || \
    defined(UI_CJ) || \
    defined(UI_ALLYTECH) || \
    defined(UI_ALLY_MODEB) || \
    defined(UI_ALLY_SWAP) || \
    defined(UI_JPGDISP) || \
    defined(UI_SLIP_M) || \
    defined(UI_NJTOYO) || \
    defined(UI_ALLY_ROTATE) || \
	defined(UI_VJDZ) || \
    defined(UI_STELIGENT)
#define DEF_81X

#if ((defined(UI_NJTOYO) || \
      defined(UI_AVI)) && \
      defined(FT9XXEV))
#define DEF_TIMER
#define EVE_SPI_TYPE 4
#else
#define EVE_SPI_TYPE 1
#endif

#define DEF_CAP_NONMULTI
#define LCD_WVGA

#if (defined(ARDUINO) || \
     defined(STM32F4) || \
     defined(DEF_80X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/
#if defined(UI_BOMS_HID)
#define DEF_81X
#define EVE_SPI_TYPE 1
#define DEF_CAP_NONMULTI
#define LCD_WVGA

#ifndef VC_EMULATOR
#error "not supported"
#endif
#endif
/*----------------------------------------------------------------------------*/
#if defined(UI_MIDEA_W)
#define DEF_81X
#define LCD_WQVGA
#define DEF_CAP_NONMULTI
#define EVE_SPI_TYPE 1

#if (defined(ARDUINO) || \
     defined(STM32F4) || \
     defined(DEF_80X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/
#if defined(UI_AUPU)
#define DEF_81X
#define LCD_HVGA
#define DEF_CAP_NONMULTI
#define EVE_SPI_TYPE 1

#if (defined(FT9XXEV) || \
     defined(ARDUINO) || \
     defined(STM32F4) || \
     defined(DEF_80X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/
#if defined(UI_RIPPLE) || \
    defined(UI_ALLY_FONT)
#define DEF_81X
#define DEF_CAP_NONMULTI
#define LCD_WVGA
#define EVE_SPI_TYPE 1

#if (defined(FT9XXEV) || \
     defined(ARDUINO) || \
     defined(STM32F4) || \
     defined(DEF_80X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/
#if defined(UI_XIZI)
#define DEF_80X 

#define LCD_WQVGA
#define EVE_SPI_TYPE 1
#define DEF_CAP_NONMULTI

#if (defined(FT9XXEV) || \
     defined(ARDUINO) || \
     defined(STM32F4))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/
#if defined(UI_EVEUI) || \
    defined(UI_AUDIO) || \
    defined(UI_MEMOPT) || \
    defined(UI_DRAGICON)
#define DEF_81X

#if defined(DEF_80X)
#define LCD_WQVGA
#else
#define LCD_WVGA
#endif

#define DEF_CAP_NONMULTI
#define EVE_SPI_TYPE 1

#if (defined(DEF_80X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/
#if defined(UI_ALLY_LCDTST)
#define DEF_81X
#define DEF_CAP_NONMULTI
#define LCD_WVGA
#define EVE_SPI_TYPE 1

#if (defined(DEF_80X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/
#if defined(UI_TOUCH)
#define DEF_81X
#define DEF_CAP_MULTI
#define LCD_WVGA
#define EVE_SPI_TYPE 1

#if (defined(DEF_80X))
#error "not supported"
#endif
#endif

/*----------------------------------------------prevent undefined EVE setting */
#if (!defined(DEF_80X) && \
     !defined(DEF_81X) && \
     !defined(DEF_BT81X))
#error "no EVE be selected"
#endif

/*----------------------------------------------prevent undefined SPI setting */
#ifndef EVE_SPI_TYPE
#error "no SPI be selected"
#endif

/*--------------------------------------------prevent undefined TOUCH setting */
#if (!defined(DEF_CAP_NONMULTI) && \
     !defined(DEF_CAP_MULTI))
#error "no touch be selected"
#endif

/*----------------------------------------------prevent undefined LCD setting */
#if (!defined(LCD_QVGA) && \
    !defined(LCD_HVGA) && \
    !defined(LCD_WQVGA) && \
    !defined(LCD_WVGA))
#error "no display be selected"
#endif

/*---------------------------------------------------Universal platform define*/
/*
 * Version: A.B.C.D
 * A: Basic version number, increase it when modify happen on core function
 (EVE chip, APP, HAL, etc.), or add a new MCU platform, 
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
#define APPS_VER "5.B.43.0"

/*
 * In SampleApp, it use a better way, you may use it if you like:
 * x = (x+3)&0xFFC;
 * but I keep using below stupid way
 */
#define BYTES4ALIGN(l) ((l)%4)?((l)+(4-(l)%4)):(l)

/*
 * define this length base on your own system (MCU) memory
 */
#define USE_STATIC_MEM_LEN 512 

/* 
 * may enable the print for debug purpose
 */
#define DBG_PRINT

/* 
 * when not using external OSC, recommend to do the clock trimming
 * for H35R, it only use internal OSC, so trimming is needed
 */
#if defined(LCD_HVGA)
#define TRIM_NEEDED
#endif

/* 
 * you may bypass the calibration phase, when no touch in your UI
 */
#define CAL_NEEDED
/* you need to carfully consider the size of the block
 * base on your system available memory and data transfer buffering space 
 * WINDOWS: consider it as unlimited memory
 * FT9xx  : totally got 64K memory
 */
#if defined(VC_MPSSE) || defined(VC_EMULATOR) || defined(FT9XXEV)
#define MCU_BLOCK_SIZE      (10*1024)
#else
/* limited by sdcard sector size
 * should be the same as SDC_SECTOR_SIZE if sdcard.h be used */
#define MCU_BLOCK_SIZE      (512)
#endif
/*--------------------------------------------------------VC_EMULATOR,VC_MPSSE*/
#if defined(VC_MPSSE) || defined(VC_EMULATOR)
#include "vc_xdemo.h"
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

