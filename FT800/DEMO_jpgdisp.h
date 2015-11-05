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
#define PATH_DISP ROOT_PATH"jpgdisp\\00.jpg"
#else
#define PATH_DISP ROOT_PATH"00.jpg"
#endif
#define FIFOSIZE        (60*1024)/* make sure this size larger than JPG file size */
#define FIFOADDR        (FT800_RAMG_SIZE - FIFOSIZE)

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(FT9XXEV)
#define DISP_PATH_LEN   (30)
#if defined(FT9XXEV)
#define DISP_PATH_INDEX (9)
#else
#define DISP_PATH_INDEX (19)
#endif
#else
#define DISP_PATH_LEN   (8+1+3+1)
#define DISP_PATH_INDEX (7)
#endif

#define FRAME_LAST_CHAR '9'
#define FRAME_SECON_CHAR '1'

typedef struct disp_path_ {
	FTU8 path_disp[DISP_PATH_LEN];
} disp_path_st;

disp_path_st vPath = {
	PATH_DISP,
};

FTINDEF FTU32 mfifoJpegWrite (FTU32 mfifo_addr, FTU32 mfifo_size,FTU32 disp_addr,FTU32 opt,FTU32 resHDL, FTU32 file_len)
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

FTINDEF FTVOID cmdbufJpegWrite (FTU32 disp_addr,FTU32 opt,FTU32 resHDL, FTU32 file_len)
{
#define CMD_BLOCK_LEN (CMDBUF_SIZE/2)
	FTU32 i = 0, l = 0;
	
	HAL_CmdToReg(CMD_LOADIMAGE);
	HAL_CmdToReg(disp_addr);
	HAL_CmdToReg(opt);

	for (i = 0; i < file_len; i += l) {
		l = ((file_len - i) > CMD_BLOCK_LEN)?CMD_BLOCK_LEN:(file_len - i);
		HAL_CmdWait(appResToDes(resHDL, HAL_Read32(REG_CMD_WRITE), i, l, resWrEveCmd));
	}
}

FTINDEF FTU32 JPEGToRamG(FTU8 *path, FTU32 ramgAddr, FTU32 fifoAddr, FTU32 fifoSize, FTU32 opt)
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
		 * Command Buffer: |---------------not used for JPEG file buffer--------------|
		 * RAMG          : |---***display area***---***media fifo JPEG file data***---|
		 */
		mfifoJpegWrite(fifoAddr,fifoSize,ramgAddr,opt,resHDL,Len);
	} else {
		/*
		 * Command Buffer: |--loop buffering JPEG file data---|
		 * RAMG          : |--------***display area***--------|
		 */
		cmdbufJpegWrite(ramgAddr,opt,resHDL,Len);
	}

	appResClose(resHDL);

	return Len;
}

FTINDEF FTVOID valueChg (disp_path_st *pst, FTU32 *opt)
{
	if (pst->path_disp[DISP_PATH_INDEX] == FRAME_LAST_CHAR) {
		if (pst->path_disp[DISP_PATH_INDEX-1] == FRAME_SECON_CHAR) {
			pst->path_disp[DISP_PATH_INDEX] = '0';
			pst->path_disp[DISP_PATH_INDEX-1] = '0';
			*opt = (*opt&OPT_MEDIAFIFO)?0:OPT_MEDIAFIFO;
		} else {
			pst->path_disp[DISP_PATH_INDEX-1]++;
			pst->path_disp[DISP_PATH_INDEX] = '0';
		}
	} else {
		pst->path_disp[DISP_PATH_INDEX]++;
	}
}

FTINDEF FTVOID Display (FTU32 hdl, FTU32 addr, FTU32 opt)
{
	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
	HAL_CmdBufIn(CLEAR(1,1,1));

	HAL_CmdBufIn(BITMAP_HANDLE(hdl));
	HAL_CmdBufIn(BITMAP_SOURCE(addr));
	HAL_CmdBufIn(BITMAP_LAYOUT(RGB565, (FT800_LCD_WIDTH * 2), FT800_LCD_HIGH));
	HAL_CmdBufIn(BITMAP_LAYOUT_H((FT800_LCD_WIDTH * 2) >> 10, FT800_LCD_HIGH >> 9)); 
	HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, FT800_LCD_WIDTH, FT800_LCD_HIGH));
	HAL_CmdBufIn(BITMAP_SIZE_H(FT800_LCD_WIDTH>>9, FT800_LCD_HIGH>>9));
	HAL_CmdBufIn(BEGIN(BITMAPS));
	HAL_CmdBufIn(VERTEX2F(0,0));
	HAL_CmdBufIn(END());

	if (OPT_MEDIAFIFO&opt) {
		HAL_CmdBufIn(COLOR_RGB(0,0xFF,0));
		CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,25,OPT_CENTERX,"show JPEG using MEDIAFIFO");
	} else {
		HAL_CmdBufIn(COLOR_RGB(0xFF,0xFF,0xFF));
		CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,25,OPT_CENTERX,"show JPEG using CMD buffer");
	}

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
}

FTVOID jpgdisp (FTU32 para)
{
	FTU32 len, opt = OPT_MEDIAFIFO;
	disp_path_st *path = &vPath;

	/* just for debug */
	appGP.appIndex = 1;
	appGP.appPara = 0;
#if defined(STM32F4)
	return;
#endif	
	do {
	
		if (OPT_MEDIAFIFO&opt) {
			len = JPEGToRamG(path->path_disp,RAM_G,FIFOADDR,FIFOSIZE,opt);
		} else {
			len = JPEGToRamG(path->path_disp,RAM_G,0,0,0);
		}

		if (len == 0) {
			DBGPRINT;
			return;
		}

		Display(0, RAM_G, opt);

		valueChg(path, &opt);
	} while (1);
}

AppFunc Apps[] = {
	jpgdisp,
	/* Leave this NULL at the buttom of this array */
	NULL
};

#endif

