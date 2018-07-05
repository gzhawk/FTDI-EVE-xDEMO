/* 
    Sample code to show how to program the Flash 
    from the file in SD Card
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Jul
*/

#define FLASH_INFO_LEN     36

#define FLASH_F_BLOB       ROOT_PATH"flash_prog\\unified.blob"
#define FLASH_F_RES        ROOT_PATH"flash_prog\\ASTC4x4.bin"

#define FLASH_ADDR_BLOB    0
#define FLASH_ADDR_RES     4096

typedef enum PROG_STEP_ {
    P_STAT = 0,
    P_STAT_F,
    P_CHECK_BLOB,
    P_CHECK_BLOB_F,
    P_CHECK_FILE,
    P_CHECK_FILE_F,
    P_ERASE,
    P_PG_BLOB,
    P_PG_BLOB_F,
    P_VR_BLOB,
    P_VR_BLOB_F,
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
"Fail to set Flash to full",
"Check blob file",
"Check blob file fail",
"Check file",
"Check file fail",
"Erase Flash",
"Write blob file to Flash",
"Fail to write blob file",
"Verify blob file",
"Fail to verify blob file",
"Write file to Flash",
"Fail to write file",
"Verify file",
"Fail to verify file",
"Success",
"Unknown error",
};

FTVOID flash_ui (FTU32 para)
{
	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));
	
	if (appGP.appPara == P_SUCC) {
	    HAL_CmdBufIn(COLOR_RGB(0, 255, 0));
	} else if (appGP.appPara == P_STAT_F || 
		appGP.appPara == P_PG_BLOB_F ||
		appGP.appPara == P_VR_BLOB_F ||
		appGP.appPara == P_PG_RES_F ||
		appGP.appPara == P_VR_RES_F ||
		appGP.appPara == P_CHECK_BLOB_F ||
		appGP.appPara == P_CHECK_FILE_F ||
        appGP.appPara == P_ERROR ) {
	    HAL_CmdBufIn(COLOR_RGB(255, 0, 0));
	} else {
	    HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
	}
	CoCmd_TEXT(EVE_LCD_WIDTH / 2, EVE_LCD_HIGH / 2, 24, OPT_CENTER , &prog_info[appGP.appPara][0]);
    
    HAL_CmdBufIn(END());
    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

    HAL_BufToReg(RAM_CMD,0);

    if (appGP.appPara == P_STAT_F || 
		appGP.appPara == P_PG_BLOB_F ||
		appGP.appPara == P_VR_BLOB_F ||
		appGP.appPara == P_PG_RES_F ||
		appGP.appPara == P_VR_RES_F ||
		appGP.appPara == P_CHECK_BLOB_F ||
		appGP.appPara == P_CHECK_FILE_F ||
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
    switch (appGP.appPara) {
        case P_STAT:
            if (!appFlashSetFull()) {
                appGP.appPara = P_CHECK_BLOB;
            } else {
                appGP.appPara = P_STAT_F;
            }
            break;
        case P_CHECK_BLOB:
            if(appFileExist((FTC8 *)FLASH_F_BLOB)) {
                appGP.appPara = P_CHECK_FILE;
            } else {
                appGP.appPara = P_CHECK_BLOB_F;
            }
            break;
        case P_CHECK_FILE:
            if(appFileExist((FTC8 *)FLASH_F_RES)) {
                appGP.appPara = P_ERASE;
            } else {
                appGP.appPara = P_CHECK_FILE_F;
            }
            break;
        case P_ERASE:
            appFlashErase();
            appGP.appPara = P_PG_BLOB;
            break;
        case P_PG_BLOB:
            if (appFlashProg((FTU8 *)FLASH_F_BLOB, FLASH_ADDR_BLOB)) {
                appGP.appPara = P_VR_BLOB;
            } else {
				appGP.appPara = P_PG_BLOB_F;
            }
            break;
        case P_VR_BLOB:
            if (appFlashVerify((FTU8 *)FLASH_F_BLOB, FLASH_ADDR_BLOB)) {
                appGP.appPara = P_PG_RES;
            } else {
				appGP.appPara = P_PG_BLOB_F;
            }
            break;
        case P_PG_RES:
            if (appFlashProg((FTU8 *)FLASH_F_RES, FLASH_ADDR_RES)) {
                appGP.appPara = P_VR_RES;
            } else {
				appGP.appPara = P_PG_RES_F;
            }
            break;
        case P_VR_RES:
            if (appFlashVerify((FTU8 *)FLASH_F_RES, FLASH_ADDR_RES)) {
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

