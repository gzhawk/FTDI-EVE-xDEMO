/* 
    Sample to play a video in FT800 way
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2014/Jan
*/
#ifdef DEMO_FVIDEO

typedef struct app_para_st {
	FTU32 appIndex;
	FTU32 appPara;
}app_para_t;

app_para_t appGP = {0};

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
#define PATH_V1_C0 ROOT_PATH"fvideo\\h00_c0.raw"
#define PATH_V1_C1 ROOT_PATH"fvideo\\h00_c1.raw"
#define PATH_V1_B0 ROOT_PATH"fvideo\\h00_b0.raw"
#define PATH_V1_B1 ROOT_PATH"fvideo\\h00_b1.raw"
#define PATH_V2_C0 ROOT_PATH"fvideo\\b00_c0.raw"
#define PATH_V2_C1 ROOT_PATH"fvideo\\b00_c1.raw"
#define PATH_V2_B0 ROOT_PATH"fvideo\\b00_b0.raw"
#define PATH_V2_B1 ROOT_PATH"fvideo\\b00_b1.raw"
#define PATH_V3_C0 ROOT_PATH"fvideo\\m00_c0.raw"
#define PATH_V3_C1 ROOT_PATH"fvideo\\m00_c1.raw"
#define PATH_V3_B0 ROOT_PATH"fvideo\\m00_b0.raw"
#define PATH_V3_B1 ROOT_PATH"fvideo\\m00_b1.raw"
#else
#define PATH_V1_C0 ROOT_PATH"h00_c0.raw"
#define PATH_V1_C1 ROOT_PATH"h00_c1.raw"
#define PATH_V1_B0 ROOT_PATH"h00_b0.raw"
#define PATH_V1_B1 ROOT_PATH"h00_b1.raw"
#define PATH_V2_C0 ROOT_PATH"b00_c0.raw"
#define PATH_V2_C1 ROOT_PATH"b00_c1.raw"
#define PATH_V2_B0 ROOT_PATH"b00_b0.raw"
#define PATH_V2_B1 ROOT_PATH"b00_b1.raw"
#define PATH_V3_C0 ROOT_PATH"m00_c0.raw"
#define PATH_V3_C1 ROOT_PATH"m00_c1.raw"
#define PATH_V3_B0 ROOT_PATH"m00_b0.raw"
#define PATH_V3_B1 ROOT_PATH"m00_b1.raw"
#endif

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
#define DXT1_PATH_LEN   (30)
#define DXT1_PATH_INDEX (19)
#else
#define DXT1_PATH_LEN   (8+1+3+1)
#define DXT1_PATH_INDEX (2)
#endif

#define DXT1_BLOCK_NUMS (4)
#define DXT1_START_ADDR RAM_G
#define DXT1_START_HDL  (0)
#define DXT1_HDL_NUMS   (2)
#define DXT1_TLT_FONT   (24)

#define FRAME_LAST_CHAR '9'
#define FRAME_SECN_CHAR '2'

typedef struct dxt1_path_ {
	FTU8 path_c0[DXT1_PATH_LEN];
	FTU8 path_c1[DXT1_PATH_LEN];
	FTU8 path_b0[DXT1_PATH_LEN];
	FTU8 path_b1[DXT1_PATH_LEN];
} dxt1_path_st;

dxt1_path_st vPath = {
	PATH_V1_C0,
	PATH_V1_C1,
	PATH_V1_B0,
	PATH_V1_B1
};

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

FTINDEF FTVOID dxt1FormatInfo (FTU8 startHdl, FTU16 X, FTU16 Y)
{
	HAL_CmdBufIn(BLEND_FUNC(ONE,ZERO));
	HAL_CmdBufIn(COLOR_A(0x55));
	HAL_CmdBufIn(VERTEX2II(X,Y,startHdl,0));

	HAL_CmdBufIn(BLEND_FUNC(ONE,ONE));
	HAL_CmdBufIn(COLOR_A(0xAA));
	HAL_CmdBufIn(VERTEX2II(X,Y,startHdl,1));

	HAL_CmdBufIn(COLOR_MASK(1,1,1,0));

	CoCmd_LOADIDENTITY;
	CoCmd_SCALE(4*FT800_TRANSFORM_MAX, 4*FT800_TRANSFORM_MAX);
	CoCmd_SETMATRIX;

	HAL_CmdBufIn(BLEND_FUNC(DST_ALPHA,ZERO));
	HAL_CmdBufIn(VERTEX2II(X,Y,startHdl+1,1));

	HAL_CmdBufIn(BLEND_FUNC(ONE_MINUS_DST_ALPHA,ONE));
	HAL_CmdBufIn(VERTEX2II(X,Y,startHdl+1,0));

	HAL_CmdBufIn(END());
}

FTINDEF FTVOID valueChg (FTU32 *paddr, FTU8 *phdl, dxt1_path_st *pst, FTU32 len)
{
	if (pst->path_c0[DXT1_PATH_INDEX] == FRAME_LAST_CHAR) {
		if (pst->path_c0[DXT1_PATH_INDEX-1] == FRAME_SECN_CHAR) {
			switch (pst->path_c0[DXT1_PATH_INDEX-2]) {
				case 'h':
					pst->path_c0[DXT1_PATH_INDEX-2] = 'b';
					pst->path_c1[DXT1_PATH_INDEX-2] = 'b';
					pst->path_b0[DXT1_PATH_INDEX-2] = 'b';
					pst->path_b1[DXT1_PATH_INDEX-2] = 'b';
					break;
				case 'b':
					pst->path_c0[DXT1_PATH_INDEX-2] = 'm';
					pst->path_c1[DXT1_PATH_INDEX-2] = 'm';
					pst->path_b0[DXT1_PATH_INDEX-2] = 'm';
					pst->path_b1[DXT1_PATH_INDEX-2] = 'm';
					break;
				case 'm':
					pst->path_c0[DXT1_PATH_INDEX-2] = 'h';
					pst->path_c1[DXT1_PATH_INDEX-2] = 'h';
					pst->path_b0[DXT1_PATH_INDEX-2] = 'h';
					pst->path_b1[DXT1_PATH_INDEX-2] = 'h';
					break;
				default:
					DBGPRINT;
					return;
			}
			pst->path_c0[DXT1_PATH_INDEX-1] = '0';
			pst->path_c1[DXT1_PATH_INDEX-1] = '0';
			pst->path_b0[DXT1_PATH_INDEX-1] = '0';
			pst->path_b1[DXT1_PATH_INDEX-1] = '0';
		} else {
			pst->path_c0[DXT1_PATH_INDEX-1]++;
			pst->path_c1[DXT1_PATH_INDEX-1]++;
			pst->path_b0[DXT1_PATH_INDEX-1]++;
			pst->path_b1[DXT1_PATH_INDEX-1]++;
		}
		pst->path_c0[DXT1_PATH_INDEX] = '0';
		pst->path_c1[DXT1_PATH_INDEX] = '0';
		pst->path_b0[DXT1_PATH_INDEX] = '0';
		pst->path_b1[DXT1_PATH_INDEX] = '0';
	} else {
		pst->path_c0[DXT1_PATH_INDEX]++;
		pst->path_c1[DXT1_PATH_INDEX]++;
		pst->path_b0[DXT1_PATH_INDEX]++;
		pst->path_b1[DXT1_PATH_INDEX]++;
	}

	if (*paddr == DXT1_START_ADDR) {
		*paddr += len;
		*phdl += DXT1_HDL_NUMS;
	} else {
		*paddr = DXT1_START_ADDR;
		*phdl = DXT1_START_HDL;
	}
}

FTINDEF FTVOID dxt1Display (FTU32 hdl, FTU32 addr, FTU8 t)
{
	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
	HAL_CmdBufIn(CLEAR(1,1,1));

	HAL_CmdBufIn(SAVE_CONTEXT());
	dxt1BitmapInfo(hdl, addr, WQVGA_WIDTH, WQVGA_HIGH);

	HAL_CmdBufIn(BEGIN(BITMAPS));

	dxt1FormatInfo(hdl,0,0);
	HAL_CmdBufIn(RESTORE_CONTEXT());

	HAL_CmdBufIn(COLOR_RGB(0xFF,0,0));
	if (t) {
		CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/4,DXT1_TLT_FONT,OPT_CENTERX,"Due to limited memory in Arduino");
		CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,DXT1_TLT_FONT,OPT_CENTERX,"Frame exchange become really slow");
		CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/4*3,DXT1_TLT_FONT,OPT_CENTERX,"Better play me on MSVC");
	} else {
		CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/4,DXT1_TLT_FONT,OPT_CENTERX,"Due to my frame capture tools limitation");
		CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,DXT1_TLT_FONT,OPT_CENTERX,"Frame border looks not good");
		CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/4*3,DXT1_TLT_FONT,OPT_CENTERX,"Not FT800's fault! But Frame itself!");
	}

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
}

FTVOID playfv (FTU32 para)
{
	FTU32 len, addr = DXT1_START_ADDR;
	FTU8 hdl = DXT1_START_HDL;
	dxt1_path_st *path = &vPath;

	/* just for debug */
	appGP.appIndex = 1;
	appGP.appPara = 0;
#if defined(STM32F4)
	return;
#endif	
	do {
		len = dxt1File(path,addr);
		if (len == 0) {
			DBGPRINT;
			return;
		}

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
		dxt1Display(hdl, addr, 0);
#else
		dxt1Display(hdl, addr, 1);
#endif

		valueChg(&addr, &hdl, path, len);
	} while (1);
}

AppFunc Apps[] = {
	playfv,
	/* Leave this NULL at the buttom of this array */
	NULL
};

#endif

