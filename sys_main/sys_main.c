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

FTVOID APP_RUN (FTVOID)
{
    FTU8 exit_index, index;

    for (index = 0, appGP.appPara = 0, 
         exit_index = sizeof(APPS_UI)/sizeof(AppFunc);
         index < exit_index;
         index = appGP.appIndex, appGP.appIndex = exit_index) {
        /* 
         run the indexed UI application
         it should set the appGP.appIndex within limit
         or, the main routine would exit to exception handle
         UI_END, SYS_END
         */
        APPS_UI[index](appGP.appPara);

        /* system/MCU control related application */
        APPS_SYS(appGP.appPara);
    }
}

/* main() from here */
FTMAIN
{
    SYS_INIT;

    UI_INIT();

    APP_RUN();
    
    UI_END();

    SYS_END;
}

/* platform related special handle */
FTDUMMY

