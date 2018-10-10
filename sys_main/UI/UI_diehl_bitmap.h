/* 
    Sample code to show Diehl's bitmap
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Sep
*/

#if defined(VC_EMULATOR)
#error "copy res/d_bitmap/bt81x.flash to res/flash, then comment this line"
#elif defined(VC_MPSSE) || defined(VC_FT4222)
#error "program res/d_bitmap/bt81x.flash to on-board flash, then comment this line"
#endif

/* raw file in Flash, display from RAM_G */
#define IMG1_INX       "FLASH@388096:384000"
#define IMG1_LUT       "FLASH@772096:1024"
#define IMG1_RGB565    "FLASH@773120:768000"
#define IMG2_INX       "FLASH@1925120:384000"
#define IMG2_LUT       "FLASH@2309120:1024"
#define IMG2_RGB565    "FLASH@2310144:768000"
#define IMG3_INX       "FLASH@3462144:384000"
#define IMG3_LUT       "FLASH@3846144:1024"
#define IMG3_RGB565    "FLASH@3847168:768000"
/* raw file in Flash, direct display from Flash*/
#define IMG1_ASTC      "ASTC_FLASH@4096"
#define IMG2_ASTC      "ASTC_FLASH@1541120"
#define IMG3_ASTC      "ASTC_FLASH@3078144"

#define HDL_START      0
#define FNT_NUM        24
#define FNT_OPT        0

bmpHDR_st bmp_header[] = {
    //{IMG1_INX,  IMG1_LUT, 0,PALETTED8, 0, 0, 800, 480},
    {IMG1_ASTC, 0,        0, COMPRESSED_RGBA_ASTC_4x4_KHR, 0, 0, 800, 480},
    //{IMG1_RGB565,      0, 0,RGB565,    0, 0, 800, 480},
    //{IMG2_INX,  IMG2_LUT, 0,PALETTED8, 0, 0, 800, 480},
    {IMG2_ASTC, 0,        0, COMPRESSED_RGBA_ASTC_4x4_KHR, 0, 0, 800, 480},
    //{IMG2_RGB565,      0, 0,RGB565,    0, 0, 800, 480},
    //{IMG3_INX,  IMG3_LUT, 0,PALETTED8, 0, 0, 800, 480},
    {IMG3_ASTC, 0,        0, COMPRESSED_RGBA_ASTC_4x4_KHR, 0, 0, 800, 480},
    //{IMG3_RGB565,      0, 0,RGB565,    0, 0, 800, 480},
};

FTVOID dispTEXT (FTU32 format, FTU32 X, FTU32 Y)
{
    switch (format) {
        case COMPRESSED_RGBA_ASTC_4x4_KHR:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"ASTC");
            break;
        case PALETTED8:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"PAL8");
            break;
        case RGB565:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"RGB565");
            break;
        default:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"UNKNOWN");
            break;
    }
}

FTVOID Title(FTU32 index, FTU32 X, FTU32 Y)
{
    HAL_CmdBufIn(SAVE_CONTEXT());
    HAL_CmdBufIn(COLOR_RGB(0,0,255));
    dispTEXT(bmp_header[index].format, X, Y);
    HAL_CmdBufIn(RESTORE_CONTEXT());
}

FTVOID Image(FTU32 index, FTU32 X, FTU32 Y)
{
    if (PALETTED8 == bmp_header[index].format) {
        appDispPalette8(X,Y,bmp_header[index].lut_src, index, 0);
    } else {
        HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
    }

    Title(index,X,Y);
}

FTVOID clear_screen(FTVOID)
{
    HAL_CmdBufIn(CMD_DLSTART);
    HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
    HAL_CmdBufIn(CLEAR(1,1,1));
    HAL_CmdBufIn(DISPLAY());
    HAL_CmdBufIn(CMD_SWAP);
    HAL_BufToReg(RAM_CMD,0);
}

//#define TST
#define TICK_DELAY 200
FTVOID diehl_3 (FTU32 para)
{
	static FTU8 flag = 0, i = 0, t = 0;
#if !defined(CAL_NEEDED)
    static FTU8 tick = 0;
#endif

#ifdef TST
    FTU32 n = 9;
#else
    FTU32 n = sizeof(bmp_header)/sizeof(bmpHDR_st);

	/* only load each bitmap file once */
	if (flag == 0) {
        clear_screen();
		/* load bitmap resources data into EVE */
		if(APP_OK != appBmpToRamG(HDL_START, RAM_G, &bmp_header[i], 1)){
			DBGPRINT;
			return;
		}
		flag = 1;
	}
#endif

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));
#ifdef TST
    switch (i) {
        case 1:
	        CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2,28,OPT_CENTER,"TEST FOR THE GRADIENT-ALPHA");
            CoCmd_GRADIENTA(0,0,0xff00ff00,800,0,0x0000ff00);
            break;
        case 2:
	        CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2,28,OPT_CENTER,"TEST FOR THE GRADIENT-ALPHA");
            CoCmd_GRADIENTA(0,0,0xffff0000,800,0,0x00ff0000);
            break;
        case 3:
	        CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2,28,OPT_CENTER,"TEST FOR THE GRADIENT-ALPHA");
            CoCmd_GRADIENTA(0,0,0xff0000ff,800,0,0x000000ff);
            break;
        case 4:
	        CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2,28,OPT_CENTER,"TEST FOR THE GRADIENT-ALPHA");
            CoCmd_GRADIENTA(0,0,0xffA0A0A0,800,0,0x00a0a0a0);
            break;
        case 5:
            CoCmd_GRADIENT(0,0,0x000000,800,0,0xff0000);
	        CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2,28,OPT_CENTER,"TEST FOR THE GRADIENT-NO-ALPHA");
            break;
        case 6:
            CoCmd_GRADIENT(0,0,0x000000,800,0,0x00ff00);
	        CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2,28,OPT_CENTER,"TEST FOR THE GRADIENT-NO-ALPHA");
            break;
        case 7:
            CoCmd_GRADIENT(0,0,0x000000,800,0,0x0000ff);
	        CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2,28,OPT_CENTER,"TEST FOR THE GRADIENT-NO-ALPHA");
            break;
        case 8:
            CoCmd_GRADIENT(0,0,0x000000,800,0,0xa0a0a0);
	        CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2,28,OPT_CENTER,"TEST FOR THE GRADIENT-NO-ALPHA");
            break;
        default:
	        CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2,28,OPT_CENTER,"PRESS TO START");
            break;
    }
#else
    HAL_CmdBufIn(BEGIN(BITMAPS));
    HAL_CmdBufIn(BITMAP_HANDLE(HDL_START));
    HAL_CmdBufIn(CELL(0));
    Image(i,0,0);
#endif
    HAL_CmdBufIn(END());
    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

    HAL_BufToReg(RAM_CMD,0);
	appGP.appIndex = 0;

#if !defined(CAL_NEEDED)
    if (++tick == TICK_DELAY) {
        tick = 0;
        t = 1;
    }
#else
    /* hang if no touch */
    while (!TOUCHED);

    /* hang if keep touching */
    while (TOUCHED) {
        t = 1;
    };
#endif

    /* switch image if release from touching */
    if (t) {
        i = (i == (n - 1)) ? 0 : i + 1;
        t = 0;
        flag = 0;
    }
}

AppFunc APPS_UI[] = {
	diehl_3
};

