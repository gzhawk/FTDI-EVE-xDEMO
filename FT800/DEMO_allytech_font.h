/* 
    Demo for AllyTech font/bitmap test
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Apr
*/
#ifdef DEMO_ALLY_FONT

/* change related info here for new font to be tested */
#define FONT_POINT  6
#define FONT_NUM    23
#define FONT_PATH   ROOT_PATH"allyfont\\L4.raw"
#define FONT_ADDR   (RAM_G+650000)
#define LINE1_COLOR COLOR_RGB(0,0xFF,0)
#define LINE2_COLOR COLOR_RGB(255,255,255)
FTU8 screen_title[FONT_NUM] = {1,2,3,4,5,6,7,8,9,10,0}; 
FT_Gpu_Fonts_t stFontBlock;

/* change related info here for new bitmap to be tested */
#define PIC_NUM      3 //don't larger than FONT_POINT
#define BMP_ARGB4444 ROOT_PATH"allyfont\\BMP_ARGB4444.raw"
#define BMP_ARGB1555 ROOT_PATH"allyfont\\BMP_ARGB1555.raw"
#define BMP_PAL8_INX ROOT_PATH"allyfont\\PAL8-inx.bin"
#define BMP_PAL8_LUT ROOT_PATH"allyfont\\PAL8-lut.bin"
#define BMP_ADDR     (RAM_G)
bmpHDR_st bmp_header[PIC_NUM] = {
    {BMP_ARGB4444,   0,           0,ARGB4,   0,0,214,43},
    {BMP_ARGB1555,   0,           0,ARGB1555,0,0,214,43},
    {BMP_PAL8_INX,   BMP_PAL8_LUT,0,PALETTED8,0,0,6,56},
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
FTVOID ally_font_main (FTU32 para)
{
	static FTU8 init;
    FTU16 h;

	/* if something wrong in the code, force out */
	appGP.appIndex = 1;
	
	if (!init) {
	    /* load the bitmap resources into FT800 */
		if(APP_OK != appBmpToRamG(0, BMP_ADDR, bmp_header, PIC_NUM)){
			DBGPRINT;
			return;
		}

	    /* load the font resources into FT800 */
		if (0 == appFileToRamG(FONT_PATH,FONT_ADDR,0,(FTU8 *)&stFontBlock,
                               sizeof(FT_Gpu_Fonts_t))) {
			DBGPRINT;
			return;
		}
        /* only do it once */
    	init = 1;
	}

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));
	/* set the external font to each font index */
	CoCmd_SETFONT(FONT_POINT, FONT_ADDR, &stFontBlock);
   
    /* bitmap display */
    HAL_CmdBufIn(BEGIN(BITMAPS));
    HAL_CmdBufIn(BITMAP_HANDLE(0));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(VERTEX2F(0*FT800_PIXEL_UNIT,0*FT800_PIXEL_UNIT));    
    HAL_CmdBufIn(BITMAP_HANDLE(1));
    HAL_CmdBufIn(CELL(0));
    h = bmp_header[0].high;
    HAL_CmdBufIn(VERTEX2F(0*FT800_PIXEL_UNIT,h*FT800_PIXEL_UNIT));    
    
    /* Palette use different way to display */
    h += bmp_header[1].high;
    dispPal8(100,h,bmp_header[2].lut_src,2,0);
    HAL_CmdBufIn(END());

    /* font display */
	HAL_CmdBufIn(LINE1_COLOR);
    h += bmp_header[2].high;
	CoCmd_TEXT(0,h,FONT_POINT,OPT_CENTERX,screen_title);
    h += stFontBlock.FontHeightInPixels;
	HAL_CmdBufIn(LINE2_COLOR);
	HAL_CmdBufIn(VERTEX2II(stFontBlock.FontWidthInPixels,h,FONT_POINT,1));
    h += stFontBlock.FontHeightInPixels;
	HAL_CmdBufIn(VERTEX2II(2*stFontBlock.FontWidthInPixels,h,FONT_POINT,2));

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
	
	appGP.appIndex = 0;
	
	return;
}

AppFunc Apps[] = {
	ally_font_main,
	/* Leave this NULL at the buttom of this array */
	NULL
};

#endif


