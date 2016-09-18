/* 
    Sample to play a video in FT800 way for Invensys setup logo
    Author: Born
	Email : born.jiang@ftdichip.com
	Date  : 2014/Mar.
*/

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
#define PATH_V1_C0 ROOT_PATH"Invensys\\h000_c0.raw" //fvideo
#define PATH_V1_C1 ROOT_PATH"Invensys\\h000_c1.raw"
#define PATH_V1_B0 ROOT_PATH"Invensys\\h000_b0.raw"
#define PATH_V1_B1 ROOT_PATH"Invensys\\h000_b1.raw"
#else
#define PATH_V1_C0 ROOT_PATH"h000_c0.raw"
#define PATH_V1_C1 ROOT_PATH"h000_c1.raw"
#define PATH_V1_B0 ROOT_PATH"h000_b0.raw"
#define PATH_V1_B1 ROOT_PATH"h000_b1.raw"
#endif

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
#define DXT1_PATH_LEN   (31)
#define DXT1_PATH_INDEX (22)
#else
#define DXT1_PATH_LEN   (8+1+3+1)
#define DXT1_PATH_INDEX (3)
#endif

#define DXT1_BLOCK_NUMS (4)
#define DXT1_START_ADDR RAM_G
#define DXT1_START_HDL  (0)
#define DXT1_HDL_NUMS   (2)
#define DXT1_TLT_FONT   (24)

#define FRAME_LAST_CHAR '9'
#define FRAME_SECN_CHAR '9'
#define FRAME_THRD_CHAR '1'

#define MAX_FRAME_LAST_CHAR '2'
#define MAX_FRAME_SECN_CHAR '2'
#define MAX_FRAME_THRD_CHAR '1'

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

STATIC FTU32 dxt1File (dxt1_path_st *path, FTU32 addrRAM)
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

STATIC FTVOID dxt1BitmapInfo (FTU8 startHdl, FTU32 startAddr, FTU16 W, FTU16 H)
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

STATIC FTVOID dxt1FormatInfo (FTU8 startHdl, FTU16 X, FTU16 Y)
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

STATIC FTVOID valueChg (FTU32 *paddr, FTU8 *phdl, dxt1_path_st *pst, FTU32 len)
{
	if (pst->path_c0[DXT1_PATH_INDEX] == FRAME_LAST_CHAR) {
		if (pst->path_c0[DXT1_PATH_INDEX-1] == FRAME_SECN_CHAR) {
			pst->path_c0[DXT1_PATH_INDEX-1] = '0';
			pst->path_c1[DXT1_PATH_INDEX-1] = '0';
			pst->path_b0[DXT1_PATH_INDEX-1] = '0';
			pst->path_b1[DXT1_PATH_INDEX-1] = '0';
			pst->path_c0[DXT1_PATH_INDEX-2]++;
			pst->path_c1[DXT1_PATH_INDEX-2]++;
			pst->path_b0[DXT1_PATH_INDEX-2]++;
			pst->path_b1[DXT1_PATH_INDEX-2]++;
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


	if (pst->path_c0[DXT1_PATH_INDEX] == MAX_FRAME_LAST_CHAR  && pst->path_c0[DXT1_PATH_INDEX-1] == MAX_FRAME_SECN_CHAR && pst->path_c0[DXT1_PATH_INDEX-2] == MAX_FRAME_THRD_CHAR) {
			pst->path_c0[DXT1_PATH_INDEX] = '0';
			pst->path_c1[DXT1_PATH_INDEX] = '0';
			pst->path_b0[DXT1_PATH_INDEX] = '0';
			pst->path_b1[DXT1_PATH_INDEX] = '0';
			pst->path_c0[DXT1_PATH_INDEX-1] = '0';
			pst->path_c1[DXT1_PATH_INDEX-1] = '0';
			pst->path_b0[DXT1_PATH_INDEX-1] = '0';
			pst->path_b1[DXT1_PATH_INDEX-1] = '0';
			pst->path_c0[DXT1_PATH_INDEX-2] = '0';
			pst->path_c1[DXT1_PATH_INDEX-2] = '0';
			pst->path_b0[DXT1_PATH_INDEX-2] = '0';
			pst->path_b1[DXT1_PATH_INDEX-2] = '0';
	}

	if (*paddr == DXT1_START_ADDR) {
		*paddr += len;
		*phdl += DXT1_HDL_NUMS;
	} else {
		*paddr = DXT1_START_ADDR;
		*phdl = DXT1_START_HDL;
	}
}

STATIC FTVOID dxt1Display (FTU32 hdl, FTU32 addr, FTU8 t)
{
	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0x0,0x0,0x0));
	HAL_CmdBufIn(CLEAR(1,1,1));

	HAL_CmdBufIn(SAVE_CONTEXT());
	//dxt1BitmapInfo(hdl, addr, WQVGA_WIDTH, WQVGA_HIGH);
	dxt1BitmapInfo(hdl, addr, 408, 272);

	HAL_CmdBufIn(BEGIN(BITMAPS));

	//dxt1FormatInfo(hdl,0,0);
	dxt1FormatInfo(hdl,36,0);
	HAL_CmdBufIn(RESTORE_CONTEXT());

	//HAL_CmdBufIn(COLOR_RGB(0xFF,0,0));
	//if (t) {
	//	CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/4,DXT1_TLT_FONT,OPT_CENTERX,"Due to limited memory in Arduino");
	//	CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,DXT1_TLT_FONT,OPT_CENTERX,"Frame exchange become really slow");
	//	CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/4*3,DXT1_TLT_FONT,OPT_CENTERX,"Better play me on MSVC");
	//} else {
	//	CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/4,DXT1_TLT_FONT,OPT_CENTERX,"Due to my frame capture tools limitation");
	//	CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,DXT1_TLT_FONT,OPT_CENTERX,"Frame border looks not good");
	//	CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/4*3,DXT1_TLT_FONT,OPT_CENTERX,"Not FT800's fault! But Frame itself!");
	//}
	HAL_CmdBufIn(COLOR_RGB(0x0,0x0,0x0));
	HAL_CmdBufIn(LINE_WIDTH(2*16));
	HAL_CmdBufIn(BEGIN(LINES));
	HAL_CmdBufIn(VERTEX2F(16 * 0, 16 * 1));
	HAL_CmdBufIn(VERTEX2F(16 * 480, 16 * 1));

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
}

FTVOID playfv (FTU32 para)
{
	FTU32 len, addr = DXT1_START_ADDR;
	FTU8 hdl = DXT1_START_HDL;
	dxt1_path_st *path = &vPath;

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

AppFunc APPS_UI[] = {
	playfv
};

