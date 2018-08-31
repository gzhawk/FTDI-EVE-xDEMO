/* 
    Demo for AUPU
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2017/Aug
*/
#define ROTATE_VALUE  2
#define PATH_BKG_C0   ROOT_PATH"aupu\\bg_c0.raw"
#define PATH_BKG_C1   ROOT_PATH"aupu\\bg_c1.raw"
#define PATH_BKG_B0   ROOT_PATH"aupu\\bg_b0.raw"
#define PATH_BKG_B1   ROOT_PATH"aupu\\bg_b1.raw"

#define PATH_TXT_F    ROOT_PATH"aupu\\txt.raw"

#define PATH_NUMBER   ROOT_PATH"aupu\\num.raw"
#define PATH_LEFT_I   ROOT_PATH"aupu\\left_index.bin"
#define PATH_LEFT_L   ROOT_PATH"aupu\\left_lut.bin"
#define PATH_HOME_I   ROOT_PATH"aupu\\home_index.bin"
#define PATH_HOME_L   ROOT_PATH"aupu\\home_lut.bin"
#define PATH_OK_I     ROOT_PATH"aupu\\ok_index.bin"
#define PATH_OK_L     ROOT_PATH"aupu\\ok_lut.bin"
#define PATH_CLOCK_I  ROOT_PATH"aupu\\clock_index.bin"
#define PATH_CLOCK_L  ROOT_PATH"aupu\\clock_lut.bin"
#define PATH_MASK_I   ROOT_PATH"aupu\\mask_index.bin"
#define PATH_MASK_L   ROOT_PATH"aupu\\mask_lut.bin"
#define PATH_MASKD_I   ROOT_PATH"aupu\\mask_d_index.bin"
#define PATH_MASKD_L   ROOT_PATH"aupu\\mask_d_lut.bin"

#define LCD_W         480
#define LCD_H         320


#define MASK_W        1
#define MASK_H        30
#define NUMBER_W      36
#define NUMBER_H      41
#define LEFT_W        43
#define LEFT_H        46
#define HOME_W        38
#define HOME_H        32
#define OK_W          29
#define OK_H          32
#define CLOCK_W       200
#define CLOCK_H       200
#define CLOCK_RI      98

#define BKG_R         22
#define BKG_G         168
#define BKG_B         168

#define BUTM_R        39
#define BUTM_G        93
#define BUTM_B        93
#define BUTM_W        ((LCD_W-10)/2)
#define BUTM_H        (47)

#define DATE_POS_NUM    (5) /* up 2, middle 1, down 2 */

#define DATE_RATIO_MAX  (100)
#define DATE_RATIO_MIN  (20)

#define DATE_NUM_H  30
#define DATE_NUM_Y  74

#define DATEYEAR_W  98
#define DATEYEAR_X  98
#define DATEMONTH_X 224
#define DATEMONTH_W 45
#define DATEDAY_X   314
#define DATEDAY_W   51

#define DATE_NUM_UP_INVIS_Y   (DATE_NUM_Y-DATE_NUM_H/2)
#define DATE_NUM_UP_2_Y       DATE_NUM_Y
#define DATE_NUM_UP_1_Y       (DATE_NUM_UP_2_Y+DATE_NUM_H-4)
#define DATE_NUM_MID_Y        (DATE_NUM_UP_1_Y+DATE_NUM_H)
#define DATE_NUM_DOWN_1_Y     (DATE_NUM_MID_Y+DATE_NUM_H)
#define DATE_NUM_DOWN_2_Y     (DATE_NUM_DOWN_1_Y+DATE_NUM_H-4)
#define DATE_NUM_DOWN_INVIS_Y (DATE_NUM_DOWN_2_Y+DATE_NUM_H/2)
#define DATE_H                (DATE_NUM_DOWN_INVIS_Y-DATE_NUM_UP_2_Y)

#define TRACK_MAX   65536

typedef enum HDL_ {
    HDL_NUMBER = 0, 
    HDL_LEFT, 
    HDL_HOME, 
    HDL_OK,
    HDL_CLOCK,
    HDL_MASK,
    HDL_MASK_D,
    /* same order in imgHeader */
    HDL_BKG_START,
    /* font and bitmap share the same handle,
       and DXT1 need to use two handle */
    HDL_FONT    = HDL_BKG_START+2, 
    HDL_MP_F, 
} HDL_e;

#define IMG_NUMBER    HDL_BKG_START

typedef enum TAG_ {
    TAG_HOME       = 1,
    TAG_OK,
    TAG_LEFT,
    TAG_RIGHT,
    TAG_CLOCK,
    TAG_CLKOPEN,
    TAG_CLKCLOSE,
    TAG_YEAR,
    TAG_MONTH,
    TAG_DAY
} TAG_e;

/* !!!Attention: the items order MUST be the same as HDL_e */
bmpHDR_st imgHeader[IMG_NUMBER] = {
    {PATH_NUMBER,0,0,L8,      0,0,NUMBER_W,NUMBER_H},
    {PATH_LEFT_I,PATH_LEFT_L,0,PALETTED4444,0,0,LEFT_W,LEFT_H},
    {PATH_HOME_I,PATH_HOME_L,0,PALETTED4444,0,0,HOME_W,HOME_H},
    {PATH_OK_I,PATH_OK_L,0,PALETTED4444,0,0,OK_W,OK_H},
    {PATH_CLOCK_I,PATH_CLOCK_L,0,PALETTED4444,0,0,CLOCK_W,CLOCK_H},
    {PATH_MASK_I,PATH_MASK_L,0,PALETTED8,0,0,MASK_W,MASK_H},
    {PATH_MASKD_I,PATH_MASKD_L,0,PALETTED8,0,0,MASK_W,MASK_H},
    };

typedef struct DateTime_  {
    FTU32 year;
    FTU32 month;
    FTU32 day;
    FTU8  hour;
    FTU8  minute;
    FTU8  open;
}DateTime_st;

typedef struct DateDisp_  {
    FTU32 Y;
    FTU32 data;
}DateDisp_st;

FT_Gpu_Fonts_t stTxtFnt;
DateTime_st    stDateTime = {2017,8,22,16,17,1};
FTU32          ClkSetValue = 8*60+15;
FTU32          DatSetValue = 2017*10000+8*100+24;
DateDisp_st    stYear[DATE_POS_NUM] = {0};
DateDisp_st    stMonth[DATE_POS_NUM] = {0};
DateDisp_st    stDay[DATE_POS_NUM] = {0};
/* background image location */
FTU32          bg_start_addr  = 0;
/* record the last setting UI index, for re-entry use */
FTU32          last_index     = 1;
STATIC FTU32 loadTxtFont (FTVOID)
{
	FTU32 len;

	/* load the font resources into EVE, get related info out to variable */
	len = appFileToRamG((FTC8 *)PATH_TXT_F,RAM_G,0,(FTU8 *)&stTxtFnt,sizeof(FT_Gpu_Fonts_t));
	if (len == 0) {
		DBGPRINT;
	}

    return len;
}

STATIC FTU32 loadImg (FTU32 startAddr)
{
    /* load bitmap resources data into EVE, and set some parameter */
    if(APP_OK != appBmpToRamG(HDL_NUMBER, startAddr, imgHeader, IMG_NUMBER)){
        DBGPRINT;
        return 0;
    }
	return 1;
}

STATIC FTU32 loadBackground (FTU32 startAddr)
{
	FTU32 len, addr = startAddr;

    /* load the 4 necessary file to background photo */
	len = appFileToRamG((FTC8 *)PATH_BKG_C0,addr,0,0,0);
	if (len == 0) {
		DBGPRINT;
		return len;
	}
	addr += len;
	len = appFileToRamG((FTC8 *)PATH_BKG_C1,addr,0,0,0);
	if (len == 0) {
		DBGPRINT;
		return len;
	}
	addr += len;
	len = appFileToRamG((FTC8 *)PATH_BKG_B0,addr,0,0,0);
	if (len == 0) {
		DBGPRINT;
		return len;
	}
	addr += len;
	len = appFileToRamG((FTC8 *)PATH_BKG_B1,addr,0,0,0);
	if (len == 0) {
		DBGPRINT;
		return len;
	}
	addr += len;

	return addr;
}

STATIC FTU32 loadResources (FTVOID)
{
    FTU32 l = 0;

    l = loadTxtFont();
    if (l) {
        bg_start_addr = l;
        l += loadBackground(bg_start_addr);
        if (l > bg_start_addr) {
            return loadImg(l);
        }
        DBGPRINT;
        return 0;
    }
    DBGPRINT;
    return 0;
}

FTVOID dispPal8 (FTU32 X, FTU32 Y, FTU32 PalSrc, FTU32 hdl, FTU32 cell)
{
    /* every thing after this commands would not display
       if not use save/restore context */
    HAL_CmdBufIn(SAVE_CONTEXT());
    HAL_CmdBufIn(BLEND_FUNC(ONE, ZERO));
    HAL_CmdBufIn(COLOR_MASK(0,0,0,1));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 3));
    HAL_CmdBufIn(BITMAP_HANDLE(hdl));
    HAL_CmdBufIn(CELL(cell));
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

    HAL_CmdBufIn(BLEND_FUNC(DST_ALPHA, ONE_MINUS_DST_ALPHA));
    HAL_CmdBufIn(COLOR_MASK(1,0,0,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 2));
    HAL_CmdBufIn(BITMAP_HANDLE(hdl));
    HAL_CmdBufIn(CELL(cell));
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

    HAL_CmdBufIn(COLOR_MASK(0,1,0,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 1));
    HAL_CmdBufIn(BITMAP_HANDLE(hdl));
    HAL_CmdBufIn(CELL(cell));
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

    HAL_CmdBufIn(COLOR_MASK(0,0,1,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 0));
    HAL_CmdBufIn(BITMAP_HANDLE(hdl));
    HAL_CmdBufIn(CELL(cell));
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(RESTORE_CONTEXT());
}

STATIC FTVOID dxt1BitmapInfo (FTU8 startHdl, FTU32 startAddr, FTU16 W, FTU16 H)
{
#define DXT1_BLOCK_NUMS (4)
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl+1));
	HAL_CmdBufIn(BITMAP_SOURCE(startAddr));
	HAL_CmdBufIn(BITMAP_LAYOUT(RGB565, W/DXT1_BLOCK_NUMS*2, H/DXT1_BLOCK_NUMS));
    HAL_CmdBufIn(BITMAP_LAYOUT_H(W/DXT1_BLOCK_NUMS*2 >> 10, H/DXT1_BLOCK_NUMS >> 9));
	HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, W, H));
    HAL_CmdBufIn(BITMAP_SIZE_H(W>>9, H>>9));

	HAL_CmdBufIn(BITMAP_HANDLE(startHdl));
	HAL_CmdBufIn(BITMAP_SOURCE(startAddr + 2*(W/DXT1_BLOCK_NUMS*2*H/DXT1_BLOCK_NUMS)));
	HAL_CmdBufIn(BITMAP_LAYOUT(L1, W/DXT1_BLOCK_NUMS/2, H));
    HAL_CmdBufIn(BITMAP_LAYOUT_H(W/DXT1_BLOCK_NUMS/2 >> 10, H >> 9));
	HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, W, H));
    HAL_CmdBufIn(BITMAP_SIZE_H(W>>9, H>>9));
}

STATIC FTVOID dxt1FormatInfo (FTU8 startHdl, FT16 X, FT16 Y)
{
	HAL_CmdBufIn(BLEND_FUNC(ONE,ZERO));
	HAL_CmdBufIn(COLOR_A(0x55));
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl));
	HAL_CmdBufIn(CELL(0));
	HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

	HAL_CmdBufIn(BLEND_FUNC(ONE,ONE));
	HAL_CmdBufIn(COLOR_A(0xAA));
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl));
	HAL_CmdBufIn(CELL(1));
	HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

	HAL_CmdBufIn(COLOR_MASK(1,1,1,0));

	CoCmd_LOADIDENTITY;
	CoCmd_SCALE(4*EVE_TRANSFORM_MAX, 4*EVE_TRANSFORM_MAX);
	CoCmd_SETMATRIX;

	HAL_CmdBufIn(BLEND_FUNC(DST_ALPHA,ZERO));
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl+1));
	HAL_CmdBufIn(CELL(1));
	HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

	HAL_CmdBufIn(BLEND_FUNC(ONE_MINUS_DST_ALPHA,ONE));
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl+1));
	HAL_CmdBufIn(CELL(0));
	HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

}

STATIC FTVOID dxt1Info (FTU8 startHdl, FTU32 startAddr, FT16 X, FT16 Y, FTU16 W, FTU16 H)
{
	HAL_CmdBufIn(SAVE_CONTEXT());
	dxt1BitmapInfo(startHdl, startAddr, W, H);

	HAL_CmdBufIn(BEGIN(BITMAPS));

	dxt1FormatInfo(startHdl,X,Y);
	HAL_CmdBufIn(RESTORE_CONTEXT());
	
	HAL_CmdBufIn(END());
}

STATIC FTVOID dispBKG (FT32 addr)
{
    dxt1Info(HDL_BKG_START,addr,0,0,LCD_W,LCD_H);
}

STATIC FTVOID dispDateTime (FTVOID)
{
#define SHOW_TIME_YEAR_X 10
#define SHOW_TIME_HOUR_X 100
#define SHOW_TIME_W      8
    CoCmd_NUMBER(SHOW_TIME_YEAR_X,0,26,0,stDateTime.year);
    CoCmd_TEXT(SHOW_TIME_YEAR_X+4*SHOW_TIME_W+5,0,26,OPT_RIGHTX,"/");
    CoCmd_NUMBER(SHOW_TIME_YEAR_X+5*SHOW_TIME_W,0,26,2,stDateTime.month);
    CoCmd_TEXT(SHOW_TIME_YEAR_X+7*SHOW_TIME_W+5,0,26,OPT_RIGHTX,"/");
    CoCmd_NUMBER(SHOW_TIME_YEAR_X+8*SHOW_TIME_W,0,26,2,stDateTime.day);
    CoCmd_NUMBER(SHOW_TIME_HOUR_X,0,26,2,stDateTime.hour);
    CoCmd_TEXT(SHOW_TIME_HOUR_X+2*SHOW_TIME_W+5,0,26,OPT_RIGHTX,":");
    CoCmd_NUMBER(SHOW_TIME_HOUR_X+3*SHOW_TIME_W,0,26,2,stDateTime.minute);
}

/* HomeOK:0-Home, 1-OK; Pressed: 0-unpress, 1-press */
STATIC FTVOID dispButm (FTU8 HomeOK, FTU8 Pressed)
{
    FTU8 HomeR = 100, OKR = 100;

    if (Pressed) {
        if (HomeOK) {
            OKR = 90;
        } else {
            HomeR = 90;
        }
    }

    HAL_CmdBufIn(COLOR_RGB(BUTM_R,BUTM_G,BUTM_B));
	HAL_CmdBufIn(BEGIN(RECTS));
	HAL_CmdBufIn(VERTEX2F(0,(LCD_H-BUTM_H)*EVE_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(BUTM_W*EVE_PIXEL_UNIT,LCD_H*EVE_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F((LCD_W-BUTM_W)*EVE_PIXEL_UNIT,(LCD_H-BUTM_H)*EVE_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(LCD_W*EVE_PIXEL_UNIT,LCD_H*EVE_PIXEL_UNIT));
	HAL_CmdBufIn(END());


	HAL_CmdBufIn(TAG_MASK(1));
    HAL_CmdBufIn(BEGIN(BITMAPS));
    HAL_CmdBufIn(COLOR_RGB(0xFF,0xFF,0xFF));
    
	HAL_CmdBufIn(TAG(TAG_HOME));
    HAL_CmdBufIn(BITMAP_HANDLE(HDL_HOME));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(PALETTE_SOURCE(imgHeader[HDL_HOME].lut_src));
    HAL_CmdBufIn(SAVE_CONTEXT());
	CoCmd_LOADIDENTITY;
    CoCmd_TRANSLATE(HOME_W/2*EVE_TRANSFORM_MAX, HOME_H/2*EVE_TRANSFORM_MAX);
    CoCmd_SCALE(HomeR*EVE_TRANSFORM_MAX/100,HomeR*EVE_TRANSFORM_MAX/100);
    CoCmd_TRANSLATE(HOME_W/2*EVE_TRANSFORM_MAX*(-1), HOME_H/2*EVE_TRANSFORM_MAX*(-1));
	CoCmd_SETMATRIX;
	HAL_CmdBufIn(VERTEX2F(((BUTM_W-HOME_W)/2)*EVE_PIXEL_UNIT,
                         ((LCD_H-BUTM_H)+(BUTM_H-HOME_H)/2)*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(RESTORE_CONTEXT());
	
	HAL_CmdBufIn(TAG(TAG_OK));
    HAL_CmdBufIn(BITMAP_HANDLE(HDL_OK));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(PALETTE_SOURCE(imgHeader[HDL_OK].lut_src));
    HAL_CmdBufIn(SAVE_CONTEXT());
	CoCmd_LOADIDENTITY;
    CoCmd_TRANSLATE(HOME_W/2*EVE_TRANSFORM_MAX, HOME_H/2*EVE_TRANSFORM_MAX);
    CoCmd_SCALE(OKR*EVE_TRANSFORM_MAX/100,OKR*EVE_TRANSFORM_MAX/100);
    CoCmd_TRANSLATE(HOME_W/2*EVE_TRANSFORM_MAX*(-1), HOME_H/2*EVE_TRANSFORM_MAX*(-1));
	CoCmd_SETMATRIX;
	HAL_CmdBufIn(VERTEX2F(((LCD_W-BUTM_W)+(BUTM_W-OK_W)/2)*EVE_PIXEL_UNIT,
                         ((LCD_H-BUTM_H)+(BUTM_H-OK_H)/2)*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(RESTORE_CONTEXT());
    HAL_CmdBufIn(END());
	HAL_CmdBufIn(TAG_MASK(0));
}

STATIC FTVOID dispArrow (FTU8 LeftRight, FTU8 Pressed)
{
    FTU8 LeftR = 100, RightR = 100;

    if (Pressed) {
        if (LeftRight) {
            RightR = 90;
        } else {
            LeftR = 90;
        }
    }

	HAL_CmdBufIn(TAG_MASK(1));
    HAL_CmdBufIn(BEGIN(BITMAPS));
    HAL_CmdBufIn(COLOR_RGB(0xFF,0xFF,0xFF));
    
	HAL_CmdBufIn(TAG(TAG_LEFT));
    HAL_CmdBufIn(BITMAP_HANDLE(HDL_LEFT));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(PALETTE_SOURCE(imgHeader[HDL_LEFT].lut_src));
    HAL_CmdBufIn(SAVE_CONTEXT());
	CoCmd_LOADIDENTITY;
    CoCmd_TRANSLATE(LEFT_W/2*EVE_TRANSFORM_MAX, LEFT_H/2*EVE_TRANSFORM_MAX);
    CoCmd_SCALE(LeftR*EVE_TRANSFORM_MAX/100,LeftR*EVE_TRANSFORM_MAX/100);
    CoCmd_TRANSLATE(LEFT_W/2*EVE_TRANSFORM_MAX*(-1), LEFT_H/2*EVE_TRANSFORM_MAX*(-1));
	CoCmd_SETMATRIX;
	HAL_CmdBufIn(VERTEX2F(0*EVE_PIXEL_UNIT,
                         (LCD_H/2-(LEFT_H)/2)*EVE_PIXEL_UNIT));
	HAL_CmdBufIn(RESTORE_CONTEXT());

	HAL_CmdBufIn(TAG(TAG_RIGHT));
    HAL_CmdBufIn(BITMAP_HANDLE(HDL_LEFT));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(PALETTE_SOURCE(imgHeader[HDL_LEFT].lut_src));
    HAL_CmdBufIn(SAVE_CONTEXT());
	CoCmd_LOADIDENTITY;
    CoCmd_TRANSLATE(LEFT_W/2*EVE_TRANSFORM_MAX, LEFT_H/2*EVE_TRANSFORM_MAX);
    CoCmd_ROTATE(180*EVE_TRANSFORM_MAX/360);
    CoCmd_SCALE(RightR*EVE_TRANSFORM_MAX/100,RightR*EVE_TRANSFORM_MAX/100);
    CoCmd_TRANSLATE(LEFT_W/2*EVE_TRANSFORM_MAX*(-1), LEFT_H/2*EVE_TRANSFORM_MAX*(-1));
	CoCmd_SETMATRIX;
	HAL_CmdBufIn(VERTEX2F((LCD_W-LEFT_W)*EVE_PIXEL_UNIT,
                         (LCD_H/2-(LEFT_H)/2)*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(RESTORE_CONTEXT());
    HAL_CmdBufIn(END());
	HAL_CmdBufIn(TAG_MASK(0));
}

STATIC FTVOID dispClockBkg (FTVOID)
{

    HAL_CmdBufIn(BEGIN(BITMAPS));
    HAL_CmdBufIn(COLOR_RGB(0xFF,0xFF,0xFF));
    HAL_CmdBufIn(BITMAP_HANDLE(HDL_CLOCK));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(PALETTE_SOURCE(imgHeader[HDL_CLOCK].lut_src));
	HAL_CmdBufIn(VERTEX2F(((LCD_W-CLOCK_W)/2)*EVE_PIXEL_UNIT,
                          ((LCD_H-CLOCK_H)/2)*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(END());

}

STATIC FTVOID dispClockTxtBkg (FTU32 x, FTU32 y, FTU8 Selected)
{
#define TXTBKG_R    161
#define TXTBKG_G    220
#define TXTBKG_B    220
#define TXTBKG_P_SZ 18
#define TXTBKG_L_LN 50
#define TXTBKG_GAP  2
    FTU32 X = x+TXTBKG_P_SZ-7, Y = y+TXTBKG_P_SZ-5;

    if (!Selected) {
        HAL_CmdBufIn(COLOR_RGB(0xFF,0xFF,0xFF));
    } else {
        HAL_CmdBufIn(COLOR_RGB(TXTBKG_R,TXTBKG_G,TXTBKG_B));
    }
    HAL_CmdBufIn(POINT_SIZE(TXTBKG_P_SZ*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(BEGIN(FTPOINTS));
	HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F((X+TXTBKG_L_LN)*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(END());
	HAL_CmdBufIn(BEGIN(RECTS));
	HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,(Y-TXTBKG_P_SZ+1)*EVE_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F((X+TXTBKG_L_LN)*EVE_PIXEL_UNIT,(Y+TXTBKG_P_SZ-1)*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(END());
    if (!Selected) {
        HAL_CmdBufIn(COLOR_RGB(BKG_R,BKG_G,BKG_B));
        HAL_CmdBufIn(POINT_SIZE((TXTBKG_P_SZ-TXTBKG_GAP)*EVE_PIXEL_UNIT));
        HAL_CmdBufIn(BEGIN(FTPOINTS));
        HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
        HAL_CmdBufIn(VERTEX2F((X+TXTBKG_L_LN)*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
        HAL_CmdBufIn(END());
        HAL_CmdBufIn(BEGIN(RECTS));
        HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,(Y-TXTBKG_P_SZ+TXTBKG_GAP+1)*EVE_PIXEL_UNIT));
        HAL_CmdBufIn(VERTEX2F((X+TXTBKG_L_LN)*EVE_PIXEL_UNIT,(Y+TXTBKG_P_SZ-TXTBKG_GAP-1)*EVE_PIXEL_UNIT));
        HAL_CmdBufIn(END());
    }
}

STATIC FTVOID dispClockTxt (FTU32 tag)
{
#define TXT_TITLE_X   LCD_W/2
#define TXT_TITLE_Y   20
#define TXT_OPEN_X    64
#define TXT_OPEN_Y    232
#define TXT_CLOSE_X   354
    FTC8 txtSetClock[] = {5,6,1,2,0},
         txtOpen[]     = {5,6,3,0},
         txtClose[]    = {5,6,4,0};

    if (TAG_CLKCLOSE == tag) {
        stDateTime.open = 0;
    } else if (TAG_CLKOPEN == tag) {
        stDateTime.open = 1;
    } else {
        /* do nothing leave stDateTime.open unchange */
    }

    HAL_CmdBufIn(COLOR_RGB(0xFF,0xFF,0xFF));
    CoCmd_TEXT(TXT_TITLE_X,TXT_TITLE_Y,HDL_FONT,OPT_CENTERX,txtSetClock);
    
    HAL_CmdBufIn(TAG_MASK(1));
	HAL_CmdBufIn(TAG(TAG_CLKOPEN));
    dispClockTxtBkg(TXT_OPEN_X,TXT_OPEN_Y,stDateTime.open);
    HAL_CmdBufIn(COLOR_RGB(0xFF,0xFF,0xFF));
    CoCmd_TEXT(TXT_OPEN_X,TXT_OPEN_Y,HDL_FONT,0,txtOpen);

	HAL_CmdBufIn(TAG(TAG_CLKCLOSE));
    dispClockTxtBkg(TXT_CLOSE_X,TXT_OPEN_Y,stDateTime.open?0:1);
    HAL_CmdBufIn(COLOR_RGB(0xFF,0xFF,0xFF));
    CoCmd_TEXT(TXT_CLOSE_X,TXT_OPEN_Y,HDL_FONT,0,txtClose);
    HAL_CmdBufIn(TAG_MASK(0));
}

STATIC FTVOID dispClock (FTVOID)
{
	
    HAL_CmdBufIn(TAG_MASK(1));
	HAL_CmdBufIn(TAG(TAG_CLOCK));
    //can not use OPT_NOBACK, it would make the tag area too small
    CoCmd_BGCOLOR(BKG_R<<16|BKG_G<<8|BKG_B);
    CoCmd_CLOCK(LCD_W/2,LCD_H/2,CLOCK_RI,OPT_NOTICKS|OPT_NOSECS,0,ClkSetValue,0,0);
	HAL_CmdBufIn(TAG_MASK(0));
    CoCmd_TRACK(LCD_W/2,LCD_H/2,1,1,TAG_CLOCK);
    
    dispClockBkg();
}

STATIC FTU32 setClock (FTU32 clkValue, FTU32 clkTrack)
{
/* maxium move(in minute) between two track read for track critical point check */
/* x:30 track=1; x:00 track=65536/2; x:29=65536 */
#define SETCLK_MAX_MOVE   15 
#define SETCLK_MAX_MINUTE 60
#define SETCLK_MAX_CLOCK  12*60

    static FT32 newValue = 0, preTrack = 0;
    FTU32 track;
    FTU32 moveUnit = (TRACK_MAX/SETCLK_MAX_MINUTE);
    FT32  move;

    if (TAG_CLOCK != (FTU8)clkTrack) {
        newValue = clkValue*moveUnit;
        preTrack = 0;
        return clkValue;
    }

    track = clkTrack>>16;

    /* skip the non-touch and non-move value */
    if (!preTrack || preTrack == track) {
        preTrack = track;
        return clkValue;
    }
    move = (FT32)(track - preTrack);
    
    /* cross track critical point check */
    if (move > 0 && move > (FT32)(SETCLK_MAX_MOVE*moveUnit)) {
        move = (1 - preTrack) + (track - TRACK_MAX);
    } else if (move < 0 && move*(-1) > (FT32)(SETCLK_MAX_MOVE*moveUnit)) {
        move = (TRACK_MAX - preTrack) + (track - 1);
    }

    newValue += move;

    /* cross clock critical point check */
	if (newValue < 0) {
	    newValue += SETCLK_MAX_CLOCK*moveUnit;
    }
	if (newValue >= (FT32)(SETCLK_MAX_CLOCK*moveUnit)) {
	    newValue -= SETCLK_MAX_CLOCK*moveUnit;
    }

    preTrack = track;
    return newValue/moveUnit;
}

STATIC FTU32 getNumRatio (FT32 oY)
{
    /* up and down offset quite the same, use down as countiong sample */
	FTU32 H = (DATE_NUM_DOWN_INVIS_Y - DATE_NUM_MID_Y);
	FTU32 R = (DATE_RATIO_MAX - DATE_RATIO_MIN);
    FT32 offset = oY - DATE_NUM_MID_Y;

    offset = (offset < 0)?(offset*(-1)):offset;

	if (H >= R) {
		return DATE_RATIO_MAX - offset / (H/R);
	} else {
		return DATE_RATIO_MAX - offset * (R/H);
	}
}

STATIC FTU32 getPosData (DateDisp_st *stDate, FTU8 TopButm)
{
    FTU8 i,j;
    FTU32 tmp;

    for (i = 1,tmp = stDate[0].Y,j=0;i < DATE_POS_NUM;i++) {
        if (TopButm) {
            if (tmp < stDate[i].Y) {
                tmp = stDate[i].Y;
                j = i;
            }
        } else {
            if (tmp > stDate[i].Y) {
                tmp = stDate[i].Y;
                j = i;
            }
        }
    }

    return stDate[j].data;
}

STATIC FT32 getSavedDate (DateDisp_st *stDate, FT8 seed)
{
    FT32 tmp = 0;

    if (stYear == stDate) {
        tmp = DatSetValue/10000;
        if (!seed) {
            return tmp;
        }
        tmp += seed;
        if (seed) {
            return (tmp > 9999)?1900:tmp;
        } else {
            return (tmp < 1900)?9999:tmp;
        }
    } else if (stMonth == stDate) {
        tmp = (DatSetValue/100)%100;
        if (!seed) {
            return tmp;
        }
        tmp += seed;
        if (seed) {
            return (tmp > 12)?1:tmp;
        } else {
            return (tmp < 1)?1:tmp;
        }
    } else if (stDay == stDate) {
        tmp = DatSetValue%100;
        if (!seed) {
            return tmp;
        }
        tmp += seed;
        if (seed) {
            return (tmp > 31)?1:tmp;
        } else {
            return (tmp < 1)?1:tmp;
        }
    } else {
        return tmp;
    }
}

STATIC FTVOID initDate (DateDisp_st *stDate)
{
    /* first time display */
    stDate[0].Y = DATE_NUM_UP_2_Y;
    stDate[0].data = getSavedDate(stDate,-2);

    stDate[1].Y = DATE_NUM_UP_1_Y;
    stDate[1].data = getSavedDate(stDate,-1);

    stDate[2].Y = DATE_NUM_MID_Y;
    stDate[2].data = getSavedDate(stDate,0);

    stDate[3].Y = DATE_NUM_DOWN_1_Y;
    stDate[3].data = getSavedDate(stDate,1);

    stDate[4].Y = DATE_NUM_DOWN_2_Y;
    stDate[4].data = getSavedDate(stDate,2);
}

STATIC FTVOID sortDate (DateDisp_st *stp)
{
    /* sort the data after release */
    FTU8 in,out;
    FT32 offset,tmp,found = DATE_POS_NUM;
    FTU32 pos[DATE_POS_NUM] = {DATE_NUM_UP_2_Y,DATE_NUM_UP_1_Y,
                               DATE_NUM_MID_Y,DATE_NUM_DOWN_1_Y,
                               DATE_NUM_DOWN_2_Y};

    for (out = 0;out < DATE_POS_NUM;out++) {
        tmp = LCD_H;/* give a largest number */ 
        found = DATE_POS_NUM;
        for (in = 0;in < DATE_POS_NUM;in++) {
            offset = (pos[out] - stp[in].Y);
            if (offset < 0) {
                offset *= (-1);
            }
            if (tmp > offset) {
                tmp = offset;
                found = in;
            }
        }
        if (found != DATE_POS_NUM) {
            stp[found].Y = pos[out];
        }
    }
}

STATIC FTU32 getDateSelected (DateDisp_st *stp)
{
    FTU8 i;

    for (i = 0;i < DATE_POS_NUM;i++) {
        if (stp[i].Y == DATE_NUM_MID_Y) {
            return stp[i].data;
        }
    }
    return 0;
}

STATIC FTVOID sortDateAll (FTVOID)
{

    /* TODO: not work yet */
    //return;
    sortDate(stYear);
    sortDate(stMonth);
    sortDate(stDay);

    DatSetValue = getDateSelected(stYear)*10000+getDateSelected(stMonth)*100+getDateSelected(stDay);
}

STATIC FTVOID moveDate (FTU32 track)
{
    FT32 i,dir = 1;
    FTU32 unit = TRACK_MAX/DATE_H;
    DateDisp_st *stp;
    static FT32 t = 0; /* record the last track value */

    if (!stYear[0].data) {
        initDate(stYear);
    }
    if (!stMonth[0].data) {
        initDate(stMonth);
    }
    if (!stDay[0].data) {
        initDate(stDay);
    }

    if(TAG_YEAR == (track & 0xFF)) {
        stp = stYear;
    } else if(TAG_MONTH == (track & 0xFF)) {
        stp = stMonth;
    } else if(TAG_DAY == (track & 0xFF)) {
        stp = stDay;
    } else {
        sortDateAll();
        t = 0;
        return;
    }

    if (!t) {
        /* skip the first touch */
        t = (track >> 16);
        return;
    }

    t = (track >> 16) - t;
    /* no movement */
    if (!t) {
        return;
    }

    if (t < 0) {
        t *= (-1);
        dir = -1;
    }

    /* movement too small */
    if (t <= (FT32)unit) {
        return;
    }

    t = t/unit;

    while (t--) {
        for (i = 0;i < DATE_POS_NUM;i++) {
            stp[i].Y += dir;
        }
        for (i = 0;i < DATE_POS_NUM;i++) {
            if (dir>0) {
                /* move down */
                if (DATE_NUM_DOWN_INVIS_Y <= stp[i].Y) {
                    stp[i].data = getPosData(stp,0)-1;
                    if (stp == stYear && stp[i].data < 1900) {
                        stp[i].data = 9999;
                    } else if (stp == stMonth && stp[i].data < 1) {
                        stp[i].data = 12;
                    } else if (stp == stDay && stp[i].data < 1) {
                        stp[i].data = 31;
                    }
                    stp[i].Y = DATE_NUM_UP_INVIS_Y;
                    break;
                }
            } else {
                /* move up */
                if (DATE_NUM_UP_INVIS_Y >= stp[i].Y) {
                    stp[i].data = getPosData(stp,1)+1;
                    if (stp == stYear && stp[i].data > 9999) {
                        stp[i].data = 1900;
                    } else if (stp == stMonth && stp[i].data > 12) {
                        stp[i].data = 1;
                    } else if (stp == stDay && stp[i].data > 31) {
                        stp[i].data = 1;
                    }
                    stp[i].Y = DATE_NUM_DOWN_INVIS_Y;
                    break;
                }
            }
        }
    }

    t = (FT32)(track >> 16);
}

STATIC FTVOID dispDateNumPos (FTU32 X, FTU32 Y, FTU32 number)
{
#define DATENUM_W 18
    FTU32 x = X;
    FTU8 n1000 = (number/1000)%10,
         n100 = (number/100)%10, 
         n10 = (number/10)%10, 
         n1 = number%10;

    if (number >= 1000) {
        HAL_CmdBufIn(CELL(n1000));
        HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
    }
    if (number >= 100) {
        HAL_CmdBufIn(CELL(n100));
        if (number >= 1000) {
            x += DATENUM_W;
        }
        HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
    }
    if (number >= 10) {
        HAL_CmdBufIn(CELL(n10));
        if (number >= 100) {
            x += DATENUM_W;
        }
        HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
    }

    HAL_CmdBufIn(CELL(n1));
    if (number >= 10) {
        x += DATENUM_W;
    }
    HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

}

STATIC FTVOID dispDateNumber (FTU32 X, FTU32 Y, FTU32 number, FTU32 ratio)
{
    HAL_CmdBufIn(SAVE_CONTEXT());
	CoCmd_LOADIDENTITY;
    CoCmd_TRANSLATE(LEFT_W/2*EVE_TRANSFORM_MAX, LEFT_H/2*EVE_TRANSFORM_MAX);
    CoCmd_SCALE(EVE_TRANSFORM_MAX,ratio*EVE_TRANSFORM_MAX/100); /* only change the Y direction */
    CoCmd_TRANSLATE(LEFT_W/2*EVE_TRANSFORM_MAX*(-1), LEFT_H/2*EVE_TRANSFORM_MAX*(-1));
	CoCmd_SETMATRIX;
    dispDateNumPos(X,Y,number);
	HAL_CmdBufIn(RESTORE_CONTEXT());
}

STATIC FTVOID dispDate (FTVOID)
{
#define MASK_ENLARGE ((DATEDAY_X + DATEDAY_W - DATEYEAR_X)/MASK_W)
    FTU8 i;

	HAL_CmdBufIn(TAG_MASK(1));
    HAL_CmdBufIn(COLOR_RGB(BKG_R,BKG_G,BKG_B));
    HAL_CmdBufIn(BEGIN(RECTS));

    HAL_CmdBufIn(TAG(TAG_YEAR));
    HAL_CmdBufIn(VERTEX2F(DATEYEAR_X*EVE_PIXEL_UNIT,DATE_NUM_Y*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(VERTEX2F((DATEYEAR_X+DATEYEAR_W)*EVE_PIXEL_UNIT,(DATE_NUM_Y+DATE_H)*EVE_PIXEL_UNIT));
    
    HAL_CmdBufIn(TAG(TAG_MONTH));
    HAL_CmdBufIn(VERTEX2F(DATEMONTH_X*EVE_PIXEL_UNIT,DATE_NUM_Y*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(VERTEX2F((DATEMONTH_X+DATEMONTH_W)*EVE_PIXEL_UNIT,(DATE_NUM_Y+DATE_H)*EVE_PIXEL_UNIT));
   
    HAL_CmdBufIn(TAG(TAG_DAY));
    HAL_CmdBufIn(VERTEX2F(DATEDAY_X*EVE_PIXEL_UNIT,DATE_NUM_Y*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(VERTEX2F((DATEDAY_X+DATEDAY_W)*EVE_PIXEL_UNIT,(DATE_NUM_Y+DATE_H)*EVE_PIXEL_UNIT));
    
    HAL_CmdBufIn(END());
    HAL_CmdBufIn(TAG_MASK(0));
    
    CoCmd_TRACK(DATEYEAR_X,DATE_NUM_Y,DATEYEAR_W,DATE_H,TAG_YEAR);
    CoCmd_TRACK(DATEYEAR_X,DATE_NUM_Y,DATEMONTH_W,DATE_H,TAG_MONTH);
    CoCmd_TRACK(DATEYEAR_X,DATE_NUM_Y,DATEDAY_W,DATE_H,TAG_DAY);

    HAL_CmdBufIn(BEGIN(BITMAPS));
    HAL_CmdBufIn(COLOR_RGB(0xFF,0xFF,0xFF));
    HAL_CmdBufIn(BITMAP_HANDLE(HDL_NUMBER));
    for (i = 0;i < DATE_POS_NUM;i++) {
        dispDateNumber(DATEYEAR_X,stYear[i].Y,stYear[i].data,getNumRatio(stYear[i].Y));
        dispDateNumber(DATEMONTH_X,stMonth[i].Y,stMonth[i].data,getNumRatio(stMonth[i].Y));
        dispDateNumber(DATEDAY_X,stDay[i].Y,stDay[i].data,getNumRatio(stDay[i].Y));
    }

    /* use a mask bitmap to give an effect */
    HAL_CmdBufIn(BITMAP_HANDLE(HDL_MASK));
    HAL_CmdBufIn(BITMAP_SIZE(NEAREST,BORDER,BORDER,MASK_ENLARGE*imgHeader[HDL_MASK].wide,imgHeader[HDL_MASK].high));
    HAL_CmdBufIn(BITMAP_SIZE_H(MASK_ENLARGE*imgHeader[HDL_MASK].wide >> 9,imgHeader[HDL_MASK].high>>9));
    HAL_CmdBufIn(BITMAP_HANDLE(HDL_MASK_D));
    HAL_CmdBufIn(BITMAP_SIZE(NEAREST,BORDER,BORDER,MASK_ENLARGE*imgHeader[HDL_MASK_D].wide,imgHeader[HDL_MASK_D].high));
    HAL_CmdBufIn(BITMAP_SIZE_H(MASK_ENLARGE*imgHeader[HDL_MASK_D].wide >> 9,imgHeader[HDL_MASK_D].high>>9));

    HAL_CmdBufIn(SAVE_CONTEXT());
	CoCmd_LOADIDENTITY;
    CoCmd_SCALE(MASK_ENLARGE*EVE_TRANSFORM_MAX,EVE_TRANSFORM_MAX);
	CoCmd_SETMATRIX;
    dispPal8(DATEYEAR_X,DATE_NUM_UP_2_Y,imgHeader[HDL_MASK].lut_src,HDL_MASK,0);
    dispPal8(DATEYEAR_X,DATE_NUM_DOWN_2_Y+15,imgHeader[HDL_MASK_D].lut_src,HDL_MASK_D,0);
	HAL_CmdBufIn(RESTORE_CONTEXT());
   
    HAL_CmdBufIn(END());
}

STATIC FTVOID dispDateTxt (FTVOID)
{
#define DATETXT_YEAR_X 134
#define DATETXT_YEAR_Y 246
#define DATETXT_MONTH_X  234
#define DATETXT_DAY_X    330
    FTC8 txtDate[]   = {7,8,1,2,0},
         txtYear[]   = {9,0},
         txtMonth[]  = {10,0},
         txtDay[]    = {11,0};

    HAL_CmdBufIn(COLOR_RGB(0xFF,0xFF,0xFF));
    CoCmd_TEXT(TXT_TITLE_X,TXT_TITLE_Y,HDL_FONT,OPT_CENTERX,txtDate);

    CoCmd_TEXT(DATETXT_YEAR_X,DATETXT_YEAR_Y,HDL_FONT,OPT_CENTERX,txtYear);
    CoCmd_TEXT(DATETXT_MONTH_X,DATETXT_YEAR_Y,HDL_FONT,OPT_CENTERX,txtMonth);
    CoCmd_TEXT(DATETXT_DAY_X,DATETXT_YEAR_Y,HDL_FONT,OPT_CENTERX,txtDay);

}

STATIC FTVOID okAction (FTVOID)
{
    stDateTime.year  = DatSetValue/10000;
    stDateTime.month = (DatSetValue/100)%100;
    stDateTime.day   = DatSetValue%100;
    stDateTime.hour  = (ClkSetValue/60)%100;
    stDateTime.minute = ClkSetValue%60;
}

STATIC FTVOID tagAction (FTU32 tagValue)
{
    static FTU8 butm = 0, arrow = 0;

    appGP.appIndex = last_index;
    
    switch (tagValue) {
        case TAG_HOME:
            dispButm(0,1);
            dispArrow(0,0);
            butm = 1;
            break;
        case TAG_OK:
            dispButm(1,1);
            dispArrow(0,0);
            okAction();
            break;
        case TAG_LEFT:
            dispArrow(0,1);
            dispButm(0,0);
            arrow = 1;
            break;
        case TAG_RIGHT:
            dispArrow(1,1);
            dispButm(0,0);
            arrow = 1;
            break;
        default:
            dispButm(0,0);
            dispArrow(0,0);
            if (butm) {
                butm = 0;
                appGP.appIndex = 0;
            }
            if (arrow) {
                arrow = 0;
                appGP.appIndex = (last_index==1)?2:1;
            }
            break;
    }
}

STATIC FTVOID subUI (FTU32 uiIndex)
{
    FTU32 Tag = HAL_Read32(REG_TOUCH_TAG), 
          Track = HAL_Read32(REG_TRACKER),
          tmp = ClkSetValue;

    last_index = uiIndex;

    HAL_CmdBufIn(CMD_DLSTART);
    HAL_CmdBufIn(CLEAR_COLOR_RGB(BKG_R,BKG_G,BKG_B));
    HAL_CmdBufIn(CLEAR(1,1,1));
    
    dispDateTime();

    if (uiIndex == 1) {
        dispClock();

        dispClockTxt(Tag);

        ClkSetValue = setClock(tmp,Track);
    } else {
        dispDateTxt();
        
        dispDate();

        moveDate(Track);
    }
    
    tagAction(Tag);

    HAL_CmdBufIn(DISPLAY());
    HAL_CmdBufIn(CMD_SWAP);
    HAL_BufToReg(RAM_CMD,0);
}

FTVOID aupu_time_ui (FTU32 para)
{
    subUI(1);
}

FTVOID aupu_date_ui (FTU32 para)
{
    subUI(2);
}

FTVOID aupu_main_ui (FTU32 para)
{
#define TEST_H (40)
#define TEST_X (LCD_W/2)
#define TEST_W (30)

    static FTU32 load = 0, touched = 0;

    if (!load) {
        load = loadResources();
    }

    if (!load) {
        DBGPRINT;
        return;
    }

    HAL_CmdBufIn(CMD_DLSTART);
    HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
    HAL_CmdBufIn(CLEAR(1,1,1));

    /* after the rotation, the PC emulation API
       does not handle properly for track, tag, etc.
       try on real chip to see if the code works.
       to rotate on PC emulator, need to use 
       HAL_Write32(REG_ROTATE,2), or HAL_Write32(REG_ROTATE,0)
       but, as I said above, the over all function of rotate
       is not working well on PC emulator.
    */
	CoCmd_SETROTATE(ROTATE_VALUE);

    /* set the user font */
	CoCmd_SETFONT(HDL_FONT, RAM_G, &stTxtFnt);
    
    /* display the background */
    dispBKG(bg_start_addr);

    /* just for setting result display */
    HAL_CmdBufIn(COLOR_RGB(0,0xFF,0));
    CoCmd_ROMFONT(HDL_MP_F,32);

    CoCmd_NUMBER(TEST_X-8*(TEST_W),TEST_H,HDL_MP_F,0,stDateTime.year);
    CoCmd_TEXT(TEST_X-4*(TEST_W),TEST_H,HDL_MP_F,0,"/");
    CoCmd_NUMBER(TEST_X-3*TEST_W,TEST_H,HDL_MP_F,2,stDateTime.month);
    CoCmd_TEXT(TEST_X-TEST_W,TEST_H,HDL_MP_F,0,"/");
    CoCmd_NUMBER(TEST_X,TEST_H,HDL_MP_F,2,stDateTime.day);
    CoCmd_NUMBER(TEST_X+3*TEST_W,TEST_H,HDL_MP_F,2,stDateTime.hour);
    CoCmd_TEXT(TEST_X+5*TEST_W,TEST_H,HDL_MP_F,0,":");
    CoCmd_NUMBER(TEST_X+6*TEST_W,TEST_H,HDL_MP_F,2,stDateTime.minute);
    if (stDateTime.open) {
        CoCmd_TEXT(TEST_X,LCD_H-TEST_H,HDL_MP_F,OPT_CENTER,"System ON");
    } else {
        CoCmd_TEXT(TEST_X,LCD_H-TEST_H,HDL_MP_F,OPT_CENTER,"System OFF");
    }

    HAL_CmdBufIn(DISPLAY());
    HAL_CmdBufIn(CMD_SWAP);
    HAL_BufToReg(RAM_CMD,0);

    if (TOUCHED) {
        if (!touched) {
            touched = 1;
        }
    } else {
        if (touched) {
            touched = 0;
            appGP.appIndex = last_index;
            return;
        }
    }
    appGP.appIndex = 0; 
}

AppFunc APPS_UI[] = {
	aupu_main_ui,
    aupu_time_ui,
    aupu_date_ui
};
