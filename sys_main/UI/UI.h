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
 * global parameter, data exchange between
 * 1. UI.n and UI.n+1
 * 2. UI and SYS
 */
app_para_t appGP = {0};

#if defined(UI_EVEUI)
#include "UI_eveui.h"
#elif defined(UI_AUDIO)
#include "UI_audio.h"
#elif defined(UI_DRAGICON)
#include "UI_dragicon.h"
#elif defined(UI_AVI)
#include "UI_avi.h"
#elif defined(UI_LIERDA)
#include "UI_lierda.h"
#elif defined(UI_XIZI)
#include "UI_xizi.h"
#elif defined(UI_RIPPLE)
#include "UI_ripple.h"
#elif defined(UI_MIDEA_W)
#include "UI_midea_w.h"
#elif defined(UI_CJ)
#include "UI_cj.h"
#elif defined(UI_JPGDISP)
#include "UI_jpgdisp.h"
#elif defined(UI_BSH)
#include "UI_bsh.h"
#elif defined(UI_GRG)
#include "UI_grg.h"
#elif defined(UI_PUZZLE)
#include "UI_puzzle.h"
#elif defined(UI_MIDEA)
#include "UI_midea.h"
#elif defined(UI_LITTLESWAN)
#include "UI_littleswan.h"
#elif defined(UI_XIZI_BKGND)
#include "UI_xizi_bkgnd.h"
#elif defined(UI_AMICON)
#include "UI_amicon.h"
#elif defined(UI_DIAL)
#include "UI_dial.h"
#elif defined(UI_FV_INVENSYS)
#include "UI_fvideo_invensys.h"
#elif defined(UI_WELLING)
#include "UI_welling.h"
#elif defined(UI_COOBOT)
#include "UI_coobot.h"
#elif defined(UI_STELIGENT)
#include "UI_steligent.h"
#elif defined(UI_BITMAP)
#include "UI_bitmap.h"
#elif defined(UI_DISPRAW)
#include "UI_dispraw.h"
#elif defined(UI_FONT)
#include "UI_font.h"
#elif defined(UI_DXT1)
#include "UI_dxt1.h"
#elif defined(UI_BKGND)
#include "UI_bkgnd.h"
#elif defined(UI_FVIDEO)
#include "UI_fvideo.h"
#elif defined(UI_ALLYTECH)
#include "UI_allytech.h"
#elif defined(UI_ALLY_FONT)
#include "UI_allytech_font.h"
#elif defined(UI_ALLY_ROTATE)
#include "UI_allytech_rotate.h"
#elif defined(UI_NJTOYO)
#include "UI_njtoyo.h"
#elif defined(UI_ALLY_MODEB)
#include "UI_ally_modeb.h"
#elif defined(UI_ALLY_LCDTST)
#include "UI_ally_lcdtst.h"
#elif defined(UI_TOUCH)
#include "UI_touch.h"
#elif defined(UI_MEMOPT)
#include "UI_memopt.h"
#elif defined(UI_ALLY_SWAP)
#include "UI_ally_swap.h"
#elif defined(UI_BOMS_HID)
#include "UI_boms_hid.h"
#elif defined(UI_SLIP_M)
#include "UI_slip_menu.h"
#elif defined(UI_VJDZ)
#include "UI_vjdz.h"
#elif defined(UI_AUPU)
#include "UI_aupu.h"
#elif defined(UI_FLASH_ANIM)
#include "UI_flash_anim.h"
#elif defined(UI_FLASH_PROG)
#include "UI_flash_prog.h"
#elif defined(UI_WATER)
#include "UI_water.h"
#else
#error "no UI be selected"
#endif


