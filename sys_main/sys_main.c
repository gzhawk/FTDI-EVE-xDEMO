/* 
    Main logic control of this program 
    Author: Hawk
    Email : hawk.gao@ftdichip.com	
    Date  : 2013/Oct
 */

#include "platform.h"
#include "EVE_Gpu.h"
#include "EVE_HAL.h"
#include "EVE_APP.h"
#include "UI.h"

FTVOID run_apps (FTVOID)
{
    FTPRINT("\nApp running");
    while (Apps[appGP.appIndex]) {
        Apps[appGP.appIndex](appGP.appPara);
    }
}

/* 
 * 'keep looping' function should not come here, just for debugging
 * 'run once' function finished indication 
 */
FTVOID end_loop (FTVOID)
{
    FTPRINT("\nApp End");

    HAL_FT800_EndDisp();
}

/* main() from here */
FTMAIN
{
    FTPREINIT;

    ui_init();

    run_apps();

    end_loop();

    FTEND;
}

/* only arduino platform would use it */
FTDUMMY

/* only MSVC2012 emulator platform would use it */
FTEMU

