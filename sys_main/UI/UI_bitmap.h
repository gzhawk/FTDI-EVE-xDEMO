/* 
    Sample code to show how to display bitmap
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Jan
*/

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(FT9XXEV)

#define DISPBP_RGB332 ROOT_PATH"bitmap\\RGB332.raw"
#define DISPBP_RGB565 ROOT_PATH"bitmap\\RGB565.raw"
#define DISPBP_ARGB4 ROOT_PATH"bitmap\\ARGB4.raw"
/* use zlib compressed file to test the INFLATE function */
#define DISPBP_ARGB1555 ROOT_PATH"bitmap\\ARGB1555.bin"
#define DISPBP_L8 ROOT_PATH"bitmap\\L8.raw"
#ifdef DEF_81X
#define DISPBP_PALETTE8 ROOT_PATH"bitmap\\Pal8_inx.raw"
#define DISPBP_PALETTE8_LUT ROOT_PATH"bitmap\\Pal8_lut.raw"
/* use zlib compressed file to test the INFLATE function */
#define DISPBP_PALETTE565 ROOT_PATH"bitmap\\Pal565_inx.bin"
/* use zlib compressed file to test the INFLATE function */
#define DISPBP_PALETTE565_LUT ROOT_PATH"bitmap\\Pal565_lut.bin"
#define DISPBP_PALETTE4444 ROOT_PATH"bitmap\\Pal4444_inx.raw"
#define DISPBP_PALETTE4444_LUT ROOT_PATH"bitmap\\Pal4444_lut.raw"
#else
#define DISPBP_PALETTE ROOT_PATH"bitmap\\Pal_inx.raw"
#define DISPBP_PALETTE_LUT ROOT_PATH"bitmap\\Pal_lut.raw"
#endif

#else

#define DISPBP_RGB332 ROOT_PATH"RGB332.raw"
#define DISPBP_RGB565 ROOT_PATH"RGB565.raw"
#define DISPBP_ARGB4 ROOT_PATH"ARGB4.raw"
#define DISPBP_ARGB1555 ROOT_PATH"ARGB1555.raw"
#define DISPBP_L8 ROOT_PATH"L8.raw"
#ifdef DEF_81X
#define DISPBP_PALETTE8 ROOT_PATH"Pal8_inx.raw"
#define DISPBP_PALETTE8_LUT ROOT_PATH"Pal8_lut.raw"
#define DISPBP_PALETTE565 ROOT_PATH"Pal565_inx.raw"
#define DISPBP_PALETTE565_LUT ROOT_PATH"Pal565_lut.raw"
#define DISPBP_PALETTE4444 ROOT_PATH"Pal4444_inx.raw"
#define DISPBP_PALETTE4444_LUT ROOT_PATH"Pal4444_lut.raw"
#else
#define DISPBP_PALETTE ROOT_PATH"Pal_inx.raw"
#define DISPBP_PALETTE_LUT ROOT_PATH"Pal_lut.raw"
#endif

#endif

#define FNT_WIDE  30

#define BMP_NUM   4

#ifdef DEF_81X
#define PAL_NUM   3
#else
#define PAL_NUM   1
#endif

#define RAW_NUM   (BMP_NUM+PAL_NUM)
#define HDL_START 0
bmpHDR_st bmp_header[RAW_NUM] = {
    {DISPBP_ARGB1555,   0,                     0,ARGB1555,    0,0,128,128},
    {DISPBP_ARGB4,      0,                     0,ARGB4,       0,0,128,128},
    {DISPBP_RGB332,     0,                     0,RGB332,      0,0,128,128},
    {DISPBP_RGB565,     0,                     0,RGB565,      0,0,128,128},
#ifdef DEF_81X
    {DISPBP_PALETTE4444,DISPBP_PALETTE4444_LUT,0,PALETTED4444,0,0,128,128},
    {DISPBP_PALETTE8,   DISPBP_PALETTE8_LUT,   0,PALETTED8,   0,0,128,128},
    {DISPBP_PALETTE565, DISPBP_PALETTE565_LUT, 0,PALETTED565, 0,0,128,128},
#else
    {DISPBP_PALETTE,    DISPBP_PALETTE_LUT,    0,PALETTED,    0,0,128,128}
#endif
};

FTVOID dispTEXT (FTU32 format, FTU32 X)
{
    switch (format) {
        case L8:
            CoCmd_TEXT(X,0,24,OPT_CENTERX,"L8");
            break;
        case RGB565:
            CoCmd_TEXT(X,0,24,OPT_CENTERX,"RGB565");
            break;
        case RGB332:
            CoCmd_TEXT(X,0,24,OPT_CENTERX,"RGB332");
            break;
        case ARGB4:
            CoCmd_TEXT(X,0,24,OPT_CENTERX,"ARGB4");
            break;
        case ARGB1555:
            CoCmd_TEXT(X,0,24,OPT_CENTERX,"ARGB1555");
            break;
#ifdef DEF_81X
        case PALETTED8:
            CoCmd_TEXT(X,(FT800_LCD_HIGH-FNT_WIDE),24,OPT_CENTERX,"PALETTED8");
            break;
        case PALETTED565:
            CoCmd_TEXT(X,(FT800_LCD_HIGH-FNT_WIDE),24,OPT_CENTERX,"PALETTED565");
            break;
        case PALETTED4444:
            CoCmd_TEXT(X,(FT800_LCD_HIGH-FNT_WIDE),24,OPT_CENTERX,"PALETTED4444");
            break;
#else                
        case PALETTED:
            CoCmd_TEXT(X,(FT800_LCD_HIGH-FNT_WIDE),24,OPT_CENTERX,"PALETTED");
            break;
#endif
        default:
            CoCmd_TEXT(X,(FT800_LCD_HIGH-FNT_WIDE),24,OPT_CENTERX,"Unknown");
            break;
    }
}

#ifdef DEF_81X
FTVOID dispPal8 (FTU32 X, FTU32 Y, FTU32 PalSrc, FTU32 hdl, FTU32 cell)
{
    /* every thing after this commands would not display
       if not use save/restore context */
    HAL_CmdBufIn(SAVE_CONTEXT());
    HAL_CmdBufIn(BITMAP_HANDLE(hdl));
    HAL_CmdBufIn(CELL(cell));

    HAL_CmdBufIn(BLEND_FUNC(ONE, ZERO));
    HAL_CmdBufIn(COLOR_MASK(0,0,0,1));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 3));
    HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

    HAL_CmdBufIn(BLEND_FUNC(DST_ALPHA, ONE_MINUS_DST_ALPHA));
    HAL_CmdBufIn(COLOR_MASK(1,0,0,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 2));
    HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

    HAL_CmdBufIn(COLOR_MASK(0,1,0,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 1));
    HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

    HAL_CmdBufIn(COLOR_MASK(0,0,1,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 0));
    HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

    HAL_CmdBufIn(RESTORE_CONTEXT());
}
#endif

FTVOID disp_bitmap (FTU32 para)
{
	static FTU8 flag = 0;
    FTU32 i,j;

	/* never mind, it's for debug,
	 * this part just for this routine jump out the outside caller when error happen */
	appGP.appIndex = 1;
	appGP.appPara = 0;

	/* only load the file once */
	if (flag == 0) {
		/* load bitmap resources data into FT800 */
		if(APP_OK != appBmpToRamG(HDL_START, RAM_G, bmp_header, RAW_NUM)){
			DBGPRINT;
			return;
		}
		flag = 1;
	}

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255,255,255));
	HAL_CmdBufIn(CLEAR(1,1,1));
	
    HAL_CmdBufIn(BEGIN(BITMAPS));
   
    for (i = 0; i < RAW_NUM; i++) {
        HAL_CmdBufIn(BITMAP_HANDLE(i));
        HAL_CmdBufIn(CELL(0));

        /* bitmap display location */
        if (i < BMP_NUM) {
            j = (FT800_LCD_WIDTH/(BMP_NUM+1))*(i+1);
            HAL_CmdBufIn(VERTEX2F((j-bmp_header[i].wide/2)*FT800_PIXEL_UNIT,
                                    FNT_WIDE*FT800_PIXEL_UNIT));
        } else {
            j = (FT800_LCD_WIDTH/(PAL_NUM+1))*(i-BMP_NUM+1);
#ifdef DEF_81X
            if (PALETTED8 == bmp_header[i].format) {
                dispPal8((j-bmp_header[i].wide/2), 
                         (FT800_LCD_HIGH-bmp_header[i].high-FNT_WIDE), 
                         bmp_header[i].lut_src, i, 0);
            } else {
                HAL_CmdBufIn(PALETTE_SOURCE(bmp_header[i].lut_src));
                HAL_CmdBufIn(VERTEX2F((j-bmp_header[i].wide/2)*FT800_PIXEL_UNIT,
                                    (FT800_LCD_HIGH-bmp_header[i].high-FNT_WIDE)*FT800_PIXEL_UNIT));
            }
#else
            HAL_CmdBufIn(VERTEX2F((j-bmp_header[i].wide/2)*FT800_PIXEL_UNIT,
                                    (FT800_LCD_HIGH-bmp_header[i].high-FNT_WIDE)*FT800_PIXEL_UNIT));
#endif
        }
        /* format title display */
        HAL_CmdBufIn(SAVE_CONTEXT());
        HAL_CmdBufIn(COLOR_RGB(0,0,255));
        dispTEXT(bmp_header[i].format, j);
        HAL_CmdBufIn(RESTORE_CONTEXT());
    }

	HAL_CmdBufIn(END());
    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

    HAL_BufToReg(RAM_CMD,0);
	appGP.appIndex = 0;
}

AppFunc Apps[] = {
	disp_bitmap,
	/* Leave this NULL at the buttom of this array */
	NULL
};

