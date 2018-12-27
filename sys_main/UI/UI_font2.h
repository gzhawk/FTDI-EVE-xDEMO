/* 
    Demo for teaching how to use 'Extended Font'
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Dec
*/

#if defined(DEF_BT81X) && defined(VC_EMULATOR)
#error "copy res/font2/eveflh.bin to res/eveflh, then comment this line"
#elif defined(DEF_BT81X) && (defined(VC_MPSSE) || defined(VC_FT4222))
#error "program res/font2/eveflh.bin to eve-connected-flash, then comment this line"
#endif

/* look this file as a input text string file */
#define TXT_FILE ROOT_PATH"font2\\font2.txt"
/* 
   this demo's GLYPH file store in address 4096 of the eve-connected-flash
   should be the same address you input in EAB tools while generate xfont/glyph
   it's address would be saved in XFONT+0x20 offset
   the value would be "0x800000 | (address / 32)", e.g. "4096" would be "0x800080"

   if you put your GLYPH in other address than the prevous address input in EAB
   you may also follow above formula, change the data in XFONT+0x20 
   then EVE would be able to find it 

   during generate font by using EAB,
   some convert set may not bring good display effect, for example:
   simfang.ttf+ASTC10x5+FontSize28 will display abnormal
   while below set would bring good display
   simfnag.ttf+ASTC10x5+FontSize32 will display normal

   open eveflh.map to see the address of each file in flash
*/
#define FONT_XFONT "EVEFLH@9530944:4544" 
#define FONT_ADDR RAM_G
#define FONT_NUM 6
#define FONT_W   40
#define FONT_H   40
/*ONE UFT8 char == THREE bytes*/
#define UTF8_LEN 3
#define DISP_LEN    (EVE_LCD_WIDTH/FONT_W)
#define LINE1_COLOR COLOR_RGB(0xFF,   0,   0)
#define LINE2_COLOR COLOR_RGB(0   ,0xFF,   0)
#define LINE3_COLOR COLOR_RGB(   0,   0,0xFF)
/*extra 1 for '\0'*/
FTU8 line1[DISP_LEN*UTF8_LEN+1] = {0};
FTU8 line2[DISP_LEN*UTF8_LEN+1] = {0}; 
FTU8 line3[DISP_LEN*UTF8_LEN+1] = {0}; 

FTVOID font2_main (FTU32 para)
{
    static FTU8 load = 1;
    /* 
       GLYPH: 
       1. the customer font 'bitmap' resource
       2. can be stored in eve-connected-flash, or RAM_G of EVE
       
       XFONT:
       1. the Extended Font Metrics Block, for EVE to retrieve font 'bitmap' from GLYPH file
       2. has to be stored in RAM_G of EVE
       
       both GLYPH and XFONT are necessary when using 'unicode' font
       while customer font character liberary increase, 
       GLYPH file increase, while XFONT file not increase that much
       so better store GLYPH in eve-connected-flash.
     */
    if (load) {
        appEVEFLHSetFull();
        if (appEVEFLHToEVE(appEVEFLHAddr(FONT_XFONT), FONT_ADDR, appEVEFLHLen(FONT_XFONT))) {
            DBGPRINT;
            return;
        }
        load = 0;
    }

    /*keep pumping test data as customer random input for display*/
	if (!UTF8_Pump(TXT_FILE,line1,DISP_LEN) || 
        !UTF8_Pump(TXT_FILE,line2,DISP_LEN) ||
        !UTF8_Pump(TXT_FILE,line3,DISP_LEN)) {
        DBGPRINT;
    };

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));

	/* set the external font to each font index */
	CoCmd_SETFONT2(FONT_NUM, FONT_ADDR, 0);

    /* show how to impliment customer font directly on screen */
	HAL_CmdBufIn(LINE1_COLOR);
	CoCmd_TEXT(0,0,FONT_NUM,0,line1);
    HAL_CmdBufIn(LINE2_COLOR);
	CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2,FONT_NUM,OPT_CENTERX,line2);
    HAL_CmdBufIn(LINE3_COLOR);
	CoCmd_TEXT(0,EVE_LCD_HIGH-FONT_H,FONT_NUM,0,line3);
    
    /* after finish UTF-8 text display, in some cases, 
       add below command to avoid some corner cases */
	HAL_CmdBufIn(BITMAP_SWIZZLE(RED, GREEN, BLUE, ALPHA));

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);

    FTDELAY(500);
	appGP.appIndex = 0;
}

AppFunc APPS_UI[] = {
	font2_main
};

