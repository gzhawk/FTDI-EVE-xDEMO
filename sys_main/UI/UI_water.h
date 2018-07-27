/* 
    Sample code to demonstrate impliment a water machine UI
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Jun
*/

#if defined(VC_EMULATOR)
#error "copy res/water/bt81x.flash to res/flash, then comment this line"
#elif defined(VC_MPSSE)
#error "program res/water/bt81x.flash to on-board flash, then comment this line"
#endif

#define BKGRD_PATH         "FLASH:30976"
#define BKGRD_X            0
#define BKGRD_Y            0

#define BAR_PATH           "FLASH:4096"
#define BAR_X              0
#define BAR_Y              0

#define NUM_PATH           "FLASH:219776"
#define NUM_X              13
#define NUM_Y              69

#define TUBE_ANIM_ADDR     810368
#define TUBE_ANIM_FRAM     24
#define TUBE_W             222
#define TUBE_H             134
#define TUBE_X             255
#define TUBE_Y             69

#define DOWN_W             128
#define DOWN_H             64
#define DOWN_1_PATH        "FLASH:211584"
#define DOWN_1_ANIM_ADDR   211456
#define DOWN_1_ANIM_FRAM   10
#define DOWN_1_X           34
#define DOWN_1_Y           203

#define UP_W               48
#define UP_H               52
#define UP_1_PATH          "FLASH:860352"
#define UP_1_ANIM_ADDR     860224
#define UP_1_ANIM_FRAM     10
#define UP_1_X             144
#define UP_1_Y             0

#define SUB_UP_PATH        "FLASH:379904"
#define SUB_DOWN_PATH      "FLASH:249344"

/* I don't have 480x272 screen on my hand now,
   so use 800x480 screen to demonstrate it
#ifdef EVE_LCD_WIDTH
#undef EVE_LCD_WIDTH
#define EVE_LCD_WIDTH 480
#endif

#ifdef EVE_LCD_HIGH
#undef EVE_LCD_HIGH
#define EVE_LCD_HIGH 272
#endif
*/

bmpHDR_st bmp_hdr_main[] = {
    {BKGRD_PATH,  0,0,COMPRESSED_RGBA_ASTC_4x4_KHR,0,0,EVE_LCD_WIDTH,EVE_LCD_HIGH},
    {BAR_PATH,    0,0,COMPRESSED_RGBA_ASTC_4x4_KHR,0,0,EVE_LCD_WIDTH,56},
    {NUM_PATH,    0,0,COMPRESSED_RGBA_ASTC_4x4_KHR,0,0,224,132},
    {DOWN_1_PATH, 0,0,COMPRESSED_RGBA_ASTC_4x4_KHR,0,0,DOWN_W,DOWN_H},
    {UP_1_PATH,   0,0,COMPRESSED_RGBA_ASTC_4x4_KHR,0,0,UP_W,UP_H},
    {SUB_UP_PATH, 0,0,COMPRESSED_RGBA_ASTC_4x4_KHR,0,0,EVE_LCD_WIDTH,EVE_LCD_HIGH},
    {SUB_DOWN_PATH,0,0,COMPRESSED_RGBA_ASTC_4x4_KHR,0,0,EVE_LCD_WIDTH,EVE_LCD_HIGH},
};

#define HDL_START   0
#define SUB_ID_UP   0xaa
#define SUB_ID_DOWN 0xbb

typedef enum ITEM_ {
    HDL_BKGRD = HDL_START,
    HDL_BAR,
    HDL_NUM,
    HDL_DOWN_1,
    HDL_UP_1,
    HDL_SUB_UP,
    HDL_SUB_DOWN,
} ITEM_ENUM;

typedef enum ITEM_TAG_ {
    TAG_DOWN_1 = 1,
    TAG_UP_1,
} ITEM_TAG_ENUM;

FTU8 load_resources(bmpHDR_st *p_res, FTU8 num)
{
    if (appFlashSetFull()) {
        FTPRINT("\nflash error occur");
        return 1;
    }

    if(appBmpToRamG(HDL_START, RAM_G, p_res, num)){
        FTPRINT("\nload bitmap error occur");
        return 1;
    }

    return 0;
}

FTVOID loop_frame(FTU32 *pframe, FTU32 max)
{
    if (*pframe >= max-1) {
        *pframe = 0;
    } else {
        *pframe = *pframe + 1;
    }
}

FTVOID water_machine_sub(FTU32 para)
{
    FTU8 go_back = 0;

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255, 255, 255));
	HAL_CmdBufIn(CLEAR(1, 1, 1));

    HAL_CmdBufIn(BEGIN(BITMAPS));

    if (SUB_ID_UP == para) 
        HAL_CmdBufIn(BITMAP_HANDLE(HDL_SUB_UP));
    if (SUB_ID_DOWN == para) 
        HAL_CmdBufIn(BITMAP_HANDLE(HDL_SUB_DOWN));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(VERTEX2F(0,0));
 	
    HAL_CmdBufIn(END());
	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

	HAL_BufToReg(RAM_CMD, 0);

    while (TOUCHED) {
        go_back = 1;
        appGP.appPara = 0;
    }

    appGP.appIndex = go_back?0:1;
}

FTVOID water_machine(FTU32 para)
{
	static FTU8 load = 1, slow_tub = 0, slow_up1 = 0, slow_down1 = 0;
	static FTU32 anim_tub = 0, anim_up1 = 0, anim_down1 = 0;
    FTU8 tag = TOUCHED?HAL_Read8(REG_TOUCH_TAG):0;

	if (load) {
        if(load_resources(bmp_hdr_main, sizeof(bmp_hdr_main)/sizeof(bmpHDR_st))) {
            return;
        }
        load = 0;
	}

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255, 255, 255));
	HAL_CmdBufIn(CLEAR(1, 1, 1));

    HAL_CmdBufIn(BEGIN(BITMAPS));

    /* main UI background */
    HAL_CmdBufIn(BITMAP_HANDLE(HDL_BKGRD));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(VERTEX2F(BKGRD_X,BKGRD_Y));
    
    /* main UI bar */
    HAL_CmdBufIn(BITMAP_HANDLE(HDL_BAR));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(VERTEX2F(BAR_X*EVE_PIXEL_UNIT,BAR_Y*EVE_PIXEL_UNIT));
   
    /* main UI number */
    HAL_CmdBufIn(BITMAP_HANDLE(HDL_NUM));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(VERTEX2F(NUM_X*EVE_PIXEL_UNIT,NUM_Y*EVE_PIXEL_UNIT));

    /* main UI down 1 */
    HAL_CmdBufIn(TAG_MASK(1));
    HAL_CmdBufIn(TAG(TAG_DOWN_1));
    if (TAG_DOWN_1 == tag) {
        CoCmd_ANIMFRAME(DOWN_1_X+DOWN_W/2+8, DOWN_1_Y+DOWN_H/2+4, DOWN_1_ANIM_ADDR, anim_down1);
        if (++slow_down1 % 3 == 0) {
            loop_frame(&anim_down1, DOWN_1_ANIM_FRAM);
        }
        appGP.appPara = SUB_ID_DOWN;
    } else {
        HAL_CmdBufIn(BITMAP_HANDLE(HDL_DOWN_1));
        HAL_CmdBufIn(CELL(0));
        HAL_CmdBufIn(VERTEX2F(DOWN_1_X*EVE_PIXEL_UNIT,DOWN_1_Y*EVE_PIXEL_UNIT));
    }
    HAL_CmdBufIn(TAG_MASK(0));
    
    /* main UI up 1 */
    HAL_CmdBufIn(TAG_MASK(1));
    HAL_CmdBufIn(TAG(TAG_UP_1));
    if (TAG_UP_1 == tag) {
        CoCmd_ANIMFRAME(UP_1_X+UP_W/2, UP_1_Y+UP_H/2+10, UP_1_ANIM_ADDR, anim_up1);
        if (++slow_up1 % 3 == 0) {
            loop_frame(&anim_up1, UP_1_ANIM_FRAM);
        }
        appGP.appPara = SUB_ID_UP;
    } else {
        HAL_CmdBufIn(BITMAP_HANDLE(HDL_UP_1));
        HAL_CmdBufIn(CELL(0));
        HAL_CmdBufIn(VERTEX2F(UP_1_X*EVE_PIXEL_UNIT,UP_1_Y*EVE_PIXEL_UNIT));
    }
    HAL_CmdBufIn(TAG_MASK(0));

    /* animation parts */
	CoCmd_ANIMFRAME(TUBE_X+TUBE_W/2, TUBE_Y+TUBE_H/2, TUBE_ANIM_ADDR, anim_tub);
    if (++slow_tub % 8 == 0) {
        loop_frame(&anim_tub, TUBE_ANIM_FRAM);
    }

	HAL_CmdBufIn(END());
	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

	HAL_BufToReg(RAM_CMD, 0);

    if (tag) {
        appGP.appIndex = 0;
    } else {
        appGP.appIndex = appGP.appPara?1:0;
    }
}

AppFunc APPS_UI[] = {
	water_machine,
	water_machine_sub,
};

