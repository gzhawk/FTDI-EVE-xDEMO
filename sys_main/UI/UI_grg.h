/* 
	A demo for GRG's cash machine
	Author: Hawk
    Email : hawk.gao@ftdichip.com	
	Date  : 2013/Oct
*/

/* 
 * The sequence below HAS to be the SAME 
 * as function point be put in APPS_UI 
 */
typedef enum CASH_NUM_e {
	CASH_BOOT = 0,
	CASH_MAIN,
	CASH_REST,
	CASH_NETS,
	CASH_FAKE,
	CASH_TIME,
	CASH_VERS,
	CASH_DBSM,
	CASH_NULL
} CASH_NUM;

typedef enum TABLE_TITLE_e {
	TBL_NO = 0,//no bold line
	TBL_ROW,//a bold line on top
	TBL_COL,//a bold line on left
	TBL_BOTH,//a bold line on top and left
} TABLE_TITL;

typedef enum TIME_EVENT_e {
	NO_CHNG = 0,//clock interface
	TIME_CHNG,//time setting interface
	DATE_CHNG,//date setting interface
} TIME_EVENT;

#define TAG_OFFSET (5)//not using 0 as tag
typedef enum TAG_DEF_e {
	TAG_CASH_MAIN = TAG_OFFSET+CASH_MAIN,
	TAG_CASH_REST,
	TAG_CASH_NETS,
	TAG_CASH_FAKE,
	TAG_CASH_TIME,
	TAG_CASH_VERS,
	TAG_CASH_DBSM,

	TAG_DATE,
	TAG_DATE_LTAG,
	TAG_DATE_MTAG,
	TAG_DATE_RTAG,

	TAG_CLCK,
	TAG_CLCK_LTAG,
	TAG_CLCK_MTAG,
	TAG_CLCK_RTAG
};


typedef enum TOUCH_DEF_e {
	TOUCH_NO = 0,
	TOUCH_PRESS,
	TOUCH_RELES,
};

typedef struct track_ctrl_ {
	FT32 SkipP;  //skip the too small forward moving (trembling)
	FT32 SkipM;  //skip the too small backward moving (trembling)
	FT32 ExSkipP;//skip the too far forward moving (touch another other location), but record the position
	FT32 ExSkipM;//skip the too far backward moving (touch another other location, but record the position
} track_ctrl_st;


#define TABLE_X 20
#define TABLE_Y 10
#define TABLE_ROW 8
#define TABLE_COL 3
#define TABLE_LW 1
#define TABLE_TW 2
#define TABLE_DATA_LEN 10
typedef struct table_contents_ {
	FTU32 font;
	FTC8 str[TABLE_DATA_LEN];
} table_contents_st;
typedef struct table_layout_ {
	FTU32 X;//Top left point of table
	FTU32 Y;//Top left point of table
	TABLE_TITL numTitle;
	FTU32 rowNum;
	FTU32 *rowHighList;
	FTU32 columnNum;
	FTU32 *columnWideList;
} table_layout_st;

#define TIME_R 80
#define TIME_BMP_NUM 2
#define TIME_W 24 // limited to the font FONT_TIME_DAT WxH for demo 
#define TIME_H 29
#define TIME_XL (EVE_LCD_WIDTH/2-4*TIME_W)
#define TIME_XM (EVE_LCD_WIDTH/2-TIME_W)
#define TIME_XR (EVE_LCD_WIDTH/2+2*TIME_W)
#define TIME_D_XL (EVE_LCD_WIDTH/2-5*TIME_W)
#define TIME_D_XM (EVE_LCD_WIDTH/2)
#define TIME_D_XR (EVE_LCD_WIDTH/2+3*TIME_W)

#define TIME_YT (EVE_LCD_HIGH/2-3*TIME_H)
#define TIME_YB (EVE_LCD_HIGH/2+2*TIME_H)
#define TIME_TRACK_BASE (EVE_TRACK_VALUE_MAX/(TIME_H*5))
#define TIME_TRACK_VALID (TIME_TRACK_BASE*TIME_H)
#define TIME_TRACK_MORE (TIME_TRACK_VALID+TIME_H)
#define TIME_TRACK_LESS (TIME_TRACK_VALID/2)
#define TIME_D_Y_MAX 9999
#define TIME_D_M_MAX 12
#define TIME_D_D_MAX 31
#define TIME_H_MAX 23
#define TIME_M_MAX 59
#define TIME_S_MAX 59
typedef struct datetime_st_ {
	FTU32 year;
	FTU32 month;
	FTU32 day;
	FTU32 hours;
	FTU32 minutes;
	FTU32 seconds;
	FTU32 minseconds;
} datetime_st;

#define ICON_NUM (6)
#define ICON_W (90)
#define ICON_H (90)

#define ICON_X_GAP ((EVE_LCD_WIDTH-ICON_W*3)/4)
#define ICON_Y_TGAP ((EVE_LCD_HIGH-ICON_H*2)/3-10)
#define ICON_Y_MGAP ((EVE_LCD_HIGH-ICON_H*2)/3-12)
/* n from 0 to 5 */
#define ICON_X0(n) ((n < 3)?((n+1)*ICON_X_GAP+n*ICON_W):((n-2)*ICON_X_GAP+(n-3)*ICON_W))
#define ICON_Y0(n) ((n < 3)?ICON_Y_TGAP:ICON_H+ICON_Y_TGAP+ICON_Y_MGAP)

#define ICON_X1(n) (ICON_X0(n)+ICON_W)
#define ICON_Y1(n) (ICON_Y0(n)+ICON_H)
#define ICON_T_X(n) (ICON_X0(n)+ICON_W/2)
#define ICON_T_Y(n) (ICON_Y1(n)-5)
#define ICON_T_MLEN 4
#define ICON_ROTATE_D 10

#define BUTN_W 80
#define BUTN_H 40
#define BUTN_M_GAP 5
#define BUTN_R_GAP 20
#define BUTN_B_GAP 10
/* n from 0 ~ 2, sequence from right to left, at the right bottom corner */
#define BUTN_X(n) (EVE_LCD_WIDTH-((n+1)*BUTN_W)-BUTN_M_GAP-((n)?BUTN_M_GAP:0))
#define BUTN_Y (EVE_LCD_HIGH-BUTN_H-BUTN_B_GAP)

/* the raw font file also need to follow this address (-d parameter)
 * while using fnt_cvt tools, same address MUST not be used in same pages */
#define FONT_BUTN_ADDR RAM_G
#define FONT_BUTN_RAW_SIZE (7348)
#define FONT_MENU_1_ADDR FONT_BUTN_RAW_SIZE
#define FONT_MENU_RAW_SIZE (13668)
#define FONT_VERS_1_ADDR (FONT_MENU_1_ADDR+FONT_MENU_RAW_SIZE)//21016 == 7348 + 13668
#define FONT_VERS_1_RAW_SIZE (10948)
#define FONT_VERS_2_ADDR (FONT_VERS_1_ADDR+FONT_VERS_1_RAW_SIZE)//31964 == 21016 + 10948
#define FONT_VERS_2_RAW_SIZE (20020)

#define MENU_BMP_ADDR FONT_VERS_2_ADDR+FONT_VERS_2_RAW_SIZE
#define TIME_BMP_ADDR MENU_BMP_ADDR

typedef enum FONT_NUM_e {
	/*
	 * bitmap for icon use handle 0~5
	 * bitmap for time change column border use 0~1
	 * to avoid external font on above page
	 * using the same handle as icon and column border
	 * use the largest one to give a offset
	 */
	BMP_H_0 = 0,
	FONT_BUTN = ICON_NUM,
	FONT_MENU,
	FONT_TBLE_TLT,
	FONT_TBLE_DES,//external font number less than 16
	/* --------------------- */
	FONT_TBLE_DAT = 23, //internal font start from 16
	FONT_TBLE_DEF, // default font when font equal 0 (not define)
	FONT_TIME_DAT = 24,
	FONT_DEVP_TLT, //internal font not more than 31
} FONT_NUM;

#define BOOT_CUBE_W 50
#define BOOT_CUBE_H 50
#define BOOT_LOGO_W 300
#define BOOT_LOGO_H 120
#define BOOT_LOGO_ZOOM  50
#define BOOT_BMP_SIZE 72000
#define BOOT_BMP_ADDR (EVE_RAMG_SIZE - BOOT_BMP_SIZE)

/* manage the color here */
#define COLOR_CLR CLEAR_COLOR_RGB(63,97,161)
#define COLOR_GRA CoCmd_GRADIENT(0,0,0x3f61a1,0,EVE_LCD_HIGH,0x000000)
#define COLOR_CLR_B0 CLEAR_COLOR_RGB(0,0,0)
#define COLOR_CLR_B1 CLEAR_COLOR_RGB(0xff,0xff,0xff)
#define COLOR_BOT COLOR_RGB(63,97,161)
#define COLOR_TITLE COLOR_RGB(0xff,0xff,0xff)
#define COLOR_TIME COLOR_RGB(0,0,0)
#define COLOR_BKGRD COLOR_RGB(0xff,0xff,0xff)
#define COLOR_FG_PRESS 0x5F5F5F00
#define COLOR_FG_RELES 0x80808000
/* 
 * manage the file path here for better managment 
 * limited to the sdcard.h we used, the file name
 * can only support equal or less than 8 char file name with 3 char extantion
 */ 

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(MSVC2017EMU) 
#define PATH_ICON_1 ROOT_PATH"cash\\I1.raw"
#define PATH_ICON_2 ROOT_PATH"cash\\I2.raw"
#define PATH_ICON_3 ROOT_PATH"cash\\I3.raw"
#define PATH_ICON_4 ROOT_PATH"cash\\I4.raw"
#define PATH_ICON_5 ROOT_PATH"cash\\I5.raw"
#define PATH_ICON_6 ROOT_PATH"cash\\I6.raw"
#define PATH_ICON_T ROOT_PATH"cash\\IM.raw"
#define PATH_NUM_T  ROOT_PATH"cash\\NT.raw"
#define PATH_NUM_B  ROOT_PATH"cash\\NB.raw"
#define PATH_BUTN   ROOT_PATH"cash\\BN.raw"
#define PATH_VER_T  ROOT_PATH"cash\\V1.raw"
#define PATH_VER_C  ROOT_PATH"cash\\V2.raw"
#define PATH_BOOT  ROOT_PATH"cash\\LG.raw"
#else
#define PATH_ICON_1 ROOT_PATH"I1.raw"
#define PATH_ICON_2 ROOT_PATH"I2.raw"
#define PATH_ICON_3 ROOT_PATH"I3.raw"
#define PATH_ICON_4 ROOT_PATH"I4.raw"
#define PATH_ICON_5 ROOT_PATH"I5.raw"
#define PATH_ICON_6 ROOT_PATH"I6.raw"
#define PATH_ICON_T ROOT_PATH"IM.raw"
#define PATH_NUM_T  ROOT_PATH"NT.raw"
#define PATH_NUM_B  ROOT_PATH"NB.raw"
#define PATH_BUTN   ROOT_PATH"BN.raw"
#define PATH_VER_T  ROOT_PATH"V1.raw"
#define PATH_VER_C  ROOT_PATH"V2.raw"
#define PATH_BOOT  ROOT_PATH"LG.raw"
#endif
/*
 * define here to use static memory
 * to avoid use too much heap spaces in cashXXXX routines 
 */
bmpHDR_st bmpHD[ICON_NUM] = {
	{PATH_ICON_1, 0,0,ARGB1555,  0,0, ICON_W, ICON_H},
	{PATH_ICON_2, 0,0,ARGB1555,  0,0, ICON_W, ICON_H},
	{PATH_ICON_3, 0,0,ARGB1555,  0,0, ICON_W, ICON_H},
	{PATH_ICON_4, 0,0,ARGB1555,  0,0, ICON_W, ICON_H},
	{PATH_ICON_5, 0,0,ARGB1555,  0,0, ICON_W, ICON_H},
	{PATH_ICON_6, 0,0,ARGB1555,  0,0, ICON_W, ICON_H},
};
bmpHDR_st bmpNumHD[TIME_BMP_NUM] = {
	{PATH_NUM_T, 0,0,ARGB1555, 0,0, TIME_W, TIME_H/2},
	{PATH_NUM_B, 0,0,ARGB1555, 0,0, TIME_W, TIME_H/2},
};
bmpHDR_st bmpLogoHD[1] = {
	{PATH_BOOT, 0,0,ARGB1555, 0,0, BOOT_LOGO_W, BOOT_LOGO_H},
};

FTC8 iconStr[ICON_NUM][ICON_T_MLEN+1] = {
	/* has to be ended with a 0 on each string */
	{ 1, 2, 3, 4, 0},
	{ 5, 6, 7, 8, 0},
	{ 9,10,11,12, 0},
	{13,14,15,16, 0},
	{17,18,19,20, 0},
	{21,22,23,24, 0}
};
/* ended with 0,0 for better operation judgement */
table_contents_st tblData[TABLE_ROW*TABLE_COL+1] = {
	/* has to be ended with a 0 on each string */
	{FONT_TBLE_TLT, {1,2,0}},
	{FONT_TBLE_TLT, {3,4,0}},
	{FONT_TBLE_TLT, {5,6,7,0}},

	{FONT_TBLE_DAT, "1"},
	{FONT_TBLE_DES, {1,2,3,4,5,6,7,0}},
	{FONT_TBLE_DAT, "V1.0B200"},
	
	{FONT_TBLE_DAT, "2"},
	{FONT_TBLE_DES, {8,9,10,11,12,13,0}},
	{FONT_TBLE_DAT, "GPU-V42$"},
	
	{FONT_TBLE_DAT, "3"},
	{FONT_TBLE_DES, {14,15,16,17,18,0}},
	{FONT_TBLE_DAT, "V800x8.3"},

	{FONT_TBLE_DAT, "4"},
	{FONT_TBLE_DES, {19,20,21,22,23,24,0}},
	{FONT_TBLE_DAT, "H_4"},
	
	{FONT_TBLE_DAT, "5"},
	{FONT_TBLE_DES, {25,26,27,28,29,30,0}},
	{FONT_TBLE_DAT, "P989*087"},
	
	{FONT_TBLE_DAT, "6"},
	{FONT_TBLE_DES, {31,32,33,34,35,36,37,0}},
	{FONT_TBLE_DAT, "SUB_GPU3"},
	
	{FONT_TBLE_DAT, "7"},
	{FONT_TBLE_DES, {38,39,40,41,42,43,44,0}},
	{FONT_TBLE_DAT, "V56(23)6"},

	{0, 0},
};
/* vertical direction */
track_ctrl_st track_ctrl = {
	TIME_TRACK_LESS,
	-1*(TIME_TRACK_LESS),

	TIME_TRACK_MORE,
	-1*(TIME_TRACK_MORE),
};
datetime_st curTime = {2013,10,17,10,10,0,0};//the date I start my coding :-)

/* it's an example for you to pass the data between each page */
FTU32 gData;
/* 
 * RAM_M Content Map
 * bitmap: 0 ~ bitmap size
 * font  : bitmap size ~ font size
 */
static FTVOID devp (FTU32 cashPara)
{
	FT_Gpu_Fonts_t stFB;
	FTU32 i = *(FTU32 *)appGP.appPara;
	FTC8 butnStr[] = {1,2,0};//go back
	FTU8 Touch = 0, tFlag = TOUCH_NO;

	/* load the font resources into EVE */
	if (0 == appFileToRamG(PATH_BUTN,FONT_BUTN_ADDR,0,(FTU8 *)&stFB,sizeof(FT_Gpu_Fonts_t))) {
		DBGPRINT;
		return;
	}

	while (1) {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(COLOR_CLR);
		HAL_CmdBufIn(CLEAR(1,1,1));
		HAL_CmdBufIn(COLOR_TITLE);
		COLOR_GRA;

		/* set the external font to each font index */
		CoCmd_SETFONT(FONT_BUTN, FONT_BUTN_ADDR, &stFB);

		/* cancel bottun */
		HAL_CmdBufIn(TAG(TAG_CASH_MAIN));
		if (Touch == TAG_CASH_MAIN) {
			tFlag = TOUCH_PRESS;
			appGP.appIndex = CASH_MAIN;
			gData = CASH_MAIN;
			appGP.appPara = (FTU32)&gData;
			CoCmd_FGCOLOR(COLOR_FG_PRESS);
		} else {
			tFlag = (tFlag == TOUCH_PRESS)?TOUCH_RELES:tFlag;
			CoCmd_FGCOLOR(COLOR_FG_RELES);
		}
		CoCmd_BUTTON(BUTN_X(0),BUTN_Y,BUTN_W, BUTN_H, FONT_BUTN,0,butnStr);
		HAL_CmdBufIn(TAG_MASK(0));

		/* display the titel on each icon by using external font */
		CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/4,FONT_DEVP_TLT,OPT_CENTER,"Menu");
		CoCmd_NUMBER(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2,FONT_DEVP_TLT,OPT_CENTER,i);
		CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/4*3,FONT_DEVP_TLT,OPT_CENTER,"Developing...");

		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);

		if (TOUCH_RELES == tFlag) {
			return;
		}
		/* detecting the touch event */
		Touch = HAL_Read8(REG_TOUCH_TAG);
	}

}
static FTVOID tblLines(FTU32 x0, FTU32 y0, FTU32 x1, FTU32 y1, FTU32 w)
{
	HAL_CmdBufIn(LINE_WIDTH(w*EVE_PIXEL_UNIT));
	HAL_CmdBufIn(BEGIN(LINES)); 
	HAL_CmdBufIn(VERTEX2F((x0)*EVE_PIXEL_UNIT,(y0)*EVE_PIXEL_UNIT)); 
	HAL_CmdBufIn(VERTEX2F((x1)*EVE_PIXEL_UNIT,(y1)*EVE_PIXEL_UNIT)); 
	HAL_CmdBufIn(END());
}
static FTVOID drawTable (table_layout_st *tblL)
{
	FTU32 r, c, W, H, tmp;

	/* get total high of table, depands on row numbers */
	for (r = 0, H = 0; r < tblL->rowNum; r++) {
		H += tblL->rowHighList[r];
	}
	
	/* get total wide of table, depands on column numbers */
	for (c = 0, W = 0; c < tblL->columnNum; c++) {
		W += tblL->columnWideList[c];
	}	

	/* row */
	for (r = 0, tmp = tblL->Y; ; r++) {
		if (r == 1 && (tblL->numTitle == TBL_ROW || tblL->numTitle == TBL_BOTH)) {
			tblLines(tblL->X,tmp,tblL->X+W,tmp,TABLE_TW);
		} else {
			tblLines(tblL->X,tmp,tblL->X+W,tmp,TABLE_LW);
		}
		if (r == tblL->rowNum) {
			break;
		}
		tmp += tblL->rowHighList[r];
	}

	/* column */
	for (c = 0, tmp = tblL->X; ; c++) {
		if (c == 1 && (tblL->numTitle == TBL_COL || tblL->numTitle == TBL_BOTH)) {
			tblLines(tmp,tblL->Y,tmp,tblL->Y+H,TABLE_TW);
		} else {
			tblLines(tmp,tblL->Y,tmp,tblL->Y+H,TABLE_LW);
		}
		if (c == tblL->columnNum) {
			break;
		}
		tmp += tblL->columnWideList[c];
	}
}

/* analysis the data line by line in the table, from left to right */
static FTVOID showData (table_layout_st *tblL, table_contents_st *tblD)
{
	FTU32 r, c, i, X, Y;

	for (r = 0, Y = tblL->Y; r < tblL->rowNum; r++) {
		for (c = 0, X = tblL->X; c < tblL->columnNum; c++) {
			i = TABLE_COL*r+c;
			/* while no data in this field
			 * can set font or str to 0 
			 * for easy operation
			 */
			if (tblD[i].font && tblD[i].str) {
				CoCmd_TEXT (X + tblL->columnWideList[c]/2, Y, tblD[i].font, OPT_CENTERX, tblD[i].str);
			} else {
				CoCmd_TEXT (X + tblL->columnWideList[c]/2, Y, FONT_TBLE_DEF, OPT_CENTERX, " ");
			}
			X += tblL->columnWideList[c];
		}
		Y += tblL->rowHighList[r];
	}
}
static FTU32 getTrackNum (track_ctrl_st *ptrack_ctrl, FTU32 track_data, FTU32 init_num,FTU32 start_num, FTU32 max_num)
{
	static FTU32 base = EVE_TRACK_VALUE_MAX+1;
	FT32 offset, num = init_num;

	if (track_data == 0) {
		return init_num;	
	}

	if (base == EVE_TRACK_VALUE_MAX+1) {
		/* record the first touch data as base data */
		base = track_data;
		num = init_num;
	} else {
		offset = (track_data - base);
		if (offset >= 0) {
			/* forward moving */
			if (offset >= ptrack_ctrl->ExSkipP) {
				/* touch another location than previous touch */	
				base = track_data;
			} else if (offset <= ptrack_ctrl->SkipP) {
				/* do nothing */	
			} else {
				/* reasonable moving */	
				num = (num == start_num) ? max_num - 1 : num - 1;
				base = track_data;
			}
		} else {
			/* backward moving */
			if (offset <= ptrack_ctrl->ExSkipM) {
				/* touch another location than previous touch */	
				base = track_data;
			} else if (offset >= ptrack_ctrl->SkipM) {
				/* do nothing */	
			} else {
				/* reasonable moving */	
				num = ((FTU32)num == max_num ) ? start_num : num + 1;
				base = track_data;
			}		
		}
	}
	
	
	return num;
}
static FTVOID drawChgCol (FTU32 cenX, FTU32 cenY, FTU32 ColH, FTU8 font, FTU32 fontH, FTU32 num, FTU32 start_num, FTU32 max_num)
{
	FT32 i;
	FTU32 y;

	/* display the middle */
	y = cenY;
	i = num;
	CoCmd_NUMBER(cenX,y,font,OPT_CENTER,i);
		
	/* display the half to bottom */
	for (y = cenY, i = num; ;) {
		i++;
		y += fontH;
		if ((FTU32)i > max_num) {
			i = start_num;
		}
		if (y >= cenY+ColH/2) {
			break;
		}
		CoCmd_NUMBER(cenX,y,font,OPT_CENTER,i);
	}

	/* display the half to top */
	for (y = cenY, i = num; ;) {
		i--;
		y -= fontH;
		if (i < (FT32)start_num) {
			i = max_num;
		}
		if (y <= cenY-ColH/2) {
			break;
		}
		CoCmd_NUMBER(cenX,y,font,OPT_CENTER,i);
	}

}

static FTVOID drawCol (datetime_st *pstDT, FTU32 timeORdate)
{
	FTU32 i,j,k,w = timeORdate?8*TIME_W:10*TIME_W,h = TIME_H;
	table_layout_st tblS = {
		timeORdate?TIME_XL:TIME_D_XL,
		TIME_YT+2*TIME_H,
		TBL_NO,
		1,
		&h,
		1,
		&w,
	};

	/* set the tag to each part */
	HAL_CmdBufIn(COLOR_BKGRD);
	HAL_CmdBufIn(BEGIN(RECTS));
	if (timeORdate) {
		HAL_CmdBufIn(TAG(TAG_CLCK_LTAG));
		HAL_CmdBufIn(VERTEX2F(TIME_XL*EVE_PIXEL_UNIT,TIME_YT*EVE_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((TIME_XL+2*TIME_W)*EVE_PIXEL_UNIT,(TIME_YT+5*TIME_H)*EVE_PIXEL_UNIT));

		HAL_CmdBufIn(TAG(TAG_CLCK_MTAG));
		HAL_CmdBufIn(VERTEX2F(TIME_XM*EVE_PIXEL_UNIT,TIME_YT*EVE_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((TIME_XM+2*TIME_W)*EVE_PIXEL_UNIT,(TIME_YT+5*TIME_H)*EVE_PIXEL_UNIT));

		HAL_CmdBufIn(TAG(TAG_CLCK_RTAG));
		HAL_CmdBufIn(VERTEX2F(TIME_XR*EVE_PIXEL_UNIT,TIME_YT*EVE_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((TIME_XR+2*TIME_W)*EVE_PIXEL_UNIT,(TIME_YT+5*TIME_H)*EVE_PIXEL_UNIT));
	} else {
		HAL_CmdBufIn(TAG(TAG_DATE_LTAG));
		HAL_CmdBufIn(VERTEX2F(TIME_D_XL*EVE_PIXEL_UNIT,TIME_YT*EVE_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((TIME_D_XL+4*TIME_W)*EVE_PIXEL_UNIT,(TIME_YT+5*TIME_H)*EVE_PIXEL_UNIT));

		HAL_CmdBufIn(TAG(TAG_DATE_MTAG));
		HAL_CmdBufIn(VERTEX2F(TIME_D_XM*EVE_PIXEL_UNIT,TIME_YT*EVE_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((TIME_D_XM+2*TIME_W)*EVE_PIXEL_UNIT,(TIME_YT+5*TIME_H)*EVE_PIXEL_UNIT));

		HAL_CmdBufIn(TAG(TAG_DATE_RTAG));
		HAL_CmdBufIn(VERTEX2F(TIME_D_XR*EVE_PIXEL_UNIT,TIME_YT*EVE_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((TIME_D_XR+2*TIME_W)*EVE_PIXEL_UNIT,(TIME_YT+5*TIME_H)*EVE_PIXEL_UNIT));
	}
	HAL_CmdBufIn(TAG_MASK(0));

	/* a highlight select rectangle */
	HAL_CmdBufIn(COLOR_TIME);
	drawTable(&tblS);
	HAL_CmdBufIn(COLOR_BKGRD);

	/* set the track tag to each part */
	if (timeORdate) {
		CoCmd_TRACK(TIME_XL,TIME_YT,2*TIME_W,5*TIME_H,TAG_CLCK_LTAG);
		CoCmd_TRACK(TIME_XM,TIME_YT,2*TIME_W,5*TIME_H,TAG_CLCK_MTAG);
		CoCmd_TRACK(TIME_XR,TIME_YT,2*TIME_W,5*TIME_H,TAG_CLCK_RTAG);	
	} else {
		CoCmd_TRACK(TIME_D_XL,TIME_YT,4*TIME_W,5*TIME_H,TAG_DATE_LTAG);
		CoCmd_TRACK(TIME_D_XM,TIME_YT,2*TIME_W,5*TIME_H,TAG_DATE_MTAG);
		CoCmd_TRACK(TIME_D_XR,TIME_YT,2*TIME_W,5*TIME_H,TAG_DATE_RTAG);	
	}

	/* 
	 * bitmap for change value column border 
	 */
	HAL_CmdBufIn(BEGIN(BITMAPS));
	if (timeORdate) {
		for (i = 0, k = TIME_XL; i < 3; i++, k += 3*TIME_W) {
			for (j = 0; j < 2; j++) {
                HAL_CmdBufIn(BITMAP_HANDLE(0));
                HAL_CmdBufIn(VERTEX2F((k+j*TIME_W)*EVE_PIXEL_UNIT,TIME_YT*EVE_PIXEL_UNIT));
                HAL_CmdBufIn(BITMAP_HANDLE(1));
                HAL_CmdBufIn(VERTEX2F((k+j*TIME_W)*EVE_PIXEL_UNIT,(TIME_YB-TIME_H/2)*EVE_PIXEL_UNIT));
			}
		}
	} else {
		for (i = 0, k = TIME_D_XL; i < 3; i++, k += ((i==1)?5*TIME_W:3*TIME_W)) {
			for (j = 0; j < (i?(FTU32)2:(FTU32)4); j++) {
                HAL_CmdBufIn(BITMAP_HANDLE(0));
                HAL_CmdBufIn(VERTEX2F((k+j*TIME_W)*EVE_PIXEL_UNIT,TIME_YT*EVE_PIXEL_UNIT));
                HAL_CmdBufIn(BITMAP_HANDLE(1));
                HAL_CmdBufIn(VERTEX2F((k+j*TIME_W)*EVE_PIXEL_UNIT,(TIME_YB-TIME_H/2)*EVE_PIXEL_UNIT));
			}
		}
	}
	HAL_CmdBufIn(END());
	HAL_CmdBufIn(COLOR_TIME);
	if (timeORdate) {
		drawChgCol(TIME_XL+TIME_W,TIME_YT+5*TIME_H/2,5*TIME_H,FONT_TIME_DAT,TIME_H,pstDT->hours,0,TIME_H_MAX);
		drawChgCol(TIME_XM+TIME_W,TIME_YT+5*TIME_H/2,5*TIME_H,FONT_TIME_DAT,TIME_H,pstDT->minutes,0,TIME_M_MAX);
		drawChgCol(TIME_XR+TIME_W,TIME_YT+5*TIME_H/2,5*TIME_H,FONT_TIME_DAT,TIME_H,pstDT->seconds,0,TIME_S_MAX);
	} else {
		drawChgCol(TIME_D_XL+2*TIME_W,TIME_YT+5*TIME_H/2,5*TIME_H,FONT_TIME_DAT,TIME_H,pstDT->year,1900,TIME_D_Y_MAX);
		drawChgCol(TIME_D_XM+TIME_W,TIME_YT+5*TIME_H/2,5*TIME_H,FONT_TIME_DAT,TIME_H,pstDT->month,1,TIME_D_M_MAX);
		drawChgCol(TIME_D_XR+TIME_W,TIME_YT+5*TIME_H/2,5*TIME_H,FONT_TIME_DAT,TIME_H,pstDT->day,1,TIME_D_D_MAX);
	}
}

static FTVOID chgValue (FTU8 Tag, datetime_st *pstDT)
{
	FTU32 centrlX, *tmp, max;
	
	/* set the number on column */
	HAL_CmdBufIn(COLOR_TIME);
	switch (Tag) {
		case TAG_CLCK_LTAG:
			tmp = &pstDT->hours;
			centrlX = TIME_XL+TIME_W;
			max = TIME_H_MAX;
			break;
		case TAG_CLCK_MTAG:
			tmp = &pstDT->minutes;
			centrlX = TIME_XM+TIME_W;
			max = TIME_M_MAX;
			break;
		case TAG_CLCK_RTAG:
			tmp = &pstDT->seconds;
			centrlX = TIME_XR+TIME_W;
			max = TIME_S_MAX;
			break;
		case TAG_DATE_LTAG:
			tmp = &pstDT->year;
			centrlX = TIME_D_XL+2*TIME_W;
			max = TIME_D_Y_MAX;
			break;
		case TAG_DATE_MTAG:
			tmp = &pstDT->month;
			centrlX = TIME_D_XM+TIME_W;
			max = TIME_D_M_MAX;
			break;
		case TAG_DATE_RTAG:
			tmp = &pstDT->day;
			centrlX = TIME_D_XR+TIME_W;
			max = TIME_D_D_MAX;
			break;

		default:
			return;
	}
	
	if (TAG_DATE_MTAG == Tag || TAG_DATE_RTAG == Tag ) {
		*tmp = getTrackNum(&track_ctrl,HAL_Read32(REG_TRACKER)>>16,*tmp,1,max);
		drawChgCol(centrlX,TIME_YT+5*TIME_H/2,5*TIME_H,FONT_TIME_DAT,TIME_H,*tmp,1,max);
	} else if (TAG_DATE_LTAG == Tag) {
		*tmp = getTrackNum(&track_ctrl,HAL_Read32(REG_TRACKER)>>16,*tmp,1900,max);
		drawChgCol(centrlX,TIME_YT+5*TIME_H/2,5*TIME_H,FONT_TIME_DAT,TIME_H,*tmp,0,max);
	} else {
		*tmp = getTrackNum(&track_ctrl,HAL_Read32(REG_TRACKER)>>16,*tmp,0,max);
		drawChgCol(centrlX,TIME_YT+5*TIME_H/2,5*TIME_H,FONT_TIME_DAT,TIME_H,*tmp,0,max);
	}
}
static FTVOID walk (datetime_st *pT, FTU32 init)
{
	static FTU32 iBakTick;
	FTU32 iTick;

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(MSVC2017EMU) 
		iTick = (FTU32)GetTickCount();
#elif defined(STM32F4)
		iTick = 0;
#else
 		iTick = (FTU32)millis();
#endif

	if (init) {
		iBakTick = iTick;
		return;
	}

	/* just for demo, only trigger the second hand to run */
	pT->minseconds = (iTick - iBakTick);
}

static FTVOID defaultMatrix(FTVOID)
{
	HAL_CmdBufIn(BITMAP_TRANSFORM_A(256));
	HAL_CmdBufIn(BITMAP_TRANSFORM_B(0));
	HAL_CmdBufIn(BITMAP_TRANSFORM_C(0));
	HAL_CmdBufIn(BITMAP_TRANSFORM_D(0));
	HAL_CmdBufIn(BITMAP_TRANSFORM_E(256));
	HAL_CmdBufIn(BITMAP_TRANSFORM_F(0));
}

static FTVOID aniMoveup (FTVOID)
{
	FT32 X,Y;
	/* animation for logo moving up */
	for (Y = EVE_LCD_HIGH, X = EVE_LCD_WIDTH/2-BOOT_LOGO_W/2; Y > EVE_LCD_HIGH/2-BOOT_LOGO_H/2; Y--) {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(COLOR_CLR_B1);
		HAL_CmdBufIn(CLEAR(1,1,1));

		/* bitmap area setting */
		HAL_CmdBufIn(BEGIN(BITMAPS));
        HAL_CmdBufIn(BITMAP_HANDLE(BMP_H_0));
        HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
	}
}

FTVOID aniZoomin (FTVOID)
{
	FT32 X,Y,step;
	
	X = EVE_LCD_WIDTH/2-BOOT_LOGO_W/2;
	Y = EVE_LCD_HIGH/2-BOOT_LOGO_H/2;

	/* animation for logo zoom in */
	for (step = BOOT_LOGO_ZOOM; step >= 1; step--) {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(COLOR_CLR_B1);
		HAL_CmdBufIn(CLEAR(1,1,1));

		/* bitmap area setting */
		HAL_CmdBufIn(BEGIN(BITMAPS));
		CoCmd_LOADIDENTITY;
		CoCmd_TRANSLATE(BOOT_LOGO_W/2*EVE_TRANSFORM_MAX,BOOT_LOGO_H/2*EVE_TRANSFORM_MAX);
		CoCmd_SCALE(EVE_TRANSFORM_MAX/BOOT_LOGO_ZOOM*step,EVE_TRANSFORM_MAX/BOOT_LOGO_ZOOM*step);
		CoCmd_TRANSLATE(-BOOT_LOGO_W/2*EVE_TRANSFORM_MAX,-BOOT_LOGO_H/2*EVE_TRANSFORM_MAX);
		CoCmd_SETMATRIX;
        HAL_CmdBufIn(BITMAP_HANDLE(BMP_H_0));
        HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
	}
}

FTVOID aniCube (FTVOID)
{
	FT32 X,Y,iX;
	
	/* animation for cube moving */
	for (Y = 0, iX = 0;Y < EVE_LCD_HIGH; iX += BOOT_CUBE_W) {
		if (iX >= EVE_LCD_WIDTH) {
			Y += BOOT_CUBE_H;
			iX = 0;
		}
		for (X = EVE_LCD_WIDTH; X > iX; X -= BOOT_CUBE_W) {
			HAL_CmdBufIn(CMD_DLSTART);
			HAL_CmdBufIn(COLOR_CLR_B1);
			HAL_CmdBufIn(CLEAR(1,1,1));

			HAL_CmdBufIn(BEGIN(RECTS));
			HAL_CmdBufIn(COLOR_BOT);
			HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
			HAL_CmdBufIn(VERTEX2F((X+BOOT_CUBE_W)*EVE_PIXEL_UNIT,(Y+BOOT_CUBE_H)*EVE_PIXEL_UNIT));

			if (iX) {
				HAL_CmdBufIn(BEGIN(EDGE_STRIP_L));
				HAL_CmdBufIn(VERTEX2F(iX*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
				HAL_CmdBufIn(VERTEX2F(iX*EVE_PIXEL_UNIT,(Y+BOOT_CUBE_H)*EVE_PIXEL_UNIT));
				HAL_CmdBufIn(END());
			}

			if (Y) {
				HAL_CmdBufIn(BEGIN(EDGE_STRIP_A));
				HAL_CmdBufIn(VERTEX2F(0,Y*EVE_PIXEL_UNIT));
				HAL_CmdBufIn(VERTEX2F(EVE_LCD_WIDTH*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
				HAL_CmdBufIn(END());
			}

			HAL_CmdBufIn(DISPLAY());
			HAL_CmdBufIn(CMD_SWAP);
			HAL_BufToReg(RAM_CMD,0);
		}
	}

}

FTVOID aniGradient(FTVOID)
{
	FT32 Y;

	/* animation for background gradient */
	for (Y = 0; Y < EVE_LCD_HIGH; Y += 10) {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(COLOR_CLR_B0);
		HAL_CmdBufIn(CLEAR(1,1,1));
		CoCmd_GRADIENT(0,0,0x3f61a1,0,Y,0);
		
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
	}
}

FTVOID cashBootup (FTU32 cashPara)
{
	appGP.appPara = 0;
	
	appGP.appIndex = CASH_BOOT;
	gData = CASH_MAIN;
#if defined(STM32F4)
	return;
#endif
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(MSVC2017EMU) 
	/* load bitmap resources data into EVE */
	if(APP_OK != appBmpToRamG(BMP_H_0, BOOT_BMP_ADDR, bmpLogoHD, 1)) {
		DBGPRINT;
		return;
	}

	FTDELAY(1000);
	
	aniMoveup();
	
	aniZoomin();

	//aniCube();

	aniGradient();

	/* TODO: for debugging */
	appGP.appIndex = CASH_MAIN;
	gData = CASH_MAIN;
#else
	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(COLOR_CLR_B0);
	HAL_CmdBufIn(CLEAR(1,1,1));

	CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/4,24,OPT_CENTERX,"Due to limited memory in Arduino");
	CoCmd_TEXT(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/4*3,24,OPT_CENTERX,"Please run me under the MSVC");

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
#endif
	return;
}

FTVOID cashMain (FTU32 cashPara)
{
	FTU8 Touch = 0, tFlag = TOUCH_NO, i, Rotate = ICON_ROTATE_D;
	FT_Gpu_Fonts_t stF;

	/* load the font resources into EVE */
	if (0 == appFileToRamG(PATH_ICON_T,FONT_MENU_1_ADDR,0,(FTU8 *)&stF,sizeof(FT_Gpu_Fonts_t))) {
		DBGPRINT;
		return;
	}

	/* load bitmap resources data into EVE */
	if(APP_OK != appBmpToRamG(BMP_H_0, MENU_BMP_ADDR, bmpHD, ICON_NUM)){
		DBGPRINT;
		return;
	}

	while (1) {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(COLOR_CLR);
		HAL_CmdBufIn(CLEAR(1,1,1));
		COLOR_GRA;

		/* set the external font to each font index */
		CoCmd_SETFONT(FONT_MENU, FONT_MENU_1_ADDR, &stF);

		/* bitmap area setting */
		HAL_CmdBufIn(BEGIN(BITMAPS));
		switch (Touch) {
			case TAG_CASH_REST:
			case TAG_CASH_NETS:
			case TAG_CASH_FAKE:
			case TAG_CASH_TIME:
			case TAG_CASH_VERS:
			case TAG_CASH_DBSM:
				appGP.appIndex = CASH_REST+(Touch-TAG_CASH_REST);
				gData = appGP.appIndex;
				i =  Touch-TAG_CASH_REST;
				appGP.appPara = (FTU32)&gData;
				tFlag = TOUCH_PRESS;
				Rotate += ICON_ROTATE_D;

				/* rotate the touched icon */
				HAL_CmdBufIn(TAG(TAG_CASH_REST+i));
				CoCmd_LOADIDENTITY;
				CoCmd_TRANSLATE(ICON_W/2*EVE_TRANSFORM_MAX,ICON_H/2*EVE_TRANSFORM_MAX);
				CoCmd_ROTATE(Rotate*EVE_TRANSFORM_MAX/360);
				CoCmd_TRANSLATE(-ICON_W/2*EVE_TRANSFORM_MAX,-ICON_H/2*EVE_TRANSFORM_MAX);
				CoCmd_SETMATRIX;
                HAL_CmdBufIn(BITMAP_HANDLE(i));
                HAL_CmdBufIn(VERTEX2F(ICON_X0(i)*EVE_PIXEL_UNIT,ICON_Y0(i)*EVE_PIXEL_UNIT));
				defaultMatrix();
				HAL_CmdBufIn(COLOR_TITLE);
				CoCmd_TEXT(ICON_T_X(i),ICON_T_Y(i), FONT_MENU, OPT_CENTERX, &iconStr[i][0]);
				break;
			default:
				tFlag = (TOUCH_PRESS == tFlag)?TOUCH_RELES:tFlag;
				break;
		}
		for (i = 0; i < ICON_NUM; i++) {
			
			if (tFlag && (i == (Touch-TAG_CASH_REST))) {

				/* skip the previous display icon */
				continue;
			}
			/* keep showing the rest of the icon */
			HAL_CmdBufIn(TAG(TAG_CASH_REST+i));
			defaultMatrix();
            HAL_CmdBufIn(BITMAP_HANDLE(i));
            HAL_CmdBufIn(VERTEX2F(ICON_X0(i)*EVE_PIXEL_UNIT,ICON_Y0(i)*EVE_PIXEL_UNIT));
			/* display the title on each icon by using external font */
			HAL_CmdBufIn(COLOR_TITLE);
			CoCmd_TEXT(ICON_T_X(i),ICON_T_Y(i), FONT_MENU, OPT_CENTERX, &iconStr[i][0]);
		}	
		HAL_CmdBufIn(END());

		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);

		if (TOUCH_RELES == tFlag) {
			return;
		}

		/* detecting the touch event */
		Touch = HAL_Read8(REG_TOUCH_TAG);
	}
}

FTVOID cashVers (FTU32 cashPara)
{
	FT_Gpu_Fonts_t stFB, stFT1, stFT2;
	/* has to be ended with a 0 on each title */
	FTC8 butnStr[] = {1,2,0};//go back
	FTU8 Touch = 0, tFlag = TOUCH_NO;

	FTU32 tblRowHigh[TABLE_ROW] = {
		40, 30, 30, 30, 30, 30, 30 , 30
	};
	FTU32 tblColumnWide[TABLE_COL] = {
		70, 140, 140
	};
	table_layout_st tbl = {
		TABLE_X,
		TABLE_Y,
		TBL_ROW,
		TABLE_ROW,
		tblRowHigh,
		TABLE_COL,
		tblColumnWide
	};

	/* load the font resources into EVE */
	if (0 == appFileToRamG(PATH_BUTN,FONT_BUTN_ADDR,0,(FTU8 *)&stFB,sizeof(FT_Gpu_Fonts_t))) {
		DBGPRINT;
		return;
	}
	if (0 == appFileToRamG(PATH_VER_T,FONT_VERS_1_ADDR,0,(FTU8 *)&stFT1,sizeof(FT_Gpu_Fonts_t))) {
		DBGPRINT;
		return;
	}
	if (0 == appFileToRamG(PATH_VER_C,FONT_VERS_2_ADDR,0,(FTU8 *)&stFT2,sizeof(FT_Gpu_Fonts_t))) {
		DBGPRINT;
		return;
	}

	while (1) {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(COLOR_CLR);
		HAL_CmdBufIn(CLEAR(1,1,1));
		HAL_CmdBufIn(COLOR_TITLE);
		COLOR_GRA;

		/* set the external font to each font index */
		CoCmd_SETFONT(FONT_BUTN, FONT_BUTN_ADDR, &stFB);
		HAL_CmdBufIn(TAG(TAG_CASH_MAIN));

		if (Touch == TAG_CASH_MAIN) {
			appGP.appIndex = CASH_MAIN;
			gData = CASH_MAIN;
			appGP.appPara = (FTU32)&gData;
			tFlag = TOUCH_PRESS;
			CoCmd_FGCOLOR(COLOR_FG_PRESS);
		} else {
			/* press has a color, release has a color, then return */
			tFlag = (tFlag == TOUCH_PRESS)?TOUCH_RELES:tFlag;
			CoCmd_FGCOLOR(COLOR_FG_RELES);
		}

		CoCmd_BUTTON(BUTN_X(0),BUTN_Y,BUTN_W, BUTN_H, FONT_BUTN,0,butnStr);
		HAL_CmdBufIn(TAG_MASK(0));

		/* draw a table border */
		drawTable(&tbl);
		/* set the external font to each font index */
		CoCmd_SETFONT(FONT_TBLE_TLT, FONT_VERS_1_ADDR, &stFT1);
		CoCmd_SETFONT(FONT_TBLE_DES, FONT_VERS_2_ADDR, &stFT2);

		/* fill in data to table */
		showData(&tbl,tblData);


		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);

		if (tFlag == TOUCH_RELES) {
			return;
		}

		/* detecting the touch event */
		Touch = HAL_Read8(REG_TOUCH_TAG);
	}

	return;
}

FTVOID cashTime (FTU32 cashPara)
{
	FT_Gpu_Fonts_t stFB;
	/* has to be ended with a 0 on each title */
	FTC8 cancelStr[] = {1,2,0};//go back
	FTC8 okStr[] = {3,4,0};//ok
	TIME_EVENT event = NO_CHNG;
	static FTU8 Touch;
	FTU8 okFlag = 0, cancelFlag = 0;

	/* load the font resources into EVE */
	if (0 == appFileToRamG(PATH_BUTN,FONT_BUTN_ADDR,0,(FTU8 *)&stFB,sizeof(FT_Gpu_Fonts_t))) {
		DBGPRINT;
		return;
	}

	/* load bitmap resources data into EVE */
	if(APP_OK != appBmpToRamG(BMP_H_0, TIME_BMP_ADDR, bmpNumHD, TIME_BMP_NUM)){
		DBGPRINT;
		return;
	}

	Touch = 0;
	/* init the internal counter */
	walk(&curTime,1);
	while (1) {
		/* make the clock walk while not in config mode */
		walk(&curTime,Touch);
		
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(COLOR_CLR);
		HAL_CmdBufIn(CLEAR(1,1,1));
		/* set chinese cancel and ok string to bottom */
		HAL_CmdBufIn(COLOR_TITLE);
		COLOR_GRA;
		CoCmd_SETFONT(FONT_BUTN, FONT_BUTN_ADDR, &stFB);


		HAL_CmdBufIn(TAG(TAG_CASH_TIME));
		if (TAG_CASH_TIME == Touch) {
			okFlag = 1;
			CoCmd_FGCOLOR(COLOR_FG_PRESS);
			CoCmd_BUTTON(BUTN_X(1),BUTN_Y,BUTN_W, BUTN_H, FONT_BUTN,0,okStr);
			event = NO_CHNG;
		} else {
			okFlag = (okFlag == 1)?2:okFlag;
			CoCmd_FGCOLOR(COLOR_FG_RELES);
			CoCmd_BUTTON(BUTN_X(1),BUTN_Y,BUTN_W, BUTN_H, FONT_BUTN,0,okStr);
		}

		HAL_CmdBufIn(TAG(TAG_CASH_MAIN));
		if (TAG_CASH_MAIN == Touch) {
			cancelFlag = 1;
			CoCmd_FGCOLOR(COLOR_FG_PRESS);
			CoCmd_BUTTON(BUTN_X(0),BUTN_Y,BUTN_W, BUTN_H, FONT_BUTN,0,cancelStr);
			appGP.appIndex = CASH_MAIN;
			gData = CASH_MAIN;
			appGP.appPara = 0;
		} else {
			cancelFlag = (cancelFlag == 1)?2:cancelFlag;
			CoCmd_FGCOLOR(COLOR_FG_RELES);
			CoCmd_BUTTON(BUTN_X(0),BUTN_Y,BUTN_W, BUTN_H, FONT_BUTN,0,cancelStr);
		}

		switch (event) {
			case TIME_CHNG:
				HAL_CmdBufIn(TAG_MASK(1));
				drawCol(&curTime, 1);
				break;
			case DATE_CHNG:
				HAL_CmdBufIn(TAG_MASK(1));
				drawCol(&curTime, 0);
				break;
			case NO_CHNG:
			default:
				/* tag and display clock */
				HAL_CmdBufIn(TAG_MASK(1));
				/* draw a clock */
				HAL_CmdBufIn(TAG(TAG_CLCK)); 
				CoCmd_CLOCK(EVE_LCD_WIDTH/2,EVE_LCD_HIGH/2,TIME_R,0,
						0,0,curTime.hours*3600+curTime.minutes*60+curTime.seconds,curTime.minseconds);
				/* draw a date */
				HAL_CmdBufIn(TAG(TAG_DATE)); 
				CoCmd_NUMBER(EVE_LCD_WIDTH/2-3*TIME_W+10,0,FONT_TIME_DAT,0,curTime.year);
				CoCmd_TEXT(EVE_LCD_WIDTH/2-TIME_W+15,0,FONT_TIME_DAT,0,"/");
				CoCmd_NUMBER(EVE_LCD_WIDTH/2,0,FONT_TIME_DAT,0,curTime.month);
				CoCmd_TEXT(EVE_LCD_WIDTH/2+TIME_W+3,0,FONT_TIME_DAT,0,"/");
				CoCmd_NUMBER(EVE_LCD_WIDTH/2+TIME_W+8,0,FONT_TIME_DAT,0,curTime.day);
				HAL_CmdBufIn(TAG_MASK(0));
				break;
		}

		switch (Touch) {
			case TAG_CLCK:
			case TAG_CLCK_LTAG:
			case TAG_CLCK_MTAG:
			case TAG_CLCK_RTAG:
				chgValue(Touch,&curTime);
				event = TIME_CHNG;
				break;
			case TAG_DATE:
			case TAG_DATE_LTAG:
			case TAG_DATE_MTAG:
			case TAG_DATE_RTAG:
				chgValue(Touch,&curTime);
				event = DATE_CHNG;
				break;
			default:
				break;
		}

		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);

		if (okFlag == 2 || cancelFlag == 2) {
			return;
		}

		/* detect if touch happen */
		Touch = HAL_Read8(REG_TOUCH_TAG);
	}
}

FTVOID cashRest (FTU32 cashPara)
{
	devp(cashPara);
}

FTVOID cashNets (FTU32 cashPara)
{

	devp(cashPara);
}

FTVOID cashFake (FTU32 cashPara)
{

	devp(cashPara);
}

FTVOID cashDbsm (FTU32 cashPara)
{
	devp(cashPara);
}

AppFunc APPS_UI[] = {
	cashBootup,
	cashMain,
	cashRest,
	cashNets,
	cashFake,
	cashTime,
	cashVers,
	cashDbsm
};

