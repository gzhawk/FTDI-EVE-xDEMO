/* 
    Sample code to show how to create test bitmap
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Jun
*/

#define TST_BMP_W    (800)
#define TST_BMP_H    (480)

FTVOID create_bitmap(FTVOID)
{
    FTU32 w,h,addr;
    /* create a black and white test background bitmap
       for LCD test*/
    for (h = 0,addr = RAM_G; h < TST_BMP_H; h++) {
        for (w = 0; w < TST_BMP_W; w += 2) {
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

    HAL_DlpBufIn(BITMAP_LAYOUT(RGB565,TST_BMP_W*2,TST_BMP_H));
    HAL_DlpBufIn(BITMAP_LAYOUT_H((TST_BMP_W*2)>>10,TST_BMP_H>>9));
    HAL_DlpBufIn(BITMAP_SIZE(NEAREST,BORDER,BORDER,TST_BMP_W,TST_BMP_H));
    HAL_DlpBufIn(BITMAP_SIZE_H(TST_BMP_W>>9,TST_BMP_H>>9));

    HAL_DlpBufIn(DISPLAY());
    HAL_BufToReg(RAM_DL,0);
}

FTVOID lcdtst (FTU32 para)
{
	/* never mind, it's for debug,
	 * this part just for this routine 
     * jump out the outside caller 
     * when error happen */
	appGP.appIndex = 1;
	appGP.appPara = 0;

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
	lcdtst,
	/* Leave this NULL at the buttom of this array */
	NULL
};

