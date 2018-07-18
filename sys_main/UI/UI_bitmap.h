/* 
    Sample code to show how to display bitmap
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Jan
            2018/Jun - Add ASTC support
*/

#if defined(VC_MPSSE) || defined(VC_EMULATOR) || defined(FT9XXEV)
#define DISPBP_L1              ROOT_PATH"bitmap\\L1.raw"
#define DISPBP_L8              ROOT_PATH"bitmap\\L8.raw"
#define DISPBP_RGB332          ROOT_PATH"bitmap\\RGB332.raw"
#define DISPBP_RGB565          ROOT_PATH"bitmap\\RGB565.raw"
#define DISPBP_ARGB4           ROOT_PATH"bitmap\\ARGB4.raw"
/* use zlib compressed file to test the INFLATE function */
#define DISPBP_ARGB1555        ROOT_PATH"bitmap\\ARGB1555.bin"

#if defined(DEF_81X) || defined(DEF_BT81X)
#define DISPBP_PALETTE8        ROOT_PATH"bitmap\\Pal8_inx.raw"
#define DISPBP_PALETTE8_LUT    ROOT_PATH"bitmap\\Pal8_lut.raw"
/* use zlib compressed file to test the INFLATE function */
#define DISPBP_PALETTE565      ROOT_PATH"bitmap\\Pal565_inx.bin"
#define DISPBP_PALETTE565_LUT  ROOT_PATH"bitmap\\Pal565_lut.bin"
#define DISPBP_PALETTE4444     ROOT_PATH"bitmap\\Pal4444_inx.raw"
#define DISPBP_PALETTE4444_LUT ROOT_PATH"bitmap\\Pal4444_lut.raw"
/* use zlib compressed file to test the INFLATE function*/
#define DISPBP_ASTC4X4_RAM     ROOT_PATH"bitmap\\ASTC4x4.bin"
/* use zlib compressed file to test the INFLATE function from Flash*/
#define DISPBP_ASTC4X4_ZFLH    "ZFLASH:2217216"
/* use raw Flash file to test the display function from Flash*/
#define DISPBP_ASTC4X4_FLH     "FLASH:2225344"

#else
#define DISPBP_PALETTE         ROOT_PATH"bitmap\\Pal_inx.raw"
#define DISPBP_PALETTE_LUT     ROOT_PATH"bitmap\\Pal_lut.raw"
#endif

#else

#define DISPBP_L1              ROOT_PATH"L1.raw"
#define DISPBP_L8              ROOT_PATH"L8.raw"
#define DISPBP_RGB332          ROOT_PATH"RGB332.raw"
#define DISPBP_RGB565          ROOT_PATH"RGB565.raw"
#define DISPBP_ARGB4           ROOT_PATH"ARGB4.raw"
#define DISPBP_ARGB1555        ROOT_PATH"ARGB1555.raw"

#if defined(DEF_81X) || defined(DEF_BT81X)
#define DISPBP_PALETTE8        ROOT_PATH"Pal8_inx.raw"
#define DISPBP_PALETTE8_LUT    ROOT_PATH"Pal8_lut.raw"
#define DISPBP_PALETTE565      ROOT_PATH"Pal565_inx.raw"
#define DISPBP_PALETTE565_LUT  ROOT_PATH"Pal565_lut.raw"
#define DISPBP_PALETTE4444     ROOT_PATH"Pal4444_inx.raw"
#define DISPBP_PALETTE4444_LUT ROOT_PATH"Pal4444_lut.raw"
#define DISPBP_ASTC4X4_RAM     ROOT_PATH"ASTC4x4.bin"
#define DISPBP_ASTC4X4_ZFLH    "ZFLASH:2217216"
#define DISPBP_ASTC4X4_FLH     "FLASH:2225344"

#else
#define DISPBP_PALETTE         ROOT_PATH"Pal_inx.raw"
#define DISPBP_PALETTE_LUT     ROOT_PATH"Pal_lut.raw"
#endif

#endif

#define HDL_START 0
#define FNT_WIDE  30
#define FNT_NUM   24
#define FNT_OPT   OPT_CENTERX

bmpHDR_st bmp_header[] = {
#if defined(DEF_81X) || defined(DEF_BT81X)
    {DISPBP_PALETTE8,   DISPBP_PALETTE8_LUT,   0,PALETTED8,   0,0,128,128},
    {DISPBP_PALETTE4444,DISPBP_PALETTE4444_LUT,0,PALETTED4444,0,0,128,128},
    {DISPBP_PALETTE565, DISPBP_PALETTE565_LUT, 0,PALETTED565, 0,0,128,128},

#if defined(DEF_BT81X)
    {DISPBP_ASTC4X4_RAM, 0,0,COMPRESSED_RGBA_ASTC_4x4_KHR,    0,0,128,128},
    {DISPBP_ASTC4X4_ZFLH,0,0,COMPRESSED_RGBA_ASTC_4x4_KHR,    0,0,128,128},
    {DISPBP_ASTC4X4_FLH, 0,0,COMPRESSED_RGBA_ASTC_4x4_KHR,    0,0,128,128},
#endif

#else
    {DISPBP_PALETTE,    DISPBP_PALETTE_LUT,    0,PALETTED,    0,0,128,128},
#endif
    {DISPBP_L1,         0,                     0,L1,          0,0,128,128},
    {DISPBP_L8,         0,                     0,L8,          0,0,128,128},
    {DISPBP_ARGB1555,   0,                     0,ARGB1555,    0,0,128,128},
    {DISPBP_ARGB4,      0,                     0,ARGB4,       0,0,128,128},
    {DISPBP_RGB565,     0,                     0,RGB565,      0,0,128,128},
    {DISPBP_RGB332,     0,                     0,RGB332,      0,0,128,128},
};

FTVOID dispTEXT (FTU32 format, FTU32 X, FTU32 Y)
{
    switch (format) {
        case L1:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"L1");
            break;
        case L8:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"L8");
            break;
        case RGB565:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"RGB565");
            break;
        case RGB332:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"RGB332");
            break;
        case ARGB4:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"ARGB4");
            break;
        case ARGB1555:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"ARGB1555");
            break;
#if defined(DEF_BT81X)
        case COMPRESSED_RGBA_ASTC_4x4_KHR:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"ASTC4x4");
            break;
#endif
#if defined(DEF_81X) || defined(DEF_BT81X)
        case PALETTED8:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"PAL8");
            break;
        case PALETTED565:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"PAL565");
            break;
        case PALETTED4444:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"PAL4444");
            break;
#else                
        case PALETTED:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"PALETTED");
            break;
#endif
        default:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"UNKNOWN");
            break;
    }
}

FTVOID dispTitle(FTU32 index, FTU32 X, FTU32 Y)
{
    HAL_CmdBufIn(SAVE_CONTEXT());
    HAL_CmdBufIn(COLOR_RGB(0,0,255));
    dispTEXT(bmp_header[index].format, X, Y);
    HAL_CmdBufIn(RESTORE_CONTEXT());
}

#if defined(DEF_81X) || defined(DEF_BT81X)
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
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

    HAL_CmdBufIn(BLEND_FUNC(DST_ALPHA, ONE_MINUS_DST_ALPHA));
    HAL_CmdBufIn(COLOR_MASK(1,0,0,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 2));
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

    HAL_CmdBufIn(COLOR_MASK(0,1,0,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 1));
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

    HAL_CmdBufIn(COLOR_MASK(0,0,1,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 0));
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

    HAL_CmdBufIn(RESTORE_CONTEXT());
}
#endif

FTVOID dispImage(FTU32 index, FTU32 X, FTU32 Y)
{
#if defined(DEF_81X) || defined(DEF_BT81X)
    if (PALETTED8 == bmp_header[index].format) {
        dispPal8(X,Y,bmp_header[index].lut_src, index, 0);
    } else if (PALETTED565 == bmp_header[index].format || 
               PALETTED4444 == bmp_header[index].format) {
        HAL_CmdBufIn(PALETTE_SOURCE(bmp_header[index].lut_src));
    }
#endif
    if (L1 == bmp_header[index].format || 
        L8 == bmp_header[index].format) {
        /* for simple image such as L1,L4,L8
           if no different color be given, it would be the same color
           as the background, make the image invisable */
        HAL_CmdBufIn(SAVE_CONTEXT());
        HAL_CmdBufIn(COLOR_RGB(0,0,0));
        HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
        HAL_CmdBufIn(RESTORE_CONTEXT());   
    } else {
        HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
    }

}

FTVOID disp_bitmap (FTU32 para)
{
	static FTU8 flag = 0;
    FTU32 i,j,X,Y,n = sizeof(bmp_header)/sizeof(bmpHDR_st);

	/* only load the file once */
	if (flag == 0) {
		/* load bitmap resources data into EVE */
		if(APP_OK != appBmpToRamG(HDL_START, RAM_G, bmp_header, n)){
			DBGPRINT;
			return;
		}
		flag = 1;
	}

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255,255,255));
	HAL_CmdBufIn(CLEAR(1,1,1));
	
    HAL_CmdBufIn(BEGIN(BITMAPS));
   
    for (i = 0, j = (n%2)?(n/2+1):(n/2); 
         i < n; i++) {
        HAL_CmdBufIn(BITMAP_HANDLE(i));
        HAL_CmdBufIn(CELL(0));

        if (i < j) {
            X = EVE_LCD_WIDTH/(j + 1)*(i + 1) - bmp_header[i].wide/2;
            Y = 0;
            dispTitle(i,X+bmp_header[i].wide/2,Y);
            Y = FNT_WIDE;
            dispImage(i,X,Y);
        } else {
            X = EVE_LCD_WIDTH/(n - j + 1)*(i - j + 1) - bmp_header[i].wide/2;
            Y = 2*FNT_WIDE+bmp_header[i].high;
            dispImage(i,X,Y);
            Y += bmp_header[i].high;
            dispTitle(i,X+bmp_header[i].wide/2,Y);
        }
    }

    HAL_CmdBufIn(END());
    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

    HAL_BufToReg(RAM_CMD,0);
	appGP.appIndex = 0;
}

AppFunc APPS_UI[] = {
	disp_bitmap
};

