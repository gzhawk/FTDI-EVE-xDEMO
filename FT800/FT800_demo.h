/* 
    Demo select control of this program 
    Author: Hawk
    Email : hawk.gao@ftdichip.com	
    Date  : 2015/May
*/

/* ONLY enable ONE of them per times */
#define DEMO_EVEUI
//#define DEMO_AVI
//#define DEMO_JPGDISP
//#define DEMO_DISPRAW
//#define DEMO_FONT
//#define DEMO_BITMAP
//#define DEMO_BKGND
//#define DEMO_AUDIO
//#define DEMO_DRAGICON
//#define DEMO_DXT1
//#define DEMO_FVIDEO
//#define DEMO_RIPPLE
/* code for customer: useful */
//#define DEMO_CJ
//#define DEMO_STELIGENT
//#define DEMO_LIERDA
//#define DEMO_MIDEA_W
//#define DEMO_WELLING
//#define DEMO_XIZI
//#define DEMO_GRG
/* code for customer: useless */
//#define DEMO_LITTLESWAN
//#define DEMO_BSH
//#define DEMO_COOBOT
//#define DEMO_PUZZLE
//#define DEMO_MIDEA
//#define DEMO_XIZI_BKGND
/* coded by Born */
//#define DEMO_AMICON
//#define DEMO_DIAL
//#define DEMO_FVIDEO_INVENSYS

#if defined(DEMO_EVEUI)
#include "DEMO_eveui.h"
#elif defined(DEMO_AUDIO)
#include "DEMO_audio.h"
#elif defined(DEMO_DRAGICON)
#include "DEMO_dragicon.h"
#elif defined(DEMO_AVI)
#if defined(DEF_81X) && defined(LCD_WVGA) && (defined(FT9XXEV) || defined(MSVC2010EXPRESS) || defined(MSVC2012EMU))
#include "DEMO_avi.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_LIERDA)
#if defined(DEF_81X) && defined(LCD_WVGA) && (defined(FT9XXEV) || defined(MSVC2010EXPRESS) || defined(MSVC2012EMU))
#include "DEMO_lierda.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_XIZI)
#if !defined(DEF_81X) && defined(LCD_WQVGA) && !defined(STM32F4) && (defined(MSVC2010EXPRESS) || defined(MSVC2012EMU))
#include "DEMO_xizi.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_RIPPLE)
#if defined(DEF_81X) && defined(LCD_WVGA) && (defined(MSVC2010EXPRESS) || defined(MSVC2012EMU))
#include "DEMO_ripple.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_MIDEA_W)
#if defined(LCD_WQVGA) && (defined(FT9XXEV) || defined(MSVC2010EXPRESS) || defined(MSVC2012EMU))
#include "DEMO_midea_w.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_CJ)
#if defined(LCD_WVGA) && (defined(FT9XXEV) || defined(MSVC2010EXPRESS) || defined(MSVC2012EMU))
#include "DEMO_cj.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_JPGDISP)
#if defined(DEF_81X) && defined(LCD_WVGA)
#include "DEMO_jpgdisp.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_BSH)
#if defined(LCD_WQVGA) && !defined(STM32F4)
#include "DEMO_bsh.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_GRG)
#if defined(LCD_WQVGA) && !defined(STM32F4)
#include "DEMO_grg.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_PUZZLE)
#if defined(LCD_WQVGA) && !defined(STM32F4)
#include "DEMO_puzzle.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_MIDEA)
#if defined(LCD_WQVGA) && !defined(STM32F4)
#include "DEMO_midea.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_LITTLESWAN)
#if defined(DEF_81X) && defined(LCD_WVGA)
#include "DEMO_littleswan.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_XIZI_BKGND)
#if defined(LCD_WQVGA) && !defined(STM32F4)
#include "DEMO_xizi_bkgnd.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_AMICON)
#if defined(LCD_WQVGA) && !defined(STM32F4)
#include "DEMO_amicon.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_DIAL)
#if defined(LCD_WQVGA) && !defined(STM32F4)
#include "DEMO_dial.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_FVIDEO_INVENSYS)
#if defined(LCD_WQVGA) && !defined(STM32F4)
#include "DEMO_fvideo_invensys.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_WELLING)
#include "DEMO_welling.h"
#elif defined(DEMO_COOBOT)
#if defined(LCD_WQVGA) && !defined(STM32F4)
#include "DEMO_coobot.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_STELIGENT)
#if defined(LCD_WVGA)
#include "DEMO_steligent.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_BITMAP)
#if !defined(STM32F4)
#include "DEMO_bitmap.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_DISPRAW)
#if !defined(STM32F4)
#include "DEMO_dispraw.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_FONT)
#if !defined(STM32F4)
#include "DEMO_font.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_DXT1)
#if !defined(STM32F4)
#include "DEMO_dxt1.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_BKGND)
#if !defined(STM32F4)
#include "DEMO_bkgnd.h"
#else
#error "Necessary definition missing!"
#endif
#elif defined(DEMO_FVIDEO)
#if !defined(STM32F4)
#include "DEMO_fvideo.h"
#else
#error "Necessary definition missing!"
#endif
#else
/* make it goes to end when nothing be defined */
AppFunc Apps[] = {0};
typedef struct app_para_st {
	FTU32 appIndex;
	FTU32 appPara;
}app_para_t;
app_para_t appGP = {0};
#endif


