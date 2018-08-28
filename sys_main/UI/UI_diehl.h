/* 
    Sample code to show Diehl's bootup
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Aug
*/

#if defined(VC_EMULATOR)
#error "copy res/diehl/bt81x.flash to res/flash, then comment this line"
#elif defined(VC_MPSSE) || defined(VC_FT4222)
#error "program res/diehl/bt81x.flash to on-board flash, then comment this line"
#endif

#define ANIM_1_ADDR  14703936
#define ANIM_1_FRAME 478
#define ANIM_2_ADDR  15369984
#define ANIM_2_FRAME 34

FTVOID loop_frame(FTU32 *pframe, FTU32 max)
{
    if (*pframe >= max-1) {
        *pframe = 0;
    } else {
        *pframe = *pframe + 1;
    }
}

FTVOID flash_anim(FTU32 para)
{
	static FTU8 load = 1, set = 0;
	static FTU32 addr = ANIM_1_ADDR, index = 0, max = ANIM_1_FRAME;

	if (load) {
		if (!appFlashSetFull()) {
			load = 0;
		} else {
			FTPRINT("\nflash error occur");
			return;
		}
	}

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255, 255, 255));
	HAL_CmdBufIn(CLEAR(1, 1, 1));

    /* display one frame */
    CoCmd_ANIMFRAME(EVE_LCD_WIDTH/2+5, EVE_LCD_HIGH/2, addr, index);
    
    /* change the frame index to make it 'animation' */
    loop_frame(&index, max);

	HAL_CmdBufIn(END());
	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

	HAL_BufToReg(RAM_CMD, 0);

    while (TOUCHED) {
        set = 1;
    };
    
    if (set) {
        if (addr == ANIM_1_ADDR) {
            addr = ANIM_2_ADDR;
            max = ANIM_2_FRAME;
        } else {
            addr = ANIM_1_ADDR;
            max = ANIM_1_FRAME;
        }
        index = 0;
        set = 0;
    }

	appGP.appIndex = 0;
}

AppFunc APPS_UI[] = {
	flash_anim
};

