/* 
    Demo for teaching how to use font
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2014/Sep
            2018/Dec
*/

#if defined(DEF_BT81X) && defined(VC_EMULATOR)
#error "copy res/font/eveflh.bin to res/eveflh, then comment this line"
#elif defined(DEF_BT81X) && (defined(VC_MPSSE) || defined(VC_FT4222))
#error "program res/font/eveflh.bin to eve-connected-flash, then comment this line"
#endif

#define FONT_1 6
#define FONT_2 7
#define FONT_3 8

#define FONT_2_W 60
#define FONT_2_H 70

#define LINE1_COLOR COLOR_RGB(0,0xFF,0)
#define LINE2_COLOR COLOR_RGB(0,0,0xFF)
#define LINE3_COLOR COLOR_RGB(0xFF,0xFF,0xFF)

#define FONT_LEN 10
#define FONT_LINE 3
FTU8 screen_title[FONT_LINE][FONT_LEN] = {
	{1,2,3,4,0},//ftdi
	{1,2,0},//we
	{3,4,5,6,7,8,0}//make design easy
}; 

#if defined(FILESYS_USED) 
#define PATH_FONT1 ROOT_PATH"font\\line1.raw"
#define PATH_FONT2 ROOT_PATH"font\\line2.raw"
#else
#define PATH_FONT1 ROOT_PATH"line1.raw"
#define PATH_FONT2 ROOT_PATH"line2.raw"
#endif
/* this demo's GLYPH file store in address 4096 of the eve-connected-flash
   should be the same address you input in EAB tools while generate xfont/glyph
   it's address would be saved in XFONT+0x20 offset
   the value would be "0x800000 | (address / 32)", e.g. "4096" would be "0x800080"
   if you put your GLYPH in other address than the prevous address input in EAB
   you may also follow above formula, change the data in XFONT+0x20 
   then EVE would be able to find it */
#define PATH_FONT3_XFONT "EVEFLH@279680:4672"

#define FONT_ADDR1 RAM_G
#define FONT_ADDR2 (21124)//the size of line1.raw

FTVOID font_main (FTU32 para)
{
#if defined(DEF_BT81X)
	static FT_Gpu_Fonts_t stFB2;

    /* instead of using index, coder may directly use 'unicode'
       in code while using BT81X, to display the customer font
       GLYPH: 
       1. the customer font 'image' resource
       2. can be stored in eve-connected-flash, or RAM_G of EVE
       XFONT:
       1. the header of each customer font 'image'
       2. has to be stored in RAM_G of EVE
       both GLYPH and XFONT are necessary when using 'unicode' font
       */
    appEVEFLHSetFull();
    if (appEVEFLHToEVE(appEVEFLHAddr(PATH_FONT3_XFONT), FONT_ADDR1, appEVEFLHLen(PATH_FONT3_XFONT))) {
        DBGPRINT;
        return;
    }
#else
	FT_Gpu_Fonts_t stFB1,stFB2;

    /* BT81X do not support file address smaller than 
       (FontLineStride*FontHeightInPixels - 148), 
       FONT_ADDR1 == 0, so skip this */
    if (0 == appFileToRamG(PATH_FONT1,FONT_ADDR1,0,(FTU8 *)&stFB1,sizeof(FT_Gpu_Fonts_t))) {
        DBGPRINT;
        return;
    }
#endif
    if (0 == appFileToRamG(PATH_FONT2,FONT_ADDR2,0,(FTU8 *)&stFB2,sizeof(FT_Gpu_Fonts_t))) {
        DBGPRINT;
        return;
    }

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));

	/* set the external font to each font index */
#if defined(DEF_BT81X)
	CoCmd_SETFONT2(FONT_3, FONT_ADDR1, 0);
#else
	CoCmd_SETFONT(FONT_1, FONT_ADDR1, &stFB1);
#endif
	CoCmd_SETFONT(FONT_2, FONT_ADDR2, &stFB2);

    /* show how to impliment customer font directly on screen */
#if defined(DEF_BT81X)
    /* 
       as long as your compiler can recognize UTF-8, you can do whatever you like while coding.
       for this demo, I below two things to make sure my platform compiler recognize it:
       1. make this file in UTF-8 format 
          (simplest way is open your file in binary editer, add "0xEF, 0xBB, 0xEF" at your file head)
       2. use 'u8' to indicate the compiler for the UFT-8 string
     */
	HAL_CmdBufIn(LINE1_COLOR);
	CoCmd_TEXT(0,0,FONT_3,0,u8"全新的");
	HAL_CmdBufIn(LINE2_COLOR);
	CoCmd_TEXT(EVE_LCD_WIDTH/2,0,FONT_3,OPT_CENTERX,u8"显示");
	HAL_CmdBufIn(LINE3_COLOR);
	CoCmd_TEXT(EVE_LCD_WIDTH,0,FONT_3,OPT_RIGHTX,u8"技术");
    /* after finish UTF-8 text display, in some cases, 
       add below command to avoid some corner cases */
	HAL_CmdBufIn(BITMAP_SWIZZLE(RED, GREEN, BLUE, ALPHA));
#else
	HAL_CmdBufIn(LINE1_COLOR);
	CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/4-FONT_2_H,FONT_1,OPT_CENTERX,(FTC8 *)&screen_title[0][0]);
#endif

	HAL_CmdBufIn(LINE2_COLOR);
	CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2-FONT_2_H/2,FONT_2,OPT_CENTER,(FTC8 *)&screen_title[1][0]);

    /* show how to impliment customer font on widget */
 	HAL_CmdBufIn(LINE3_COLOR);
	CoCmd_KEY(EVE_LCD_WIDTH/2-FONT_2_W/2,EVE_LCD_HIGH/2+FONT_2_H/2,FONT_2_W,FONT_2_H,FONT_2,OPT_CENTER,&screen_title[2][0]);

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);

    /* if showing a static screen
       from now on
       don't need to do anything */
	while(1);
}

AppFunc APPS_UI[] = {
	font_main
};

