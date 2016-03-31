/* 
    Sample code to demo how to play AVI file
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2015/May
*/
#ifdef DEMO_AVI

#define PATH_AVI ROOT_PATH"avi\\x.avi"

/* your avi frame, NOT your LCD resolution */
#define AVI_FRAME_WIDE 800
#define AVI_FRAME_HIGH 480

/* 
 * EVE need a 4 bytes space to store a flag, 
 * to indicate if the current frame is the last frame.
 * one frame just like one JPEG picture
 * while using RGB565 to display frame
 * at least need W*H*2 RAM_G space
 * I'm going to put the fifo at the end of RAM_G
 * you can put any where you like
*/
#define AVI_HANDLE   0
#define AVI_RAM_ADDR RAM_G
#define AVI_COMPLETE (AVI_FRAME_WIDE*AVI_FRAME_HIGH*2)
#define AVI_FIFOADDR (AVI_COMPLETE + 4)
#define AVI_FIFOSIZE (FT800_RAMG_SIZE - AVI_FIFOADDR)
#define SETDEFAULT HAL_CmdBufIn(BITMAP_TRANSFORM_A(256)); \
	HAL_CmdBufIn(BITMAP_TRANSFORM_B(0)); \
	HAL_CmdBufIn(BITMAP_TRANSFORM_C(0)); \
	HAL_CmdBufIn(BITMAP_TRANSFORM_D(0)); \
	HAL_CmdBufIn(BITMAP_TRANSFORM_E(256)); \
	HAL_CmdBufIn(BITMAP_TRANSFORM_F(0))
FTINDEF FTVOID Display (FTU32 opt, FTU32 start, FTU32 frame, FTU32 fIndex, FTU32 fLen)
{
	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
	HAL_CmdBufIn(CLEAR(1,1,1));

	if (OPT_MEDIAFIFO&opt) {
		HAL_CmdBufIn(BITMAP_HANDLE(AVI_HANDLE));
		HAL_CmdBufIn(BITMAP_SOURCE(AVI_RAM_ADDR));
		HAL_CmdBufIn(BITMAP_LAYOUT(RGB565, (AVI_FRAME_WIDE * 2), AVI_FRAME_HIGH));
		HAL_CmdBufIn(BITMAP_LAYOUT_H((AVI_FRAME_WIDE * 2) >> 10, AVI_FRAME_HIGH >> 9)); 
		HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, AVI_FRAME_WIDE, AVI_FRAME_HIGH));
		HAL_CmdBufIn(BITMAP_SIZE_H(AVI_FRAME_WIDE>>9, AVI_FRAME_HIGH>>9));
		HAL_CmdBufIn(BEGIN(BITMAPS));
		if (start) {
			HAL_CmdBufIn(VERTEX2F(0,0));
		} else {
			CoCmd_LOADIDENTITY;
			CoCmd_SCALE(FT800_TRANSFORM_MAX/2,FT800_TRANSFORM_MAX/2);
			CoCmd_SETMATRIX;
			
			HAL_CmdBufIn(VERTEX2F(0,0));
			HAL_CmdBufIn(VERTEX2F(400*FT800_PIXEL_UNIT,240*FT800_PIXEL_UNIT));
			HAL_CmdBufIn(VERTEX2F(200*FT800_PIXEL_UNIT,120*FT800_PIXEL_UNIT));

			SETDEFAULT;
		}
		HAL_CmdBufIn(END());

		HAL_CmdBufIn(COLOR_RGB(0,0xFF,0));
		if (start) {
			CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/3,25,OPT_CENTERX,"MEDIAFIFO AVI Frame (full screen)");
		} else {
			CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,25,OPT_CENTERX,"MEDIAFIFO AVI Frame (zoom out, overlap)");
		}
		CoCmd_NUMBER(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/3*2,25,OPT_CENTERX,frame);
		CoCmd_NUMBER(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/3*2+30,25,OPT_CENTERX,fIndex);
		CoCmd_NUMBER(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/3*2+60,25,OPT_CENTERX,fLen);
	} else {
		if (start) {
			HAL_CmdBufIn(COLOR_RGB(0,0xFF,0));
			CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,25,OPT_CENTERX,"Start: play AVI using CMD buffer");
		} else {
			HAL_CmdBufIn(COLOR_RGB(0xFF,0,0));
			CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,25,OPT_CENTERX,"Stop: play AVI using CMD buffer");
		}
	}

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
}

FTINDEF FTU32 mfifoAviWrite (FTU32 mfifo_addr, FTU32 mfifo_size,FTU32 avi_addr,FTU32 flag_addr, FTU32 opt,FTU32 resHDL, FTU32 file_len)
{
	FTU32 mfifo_rd = HAL_Read32(REG_MEDIAFIFO_READ), mfifo_wr = HAL_Read32(REG_MEDIAFIFO_WRITE),
		  l, first_block = (mfifo_size >= file_len)?file_len:mfifo_size;
	FTU32 index = 0, frame = 1;
	static FTU8 start = 1;

	/* set the MEDIAFIFO space in RAMG*/
	CoCmd_MEDIAFIFO(mfifo_addr, mfifo_size);

	HAL_BufToReg(RAM_CMD, 0);

	/* write data to RAM_G */
	appResToDes(resHDL, mfifo_addr, index, first_block, resWrEve);

	index += first_block;

	HAL_Write32(REG_MEDIAFIFO_WRITE, first_block);

	CoCmd_VIDEOSTART;

	CoCmd_VIDEOFRAME(avi_addr, flag_addr);

	HAL_BufToReg(RAM_CMD, 0);

	Display(opt, start, frame++, index, file_len);
	while (HAL_Read32(flag_addr) && (index < file_len)) {
		/* EVE control the read pointer */
		mfifo_rd = HAL_Read32(REG_MEDIAFIFO_READ); 
		/* MCU control the write pointer */
		mfifo_wr = HAL_Read32(REG_MEDIAFIFO_WRITE);

		if (mfifo_rd == mfifo_wr) {
			/* full avi frame parsing finished */
			start = !start;
			break;
		}

		/* file remain */
		l = file_len - index;
	
		if (mfifo_wr > mfifo_rd) {
			if (mfifo_size - mfifo_wr >= l) {
				/* 
				 * fifo |---r---w---|
				 * l    |**|
				 */
				appResToDes(resHDL, mfifo_addr+mfifo_wr, index, l, resWrEve);

				HAL_Write32(REG_MEDIAFIFO_WRITE, mfifo_wr+l);
			} else if (mfifo_size == mfifo_wr) {
				/* 
				 * fifo |---r------w|
				 * l1   |**|
				 * l2   |*****|
				 */
				l = (mfifo_rd >= l)?l:mfifo_rd - 4;
				
				appResToDes(resHDL, mfifo_addr, index, l, resWrEve);

				HAL_Write32(REG_MEDIAFIFO_WRITE, l);
			} else {
				/* 
				 * fifo |---r---w---|
				 * l1   |*****|
				 * l2   |*******|
				 */
				appResToDes(resHDL, mfifo_addr+mfifo_wr, index, mfifo_size - mfifo_wr, resWrEve);

				index += (mfifo_size - mfifo_wr);
				
				l -= (mfifo_size - mfifo_wr);

				l = (mfifo_rd >= l)?l:mfifo_rd - 4;

				appResToDes(resHDL, mfifo_addr, index, l, resWrEve);

				HAL_Write32(REG_MEDIAFIFO_WRITE, l);
			}
		} else {
			/* 
			 * fifo |---w---r---|
			 * l1   |**|
			 * l2   |*****|
			 */
			if (mfifo_rd - mfifo_wr < l) {
				l = mfifo_rd - mfifo_wr - 4;
			}
				
			appResToDes(resHDL, mfifo_addr+mfifo_wr, index, l, resWrEve);

			HAL_Write32(REG_MEDIAFIFO_WRITE, mfifo_wr+l);
		}
		/* file buffer pointer */
		index += l;
		
		/* buffer show one frame command to command RAM */
		CoCmd_VIDEOFRAME(avi_addr, flag_addr);

		/* write the command to co-processor */
		HAL_BufToReg(RAM_CMD, 0);

		Display(opt, start, frame++, index, file_len);
		if (TOUCHED) {
			while (TOUCHED) {
				/* hold till touch released then go next */;
				FTDELAY(50);
			}
			start = !start;
			return 0;
		}
	}
	
	start = !start;
	return file_len;
}
FTINDEF FTVOID cmdbufAviWrite (FTU32 resHDL, FTU32 file_len, FTU32 opt)
{
	FTU32 i = 0, l = 0;
	
	HAL_CmdToReg(CMD_PLAYVIDEO);
	HAL_CmdToReg(opt | OPT_SOUND);

	while (i < file_len) {
		l = HAL_Read32(REG_CMDB_SPACE);
		/* copy it into CMD buffer in EVE */
		appResToDes(resHDL, REG_CMDB_WRITE, i, l, resWrEve);
		i += l;
		if (TOUCHED) {
			while (TOUCHED) {
				/* hold till touch released then go next */;
				FTDELAY(50);
			}
			/* Once cmd_playvideo is issued, the co processor expects the whole file to be streamed to it
			 * as the AVI parser is also part of the firmware. 
			 * There is no support for pause, stop, fast forward/fast rewind player commands. 
			 * So the only way to stop the playvideo in between is by co processor reset. */

			/*
			   Bit 2 - 0 :
			   Bit 0 for coprocessor engine,
			   Bit 1 for touch engine,
			   Bit 2 for audio engine.
			   Write 1 to reset the corresponding engine. Write zero to go back normal working
			   status.
			*/
			HAL_Write32(REG_CPURESET, 1);
			do {
				FTDELAY(100);
				HAL_Write32(REG_CPURESET, 0);
				FTDELAY(100);
				i = HAL_Read32(REG_CPURESET);
			} while (1&i);
			break;
		}
	}
}

FTINDEF FTU32 AVIToRamG(FTU8 *path, FTU32 ramgAddr, FTU32 flagAddr, FTU32 fifoAddr, FTU32 fifoSize, FTU32 opt)
{
	FTU32 resHDL, Len;

	resHDL = appResOpen(path);
	if (resHDL == 0) {
		DBGPRINT;
		return 0;
	}

	Len = appResSize(resHDL);

	if (OPT_MEDIAFIFO & opt) {
		mfifoAviWrite(fifoAddr,fifoSize,ramgAddr,flagAddr,opt,resHDL,Len);
	} else {
		cmdbufAviWrite(resHDL, Len, opt);
	}

	appResClose(resHDL);
	return Len;
}

/* 
 * base on my understanding, when using EVE to play AVI file:
 * 1. CMD_PLAYVIDEO, can play AVI by using CMD buffer or MEDIAFIFO
 *    example just use CMD buffer to use CMD_PLAYVIDEO function
 * 2. only CMD_PLAYVIDEO can let EVE output the audio in AVI, 
 *    if CoCmd_VIDEOFRAME be used, MCU has to parse the audio and do the output by it's own
 * 3. CMD_PLAYVIDEO always start from (0,0), which means no display offset is available
 * 4. CMD_PLAYVIDEO no other layer (button, text, number, etc.) can be overlap on screen while playing,
 *    use CoCmd_VIDEOFRAME for display offset or give an extra layer on frame.
 */
FTVOID play_avi (FTU32 para)
{
	FTU32 opt = OPT_MEDIAFIFO | OPT_FULLSCREEN | OPT_NOTEAR;

	/* just for debug */
	appGP.appIndex = 1;
	appGP.appPara = 0;

	do {
		if (OPT_MEDIAFIFO&opt) {
			if (AVIToRamG((FTU8 *)PATH_AVI,AVI_RAM_ADDR,AVI_COMPLETE,AVI_FIFOADDR,AVI_FIFOSIZE,opt) == 0) {
				DBGPRINT;
				return;
			}
			
			if (AVIToRamG((FTU8 *)PATH_AVI,AVI_RAM_ADDR,AVI_COMPLETE,AVI_FIFOADDR,AVI_FIFOSIZE,opt) == 0) {
				DBGPRINT;
				return;
			}
		} else {
			Display(opt, 1, 0, 0, 0);
			FTDELAY(1000);
			if (AVIToRamG((FTU8 *)PATH_AVI,AVI_RAM_ADDR,0,0,0,0) == 0) {
				DBGPRINT;
				return;
			}
			Display(opt, 0, 0, 0, 0);
			FTDELAY(1000);
		}

		opt = (opt & OPT_MEDIAFIFO)?(opt & (~OPT_MEDIAFIFO)):(opt | OPT_MEDIAFIFO);
	} while (1);
}

AppFunc Apps[] = {
	play_avi,
	/* Leave this NULL at the buttom of this array */
	NULL
};

#endif

