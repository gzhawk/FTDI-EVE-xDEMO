/* 
    Sample to play dxt1 function for XIZI
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2014/Jan
*/
#ifdef DEMO_XIZI_BKGND

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
#define PATH_SCN1_C0 ROOT_PATH"xizi\\bk0_c0.raw"
#define PATH_SCN1_C1 ROOT_PATH"xizi\\bk0_c1.raw"
#define PATH_SCN1_B0 ROOT_PATH"xizi\\bk0_b0.raw"
#define PATH_SCN1_B1 ROOT_PATH"xizi\\bk0_b1.raw"

#define PATH_SCN2_C0 ROOT_PATH"xizi\\bk1_c0.raw"
#define PATH_SCN2_C1 ROOT_PATH"xizi\\bk1_c1.raw"
#define PATH_SCN2_B0 ROOT_PATH"xizi\\bk1_b0.raw"
#define PATH_SCN2_B1 ROOT_PATH"xizi\\bk1_b1.raw"
#else
#define PATH_SCN1_C0 ROOT_PATH"bk0_c0.raw"
#define PATH_SCN1_C1 ROOT_PATH"bk0_c1.raw"
#define PATH_SCN1_B0 ROOT_PATH"bk0_b0.raw"
#define PATH_SCN1_B1 ROOT_PATH"bk0_b1.raw"

#define PATH_SCN2_C0 ROOT_PATH"bk1_c0.raw"
#define PATH_SCN2_C1 ROOT_PATH"bk1_c1.raw"
#define PATH_SCN2_B0 ROOT_PATH"bk1_b0.raw"
#define PATH_SCN2_B1 ROOT_PATH"bk1_b1.raw"
#endif

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
#define DXT1_PATH_LEN   (30)
#define DXT1_PATH_INDEX_RUN (17)
#else
#define DXT1_PATH_LEN   (8+1+3+1)
#define DXT1_PATH_INDEX_RUN (2)
#endif

#define DXT1_BLOCK_NUMS (4)
#define DXT1_START_ADDR RAM_G
#define DXT1_START_HDL  (0)
#define DXT1_HDL_NUMS   (2)
#define DXT1_TLT_FONT   (24)

#define MOVING_WINDOWS 272

typedef struct dxt1_path_ {
	FTU8 path_c0[DXT1_PATH_LEN];
	FTU8 path_c1[DXT1_PATH_LEN];
	FTU8 path_b0[DXT1_PATH_LEN];
	FTU8 path_b1[DXT1_PATH_LEN];
} dxt1_path_st;

dxt1_path_st ScnPath[2] = {
	{
	PATH_SCN1_C0,
	PATH_SCN1_C1,
	PATH_SCN1_B0,
	PATH_SCN1_B1
	},
	{
	PATH_SCN2_C0,
	PATH_SCN2_C1,
	PATH_SCN2_B0,
	PATH_SCN2_B1
	}
};

typedef struct moving_ {
	dxt1_path_st *path;
	FTU32 addr;
	FTU32 hdl;
	FT16 X;
	FT16 Y;
	FTU16 W;
	FTU16 H;
}moving_st;

FTINDEF FTU32 dxt1File (dxt1_path_st *path, FTU32 addrRAM)
{
	FTU32 len, addr = addrRAM;

	len = appFileToRamG((FTC8 *)path->path_c0,addr,0,0,0);
	if (len == 0) {
		DBGPRINT;
		return len;
	}
	addr += len;
	len = appFileToRamG((FTC8 *)path->path_c1,addr,0,0,0);
	if (len == 0) {
		DBGPRINT;
		return len;
	}
	addr += len;
	len = appFileToRamG((FTC8 *)path->path_b0,addr,0,0,0);
	if (len == 0) {
		DBGPRINT;
		return len;
	}
	addr += len;
	len = appFileToRamG((FTC8 *)path->path_b1,addr,0,0,0);
	if (len == 0) {
		DBGPRINT;
		return len;
	}
	addr += len;

	return (addr - addrRAM);
}

FTINDEF FTVOID dxt1BitmapInfo (FTU8 startHdl, FTU32 startAddr, FTU16 W, FTU16 H)
{
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl+1));
	HAL_CmdBufIn(BITMAP_SOURCE(startAddr));
	HAL_CmdBufIn(BITMAP_LAYOUT(RGB565, W/DXT1_BLOCK_NUMS*2, H/DXT1_BLOCK_NUMS));
	HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, W, H));

	HAL_CmdBufIn(BITMAP_HANDLE(startHdl));
	HAL_CmdBufIn(BITMAP_SOURCE(startAddr + 2*(W/DXT1_BLOCK_NUMS*2*H/DXT1_BLOCK_NUMS)));
	HAL_CmdBufIn(BITMAP_LAYOUT(L1, W/DXT1_BLOCK_NUMS/2, H));
	HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, W, H));
}

FTINDEF FTVOID dxt1FormatInfo (FTU8 startHdl, FT16 X, FT16 Y)
{
	HAL_CmdBufIn(BLEND_FUNC(ONE,ZERO));
	HAL_CmdBufIn(COLOR_A(0x55));
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl));
	HAL_CmdBufIn(CELL(0));
	HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(BLEND_FUNC(ONE,ONE));
	HAL_CmdBufIn(COLOR_A(0xAA));
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl));
	HAL_CmdBufIn(CELL(1));
	HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(COLOR_MASK(1,1,1,0));

	CoCmd_LOADIDENTITY;
	CoCmd_SCALE(4*FT800_TRANSFORM_MAX, 4*FT800_TRANSFORM_MAX);
	CoCmd_SETMATRIX;

	HAL_CmdBufIn(BLEND_FUNC(DST_ALPHA,ZERO));
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl+1));
	HAL_CmdBufIn(CELL(1));
	HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(BLEND_FUNC(ONE_MINUS_DST_ALPHA,ONE));
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl+1));
	HAL_CmdBufIn(CELL(0));
	HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(END());
}

FTINDEF FTVOID valueChg (moving_st *pm)
{
	FTU8 i = pm->path->path_c0[DXT1_PATH_INDEX_RUN] - '0';

	i += 2;
	i %= 3;
	i += '0';

	pm->Y = -272;

	pm->path->path_c0[DXT1_PATH_INDEX_RUN] = i;
	pm->path->path_c1[DXT1_PATH_INDEX_RUN] = i;
	pm->path->path_b0[DXT1_PATH_INDEX_RUN] = i;
	pm->path->path_b1[DXT1_PATH_INDEX_RUN] = i;
}

FTINDEF FTVOID Disp2Pic (moving_st *pm1, moving_st *pm2)
{
	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
	HAL_CmdBufIn(CLEAR(1,1,1));

	HAL_CmdBufIn(SAVE_CONTEXT());
	dxt1BitmapInfo(pm1->hdl, pm1->addr, pm1->W, pm1->H);

	HAL_CmdBufIn(BEGIN(BITMAPS));

	dxt1FormatInfo(pm1->hdl,pm1->X,pm1->Y);
	HAL_CmdBufIn(RESTORE_CONTEXT());

	HAL_CmdBufIn(END());
	
	HAL_CmdBufIn(SAVE_CONTEXT());
	dxt1BitmapInfo(pm2->hdl, pm2->addr, pm2->W, pm2->H);

	HAL_CmdBufIn(BEGIN(BITMAPS));

	dxt1FormatInfo(pm2->hdl,pm2->X,pm2->Y);
	HAL_CmdBufIn(RESTORE_CONTEXT());

	HAL_CmdBufIn(END());

	HAL_CmdBufIn(COLOR_RGB(0xFF,0,0));
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
	CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/4,DXT1_TLT_FONT,OPT_CENTERX,"You would see a line in background");
	CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,DXT1_TLT_FONT,OPT_CENTERX,"Because original background content");
	CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/4*3,DXT1_TLT_FONT,OPT_CENTERX,"Do not continues link from head to end");
#else
	CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/4,DXT1_TLT_FONT,OPT_CENTERX,"Due to limited memory");
	CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/4*3,DXT1_TLT_FONT,OPT_CENTERX,"Play me on MSVC for better effect");
#endif

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
}

FTVOID playdxt1xizi (FTU32 para)
{
	FTU32 len, windows = 0;
	moving_st m1 = {&ScnPath[0],DXT1_START_ADDR,DXT1_START_HDL,0,0,480,272},
			  m2 = {&ScnPath[1],DXT1_START_ADDR,DXT1_START_HDL+DXT1_HDL_NUMS,0,-272,480,272},
			  *pm;

	/* just for debug */
	appGP.appIndex = 1;
	appGP.appPara = 0;
#if defined(STM32F4)
	return;
#endif
	/* pre-stored the first two screen information */
	len = dxt1File(&ScnPath[0],m1.addr);
	if (len == 0) {
		DBGPRINT;
		return;
	}

	m2.addr += len;
	len = dxt1File(&ScnPath[1],m2.addr);
	if (len == 0) {
		DBGPRINT;
		return;
	}

	do {
		Disp2Pic((m1.Y>m2.Y)?(&m1):(&m2), (m1.Y>m2.Y)?(&m2):(&m1));

		m1.Y++;
		m2.Y++;

		if (++windows >= MOVING_WINDOWS) {
			pm = (m1.Y >= m2.Y)?(&m1):(&m2);

			valueChg(pm);

			/* change the undisplay screen */
			len = dxt1File(pm->path,pm->addr);
			if (len == 0) {
				DBGPRINT;
				return;
			}
			windows = 0;
		}
	} while (1);
}

AppFunc Apps[] = {
	playdxt1xizi,
	/* Leave this NULL at the buttom of this array */
	NULL
};

#endif

