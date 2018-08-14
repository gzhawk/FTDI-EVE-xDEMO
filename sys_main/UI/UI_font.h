/* 
    Demo for teaching how to use font
	1. download our font convert toos from 
       http://www.ftdichip.com/Support/Utilities/EVE/fnt_cvt_0.3.1.zip
	2. follow the instruction to generate the raw or rawh file
	   here, I would like to use raw file
	   http://www.ftdichip.com/Support/Documents/AppNotes/AN_277_FT800_Custom_Font_Implement.pdf
	3. after you got the raw file, follow the code below to see how it works in EVE
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2014/Sep
*/

#define FONT_1 6
#define FONT_2 7
#define FONT_2_W 60
#define FONT_2_H 70
#define FONT_LEN 10
#define FONT_LINE 3
#define LINE1_COLOR COLOR_RGB(0,0xFF,0)
#define LINE2_COLOR COLOR_RGB(0xFF,0xFF,0xFF)

FTU8 screen_title[FONT_LINE][FONT_LEN] = {
	{1,2,3,4,0},//ftdi
	{1,2,0},//we
	{3,4,5,6,7,8,0}//make design easy
}; 

#if defined(FILESYS_USED) 
#define PATH_LINE1 ROOT_PATH"font\\line1.raw"
#define PATH_LINE2 ROOT_PATH"font\\line2.raw"
#else
#define PATH_LINE1 ROOT_PATH"line1.raw"
#define PATH_LINE2 ROOT_PATH"line2.raw"
#endif

#define LINE1_ADDR RAM_G
#define LINE2_ADDR (21124)//the size of line1.raw

FTU8 gIndex = 0;

FTVOID font_main (FTU32 para)
{
	static FTU8 init = 1;

	static FT_Gpu_Fonts_t stFB1,stFB2;

	/* load the font resources into EVE */
	if (init) {
		if (0 == appFileToRamG(PATH_LINE1,LINE1_ADDR,0,(FTU8 *)&stFB1,sizeof(FT_Gpu_Fonts_t))) {
			DBGPRINT;
			return;
		}
		if (0 == appFileToRamG(PATH_LINE2,LINE2_ADDR,0,(FTU8 *)&stFB2,sizeof(FT_Gpu_Fonts_t))) {
			DBGPRINT;
			return;
		}
	}

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));

	/* set the external font to each font index */
	CoCmd_SETFONT(FONT_1, LINE1_ADDR, &stFB1);
	CoCmd_SETFONT(FONT_2, LINE2_ADDR, &stFB2);

	HAL_CmdBufIn(LINE1_COLOR);
	CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/4-FONT_2_H,FONT_1,OPT_CENTERX,&screen_title[0][0]);
	HAL_CmdBufIn(LINE2_COLOR);
	CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2-FONT_2_H/2,FONT_2,OPT_CENTER,&screen_title[1][0]);
	CoCmd_KEY(EVE_LCD_WIDTH/2-FONT_2_W/2,EVE_LCD_HIGH/2+FONT_2_H/2,FONT_2_W,FONT_2_H,FONT_2,OPT_CENTER,&screen_title[2][0]);

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
	
	init = 0;

	appGP.appIndex = 0;
	gIndex = 0;
	
	return;
}

AppFunc APPS_UI[] = {
	font_main
};

