/* 
    Sample code to demonstrate 1200x280 UI
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Oct
*/

#if defined(VC_EMULATOR)
#error "copy res/longlcd/bt81x.flash to res/flash, then comment this line"
#elif defined(VC_MPSSE) || defined(VC_FT4222)
#error "program res/longlcd/bt81x.flash to on-board flash, then comment this line"
#endif 

#define PATH_MENU1      "FLASH@4096  : 1344"
#define PATH_MENU2      "FLASH@5440  : 13824"
#define PATH_MENU3      "FLASH@19264 : 2048"
#define PATH_MASK       "FLASH@21312  : 268800"
#define PATH_MENU4_1    "FLASH@290112 : 62400"
#define PATH_MENU4_2    "FLASH@352512 : 62400"
#define PATH_MENU4_3    "FLASH@414912 : 62400"
#define PATH_MENU4_4    "FLASH@477312 : 62400"
#define PATH_MENU4_5    "FLASH@539712 : 62400"
#define PATH_MENU4_6    "FLASH@602112 : 62400"
#define PATH_MENU4_7    "FLASH@664512 : 62400"

#define MENU1_W 28
#define MENU1_H 24
#define MENU2_W 128
#define MENU2_H 108
#define MENU3_W 28
#define MENU3_H 24

#define MENU4_W   240
#define MENU4_H   260
#define MENU4_X   124
#define MENU4_Y   10
#define MENU4_GAP 10

#define MASK_W 1200
#define MASK_H 280

typedef enum MENU_INX_ {
    INX_MENU1 = 0,
    INX_MENU2,
    INX_MENU3,
    INX_MASK,
    INX_MENU4_1,
    INX_MENU4_2,
    INX_MENU4_3,
    INX_MENU4_4,
    INX_MENU4_5,
    INX_MENU4_6,
    INX_MENU4_7,
} MENU_INX_ENUM;

ImgInfo_st info_hdr[] = {
    {PATH_MENU1,0,0,0},
    {PATH_MENU2,0,0,0},
    {PATH_MENU3,0,0,0},
    {PATH_MASK,0,0,0},
    {PATH_MENU4_1,0,0,0},
    {PATH_MENU4_2,0,0,0},
    {PATH_MENU4_3,0,0,0},
    {PATH_MENU4_4,0,0,0},
    {PATH_MENU4_5,0,0,0},
    {PATH_MENU4_6,0,0,0},
    {PATH_MENU4_7,0,0,0},
};

bmpHDR_st bmp_hdr_m[] = {
	{COMPRESSED_RGBA_ASTC_4x4_KHR,MENU1_W,MENU1_H,(FTU32)&info_hdr[INX_MENU1]},
	{COMPRESSED_RGBA_ASTC_4x4_KHR,MENU2_W,MENU2_H,(FTU32)&info_hdr[INX_MENU2]},
	{COMPRESSED_RGBA_ASTC_4x4_KHR,MENU3_W,MENU3_H,(FTU32)&info_hdr[INX_MENU3]},
	{COMPRESSED_RGBA_ASTC_5x4_KHR,MASK_W,MASK_H,(FTU32)&info_hdr[INX_MASK]},
	{COMPRESSED_RGBA_ASTC_4x4_KHR,MENU4_W,MENU4_H,(FTU32)&info_hdr[INX_MENU4_1]},
	{COMPRESSED_RGBA_ASTC_4x4_KHR,MENU4_W,MENU4_H,(FTU32)&info_hdr[INX_MENU4_2]},
	{COMPRESSED_RGBA_ASTC_4x4_KHR,MENU4_W,MENU4_H,(FTU32)&info_hdr[INX_MENU4_3]},
	{COMPRESSED_RGBA_ASTC_4x4_KHR,MENU4_W,MENU4_H,(FTU32)&info_hdr[INX_MENU4_4]},
	{COMPRESSED_RGBA_ASTC_4x4_KHR,MENU4_W,MENU4_H,(FTU32)&info_hdr[INX_MENU4_5]},
	{COMPRESSED_RGBA_ASTC_4x4_KHR,MENU4_W,MENU4_H,(FTU32)&info_hdr[INX_MENU4_6]},
	{COMPRESSED_RGBA_ASTC_4x4_KHR,MENU4_W,MENU4_H,(FTU32)&info_hdr[INX_MENU4_7]},
};

FTU8 FileToEVE(bmpHDR_st *p_res, FTU8 num)
{
    if (appFlashSetFull()) {
        FTPRINT("\nflash error occur");
        return 1;
    }

    if(appBmpToRamG(INX_MENU1, RAM_G, p_res, num)){
        FTPRINT("\nload bitmap error occur");
        return 1;
    }

    return 0;
}

FTVOID ui_mask(FTVOID)
{
	HAL_CmdBufIn(BEGIN(BITMAPS));
	HAL_CmdBufIn(BITMAP_HANDLE(info_hdr[INX_MASK].handle));
	HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(VERTEX2F(0, 0));
}

FTVOID ui_move(FTU16 x_in)
{
#define MENU4_NUM       7
#define MENU4_LEFT_MAX  (MENU4_X - (MENU4_W + MENU4_GAP))
#define MENU4_PIXEL_UNIT 8
    static FTU16 preX = 0;
    static FT16  offset = 0, initX = 10;
    FT16 i, j;

    if (0x8000 & x_in) {
        preX = 0;
        offset = 0;
    } else if (preX == 0) {
        preX = x_in;
        offset = 0;
    } else {
        offset = x_in - preX;
        preX = x_in;
    }

    initX += offset;
    HAL_CmdBufIn(SAVE_CONTEXT());
    /* make the VERTEX2F range -2048 to 2047 */
    HAL_CmdBufIn(VERTEX_FORMAT(3));
	HAL_CmdBufIn(BEGIN(BITMAPS));
    
    for (i = 0;i < MENU4_NUM;i++) {
        j = initX + MENU4_X + i*MENU4_GAP + i*MENU4_W;
        /* don't need to show the invisable part */
        if (j > EVE_LCD_WIDTH) {
            break;
        } else if (j < MENU4_LEFT_MAX) {
            continue;
        }
	    HAL_CmdBufIn(BITMAP_HANDLE(info_hdr[INX_MENU4_1 + i].handle));
	    HAL_CmdBufIn(CELL(0));
        HAL_CmdBufIn(VERTEX2F(j*MENU4_PIXEL_UNIT, MENU4_Y*MENU4_PIXEL_UNIT));
    }
    HAL_CmdBufIn(RESTORE_CONTEXT());
}

FTVOID ui_unchange(FTVOID)
{
    HAL_CmdBufIn(SAVE_CONTEXT());
    HAL_CmdBufIn(COLOR_RGB(33,32,38));
    HAL_CmdBufIn(BEGIN(RECTS));
    HAL_CmdBufIn(VERTEX2F(0, 0));
    HAL_CmdBufIn(VERTEX2F(MENU4_X*EVE_PIXEL_UNIT, EVE_LCD_HIGH*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(RESTORE_CONTEXT());

	HAL_CmdBufIn(BEGIN(BITMAPS));
	HAL_CmdBufIn(BITMAP_HANDLE(info_hdr[INX_MENU1].handle));
	HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(VERTEX2F(20*EVE_PIXEL_UNIT, 10*EVE_PIXEL_UNIT));
	HAL_CmdBufIn(CELL(1));
    HAL_CmdBufIn(VERTEX2F(78*EVE_PIXEL_UNIT, 10*EVE_PIXEL_UNIT));
	HAL_CmdBufIn(BITMAP_HANDLE(info_hdr[INX_MENU2].handle));
	HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(VERTEX2F(0, 40*EVE_PIXEL_UNIT));
	HAL_CmdBufIn(BITMAP_HANDLE(info_hdr[INX_MENU3].handle));
	HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(VERTEX2F(26*EVE_PIXEL_UNIT, 180*EVE_PIXEL_UNIT));
	HAL_CmdBufIn(CELL(1));
    HAL_CmdBufIn(VERTEX2F(54*EVE_PIXEL_UNIT, 180*EVE_PIXEL_UNIT));
	HAL_CmdBufIn(CELL(2));
    HAL_CmdBufIn(VERTEX2F(80*EVE_PIXEL_UNIT, 180*EVE_PIXEL_UNIT));

    HAL_CmdBufIn(SAVE_CONTEXT());
    HAL_CmdBufIn(COLOR_RGB(56,55,60));
    HAL_CmdBufIn(BEGIN(LINES));
    HAL_CmdBufIn(VERTEX2F(7*EVE_PIXEL_UNIT, 240*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(VERTEX2F(112*EVE_PIXEL_UNIT, 240*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(RESTORE_CONTEXT());
}

FTU16 simulate_x(FTVOID)
{
#define MOVE_DELAY 5
#define MOVE_STEP  10
#define MOVE_LEN   ((EVE_LCD_WIDTH - MENU4_X)/2)
#define MOVE_MAX   ((MENU4_W*7 + MENU4_GAP*6) - (EVE_LCD_WIDTH - MENU4_X))
#define MOVE_START (MENU4_X + MOVE_LEN)
	static FTU8 delay = 0, mark = 1;
	static FTU16 dir = 0, X = 0x8000, i = 0;
    
    /* make it not move too fast */
    if (MOVE_DELAY >= ++delay) {
        return X;
    }
    delay = 0;
    if (X & 0x8000) {
        X = MOVE_START;
    }

    /* consider MOVE_LEN < MOVE_MAX */
    if (MOVE_STEP*i >= MOVE_LEN && mark) {
        X = 0x8000;
        mark = 0;
        return X;
    }

    if (MOVE_STEP*i >= MOVE_MAX) {
        dir = dir?0:1;
        X = 0x8000;
        i = 0;
        mark = 1;
        return X;
    }

    X += dir?(MOVE_STEP):(-1*MOVE_STEP);

    i++;

    return X;
}

FTVOID long_panel(FTU32 para)
{
#define MASK_PERIOD 100
	static FTU8 load = 1, mask = 0, mask_tick = 0;

    if (load) {
        if(FileToEVE(bmp_hdr_m, sizeof(bmp_hdr_m)/sizeof(bmpHDR_st))) {
            FTPRINT("\nFileToEVE fail");
            return;
        }
        load = 0;
    }

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1, 1, 1));
    
    ui_move(simulate_x());
    ui_unchange();
    if (mask) {
        ui_mask();
    }

    HAL_CmdBufIn(END());
	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

	HAL_BufToReg(RAM_CMD, 0);

    if (MASK_PERIOD <= mask_tick++) {
        mask = mask?0:1;
        mask_tick = 0;
    }

    appGP.appIndex = 0;
}

AppFunc APPS_UI[] = {
	long_panel,
};

