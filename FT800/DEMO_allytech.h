/* 
    Sample code AllyTech
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Mar
*/
#ifdef DEMO_ALLYTECH

#define BACK_INX ROOT_PATH"allytech\\back-inx.bin"
#define BACK_LUT ROOT_PATH"allytech\\back-lut.bin"
#define BG_INX ROOT_PATH"allytech\\bg-inx.bin"
#define BG_LUT ROOT_PATH"allytech\\bg-lut.bin"
#define N0_INX ROOT_PATH"allytech\\n0-inx.bin"
#define N0_LUT ROOT_PATH"allytech\\n0-lut.bin"
#define N40_INX ROOT_PATH"allytech\\n40-inx.bin"
#define N40_LUT ROOT_PATH"allytech\\n40-lut.bin"

#define BITMAP_NUM   (4)

bmpHDR_st bmp_header[BITMAP_NUM] = {
	{BACK_INX,BACK_LUT,0,PALETTED8,0,0,800,84},
	{BG_INX,BG_LUT,0,PALETTED8,0,0,796,147},
    {N0_INX,N0_LUT,0,PALETTED8,0,0,47,61},
    {N40_INX,N40_LUT,0,PALETTED8,0,0,8,58}
};

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

FTVOID allytech_bitmap (FTU32 para)
{
#define MOVE_END 400
#define MOVE_START 100
	static FTU8 flag = 0;
	static FTU32 w = MOVE_START;

	/* never mind, it's for debug,
	 * this part just for this routine jump out the outside caller when error happen */
	appGP.appIndex = 1;
	appGP.appPara = 0;

	/* only load the file once */
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

	/* shadow cutted area */
	HAL_CmdBufIn(SAVE_CONTEXT());
	HAL_CmdBufIn(SCISSOR_XY(0,0));
	HAL_CmdBufIn(SCISSOR_SIZE(w,bmp_header[1].high));
	dispPal8(0,0,bmp_header[1].lut_src,1,0);
	HAL_CmdBufIn(RESTORE_CONTEXT());

	/* needle cutted area */
	if (w < MOVE_END) {
		dispPal8(w,0,bmp_header[2].lut_src,2,0);
	} else {
		dispPal8(MOVE_END,0,bmp_header[3].lut_src,3,0);
	}

    HAL_CmdBufIn(END());

    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

    HAL_BufToReg(RAM_CMD,0);
	if (w >= MOVE_END) {
		w = MOVE_START;
		FTDELAY(1500);
	} else {
		w++;
	}

	appGP.appIndex = 0;
}

AppFunc Apps[] = {
	allytech_bitmap,
	/* Leave this NULL at the buttom of this array */
	NULL
};

#endif
