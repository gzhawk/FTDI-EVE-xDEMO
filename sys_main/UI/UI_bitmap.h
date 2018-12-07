/* 
    Sample code to show how to display bitmap
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Jan
            2018/Jun - Add ASTC support
*/

#if defined(FILESYS_USED)

#define DXT1_B0                ROOT_PATH"bitmap\\DXT1_b0.raw"
#define DXT1_B1                ROOT_PATH"bitmap\\DXT1_b1.raw"
#define DXT1_C0                ROOT_PATH"bitmap\\DXT1_c0.raw"
#define DXT1_C1                ROOT_PATH"bitmap\\DXT1_c1.raw"
#define DISPBP_L1              ROOT_PATH"bitmap\\L1.raw"
#define DISPBP_L8              ROOT_PATH"bitmap\\L8.raw"
#define DISPBP_RGB332          ROOT_PATH"bitmap\\RGB332.raw"
#define DISPBP_RGB565          ROOT_PATH"bitmap\\RGB565.raw"
/* use zlib compressed file to test the INFLATE function */
#define DISPBP_ARGB1555        ROOT_PATH"bitmap\\ARGB1555.bin"
#define DISPBP_PALETTE8        ROOT_PATH"bitmap\\Pal8_inx.raw"
#define DISPBP_PALETTE8_LUT    ROOT_PATH"bitmap\\Pal8_lut.raw"
/* use zlib compressed file to test the INFLATE function */
#define DISPBP_PALETTE565      ROOT_PATH"bitmap\\Pal565_inx.bin"
#define DISPBP_PALETTE565_LUT  ROOT_PATH"bitmap\\Pal565_lut.bin"
#define DISPBP_PALETTE4444     ROOT_PATH"bitmap\\Pal4444_inx.raw"
#define DISPBP_PALETTE4444_LUT ROOT_PATH"bitmap\\Pal4444_lut.raw"
/* use zlib compressed file to test the INFLATE function*/
#define DISPBP_ASTC4X4_RAM     ROOT_PATH"bitmap\\ASTC4x4.bin"
#define DISPBP_PALETTE         ROOT_PATH"bitmap\\Pal_inx.raw"
#define DISPBP_PALETTE_LUT     ROOT_PATH"bitmap\\Pal_lut.raw"

#else

#define DXT1_B0                ROOT_PATH"DXT1_b0.raw"
#define DXT1_B1                ROOT_PATH"DXT1_b1.raw"
#define DXT1_C0                ROOT_PATH"DXT1_c0.raw"
#define DXT1_C1                ROOT_PATH"DXT1_c1.raw"
#define DISPBP_L1              ROOT_PATH"L1.raw"
#define DISPBP_L8              ROOT_PATH"L8.raw"
#define DISPBP_RGB332          ROOT_PATH"RGB332.raw"
#define DISPBP_RGB565          ROOT_PATH"RGB565.raw"
#define DISPBP_ARGB1555        ROOT_PATH"ARGB1555.raw"
#define DISPBP_PALETTE8        ROOT_PATH"Pal8_inx.raw"
#define DISPBP_PALETTE8_LUT    ROOT_PATH"Pal8_lut.raw"
#define DISPBP_PALETTE565      ROOT_PATH"Pal565_inx.raw"
#define DISPBP_PALETTE565_LUT  ROOT_PATH"Pal565_lut.raw"
#define DISPBP_PALETTE4444     ROOT_PATH"Pal4444_inx.raw"
#define DISPBP_PALETTE4444_LUT ROOT_PATH"Pal4444_lut.raw"
/* use zlib compressed file to test the INFLATE function*/
#define DISPBP_ASTC4X4_RAM     ROOT_PATH"ASTC4x4.bin"
#define DISPBP_PALETTE         ROOT_PATH"Pal_inx.raw"
#define DISPBP_PALETTE_LUT     ROOT_PATH"Pal_lut.raw"

#endif

/* zlib compressed file in flash, DEINFLATE in RAM_G and display*/
#define DISPBP_ASTC4X4_ZFLH    "Z_EVEFLH@2249984"
/* raw file in EVEFLH, display from RAM_G */
#define DISPBP_ARGB4_FLH       "EVEFLH@2217216:32768"
/* raw file in EVEFLH, direct display from EVEFLH*/
#define DISPBP_ASTC12X12_AFLH  "ASTC_EVEFLH@2257856"

#define HDL_START 0
#define FNT_WIDE  30
#define FNT_NUM   24
#define FNT_OPT   OPT_CENTERX

ImgInfo_st info_header[] = {
    {DISPBP_ASTC4X4_RAM,   0,0,0},
    {DISPBP_ASTC12X12_AFLH,0,0,0},
    {DISPBP_ASTC4X4_ZFLH,  0,0,0},
    {DISPBP_ARGB4_FLH,     0,0,0},
    {DISPBP_L1,            0,0,0},
    {DISPBP_L8,            0,0,0},
    {DISPBP_RGB565,        0,0,0},
    {DISPBP_ARGB1555,      0,0,0},
    {DISPBP_RGB332,        0,0,0},
};

ImgInfoPal_st info_pal_header[] = {
    {DISPBP_PALETTE8,   DISPBP_PALETTE8_LUT,   0,0,0,0,0}, 
    {DISPBP_PALETTE4444,DISPBP_PALETTE4444_LUT,0,0,0,0,0},
    {DISPBP_PALETTE565, DISPBP_PALETTE565_LUT, 0,0,0,0,0},
    {DISPBP_PALETTE,    DISPBP_PALETTE_LUT,    0,0,0,0,0},
};

ImgInfoDXT1_st info_header_dxt1 = {
    0,0,
    DXT1_C0,0,
    DXT1_C1,0,
    DXT1_B0,0,
    DXT1_B1,0
};

bmpHDR_st bmp_header[] = {
#if defined(DEF_81X) || defined(DEF_BT81X)
    {PALETTED8,   128,128,(FTU32)&info_pal_header[0]},
    {PALETTED4444,128,128,(FTU32)&info_pal_header[1]},
    {PALETTED565, 128,128,(FTU32)&info_pal_header[2]},
#if defined(DEF_BT81X)
    {COMPRESSED_RGBA_ASTC_4x4_KHR,  128,128,(FTU32)&info_header[0]},
    {COMPRESSED_RGBA_ASTC_12x12_KHR,132,132,(FTU32)&info_header[1]},
    {COMPRESSED_RGBA_ASTC_4x4_KHR,  128,128,(FTU32)&info_header[2]},
    {ARGB4,                         128,128,(FTU32)&info_header[3]},
#endif
#else
    {PALETTED,    128,128,(FTU32)&info_pal_header[3]},
#endif
    {L1,          128,128,(FTU32)&info_header[4]},
    {FORMAT_DXT1, 128,128,(FTU32)&info_header_dxt1},
    {L8,          128,128,(FTU32)&info_header[5]},
    {RGB565,      128,128,(FTU32)&info_header[6]},
    {ARGB1555,    128,128,(FTU32)&info_header[7]},
    {RGB332,      128,128,(FTU32)&info_header[8]},
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
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"4x4");
            break;
        case COMPRESSED_RGBA_ASTC_12x12_KHR:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"12x12");
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
        case FORMAT_DXT1:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"DXT1");
            break;
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

FTVOID dispNumber(FTU32 number, FTU32 X, FTU32 Y)
{
    HAL_CmdBufIn(SAVE_CONTEXT());
    HAL_CmdBufIn(COLOR_RGB(0,0,255));
    CoCmd_NUMBER(X,Y,FNT_NUM,FNT_OPT,number);
    HAL_CmdBufIn(RESTORE_CONTEXT());
}

FTVOID dispImage(FTU32 index, FTU32 X, FTU32 Y)
{
    switch (bmp_header[index].format) {
        case PALETTED8:
            HAL_CmdBufIn(BITMAP_HANDLE(((ImgInfoPal_st *)(bmp_header[index].info))->handle));
            appDispPalette8(X,Y,((ImgInfoPal_st *)(bmp_header[index].info))->addr_lut, index, 0);
            dispNumber(((ImgInfoPal_st *)(bmp_header[index].info))->handle,X+bmp_header[index].wide/2,Y+bmp_header[index].high/2);
            break;
        case PALETTED565:
        case PALETTED4444:
            HAL_CmdBufIn(BITMAP_HANDLE(((ImgInfoPal_st *)(bmp_header[index].info))->handle));
            HAL_CmdBufIn(PALETTE_SOURCE(((ImgInfoPal_st *)(bmp_header[index].info))->addr_lut));
            HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
            dispNumber(((ImgInfoPal_st *)(bmp_header[index].info))->handle,X+bmp_header[index].wide/2,Y+bmp_header[index].high/2);
            break;
        case L1:
        case L8:
            /* for simple image such as L1,L4,L8
               if no different color be given, it would be the same color
               as the background, make the image invisable */
            HAL_CmdBufIn(BITMAP_HANDLE(((ImgInfo_st *)(bmp_header[index].info))->handle));
            HAL_CmdBufIn(SAVE_CONTEXT());
            HAL_CmdBufIn(COLOR_RGB(0,0,0));
            HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
            HAL_CmdBufIn(RESTORE_CONTEXT());   
            dispNumber(((ImgInfo_st *)(bmp_header[index].info))->handle,X+bmp_header[index].wide/2,Y+bmp_header[index].high/2);
            break;
        case FORMAT_DXT1:
            appDispDXT1(((ImgInfoDXT1_st *)(bmp_header[index].info))->handle, X, Y);
            /* DXT1 use 2 handle for 1 image, so after DXT1 there would plus 1 */
            dispNumber(((ImgInfoDXT1_st *)(bmp_header[index].info))->handle,X+(bmp_header[index].wide-FNT_WIDE)/2,Y+bmp_header[index].high/2);
            dispNumber(((ImgInfoDXT1_st *)(bmp_header[index].info))->handle+1,X+(bmp_header[index].wide+FNT_WIDE)/2,Y+bmp_header[index].high/2);
            break;
        default:
            HAL_CmdBufIn(BITMAP_HANDLE(((ImgInfo_st *)(bmp_header[index].info))->handle));
            HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
            dispNumber(((ImgInfo_st *)(bmp_header[index].info))->handle,X+bmp_header[index].wide/2,Y+bmp_header[index].high/2);
            break;
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
   
    for (i = 0, j = (n%2)?(n/2+1):(n/2);i < n; i++) {
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

