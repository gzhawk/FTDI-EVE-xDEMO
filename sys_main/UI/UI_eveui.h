/* 
    Demo to give a UI design structure idea
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2015/May
*/

/* when use bubble effect, define it 
*/
#define USE_BUBBLE

/* go APPS_UI[] define to count the number */
typedef enum APPINDEX_e {
	INDEXBOOTUP = 0,
	INDEXMAIN,
	INDEXSTANDBY,
	INDEXEND
} APPINDEX;

typedef enum TAGINDEX_e {
	TAG_STBY = 10,
	TAG_MENU
} TAGINDEX;

#define PURE_RGB(red,green,blue) ((((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0))
#define COLOR_BAKGRD_CLR CLEAR_COLOR_RGB(236, 240, 241) /* background color */
#define COLOR_TXT_BOOTUP COLOR_RGB(124,156,188)         /* bootup text color */
#define COLOR_PNT_BAKGRD COLOR_RGB(236, 240, 241)       /* same as CLEAR_COLOR_RGB */
#define COLOR_PNT_STANDBY COLOR_RGB(124,156,188)        /* lock screen icon color */
#define PCOLOR_MENU       PURE_RGB(124,156,188)         /* menu front color */
#define COLOR_MENU_PRESS  COLOR_RGB(142, 136, 130)      /* after press menu color */
#define PCOLOR_MENU_PRESS PURE_RGB(142, 136, 130)       /* after press right space color */
#define COLOR_MENU_TITLE  COLOR_RGB(189,194,190)        /* menu text color */
/* it's a tricky number,
 * depend your own device,
 * you need to test to get it*/
#define PRESS_HOLD_WAIT 150
/* for Arduino platform 64 is the MAX, 
 * or Arduino would run out of memory */
#define BUBBLE_NUM  64

#define SUBMEN_OK   1
#define SUBMEN_ERR  0
#define SUBMEN_HIDE SUBMEN_ERR

#if defined(USE_BUBBLE)
typedef struct bubble_ {
	FT16 x, y;
	FT16 xOffset, yOffset;
} bubble_st;
bubble_st Bubble[BUBBLE_NUM];
#define BUBBLE_VALUE_1 4
#define BUBBLE_VALUE_2 8

#define BUBLE_BORDER 72
#define BUBLE_OUT_SIZE(a) (3 * (a) + BUBLE_BORDER)
#define BUBLE_IN_SIZE(a) (3 * (a))

#define COLOR_BUBLE_1 COLOR_RGB(255,255,255)/* smaller bubble color */
#define COLOR_BUBLE_2 COLOR_RGB(218,216,214)/* larger bubble color */
#define BUBLE_LO_1 0
#define BUBLE_HI_1 (BUBBLE_NUM/2-1)
#define BUBLE_LO_2 (BUBBLE_NUM/2)
#define BUBLE_HI_2 BUBBLE_NUM
#define BUBLE_OUT_ALPHA 0x20
#define BUBLE_IN_ALPHA  0xFF

FTINDEF FT8 offset(FTVOID)
{
	return FTRANDOM(BUBBLE_VALUE_2) - BUBBLE_VALUE_1;
}
#endif

#if defined(USE_BUBBLE)
FTINDEF FTVOID init_bubble (FTVOID)
{
	FTU32 i;

	/* init the bubble effect value */
	for (i = 0; i < BUBBLE_NUM; i++) {
		Bubble[i].x = FTRANDOM(FT800_PIXEL_UNIT*FT800_LCD_WIDTH);
		Bubble[i].y = FTRANDOM(FT800_PIXEL_UNIT*FT800_LCD_HIGH);
	}
}

FTINDEF FTVOID pos_bubble (bubble_st *p)
{
	FTU8 k;

	for (k = 0; k < BUBBLE_NUM; ++k) {
		p[k].yOffset--; /* effect is moving up */
		p[k].x += p[k].xOffset;
		if (p[k].x < FT800_PIXEL_UNIT*FT800_LCD_WIDTH/3) {
			/* set the border of effect */
			p[k].x = FT800_PIXEL_UNIT*FT800_LCD_WIDTH/3 + BUBLE_OUT_SIZE(BUBLE_HI_2);
		}
		p[k].y += p[k].yOffset;
	}

	p[FTRANDOM(BUBBLE_NUM)].xOffset = offset();
	p[FTRANDOM(BUBBLE_NUM)].yOffset = offset();

}
#endif

FTINDEF FTU32 ui_bootup_eve_show (FTVOID)
{
#ifdef DEF_81X
#define BOOTUP_FONT       34
#else
#define BOOTUP_FONT       31
#endif
#define X_E1_POS (FT800_LCD_WIDTH/3)
#define Y_V_POS  (FT800_LCD_HIGH/2)
#define X_E2_POS (FT800_LCD_WIDTH/3*2)
#define POS_SEED   (15)
#define ARPHA_WORD (255)
#define ARPHA_SEED (5)
	static FTU32 X_e1 = 0,
				 Y_e1 = FT800_LCD_HIGH/2, 
				 X_v  = FT800_LCD_WIDTH/2, 
				 Y_v  = FT800_LCD_HIGH,
				 X_e2 = FT800_LCD_WIDTH,
				 Y_e2 = FT800_LCD_HIGH/2;
	static FTU8  A_w = ARPHA_WORD;
	FTU8 font = BOOTUP_FONT;

#if defined(USE_BUBBLE)
	/* do some position change ahead for later better effect */
	pos_bubble(Bubble);
#endif

#ifdef DEF_81X
	font = 1;
	CoCmd_ROMFONT(font,BOOTUP_FONT);
#endif
	HAL_CmdBufIn(COLOR_TXT_BOOTUP);
	
	HAL_CmdBufIn(COLOR_A(A_w));
	
	CoCmd_TEXT(X_e1,Y_e1,font,OPT_CENTERY,"E");
	CoCmd_TEXT(X_v,Y_v,font,OPT_CENTERY,"V");
	CoCmd_TEXT(X_e2,Y_e2,font,OPT_CENTERY,"E");

	if (X_e1 < X_E1_POS) {
		X_e1 += POS_SEED;
	} else {
		X_e1 = X_E1_POS;
	}

	if (Y_v > Y_V_POS) {
		Y_v  -= POS_SEED;
	} else {
		Y_v  = Y_V_POS;
	}

	if (X_e2 > X_E2_POS) {
		X_e2 -= POS_SEED;
	} else {
		X_e2 = X_E2_POS;
	}

	if ((X_e1 == X_E1_POS) && (Y_v == Y_V_POS) && (X_e2 == X_E2_POS)) {
		if (A_w) {
			A_w -= ARPHA_SEED;
			return 1;
		} else {
			return 0;
		}
	} else {
		return 1;
	}
}

FTVOID ui_bootup (FTU32 para)
{
	FTU32 ret = 1;

#if defined(USE_BUBBLE)
	init_bubble();
#endif

	while (ret) {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(COLOR_BAKGRD_CLR);
		HAL_CmdBufIn(CLEAR(1,1,1));

		ret = ui_bootup_eve_show();

		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);

		appGP.appIndex = INDEXMAIN;
	}
}

FTINDEF FTU32 ui_standby_point_show (FTU32 X)
{
	FTU32 ret = 1;
	static FTU8 alpha = 0, flag = 0;
	static FTU32 size = 5;
#define KEY_LEN            (FT800_LCD_HIGH/4)
#define KEY_LINE1_X1        (FT800_LCD_WIDTH/4*FT800_PIXEL_UNIT)
#define KEY_LINE1_Y1        ((FT800_LCD_HIGH/2-KEY_LEN/2)*FT800_PIXEL_UNIT)
#define KEY_LINE1_Y2        (FT800_LCD_HIGH/2*FT800_PIXEL_UNIT)
#define KEY_POINT_X1       KEY_LINE1_X1
#define KEY_POINT_Y1       (KEY_LINE1_Y1+KEY_LEN/8*7*FT800_PIXEL_UNIT)
#define KEY_POINT_SIZE_OUT (KEY_LEN/8*FT800_PIXEL_UNIT)
#define KEY_POINT_SIZE_IN  (KEY_LEN/16*FT800_PIXEL_UNIT)
#define KEY_LINE_W        (KEY_POINT_SIZE_OUT/4)
#define KEY_LINE2_X1        (KEY_LINE1_X1+KEY_LINE_W)
#define KEY_LINE2_Y1        (KEY_LINE1_X1+KEY_LINE_W)
#define KEY_LINE2_Y2        (KEY_LINE1_Y2+KEY_LEN/16*FT800_PIXEL_UNIT)

#define LOCK_X1             ((FT800_LCD_WIDTH/4*3-KEY_LEN/4)*FT800_PIXEL_UNIT)
#define LOCK_Y1             (FT800_LCD_HIGH/2*FT800_PIXEL_UNIT)
#define LOCK_X2             (LOCK_X1+KEY_LEN/2*FT800_PIXEL_UNIT)
#define LOCK_Y2             (LOCK_Y1+KEY_LEN/2*FT800_PIXEL_UNIT)
#define LOCK_POINT_X        (LOCK_X1+KEY_LEN/4*FT800_PIXEL_UNIT)
#define LOCK_POINT_Y        (LOCK_Y1+KEY_LEN/4*FT800_PIXEL_UNIT)
#define LOCK_LINE1_X        (FT800_LCD_WIDTH/4*3*FT800_PIXEL_UNIT)
#define LOCK_LINE1_Y        (LOCK_Y1-KEY_LEN/8*FT800_PIXEL_UNIT)
#define LOCK_LINE1_W        (KEY_LEN/8*FT800_PIXEL_UNIT)

	/* draw key */
	if (X < KEY_LINE1_X1/FT800_PIXEL_UNIT) {
		/* touch area */
		HAL_CmdBufIn(TAG_MASK(1));
		if (flag == 0) {
			alpha += 5;
			HAL_CmdBufIn(COLOR_A(alpha));
			flag = (alpha==0xFF)?1:0;
		} else {
			alpha -= 5;
			HAL_CmdBufIn(COLOR_A(alpha--));
			flag = (alpha==0)?0:1;
		}
		HAL_CmdBufIn(TAG(TAG_STBY));
		HAL_CmdBufIn(COLOR_MENU_TITLE);
		HAL_CmdBufIn(BEGIN(FTPOINTS));
		HAL_CmdBufIn(POINT_SIZE(KEY_LEN/2*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F(FT800_LCD_WIDTH/4*FT800_PIXEL_UNIT, FT800_LCD_HIGH/2*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(TAG_MASK(0));

		HAL_CmdBufIn(COLOR_A(0xFF));
		HAL_CmdBufIn(COLOR_PNT_STANDBY);
		HAL_CmdBufIn(BEGIN(LINES));
		HAL_CmdBufIn(LINE_WIDTH(KEY_LINE_W));
		HAL_CmdBufIn(VERTEX2F(KEY_LINE1_X1, KEY_LINE1_Y1));
		HAL_CmdBufIn(VERTEX2F(KEY_POINT_X1, KEY_POINT_Y1));
		HAL_CmdBufIn(LINE_WIDTH(KEY_LINE_W*2));
		HAL_CmdBufIn(VERTEX2F(KEY_LINE2_X1, KEY_LINE2_Y1));
		HAL_CmdBufIn(VERTEX2F(KEY_LINE2_X1, (KEY_LINE2_Y1+KEY_LEN/8*FT800_PIXEL_UNIT)));

		HAL_CmdBufIn(BEGIN(FTPOINTS));
		HAL_CmdBufIn(POINT_SIZE(KEY_POINT_SIZE_OUT));
		HAL_CmdBufIn(VERTEX2F(KEY_POINT_X1, KEY_POINT_Y1));

		HAL_CmdBufIn(COLOR_PNT_BAKGRD);
		HAL_CmdBufIn(POINT_SIZE(KEY_POINT_SIZE_IN ));
		HAL_CmdBufIn(VERTEX2F(KEY_POINT_X1, KEY_POINT_Y1));
		HAL_CmdBufIn(END());
	} else {
		/* touch area */
		HAL_CmdBufIn(COLOR_A(0xFF));
		HAL_CmdBufIn(TAG_MASK(1));
		HAL_CmdBufIn(TAG(TAG_STBY));
		HAL_CmdBufIn(COLOR_MENU_TITLE);
		HAL_CmdBufIn(BEGIN(FTPOINTS));
		HAL_CmdBufIn(POINT_SIZE(KEY_LEN/2*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT, FT800_LCD_HIGH/2*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(TAG_MASK(0));
		
		HAL_CmdBufIn(COLOR_PNT_STANDBY);
		HAL_CmdBufIn(BEGIN(LINES));
		HAL_CmdBufIn(LINE_WIDTH(KEY_LINE_W));
		HAL_CmdBufIn(VERTEX2F((X-KEY_LEN/2)*FT800_PIXEL_UNIT, KEY_LINE1_Y2));
		HAL_CmdBufIn(VERTEX2F((X+KEY_LEN/2)*FT800_PIXEL_UNIT, KEY_LINE1_Y2));
		HAL_CmdBufIn(LINE_WIDTH(KEY_LINE_W*2));
		HAL_CmdBufIn(VERTEX2F((X+KEY_LEN/8)*FT800_PIXEL_UNIT, KEY_LINE2_Y2));
		HAL_CmdBufIn(VERTEX2F((X+KEY_LEN/4)*FT800_PIXEL_UNIT, KEY_LINE2_Y2));

		HAL_CmdBufIn(BEGIN(FTPOINTS));
		HAL_CmdBufIn(POINT_SIZE(KEY_POINT_SIZE_OUT));
		HAL_CmdBufIn(VERTEX2F((X-KEY_LEN/2)*FT800_PIXEL_UNIT, KEY_LINE1_Y2));

		HAL_CmdBufIn(COLOR_PNT_BAKGRD);
		HAL_CmdBufIn(POINT_SIZE(KEY_POINT_SIZE_IN ));
		HAL_CmdBufIn(VERTEX2F((X-KEY_LEN/2)*FT800_PIXEL_UNIT, KEY_LINE1_Y2));
		HAL_CmdBufIn(END());
	}

	/* draw lock: body out */
	HAL_CmdBufIn(COLOR_PNT_STANDBY);
	HAL_CmdBufIn(BEGIN(RECTS));
	HAL_CmdBufIn(LINE_WIDTH(KEY_LINE_W));
	HAL_CmdBufIn(VERTEX2F(LOCK_X1, LOCK_Y1));
	HAL_CmdBufIn(VERTEX2F(LOCK_X2, LOCK_Y2));

	HAL_CmdBufIn(BEGIN(LINES));
	HAL_CmdBufIn(LINE_WIDTH(LOCK_LINE1_W));
	HAL_CmdBufIn(VERTEX2F(LOCK_LINE1_X, LOCK_LINE1_Y));
	HAL_CmdBufIn(VERTEX2F(LOCK_LINE1_X, (LOCK_LINE1_Y+KEY_LEN/8*FT800_PIXEL_UNIT)));

	/* draw lock: body in */
	HAL_CmdBufIn(COLOR_PNT_BAKGRD);
	HAL_CmdBufIn(BEGIN(FTPOINTS));
	if (X >= (LOCK_X1/FT800_PIXEL_UNIT)) {
		HAL_CmdBufIn(POINT_SIZE(KEY_POINT_SIZE_OUT+size*FT800_PIXEL_UNIT));
		size += 5;
		if (size >= KEY_LEN/2) {
			alpha = 0;
			flag = 0;
			size = 5;
			appGP.appIndex = INDEXMAIN;
			ret = 0;
		}
	} else {
		HAL_CmdBufIn(POINT_SIZE(KEY_POINT_SIZE_OUT));
		size = 5;
	}
	HAL_CmdBufIn(VERTEX2F(LOCK_POINT_X, LOCK_POINT_Y));
	HAL_CmdBufIn(END());

	return ret;
}

FTVOID ui_standby (FTU32 para)
{
	FTU32 ret = 1;

	while (ret) {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(COLOR_BAKGRD_CLR);
		HAL_CmdBufIn(CLEAR(1,1,1));

		if (TAG_STBY == HAL_Read8(REG_TOUCH_TAG)) {
			ret = ui_standby_point_show(HAL_Read32(REG_TOUCH_SCREEN_XY)>>16&0xFFFF);
		} else {
			ret = ui_standby_point_show(0);
		}

		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
	}
}

FTU8 menu_a (FTVOID)
{
#define G_X (FT800_LCD_WIDTH/3*2)
#define G_Y (FT800_LCD_HIGH/2)
#define G_R (FT800_LCD_HIGH/3)
#define G_L (FT800_LCD_HIGH-FT800_LCD_HIGH/16)
#define G_F (31)
#define G_MA 8
#define G_MI 3
#define G_RG 255
	static FTU8 value = 0, flag = 0;
	HAL_CmdBufIn(BEGIN(RECTS));
 	HAL_CmdBufIn(COLOR_MENU_PRESS);
	HAL_CmdBufIn(VERTEX2F(FT800_LCD_WIDTH/3*FT800_PIXEL_UNIT,0));
	HAL_CmdBufIn(VERTEX2F(FT800_LCD_WIDTH*FT800_PIXEL_UNIT,FT800_LCD_HIGH*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(END());

	HAL_CmdBufIn(COLOR_RGB(value, 255-value, 0));
	CoCmd_GAUGE(G_X,G_Y,G_R,OPT_NOPOINTER|OPT_NOBACK,G_MA,G_MI,0,G_RG);

	HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
	CoCmd_GAUGE(G_X,G_Y,G_R,OPT_NOTICKS|OPT_NOBACK,G_MA,G_MI,value,G_RG);

	HAL_CmdBufIn(COLOR_MENU_TITLE);
	CoCmd_NUMBER(G_X,G_Y+G_R,G_F,OPT_CENTER,value);
	
	HAL_CmdBufIn(BEGIN(LINES));
	HAL_CmdBufIn(VERTEX2F((G_X-G_R/2)*FT800_PIXEL_UNIT,G_L*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F((G_X+G_R/2)*FT800_PIXEL_UNIT,G_L*FT800_PIXEL_UNIT));

	if (value>=G_RG) {
		flag = 1;
	}
	if (value==0) {
		flag = 0;
	}
	value = (flag)?value-1:value+1;

	return SUBMEN_OK;
}

FTU8 menu_b (FTVOID)
{
#define PRG_X (FT800_LCD_WIDTH/3+FT800_LCD_WIDTH/12)
#define PRG_Y (FT800_LCD_HIGH/2)
#define PRG_W (FT800_LCD_WIDTH/3*2-FT800_LCD_WIDTH/6)
#define PRG_R (255)
#define PRG_F (31)
#define PRG_H (20)
	static FTU8 value = 0, flag = 0;

	HAL_CmdBufIn(BEGIN(RECTS));
 	HAL_CmdBufIn(COLOR_MENU_PRESS);
	HAL_CmdBufIn(VERTEX2F(FT800_LCD_WIDTH/3*FT800_PIXEL_UNIT,0));
	HAL_CmdBufIn(VERTEX2F(FT800_LCD_WIDTH*FT800_PIXEL_UNIT,FT800_LCD_HIGH*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(END());

	CoCmd_BGCOLOR(PURE_RGB(255,255,255));
	CoCmd_PROGRESS(PRG_X, PRG_Y, PRG_W, PRG_H, OPT_FLAT, value, PRG_R);

	HAL_CmdBufIn(COLOR_RGB(255, 255, 255-value));
	CoCmd_NUMBER((FT800_LCD_WIDTH/3*2),(FT800_LCD_HIGH/3),PRG_F,OPT_CENTER,value);

	if (value>=PRG_R) {
		flag = 1;
	}
	if (value==0) {
		flag = 0;
	}
	value = (flag)?value-1:value+1;

	return SUBMEN_OK;
}

FTU8 menu_c (FTVOID)
{
#define SLD_X1 (FT800_LCD_WIDTH/3+FT800_LCD_WIDTH/6)
#define SLD_X2 (FT800_LCD_WIDTH/3*2)
#define SLD_X3 (FT800_LCD_WIDTH/3*2+FT800_LCD_WIDTH/6)
#define SLD_Y (FT800_LCD_HIGH/6)
#define SLD_W (FT800_LCD_WIDTH/64)
#define SLD_H (FT800_LCD_HIGH/3*2)
#define SLD_RG (255)
	static FTU8 value = 0, value_m = 0, flag = 0, flag_m = 0;

	HAL_CmdBufIn(BEGIN(RECTS));
 	HAL_CmdBufIn(COLOR_MENU_PRESS);
	HAL_CmdBufIn(VERTEX2F(FT800_LCD_WIDTH/3*FT800_PIXEL_UNIT,0));
	HAL_CmdBufIn(VERTEX2F(FT800_LCD_WIDTH*FT800_PIXEL_UNIT,FT800_LCD_HIGH*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(END());

	CoCmd_BGCOLOR(PURE_RGB(0,value,0));
	CoCmd_FGCOLOR(PURE_RGB(255,255,255));
	CoCmd_SLIDER(SLD_X1,SLD_Y,SLD_W,SLD_H,0,value,SLD_RG);

	CoCmd_BGCOLOR(PURE_RGB(255,255,255));
	CoCmd_FGCOLOR(PURE_RGB(0,(255-value),0));
	CoCmd_SLIDER(SLD_X3,SLD_Y,SLD_W,SLD_H,0,255-value,SLD_RG);
	
	CoCmd_FGCOLOR(PURE_RGB(0,0,value_m));
	CoCmd_BGCOLOR(PURE_RGB(value_m,0,0));
	CoCmd_SLIDER(SLD_X2,SLD_Y,SLD_W,SLD_H,0,value_m,SLD_RG);

	if (value>=SLD_RG) {
		flag = 1;
	}
	if (value==0) {
		flag = 0;
	}
	value = (flag)?value-1:value+1;

	if (value_m>=SLD_RG) {
		flag_m = 1;
	}
	if (value_m==0) {
		flag_m = 0;
	}
	value_m = (flag_m)?value_m-5:value_m+5;

	return SUBMEN_OK;
}

#if defined(USE_BUBBLE)
FTINDEF FTVOID Bubbles(FTU8 lo, FTU8 hi, bubble_st *p)
{
	FTU8 i = lo;

	HAL_CmdBufIn(SAVE_CONTEXT());
	HAL_CmdBufIn(COLOR_RGB(0,0,0));
	HAL_CmdBufIn(COLOR_A(BUBLE_OUT_ALPHA));
	while (i < hi) {
		HAL_CmdBufIn(POINT_SIZE(BUBLE_OUT_SIZE(i)));
		HAL_CmdBufIn(VERTEX2F(p[i].x, p[i].y));
		i++;
	}
	HAL_CmdBufIn(RESTORE_CONTEXT());
	i = lo;
	while (i < hi) {
		HAL_CmdBufIn(POINT_SIZE(BUBLE_IN_SIZE(i)));
		HAL_CmdBufIn(VERTEX2F(p[i].x, p[i].y));
		i++;
	}
}

FTINDEF FTVOID draw_bubble(bubble_st *p)
{
	HAL_CmdBufIn(BEGIN(FTPOINTS));
	HAL_CmdBufIn(COLOR_A(BUBLE_IN_ALPHA));
	
	HAL_CmdBufIn(COLOR_BUBLE_1);
	Bubbles(BUBLE_LO_1, BUBLE_HI_1, p);

	HAL_CmdBufIn(COLOR_BUBLE_2);
	Bubbles(BUBLE_LO_2, BUBLE_HI_2, p);
}
#endif

FTU8 menu_blank (FTVOID)
{

#if defined(USE_BUBBLE)
	pos_bubble(Bubble);

	draw_bubble(Bubble);
#endif
	return SUBMEN_OK;
}

FTU8 menu_hide (FTVOID)
{

	(FTVOID) menu_blank();

	appGP.appIndex = INDEXSTANDBY;
	return SUBMEN_HIDE;
}

#define MENU_TITL_FONT   24
#define MENU_NUM         4
#define MENU_TITL_WORDS  30
#define MENU_ALPHA_MAX   255
#define MENU_EFFT_SPED   5 /* this number should be exact divistion by MENU_ALPHA_MAX */
#define MENU_EFFT_SEED   (MENU_ALPHA_MAX/MENU_EFFT_SPED)
FTU8 pMenuTitle[MENU_NUM][MENU_TITL_WORDS] = {
	"Menu: Gauge",	
	"Menu: Bar",	
	"Menu: Slider",	
	"Lock"
};

/* the return FTU8 can be used to let outer routine judge if exit is need */
typedef FTU8 (* MenuAppFunc) (FTVOID);

MenuAppFunc menuApp[] = {
	menu_blank,
	menu_a,
	menu_b,
	menu_c,
	menu_hide
};

FTINDEF FTU8 ui_main_title_show (FTU8 hide, FTU8 title_index)
{
	FTU32 i,tag;
	static FTU8 A_m = 0,j = 0,last_status = 1;

	if (last_status != hide) {
		if (hide) {
			if (j++ >= MENU_EFFT_SPED) {
				j = 0;
				if (A_m > 0) {
					A_m -= MENU_EFFT_SEED;
				} else if (A_m == 0) {					
					last_status = hide;
				}
			}
		} else {
			if (j++ >= MENU_EFFT_SPED) {
				j = 0;
				if (A_m < MENU_ALPHA_MAX) {
					A_m += MENU_EFFT_SEED;
				} else if (A_m == MENU_ALPHA_MAX) {					
					last_status = hide;
				}
			}
		}
	}

	HAL_CmdBufIn(TAG_MASK(1));
	for (i = 0,tag = TAG_MENU; i < MENU_NUM;i++,tag++) {
		HAL_CmdBufIn(TAG(tag));
		HAL_CmdBufIn(COLOR_A(A_m));
		if (i == (title_index - 1)) {
			CoCmd_FGCOLOR(PCOLOR_MENU_PRESS);
		} else {
			CoCmd_FGCOLOR(PCOLOR_MENU);
		}
		HAL_CmdBufIn(COLOR_MENU_TITLE);
		CoCmd_BUTTON(0,(FT800_LCD_HIGH/MENU_NUM)*i,FT800_LCD_WIDTH/3, \
				     FT800_LCD_HIGH/MENU_NUM,MENU_TITL_FONT,OPT_FLAT,&pMenuTitle[i][0]);
	}
	HAL_CmdBufIn(TAG_MASK(0));
	
	return ((last_status == hide)?1:0);
}

FTVOID ui_clock (FTVOID)
{
#define CLK_R   (FT800_LCD_WIDTH/32)
#define CLK_H2S (3600)
#define CLK_M2S (60)
#define CLK_H   (1)
#define CLK_M   (40)
	static FTU32 s=15; /* just the time when I coded here */

	CoCmd_CLOCK(FT800_LCD_WIDTH-CLK_R,CLK_R,CLK_R,OPT_NOBACK,0,0,(CLK_H*CLK_H2S+CLK_M*CLK_M2S+s++),0);

}
FTVOID ui_main (FTU32 para)
{
	FTU32 ret = 1;
	FTU8  touch,i = 0, ret_title, ret_app;
	static FTU8 last_index = 0;

	while (ret) {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(COLOR_BAKGRD_CLR);
		HAL_CmdBufIn(CLEAR(1,1,1));

		ret_app = menuApp[i]();
		
		ret_title = ui_main_title_show(!ret_app, i);
		
		ui_clock();
	
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);

		/* only when title effect finish, then active the touch */
		if (ret_title) {
			touch = HAL_Read8(REG_TOUCH_TAG);
			if (TAG_MENU <= touch && (TAG_MENU+MENU_NUM) >= touch) {
				FTDELAY(PRESS_HOLD_WAIT);
				if (touch != HAL_Read8(REG_TOUCH_TAG)) {
					/* change too fast, skip the action */
					continue;
				}

				i = touch - TAG_MENU + 1; /* skip the menu_blank routine */
				if (last_index == i) {
					/* double click would release the menu */
					i = 0;
				}
				last_index = i;
			}
		}

		if (ret_title == 1 && ret_app == 0) {
			ret = 0;
		}
	}
}

AppFunc APPS_UI[] = {
	ui_bootup,
	ui_main,
	ui_standby
};

