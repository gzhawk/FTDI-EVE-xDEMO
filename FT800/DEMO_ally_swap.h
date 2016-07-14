/* 
    Sample code to show how to do a 2 phase screen swap
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Jul
*/
#ifdef DEMO_ALLY_SWAP

#if 1
#define BG_A_INX ROOT_PATH"allyswap\\BG_A_inx.bin"
#define BG_A_LUT ROOT_PATH"allyswap\\BG_A_lut.bin"
#define BG_B_INX ROOT_PATH"allyswap\\BG_B_inx.bin"
#define BG_B_LUT ROOT_PATH"allyswap\\BG_B_lut.bin"
#else
#define BG_A_INX ROOT_PATH"allyswap\\BG_A_inx.raw"
#define BG_A_LUT ROOT_PATH"allyswap\\BG_A_lut.raw"
#define BG_B_INX ROOT_PATH"allyswap\\BG_B_inx.raw"
#define BG_B_LUT ROOT_PATH"allyswap\\BG_B_lut.raw"
#endif

#if 1
#define BG_A_C0 ROOT_PATH"allyswap\\BG_A_c0.bin"
#define BG_A_C1 ROOT_PATH"allyswap\\BG_A_c1.bin"
#define BG_A_B0 ROOT_PATH"allyswap\\BG_A_b0.bin"
#define BG_A_B1 ROOT_PATH"allyswap\\BG_A_b1.bin"
#define BG_B_C0 ROOT_PATH"allyswap\\BG_B_c0.bin"
#define BG_B_C1 ROOT_PATH"allyswap\\BG_B_c1.bin"
#define BG_B_B0 ROOT_PATH"allyswap\\BG_B_b0.bin"
#define BG_B_B1 ROOT_PATH"allyswap\\BG_B_b1.bin"
#else
#define BG_A_C0 ROOT_PATH"allyswap\\BG_A_c0.raw"
#define BG_A_C1 ROOT_PATH"allyswap\\BG_A_c1.raw"
#define BG_A_B0 ROOT_PATH"allyswap\\BG_A_b0.raw"
#define BG_A_B1 ROOT_PATH"allyswap\\BG_A_b1.raw"
#define BG_B_C0 ROOT_PATH"allyswap\\BG_B_c0.raw"
#define BG_B_C1 ROOT_PATH"allyswap\\BG_B_c1.raw"
#define BG_B_B0 ROOT_PATH"allyswap\\BG_B_b0.raw"
#define BG_B_B1 ROOT_PATH"allyswap\\BG_B_b1.raw"
#endif

#define BITMAP_W 800
#define BITMAP_H 480

#define BITMAP_NUM   (2)

#define HDL_DXT1  0
#define HDL_PAL8  (HDL_DXT1 + 2) /* DXT1 use 2 handle per bitmap */

#define GRAM_ADDR_PAL8     RAM_G
#define GRAM_ADDR_PAL8_LUT (GRAM_ADDR_PAL8 + BITMAP_W*BITMAP_H)
#define GRAM_ADDR_DXT1     (RAM_G + BITMAP_W*BITMAP_H + 1024)

typedef struct pal_path_ {
	FTC8 *path_inx;
	FTC8 *path_lut;
} pal_path_st;

pal_path_st pal_header[BITMAP_NUM] = {
    {BG_A_INX,   BG_A_LUT},
    {BG_B_INX,   BG_B_LUT},
};

typedef struct dxt1_path_ {
	FTC8 *path_c0;
	FTC8 *path_c1;
	FTC8 *path_b0;
	FTC8 *path_b1;
} dxt1_path_st;

dxt1_path_st dxt1_header[BITMAP_NUM] = {
	{BG_A_C0,BG_A_C1,BG_A_B0,BG_A_B1},
	{BG_B_C0,BG_B_C1,BG_B_B0,BG_B_B1}
};

FTINDEF FTU8 palFile (pal_path_st *path, FTU32 addrRAM)
{

	if (!appFileToRamG(path->path_inx,addrRAM,0,0,0)) {
		DBGPRINT;
		return 0;
	}

	if (!appFileToRamG(path->path_lut,addrRAM+BITMAP_W*BITMAP_H,0,0,0)) {
		DBGPRINT;
		return 0;
	}

	return 1;
}
FTINDEF FTU8 dxt1File (dxt1_path_st *path, FTU32 addrRAM)
{
#define DXT1_LEN 48000
	FTU32 addr = addrRAM;

	if (!appFileToRamG(path->path_c0,addr,0,0,0)) {
		DBGPRINT;
		return 0;
	}
	addr += DXT1_LEN;
	if (!appFileToRamG(path->path_c1,addr,0,0,0)) {
		DBGPRINT;
		return 0;
	}
	addr += DXT1_LEN;
	if (!appFileToRamG(path->path_b0,addr,0,0,0)) {
		DBGPRINT;
		return 0;
	}
	addr += DXT1_LEN;
	if (!appFileToRamG(path->path_b1,addr,0,0,0)) {
		DBGPRINT;
		return 0;
	}

	return 1;
}

FTINDEF FTVOID pal8BitmapDL (FTU8 Hdl, FTU32 Addr, FTU16 W, FTU16 H)
{
	HAL_DlpBufIn(BITMAP_HANDLE(Hdl));
	HAL_DlpBufIn(BITMAP_SOURCE(Addr));
	HAL_DlpBufIn(BITMAP_LAYOUT(PALETTED8, W, H));
    HAL_DlpBufIn(BITMAP_LAYOUT_H(W >> 10, H >> 9));
	HAL_DlpBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, W, H));
    HAL_DlpBufIn(BITMAP_SIZE_H(W>>9, H>>9));
}

FTINDEF FTVOID dxt1BitmapDL (FTU8 startHdl, FTU32 startAddr, FTU16 W, FTU16 H)
{
#define DXT1_BLOCK_NUMS (4)
	HAL_DlpBufIn(BITMAP_HANDLE(startHdl));
	HAL_DlpBufIn(BITMAP_SOURCE(startAddr + 2*(W/DXT1_BLOCK_NUMS*2*H/DXT1_BLOCK_NUMS)));
	HAL_DlpBufIn(BITMAP_LAYOUT(L1, W/DXT1_BLOCK_NUMS/2, H));
    HAL_DlpBufIn(BITMAP_LAYOUT_H(W/DXT1_BLOCK_NUMS/2 >> 10, H >> 9));
	HAL_DlpBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, W, H));
    HAL_DlpBufIn(BITMAP_SIZE_H(W>>9, H>>9));

	HAL_DlpBufIn(BITMAP_HANDLE(startHdl+1));
	HAL_DlpBufIn(BITMAP_SOURCE(startAddr));
	HAL_DlpBufIn(BITMAP_LAYOUT(RGB565, W/DXT1_BLOCK_NUMS*2, H/DXT1_BLOCK_NUMS));
    HAL_DlpBufIn(BITMAP_LAYOUT_H(W/DXT1_BLOCK_NUMS*2 >> 10, H/DXT1_BLOCK_NUMS >> 9));
	HAL_DlpBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, W, H));
    HAL_DlpBufIn(BITMAP_SIZE_H(W>>9, H>>9));
}

FTINDEF FTVOID fillBitmapDL (FTVOID)
{
    pal8BitmapDL(HDL_PAL8,GRAM_ADDR_PAL8,BITMAP_W,BITMAP_H);
    
    dxt1BitmapDL(HDL_DXT1,GRAM_ADDR_DXT1,BITMAP_W,BITMAP_H);
   
    HAL_DlpBufIn(DISPLAY());
    HAL_BufToReg(RAM_DL,0);
}

FTINDEF FTVOID DispDXT1 (FTU8 startHdl, FT16 X, FT16 Y)
{
	HAL_CmdBufIn(SAVE_CONTEXT());
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
	HAL_CmdBufIn(RESTORE_CONTEXT());
}

FTINDEF FTVOID ally_disp_dxt1 (FTU32 X, FTU32 Y, FTU32 addr, FTU32 hdl)
{
	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));
	HAL_CmdBufIn(BEGIN(BITMAPS));

	DispDXT1(hdl,X,Y);

	HAL_CmdBufIn(END());
	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
}

FTVOID dispPal8 (FTU32 X, FTU32 Y, FTU32 PalSrc, FTU32 hdl, FTU32 cell)
{
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

FTVOID ally_disp_pal8(FTU8 mode)
{
    HAL_CmdBufIn(CMD_DLSTART);
    HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
    HAL_CmdBufIn(CLEAR(1,1,1));

    HAL_CmdBufIn(BEGIN(BITMAPS));

    dispPal8(0,0,GRAM_ADDR_PAL8_LUT, HDL_PAL8, 0);

    HAL_CmdBufIn(END());
    HAL_CmdBufIn(DISPLAY());
    HAL_CmdBufIn(CMD_SWAP);

    HAL_BufToReg(RAM_CMD,0);
}

FTU8 ally_low (FTU8 m)
{
    /* load bitmap resources data into FT800 */
    if (!dxt1File(&dxt1_header[m], GRAM_ADDR_DXT1)) {
        return 0;
    }
    /* display low resolution background */
    ally_disp_dxt1(0, 0, GRAM_ADDR_DXT1, HDL_DXT1);
    return 1;
}

FTU8 ally_high (FTU8 m)
{
    /* load bitmap resources data into FT800 */
    if (!palFile(&pal_header[m], GRAM_ADDR_PAL8)) {
        return 0;
    }
    /* display high resolution background */
    ally_disp_pal8(m);
    return 1;
}

FTVOID ally_modeswap (FTU32 para)
{
    static FTU8 mode=0,init=1;

    /* never mind, it's for debug,
     * this part just for this routine 
     * jump out the outside caller when error happen */
    appGP.appIndex = 1;
    appGP.appPara = 0;

    if (init) {
        fillBitmapDL();
        init = 0;
    }

    if (!ally_low(mode)) {
        return;
    }

    if (!ally_high(mode)) {
        return;
    }

    /* detect if any touch */
    while (!TOUCHED);
    /* hold until untouch */
    while (TOUCHED);
    appGP.appIndex = 0;
    /* switch mode */
    mode = (mode == 0)?1:0;
}

AppFunc Apps[] = {
    ally_modeswap,
    /* Leave this NULL at the buttom of this array */
    NULL
};

#endif
