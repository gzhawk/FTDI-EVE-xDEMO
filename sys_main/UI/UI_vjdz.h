/* 
    Sample code to VJDZ demo
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2017/Mar
*/

/* make sure FIFOSIZE larger than JPG file size
   and less than (1024-LCD_WIDTH*LCD_HIGH*2/1024) */
#define FIFOSIZE        (16*1024) /* the MAX file size + 1024 for safty buffer */
#define FIFOADDR        (FT800_RAMG_SIZE - FIFOSIZE)
#define PALINXSIZE      (376*1024)
#define PALLUTADDR_a    (FIFOADDR - 1024)
#define PALINXADDR_a    (PALLUTADDR_a - PALINXSIZE)
#define PALLUTADDR_b    (PALINXADDR_a - 1024)
#define PALINXADDR_b    (PALLUTADDR_b - PALINXSIZE)
#define LOGOADDR        (PALINXADDR_b - 493*128*2*1024)

#define LOGO_HDL_START  0
#define UI_HDL_START    1
#define UI_NUM          2
FTU32 ui_index = 0;

FTU8 PATH_LOGO[] = ROOT_PATH"vjdz\\logo00.jpg";

bmpHDR_st UI_LIST[UI_NUM] = {
    {ROOT_PATH"vjdz\\7_inx.bin", ROOT_PATH"vjdz\\7_lut.bin", PALLUTADDR_a,
     PALETTED565, PALINXSIZE, 1024, 800, 480},
    {ROOT_PATH"vjdz\\8_inx.bin", ROOT_PATH"vjdz\\8_lut.bin", PALLUTADDR_b,
     PALETTED565, PALINXSIZE, 1024, 800, 480}
};

STATIC FTU8 indexIncrease (FTVOID)
{
	FTU8 index_one = sizeof(PATH_LOGO)-6, index_ten = index_one - 1;

	if (PATH_LOGO[index_ten] == '3' && PATH_LOGO[index_one] == '5') {
		PATH_LOGO[index_one] = '0';
		PATH_LOGO[index_ten] = '0';

        return 1;
	} else if (PATH_LOGO[index_one] == '9') {
		PATH_LOGO[index_one] = '0';
		PATH_LOGO[index_ten] ++;
	} else {
		PATH_LOGO[index_one] ++;
	}

    return 0;
}

STATIC FTU32 mfifoImageWrite (FTU32 mfifo_addr, FTU32 mfifo_size,FTU32 disp_addr,FTU32 resHDL, FTU32 file_len)
{
	FTU32 mfifo_rd, mfifo_wr;

	/* mfifo size should not less than file len ,
	 * mfifo address must different with display area in RAMG */
	if ((mfifo_size < file_len) || 
		((disp_addr >= mfifo_addr) && 
		 (disp_addr <= (mfifo_addr + mfifo_size)))) {
		return 0;
	}

	/* set the MEDIAFIFO space in RAMG*/
	CoCmd_MEDIAFIFO(mfifo_addr, mfifo_size);

	HAL_BufToReg(RAM_CMD, 0);

	/* write data to RAM_G */
	appResToDes(resHDL, mfifo_addr, 0, file_len, resWrEve);

	HAL_Write32(REG_MEDIAFIFO_WRITE, file_len);

	CoCmd_LOADIMAGE(disp_addr,OPT_MEDIAFIFO);

	HAL_BufToReg(RAM_CMD, 0);

	/* wait till previous action finished */
	do {
		mfifo_rd = HAL_Read32(REG_MEDIAFIFO_READ); 
		mfifo_wr = HAL_Read32(REG_MEDIAFIFO_WRITE);
	} while (mfifo_rd != mfifo_wr);

	return file_len;
}

STATIC FTU32 ImageToRamG(FTU8 *path, FTU32 ramgAddr, FTU32 fifoAddr, FTU32 fifoSize)
{
	FTU32 resHDL, Len;

	resHDL = appResOpen(path);
	if (resHDL == 0) {
		DBGPRINT;
		return 0;
	}

	Len = appResSize(resHDL);
	if (FT800_RAMG_SIZE < ramgAddr + Len) {
		appResClose(resHDL);
		DBGPRINT;
		return 0;
	}

	/*
	  Command Buffer: |---------------not used for Image file buffer--------------|
	  RAMG          : |---***display area***---***media fifo JPEG file data***---|
	 */
	mfifoImageWrite(fifoAddr,fifoSize,ramgAddr,resHDL,Len);
		
	appResClose(resHDL);

	return Len;
}

STATIC FTVOID DisplayJPG (FTU32 hdl, FTU32 addr)
{
#define LOGO_X (153)
#define LOGO_Y (178)
	FTU32 len;

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));
	
    HAL_BufToReg(RAM_CMD,0);
	
    len = ImageToRamG(PATH_LOGO,RAM_G,FIFOADDR,FIFOSIZE);

    if (len == 0) {
        DBGPRINT;
        return;
    }

    HAL_CmdBufIn(BEGIN(BITMAPS));
	HAL_CmdBufIn(VERTEX2F(LOGO_X*FT800_PIXEL_UNIT,LOGO_Y*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(END());

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
}

STATIC FTU8 loadUI (FTVOID)
{
    if (APP_OK != appLoadBmp(PALINXADDR_b,UI_LIST,UI_NUM) ) {
        return 1;
    }

    /* set the bitmap global variable */
    appUI_FillBmpDL(UI_HDL_START, PALINXADDR_b, UI_LIST, UI_NUM);
    
    return 0;
}

STATIC FTVOID DisplayUI (FTU32 ui)
{
	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));
	
    HAL_CmdBufIn(BEGIN(BITMAPS));
    HAL_CmdBufIn(BITMAP_HANDLE(UI_HDL_START+ui));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(PALETTE_SOURCE(UI_LIST[ui].lut_src));
    HAL_CmdBufIn(VERTEX2F(0,0));
	HAL_CmdBufIn(END());

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
}

FTVOID logoDisp (FTU32 para)
{
    loadUI();

	do {
		DisplayJPG(LOGO_HDL_START, LOGOADDR);
        
        if(indexIncrease()){
            break;
        }
	} while (1);

    appGP.appPara  = (FTU32)&ui_index;
    appGP.appIndex = 1;
}

FTVOID uiDisp (FTU32 para)
{
    DisplayUI(*(FTU32 *)para);

    while (!TOUCHED);
    
    while (TOUCHED);

    if(*(FTU32 *)para) {
        *(FTU32 *)para = 0;
    } else {
        *(FTU32 *)para = 1;
    }

    appGP.appIndex = 1;
}

AppFunc APPS_UI[] = {
	logoDisp,
    uiDisp
};

