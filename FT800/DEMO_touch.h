/* 
    Sample code to show how to react in touch action
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Jun
*/
#ifdef DEMO_TOUCH

#define T_F_NUM 23
#define T_F_W   25
#define T_F_X   80
#define T_MASK  (0x7FFF)

typedef struct _t_xy {
    FTU16 tx;
    FTU16 ty;
    FTU32 hold; //hold flag
    FTU32 ht;   //hold time
} t_xy;

t_xy tGrp[5] = {0};

FTVOID touch_detect (FTU32 para)
{
    FTU32 tmp,i;

    for (i = 0; i < 5; i++) {
        switch (i) {
            case 0:
                tmp = HAL_Read32(REG_CTOUCH_TOUCH0_XY);
                tGrp[i].tx = T_MASK&(FTU16)(tmp>>16);
                tGrp[i].ty = T_MASK&(FTU16)(tmp);
                break;
            case 1:
                tmp = HAL_Read32(REG_CTOUCH_TOUCH1_XY);
                tGrp[i].tx = T_MASK&(FTU16)(tmp>>16);
                tGrp[i].ty = T_MASK&(FTU16)(tmp);
                break;
            case 2:
                tmp = HAL_Read32(REG_CTOUCH_TOUCH2_XY);
                tGrp[i].tx = T_MASK&(FTU16)(tmp>>16);
                tGrp[i].ty = T_MASK&(FTU16)(tmp);
                break;
            case 3:
                tmp = HAL_Read32(REG_CTOUCH_TOUCH3_XY);
                tGrp[i].tx = T_MASK&(FTU16)(tmp>>16);
                tGrp[i].ty = T_MASK&(FTU16)(tmp);
                break;
            case 4:
                tGrp[i].tx = T_MASK&HAL_Read16(REG_CTOUCH_TOUCH4_X);
                tGrp[i].ty = T_MASK&HAL_Read16(REG_CTOUCH_TOUCH4_Y);
                break;
            default:
	            appGP.appIndex = 2;
                return;
        }

        if (tGrp[i].tx || tGrp[i].ty) {
            if (0 == tGrp[i].hold) {
                tGrp[i].ht = 0;
                tGrp[i].hold = 1;
            }
            tGrp[i].ht++;
        } else {
            tGrp[i].hold = 0;
        }
    }
	appGP.appIndex = 1;
}

FTVOID touch_show (FTU32 para)
{
    FTU8 i;
    static FTU8 f = 0;
	/* never mind, it's for debug,
	 * this part just for this routine 
     * jump out the outside caller 
     * when error happen */
	appGP.appIndex = 2;
	appGP.appPara = 0;

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));

    if (f == 0) {
        CoCmd_ROMFONT(1,34);
        f = 1;
    }

    HAL_CmdBufIn(BEGIN(LINES));
    HAL_CmdBufIn(LINE_WIDTH(10));
    HAL_CmdBufIn(COLOR_RGB(255,255,255));
    HAL_CmdBufIn(VERTEX2F(FT800_LCD_WIDTH/4*FT800_PIXEL_UNIT,0));
    HAL_CmdBufIn(VERTEX2F(FT800_LCD_WIDTH/4*FT800_PIXEL_UNIT,FT800_LCD_HIGH/4*FT800_PIXEL_UNIT));
    HAL_CmdBufIn(VERTEX2F(0,FT800_LCD_HIGH/4*FT800_PIXEL_UNIT));
    HAL_CmdBufIn(VERTEX2F(FT800_LCD_WIDTH/4*FT800_PIXEL_UNIT,FT800_LCD_HIGH/4*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(END());

    CoCmd_TEXT(0,      0,T_F_NUM,0,"Touch 1: ");
    CoCmd_NUMBER(T_F_X,0,T_F_NUM,0,tGrp[0].ht);
    CoCmd_TEXT(0,  T_F_W,T_F_NUM,0,"Touch 2: ");
    CoCmd_NUMBER(T_F_X,T_F_W,T_F_NUM,0,tGrp[1].ht);
    CoCmd_TEXT(0,T_F_W*2,T_F_NUM,0,"Touch 3: ");
    CoCmd_NUMBER(T_F_X,T_F_W*2,T_F_NUM,0,tGrp[2].ht);
    CoCmd_TEXT(0,T_F_W*3,T_F_NUM,0,"Touch 4: ");
    CoCmd_NUMBER(T_F_X,T_F_W*3,T_F_NUM,0,tGrp[3].ht);
    CoCmd_TEXT(0,T_F_W*4,T_F_NUM,0,"Touch 5: ");
    CoCmd_NUMBER(T_F_X,T_F_W*4,T_F_NUM,0,tGrp[4].ht);

    for (i = 0; i < 5; i++) {
        if (tGrp[i].hold) {
            HAL_CmdBufIn(COLOR_RGB(255,255,255));
            CoCmd_NUMBER(tGrp[i].tx,tGrp[i].ty,1,OPT_CENTER,i+1);
        }
    }

    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

    HAL_BufToReg(RAM_CMD,0);
	appGP.appIndex = 0;
}

AppFunc Apps[] = {
    touch_detect,
	touch_show,
	/* Leave this NULL at the buttom of this array */
	NULL
};

#endif
