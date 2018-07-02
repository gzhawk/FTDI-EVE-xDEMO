/* 
    Sample to play demo for Midea
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2013/Dec
*/

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
#define PATH_M_C0 ROOT_PATH"midea\\m0_c0.raw"
#define PATH_M_C1 ROOT_PATH"midea\\m0_c1.raw"
#define PATH_M_B0 ROOT_PATH"midea\\m0_b0.raw"
#define PATH_M_B1 ROOT_PATH"midea\\m0_b1.raw"
#define PATH_LINE_TOP0 ROOT_PATH"midea\\top0.raw"
#define PATH_LINE_TOP1 ROOT_PATH"midea\\top1.raw"
#define PATH_STATUS ROOT_PATH"midea\\status00.raw"
#else
#define PATH_M_C0 ROOT_PATH"m0_c0.raw"
#define PATH_M_C1 ROOT_PATH"m0_c1.raw"
#define PATH_M_B0 ROOT_PATH"m0_b0.raw"
#define PATH_M_B1 ROOT_PATH"m0_b1.raw"
#define PATH_LINE_TOP0 ROOT_PATH"top0.raw"
#define PATH_LINE_TOP1 ROOT_PATH"top1.raw"
#define PATH_STATUS ROOT_PATH"status00.raw"
#endif

#define DXT1_BLOCK_NUMS (4)
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
#define DXT1_PATH_LEN   (30)
#define DXT1_PATH_INDEX_M (17)
#define BITMAP_STATUS_INDEX (23)
#else
#define DXT1_PATH_LEN   (8+1+3+1)
#define DXT1_PATH_INDEX_M (1)
#define BITMAP_STATUS_INDEX (7)
#endif

#define DXT1_START_ADDR    RAM_G
#define DXT1_START_HDL     0
#define DXT1_HDL_NUMS      2
#define BITMAP_START_ADDR  (128*1024UL)
#define BITMAP_START_HDL   (DXT1_START_HDL+2*DXT1_HDL_NUMS)
#define BITMAP_STATUS_ADDR (BITMAP_START_ADDR+9600*2)
#define BITMAP_NUMS        (3)
#define BITMAP_SPEED       (10) /* this number depends on your MCU speed */

typedef struct dxt1_path_ {
	FTU8 path_c0[DXT1_PATH_LEN];
	FTU8 path_c1[DXT1_PATH_LEN];
	FTU8 path_b0[DXT1_PATH_LEN];
	FTU8 path_b1[DXT1_PATH_LEN];
} dxt1_path_st;

dxt1_path_st dxt1MPath = {
	PATH_M_C0,
	PATH_M_C1,
	PATH_M_B0,
	PATH_M_B1
};

STATIC FTU32 dxt1File (dxt1_path_st *path, FTU32 addrRAM)
{
	FTU32 len, addr = addrRAM;

	len = appFileToRamG((FTC8 *)path->path_c0,addr,0,0,0);
	if (len == 0) {
		DBGPRINT;
		return len;
	}
	addr += len;
	len = appFileToRamG((FTC8 *)path->path_c1,addr,0,0,0);
	if (len == 0) {
		DBGPRINT;
		return len;
	}
	addr += len;
	len = appFileToRamG((FTC8 *)path->path_b0,addr,0,0,0);
	if (len == 0) {
		DBGPRINT;
		return len;
	}
	addr += len;
	len = appFileToRamG((FTC8 *)path->path_b1,addr,0,0,0);
	if (len == 0) {
		DBGPRINT;
		return len;
	}
	addr += len;

	return (addr - addrRAM);
}

STATIC FTVOID dxt1BitmapInfo (FTU8 startHdl, FTU32 startAddr, FTU16 W, FTU16 H)
{
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl+1));
	HAL_CmdBufIn(BITMAP_SOURCE(startAddr));
	HAL_CmdBufIn(BITMAP_LAYOUT(RGB565, W/DXT1_BLOCK_NUMS*2, H/DXT1_BLOCK_NUMS));
	HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, W, H));

	HAL_CmdBufIn(BITMAP_HANDLE(startHdl));
	HAL_CmdBufIn(BITMAP_SOURCE(startAddr + 2*(W/DXT1_BLOCK_NUMS*2*H/DXT1_BLOCK_NUMS)));
	HAL_CmdBufIn(BITMAP_LAYOUT(L1, W/DXT1_BLOCK_NUMS/2, H));
	HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, W, H));
}

STATIC FTVOID dxt1FormatInfo (FTU8 startHdl, FTU16 X, FTU16 Y)
{
	HAL_CmdBufIn(BLEND_FUNC(ONE,ZERO));
	HAL_CmdBufIn(COLOR_A(0x55));
	HAL_CmdBufIn(VERTEX2II(X,Y,startHdl,0));

	HAL_CmdBufIn(BLEND_FUNC(ONE,ONE));
	HAL_CmdBufIn(COLOR_A(0xAA));
	HAL_CmdBufIn(VERTEX2II(X,Y,startHdl,1));

	HAL_CmdBufIn(COLOR_MASK(1,1,1,0));

	CoCmd_LOADIDENTITY;
	CoCmd_SCALE(4*EVE_TRANSFORM_MAX, 4*EVE_TRANSFORM_MAX);
	CoCmd_SETMATRIX;

	HAL_CmdBufIn(BLEND_FUNC(DST_ALPHA,ZERO));
	HAL_CmdBufIn(VERTEX2II(X,Y,startHdl+1,1));

	HAL_CmdBufIn(BLEND_FUNC(ONE_MINUS_DST_ALPHA,ONE));
	HAL_CmdBufIn(VERTEX2II(X,Y,startHdl+1,0));

	HAL_CmdBufIn(END());

}

STATIC FTVOID valueChg (FTU32 *paddr, FTU8 *phdl, dxt1_path_st *pst, FTU32 len, FTU8 *ptouch)
{
	if (TOUCHED == 0) {
		if (*ptouch == 0) {
			return;
		}
	} else {
		*ptouch = 1;
		return;
	}

	*ptouch = 0;

	if (pst->path_c0[DXT1_PATH_INDEX_M] == '1') {
		pst->path_c0[DXT1_PATH_INDEX_M] = '0';
		pst->path_c1[DXT1_PATH_INDEX_M] = '0';
		pst->path_b0[DXT1_PATH_INDEX_M] = '0';
		pst->path_b1[DXT1_PATH_INDEX_M] = '0';
	} else {
		pst->path_c0[DXT1_PATH_INDEX_M]++;
		pst->path_c1[DXT1_PATH_INDEX_M]++;
		pst->path_b0[DXT1_PATH_INDEX_M]++;
		pst->path_b1[DXT1_PATH_INDEX_M]++;
	}

	if (*paddr == DXT1_START_ADDR) {
		*paddr += len;
		*phdl += DXT1_HDL_NUMS;
	} else {
		*paddr = DXT1_START_ADDR;
		*phdl = DXT1_START_HDL;
	}
}

STATIC FTVOID addBMP (dxt1_path_st *pst)
{
	HAL_CmdBufIn(BEGIN(BITMAPS));

	if (pst->path_c0[DXT1_PATH_INDEX_M] == '0') {
		/* line top */
		HAL_CmdBufIn(VERTEX2II(0,52,BITMAP_START_HDL,0));
		HAL_CmdBufIn(VERTEX2II(34,203,BITMAP_START_HDL+2,0));
	} else {
		/* line top */
		HAL_CmdBufIn(VERTEX2II(0,42,BITMAP_START_HDL+1,0));
	}
	HAL_CmdBufIn(END());
}

FTVOID playmidea (FTU32 para)
{
	FTU32 len, addr = DXT1_START_ADDR, count = 0;
	FTU8 hdl = DXT1_START_HDL, touch = 0, pre_touch = 0, tmp[DXT1_PATH_LEN] = PATH_STATUS;
	dxt1_path_st *path = &dxt1MPath;
	bmpHDR_st bmphdl[BITMAP_NUMS] = {
		{PATH_LINE_TOP0, 0,0,RGB565, 0,0, 480, 10},
		{PATH_LINE_TOP1, 0,0,RGB565, 0,0, 480, 10},
		{PATH_STATUS,    0,0,RGB565, 0,0, 126, 62},
	};

#if defined(STM32F4)
	return;
#endif
	appBmpToRamG(BITMAP_START_HDL, BITMAP_START_ADDR, bmphdl, BITMAP_NUMS);
	len = dxt1File(path,addr);
	if (len == 0) {
		DBGPRINT;
		return;
	}
	do {
		if (pre_touch == 1 && touch == 0) {
			len = dxt1File(path,addr);
			if (len == 0) {
				DBGPRINT;
				return;
			}
		}

		if (touch) {
			if (tmp[BITMAP_STATUS_INDEX] != '5') {
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
				if (BITMAP_SPEED == count++) {
					tmp[BITMAP_STATUS_INDEX]++;
					count = 0;
				}
#else
				tmp[BITMAP_STATUS_INDEX]++;
#endif
			} else {
				tmp[BITMAP_STATUS_INDEX] = '1';
			}
			if (appFileToRamG((FTC8 *)tmp,BITMAP_STATUS_ADDR,0,0,0) == 0) {
				DBGPRINT;
				return;
			}
		} else {
			if (tmp[BITMAP_STATUS_INDEX] != '0') {
				tmp[BITMAP_STATUS_INDEX] = '0';
				if (appFileToRamG((FTC8 *)tmp,BITMAP_STATUS_ADDR,0,0,0) == 0) {
					DBGPRINT;
					return;
				}
			}
		}

		if (touch == 0) {
			HAL_CmdBufIn(CMD_DLSTART);
			HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
			HAL_CmdBufIn(CLEAR(1,1,1));

			HAL_CmdBufIn(SAVE_CONTEXT());
			dxt1BitmapInfo(hdl, addr, WQVGA_WIDTH, WQVGA_HIGH);

			HAL_CmdBufIn(BEGIN(BITMAPS));

			dxt1FormatInfo(hdl,0,0);
			HAL_CmdBufIn(RESTORE_CONTEXT());

			addBMP(path);

			HAL_CmdBufIn(DISPLAY());
			HAL_CmdBufIn(CMD_SWAP);
			HAL_BufToReg(RAM_CMD,0);
		}

		pre_touch = touch;
		valueChg(&addr, &hdl, path, len, &touch);
	} while (1);
}

AppFunc APPS_UI[] = {
	playmidea
};

