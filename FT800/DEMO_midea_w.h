/* 
    Sample code to show how to impliment hardest part of Midea's demo
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2015/Sep
*/
#ifdef DEMO_MIDEA_W

typedef struct app_para_st {
	FTU32 appIndex;
	FTU32 appPara;
}app_para_t;

app_para_t appGP = {0};

#define FIFOSIZE        (18*1024)/* make sure this size larger than JPG file size */
#define FIFOADDR        (FT800_RAMG_SIZE - FIFOSIZE)

#define PATH_BUP ROOT_PATH"midea_w\\bup\\m (1).jpg"
#define PATH_BKGND ROOT_PATH"midea_w\\bkgnd\\b (1).jpg"
#define PATH_L ROOT_PATH"midea_w\\icon\\L.raw"
#define PATH_Lx ROOT_PATH"midea_w\\icon\\L\\L1.raw"
#define PATH_R ROOT_PATH"midea_w\\icon\\R.raw"

#define L_X 48
#define L_Y 123
#define Lx_X 59
#define Lx_Y 99
#define R_X 25
#define R_Y 83

#define R_SPEED 10
#define Lx_SPEED (2*R_SPEED)

enum ICON_HDL_ {
	HDL_L = 0,
	HDL_Lx,
	HDL_R,
	HDL_MAX
} ICON_HDL;

#if defined(FT9XXEV)
#define DISP_PATH_LEN   (30)
#else /*defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)*/
#define DISP_PATH_LEN   (50)
#endif

FTU8 path_bup[DISP_PATH_LEN] = PATH_BUP;
FTU8 path_bkgnd[DISP_PATH_LEN] = PATH_BKGND;
FTU8 path_icon_l[DISP_PATH_LEN] = PATH_Lx;


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

	/*
	 * Command Buffer: |---------------not used for JPEG file buffer--------------|
	 * RAMG          : |---***display area***---***media fifo JPEG file data***---|
	 */
	mfifoJpegWrite(fifoAddr,fifoSize,ramgAddr,opt,resHDL,Len);

	appResClose(resHDL);

	return Len;
}

FTINDEF FTU8 valueChg_bup (FTU8 *p)
{
#define BUP_END 399
	static FTU32 index = 1;

	sprintf((char *)p,ROOT_PATH"midea_w\\bup\\m (%d).jpg",++index);
	if (index == BUP_END) {
		/* stop the loop */
		index = 0;
		return 0;
	}

	return 1;
}

FTINDEF FTVOID valueChg_bkgnd (FTU8 *p)
{
#define BKGND_END 300
	static FTU32 index = 1;
	
	sprintf((char *)p,ROOT_PATH"midea_w\\bkgnd\\b (%d).jpg",++index);
	if (index == BKGND_END) {
		/* keep looping */
		index = 1;
	}
}

FTINDEF FTVOID valueChg_icon_l (FTU8 *p)
{
#define ICON_L_END 23
	static FTU32 index = 1;
	
	if (index == ICON_L_END) {
		/* keep looping */
		index = 1;
	} else if (index == 4) {//I make a wrong L4.raw file...
		index = 5;
	}
	sprintf((char *)p,ROOT_PATH"midea_w\\icon\\L\\L%d.raw",index);
	index++;
}

FTINDEF FTVOID JpegDisplay (FTU32 hdl, FTU32 addr, FTU32 X, FTU32 Y)
{
	HAL_CmdBufIn(BITMAP_HANDLE(hdl));
	HAL_CmdBufIn(BITMAP_SOURCE(addr));
	HAL_CmdBufIn(BITMAP_LAYOUT(RGB565, (FT800_LCD_WIDTH * 2), FT800_LCD_HIGH));
	HAL_CmdBufIn(BITMAP_LAYOUT_H((FT800_LCD_WIDTH * 2) >> 10, FT800_LCD_HIGH >> 9)); 
	HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, FT800_LCD_WIDTH, FT800_LCD_HIGH));
	HAL_CmdBufIn(BITMAP_SIZE_H(FT800_LCD_WIDTH>>9, FT800_LCD_HIGH>>9));
	HAL_CmdBufIn(BEGIN(BITMAPS));
	HAL_CmdBufIn(VERTEX2F(X,Y));
	HAL_CmdBufIn(END());
}

FTVOID bup (FTU32 para)
{
	FTU32 len;

	/* just for debug */
	appGP.appIndex = 2;
	appGP.appPara = 0;

	do {
	
		len = JPEGToRamG(path_bup,RAM_G,FIFOADDR,FIFOSIZE,OPT_MEDIAFIFO);

		if (len == 0) {
			DBGPRINT;
			return;
		}
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
		HAL_CmdBufIn(CLEAR(1,1,1));

		JpegDisplay(0, RAM_G, 0, 0);

		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
	} while (valueChg_bup(path_bup));

	appGP.appIndex = 1;
}

FTVOID main_m (FTU32 para)
{
#define SETDEFAULT HAL_CmdBufIn(BITMAP_TRANSFORM_A(256)); \
	HAL_CmdBufIn(BITMAP_TRANSFORM_B(0)); \
	HAL_CmdBufIn(BITMAP_TRANSFORM_C(0)); \
	HAL_CmdBufIn(BITMAP_TRANSFORM_D(0)); \
	HAL_CmdBufIn(BITMAP_TRANSFORM_E(256)); \
	HAL_CmdBufIn(BITMAP_TRANSFORM_F(0))

	bmpHDR_st bmp_header[HDL_MAX] = {
		{PATH_L,ARGB4,0,59,47},
		{PATH_Lx,ARGB4,0,36,30},
		{PATH_R,ARGB4,0,110,110}
	};
	FTU32 len;
	static FTU16 angle = 0, flag = 0;
	static FTU32 bkgnd_offset, icon_l_offset;

	/* just for debug */
	appGP.appIndex = 2;
	appGP.appPara = 0;

	if (flag == 0) {
		/* load bitmap resources data into FT800 */
		if(APP_OK != appBmpToRamG(0, RAM_G, bmp_header, 3)){
			DBGPRINT;
			return;
		}
		flag = 1;
		bkgnd_offset = RAM_G+bmp_header[0].len+bmp_header[1].len+bmp_header[2].len;
		icon_l_offset = RAM_G+bmp_header[0].len;
	}

	len = JPEGToRamG(path_bkgnd,bkgnd_offset,FIFOADDR,FIFOSIZE,OPT_MEDIAFIFO);
	if (len == 0) {
		DBGPRINT;
		return;
	}
	/* keep flashing BKGND */
	valueChg_bkgnd(path_bkgnd);

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));

	/* BKGND */
	JpegDisplay(HDL_MAX, bkgnd_offset, 0, 0);
	
	HAL_CmdBufIn(BEGIN(BITMAPS));

	/* L */
	HAL_CmdBufIn(BITMAP_HANDLE(HDL_L));
	HAL_CmdBufIn(VERTEX2F(L_X*FT800_PIXEL_UNIT,L_Y*FT800_PIXEL_UNIT));

	/* Lx */
	HAL_CmdBufIn(BITMAP_HANDLE(HDL_Lx));
	HAL_CmdBufIn(VERTEX2F(Lx_X*FT800_PIXEL_UNIT,Lx_Y*FT800_PIXEL_UNIT));

	/* rotate R */
	CoCmd_LOADIDENTITY;	      
	CoCmd_TRANSLATE(bmp_header[HDL_R].wide/2*FT800_TRANSFORM_MAX,bmp_header[HDL_R].high/2*FT800_TRANSFORM_MAX);
	CoCmd_ROTATE(angle*FT800_TRANSFORM_MAX/360);
	CoCmd_TRANSLATE(-1*bmp_header[HDL_R].wide/2*FT800_TRANSFORM_MAX,-1*bmp_header[HDL_R].high/2*FT800_TRANSFORM_MAX);
	CoCmd_SETMATRIX;
	HAL_CmdBufIn(BITMAP_HANDLE(HDL_R));
	HAL_CmdBufIn(VERTEX2F(R_X*FT800_PIXEL_UNIT,R_Y*FT800_PIXEL_UNIT));
	SETDEFAULT;

	HAL_CmdBufIn(END());
	
	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);

	/* keep rotating R */
	if (angle == 360) {
		angle = 0;
	} else {
		angle += R_SPEED;
	}

	/* keep flashing Lx */
	if (angle%Lx_SPEED == 0) {
		valueChg_icon_l(path_icon_l);
		appFileToRamG((FTC8 *)path_icon_l,icon_l_offset,0,0,0);
	}

	appGP.appIndex = 1;
}

AppFunc Apps[] = {
	bup,
	main_m,
	/* Leave this NULL at the buttom of this array */
	NULL
};

#endif

