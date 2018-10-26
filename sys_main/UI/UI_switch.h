/* 
    Sample code to demonstrate impliment a switch UI
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Oct
*/

#if defined(VC_EMULATOR)
#error "copy res/switch/bt81x.flash to res/flash, then comment this line"
#elif defined(VC_MPSSE) || defined(VC_FT4222)
#error "program res/switch/bt81x.flash to on-board flash, then comment this line"
#endif 

#define UI1_BKGN_INX  "FLASH@155392:384000"
#define UI2_BKGN_INX  "FLASH@539392:384000"
#define UI3_BKGN_INX  "FLASH@923392:384000"
#define UI1_BKGN_LUT  "FLASH@1307392:1024"
#define UI2_BKGN_LUT  "FLASH@1308416:1024"
#define UI3_BKGN_LUT  "FLASH@1309440:640"

#define UI1_BUTN_INX  "FLASH@4096:28480"
#define UI2_BUTN_INX  "FLASH@32576:16384"
#define UI3_BUTN_INX  "FLASH@48960:103360"
#define UI1_BUTN_LUT  "FLASH@152320:1024"
#define UI2_BUTN_LUT  "FLASH@153344:1024"
#define UI3_BUTN_LUT  "FLASH@154368:1024"

#define UI1_BUTN_W 146
#define UI1_BUTN_H 195
#define UI2_BUTN_W 122
#define UI2_BUTN_H 134
#define UI3_BUTN_W 123
#define UI3_BUTN_H 168

typedef struct butn_ {
    FT16 X;
    FT16 Y;
    FTU8 stat;
}butn_st;

typedef enum UI_INX_ {
    INX_UI1_BKG = 0,
    INX_UI2_BKG,
    INX_UI3_BKG,
    INX_UI1_BUTN,
    INX_UI2_BUTN,
    INX_UI3_BUTN,
} UI_INX_ENUM;

ImgInfoPal_st info_pal_hdr[] = {
    {UI1_BKGN_INX,UI1_BKGN_LUT,0,0,0,0},
    {UI2_BKGN_INX,UI2_BKGN_LUT,0,0,0,0},
    {UI3_BKGN_INX,UI3_BKGN_LUT,0,0,0,0},

    {UI1_BUTN_INX,UI1_BUTN_LUT,0,0,0,0},
    {UI2_BUTN_INX,UI2_BUTN_LUT,0,0,0,0},
    {UI3_BUTN_INX,UI3_BUTN_LUT,0,0,0,0},
};
bmpHDR_st bmp_hdr_m[] = {
	{PALETTED8,EVE_LCD_WIDTH,EVE_LCD_HIGH,(FTU32)&info_pal_hdr[INX_UI2_BKG] },
    {PALETTED8,UI2_BUTN_W,UI2_BUTN_H,(FTU32)&info_pal_hdr[INX_UI2_BUTN] },
};
bmpHDR_st bmp_hdr_l[] = {
	{PALETTED8,EVE_LCD_WIDTH,EVE_LCD_HIGH,(FTU32)&info_pal_hdr[INX_UI2_BKG] },
    {PALETTED8,UI2_BUTN_W,UI2_BUTN_H,(FTU32)&info_pal_hdr[INX_UI2_BUTN] },
    {PALETTED8,EVE_LCD_WIDTH,EVE_LCD_HIGH,(FTU32)&info_pal_hdr[INX_UI1_BKG]},
    {PALETTED8,UI1_BUTN_W,UI1_BUTN_H,(FTU32)&info_pal_hdr[INX_UI1_BUTN]},
};
bmpHDR_st bmp_hdr_r[] = {
	{PALETTED8,EVE_LCD_WIDTH,EVE_LCD_HIGH,(FTU32)&info_pal_hdr[INX_UI2_BKG] },
    {PALETTED8,UI2_BUTN_W,UI2_BUTN_H,(FTU32)&info_pal_hdr[INX_UI2_BUTN] },
    {PALETTED8,EVE_LCD_WIDTH,EVE_LCD_HIGH,(FTU32)&info_pal_hdr[INX_UI3_BKG]},
    {PALETTED8,UI3_BUTN_W,UI3_BUTN_H,(FTU32)&info_pal_hdr[INX_UI3_BUTN]},
};
#define BUTN1_NUM (10)
#define BUTN2_NUM (8)
#define BUTN3_NUM (5)
#define BUTN3_LINE (2)
butn_st butn1[BUTN1_NUM] = {
    {24,14,0},  {176,14,0},  {328,14,0},  {478,14,0},  {630,14,0},
    {24,225,0}, {176,225,0}, {328,225,0}, {479,225,0}, {630,225,0},
};
butn_st butn2[BUTN2_NUM] = {
    {39,34,0},  {240,34,0},  {440,34,0},  {640,34,0},
    {39,254,0}, {240,254,0}, {440,254,0}, {640,254,0}
};
butn_st butn3[BUTN3_NUM*BUTN3_LINE] = {
    {40,38,0},  {190,38,0},  {338,38,0},  {487,38,0}, {636,38,0},
    {40,250,0}, {190,250,0}, {338,250,0}, {487,250,0}, {636,250,0}
};

FTU8 FileToEVE(bmpHDR_st *p_res, FTU8 num)
{
    if (appFlashSetFull()) {
        FTPRINT("\nflash error occur");
        return 1;
    }

    if(appBmpToRamG(INX_UI1_BKG, RAM_G, p_res, num)){
        FTPRINT("\nload bitmap error occur");
        return 1;
    }

    return 0;
}

FTVOID x_operation(FT16 X, FT16 * pX_middle)
{
#define TCH_DELAY 8
#define OFT_DELAY 15
    static FT16 seed, t, move;
    FT16 x_middle = *pX_middle, offset;

    if (0x8000 & X) {
    /* no touch */
        t = 0;
        move = 0;
        if (x_middle >= EVE_LCD_WIDTH/2) {
            x_middle = EVE_LCD_WIDTH;
        } else if (x_middle < ((-1) * EVE_LCD_WIDTH/2)) {
            x_middle = (-1) * EVE_LCD_WIDTH;
        } else {
            x_middle = 0;
        }
    } else {
    /* touch */
        if (t == TCH_DELAY) {
            offset = X - seed;
            if (!move) {
                if (offset > OFT_DELAY || offset < (-1)*OFT_DELAY) {
                    move = 1;
                } else {
                    offset = 0;
                }
            }
        } else {
            t++;
            offset = 0;
        }
        seed = X;
        x_middle += offset;

        if (x_middle <= (-1)*EVE_LCD_WIDTH || x_middle >= EVE_LCD_WIDTH) {
			return;
        }
    }

    *pX_middle = x_middle;
}
FTVOID switch_ui(FTU8 ui, FT16 X, FTU8 t)
{
    static FT16 preX[3] = {0};
    static FTU8 preT[3];
    butn_st *p;
    FTU8 i = ui - 1, j, k, tag = t;
    FTU16 w, h, n;

    switch (ui) {
        case 1:
            p = butn1;
            j = INX_UI1_BKG;
            k = INX_UI1_BUTN;
            w = UI1_BUTN_W;
            h = UI1_BUTN_H;
            n = BUTN1_NUM;
            break;
        case 2:
            p = butn2;
            j = INX_UI2_BKG;
            k = INX_UI2_BUTN;
            w = UI2_BUTN_W;
            h = UI2_BUTN_H;
            n = BUTN2_NUM;
            break;
        case 3:
            p = butn3;
            j = INX_UI3_BKG;
            k = INX_UI3_BUTN;
            w = UI3_BUTN_W;
            h = UI3_BUTN_H;
            n = BUTN3_NUM*BUTN3_LINE;
            break;
        default:
            break;
    }

    if (preX[i] != X) {
        preX[i] = X;
        preT[i] = 0;
        tag = 0;
    } else {
        if (tag > 0 && tag <= n) {
            preT[i] = tag;
            tag = 0;
        } else if (tag == 0) {
            tag = preT[i]; 
            preT[i] = 0; 
        } else {
            tag = 0;
            preT[i] = 0; 
        }
    }

	HAL_CmdBufIn(BEGIN(BITMAPS));
    appDispPalette8(X, 0, info_pal_hdr[j].addr_lut, info_pal_hdr[j].handle, 0);

    i = 0;
    while (i < n) {
		if (X >=0 && X < EVE_LCD_WIDTH) {
			//only when in the viable area (0 ~ EVE_LCD_WIDTH) need to change record 
            if (tag == i + 1) {
                p[i].stat = (p[i].stat)?0:1;
            }
			//only when in the viable area (0 ~ EVE_LCD_WIDTH) need to set a touch tag
			HAL_CmdBufIn(COLOR_MASK(0, 0, 0, 0));//make the invisable retengle for tag
			HAL_CmdBufIn(BEGIN(RECTS));
			HAL_CmdBufIn(TAG_MASK(1));
			HAL_CmdBufIn(TAG(i + 1));
			HAL_CmdBufIn(VERTEX2F((X + p[i].X)*EVE_PIXEL_UNIT, p[i].Y*EVE_PIXEL_UNIT));
			HAL_CmdBufIn(VERTEX2F((X + p[i].X + w)*EVE_PIXEL_UNIT, (p[i].Y + h)*EVE_PIXEL_UNIT));
			HAL_CmdBufIn(TAG_MASK(0));
			HAL_CmdBufIn(COLOR_MASK(1, 1, 1, 1));
		}
		/*
          even not in the viable area (0 ~ EVE_LCD_WIDTH)
          still need to show button
         */
        if (p[i].stat) { 
			HAL_CmdBufIn(BEGIN(BITMAPS));
		    appDispPalette8(X + p[i].X, p[i].Y, 
                            info_pal_hdr[k].addr_lut, 
                            info_pal_hdr[k].handle, 
                            (k == INX_UI3_BUTN)?(i%BUTN3_NUM):0);
        }
        i++;
    }
}
FTVOID load_res(FT16 x_m)
{
	static FTU8 load_l = 1, load_r = 1, load_m = 1;

    if (x_m > 0) {
        if (load_l) {
            if(FileToEVE(bmp_hdr_l, sizeof(bmp_hdr_l)/sizeof(bmpHDR_st))) {
                return;
            }
            load_l = 0;
            load_r = 1;
            load_m = 1;
        }
    } else if (x_m < 0) {
        if (load_r) {
            if(FileToEVE(bmp_hdr_r, sizeof(bmp_hdr_r)/sizeof(bmpHDR_st))) {
                return;
            }
            load_r = 0;
            load_l = 1;
            load_m = 1;
        }
    } else {
         if (load_m) {
            if(FileToEVE(bmp_hdr_m, sizeof(bmp_hdr_m)/sizeof(bmpHDR_st))) {
                return;
            }
            load_m = 0;
            load_r = 1;
            load_l = 1;
        }
    }
}
FTVOID switch_panel(FTU32 para)
{
	static FTU8 t_delay = 0;
    static FT16 x_middle;
    FTU8 t = HAL_Read8(REG_TOUCH_TAG);
   
    x_operation(HAL_Read32(REG_TOUCH_SCREEN_XY) >> 16, &x_middle);

    load_res(x_middle);

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255, 255, 255));
	HAL_CmdBufIn(CLEAR(1, 1, 1));

    if (x_middle > 0) {
        switch_ui(2, x_middle, t);
        switch_ui(1, x_middle - EVE_LCD_WIDTH, t);
    } else if (x_middle < 0) {
        switch_ui(2, x_middle, t);
        switch_ui(3, x_middle + EVE_LCD_WIDTH, t);
    } else {
        switch_ui(2, x_middle, t);
    }
    
    HAL_CmdBufIn(END());
	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

	HAL_BufToReg(RAM_CMD, 0);

    appGP.appIndex = 0;
}

AppFunc APPS_UI[] = {
	switch_panel,
};

