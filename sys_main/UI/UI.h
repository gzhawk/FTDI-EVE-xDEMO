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
#elif defined(UI_PICTURE)
#include "UI_picture.h"
#elif defined(UI_METER)
#include "UI_meter.h"
#elif defined(UI_BITMAP)
#include "UI_bitmap.h"
#elif defined(UI_FONT)
#include "UI_font.h"
#elif defined(UI_DASHBOARD)
#include "UI_dashboard.h"
#elif defined(UI_HVGA)
#include "UI_hvga.h"
#elif defined(UI_ANIM)
#include "UI_anim.h"
#elif defined(UI_EVEFLH_PROG)
#include "UI_eveflh_prog.h"
#elif defined(UI_WATER)
#include "UI_water.h"
#elif defined(UI_BEAN)
#include "UI_bean.h"
#elif defined(UI_SCAN)
#include "UI_scan.h"
#elif defined(UI_WXGA)
#include "UI_wxga.h"
#elif defined(UI_SWITCH)
#include "UI_switch.h"
#elif defined(UI_LONGLCD)
#include "UI_longlcd.h"
#else
#error "no UI be selected"
#endif


