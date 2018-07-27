/* 
    Sample code to show how to program the Flash 
    from the file in SD Card
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Jul
*/

#define FLASH_INFO_LEN     36

#define FLASH_F            ROOT_PATH"flash\\bt81x.flash"

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

FTC8 prog_info[P_END][FLASH_INFO_LEN] = {
"Set Flash to full",
"Set Flash to full fail",
"Check user file",
"Check user file fail",
"Erase Flash",
"Write user file",
"Write user file fail",
"Verify user file",
"Verify user file fail",
"Success",
"Unknown error",
};

#define DISP_LINE 14
#define DISP_X    (EVE_LCD_WIDTH / 2)
#define DISP_FONT 23
#define DISP_OPT  OPT_CENTER

FTVOID flash_ui (FTU32 para)
{
    FTU32 i = EVE_LCD_HIGH / DISP_LINE;

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));

    if (appGP.appPara == P_STAT_F) {
        HAL_CmdBufIn(COLOR_RGB(255, 0, 0));
        CoCmd_TEXT(DISP_X, i, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_STAT_F][0]);
    } else if (appGP.appPara == P_STAT) {
        HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
        CoCmd_TEXT(DISP_X, i, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_STAT][0]);
    } else if (appGP.appPara > P_STAT) {
        HAL_CmdBufIn(COLOR_RGB(0, 255, 0));
        CoCmd_TEXT(DISP_X, i, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_STAT][0]);
    }
    
    if (appGP.appPara == P_CK_FILE_F) {
        HAL_CmdBufIn(COLOR_RGB(255, 0, 0));
        CoCmd_TEXT(DISP_X, i*2, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_CK_FILE_F][0]);
    } else if (appGP.appPara == P_CK_FILE) {
        HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
        CoCmd_TEXT(DISP_X, i*2, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_CK_FILE][0]);
    } else if (appGP.appPara > P_CK_FILE) {
        HAL_CmdBufIn(COLOR_RGB(0, 255, 0));
        CoCmd_TEXT(DISP_X, i*2, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_CK_FILE][0]);
    }
    
    if (appGP.appPara > P_ERASE) {
        HAL_CmdBufIn(COLOR_RGB(0, 255, 0));
        CoCmd_TEXT(DISP_X, i*3, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_ERASE][0]);
    } else if (appGP.appPara == P_ERASE) {
        HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
        CoCmd_TEXT(DISP_X, i*3, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_ERASE][0]);
    }

    if (appGP.appPara == P_PG_RES_F) {
        HAL_CmdBufIn(COLOR_RGB(255, 0, 0));
        CoCmd_TEXT(DISP_X, i*4, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_PG_RES_F][0]);
    } else if (appGP.appPara == P_PG_RES) {
        HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
        CoCmd_TEXT(DISP_X, i*4, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_PG_RES][0]);
    } else if (appGP.appPara > P_PG_RES) {
        HAL_CmdBufIn(COLOR_RGB(0, 255, 0));
        CoCmd_TEXT(DISP_X, i*4, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_PG_RES][0]);
    }

    if (appGP.appPara == P_VR_RES_F) {
        HAL_CmdBufIn(COLOR_RGB(255, 0, 0));
        CoCmd_TEXT(DISP_X, i*5, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_VR_RES_F][0]);
    } else if (appGP.appPara == P_VR_RES) {
        HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
        CoCmd_TEXT(DISP_X, i*5, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_VR_RES][0]);
    } else if (appGP.appPara > P_VR_RES) {
        HAL_CmdBufIn(COLOR_RGB(0, 255, 0));
        CoCmd_TEXT(DISP_X, i*5, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_VR_RES][0]);
    }

    if (appGP.appPara == P_ERROR) {
        HAL_CmdBufIn(COLOR_RGB(255, 0, 0));
        CoCmd_TEXT(DISP_X, i*6, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_ERROR][0]);
    } else if (appGP.appPara == P_SUCC) {
        HAL_CmdBufIn(COLOR_RGB(0, 255, 0));
        CoCmd_TEXT(DISP_X, i*6, DISP_FONT, DISP_OPT , (FTC8 *)&prog_info[P_SUCC][0]);
    }

    HAL_CmdBufIn(END());
    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

    HAL_BufToReg(RAM_CMD,0);

    if (appGP.appPara == P_STAT_F || 
		appGP.appPara == P_PG_RES_F ||
		appGP.appPara == P_VR_RES_F ||
		appGP.appPara == P_CK_FILE_F ||
		appGP.appPara == P_SUCC ||
        appGP.appPara == P_ERROR ) {
	    appGP.appIndex = 0;
    } else {
	    appGP.appIndex = 1;
    }
}
/* 
    except for FLASHSPITX, FLASHSPIRX, FLASHDESEL
    all other FLASH co-processor command such as FLASHREAD,
    FLASHWRITE, etc. need an binary flash driver
    (blob file) in the header of 4096 space of the flash.
    since the program of flash needs to be erase all,
    and eve do not have the command to only erase some particular
    page or sector the flash.
    so the first step alwasy program the blob file back into
    the first 4096 bytes of the flash, then do the rest.
*/
FTVOID flash_prog (FTU32 para)
{
    static FTU32 f_addr = 0;

    switch (appGP.appPara) {
        case P_STAT:
            if (!appFlashSetFull()) {
                appGP.appPara = P_CK_FILE;
            } else {
                appGP.appPara = P_STAT_F;
            }
            break;
        case P_CK_FILE:
            if(appFileExist((FTC8 *)FLASH_F)) {
                appGP.appPara = P_ERASE;
            } else {
                appGP.appPara = P_CK_FILE_F;
            }
            break;
        case P_ERASE:
            appFlashErase();
            appGP.appPara = P_PG_RES;
			f_addr = 0;
            break;
        case P_PG_RES:
            f_addr = appFlashProg((FTU8 *)FLASH_F, 0);
            if (f_addr) {
                appGP.appPara = P_VR_RES;
            } else {
				appGP.appPara = P_PG_RES_F;
            }
            break;
        case P_VR_RES:
            if (appFlashVerify((FTU8 *)FLASH_F, 0)) {
                appGP.appPara = P_SUCC;
            } else {
				appGP.appPara = P_VR_RES_F;
            }
            break;
        default:
			appGP.appPara = P_ERROR;
            break;
    }
    
    appGP.appIndex = 0;
    return;
}

AppFunc APPS_UI[] = {
	flash_ui,
	flash_prog
};

