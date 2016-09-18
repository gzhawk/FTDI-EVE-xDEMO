/* 
    Sample code to show how to display continous JPEG in many windows
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Apr
    when MCU support QSPI
    set the EVE_SPI_TYPE to 4 for better performance 
*/

FTU8 jpeg_a[] = ROOT_PATH"njtoyo\\A00.jpg";
FTU8 jpeg_b[] = ROOT_PATH"njtoyo\\B00.jpg";
FTU8 jpeg_c[] = ROOT_PATH"njtoyo\\C00.jpg";
FTU8 jpeg_d[] = ROOT_PATH"njtoyo\\D00.jpg";

/* make sure FIFOSIZE larger than JPG file size
   and less than (1024-LCD_WIDTH*LCD_HIGH*2/1024) */
#define FIFOSIZE        (25*1024)
/* Based on my current understanding, EVE JPEG decode
   to RGB565 for display, so file len in RAM_G
   should be wide * high * 2 */
#define BMPSIZE         (320*240*2)
/* leave a gap between 
   fifo address and bitmap address
   so far I don't know why
   use larger than 1 makes it work
   I use 4 for better align*/
#define BUF_GAP          (4)

#define FIFOADDR         (RAM_G)
#define BMPADDR          (FIFOADDR+FIFOSIZE+BUF_GAP)

#define EVE_C_ADDR       (BMPADDR+4*BMPSIZE)
#define EVE_C_MAXSIZE    (FT800_RAMG_SIZE-EVE_C_ADDR)

#define FPS_OFFSET_Y     0
#define FPS_OFFSET_X     80

#define DESCRIPTION_H    20

bmpHDR_st jpeg_bmp[4] = {
    {(FTC8 *)jpeg_a,0,0,RGB565,BMPSIZE,0,320,240},
    {(FTC8 *)jpeg_b,0,0,RGB565,BMPSIZE,0,320,240},
    {(FTC8 *)jpeg_c,0,0,RGB565,BMPSIZE,0,320,240},
    {(FTC8 *)jpeg_d,0,0,RGB565,BMPSIZE,0,320,240}
};

typedef struct jpeg_positon_ {
    FTU32 X;
    FTU32 Y;
}jpeg_positon_st;

typedef struct jpeg_play_ {
    FTU8  play;
    jpeg_positon_st pic;
    jpeg_positon_st txt;
}jpeg_play_st;

jpeg_play_st jpeg_play_list[4] = {
    {1, {0,        0        },{320/2,        240/2      }},
    {1, {(800-320),0        },{800 - (320/2),240/2      }},
    {1, {0,        (480-240)},{320/2,        480 - 240/2}},
    {1, {(800-320),(480-240)},{800 - (320/2),480 - 240/2}}
};

#define TAG_START  (10)
FTU8 frame_rate[4] = {0};

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
#define FILE_INDEX 18
#elif defined(FT9XXEV)
#define FILE_INDEX 8
#else
#error "not for this platform"
#endif

/*
 * detect press, mark it
 * then leave it unchange
 * until it release
 * then do the change
 */
FTVOID pressTHENrelease (FTVOID)
{
    static FTU8 pre_t = 0;
    FTU32 reg_tag = 0;

	if (TOUCHED) {
		reg_tag = HAL_Read32(REG_TOUCH_TAG);
		if (TAG_START <= reg_tag && TAG_START+4 >= reg_tag) {
			if (pre_t == reg_tag) {
				return;
			}
			pre_t = reg_tag;
		}
	} else if (TAG_START <= pre_t && TAG_START+4 >= pre_t) {
        jpeg_play_list[pre_t - TAG_START].play = 
			jpeg_play_list[pre_t - TAG_START].play?0:1;
        pre_t = 0;
    } 
}
void timerISR(void)
{
    static FTU8 pre_mark[4] = {0};
    FTU8 i,j;
#if defined(FT9XXEV)

    if (timer_is_interrupted(timer_select_a) == 1)
    {
        for (i = 0; i < 4; i++) {
            j = (*(FTU8 *)(jpeg_bmp[i].path + FILE_INDEX) - '0')*10;
            j += (*(FTU8 *)(jpeg_bmp[i].path + FILE_INDEX + 1) - '0')%10;
            if (pre_mark[i] <= j) {
                frame_rate[i] = j - pre_mark[i];
            } else {
                frame_rate[i] = 79 - pre_mark[i] + j;
            }
            pre_mark[i] = j;
        }
    }
#else
    static FTU64 oldTick = 0;
    FTU64 newTick = GetTickCount();

    if (newTick >= oldTick) {
        if (newTick - oldTick > 1000) {
			oldTick = (newTick - oldTick)/1000;
            for (i = 0; i < 4; i++) {
                j = (*(FTU8 *)(jpeg_bmp[i].path + FILE_INDEX) - '0')*10;
                j += (*(FTU8 *)(jpeg_bmp[i].path + FILE_INDEX + 1) - '0')%10;
                if (pre_mark[i] <= j) {
                    frame_rate[i] = (j - pre_mark[i])/(FTU8)oldTick;
                } else {
                    frame_rate[i] = (79 - pre_mark[i] + j)/(FTU8)oldTick;
                }
                pre_mark[i] = j;
            }
            oldTick = newTick;
            return;
        }
    } else {
        if (newTick + (0xFFFFFFFFFFFFFFFF - oldTick) > 1000) {
			oldTick = (newTick + (0xFFFFFFFFFFFFFFFF - oldTick))/1000;
            for (i = 0; i < 4; i++) {
                j = (*(FTU8 *)(jpeg_bmp[i].path + FILE_INDEX) - '0')*10;
                j += (*(FTU8 *)(jpeg_bmp[i].path + FILE_INDEX + 1) - '0')%10;
                if (pre_mark[i] <= j) {
                    frame_rate[i] = (j - pre_mark[i])/(FTU8)oldTick;
                } else {
                    frame_rate[i] = (79 - pre_mark[i] + j)/(FTU8)oldTick;
                }
                pre_mark[i] = j;
            }
            oldTick = newTick;
            return;
        }
    }
#endif
}
STATIC FTU32 mfifoImageWrite (FTU32 mfifo_addr, FTU32 mfifo_size,
                               FTU32 disp_addr,FTU32 opt,FTU32 resHDL, 
                               FTU32 file_len)
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

STATIC FTU32 ImageToRamG(FTU8 *path, FTU32 ramgAddr, 
                          FTU32 fifoAddr, FTU32 fifoSize)
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

    mfifoImageWrite(fifoAddr,fifoSize,ramgAddr,OPT_MEDIAFIFO,resHDL,Len);

	appResClose(resHDL);

	return Len;
}

FTVOID change4file (FTU32 para)
{
    static FTU8 frame[4] = {0};
    FTU8 i;

    appGP.appIndex = 0;

    for (i = 0; i < 4; i++) {
        if (jpeg_play_list[i].play) {
            if (frame[i] >= 79) {
                frame[i] = 0;
            } else {
                frame[i]++;
            }
            *(FTU8 *)(jpeg_bmp[i].path + FILE_INDEX) = '0' + frame[i]/10;
            *(FTU8 *)(jpeg_bmp[i].path + FILE_INDEX + 1) = '0' + frame[i]%10;
        }
    }
}

FTU8 dump4jpeg (FTVOID)
{
    FTU32 len,i;

    for (i = 0; i < 4; i++) {
        if (jpeg_play_list[i].play) {
            len = ImageToRamG((FTU8 *)jpeg_bmp[i].path,
                              BMPADDR + i*BMPSIZE,FIFOADDR,FIFOSIZE);
            if (len == 0) {
                DBGPRINT;
                return 1;
            }
        }
    }

    return 0;
}
FTU32 Code_fixed (FTVOID)
{
    FTU8 i;

    FillBmpDL(0, BMPADDR, jpeg_bmp, 4);

    HAL_CmdBufIn(CMD_DLSTART);
    HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
    HAL_CmdBufIn(CLEAR(1,1,1));

    HAL_CmdBufIn(BEGIN(BITMAPS));

    for (i = 0; i < 4; i++) {
        HAL_CmdBufIn(TAG_MASK(1));
        HAL_CmdBufIn(TAG(TAG_START+i));

        HAL_CmdBufIn(BITMAP_HANDLE(i));
        HAL_CmdBufIn(CELL(0));
        HAL_CmdBufIn(VERTEX2F(jpeg_play_list[i].pic.X*FT800_PIXEL_UNIT,
                    jpeg_play_list[i].pic.Y*FT800_PIXEL_UNIT));
        HAL_CmdBufIn(TAG_MASK(0));
    }
    HAL_CmdBufIn(END());

    HAL_CmdBufIn(COLOR_RGB(0,0xFF,0));

    CoCmd_TEXT(320,0,21,0,"Touch the JPEG");
    CoCmd_TEXT(320,DESCRIPTION_H,21,0,"trigger start/stop");
    CoCmd_TEXT(320,3*DESCRIPTION_H,21,0,"Single JPEG run speed:");
    CoCmd_TEXT(320,4*DESCRIPTION_H,21,0,"A > B > C > D");
    CoCmd_TEXT(320,5*DESCRIPTION_H,21,0,"Reason: slow file system.");
    CoCmd_TEXT(320,6*DESCRIPTION_H,21,0,"better file system or file");
    CoCmd_TEXT(320,7*DESCRIPTION_H,21,0,"seeking method would");
    CoCmd_TEXT(320,8*DESCRIPTION_H,21,0,"have better speed,");
    CoCmd_TEXT(320,9*DESCRIPTION_H,21,0,"not EVE processing slow.");
    CoCmd_TEXT(320,11*DESCRIPTION_H,21,0,"So stop others. only");
    CoCmd_TEXT(320,12*DESCRIPTION_H,21,0,"check the A FPS");
    CoCmd_TEXT(320,13*DESCRIPTION_H,21,0,"to see a roughly speed");
    if (EVE_SPI_TYPE == 4) {
        CoCmd_TEXT(320,15*DESCRIPTION_H,21,0,"SPI: Quad");
    } else if (EVE_SPI_TYPE == 2) {
        CoCmd_TEXT(320,15*DESCRIPTION_H,21,0,"SPI: Dual");
    } else {
        CoCmd_TEXT(320,15*DESCRIPTION_H,21,0,"SPI: Single");
    }
    CoCmd_TEXT(320,16*DESCRIPTION_H,21,0,"Space left in RAM_G: (K)");
    CoCmd_NUMBER(320,17*DESCRIPTION_H,21,0,EVE_C_MAXSIZE/1024);
    CoCmd_TEXT(320,18*DESCRIPTION_H,21,0,"Codes num in RAM_G:");

    HAL_BufToReg(RAM_CMD,0);

    return HAL_Read32(REG_CMD_DL);
}
FTVOID Code_realtime (FTU32 start_addr)
{
    FTU8 i;

    HAL_CmdBufIn(CMD_DLSTART);
    CoCmd_APPEND(EVE_C_ADDR,start_addr);
    for (i = 0; i < 4; i++) {
        CoCmd_TEXT(jpeg_play_list[i].txt.X,
                jpeg_play_list[i].txt.Y,25,
                OPT_CENTERX,&jpeg_bmp[i].path[FILE_INDEX-1]);
        CoCmd_TEXT(jpeg_play_list[i].pic.X,
                jpeg_play_list[i].pic.Y,25,
                0,"FPS:");
        CoCmd_NUMBER(jpeg_play_list[i].pic.X+FPS_OFFSET_X,
                jpeg_play_list[i].pic.Y,25,
                0,frame_rate[i]);
        CoCmd_NUMBER(320,19*DESCRIPTION_H,21,0,start_addr);
    }
    HAL_CmdBufIn(DISPLAY());
    HAL_CmdBufIn(CMD_SWAP);
    HAL_BufToReg(RAM_CMD,0);
#if !defined(FT9XXEV)
    timerISR();
#endif
}
FTVOID disp4jpeg (FTU32 para)
{
    static FTU32 preDLaddr = 0;

    if (dump4jpeg()) {
        DBGPRINT;
        return;
    }

    /* little trick to reduce the loading of SPI when RAM_G got extra space:
       store the fixed/unchange part of the code from displist in EVE into RAM_G
       in EVE at the beginning, then append the previous stored code in RAM_G
       from RAM_G back into displist when next screen is going to show.
       By this way, no SPI transfer for fixed/unchange part during the display
     */
    if (preDLaddr == 0) {
        preDLaddr = Code_fixed();
        
        if (EVE_C_MAXSIZE <= preDLaddr) {
            /* previous code size should not larger than
               the left space in RAM_G */
            DBGPRINT;
            return;
        }

        CoCmd_MEMCPY(EVE_C_ADDR,RAM_DL,preDLaddr);
        HAL_BufToReg(RAM_CMD,0);
    }

    Code_realtime(preDLaddr);

    pressTHENrelease();

    appGP.appIndex = 1;
}

AppFunc APPS_UI[] = {
	disp4jpeg,
    change4file
};

