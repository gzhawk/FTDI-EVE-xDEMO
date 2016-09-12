/* 
    Demo to show how to impliment:
		1. animation background 1: (MODE == 3) http://excamera.com/sphinx/gameduino2/samples/abstract/index.html
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2014/Jan
*/

typedef enum direct_ {
	L2R = 0, // left to right
	R2L,      // right to left
	B2T,      // bottom to top
	T2B,      // top to bottom
	/* leave it at the bottom */
	DIR_MAX
} direct_e;

/* do not use too much number, or Arduino would run out of memory */
#define BKGND_BNUM 32
#define BKGND_TLT_FONT 25
#define BKGND_TLT_LEN 8
#define BKGND_VALUE_1 (BKGND_BNUM/4)
#define BKGND_VALUE_2 (BKGND_VALUE_1/2)

#define BKGND_BLOB_ACCEL 1
#define BKGND_BLOB_BORDER 72
#define BKGND_BLOB_OUT_SIZE(a) (3 * (a) + BKGND_BLOB_BORDER)
#define BKGND_BLOB_IN_SIZE(a) (3 * (a))
#define BKGND_BLOB_OUT_ALPHA 0x80
#define BKGND_BLOB_IN_ALPHA 0xFF
#define BKGND_BLOB_COLOR_1 COLOR_RGB(0xff,0x80,0x80)
#define BKGND_BLOB_LO_1 0
#define BKGND_BLOB_HI_1 BKGND_BNUM/2
#define BKGND_BLOB_COLOR_2 COLOR_RGB(0xff,0xe0,0xe0)
#define BKGND_BLOB_LO_2 BKGND_BNUM/2
#define BKGND_BLOB_HI_2 BKGND_BNUM
#define BKGND_BLOB_MSPEED 1
#define BKGND_BLOB_CLR_BRD CLEAR_COLOR_RGB(203,222,182)
#define BKGND_BLOB_GRAD_BKG CoCmd_GRADIENT(0,0,0xCBDEB6,0,FT800_LCD_HIGH,0)

typedef enum icon_grp {
	BKGND_ICON_HDL_8ICON = 0,
	BKGND_ICON_HDL_STAR,
	BKGND_ICON_HDL_BALL,
	BKGND_ICON_HDL_CAR,
	BKGND_ICON_HDL_FISH,
	/* leave it at the buttom */
	BKGND_ICON_GROUP
} icon_grp_e;
#define BKGND_ICON_SUBNUM 8
#define BKGND_ICON_FISH_SUBNUM 4
#define BKGND_ICON_CAR_SUBNUM 4
#define BKGND_ICON_CLR_BRD CLEAR_COLOR_RGB(103,122,182)
#define BKGND_ICON_GRAD_BKG CoCmd_GRADIENT(0,0,0x677AB6,0,FT800_LCD_HIGH,0)

#define BKGND_SPK_CLR_BRD CLEAR_COLOR_RGB(199,251,40)
#define BKGND_SPK_GRAD_BKG CoCmd_GRADIENT(0,0,0xC7FB28,0,FT800_LCD_HIGH,0)
#define BKGND_SPK_MSPEED 100
#define BKGND_SPK_COLR    COLOR_RGB(101,190,186)
#define BKGND_SPK_SIZE_MIN    60
#define BKGND_SPK_ZOOM_MAX    280
#define BKGND_SPK_ALPHA 0x80
#define BKGND_SPK_ACCEL 5
#define BKGND_SPK_RATE 4
#define BKGND_SPK_HALF_TLT_LEN 88

#define BKGND_FIZZ_HALF_TLT_LEN 20
#define BKGND_FIZZ_NUM 20
#define BKGND_FIZZ_MSPEED 30
#define BKGND_FIZZ_SIZE_MAX 50

typedef struct coor_ {
	FT16 x, y;
	FT16 xOffset, yOffset;
} coor_st;

coor_st Coor[BKGND_BNUM];

struct {
	direct_e d;
	FTU8 tlt[BKGND_TLT_LEN];
} ctrl_a[DIR_MAX] = {
	{B2T, "Up"},
	{T2B, "Down"},
	{L2R, "->"},
	{R2L, "<-"},
};

FTU8 gIndex = 0;

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(FT9XXEV)
#define PATH_BKGND_8ICON ROOT_PATH"bkgnd\\8icon.raw"
#define PATH_BKGND_STAR ROOT_PATH"bkgnd\\star.raw"
#define PATH_BKGND_BALL ROOT_PATH"bkgnd\\balloon.raw"
#define PATH_BKGND_CAR ROOT_PATH"bkgnd\\car.raw"
#define PATH_BKGND_FISH ROOT_PATH"bkgnd\\fish.raw"
#else
#define PATH_BKGND_8ICON ROOT_PATH"8icon.raw"
#define PATH_BKGND_STAR ROOT_PATH"star.raw"
#define PATH_BKGND_BALL ROOT_PATH"balloon.raw"
#define PATH_BKGND_CAR ROOT_PATH"car.raw"
#define PATH_BKGND_FISH ROOT_PATH"fish.raw"
#endif

bmpHDR_st bmpHD[BKGND_ICON_GROUP] = {
	{(FTC8 *)PATH_BKGND_8ICON,	0,0,ARGB1555,	0,0,	32,	32},
	{(FTC8 *)PATH_BKGND_STAR,	0,0,ARGB1555,	0,0,	32,	32},
	{(FTC8 *)PATH_BKGND_BALL,	0,0,ARGB1555,	0,0,	32,	32},
	{(FTC8 *)PATH_BKGND_CAR,	0,0,ARGB1555,	0,0,	80, 80},
	{(FTC8 *)PATH_BKGND_FISH,	0,0,ARGB1555,	0,0,	50,	50},
};

FTINDEF FT8 offset(FTVOID)
{
	return FTRANDOM(BKGND_VALUE_2) - BKGND_VALUE_1;
}

FTINDEF FTVOID drawBlobs(FTU8 lo, FTU8 hi)
{
	FTU8 i = lo;

	HAL_CmdBufIn(SAVE_CONTEXT());
	HAL_CmdBufIn(COLOR_RGB(0,0,0));
	HAL_CmdBufIn(COLOR_A(BKGND_BLOB_OUT_ALPHA));
	while (i < hi) {
		HAL_CmdBufIn(POINT_SIZE(BKGND_BLOB_OUT_SIZE(i)));
		HAL_CmdBufIn(VERTEX2F(Coor[i].x, Coor[i].y));
		i++;
	}
	HAL_CmdBufIn(RESTORE_CONTEXT());
	i = lo;
	while (i < hi) {
		HAL_CmdBufIn(POINT_SIZE(BKGND_BLOB_IN_SIZE(i)));
		HAL_CmdBufIn(VERTEX2F(Coor[i].x, Coor[i].y));
		i++;
	}
}

FTINDEF FTVOID drawBlob(FTVOID)
{
	HAL_CmdBufIn(SAVE_CONTEXT());
	HAL_CmdBufIn(BEGIN(FTPOINTS));
	HAL_CmdBufIn(COLOR_A(BKGND_BLOB_IN_ALPHA));
	
	HAL_CmdBufIn(BKGND_BLOB_COLOR_1);
	drawBlobs(BKGND_BLOB_LO_1, BKGND_BLOB_HI_1);

	HAL_CmdBufIn(BKGND_BLOB_COLOR_2);
	drawBlobs(BKGND_BLOB_LO_2, BKGND_BLOB_HI_2);
	HAL_CmdBufIn(RESTORE_CONTEXT());
}

FTINDEF FTVOID drawBitmap(icon_grp_e hdl)
{
	FTU8 i, j;

	HAL_CmdBufIn(BEGIN(BITMAPS));

	if (hdl == BKGND_ICON_HDL_FISH) {
		j = BKGND_ICON_FISH_SUBNUM;
	} else if (hdl == BKGND_ICON_HDL_CAR) {
		j = BKGND_ICON_CAR_SUBNUM;
	} else {
		j = BKGND_ICON_SUBNUM;
	}


	for (i = 0; i < BKGND_BNUM; i++) {
		HAL_CmdBufIn(BITMAP_HANDLE(hdl));
		HAL_CmdBufIn(CELL(i%j));
		HAL_CmdBufIn(VERTEX2F(Coor[i].x,Coor[i].y));
	}

	HAL_CmdBufIn(END());
}

FTINDEF FTVOID initCoors(FTU8 init, direct_e d, coor_st *p, FTU8 stlen)
{
	FTU8 k;

	if (init == 0) {
		/* init only once */
		return;
	}

	for (k=0; k<stlen; ++k) {
#if 1 /* disable it if you want another starting effect */
		switch (d) {
			case L2R:
				p[k].x = 0;
				p[k].y = FT800_PIXEL_UNIT*FT800_LCD_HIGH/2;
				break;
			case R2L:
				p[k].x = FT800_PIXEL_UNIT*FT800_LCD_WIDTH;
				p[k].y = FT800_PIXEL_UNIT*FT800_LCD_HIGH/2;
				break;
			case B2T:
				p[k].x = FT800_PIXEL_UNIT*FT800_LCD_WIDTH/2;
				p[k].y = FT800_PIXEL_UNIT*FT800_LCD_HIGH;
				break;
			case T2B:
				p[k].x = FT800_PIXEL_UNIT*FT800_LCD_WIDTH/2;
				p[k].y = 0;
				break;
			default:
				p[k].x = FTRANDOM(FT800_PIXEL_UNIT*FT800_LCD_WIDTH);
				p[k].y = FTRANDOM(FT800_PIXEL_UNIT*FT800_LCD_HIGH);
				break;
		}
#else
		p[k].x = FTRANDOM(FT800_PIXEL_UNIT*FT800_LCD_WIDTH);
		p[k].y = FTRANDOM(FT800_PIXEL_UNIT*FT800_LCD_HIGH);
#endif
		p[k].xOffset = 0;
		p[k].yOffset = 0;
	}
}

FTINDEF FTVOID initSize (FTU8 init, coor_st *p, FTU8 stlen)
{
	FTU8 k;

	if (init == 0) {
		/* init only once */
		return;
	}

	for (k=0; k<stlen; ++k) {
		p[k].xOffset = FTRANDOM(BKGND_SPK_ZOOM_MAX);
	}
}

FTINDEF FTVOID spreadCoors (direct_e d, coor_st *p, FTU8 stlen)
{
	FTU8 k;

	for (k = 0; k < stlen; ++k) {
		switch (d) {
			case L2R:
				p[k].xOffset += BKGND_BLOB_ACCEL;
				break;
			case R2L:
				p[k].xOffset -= BKGND_BLOB_ACCEL;
				break;
			case B2T:
				p[k].yOffset -= BKGND_BLOB_ACCEL;
				break;
			case T2B:
				p[k].yOffset += BKGND_BLOB_ACCEL;
				break;
			default:
				DBGPRINT;
				return;
		}

		p[k].x += p[k].xOffset;
		p[k].y += p[k].yOffset;
	}

	p[FTRANDOM(BKGND_BNUM)].xOffset = offset();
	p[FTRANDOM(BKGND_BNUM)].yOffset = offset();

}

FTINDEF FTVOID animCoor (FTU8 init, direct_e d, coor_st *p, FTU8 stlen)
{
	initCoors(init, d, p, stlen);

	spreadCoors(d, p, stlen);
}

FTINDEF FTVOID animSpark (FTU8 init, direct_e d, coor_st *p, FTU8 stlen)
{
	FTU8 i,j;

	initCoors(init, d, p, stlen);

	initSize(init, p, stlen);

	HAL_CmdBufIn(BEGIN(FTPOINTS));
	HAL_CmdBufIn(SAVE_CONTEXT());
	HAL_CmdBufIn(BKGND_SPK_COLR);
	HAL_CmdBufIn(COLOR_A(BKGND_SPK_ALPHA));

	for (i = 0; i < stlen; i++) {
		HAL_CmdBufIn(POINT_SIZE(p[i].xOffset));
		HAL_CmdBufIn(VERTEX2F(p[i].x, p[i].y));
	}

	for (j = 0; j < stlen/BKGND_SPK_RATE; j++) {
		i = FTRANDOM(stlen);
		p[i].xOffset = (p[i].xOffset > BKGND_SPK_ZOOM_MAX)?BKGND_SPK_SIZE_MIN:p[i].xOffset+BKGND_SPK_ACCEL;
	}

	HAL_CmdBufIn(END());
	HAL_CmdBufIn(RESTORE_CONTEXT());
}

FTINDEF FTVOID animFizz (FTU8 init, FTU32 speed)
{
	FTU8 i;
	static FTU32 count;
	static struct {
		FT16 x,y,s;
		FTU8 r,g,b,a;
	} fz[BKGND_FIZZ_NUM];

	if (init) {
		count = 0;
	}

	if (count++ == 0) {
		for (i = 0; i< BKGND_FIZZ_NUM; i++) {
			fz[i].s = FTRANDOM(BKGND_FIZZ_SIZE_MAX)*FT800_PIXEL_UNIT;
			fz[i].x = FTRANDOM(FT800_LCD_WIDTH)*FT800_PIXEL_UNIT;
			fz[i].y = FTRANDOM(FT800_LCD_HIGH)*FT800_PIXEL_UNIT;
			fz[i].r = FTRANDOM(0xFF);
			fz[i].g = FTRANDOM(0xFF);
			fz[i].b = FTRANDOM(0xFF);
			fz[i].a = FTRANDOM(0xFF);
		}
	} else if (count >= speed) {
		count = 0;
	}

	HAL_CmdBufIn(BEGIN(FTPOINTS));
	HAL_CmdBufIn(SAVE_CONTEXT());

	for (i = 0; i < BKGND_FIZZ_NUM; i++) {
		HAL_CmdBufIn(POINT_SIZE(fz[i].s));
		HAL_CmdBufIn(COLOR_RGB(fz[i].r,fz[i].g,fz[i].b));
		HAL_CmdBufIn(COLOR_A(fz[i].a));
		HAL_CmdBufIn(VERTEX2F(fz[i].x, fz[i].y));
	}

	HAL_CmdBufIn(END());
	HAL_CmdBufIn(RESTORE_CONTEXT());
}

FTVOID background_blobs (FTU32 para)
{
	FTU16 high = 0, flag = 0, count = 0;
	FTU8 init = 1;

	while (0 == TOUCHED) {
		/* move up and down the title */
		if (++count >= BKGND_BLOB_MSPEED) {
			if (flag) {
				high--;
				flag = (high<=0)?0:1;
			} else {
				high++;
				flag = (high>=(FT800_LCD_HIGH-32))?1:0;
			}
			count = 0;
		}
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(BKGND_BLOB_CLR_BRD);
		HAL_CmdBufIn(CLEAR(1,1,1));
		BKGND_BLOB_GRAD_BKG;
	
		animCoor(init, ctrl_a[gIndex].d, Coor, BKGND_BNUM);

		drawBlob();
		
		CoCmd_TEXT(FT800_LCD_WIDTH/2,high,BKGND_TLT_FONT,OPT_CENTERX,(FT8 *)ctrl_a[gIndex].tlt);
		
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
		init = 0;
	}

	while (TOUCHED) {
		/* hold till release */
	}

	gIndex++;
	if (gIndex >= DIR_MAX) {
		appGP.appIndex++;
		gIndex = 0;
	}

	return;
}

FTVOID background_bitmap (FTU32 para)
{
	FTU16 high = 0, flag = 0, count = 0;
	FTU8 init = 1;
	static FTU8 ifLoaded = 0;

	if (ifLoaded == 0) {
		/* load bitmap resources data into FT800 */
		if(APP_OK != appBmpToRamG(BKGND_ICON_HDL_8ICON, RAM_G, &bmpHD[0], BKGND_ICON_GROUP)){
			DBGPRINT;
			return;
		}
		ifLoaded = 1;
	}

	while (0 == TOUCHED) {
		/* move up and down the title */
		if (++count >= BKGND_BLOB_MSPEED) {
			if (flag) {
				high--;
				flag = (high<=0)?0:1;
			} else {
				high++;
				flag = (high>=(FT800_LCD_HIGH-32))?1:0;
			}
			count = 0;
		}
		HAL_CmdBufIn(CMD_DLSTART);
		if (ctrl_a[gIndex].d != R2L) {
			HAL_CmdBufIn(BKGND_ICON_CLR_BRD);
			HAL_CmdBufIn(CLEAR(1,1,1));
			BKGND_ICON_GRAD_BKG;
		} else {
			HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
			HAL_CmdBufIn(CLEAR(1,1,1));
		}
	
		animCoor(init, ctrl_a[gIndex].d, Coor, BKGND_BNUM);

		switch (ctrl_a[gIndex].d) {
			case B2T:
				drawBitmap(BKGND_ICON_HDL_BALL);
				break;
			case T2B:
				drawBitmap(BKGND_ICON_HDL_STAR);
				break;
			case R2L:
				drawBitmap(BKGND_ICON_HDL_CAR);
				break;
			case L2R:
				drawBitmap(BKGND_ICON_HDL_FISH);
				break;
			default:
				drawBitmap(BKGND_ICON_HDL_8ICON);
				break;
		}
		
			
		if (ctrl_a[gIndex].d == R2L) {
			HAL_CmdBufIn(COLOR_RGB(0,0,0));
		}
		CoCmd_TEXT(FT800_LCD_WIDTH/2,high,BKGND_TLT_FONT,OPT_CENTERX,(FT8 *)ctrl_a[gIndex].tlt);
		
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
		init = 0;
	}

	while (TOUCHED) {
		/* hold till release */
	}

	gIndex++;
	if (gIndex >= DIR_MAX) {
		appGP.appIndex++;
		gIndex = 0;
	}

	return;
}

FTVOID background_spk (FTU32 para)
{
	FTU16 wide = FT800_LCD_WIDTH/2, flag = 0, count = 0;
	FTU8 init = 1;

	while (0 == TOUCHED) {
		/* move left and right the title */
		if (++count >= BKGND_BLOB_MSPEED) {
			if (flag) {
				wide--;
				flag = (wide<=BKGND_SPK_HALF_TLT_LEN)?0:1;
			} else {
				wide++;
				flag = (wide>=(FT800_LCD_WIDTH-BKGND_SPK_HALF_TLT_LEN))?1:0;
			}
			count = 0;
		}
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(BKGND_SPK_CLR_BRD);
		HAL_CmdBufIn(CLEAR(1,1,1));
		BKGND_SPK_GRAD_BKG;

		animSpark(init, DIR_MAX, Coor, BKGND_BNUM);
	
		CoCmd_TEXT(wide,FT800_LCD_HIGH/2,BKGND_TLT_FONT,OPT_CENTERX,"Blow Bubble");
		
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
		init = 0;
	}

	while (TOUCHED) {
		/* hold till release */
	}

#if defined(STM32F4)
	appGP.appIndex = 2;
#else
	appGP.appIndex = 3;
#endif
	gIndex = 0;
	
	return;
}

FTVOID background_fizz (FTU32 para)
{
	FTU16 wide = FT800_LCD_WIDTH/2, flag = 0, count = 0;
	FTU8 init = 1;

	while (0 == TOUCHED) {
		/* move left and right the title */
		if (++count >= BKGND_BLOB_MSPEED) {
			if (flag) {
				wide--;
				flag = (wide<=BKGND_FIZZ_HALF_TLT_LEN)?0:1;
			} else {
				wide++;
				flag = (wide>=(FT800_LCD_WIDTH-BKGND_FIZZ_HALF_TLT_LEN))?1:0;
			}
			count = 0;
		}
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
		HAL_CmdBufIn(CLEAR(1,1,1));

		animFizz(init, BKGND_FIZZ_MSPEED);
	
		CoCmd_TEXT(wide,FT800_LCD_HIGH/2,BKGND_TLT_FONT,OPT_CENTERX,"Fizz");
		
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
		init = 0;
	}

	while (TOUCHED) {
		/* hold till release */
	}

	appGP.appIndex = 0;
	gIndex = 0;
	
	return;
}
/*
 * actually, all background_x is able to be put in one routine and change by each touch
 * but here, just show you how to use Apps to switch between each sub routine
 * so, we seperate into some sub routine
 */
AppFunc APPS_UI[] = {
#if !defined(STM32F4)
	background_bitmap,
#endif
	background_blobs,
	background_spk,
	background_fizz
};

