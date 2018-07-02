/* 
    Sample code to show how to display continous ASTC
    image from flash
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Jun
*/

#define FLASH_ANIM_ADDR    10785472
#define FLASH_ANIM_F_WIDTH 800
#define FLASH_ANIM_F_HIGH  480
#define FLASH_ANIM_F_COUNT 1636

FTVOID flash_anim (FTU32 para)
{
    static FTU8 flag = 1;
    static FTU32 i = 0;

    if (flag) {
        if (!appFlashSetFull()) {
            flag = 0;
        } else {
            FTPRINT("\nflash error occur");
            return;
        }
    }

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255,255,255));
	HAL_CmdBufIn(CLEAR(1,1,1));

    /* it alwasy show from the frame center
       so need to put the x,y to the center of the frame*/
	CoCmd_ANIMFRAME(FLASH_ANIM_F_WIDTH/2, FLASH_ANIM_F_HIGH/2, FLASH_ANIM_ADDR, i++);
    
    /* try not to make the frame change it's own color
       so use save and restore context*/
    HAL_CmdBufIn(SAVE_CONTEXT());
    HAL_CmdBufIn(COLOR_RGB(0,0,255));
    CoCmd_TEXT(0,0,24,OPT_FORMAT,"Frame %d",i);
    HAL_CmdBufIn(RESTORE_CONTEXT());
    

    HAL_CmdBufIn(END());
    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

    HAL_BufToReg(RAM_CMD,0);

	if (i >= FLASH_ANIM_F_COUNT) {
		i = 0;
	}

    while (TOUCHED);

	appGP.appIndex = 0;
}

AppFunc APPS_UI[] = {
	flash_anim
};

