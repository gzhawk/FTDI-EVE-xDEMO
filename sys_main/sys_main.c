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

/* main() from here */
FTMAIN
{
    SYS_INIT;

    UI_INIT();

    FTPRINT("\nAPPS running");
    while (APPS_UI[appGP.appIndex]) {
        /* UI related application */
        APPS_UI[appGP.appIndex](appGP.appPara);

        /* system control related application */
        APPS_SYS(appGP.appPara);
    }

    UI_END();

    SYS_END;
}

/* platform related special handle */
FTDUMMY

