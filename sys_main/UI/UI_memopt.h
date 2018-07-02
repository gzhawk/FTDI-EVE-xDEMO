/* 
    Demo for teaching how to use EVE memory related co-cmd
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Jun
*/

#define TST_LEN 1024
#define TST_BYT 0xAA
#define FNT_H   50

FTVOID demo_memopt (FTU32 para)
{
    FTU32 i = 0;
    FTU16 addr;
    FTU8 buf[TST_LEN] = {0};

    CoCmd_MEMSET(0, TST_BYT, TST_LEN);
    HAL_BufToReg(RAM_CMD,0);
    for (i = 0; i < TST_LEN; i++) {
        buf[i] = HAL_Read8(i);
    }
    for (i = 0; i < TST_LEN; i++) {
        if (buf[i] != TST_BYT) {
            i = 1;
            break;
        }
    }

    if (i == TST_LEN) {
        CoCmd_MEMZERO(0, TST_LEN/2);
        HAL_BufToReg(RAM_CMD,0);
        for (i = 0; i < TST_LEN/2; i++) {
            buf[i] = HAL_Read8(i);
        }
        for (i = 0; i < TST_LEN/2; i++) {
            if (buf[i]) {
                i = 2;
                break;
            }
        }
    }


    if (i == TST_LEN/2) {
        CoCmd_MEMCPY(0, TST_LEN/2, TST_LEN/2);
        HAL_BufToReg(RAM_CMD,0);
        for (i = 0; i < TST_LEN/2; i++) {
            buf[i] = HAL_Read8(i);
        }
        for (i = 0; i < TST_LEN/2; i++) {
            if (buf[i] != TST_BYT) {
                i = 3;
                break;
            }
        }
    }

    if (i == TST_LEN/2) {
        addr = HAL_Read16(REG_CMD_WRITE);
        CoCmd_MEMCRC(0, TST_LEN, 0);
        HAL_BufToReg(RAM_CMD,0);
        /* the reason for "+12" is 4 bytes a command
           result located after 3 command
           memcrc+addr+len */
        i = HAL_Read32(RAM_CMD+addr+12);
    }

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));
    
    switch (i) {
        case 1:
	        HAL_CmdBufIn(COLOR_RGB(0xFF,0,0));
            CoCmd_TEXT(0, EVE_LCD_HIGH/2, 25, OPT_CENTERY, "MEMSET ERR");
            break;
        case 2:
	        HAL_CmdBufIn(COLOR_RGB(0,0xFF,0));
            CoCmd_TEXT(0, EVE_LCD_HIGH/2-FNT_H, 25, OPT_CENTERY, "MEMSET OK");
	        HAL_CmdBufIn(COLOR_RGB(0xFF,0,0));
            CoCmd_TEXT(0, EVE_LCD_HIGH/2, 25, OPT_CENTERY, "MEMZERO ERR");
            break;
        case 3:
	        HAL_CmdBufIn(COLOR_RGB(0,0xFF,0));
            CoCmd_TEXT(0, EVE_LCD_HIGH/2-2*FNT_H, 25, OPT_CENTERY, "MEMSET OK");
            CoCmd_TEXT(0, EVE_LCD_HIGH/2-FNT_H, 25, OPT_CENTERY, "MEMZERO OK");
	        HAL_CmdBufIn(COLOR_RGB(0xFF,0,0));
            CoCmd_TEXT(0, EVE_LCD_HIGH/2, 25, OPT_CENTERY, "MEMCPY ERR");
            break;
        default:
	        HAL_CmdBufIn(COLOR_RGB(0,0xFF,0));
            CoCmd_TEXT(0, EVE_LCD_HIGH/2-2*FNT_H, 25, OPT_CENTERY, "MEMSET OK");
            CoCmd_TEXT(0, EVE_LCD_HIGH/2-FNT_H, 25, OPT_CENTERY, "MEMZERO OK");
            CoCmd_TEXT(0, EVE_LCD_HIGH/2, 25, OPT_CENTERY, "MEMCPY OK");
            CoCmd_TEXT(0, EVE_LCD_HIGH/2+FNT_H, 25, OPT_CENTERY, "CRC: ");
            CoCmd_SETBASE(16);
            CoCmd_NUMBER(250, EVE_LCD_HIGH/2+FNT_H, 25, OPT_CENTERY|OPT_RIGHTX|8, i);
            break;
    }

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);

    while(1);
	
	return;
}

AppFunc APPS_UI[] = {
	demo_memopt
};

