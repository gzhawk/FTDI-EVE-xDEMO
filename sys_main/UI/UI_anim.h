/* 
    Sample code to show how to display continous ASTC
    image from eve-connected-flash
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Jun
*/

#define ANIM_1_ADDR  1434752
#define ANIM_2_ADDR  2215744

#define ANIM_1_WIDTH 400
#define ANIM_1_HIGH  300

#define ANIM_2_WIDTH 512
#define ANIM_2_HIGH  512

#define ANIM_1_COUNT 188
#define ANIM_2_COUNT 180

#define ANIM_TAG_1   1
#define ANIM_TAG_2   2

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
	static FTU8 flag = 1;
	static FTU32 c1 = 0, c2 = 0, 
                 x1 = ANIM_1_WIDTH / 2, y1 = EVE_LCD_HIGH / 2, 
                 x2 = EVE_LCD_WIDTH - ANIM_2_WIDTH / 4, y2 = EVE_LCD_HIGH / 2;
    FTU32 xy;

	if (flag) {
		if (!appEVEFLHSetFull()) {
			flag = 0;
		} else {
			FTPRINT("\neveflh error occur");
			return;
		}
	}

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255, 255, 255));
	HAL_CmdBufIn(CLEAR(1, 1, 1));

    /* set the tag and display one frame */
    HAL_CmdBufIn(TAG_MASK(1));
    HAL_CmdBufIn(TAG(ANIM_TAG_1));
    CoCmd_ANIMFRAME(x1, y1, ANIM_1_ADDR, c1);
    HAL_CmdBufIn(TAG(ANIM_TAG_2));
    CoCmd_ANIMFRAME(x2, y2, ANIM_2_ADDR, c2);
    HAL_CmdBufIn(TAG_MASK(0));
    
    /* display the text */
    HAL_CmdBufIn(SAVE_CONTEXT());
    HAL_CmdBufIn(COLOR_RGB(0, 0, 255));
    CoCmd_TEXT(x1, y1, 24, OPT_CENTER | OPT_FORMAT, "Animation 1 fram-%d (%d,%d)", c1, x1, y1);
    CoCmd_TEXT(x2, y2, 24, OPT_CENTER | OPT_FORMAT, "Animation 2 frame-%d (%d,%d)", c2, x2, y2);
    HAL_CmdBufIn(RESTORE_CONTEXT()); 
   
    /* change the frame index to make it 'animation' */
    loop_frame(&c1,ANIM_1_COUNT);
    loop_frame(&c2,ANIM_2_COUNT);

	HAL_CmdBufIn(END());
	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

	HAL_BufToReg(RAM_CMD, 0);

    if (TOUCHED) {
        /* assige the right X, Y coordinate to related frame
           base on the current detected tag's X, Y*/
        if (HAL_Read8(REG_TOUCH_TAG) == ANIM_TAG_1) {
            xy = HAL_Read32(REG_TOUCH_TAG_XY);
            x1 = xy>>16;
            y1 = xy&0xFFFF;
        }
        if (HAL_Read8(REG_TOUCH_TAG) == ANIM_TAG_2) {
            xy = HAL_Read32(REG_TOUCH_TAG_XY);
            x2 = xy>>16;
            y2 = xy&0xFFFF;
        }
        if (HAL_Read8(REG_TOUCH_TAG1) == ANIM_TAG_1) {
            xy = HAL_Read32(REG_TOUCH_TAG1_XY);
            x1 = xy>>16;
            y1 = xy&0xFFFF;
        }
        if (HAL_Read8(REG_TOUCH_TAG1) == ANIM_TAG_2) {
            xy = HAL_Read32(REG_TOUCH_TAG1_XY);
            x2 = xy>>16;
            y2 = xy&0xFFFF;
        }
        
    }

	appGP.appIndex = 0;
}

AppFunc APPS_UI[] = {
	eveflh_anim
};

