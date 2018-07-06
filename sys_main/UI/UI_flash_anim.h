/* 
    Sample code to show how to display continous ASTC
    image from flash
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Jun
*/

#define FLASH_ANIM_1_ADDR  1434752
#define FLASH_ANIM_2_ADDR  2215744

#define FLASH_ANIM_1_WIDTH 400
#define FLASH_ANIM_1_HIGH  300

#define FLASH_ANIM_2_WIDTH 512
#define FLASH_ANIM_2_HIGH  512

#define FLASH_ANIM_1_COUNT 188
#define FLASH_ANIM_2_COUNT 180

#define FLASH_ANIM_GAP     100

FTVOID flash_anim(FTU32 para)
{
	static FTU8 flag = 1;
	static FTU32 c1 = 0, c2 = 0, x = 0, y = 0;

	if (flag) {
		if (!appFlashSetFull()) {
			flag = 0;
		}
		else {
			FTPRINT("\nflash error occur");
			return;
		}
	}

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255, 255, 255));
	HAL_CmdBufIn(CLEAR(1, 1, 1));

	/* it alwasy show from the frame center */
	CoCmd_ANIMFRAME(FLASH_ANIM_1_WIDTH / 2 + x, EVE_LCD_HIGH / 2, FLASH_ANIM_1_ADDR, c1);
	CoCmd_ANIMFRAME(EVE_LCD_WIDTH - FLASH_ANIM_2_WIDTH / 4 - x, EVE_LCD_HIGH / 2, FLASH_ANIM_2_ADDR, c2);

	/* try not to make the frame change it's own color
	   so use save and restore context*/
	HAL_CmdBufIn(SAVE_CONTEXT());
	HAL_CmdBufIn(COLOR_RGB(0, 0, 255));
	CoCmd_TEXT(0, 0, 24, OPT_FORMAT, "Frame 1: %d", c1);
	CoCmd_TEXT(EVE_LCD_WIDTH - 150, 0, 24, OPT_FORMAT, "Frame 2: %d", c2);
	HAL_CmdBufIn(RESTORE_CONTEXT());


	HAL_CmdBufIn(END());
	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

	HAL_BufToReg(RAM_CMD, 0);

	if (c1++ >= (FLASH_ANIM_1_COUNT-1)) {
		c1 = 0;
	}

	if (c2++ >= (FLASH_ANIM_2_COUNT-1)) {
		c2 = 0;
	}
	
	if (y) {
		if (x-- == 0) {
			y = 0;
		}
	} else {
		if (x++ == FLASH_ANIM_GAP) {
			y = 1;
		}
    }

    while (TOUCHED);

	appGP.appIndex = 0;
}

AppFunc APPS_UI[] = {
	flash_anim
};

