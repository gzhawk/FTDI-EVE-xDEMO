/* 
    Demo select control of this program 
    Author: Hawk
    Email : hawk.gao@ftdichip.com	
    Date  : 2015/May
 */

/*
 * structure define for sub routine
 * appIndex: sub routine index
 * appPara: sub routine parameter (if has)
 */
typedef struct app_para_st {
	FTU32 appIndex;
	FTU32 appPara;
}app_para_t;

/*
 * global variable for sub routine
 */
app_para_t appGP = {0};

#if defined(DEMO_EVEUI)
#include "DEMO_eveui.h"
#elif defined(DEMO_AUDIO)
#include "DEMO_audio.h"
#elif defined(DEMO_DRAGICON)
#include "DEMO_dragicon.h"
#elif defined(DEMO_AVI)
#include "DEMO_avi.h"
#elif defined(DEMO_LIERDA)
#include "DEMO_lierda.h"
#elif defined(DEMO_XIZI)
#include "DEMO_xizi.h"
#elif defined(DEMO_RIPPLE)
#include "DEMO_ripple.h"
#elif defined(DEMO_MIDEA_W)
#include "DEMO_midea_w.h"
#elif defined(DEMO_CJ)
#include "DEMO_cj.h"
#elif defined(DEMO_JPGDISP)
#include "DEMO_jpgdisp.h"
#elif defined(DEMO_BSH)
#include "DEMO_bsh.h"
#elif defined(DEMO_GRG)
#include "DEMO_grg.h"
#elif defined(DEMO_PUZZLE)
#include "DEMO_puzzle.h"
#elif defined(DEMO_MIDEA)
#include "DEMO_midea.h"
#elif defined(DEMO_LITTLESWAN)
#include "DEMO_littleswan.h"
#elif defined(DEMO_XIZI_BKGND)
#include "DEMO_xizi_bkgnd.h"
#elif defined(DEMO_AMICON)
#include "DEMO_amicon.h"
#elif defined(DEMO_DIAL)
#include "DEMO_dial.h"
#elif defined(DEMO_FV_INVENSYS)
#include "DEMO_fvideo_invensys.h"
#elif defined(DEMO_WELLING)
#include "DEMO_welling.h"
#elif defined(DEMO_COOBOT)
#include "DEMO_coobot.h"
#elif defined(DEMO_STELIGENT)
#include "DEMO_steligent.h"
#elif defined(DEMO_BITMAP)
#include "DEMO_bitmap.h"
#elif defined(DEMO_DISPRAW)
#include "DEMO_dispraw.h"
#elif defined(DEMO_FONT)
#include "DEMO_font.h"
#elif defined(DEMO_DXT1)
#include "DEMO_dxt1.h"
#elif defined(DEMO_BKGND)
#include "DEMO_bkgnd.h"
#elif defined(DEMO_FVIDEO)
#include "DEMO_fvideo.h"
#elif defined(DEMO_ALLYTECH)
#include "DEMO_allytech.h"
#elif defined(DEMO_ALLY_FONT)
#include "DEMO_allytech_font.h"
#elif defined(DEMO_ALLY_ROTATE)
#include "DEMO_allytech_rotate.h"
#elif defined(DEMO_NJTOYO)
#include "DEMO_njtoyo.h"
#elif defined(DEMO_ALLY_MODEB)
#include "DEMO_ally_modeb.h"
#elif defined(DEMO_ALLY_LCDTST)
#include "DEMO_ally_lcdtst.h"
#elif defined(DEMO_TOUCH)
#include "DEMO_touch.h"
#elif defined(DEMO_MEMOPT)
#include "DEMO_memopt.h"
#elif defined(DEMO_ALLY_SWAP)
#include "DEMO_ally_swap.h"
#else
#error "define the DEMO in FT800_platform.h!"
#endif


