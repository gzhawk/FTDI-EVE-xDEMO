/* 
    Demo for AllyTech font sample
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Apr
*/
#ifdef DEMO_ALLY_FONT

#define FONT_1 6
#define FONT_LEN 23
#define LINE1_COLOR COLOR_RGB(0,0xFF,0)

FTU8 screen_title[FONT_LEN] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,0}; 

#define PATH_LINE1 ROOT_PATH"allyfont\\L4.raw"

#define LINE1_ADDR (RAM_G+650000)

FTU8 gIndex = 0;

FTVOID ally_font_main (FTU32 para)
{
	static FTU8 init = 1;

	static FT_Gpu_Fonts_t stFB1;

	/* if something wrong in the code, force out */
	appGP.appIndex = 1;
	
	/* load the font resources into FT800 */
	if (init) {
		if (0 == appFileToRamG(PATH_LINE1,LINE1_ADDR,0,(FTU8 *)&stFB1,sizeof(FT_Gpu_Fonts_t))) {
			DBGPRINT;
			return;
		}
	}

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,255));
	HAL_CmdBufIn(CLEAR(1,1,1));

	/* set the external font to each font index */
	CoCmd_SETFONT(FONT_1, LINE1_ADDR, &stFB1);

	HAL_CmdBufIn(LINE1_COLOR);
	CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,FONT_1,OPT_CENTERX,screen_title);
	HAL_CmdBufIn(VERTEX2II(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2+30,FONT_1,1));
	HAL_CmdBufIn(VERTEX2II(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2+60,FONT_1,2));

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
	
	init = 0;

	appGP.appIndex = 0;
	gIndex = 0;
	
	return;
}

AppFunc Apps[] = {
	ally_font_main,
	/* Leave this NULL at the buttom of this array */
	NULL
};

#endif


