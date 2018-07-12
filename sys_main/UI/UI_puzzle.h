/* 
    Sample to play puzzle image function 
	1. only fresh the RAM_G to simulate animation effect
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2013/Nov
*/

#define PATH_LEN 50
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(MSVC2017EMU) 
#define ANIMATION_NUM 10
#define ANIMATION_TUB_TAG 7 /* tag number of tube */
#else
#define ANIMATION_NUM 9
#define ANIMATION_TUB_TAG 10 /* make it an impossible number */
#endif
#define ANIMATION_SPEED 80
#define TOUCH_SENCE     100

FTU32 ani_addr[ANIMATION_NUM] = {0};

typedef struct bitmap_ {
	FTU8 path[PATH_LEN];
	FTU32 x;
	FTU32 y;
	FTU32 wide;
	FTU32 high;
} bitmap_st;

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(MSVC2017EMU) 
#define PUZZLE_PATH ROOT_PATH"puzzle\\"
#else
#define PUZZLE_PATH 
#endif
/* due to the name length limitation of SD Card DOS filesystem
 * we have to limit the file name length to 8+3=11 */
bitmap_st brgd_bitmap[] = {
	{PUZZLE_PATH"0_0.raw",     0,0,144,52}, 
	{PUZZLE_PATH"189_0.raw",   189,0,4,52},
	{PUZZLE_PATH"238_0.raw",   238,0,17,52},
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(MSVC2017EMU) 
	/* Due to limited resource of Arduino (2K RAM in total, SD card module use too much RAM)
	 * we remove some parts of this unchange resources
	 * it may looks bad, please bear it...
	 */
	{PUZZLE_PATH"300_0.raw",   300,0,14,52},
	{PUZZLE_PATH"359_0.raw",   359,0,12,52},
	{PUZZLE_PATH"416_0.raw",   416,0,7,52},
	{PUZZLE_PATH"468_0.raw",   468,0,12,52},
	
	{PUZZLE_PATH"0_52.raw",    0,52,480,17},
	
	{PUZZLE_PATH"0_69.raw",   0,69,258,134},

	{PUZZLE_PATH"0_198.raw",   0,198,34,68},
	{PUZZLE_PATH"290_198.raw",290,198,4,68},
	{PUZZLE_PATH"420_198.raw",420,198,60,68},
	{PUZZLE_PATH"160_203.raw",160,203,4,62},

	{PUZZLE_PATH"0_265.raw",  0,265,480,4},
#endif
	{0}
};
bitmap_st ani_bitmap[ANIMATION_NUM] = {
	{PUZZLE_PATH"heat00.raw",   144,0,45,52}, 
	{PUZZLE_PATH"cold00.raw",   193,0,45,52}, 
	{PUZZLE_PATH"frzr00.raw",   255,0,45,52}, 
	{PUZZLE_PATH"lack00.raw",   314,0,45,52}, 
	{PUZZLE_PATH"drain00.raw",  371,0,45,52}, 
	{PUZZLE_PATH"sound00.raw",  423,0,45,52}, 

	/* if tube position change, ANIMATION_TUB_TAG need to be changed */
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(MSVC2017EMU) 
	/* remove this animation effect in Arduino platform
	 * since it's too diffical to make the effect run smooth
	 * in such tiny RAM (2K RAM in total, SD card module use too much RAM)*/
	{PUZZLE_PATH"tube01.raw", 258,69,222,134},
#endif

	{PUZZLE_PATH"status00.raw", 34,203,126,62}, 
	{PUZZLE_PATH"set00.raw",    164,203,126,62}, 
	{PUZZLE_PATH"help00.raw",   294,203,126,62}
};

appRet_en loadBitmap (FTVOID)
{
	FTU8 i;
	FTU32 addr = 0;
	/* load unchanged bitmap resources data into EVE */
	for (i = 0, addr = RAM_G; brgd_bitmap[i].path[0];
		addr += brgd_bitmap[i].wide*brgd_bitmap[i].high*2, i++) {
		if (0 == appFileToRamG((FTC8 *)brgd_bitmap[i].path,addr,0,0,0)) {
			DBGPRINT;
			return APP_ERR_LEN;
		}
		HAL_CmdBufIn(BITMAP_SOURCE(addr));
		HAL_CmdBufIn(BITMAP_LAYOUT(RGB565, brgd_bitmap[i].wide*2, brgd_bitmap[i].high));
		HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, brgd_bitmap[i].wide, brgd_bitmap[i].high));
		HAL_CmdBufIn(BEGIN(BITMAPS));
 		HAL_CmdBufIn(VERTEX2F(brgd_bitmap[i].x*EVE_PIXEL_UNIT,brgd_bitmap[i].y*EVE_PIXEL_UNIT));
		HAL_CmdBufIn(END());
	}
	/* load changed bitmap resources data into EVE */
	HAL_CmdBufIn(TAG_MASK(1));
	for (i = 0; i < ANIMATION_NUM;
		addr += ani_bitmap[i].wide*ani_bitmap[i].high*2, i++) {
		if (0 == appFileToRamG((FTC8 *)ani_bitmap[i].path,addr,0,0,0)) {
			DBGPRINT;
			return APP_ERR_LEN;
		}
		HAL_CmdBufIn(BITMAP_SOURCE(addr));
		HAL_CmdBufIn(BITMAP_LAYOUT(RGB565, ani_bitmap[i].wide*2, ani_bitmap[i].high));
		HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, ani_bitmap[i].wide, ani_bitmap[i].high));
		HAL_CmdBufIn(BEGIN(BITMAPS));
		HAL_CmdBufIn(TAG(i+1));
 		HAL_CmdBufIn(VERTEX2F(ani_bitmap[i].x*EVE_PIXEL_UNIT,ani_bitmap[i].y*EVE_PIXEL_UNIT));
		HAL_CmdBufIn(END());
		ani_addr[i] = addr;
	}
	HAL_CmdBufIn(TAG_MASK(0));
	return APP_OK;
}
STATIC FTU8 getposition (FTC8 *p)
{
	FTU8 i = 0;

	while (p[i]) {
		i++;
	}

	/* the file name format is nnnnXY.raw, return the Y location */
	return (i - 5);
}

STATIC FTVOID deselect (bitmap_st *pre, FTU8 tag)
{
	FTU8 i = getposition((FTC8 *)pre->path);
	if (tag == ANIMATION_TUB_TAG) {
		pre->path[i] = '1';
	} else {
		pre->path[i] = '0';
	}
	pre->path[i-1] = '0';
	if (0 == appFileToRamG((FTC8 *)pre->path,ani_addr[tag - 1],0,0,0)) {
		DBGPRINT;
		return;
	}
}

STATIC FTVOID changename (bitmap_st *parry, FTU8 Tag, FTU8 i, FTU8 *p)
{
	if (Tag == ANIMATION_TUB_TAG) {
		if (parry->path[i] >= '9') {
			parry->path[i] = '0';
			parry->path[i - 1] = '1';
			*p = '1';
		} else if (parry->path[i] >= '2' && parry->path[i-1] >= '1') {
			parry->path[i] = '1';
			parry->path[i - 1] = '0';
			*p = '1';
		} else {
			(*p)++;
			parry->path[i] = *p;
		}
	} else {
		if (parry->path[i] >= '5') {
			parry->path[i] = '1';
			*p = '1';
		} else {
			(*p)++;
			parry->path[i] = *p;
		}
	}
}
FTVOID playpuzzle (FTU32 para)
{
	FTU8 Tag = 0, Touch, ani_index;
	FTU32 sensitive = 0, speed = 0;

#if defined(STM32F4)
	return;
#endif
	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));

	/* load bitmap resources data into EVE */
	if(APP_OK != loadBitmap()){
		DBGPRINT;
		return;
	}

#if !defined(MSVC2010EXPRESS) && !defined(MSVC2012EMU) && !defined(MSVC2017EMU) 
	CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/4,24,OPT_CENTERX,"Due to limited memory in Arduino");
	CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2,24,OPT_CENTERX,"We remove some parts of the background");
	CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/4*3,24,OPT_CENTERX,"Better play me on MSVC");
#endif

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
#if 0
	/* enable this to debug the bitmap location */
	while (1) {}
#else
	while(1){
		
		sensitive++;
	
		/* detect if touch happen */
		Touch = HAL_Read8(REG_TOUCH_TAG);
		if (Touch  && Touch != 0xFF && Touch != Tag){
			/* avoid too sensitive */
			if (sensitive && sensitive < TOUCH_SENCE) {
				continue;			
			}
			/* single click would select the icon, and de-select previous one if it has*/
			if (Tag) {
				deselect(&ani_bitmap[Tag - 1], Tag);
			}
			Tag = Touch;
			ani_index = '0';
			sensitive = 0;
			speed = 0;
		} else if (Touch && Touch == Tag) {
			/* avoid too sensitive */
			if (sensitive && sensitive < TOUCH_SENCE) {
				continue;			
			}
			/* double click would de-select the icon */
			deselect(&ani_bitmap[Tag - 1], Tag);
			Tag = 0;
			sensitive = 0;
			speed = 0;
		}

		if (Tag) {

			/* slow down the animation */
			if (speed++%ANIMATION_SPEED) {
				continue;			
			}
			/* change to proper animation file name for the appFileToRamG */
			changename(&ani_bitmap[Tag-1],Tag,getposition((FTC8 *)ani_bitmap[Tag-1].path),&ani_index);
			/* flash the cutted animation peices to it's proper location */
			/* Arduino: as limitation of the platform (Arduino Pro) I have on hand
			 * appFileToRamG is read the file in SD Card, and copy data to EVE block by block each time
			 * the speed and the performance is really slow, for your system
			 * please copy the proper file into your RAM then flash the animation */
			if (0 == appFileToRamG((FTC8 *)ani_bitmap[Tag-1].path,ani_addr[Tag-1],0,0,0)) {
				DBGPRINT;
				return;
			}
		}
	}
#endif
}

AppFunc APPS_UI[] = {
	playpuzzle
};

