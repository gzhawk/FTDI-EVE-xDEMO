/* 
    Demo for BSH
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2014/Sep
*/

#define BSH_TLT_FONT 6//31
#define BSH_TLT_LEN  10

#define BSH_FIZZ_HALF_TLT_LEN 20
#define BSH_FIZZ_NUM 20
#define BSH_FIZZ_MSPEED 30
#define BSH_FIZZ_SIZE_MAX 50

#define BSH_HOLD_TIME 150

#define BSH_DELTA_MODE (EVE_LCD_WIDTH/4)
#define BSH_DELTA_NUM  (EVE_LCD_HIGH/4)

typedef enum SCREEN_STAT_ {
	STAT_MODE1 = 0,
	STAT_MODE2,
} SCREEN_STAT;

#define BSH_MODE1_COLOR COLOR_RGB(0,0xFF,0)
#define BSH_MODE2_COLOR COLOR_RGB(0xFF,0xFF,0xFF)

#define BSH_NUM_START 2
#define BSH_NUM_END   12
#define BSH_NUM_DEFAULT   7

FTU8 screen_title[BSH_NUM_END+1][BSH_TLT_LEN] = {
	{1,2,3,4,5,0},//{"MODE 1"},
	{1,2,3,4,6,0},//{"MODE 2"},
	{15,5,14,0},//{"+10"},
	{15,14,12,0},//{"+08"},
	{15,14,10,0},//{"+06"},
	{15,14,8,0},//{"+04"},
	{15,14,6,0},//{"+02"},
	{15,14,14,0},//{"+00"},
	{16,14,6,0},//{"-02"},
	{16,14,8,0},//{"-04"},
	{16,14,10,0},//{"-06"},
	{16,14,12,0},//{"-08"},
	{16,5,14,0},//{"-10"}
}; 

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(MSVC2017EMU) 
#define PATH_BSH_FNT ROOT_PATH"bsh\\bsh.raw"
#else
#define PATH_BSH_FNT ROOT_PATH"bsh.raw"
#endif

#define ADDR_BSH_FNT RAM_G

FTU8 gIndex = 0;


STATIC FTVOID animFizz (FTU8 init, FTU32 speed)
{
	FTU8 i;
	static FTU32 count;
	static struct {
		FT16 x,y,s;
		FTU8 r,g,b,a;
	} fz[BSH_FIZZ_NUM];

	if (init) {
		count = 0;
	}

	if (count++ == 0) {
		for (i = 0; i< BSH_FIZZ_NUM; i++) {
			fz[i].s = FTRANDOM(BSH_FIZZ_SIZE_MAX)*EVE_PIXEL_UNIT;
			fz[i].x = FTRANDOM(EVE_LCD_WIDTH)*EVE_PIXEL_UNIT;
			fz[i].y = FTRANDOM(EVE_LCD_HIGH)*EVE_PIXEL_UNIT;
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

	for (i = 0; i < BSH_FIZZ_NUM; i++) {
		HAL_CmdBufIn(POINT_SIZE(fz[i].s));
		HAL_CmdBufIn(COLOR_RGB(fz[i].r,fz[i].g,fz[i].b));
		HAL_CmdBufIn(COLOR_A(fz[i].a));
		HAL_CmdBufIn(VERTEX2F(fz[i].x, fz[i].y));
	}

	HAL_CmdBufIn(END());
	HAL_CmdBufIn(RESTORE_CONTEXT());
}

STATIC FTVOID read_xy (FTU16 *pX, FTU16 *pY)
{
	FTU32 tmp;

	tmp = HAL_Read32(REG_TOUCH_SCREEN_XY);
	*pX = (FTU16)0x7FFF&(tmp>>16);
	*pY = (FTU16)0x7FFF&tmp;
}

STATIC FTVOID title_change (FTU32 *phold_count, FTU8 *pstat, FTU8 *pnum_index)
{
	static FTU16 X1 = 0xFFFF,Y1 = 0xFFFF,X2 = 0xFFFF,Y2 = 0xFFFF,x,y;

	if (!TOUCHED) {
		if (X2 == 0xFFFF&&Y2 == 0xFFFF) {
			return;
		}
	} else {
		if (X1 == 0xFFFF&&Y1 == 0xFFFF) {
			read_xy(&X1,&Y1);
		}
		read_xy(&X2,&Y2);
		return;
	}

	if (X1 == X2 && Y1 == Y2) {
		/* single click */
		X1 = 0xFFFF;
		Y1 = 0xFFFF;
		X2 = 0xFFFF;
		Y2 = 0xFFFF;
		return;
	}

	if (X2 <= X1) {
		x = X1 - X2;
		if (Y2 <= Y1) {
			y = Y1 - Y2;
			if (x >= y) {
				if (BSH_DELTA_MODE <= x && *pstat == STAT_MODE2) {
					*pstat = STAT_MODE1;
					*phold_count = BSH_HOLD_TIME;					
				}
			} else {
				if (BSH_DELTA_NUM <= y && *pnum_index > BSH_NUM_START) {
					*pnum_index = *pnum_index - 1;
				}
			}
		} else {
			y = Y2 - Y1;
			if (x >= y) {
				if (BSH_DELTA_MODE <= x && *pstat == STAT_MODE2) {
					*pstat = STAT_MODE1;
					*phold_count = BSH_HOLD_TIME;					
				}
			} else {
				if (BSH_DELTA_NUM <= y && *pnum_index < BSH_NUM_END) {
					*pnum_index = *pnum_index + 1;
				}
			}
		}
	} else {
		x = X2 - X1;
		if (Y2 <= Y1) {
			y = Y1 - Y2;
			if (x >= y) {
				if (BSH_DELTA_MODE <= x && *pstat == STAT_MODE1) {
					*pstat = STAT_MODE2;
					*phold_count = BSH_HOLD_TIME;					
				}
			} else {
				if (BSH_DELTA_NUM <= y && *pnum_index > BSH_NUM_START) {
					*pnum_index = *pnum_index - 1;
				}
			}
		} else {
			y = Y2 - Y1;
			if (x >= y) {
				if (BSH_DELTA_MODE <= x && *pstat == STAT_MODE1) {
					*pstat = STAT_MODE2;
					*phold_count = BSH_HOLD_TIME;					
				}
			} else {
				if (BSH_DELTA_NUM <= y && *pnum_index < BSH_NUM_END) {
					*pnum_index = *pnum_index + 1;
				}
			}
		}
	}
	
	X1 = 0xFFFF;
	Y1 = 0xFFFF;
	X2 = 0xFFFF;
	Y2 = 0xFFFF;
}

FTVOID bsh_main_win (FTU32 para)
{
	static FTU8 init = 1, 
				screen_stat = STAT_MODE1, 
				screen_title_index = BSH_NUM_DEFAULT;
	static FTU32 hold_count = 0;

	static FT_Gpu_Fonts_t stFB;

	/* load the font resources into EVE */
	if (init) {
		if (0 == appFileToRamG(PATH_BSH_FNT,ADDR_BSH_FNT,0,(FTU8 *)&stFB,sizeof(FT_Gpu_Fonts_t))) {
			DBGPRINT;
			return;
		}
	}
	title_change(&hold_count, &screen_stat, &screen_title_index);

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));

	animFizz(init, BSH_FIZZ_MSPEED);

	if (screen_stat == STAT_MODE1) {
		HAL_CmdBufIn(BSH_MODE1_COLOR);
	} else {
		HAL_CmdBufIn(BSH_MODE2_COLOR);
	}
	
	/* set the external font to each font index */
	CoCmd_SETFONT(BSH_TLT_FONT, ADDR_BSH_FNT, &stFB);

	if (!hold_count) {
		CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2,BSH_TLT_FONT,OPT_CENTERX,&screen_title[screen_title_index][0]);
	} else {
		hold_count--;
		CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2,BSH_TLT_FONT,OPT_CENTERX,&screen_title[screen_stat][0]);
	}

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
	
	init = 0;

	appGP.appIndex = 0;
	gIndex = 0;
	
	return;
}

AppFunc APPS_UI[] = {
	bsh_main_win
};

