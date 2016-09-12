/* 
    Platform related file all should be defined here
    Author: Hawk
    Email : hawk.gao@ftdichip.com	
    Date  : 2013/Oct
 */

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#define UI_EVEUI
#if 0
/* UI for customer: technical support */
#define UI_EVEUI
#define UI_MEMOPT
#define UI_AVI
#define UI_JPGDISP
#define UI_DISPRAW
#define UI_FONT
#define UI_BITMAP
#define UI_BKGND
#define UI_AUDIO
#define UI_DRAGICON
#define UI_DXT1
#define UI_FVIDEO
#define UI_RIPPLE
#define UI_TOUCH
#define UI_LITTLESWAN
#define UI_ALLYTECH
#define UI_ALLY_FONT
#define UI_ALLY_ROTATE
#define UI_ALLY_MODEB
#define UI_ALLY_LCDTST
#define UI_ALLY_SWAP
#define UI_BSH
#define UI_COOBOT
#define UI_PUZZLE
#define UI_MIDEA
#define UI_XIZI_BKGND
#define UI_BOMS_HID
#define UI_AMICON
#define UI_DIAL
#define UI_FV_INVENSYS
#define UI_SLIP_M
/* UI for customer: demonstration */
#define UI_CJ
#define UI_STELIGENT
#define UI_LIERDA
#define UI_MIDEA_W
#define UI_WELLING
#define UI_XIZI
#define UI_GRG
#define UI_NJTOYO
#endif

/*-------------------------------------------------Demo Related hardware setting
 * DEF_81X          when using FT81X, or DEMO run as FT80X 
 * DEF_CAP_MULTI    Cap touch with multiple touch control, or DEMO run as res TCP 
 * DEF_CAP_NONMULTI Cap touch with single touch control, or DEMO run as res TCP 
 * LCD_QVGA         320x240
 * LCD_HVGA         320x480 for FTDI ME8XXA_HV35R module
 * LCD_WQVGA        480x272
 * LCD_WVGA         800x480 only FT81X have WVGA
 * EVE_SPI_TYPE     SPI, DSPI or QSPI supported, only FT81X can set to 2 or 4
 *                  also, need to know if your MCU use what kind of SPI
 */
/* not support STM32 */
#if !defined(STM32F4)
/*-----------------------------------Arduino, FT9XX, MSVC platform, None STM32*/
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
#define LCD_WQVGA
#define EVE_SPI_TYPE 1
#define DEF_DISPLAY
#endif

#if defined(UI_LITTLESWAN)
#define DEF_81X
#define DEF_CAP_NONMULTI
#define LCD_WVGA
#define EVE_SPI_TYPE 1
#define DEF_DISPLAY
#endif

/*-------------------------------------------FT9XX, MSVC platform, None STM32 */
#if (defined(FT9XXEV) || defined(MSVC2010EXPRESS) || defined(MSVC2012EMU))

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
    defined(UI_BOMS_HID) || \
    defined(UI_ALLY_ROTATE) || \
    defined(UI_STELIGENT)
#define DEF_81X
#define DEF_CAP_NONMULTI
#define LCD_WVGA

#if ((defined(UI_NJTOYO) || \
      defined(UI_AVI)) && \
      defined(FT9XXEV))
#define DEF_TIMER
#define EVE_SPI_TYPE 4
#else
#define EVE_SPI_TYPE 1
#endif

#define DEF_DISPLAY
#endif

#if (defined(UI_MIDEA_W))
#define DEF_81X
#define LCD_WQVGA
#define EVE_SPI_TYPE 1
#define DEF_DISPLAY
#endif

#endif

/*--------------------------------------------------MSVC platform, None STM32 */
#if (defined(MSVC2010EXPRESS) || defined(MSVC2012EMU))

#if defined(UI_XIZI)
#define LCD_WQVGA
#define EVE_SPI_TYPE 1
#define DEF_DISPLAY
#endif

#if defined(UI_RIPPLE) || \
    defined(UI_ALLY_FONT)
#define DEF_81X
#define DEF_CAP_NONMULTI
#define LCD_WVGA
#define EVE_SPI_TYPE 1
#define DEF_DISPLAY
#endif
#endif

#endif

/*------------------------------------------------Android, FT9XX, MSVC, STM32 */
#if defined(UI_EVEUI) || \
    defined(UI_AUDIO) || \
    defined(UI_MEMOPT) || \
    defined(UI_ALLY_LCDTST) || \
    defined(UI_DRAGICON) || \
    defined(UI_TOUCH)
#define DEF_81X
#define DEF_CAP_NONMULTI
#define LCD_WVGA
#define EVE_SPI_TYPE 1

#if defined(UI_TOUCH)
#undef DEF_CAP_NONMULTI
#define DEF_CAP_MULTI
#endif

#define DEF_DISPLAY
#endif

/*------------------------------------------prevent undefined hardware setting */
#if !defined(DEF_DISPLAY)
#error "no display be selected"
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
#define APPS_VER "5.B.40.1"

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
 */
#define FT800_PRINT

/* 
 * when not using external OSC
 * you may do the clock trimming if needed
 * for H35R, it has to be internal OSC
 */
#if defined(LCD_HVGA)
#define TRIM_NEEDED
#endif

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

