/* 
    Sample code to show Diehl's bitmap
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Sep
*/

#if defined(VC_EMULATOR)
#error "copy res/d_bitmap/eveflh.bin to res/eveflh, then comment this line"
#elif defined(VC_MPSSE) || defined(VC_FT4222)
#error "program res/d_bitmap/eveflh.bin to eve-connected-flash, then comment this line"
#endif

/* raw file in EVEFLH, display from RAM_G */
#define IMG1_INX       "EVEFLH@388096:384000"
#define IMG1_LUT       "EVEFLH@772096:1024"
#define IMG1_RGB565    "EVEFLH@773120:768000"
#define IMG2_INX       "EVEFLH@1925120:384000"
#define IMG2_LUT       "EVEFLH@2309120:1024"
#define IMG2_RGB565    "EVEFLH@2310144:768000"
#define IMG3_INX       "EVEFLH@3462144:384000"
#define IMG3_LUT       "EVEFLH@3846144:1024"
#define IMG3_RGB565    "EVEFLH@3847168:768000"
/* raw file in EVEFLH, direct display from EVEFLH*/
#define IMG1_ASTC      "ASTC_EVEFLH@4096"
#define IMG2_ASTC      "ASTC_EVEFLH@1541120"
#define IMG3_ASTC      "ASTC_EVEFLH@3078144"

#define HDL_START      0
#define FNT_NUM        24
#define FNT_OPT        0

ImgInfo_st info_header[] = {
    {IMG1_ASTC,   0,0,0},
    {IMG2_ASTC,   0,0,0},
    {IMG3_ASTC,   0,0,0},
};

bmpHDR_st bmp_header[] = {
    {COMPRESSED_RGBA_ASTC_4x4_KHR, 800, 480, (FTU32)&info_header[HDL_START]},
    {COMPRESSED_RGBA_ASTC_4x4_KHR, 800, 480, (FTU32)&info_header[HDL_START+1]},
    {COMPRESSED_RGBA_ASTC_4x4_KHR, 800, 480, (FTU32)&info_header[HDL_START+2]},
};

FTVOID dispTEXT (FTU32 format, FTU32 X, FTU32 Y)
{
    switch (format) {
        case COMPRESSED_RGBA_ASTC_4x4_KHR:
            CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"ASTC");
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
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

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

#define TICK_DELAY 200
FTVOID diehl_3 (FTU32 para)
{
	static FTU8 flag = 0, i = 0, t = 0;
#if !defined(CAL_NEEDED)
    static FTU8 tick = 0;
#endif


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

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));

    HAL_CmdBufIn(BEGIN(BITMAPS));
    HAL_CmdBufIn(BITMAP_HANDLE(HDL_START));
    HAL_CmdBufIn(CELL(0));
    Image(i,0,0);
    
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

