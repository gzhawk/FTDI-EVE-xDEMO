/* 
    Platform related file all should be defined here
    Author: Hawk
    Email : hawk.gao@ftdichip.com	
    Date  : 2013/Oct
 */

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

/*TODO: define your selected UI_XXX here */
#error "#define UI_XXX"

/* 
    actually, nowhere using ARDUINO define
    only for demo platform compatable check
*/
#if (!defined(FT9XXEV) && \
     !defined(STM32F4) && \
     !defined(VC_MPSSE) && \
     !defined(VC_FT4222) && \
     !defined(VC_EMULATOR))
#define ARDUINO
#endif
/*-------------------------------------------------Demo Related hardware setting
 * DEF_80X          when using FT80X 
 * DEF_81X          when using FT81X
 * DEF_BT81X        when using BT81X
 * DEF_CAP_MULTI    Cap touch with multiple touch control
 * DEF_CAP_NONMULTI Cap touch with single touch control
 * no DEF_CAP_...   Res touch
 * LCD_QVGA         320x240
 * LCD_HVGA         320x480 for FTDI ME8XXA_HV35R module
 * LCD_WQVGA        480x272
 * LCD_WVGA         800x480 only FT81X have WVGA
 * LCD_WXGA         1280x800 only for test
 * LCD_1200X280     1200x280 only for test
 * EVE_SPI_TYPE     SPI, DSPI or QSPI supported
 */
/*------------------------------------------demo setting, platform limitation */
#if defined(UI_LONGLCD)
#define DEF_BT81X
#define LCD_1200X280
/* using RTP, no need to define DEF_CAP_... */

#if (defined(FT9XXEV) || \
     defined(VC_FT4222))
#define EVE_SPI_TYPE 4
#else
#define EVE_SPI_TYPE 1
#endif

#if (defined(ARDUINO) || \
     defined(STM32F4) || \
     defined(DEF_81X) || \
     defined(DEF_80X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/

#if defined(UI_SWITCH)
#define DEF_BT81X
#define LCD_WVGA
/* using RTP, no need to define DEF_CAP_... */

#if (defined(FT9XXEV) || \
     defined(VC_FT4222))
#define EVE_SPI_TYPE 4
#else
#define EVE_SPI_TYPE 1
#endif

#if (defined(ARDUINO) || \
     defined(STM32F4) || \
     defined(DEF_81X) || \
     defined(DEF_80X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/

#if defined(UI_WXGA)
#define DEF_BT81X
#define LCD_WXGA
#define DEF_CAP_NONMULTI
#define DEF_GOODIX

#if (defined(FT9XXEV) || \
     defined(VC_FT4222))
#define EVE_SPI_TYPE 4
#else
#define EVE_SPI_TYPE 1
#endif

#if (defined(ARDUINO) || \
     defined(STM32F4) || \
     defined(DEF_81X) || \
     defined(DEF_80X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/

#if defined(UI_SCAN)
#define DEF_BT81X
#define LCD_WQVGA
#define DEF_CAP_NONMULTI

#if (defined(FT9XXEV) || \
     defined(VC_FT4222))
#define EVE_SPI_TYPE 4
#else
#define EVE_SPI_TYPE 1
#endif

#if (defined(ARDUINO) || \
     defined(STM32F4) || \
     defined(DEF_80X) || \
     defined(DEF_81X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/

#if defined(UI_BEAN)
#define DEF_BT81X
#define LCD_WQVGA
#define DEF_CAP_NONMULTI

#if (defined(FT9XXEV) || \
     defined(VC_FT4222))
#define EVE_SPI_TYPE 4
#else
#define EVE_SPI_TYPE 1
#endif

#if (defined(ARDUINO) || \
     defined(STM32F4) || \
     defined(DEF_80X) || \
     defined(DEF_81X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/

#if defined(UI_WATER)
#define DEF_BT81X
#define LCD_WQVGA
#define DEF_CAP_NONMULTI

#if (defined(FT9XXEV) || \
     defined(VC_FT4222))
#define EVE_SPI_TYPE 4
#else
#define EVE_SPI_TYPE 1
#endif

#if (defined(ARDUINO) || \
     defined(STM32F4) || \
     defined(DEF_80X) || \
     defined(DEF_81X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/
#if defined(UI_ANIM)
#define DEF_BT81X
#define LCD_WVGA
#define DEF_CAP_MULTI

#if (defined(FT9XXEV) || \
     defined(VC_FT4222))
#define EVE_SPI_TYPE 4
#else
#define EVE_SPI_TYPE 1
#endif

#if (defined(ARDUINO) || \
     defined(STM32F4) || \
     defined(DEF_80X) || \
     defined(DEF_81X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/
#if defined(UI_FLASH_PROG)
#define DEF_BT81X
#define LCD_WVGA
#define DEF_CAP_NONMULTI

#if (defined(FT9XXEV) || \
     defined(VC_FT4222))
#define EVE_SPI_TYPE 4
#else
#define EVE_SPI_TYPE 1
#endif


#if (defined(ARDUINO) || \
     defined(STM32F4) || \
     defined(DEF_80X) || \
     defined(DEF_81X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/
#if (defined(UI_AVI) || \
    defined(UI_BITMAP) || \
    defined(UI_DASHBOARD) || \
    defined(UI_PICTURE) || \
    defined(UI_METER))
#define DEF_81X

#if (defined(FT9XXEV) || \
     defined(VC_FT4222))
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
#if defined(UI_FONT)
#define DEF_81X
#define LCD_WQVGA
#define DEF_CAP_NONMULTI

#if (defined(FT9XXEV) || \
     defined(VC_FT4222))
#define EVE_SPI_TYPE 4
#else
#define EVE_SPI_TYPE 1
#endif

#if defined(STM32F4)
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/
#if defined(UI_HVGA)
#define DEF_81X
#define LCD_HVGA
/* using RTP, no need to define DEF_CAP_... */

#if (defined(FT9XXEV) || \
     defined(VC_FT4222))
#define EVE_SPI_TYPE 4
#else
#define EVE_SPI_TYPE 1
#endif

#if (defined(ARDUINO) || \
     defined(STM32F4) || \
     defined(DEF_80X))
#error "not supported"
#endif
#endif

/*----------------------------------------------------------------------------*/
#if defined(UI_EVEUI) || \
    defined(UI_AUDIO) || \
    defined(UI_DRAGICON)
#define DEF_80X

#define LCD_WQVGA
#define DEF_CAP_NONMULTI
#define EVE_SPI_TYPE 1

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

/*----------------------------------------------prevent undefined LCD setting */
#if (!defined(LCD_QVGA) && \
    !defined(LCD_HVGA) && \
    !defined(LCD_WQVGA) && \
    !defined(LCD_WXGA) && \
    !defined(LCD_1200X280) && \
    !defined(LCD_WVGA))
#error "no display be selected"
#endif

#if defined(VC_MPSSE) || defined(VC_FT4222) || defined(VC_EMULATOR) || defined(FT9XXEV)
#define FILESYS_USED
#endif
/*---------------------------------------------------Universal platform define*/
/*
 * Version: A.B.C
 * A: Basic version number, increase it when modify happen on core function
 (EVE chip, APP, HAL, etc.), or add a new MCU platform, 
 leave 'B' part unchange and clear the 'C' part when 'A' part change.
 * B: significant structure change version number, 
 increase it when significant changed
 * C: Modify tracking number
 * 
 *    for example: 3.X.5, 
 3  - support 3 MCU platform
 X  - from A ~ Z, software structure change
 5  - 5 small changes/modify
 *
 */
#define APPS_VER "5.D.8"

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
 * default consider using 8x8x8 (24bits) LCD 
 * if your LCD is 6x6x6 (18bits) define it 
#define LCD_18BITS
 */

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
#if defined(FILESYS_USED)
#define MCU_BLOCK_SIZE      (10*1024)
#else
/* limited by sdcard sector size
 * should be the same as SDC_SECTOR_SIZE if sdcard.h be used */
#define MCU_BLOCK_SIZE      (512)
#endif

/* 
 * Check if EVE match
 */
#if !defined(VC_EMULATOR)
#define EVE_DEF_CHECK
#endif

/*----------------------------------------------VC_EMULATOR,VC_FT4222,VC_MPSSE*/
#if defined(VC_MPSSE) || defined(VC_FT4222) || defined(VC_EMULATOR)
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

