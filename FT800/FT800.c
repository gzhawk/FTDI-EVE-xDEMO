/* 
    Main logic control of this program 
    Author: Hawk
    Email : hawk.gao@ftdichip.com	
    Date  : 2013/Oct
 */

#include "FT800_platform.h"
#include "FT800_Gpu.h"
#include "FT800_HAL.h"
#include "FT800_APP.h"
#include "FT800_demo.h"

#define RETRY_COUNT  8
/* 
 * for FT9XX delay has to be larger than 10 
 * use a larger wait time (100) to expand the system's
 * adaptability
 */
#define READ_ID_WAIT 100

FTU8 verify (FTVOID)
{
    FTU8 count = RETRY_COUNT, id;
    do {
        id = HAL_Read8(REG_ID);
        count--;
        FTDELAY(READ_ID_WAIT);
    } while (count && (id != FT800_ID));

    if (count) {
        /* for FT81X, users are recommended to 
           read 4 bytes data from address 0xC0000 
           before application overwrites this address,
           since it is located in RAM_G. */
        HAL_Get_EVE_ID();
    }
    return count;
}

FTVOID ft800_init (FTVOID)
{
    FTPRINT("\nFT800 init ");
    do {
        HAL_Path_Config();

        /* in some very old BASIC board, 
         * the input should be 0,
         * but all the offical board outside should be 1
         * so leave it 1 */
        HAL_FT800_PowerCycle(1);

        HAL_FT800_Active();

        HAL_FT800_Clock();

        FTPRINT(".");
    } while(!verify());

    HAL_FT800_GPIOConfig();

    HAL_FT800_TouchConfig();

    HAL_FT800_LCDConfig();

    FTPRINT("\nDone!");
}

FTVOID run_apps (FTVOID)
{
    appWaitCal();

    FTPRINT("\nFT800 run app");
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

    ft800_init();

    run_apps();

    end_loop();

    FTEND;
}

/* only arduino platform would use it */
FTDUMMY

/* only MSVC2012 emulator platform would use it */
FTEMU

