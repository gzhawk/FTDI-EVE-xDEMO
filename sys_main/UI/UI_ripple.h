/* 
    Demo to simulate water ripple:
    Author: Hawk Gao
	Email : hawk.gao@ftdichip.com
	Date  : 2015/Jul
*/

#define PATH_DISP ROOT_PATH"ripple\\X1.jpg"

#define FIFOSIZE        (100*1024)/* make sure this size larger than all JPG file size */
#define FIFOADDR        (EVE_RAMG_SIZE - FIFOSIZE)
#define WAV_BUF_LEN     (EVE_LCD_WIDTH*EVE_LCD_HIGH+EVE_LCD_WIDTH)

/* 1pixel=RGB565=5+6+5=16=2bytes=FTU16 */
FTU8 wav_buf1[WAV_BUF_LEN] = {0};
FTU8 wav_buf2[WAV_BUF_LEN] = {0};
FTU8 img_buf[EVE_LCD_WIDTH*EVE_LCD_HIGH*2] = {0};

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

STATIC FTVOID Display (FTU32 addr)
{
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

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
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

FTVOID waveCalculate (FTU8 **pWav1, FTU8 **pWav2)
{
	FTU32 x;
	FTU8 *p,*p1=*pWav1,*p2=*pWav2;
	
	for (x = EVE_LCD_WIDTH; x < EVE_LCD_HIGH*(EVE_LCD_WIDTH-1); x++) {
		p2[x] = ((p1[x-EVE_LCD_WIDTH]+
				  p1[x+EVE_LCD_WIDTH]+
				  p1[x-1]+
				  p1[x+1])/2 - p2[x]);
		p2[x] -= p2[x]>>3;
	}
	
	p = p2;
	*pWav2 = p1;
	*pWav1 = p;
}

FTVOID pixelRender (FTU32 addr, FTU8 *pnextWav)
{
#define RENDER_SEED 0xFFF
	FT32 x,y,a,b,i=0;
	FTU16 tmp = RENDER_SEED;
	static FTU8 flag = 1;

	if (flag) {
		for (x = 0; x < EVE_LCD_WIDTH*EVE_LCD_HIGH*2; x++) {
			img_buf[x] = HAL_Read8(addr+x);
		}
		flag = 0;
	}

	for (y = 0; y < (EVE_LCD_HIGH); y++) {
		for (x = 0; x < EVE_LCD_WIDTH; x++) {
			tmp = RENDER_SEED - pnextWav[i];
			a = (x-EVE_LCD_WIDTH/2)*tmp/RENDER_SEED +EVE_LCD_WIDTH/2;
			b = (y-EVE_LCD_HIGH/2)*tmp/RENDER_SEED +EVE_LCD_HIGH/2;
		
			if (a >= EVE_LCD_WIDTH) {
				a = EVE_LCD_WIDTH - 1;
			} else if (a < 0) {
				a = 0;
			}

			if (b >= EVE_LCD_HIGH) {
				b = EVE_LCD_HIGH - 1;
			} else if (b < 0) {
				b = 0;
			}

			a += b*EVE_LCD_WIDTH;
			HAL_Write8(addr+i*2, img_buf[a*2]);
			HAL_Write8(addr+i*2+1, img_buf[a*2+1]);
			i++;
		}
	}
}

FTVOID disturb (FTU8* pcurrWav, FT32 X, FT32 Y, FTU32 size, FTU32 weight)
{
	FT32 x,y;

	if ((X >= EVE_LCD_WIDTH - size) ||
		(X < size) ||
		(Y >= EVE_LCD_HIGH - size) ||
		(Y < size)) {
		return;
	}

	for (x = X - size; x < X + size; x++) {
		for (y = Y - size; y < Y + size; y++) {
			if ((x-X)*(x-X) + (y-Y)*(y-Y) < size*size) {
				pcurrWav[EVE_LCD_WIDTH*y+x] += weight;
			}
		}
	}
}


FTVOID ripple (FTU32 para)
{
	FTU32 len, XY;
	FTU8 *pCurr = wav_buf1, *pNext = wav_buf2;

	/* X1.jpg */
	len = fileload((FTU8 *)PATH_DISP, FIFOADDR);
	if (len == 0) {
		DBGPRINT;
		return;
	}
		
	mfifoJpegWrite(FIFOADDR, FIFOSIZE, RAM_G, len);

	Display(RAM_G);
	
	do {
		if (TOUCHED) {
			while (TOUCHED) {
				XY = HAL_Read32(REG_CTOUCH_TOUCH0_XY);
				/*keep waiting for the release*/
			}
			disturb(pCurr,(XY >> 16)&0x7FFF, XY&0x7FFF,10,32);
		} 

		waveCalculate(&pCurr, &pNext);

		pixelRender(RAM_G, pCurr);
	} while (1);
}

AppFunc APPS_UI[] = {
	ripple
};

