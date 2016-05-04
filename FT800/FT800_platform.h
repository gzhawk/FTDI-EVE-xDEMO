/* 
    Platform related file all should be defined here
    Author: Hawk
    Email : hawk.gao@ftdichip.com	
    Date  : 2013/Oct
 */

#ifndef _FT800_PLATFORM_H_
#define _FT800_PLATFORM_H_

/* --------------------------------------------ONLY enable ONE of them per times
#define DEMO_EVEUI
#define DEMO_AVI
#define DEMO_JPGDISP
#define DEMO_DISPRAW
#define DEMO_FONT
#define DEMO_BITMAP
#define DEMO_BKGND
#define DEMO_AUDIO
#define DEMO_DRAGICON
#define DEMO_DXT1
#define DEMO_FVIDEO
#define DEMO_RIPPLE
 * code for customer: useful
#define DEMO_CJ
#define DEMO_STELIGENT
#define DEMO_LIERDA
#define DEMO_MIDEA_W
#define DEMO_WELLING
#define DEMO_XIZI
#define DEMO_GRG
#define DEMO_NJTOYO
 * code for customer: useless
#define DEMO_LITTLESWAN
#define DEMO_ALLYTECH
#define DEMO_ALLY_FONT
#define DEMO_ALLY_ROTATE
#define DEMO_BSH
#define DEMO_COOBOT
#define DEMO_PUZZLE
#define DEMO_MIDEA
#define DEMO_XIZI_BKGND
 * coded by Born
#define DEMO_AMICON
#define DEMO_DIAL
#define DEMO_FV_INVENSYS
*/

#define DEMO_EVEUI

/*-------------------------------------------------Demo Related hardware setting
 * DEF_81X      //when using FT81X, or DEMO run as FT80X 
 * CAP_MULTI    //Cap touch with multiple touch control, or DEMO run as res TCP 
 * CAP_NONMULTI //Cap touch with single touch control, or DEMO run as res TCP 
 * LCD_QVGA     //320x240
 * LCD_HVGA     //320x480 for FTDI ME8XXA_HV35R module
 * LCD_WQVGA    //480x272
 * LCD_WVGA     //800x480 only FT81X have WVGA
 * EVE_SPI_TYPE //SPI, DSPI or QSPI supported, only FT81X can set to 2 or 4
 *              //also, need to know if your MCU use what kind of SPI



 */
/* not support STM32 */
#if !defined(STM32F4)
/*-----------------------------------Arduino, FT9XX, MSVC platform, None STM32*/
#if (defined(DEMO_DIAL) || \
    defined(DEMO_BSH) || \
    defined(DEMO_GRG) || \
    defined(DEMO_PUZZLE) || \
    defined(DEMO_MIDEA) || \
    defined(DEMO_FVIDEO_INVENSYS) || \
    defined(DEMO_AMICON) || \
    defined(DEMO_XIZI_BKGND) || \
    defined(DEMO_FVIDEO) || \
    defined(DEMO_BKGND) || \
    defined(DEMO_DXT1) || \
    defined(DEMO_DISPRAW) || \
    defined(DEMO_FONT) || \
    defined(DEMO_WELLING) || \
    defined(DEMO_COOBOT))
#define LCD_WQVGA
#define EVE_SPI_TYPE 1
#define HW_DEFINED
#endif

/*-------------------------------------------FT9XX, MSVC platform, None STM32 */
#if (defined(FT9XXEV) || defined(MSVC2010EXPRESS) || defined(MSVC2012EMU))

#if (defined(DEMO_AVI) || \
    defined(DEMO_BITMAP) || \
    defined(DEMO_LIERDA)) || \
    defined(DEMO_CJ) || \
    defined(DEMO_ALLYTECH) || \
    defined(DEMO_JPGDISP) || \
    defined(DEMO_LITTLESWAN) || \
    defined(DEMO_NJTOYO) || \
    defined(DEMO_ALLY_ROTATE) || \
    defined(DEMO_STELIGENT)
#define DEF_81X
#define DEF_CAP_NONMULTI
#define LCD_WVGA

#if (defined(DEMO_NJTOYO) && defined(FT9XXEV))
#define DEF_TIMER
#define EVE_SPI_TYPE 4
#else
#define EVE_SPI_TYPE 1
#endif

#define HW_DEFINED
#endif

#if (defined(DEMO_MIDEA_W))
#define DEF_81X
#define LCD_WQVGA
#define EVE_SPI_TYPE 1
#define HW_DEFINED
#endif

#endif

/*--------------------------------------------------MSVC platform, None STM32 */
#if (defined(MSVC2010EXPRESS) || defined(MSVC2012EMU))

#if defined(DEMO_XIZI)
#define LCD_WQVGA
#define EVE_SPI_TYPE 1
#define HW_DEFINED
#endif

#if defined(DEMO_RIPPLE) || \
    defined(DEMO_ALLY_FONT)
#define DEF_81X
#define DEF_CAP_NONMULTI
#define LCD_WVGA
#define EVE_SPI_TYPE 1
#define HW_DEFINED
#endif
#endif

#endif

/*------------------------------------------------Android, FT9XX, MSVC, STM32 */
#if defined(DEMO_EVEUI) || \
    defined(DEMO_AUDIO) || \
    defined(DEMO_DRAGICON)
#define DEF_81X
#define DEF_CAP_NONMULTI
#define LCD_WVGA
#define EVE_SPI_TYPE 1
#define HW_DEFINED
#endif

/*------------------------------------------prevent undefined hardware setting */
#if !defined(HW_DEFINED)
#error "define your hardware setting for your DEMO"
#endif

/*---------------------------------------------------Universal platform define*/
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
#define FT800_VER "5.A.33.2"

/*
 * In SampleApp, it use a better way, you may use it if you like:
 * x = (x+3)&0xFFC;
 * but I keep using below stupid way
 */
#define BYTES4ALIGN(l) ((l)%4)?((l)+(4-(l)%4)):(l)

/*
 * !!! define this length base on your own system mem !!!
 */
#define USE_STATIC_MEM_LEN 512 

/* 
 * too limit resource on Arduino platform, 
 * may disable the print for resource saving
#define FT800_PRINT
 */

/* 
 * when not using external OSC
 * you may do the clock trimming if needed
#define TRIM_NEEDED
 */

/* 
 * you may bypass the calibration phase
 */
#define CAL_NEEDED

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

