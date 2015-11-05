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

FTINDEF FTU8 verify (FTVOID)
{
	FTU8 count = 8, id;
	do {
		id = HAL_Read8(REG_ID);
		count--;
		/* for FT9XX delay has to be larger than 10 
		   use a larger wait time (100) to expand the system's
		   adaptability*/
		FTDELAY(100);
	} while (count && (id != FT800_ID));

	if (count) {
		HAL_Get_EVE_ID();
	}
	return count;
}

FTVOID ft800_init (FTVOID)
{
	FTPRINT("\r\n");
	FTPRINT("FT800 init ");
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
	
	FTPRINT("\r\n");
	FTPRINT("Done!");
}

FTVOID run_apps (FTVOID)
{
	appWaitCal();
	
	FTPRINT("\r\n");
	FTPRINT("FT800 run app");
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
	FTPRINT("\r\n");
	FTPRINT("App End");

	HAL_FT800_EndDisp();
}

FTMAIN
{
	FTPREINIT;

	ft800_init();

	run_apps();
	
	end_loop();

	FTEND;
}

FTDUMMY

FTEMU


