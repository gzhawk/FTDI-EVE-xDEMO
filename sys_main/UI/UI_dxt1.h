/* 
    Sample to play dxt1 function 
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2014/Jan
*/

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(MSVC2017EMU) 
#define PATH_SCN_C0 ROOT_PATH"dxt1\\s1_c0.raw"
#define PATH_SCN_C1 ROOT_PATH"dxt1\\s1_c1.raw"
#define PATH_SCN_B0 ROOT_PATH"dxt1\\s1_b0.raw"
#define PATH_SCN_B1 ROOT_PATH"dxt1\\s1_b1.raw"
#else
#define PATH_SCN_C0 ROOT_PATH"s1_c0.raw"
#define PATH_SCN_C1 ROOT_PATH"s1_c1.raw"
#define PATH_SCN_B0 ROOT_PATH"s1_b0.raw"
#define PATH_SCN_B1 ROOT_PATH"s1_b1.raw"
#endif

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(MSVC2017EMU) 
#define DXT1_PATH_LEN   (30)
#define DXT1_PATH_INDEX_RUN (16)
#else
#define DXT1_PATH_LEN   (8+1+3+1)
#define DXT1_PATH_INDEX_RUN (1)
#endif

#define DXT1_BLOCK_NUMS (4)
#define DXT1_START_ADDR RAM_G
#define DXT1_START_HDL  (0)
#define DXT1_HDL_NUMS   (2)
#define DXT1_TLT_FONT   (25)

#define DXT1_PIC_W      480
#define DXT1_PIC_H      272

#define S7_W 160
#define S7_H 136

typedef struct dxt1_path_ {
	FTU8 path_c0[DXT1_PATH_LEN];
	FTU8 path_c1[DXT1_PATH_LEN];
	FTU8 path_b0[DXT1_PATH_LEN];
	FTU8 path_b1[DXT1_PATH_LEN];
} dxt1_path_st;

dxt1_path_st ScnPath = {
	PATH_SCN_C0,
	PATH_SCN_C1,
	PATH_SCN_B0,
	PATH_SCN_B1
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
    HAL_CmdBufIn(BITMAP_LAYOUT_H(W/DXT1_BLOCK_NUMS*2 >> 10, H/DXT1_BLOCK_NUMS >> 9));
	HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, W, H));
    HAL_CmdBufIn(BITMAP_SIZE_H(W>>9, H>>9));

	HAL_CmdBufIn(BITMAP_HANDLE(startHdl));
	HAL_CmdBufIn(BITMAP_SOURCE(startAddr + 2*(W/DXT1_BLOCK_NUMS*2*H/DXT1_BLOCK_NUMS)));
	HAL_CmdBufIn(BITMAP_LAYOUT(L1, W/DXT1_BLOCK_NUMS/2, H));
    HAL_CmdBufIn(BITMAP_LAYOUT_H(W/DXT1_BLOCK_NUMS/2 >> 10, H >> 9));
	HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, W, H));
    HAL_CmdBufIn(BITMAP_SIZE_H(W>>9, H>>9));
}

STATIC FTVOID dxt1FormatInfo (FTU8 startHdl, FT16 X, FT16 Y)
{
	HAL_CmdBufIn(BLEND_FUNC(ONE,ZERO));
	HAL_CmdBufIn(COLOR_A(0x55));
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl));
	HAL_CmdBufIn(CELL(0));
	HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

	HAL_CmdBufIn(BLEND_FUNC(ONE,ONE));
	HAL_CmdBufIn(COLOR_A(0xAA));
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl));
	HAL_CmdBufIn(CELL(1));
	HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

	HAL_CmdBufIn(COLOR_MASK(1,1,1,0));

	CoCmd_LOADIDENTITY;
	CoCmd_SCALE(4*EVE_TRANSFORM_MAX, 4*EVE_TRANSFORM_MAX);
	CoCmd_SETMATRIX;

	HAL_CmdBufIn(BLEND_FUNC(DST_ALPHA,ZERO));
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl+1));
	HAL_CmdBufIn(CELL(1));
	HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

	HAL_CmdBufIn(BLEND_FUNC(ONE_MINUS_DST_ALPHA,ONE));
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl+1));
	HAL_CmdBufIn(CELL(0));
	HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

}

STATIC FTVOID dxt1Info (FTU8 startHdl, FTU32 startAddr, FT16 X, FT16 Y, FTU16 W, FTU16 H)
{
	HAL_CmdBufIn(SAVE_CONTEXT());
	dxt1BitmapInfo(startHdl, startAddr, W, H);

	HAL_CmdBufIn(BEGIN(BITMAPS));

	dxt1FormatInfo(startHdl,X,Y);
	HAL_CmdBufIn(RESTORE_CONTEXT());
	
	HAL_CmdBufIn(END());
}

STATIC FTVOID valueChg (FTU32 *paddr, FTU8 *phdl, dxt1_path_st *pst, FTU32 len)
{
	if (pst->path_c0[DXT1_PATH_INDEX_RUN] == '7') {
		strcpy((char *)pst->path_c0,(char *)PATH_SCN_C0);
		strcpy((char *)pst->path_c1,(char *)PATH_SCN_C1);
		strcpy((char *)pst->path_b0,(char *)PATH_SCN_B0);
		strcpy((char *)pst->path_b1,(char *)PATH_SCN_B1);
	} else {
		pst->path_c0[DXT1_PATH_INDEX_RUN]++;
		pst->path_c1[DXT1_PATH_INDEX_RUN]++;
		pst->path_b0[DXT1_PATH_INDEX_RUN]++;
		pst->path_b1[DXT1_PATH_INDEX_RUN]++;
	}

	if (*paddr == DXT1_START_ADDR) {
		*paddr += len;
		*phdl += DXT1_HDL_NUMS;
	} else {
		*paddr = DXT1_START_ADDR;
		*phdl = DXT1_START_HDL;
	}
}

STATIC FTVOID dxt1Display (FTU8 fn, FTU32 hdl, FTU32 addr, FTU8 t)
{
	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
	HAL_CmdBufIn(CLEAR(1,1,1));

	if (fn != '7') {
		dxt1Info(hdl, addr, 0, 0, DXT1_PIC_W, DXT1_PIC_H);
	} else {
		dxt1Info(hdl, addr, -1*S7_W/4, -1*S7_H/4, S7_W, S7_H);
		dxt1Info(hdl, addr, DXT1_PIC_W-S7_W-20, DXT1_PIC_H-S7_H-20, S7_W, S7_H);

		HAL_CmdBufIn(COLOR_RGB(0,0,0xFF));
		CoCmd_TEXT(S7_W/4*3,S7_H/4*3/2,DXT1_TLT_FONT-2,0,"Any size, negative coordinate");
		CoCmd_TEXT(50,DXT1_PIC_H-S7_H/2-20,DXT1_TLT_FONT-2,0,"Any size, positive coordinate");
	}

	if (t) {
		CoCmd_TEXT(DXT1_PIC_W/2,DXT1_PIC_H/2,DXT1_TLT_FONT,OPT_CENTERX,"Tap any place go next screen");
	} else {
		CoCmd_SPINNER(DXT1_PIC_W/2,DXT1_PIC_H/2,OPT_CENTER,0);
	}

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
}

FTVOID playdxt1 (FTU32 para)
{
	FTU32 len, addr = DXT1_START_ADDR;
	FTU8 hdl = DXT1_START_HDL;
	dxt1_path_st *path = &ScnPath;

#if defined(STM32F4)
	return;
#endif	
	len = dxt1File(path,addr);
	if (len == 0) {
		DBGPRINT;
		return;
	}

	do {
		dxt1Display(path->path_c0[DXT1_PATH_INDEX_RUN], hdl, addr, 1);

		while (TOUCHED == 0) {
			/* wait for touching */
		}

		while (TOUCHED) {
			/* wait for releasing */
		}

#if !defined(MSVC2010EXPRESS) && !defined(MSVC2012EMU) && !defined(MSVC2017EMU) 
		dxt1Display(path->path_c0[DXT1_PATH_INDEX_RUN], hdl, addr, 0);
#endif

		valueChg(&addr, &hdl, path, len);

		len = dxt1File(path,addr);
		if (len == 0) {
			DBGPRINT;
			return;
		}

	} while (1);
}

AppFunc APPS_UI[] = {
	playdxt1
};

