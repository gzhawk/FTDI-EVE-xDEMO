/* 
    Sample code to show how to create test bitmap
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Jun
*/

FTVOID create_bitmap(FTVOID)
{
    FTU32 w,h,addr;
    /* create a black and white test background bitmap
       for LCD test*/
    for (h = 0,addr = RAM_G; h < EVE_LCD_HIGH; h++) {
        for (w = 0; w < EVE_LCD_WIDTH; w += 2) {
            /* use RGB565, 2byte/pixel */
            HAL_Write8(addr, 0xFF);
            HAL_Write8(addr+1, 0xFF);
            HAL_Write8(addr+2, 0);
            HAL_Write8(addr+3, 0);
            addr += 4;
        }
    }
    /* tell eve bitmap information */
    HAL_DlpBufIn(BITMAP_HANDLE(0));
    HAL_DlpBufIn(BITMAP_SOURCE(RAM_G));

    HAL_DlpBufIn(BITMAP_LAYOUT(RGB565,EVE_LCD_WIDTH*2,EVE_LCD_HIGH));
    HAL_DlpBufIn(BITMAP_LAYOUT_H((EVE_LCD_WIDTH*2)>>10,EVE_LCD_HIGH>>9));
    HAL_DlpBufIn(BITMAP_SIZE(NEAREST,BORDER,BORDER,EVE_LCD_WIDTH,EVE_LCD_HIGH));
    HAL_DlpBufIn(BITMAP_SIZE_H(EVE_LCD_WIDTH>>9,EVE_LCD_HIGH>>9));

    HAL_DlpBufIn(DISPLAY());
    HAL_BufToReg(RAM_DL,0);
}

FTVOID lcdtst (FTU32 para)
{
    create_bitmap();

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255,255,255));
	HAL_CmdBufIn(CLEAR(1,1,1));
	
    HAL_CmdBufIn(BEGIN(BITMAPS));
    HAL_CmdBufIn(BITMAP_HANDLE(0));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(VERTEX2F(0,0));
	HAL_CmdBufIn(END());
    
    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

    HAL_BufToReg(RAM_CMD,0);

    while (1);
}

AppFunc APPS_UI[] = {
	lcdtst
};

