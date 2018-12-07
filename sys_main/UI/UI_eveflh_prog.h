/* 
    Sample code to show how to program the EVEFLH 
    from the file in SD Card
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Jul
*/

/* 
   when set EVEFLH_ONESHORT to 1: 
   1. no flash progress be reported
 */
#define EVEFLH_ONESHORT     0

/* when set EVEFLH_SAFTY to 1: 
    0. no whole EVEFLH Erase action be taken
    1. it would compare the contain before program
       the action is 4KBytes a block
    1.1 if the content are the same
        no action in this block 
    1.2 if not, erase 4KBytes when necessary,
        program this 4KBytes
 */
#define EVEFLH_SAFTY        1

#define EVEFLH_INFO_LEN     36

#define EVEFLH_F            ROOT_PATH"eveflh\\eveflh.bin"

typedef enum PROG_STEP_ {
    P_STAT = 0,
    P_STAT_F,
    P_CK_FILE,
    P_CK_FILE_F,
    P_ERASE,
    P_PG_RES,
    P_PG_RES_F,
    P_VR_RES,
    P_VR_RES_F,
    P_SUCC,
    P_ERROR,
    P_END
} PROG_STEP_E;

FTC8 prog_info[P_END][EVEFLH_INFO_LEN] = {
"Set EVEFLH to full",
"Set EVEFLH to full fail",
"Check user file",
"Check user file fail",
"Erase EVEFLH",
#if EVEFLH_SAFTY
"Update user file [%d/100]",
"Update user file fail [%d/100]",
#else
"Write user file [%d/100]",
"Write user file fail [%d/100]",
#endif
"Verify user file [%d/100]",
"Verify user file fail [%d/100]",
"Success",
"Unknown error",
};

#define DISP_LINE 14
#define DISP_X    (EVE_LCD_WIDTH / 2)
#define DISP_FONT 23
#define DISP_OPT  (OPT_FORMAT | OPT_CENTER)

FTVOID flash_ui (FTU32 para)
{
    FTU32 i = EVE_LCD_HIGH / DISP_LINE;
    FTU8 state = appGP.appPara & 0xFF;
    FTU8 pro_w = (appGP.appPara >> 8) & 0xFF;
    FTU8 pro_v = (appGP.appPara >> 16) & 0x7F;

    switch (pro_v) {
        case 3:
            pro_v = 30;
            break;
        case 2:
            pro_v = 60;
            break;
        case 1:
            pro_v = 90;
            break;
        case 0:
            pro_v = 100;
            break;
        default:
            pro_v = 0xFF;
            break;
    }
	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));

    if (state == P_STAT_F) {
        HAL_CmdBufIn(COLOR_RGB(255, 0, 0));
        CoCmd_TEXT(DISP_X, i, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_STAT_F][0]);
    } else if (state == P_STAT) {
        HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
        CoCmd_TEXT(DISP_X, i, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_STAT][0]);
    } else if (state > P_STAT) {
        HAL_CmdBufIn(COLOR_RGB(0, 255, 0));
        CoCmd_TEXT(DISP_X, i, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_STAT][0]);
    }
    
    if (state == P_CK_FILE_F) {
        HAL_CmdBufIn(COLOR_RGB(255, 0, 0));
        CoCmd_TEXT(DISP_X, i*2, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_CK_FILE_F][0]);
    } else if (state == P_CK_FILE) {
        HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
        CoCmd_TEXT(DISP_X, i*2, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_CK_FILE][0]);
    } else if (state > P_CK_FILE) {
        HAL_CmdBufIn(COLOR_RGB(0, 255, 0));
        CoCmd_TEXT(DISP_X, i*2, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_CK_FILE][0]);
    }
    
    if (state > P_ERASE) {
        if (EVEFLH_SAFTY) {
            HAL_CmdBufIn(COLOR_RGB(128, 128, 128));
        } else {
            HAL_CmdBufIn(COLOR_RGB(0, 255, 0));
        }
        CoCmd_TEXT(DISP_X, i*3, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_ERASE][0]);
    } else if (state == P_ERASE) {
        HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
        CoCmd_TEXT(DISP_X, i*3, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_ERASE][0]);
    }

    if (state == P_PG_RES_F) {
        HAL_CmdBufIn(COLOR_RGB(255, 0, 0));
        CoCmd_TEXT(DISP_X, i*4, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_PG_RES_F][0], pro_w);
    } else if (state == P_PG_RES) {
        HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
        CoCmd_TEXT(DISP_X, i*4, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_PG_RES][0], pro_w);
    } else if (state > P_PG_RES) {
        HAL_CmdBufIn(COLOR_RGB(0, 255, 0));
        CoCmd_TEXT(DISP_X, i*4, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_PG_RES][0], pro_w);
    }

    if (state == P_VR_RES_F) {
        HAL_CmdBufIn(COLOR_RGB(255, 0, 0));
        CoCmd_TEXT(DISP_X, i*5, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_VR_RES_F][0], pro_v);
    } else if (state == P_VR_RES) {
        HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
        CoCmd_TEXT(DISP_X, i*5, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_VR_RES][0], pro_v);
    } else if (state > P_VR_RES) {
        HAL_CmdBufIn(COLOR_RGB(0, 255, 0));
        CoCmd_TEXT(DISP_X, i*5, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_VR_RES][0], pro_v);
    }

    if (state == P_ERROR) {
        HAL_CmdBufIn(COLOR_RGB(255, 0, 0));
        CoCmd_TEXT(DISP_X, i*6, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_ERROR][0]);
    } else if (state == P_SUCC) {
        HAL_CmdBufIn(COLOR_RGB(0, 255, 0));
        CoCmd_TEXT(DISP_X, i*6, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_SUCC][0]);
    }

    HAL_CmdBufIn(END());
    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

    HAL_BufToReg(RAM_CMD,0);

    if (state == P_STAT_F || 
		state == P_PG_RES_F ||
		state == P_VR_RES_F ||
		state == P_CK_FILE_F ||
		state == P_SUCC ||
        state == P_ERROR ) {
	    appGP.appIndex = 0;
    } else {
	    appGP.appIndex = 1;
    }
}
/* 
    except for EVEFLHSPITX, EVEFLHSPIRX, EVEFLHDESEL
    all other EVEFLH co-processor command such as EVEFLHREAD,
    EVEFLHWRITE, etc. need an binary flash driver
    (blob file) in the header of 4096 space of the flash.
    since the program of flash needs to be erase all,
    and eve do not have the command to only erase some particular
    page or sector the flash.
    so the first step alwasy program the blob file back into
    the first 4096 bytes of the flash, then do the rest.
*/
FTVOID flash_prog (FTU32 para)
{
    static FTU32 pro_w = 0, pro_v = 0;
	FTU8 state = appGP.appPara & 0xFF;

    switch (state) {
        case P_STAT:
            if (!appEVEFLHSetFull()) {
				state = P_CK_FILE;
            } else {
				state = P_STAT_F;
            }
            break;
        case P_CK_FILE:
            if(appFileExist((FTC8 *)EVEFLH_F)) {
				state = EVEFLH_SAFTY?P_PG_RES:P_ERASE;
            } else {
				state = P_CK_FILE_F;
            }
            break;
        case P_ERASE:
            appEVEFLHErase();
			state = P_PG_RES;
			pro_w = 0;
            break;
        case P_PG_RES:
            pro_w = appEVEFLHProgProgress((FTU8 *)EVEFLH_F, 0, 
                                           EVEFLH_ONESHORT?0:EVE_FLHBLOCK_LEN,
                                           EVEFLH_SAFTY);
            if (pro_w == 100) {
                state = P_VR_RES;
            } else if (pro_w == 0) {
                state = P_PG_RES_F;
            } else {
                state = P_PG_RES;
            }
            break;
        case P_VR_RES:
            pro_v = appEVEFLHVerify((FTU8 *)EVEFLH_F, 0);
            if (!pro_v) {
				state = P_SUCC;
            } else if (pro_v & 0x80){
				state = P_VR_RES_F;
            } else {
                state = P_VR_RES;
            }
            break;
        default:
			state = P_ERROR;
            break;
    }
	appGP.appPara = (pro_v << 16 | pro_w << 8 | state);
    appGP.appIndex = 0;
    return;
}

AppFunc APPS_UI[] = {
	flash_ui,
	flash_prog
};

