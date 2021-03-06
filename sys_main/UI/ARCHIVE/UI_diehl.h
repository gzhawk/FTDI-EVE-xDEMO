/* 
    Sample code to show Diehl's bootup
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Aug
*/

#if defined(VC_EMULATOR)
#error "copy res/diehl/eveflh.bin to res/eveflh, then comment this line"
#elif defined(VC_MPSSE) || defined(VC_FT4222)
#error "program res/diehl/eveflh.bin to eve-connected-flash, then comment this line"
#endif

#define ANIM_ADDR  1454400
#define ANIM_FRAME 480

FTVOID loop_frame(FTU32 *pframe, FTU32 max)
{
    if (*pframe >= max-1) {
        *pframe = 0;
    } else {
        *pframe = *pframe + 1;
    }
}

FTVOID eveflh_anim(FTU32 para)
{
	static FTU8 load = 1;
	static FTU32 addr = ANIM_ADDR, index = 0, max = ANIM_FRAME;

	if (load) {
		if (!appEVEFLHSetFull()) {
			load = 0;
		} else {
			FTPRINT("\neveflh error occur");
			return;
		}
	}

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255, 255, 255));
	HAL_CmdBufIn(CLEAR(1, 1, 1));

    CoCmd_ANIMFRAME(EVE_LCD_WIDTH/2, EVE_LCD_HIGH/2, addr, index);
    
    /* change the frame index to make it 'animation' */
    loop_frame(&index, max);

	HAL_CmdBufIn(END());
	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

	HAL_BufToReg(RAM_CMD, 0);

	appGP.appIndex = 0;
}

AppFunc APPS_UI[] = {
	eveflh_anim
};

