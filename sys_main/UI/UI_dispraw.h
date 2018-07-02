/* 
    Sample code to show how to display RAW file
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2015/May
*/

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(FT9XXEV)
#define DISPRAW_PATH ROOT_PATH"dispraw\\test.raw"
#else
#define DISPRAW_PATH ROOT_PATH"test.raw"
#endif

FTVOID dispraw (FTU32 para)
{
	bmpHDR_st bmp_header = {
		DISPRAW_PATH,
		0,
		0,
		RGB565,
		0,
		0,
		480,
		272
	};
	static FTU8 flag = 0;
	
    /* only load the file once */
	if (flag == 0) {
		/* load bitmap resources data into EVE */
		if(APP_OK != appBmpToRamG(0, RAM_G, &bmp_header, 1)){
			DBGPRINT;
			return;
		}
		flag = 1;
	}

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));

	HAL_CmdBufIn(BEGIN(BITMAPS));
    HAL_CmdBufIn(BITMAP_HANDLE(0));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(VERTEX2F(0,0));
	HAL_CmdBufIn(END());
	CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2,24,OPT_CENTERX,"show the RAW file on screen");

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);

	appGP.appIndex = 0;
}

AppFunc APPS_UI[] = {
	dispraw
};

