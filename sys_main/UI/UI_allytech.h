/* 
    Sample code AllyTech
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Mar
*/

/* meter background */
FTU8 bg_inx[] = ROOT_PATH"allytech\\bg-inx.bin";
FTU8 bg_lut[] = ROOT_PATH"allytech\\bg-lut.bin";
/* full needle effect */
FTU8 fn1_inx[] = ROOT_PATH"allytech\\n64-inx.bin";
FTU8 fn1_lut[] = ROOT_PATH"allytech\\n64-lut.bin";
FTU8 fn2_inx[] = ROOT_PATH"allytech\\n80-inx.bin";
FTU8 fn2_lut[] = ROOT_PATH"allytech\\n80-lut.bin";
/* partical needle effect */
FTU8 pn_inx[] = ROOT_PATH"allytech\\n00-inx.bin";
FTU8 pn_lut[] = ROOT_PATH"allytech\\n00-lut.bin";

#define BITMAP_NUM   (5)
#define STOP_PEROID  (1200)
#define PATH_INDEX   (20)

bmpHDR_st bmp_header[BITMAP_NUM] = {
	{(FTC8 *)bg_inx,(FTC8 *)bg_lut,0,PALETTED8,0,0,800,84},
    {(FTC8 *)fn1_inx,(FTC8 *)fn1_lut,0,PALETTED8,0,0,598,97},
    {(FTC8 *)pn_inx,(FTC8 *)pn_lut,0,PALETTED8,0,0,48,97},
	{(FTC8 *)fn2_inx,(FTC8 *)fn2_lut,0,PALETTED8,0,0,598,97},
	{(FTC8 *)pn_inx,(FTC8 *)pn_lut,0,PALETTED8,0,0,48,97}//for frame swap
};

FTU32 g_speed = 0;
FTU32 needle_cut[81] = {
120,128,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,598,452,0,0,0,0,
0,0,0,0,0,0,0,0,0,534,598};

FTVOID dispPal8 (FTU32 X, FTU32 Y, FTU32 PalSrc, FTU32 hdl, FTU32 cell)
{
    /* every thing after this commands would not display
       if not use save/restore context */
    HAL_CmdBufIn(SAVE_CONTEXT());
    HAL_CmdBufIn(BLEND_FUNC(ONE, ZERO));
    HAL_CmdBufIn(COLOR_MASK(0,0,0,1));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 3));
    HAL_CmdBufIn(BITMAP_HANDLE(hdl));
    HAL_CmdBufIn(CELL(cell));
    HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

    HAL_CmdBufIn(BLEND_FUNC(DST_ALPHA, ONE_MINUS_DST_ALPHA));
    HAL_CmdBufIn(COLOR_MASK(1,0,0,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 2));
    HAL_CmdBufIn(BITMAP_HANDLE(hdl));
    HAL_CmdBufIn(CELL(cell));
    HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

    HAL_CmdBufIn(COLOR_MASK(0,1,0,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 1));
    HAL_CmdBufIn(BITMAP_HANDLE(hdl));
    HAL_CmdBufIn(CELL(cell));
    HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

    HAL_CmdBufIn(COLOR_MASK(0,0,1,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 0));
    HAL_CmdBufIn(BITMAP_HANDLE(hdl));
    HAL_CmdBufIn(CELL(cell));
    HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));
    HAL_CmdBufIn(RESTORE_CONTEXT());
}

FTVOID allytech_speed (FTU32 para)
{
#define SPEED_START 0
#define SPEED_END   80

    static FTU8 nFlag = 0;

	appGP.appPara = (FTU32)&g_speed;
	appGP.appIndex = 1;
	if (para) {
		g_speed = *(FTU32 *)para;
	} else {
		/* at the very beginning */
		g_speed = 0;
		return;
	}

    /* moving forward or backward control */
	if (g_speed == SPEED_END) {
        nFlag = 1;
	} else if (g_speed == SPEED_START) {
        nFlag = 0;
	}


    if (nFlag == 1) {
		g_speed--;
    } else {
        g_speed++;
    }

    switch (g_speed) {
        case 2:
            g_speed = 64;
            break;
        case 66:
            g_speed = 79;
            break;
        case 78:
            g_speed = 65;
            break;
        case 63:
            g_speed = 1;
            break;
        default:
            break;
    }
}

FTVOID allytech_bitmap (FTU32 para)
{
#define NUM_BLUE     64
#define NUM_RED      80
#define SHADOW_X     35//(800 - 598)/2
	static FTU8 flag = 0;
	static FTU32 needle = 0,i_shadow = 1,i_needle = 2;
    FTU32 speed = *(FTU32 *)para;

	/* never mind, it's for debug,
	 * this part just for this routine jump out 
     * the outside caller when error happen */
	appGP.appIndex = 2;

	if (flag == 0) {
		/* load bitmap resources data into FT800 */
		if(APP_OK != appBmpToRamG(0, RAM_G, bmp_header, BITMAP_NUM)){
			DBGPRINT;
			return;
		}
		flag = 1;
	}

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));

    HAL_CmdBufIn(BEGIN(BITMAPS));
	/* meter background */
	dispPal8(0,0,bmp_header[0].lut_src,0,0);

	/* shadow cutted area: load full shadow, blue or red */
    if (speed <= NUM_BLUE) {
		i_shadow = 1;
    } else {
		i_shadow = 3;
    }

	/* shadow cutted area: show the cutted part */
    HAL_CmdBufIn(SAVE_CONTEXT());
    HAL_CmdBufIn(SCISSOR_XY(SHADOW_X,0));
    HAL_CmdBufIn(SCISSOR_SIZE(needle_cut[speed],bmp_header[i_shadow].high));
    dispPal8(SHADOW_X,0,bmp_header[i_shadow].lut_src,i_shadow,0);
    HAL_CmdBufIn(RESTORE_CONTEXT());

	/* needle cutted area */
	if (i_needle == 2) {
		i_needle = 4;
	} else {
		i_needle = 2;
	}
	if (speed != NUM_BLUE && speed != NUM_RED) {
        pn_inx[PATH_INDEX] = '0'+speed/10;
        pn_inx[PATH_INDEX+1] = '0'+speed%10;
		pn_lut[PATH_INDEX] = '0'+speed/10;
        pn_lut[PATH_INDEX+1] = '0'+speed%10;
        if (APP_OK != appLoadBmp(bmp_header[i_needle].lut_src-bmp_header[i_needle].len,&bmp_header[i_needle],1) ) {
            return;
        }
		dispPal8(SHADOW_X+needle_cut[speed],0,bmp_header[i_needle].lut_src,i_needle,0);
	}

    HAL_CmdBufIn(END());
    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
    HAL_BufToReg(RAM_CMD,0);

    needle = speed;
	appGP.appIndex = 0;

	FTDELAY(STOP_PEROID);
}

AppFunc APPS_UI[] = {
    allytech_speed, 
	allytech_bitmap,
	/* Leave this NULL at the buttom of this array */
	NULL
};

