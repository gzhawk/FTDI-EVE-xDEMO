/* 
    Sample code to show WXGA (1280x800) screen
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Oct
*/

#if defined(VC_EMULATOR)
#error "copy res/wxga/eveflh.bin to res/eveflh, then comment this line"
#elif defined(VC_MPSSE) || defined(VC_FT4222)
#error "program res/wxga/eveflh.bin to eve-connected-flash, then comment this line"
#endif

/* raw file in EVE EVEFLH, display from GRAM*/
#define IMG_1      "EVEFLH@4096:819200"
#define IMG_2      "EVEFLH@823296:819200"
#define IMG_3      "EVEFLH@1642496:819200"

#define HDL_START      0
#define FNT_NUM        24
#define FNT_OPT        0
ImgInfo_st info_header[] = {
    {IMG_1,0,0,0},
    {IMG_2,0,0,0},
    {IMG_3,0,0,0},
};
bmpHDR_st bmp_header[] = {
    {COMPRESSED_RGBA_ASTC_5x4_KHR,1280,800,(FTU32)&info_header[0]},
    {COMPRESSED_RGBA_ASTC_5x4_KHR,1280,800,(FTU32)&info_header[1]},
    {COMPRESSED_RGBA_ASTC_5x4_KHR,1280,800,(FTU32)&info_header[2]},
};


FTVOID DispImage(FTU32 index, FTU32 X, FTU32 Y)
{
	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));
   
    HAL_CmdBufIn(BEGIN(BITMAPS));
    HAL_CmdBufIn(BITMAP_HANDLE(HDL_START));
    HAL_CmdBufIn(CELL(0));

    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(SAVE_CONTEXT());
    HAL_CmdBufIn(COLOR_RGB(0,0,255));
    CoCmd_TEXT(X,Y,FNT_NUM,FNT_OPT,"ASTC5x4");
    HAL_CmdBufIn(RESTORE_CONTEXT());

    HAL_CmdBufIn(END());
    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

    HAL_BufToReg(RAM_CMD,0);
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
FTVOID wxga_3 (FTU32 para)
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

    DispImage(i,0,0);

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
	wxga_3
};

