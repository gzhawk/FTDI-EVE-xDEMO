/* 
    Sample code to show how to continous display JPG directly
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2015/May
*/
#ifdef DEMO_JPGDISP

typedef struct app_para_st {
	FTU32 appIndex;
	FTU32 appPara;
}app_para_t;

app_para_t appGP = {0};

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(FT9XXEV)
#define PATH_JPG_A ROOT_PATH"jpgdisp\\a.jpg"
#define PATH_JPG_B ROOT_PATH"jpgdisp\\b.jpg" /* higher compressed with smoothing effect */
#define PATH_PNG_A ROOT_PATH"jpgdisp\\a.png"
#define PATH_PNG_B ROOT_PATH"jpgdisp\\b.png" /* paletted PNG with dithering effect */
#else
#define PATH_JPG_A ROOT_PATH"a.jpg"
#define PATH_JPG_B ROOT_PATH"b.jpg"
#define PATH_PNG_A ROOT_PATH"a.png"
#define PATH_PNG_B ROOT_PATH"b.png"
#endif

/* make sure FIFOSIZE larger than JPG file size
   and less than (1024-LCD_WIDTH*LCD_HIGH*2/1024) */
#define FIFOSIZE        (200*1024)
#define FIFOADDR        (FT800_RAMG_SIZE - FIFOSIZE)

FTVOID PressAndRelease (FTVOID)
{
    while (!TOUCHED); //wait for press
    while (TOUCHED) {
        HAL_CmdBufIn(CMD_DLSTART);
        HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
        HAL_CmdBufIn(CLEAR(1,1,1));

        HAL_CmdBufIn(COLOR_RGB(0,0xFF,0));
        CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,25,OPT_CENTERX,"Next...");

        HAL_CmdBufIn(DISPLAY());
        HAL_CmdBufIn(CMD_SWAP);
        HAL_BufToReg(RAM_CMD,0);
    };  //wait for release
}

FTINDEF FTU32 mfifoImageWrite (FTU32 mfifo_addr, FTU32 mfifo_size,FTU32 disp_addr,FTU32 opt,FTU32 resHDL, FTU32 file_len)
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

	CoCmd_LOADIMAGE(disp_addr,opt);

	HAL_BufToReg(RAM_CMD, 0);

	/* wait till previous action finished */
	do {
		mfifo_rd = HAL_Read32(REG_MEDIAFIFO_READ); 
		mfifo_wr = HAL_Read32(REG_MEDIAFIFO_WRITE);
	} while (mfifo_rd != mfifo_wr);

	return file_len;
}

FTINDEF FTVOID cmdbufImageWrite (FTU32 disp_addr,FTU32 opt,FTU32 resHDL, FTU32 file_len)
{
    HAL_CmdToReg(CMD_LOADIMAGE);
	HAL_CmdToReg(disp_addr);
	HAL_CmdToReg(opt);

	appResToDes(resHDL, 0, 0, file_len, resWrEveCmd);
}

FTINDEF FTU32 ImageToRamG(FTU8 *path, FTU32 ramgAddr, FTU32 fifoAddr, FTU32 fifoSize, FTU32 opt)
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

	if (OPT_MEDIAFIFO & opt) {
		/*
		 * Command Buffer: |---------------not used for Image file buffer--------------|
		 * RAMG          : |---***display area***---***media fifo JPEG file data***---|
		 */
		mfifoImageWrite(fifoAddr,fifoSize,ramgAddr,opt,resHDL,Len);
	} else {
		/*
		 * Command Buffer: |--loop buffering Image file data---|
		 * RAMG          : |--------***display area***--------|
		 */
		cmdbufImageWrite(ramgAddr,opt,resHDL,Len);
	}

	appResClose(resHDL);

	return Len;
}

FTINDEF FTVOID DisplayJPG (FTU32 hdl, FTU32 addr, FTU32 opt)
{
	FTU32 len;

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));
	
    HAL_BufToReg(RAM_CMD,0);
	
#if 0
/* 
    by default, EVE display the JPEG/PNG file based on it's own format,
    see CMD_LOADIMAGE in detail for default format on different JPEG/PNG, 
    but you can also force make it display in other format.
*/
	HAL_CmdBufIn(BITMAP_HANDLE(hdl));
	HAL_CmdBufIn(BITMAP_SOURCE(addr));
    HAL_CmdBufIn(BITMAP_LAYOUT(RGB565, (PIC_W * 2), PIC_H));
	HAL_CmdBufIn(BITMAP_LAYOUT_H((PIC_W * 2) >> 10, PIC_H >> 9)); 
	HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, PIC_W, PIC_H));
	HAL_CmdBufIn(BITMAP_SIZE_H(PIC_W>>9, PIC_H>>9));
#endif

    if (OPT_MEDIAFIFO&opt) {
        len = ImageToRamG((FTU8 *)PATH_JPG_A,RAM_G,FIFOADDR,FIFOSIZE,opt);
    } else {
        len = ImageToRamG((FTU8 *)PATH_JPG_A,RAM_G,0,0,0);
    }

    if (len == 0) {
        appGP.appIndex = 2;
        DBGPRINT;
        return;
    }

    HAL_CmdBufIn(BEGIN(BITMAPS));
	HAL_CmdBufIn(VERTEX2F(0,0));
	HAL_CmdBufIn(END());

	if (OPT_MEDIAFIFO&opt) {
		HAL_CmdBufIn(COLOR_RGB(0,0xFF,0));
		CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,25,OPT_CENTERX,"show JPEG using MEDIAFIFO");
	} else {
		HAL_CmdBufIn(COLOR_RGB(0,0xFF,0));
		CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,25,OPT_CENTERX,"show JPEG using CMD buffer");
	}

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
}

FTVOID pngdisp (FTU32 para)
{
	FTU32 len;
	
    HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255,255,255));
	HAL_CmdBufIn(CLEAR(1,1,1));
	
    HAL_BufToReg(RAM_CMD,0);
   
    len = ImageToRamG((FTU8 *)PATH_PNG_A,RAM_G,0,0,0);
    if (len == 0) {
        DBGPRINT;
        appGP.appIndex = 2;
        return;
    }

#if 0
/* 
    by default, EVE display the JPEG/PNG file based on it's own format,
    see CMD_LOADIMAGE in detail for default format on different JPEG/PNG, 
    but you can also force make it display in other format.
*/
    /* use PATH_PNG_A if you want a none PALETTED other format display */
    HAL_CmdBufIn(BITMAP_LAYOUT(ARGB1555, PIC_W*2, PIC_H));
	HAL_CmdBufIn(BITMAP_LAYOUT_H((PIC_W*2) >> 10, PIC_H >> 9)); 

    /* use PATH_PNG_B if you want a PALETTED other format display */
    HAL_CmdBufIn(BITMAP_LAYOUT(PALETTED565, PIC_W, PIC_H));
	HAL_CmdBufIn(BITMAP_LAYOUT_H(PIC_W >> 10, PIC_H >> 9)); 
#endif

	HAL_CmdBufIn(BEGIN(BITMAPS));
	HAL_CmdBufIn(VERTEX2F(0,0));
	HAL_CmdBufIn(END());

    HAL_CmdBufIn(COLOR_RGB(0,0xFF,0));
    CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,25,OPT_CENTERX,"show PNG using CMD buffer");

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);

    PressAndRelease();
	
    appGP.appIndex = 0;
}

FTVOID jpgdisp (FTU32 para)
{
	FTU32 opt = OPT_MEDIAFIFO;

	do {
		DisplayJPG(0, RAM_G, opt);
        
        PressAndRelease();
       
        if (OPT_MEDIAFIFO & opt) {
            opt = 0;
        } else {
			break;
        }
	} while (1);
	
    appGP.appIndex = 1;
}

AppFunc Apps[] = {
	jpgdisp,
    pngdisp,
	/* Leave this NULL at the buttom of this array */
	NULL
};

#endif

