/* 
    Sample code to show simple demo for littleswan
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2015/July
*/

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
#define PATH_DISP ROOT_PATH"littleswan\\X1.jpg"
#define P_LEN 30
#define P_INX 22
#else
#define PATH_DISP ROOT_PATH"X1.jpg"
#define P_LEN 8
#define P_INX 1
#endif
#define FIFOSIZE        204800UL//(200*1024)/* make sure this size larger than all JPG file size */
#define FIFOADDR        843776UL//(EVE_RAMG_SIZE - FIFOSIZE)
#define FIFOGAP         102400UL//(100*1024)/* make sure this size larger than single JPG file size */
#define P_BEN '1'

FTU8 p_tmp[P_LEN] = {PATH_DISP};

STATIC FTU32 mfifoJpegWrite (FTU32 mfifo_addr, FTU32 mfifo_size,FTU32 disp_addr,FTU32 file_len)
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

STATIC FTVOID Display (FTU32 hdl, FTU32 addr, FTU32 X, FTU32 Y)
{
#define POINT_SIZE_MIN 20
#define POINT_SIZE_MAX 80
	static FTU32 size = POINT_SIZE_MIN,
		   sX = 0, sY = 0;

	if (sX != X || sY != Y) {
		sX = X;
		sY = Y;
		size = POINT_SIZE_MIN;
	}

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
	HAL_CmdBufIn(CLEAR(1,1,1));

	HAL_CmdBufIn(BITMAP_HANDLE(0));
	HAL_CmdBufIn(BITMAP_SOURCE(addr));
	HAL_CmdBufIn(BITMAP_LAYOUT(RGB565, (EVE_LCD_WIDTH * 2), EVE_LCD_HIGH));
	HAL_CmdBufIn(BITMAP_LAYOUT_H((EVE_LCD_WIDTH * 2) >> 10, EVE_LCD_HIGH >> 9)); 
	HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, EVE_LCD_WIDTH, EVE_LCD_HIGH));
	HAL_CmdBufIn(BITMAP_SIZE_H(EVE_LCD_WIDTH>>9, EVE_LCD_HIGH>>9));
	HAL_CmdBufIn(BEGIN(BITMAPS));
	HAL_CmdBufIn(VERTEX2F(0,0));
	HAL_CmdBufIn(END());
	
	if (hdl != 0) {
		HAL_CmdBufIn(COLOR_RGB(0xFF,0,0));
		CoCmd_ROMFONT(hdl, 34);
		CoCmd_TEXT(EVE_LCD_WIDTH/2, EVE_LCD_HIGH/2, hdl, OPT_CENTER, "--END--");
	}

	if (X != 0 && Y != 0) {
		HAL_CmdBufIn(COLOR_RGB(0xFF,0xFF,0xFF));
		HAL_CmdBufIn(POINT_SIZE(size*16));
		HAL_CmdBufIn(COLOR_A(128));
		HAL_CmdBufIn(BEGIN(FTPOINTS));
		HAL_CmdBufIn(VERTEX2F(X*16,Y*16));
		HAL_CmdBufIn(END());
	}

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);

	size = (size >= POINT_SIZE_MAX)?POINT_SIZE_MIN:(size + 20);
}

FTU32 fileload (FTU8 * p_file, FTU32 mfifo_addr)
{
	FTU32 resHDL, len;

	resHDL = appResOpen(p_file);
	if (resHDL == 0) {
		DBGPRINT;
		return 0;
	}

	len = appResSize(resHDL);
	if (len == 0) {
		appResClose(resHDL);
		DBGPRINT;
		return 0;
	}

	appResToDes(resHDL, mfifo_addr, 0, len, resWrEve);
		
	appResClose(resHDL);

	return len;
}

FTVOID jpgdisp (FTU32 para)
{
	FTU32 len, XY, mfifo_addr = FIFOADDR;
#if !defined(STM32F4)&&!defined(MSVC2010EXPRESS)&&!defined(MSVC2012EMU)&&!defined(FT9XXEV)
	FTU32 lenX, lenY;
#endif
    static FTU8 inited = 0;

    if (!inited) {
        /* X1.jpg */
        len = fileload(p_tmp, FIFOADDR);
        if (len == 0) {
            DBGPRINT;
            return;
        }
        /*Arduino*/
#if !defined(STM32F4)&&!defined(MSVC2010EXPRESS)&&!defined(MSVC2012EMU)&&!defined(FT9XXEV)
        /* X2.jpg */
        p_tmp[P_INX]++;
        lenX = len;
        lenY = fileload(p_tmp, FIFOADDR+FIFOGAP);
        if (lenX == 0) {
            DBGPRINT;
            return;
        }
#endif
	    appGP.appIndex = 0;
	    appGP.appPara = 0xFF;
        inited = 1;
        return;
    }

    mfifoJpegWrite(mfifo_addr, FIFOGAP, RAM_G, len);

    if (TOUCHED) {
        while (TOUCHED) {
            XY = HAL_Read32(REG_CTOUCH_TOUCH0_XY);
            /*keep waiting for the release*/
            Display(0, RAM_G, (XY >> 16)&0x7FFF, XY&0x7FFF);
            FTDELAY(100);
        }

        if (FIFOADDR == mfifo_addr) {
            mfifo_addr += FIFOGAP;
#if !defined(STM32F4)&&!defined(MSVC2010EXPRESS)&&!defined(MSVC2012EMU)&&!defined(FT9XXEV)
            len = lenY;
#endif
            /* do the IO control */
            appGP.appPara = 0;
        } else {
            mfifo_addr = FIFOADDR;
#if !defined(STM32F4)&&!defined(MSVC2010EXPRESS)&&!defined(MSVC2012EMU)&&!defined(FT9XXEV)
            len = lenX;
#endif
            /* do the IO control */
            appGP.appPara = 1;
        }
#if defined(STM32F4) || defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(FT9XXEV)
        p_tmp[P_INX]++;
        len = fileload(p_tmp, mfifo_addr);
        if (len == 0) {
            Display(1, RAM_G, 0, 0);
            p_tmp[P_INX] = P_BEN;
            len = fileload(p_tmp, mfifo_addr);
            if (len == 0) {
                DBGPRINT;
                return;
            }
        }
#endif
    } else {
        Display(0, RAM_G, 0, 0);
    }

    appGP.appIndex = 0;
}

AppFunc APPS_UI[] = {
	jpgdisp
};

