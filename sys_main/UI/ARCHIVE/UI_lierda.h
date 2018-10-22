/* 
    Sample code for Lierda
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2015/May
*/

/* go APPS_UI[] define to count the number */
typedef enum APPINDEX_e {
	I_LOCK = 0,
	I_MAIN,
	I_CLOCK,
	I_END
} APPINDEX;
#define COLOR_BAKGRD_CLR CLEAR_COLOR_RGB(33,33,33)

#define ALPHA_STEP  5

STATIC FTVOID screen_clean (FTVOID)
{
	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(COLOR_BAKGRD_CLR);
	HAL_CmdBufIn(CLEAR(1,1,1));
	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
}

FTVOID screen_lock (FTU32 para)
{
#define ULCK_TXT_P   ROOT_PATH"lierda\\ulck_txt.raw"
#define ULCK_BG_P    ROOT_PATH"lierda\\ulck_bg.raw"
#define ULCK_LCK_P   ROOT_PATH"lierda\\ulck_lck.raw"
#define ULCK_ULCK_P  ROOT_PATH"lierda\\ulck_ulck.raw"
#define ULCK_UOPN_P  ROOT_PATH"lierda\\ulck_uopn.raw"
#define ULCK_OPN_P   ROOT_PATH"lierda\\ulck_opn.raw"
#define TXT_ULCK_RAM RAM_G
#define TXT_ULCK_W   133
#define TXT_ULCK_H   32
#define TXT_ULCK_X   (EVE_LCD_WIDTH/2-TXT_ULCK_W/2)
#define TXT_ULCK_Y   (EVE_LCD_HIGH/3-TXT_ULCK_H/2)
#define BG_ULCK_W   360
#define BG_ULCK_H   96
#define BG_ULCK_X   (EVE_LCD_WIDTH/2-BG_ULCK_W/2)
#define BG_ULCK_Y   (EVE_LCD_HIGH/2-BG_ULCK_H/2)
#define ICON_ULCK_W   75
#define ICON_ULCK_H   75
#define L_ICON_ULCK_X   BG_ULCK_X
#define R_ICON_ULCK_X   (L_ICON_ULCK_X+BG_ULCK_W-ICON_ULCK_W)
#define ICON_ULCK_Y   (EVE_LCD_HIGH/2-ICON_ULCK_H/2)
#define TAG_LCK     10
#define GO_CLK      300
	enum bmp_hdl_ {
		ULCK_TXT_HDL = 0,
		ULCK_BG_HDL,
		ULCK_LCK_HDL,
		ULCK_ULCK_HDL,
		ULCK_UOPN_HDL,
		ULCK_OPN_HDL,
		ULCK_HDL_MAX
	};
	bmpHDR_st bmp_header[ULCK_HDL_MAX] = {
		      {ULCK_TXT_P, 0,0,ARGB4, 0,0, TXT_ULCK_W, TXT_ULCK_H},
		      {ULCK_BG_P, 0,0,ARGB4, 0,0, BG_ULCK_W, BG_ULCK_H},
		      {ULCK_LCK_P, 0,0,ARGB4, 0,0, ICON_ULCK_W, ICON_ULCK_H},
		      {ULCK_ULCK_P, 0,0,ARGB4, 0,0, ICON_ULCK_W, ICON_ULCK_H},
		      {ULCK_UOPN_P, 0,0,ARGB4, 0,0, ICON_ULCK_W, ICON_ULCK_H},
		      {ULCK_OPN_P, 0,0,ARGB4, 0,0, ICON_ULCK_W, ICON_ULCK_H},

	};
	static FTU32 f_loaded = 0, go_clock = 0;
	FTU8 tag,locked = 1,alpha = 255;
	FTU32 lck_hdl = ULCK_LCK_HDL,
		  opn_hdl = ULCK_UOPN_HDL,
		  lck_x = L_ICON_ULCK_X*EVE_PIXEL_UNIT;

	/* only load the file once */
	if (f_loaded == 0) {
		/* load bitmap resources data into EVE */
		if(APP_OK != appBmpToRamG(ULCK_TXT_HDL, TXT_ULCK_RAM, bmp_header, ULCK_HDL_MAX)){
			DBGPRINT;
			return;
		}
		f_loaded = 1;
	}

	while (1) {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(COLOR_BAKGRD_CLR);
		HAL_CmdBufIn(CLEAR(1,1,1));

		HAL_CmdBufIn(COLOR_A(alpha));
		HAL_CmdBufIn(BEGIN(BITMAPS));
		HAL_CmdBufIn(BITMAP_HANDLE(ULCK_TXT_HDL));
		HAL_CmdBufIn(VERTEX2F(TXT_ULCK_X*EVE_PIXEL_UNIT,TXT_ULCK_Y*EVE_PIXEL_UNIT));
		HAL_CmdBufIn(BITMAP_HANDLE(ULCK_BG_HDL));
		HAL_CmdBufIn(VERTEX2F(BG_ULCK_X*EVE_PIXEL_UNIT,BG_ULCK_Y*EVE_PIXEL_UNIT));

		HAL_CmdBufIn(TAG_MASK(1));
		HAL_CmdBufIn(TAG(TAG_LCK));
		if (locked) {
			HAL_CmdBufIn(BITMAP_HANDLE(lck_hdl));
			HAL_CmdBufIn(VERTEX2F(lck_x,ICON_ULCK_Y*EVE_PIXEL_UNIT));
		}
		HAL_CmdBufIn(TAG_MASK(0));
		HAL_CmdBufIn(BITMAP_HANDLE(opn_hdl));
		HAL_CmdBufIn(VERTEX2F(R_ICON_ULCK_X*EVE_PIXEL_UNIT,ICON_ULCK_Y*EVE_PIXEL_UNIT));
		HAL_CmdBufIn(END());

		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);

		/* unlock the screen to main */
		if (locked == 0) {
			alpha -= ALPHA_STEP;
			if (alpha == 0) {
				appGP.appIndex = I_MAIN;
				f_loaded = 0;
				go_clock = 0;
				screen_clean();
				return;
			}
			continue;
		}

		/* time up to go clock screen */
		if (go_clock == GO_CLK) {
			alpha -= ALPHA_STEP;
			if (alpha == 0) {
				appGP.appIndex = I_CLOCK;
				f_loaded = 0;
				go_clock = 0;
				screen_clean();
				return;
			}
			continue;
		}
		go_clock++;

		/* simulate the horizontal swip effect */
		tag = HAL_Read8(REG_TOUCH_TAG);
		if (tag == TAG_LCK) {
			lck_x = (HAL_Read32(REG_TOUCH_SCREEN_XY)>>16)&0xFFFF;
			if (lck_x >= L_ICON_ULCK_X && lck_x < R_ICON_ULCK_X) {
				lck_hdl = ULCK_ULCK_HDL;
				lck_x -= ICON_ULCK_W/2;
				lck_x *= EVE_PIXEL_UNIT;
			} else if (lck_x >= R_ICON_ULCK_X && lck_x <= (R_ICON_ULCK_X+ICON_ULCK_W)) {
				locked = 0;
				opn_hdl = ULCK_OPN_HDL;
			} else {
				lck_hdl = ULCK_LCK_HDL;
				lck_x = L_ICON_ULCK_X*EVE_PIXEL_UNIT;
			}
			go_clock = 0;
		} else {
			lck_hdl = ULCK_LCK_HDL;
			opn_hdl = ULCK_UOPN_HDL;
			lck_x = L_ICON_ULCK_X*EVE_PIXEL_UNIT;
		}
	}
}

FTVOID screen_main (FTU32 para)
{
#define M_TOP_P   ROOT_PATH"lierda\\m_top.raw"
#define M_MID_P   ROOT_PATH"lierda\\m_mid.raw"
#define M_BTM_P   ROOT_PATH"lierda\\m_btm.raw"
#define M_BTM1_P   ROOT_PATH"lierda\\m_btm1.raw"
#define M_BTMt_P   ROOT_PATH"lierda\\m_btmt.raw"
#define M_RAM     RAM_G
#define M_TOP_W   45
#define M_TOP_H   45
#define M_BTM_W   84
#define M_BTM_H   84
#define M_BTMt_W   36
#define M_BTMt_H   20
#define M_MID_W   148
#define M_MID_H   148
#define M_TOP_LCK_X  (EVE_LCD_WIDTH/32)
#define M_TOP_SET_X  (EVE_LCD_WIDTH/32*31-M_TOP_W)
#define M_TOP_Y  (EVE_LCD_HIGH/8)
#define M_BTM_GAP    70
#define M_BTM_WEK_X  (EVE_LCD_WIDTH/2-M_BTM_W/2)
#define M_BTM_LGT_X  (M_BTM_WEK_X-M_BTM_GAP-M_BTM_W)
#define M_BTM_ALM_X  (M_BTM_LGT_X-M_BTM_GAP-M_BTM_W)
#define M_BTM_STG_X  (M_BTM_WEK_X+M_BTM_GAP+M_BTM_W)
#define M_BTM_PWR_X  (M_BTM_STG_X+M_BTM_GAP+M_BTM_W)
#define M_BTM_Y  (EVE_LCD_HIGH/4*3-20)
#define M_BTMt_GAP    10
#define M_BTMt_WEK_X  (M_BTM_WEK_X+(M_BTM_W-M_BTMt_W)/2)
#define M_BTMt_LGT_X  (M_BTM_LGT_X+(M_BTM_W-M_BTMt_W)/2)
#define M_BTMt_ALM_X  (M_BTM_ALM_X+(M_BTM_W-M_BTMt_W)/2)
#define M_BTMt_STG_X  (M_BTM_STG_X+(M_BTM_W-M_BTMt_W)/2)
#define M_BTMt_PWR_X  (M_BTM_PWR_X+(M_BTM_W-M_BTMt_W)/2)
#define M_BTMt_Y  (M_BTM_Y+M_BTM_H+M_BTMt_GAP)
#define M_MID_GAP    50
#define M_MID_AIR_X    (EVE_LCD_WIDTH/2-M_MID_GAP/2-M_MID_W)
#define M_MID_NAV_X    (EVE_LCD_WIDTH/2+M_MID_GAP/2)
#define M_MID_Y    (EVE_LCD_HIGH/2-M_MID_H/2-20)
/* give some delay to remove finger viberate affect
 * tune this value base on different system */
#define VIBERATE_REMOVE 250
	enum bmp_hdl_ {
		M_TOP_HDL = 0,
		M_BTM_HDL,
		M_BTM1_HDL,
		M_BTMt_HDL,
		M_MID_HDL,
		M_HDL_MAX
	};
	enum bmp_top_cell_ {
		M_TOP_LCK_CELL = 0,
		M_TOP_SET_CELL
	};
	/* share the btm and btmt cell */
	enum bmp_btm_cell_ {
		M_BTM_ALM_CELL = 0,
		M_BTM_LGT_CELL,
		M_BTM_WEK_CELL,
		M_BTM_STG_CELL,
		M_BTM_PWR_CELL
	};
	enum bmp_mid_cell_ {
		M_MID_AIR_CELL = 0,
		M_MID_NAV_CELL,
	};
	enum m_tag_ {
		M_TAG_LCK = 10,
		M_TAG_SET,
		M_TAG_AIR,
		M_TAG_NAV,
		M_TAG_ALM,
		M_TAG_LGT,
		M_TAG_WEK,
		M_TAG_STG,
		M_TAG_PWR
	};

	bmpHDR_st bmp_header[M_HDL_MAX] = {
		      {M_TOP_P, 0,0,ARGB4, 0,0, M_TOP_W, M_TOP_H},
		      {M_BTM_P, 0,0,ARGB4, 0,0, M_BTM_W, M_BTM_H},
		      {M_BTM1_P, 0,0,ARGB4, 0,0, M_BTM_W, M_BTM_H},
		      {M_BTMt_P, 0,0,ARGB4, 0,0, M_BTMt_W, M_BTMt_H},
		      {M_MID_P, 0,0,ARGB4, 0,0, M_MID_W, M_MID_H}
	};

	static FTU32 f_loaded = 0;
	FTU8 hdl_btm[M_TAG_PWR-M_TAG_ALM+1], alpha = 255,tag,i;
	FTU32 ram_offset;
	
	/* only load the file once */
	if (f_loaded == 0) {
		/* load bitmap resources data into EVE */
		if(APP_OK != appBmpToRamG(M_TOP_HDL, M_RAM, bmp_header, M_HDL_MAX)){
			DBGPRINT;
			return;
		}
		f_loaded = 1;
		/* for sub UI in this screen bitmap storage offset 
		 * not use at this now, I'm too lazy at this point
		 * may impliment it later*/
		for (i = 0, ram_offset = 0;i < M_HDL_MAX; i++) {
			ram_offset += bmp_header[i].len;
		}
	}

	memset(hdl_btm,M_BTM_HDL,sizeof(hdl_btm));

	while (1) {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(COLOR_BAKGRD_CLR);
		HAL_CmdBufIn(CLEAR(1,1,1));
		HAL_CmdBufIn(COLOR_A(alpha));
		HAL_CmdBufIn(BEGIN(BITMAPS));

		HAL_CmdBufIn(TAG_MASK(1));
		/* top 2 icon */
		HAL_CmdBufIn(TAG(M_TAG_LCK));
		HAL_CmdBufIn(BITMAP_HANDLE(M_TOP_HDL));
		HAL_CmdBufIn(CELL(M_TOP_LCK_CELL));
		HAL_CmdBufIn(VERTEX2F(M_TOP_LCK_X*EVE_PIXEL_UNIT,M_TOP_Y*EVE_PIXEL_UNIT));
		HAL_CmdBufIn(TAG(M_TAG_SET));
		HAL_CmdBufIn(CELL(M_TOP_SET_CELL));
		HAL_CmdBufIn(VERTEX2F(M_TOP_SET_X*EVE_PIXEL_UNIT,M_TOP_Y*EVE_PIXEL_UNIT));

		/* show the contents base on bottom selection */			
		for (i = 0; i < sizeof(hdl_btm); i++) {
			if (hdl_btm[i] == M_BTM1_HDL) {
				/* select which UI would be displayed */
				switch (i+M_TAG_ALM) {
					case M_TAG_ALM:
						CoCmd_TEXT((EVE_LCD_WIDTH/2),(EVE_LCD_HIGH/2),25,OPT_CENTERX,"Alarm UI, not finished");
						break;
					case M_TAG_LGT:
						CoCmd_TEXT((EVE_LCD_WIDTH/2),(EVE_LCD_HIGH/2),25,OPT_CENTERX,"Light UI, not finished");
						break;
					case M_TAG_WEK:
						CoCmd_TEXT((EVE_LCD_WIDTH/2),(EVE_LCD_HIGH/2),25,OPT_CENTERX,"Weak UI, not finished");
						break;
					case M_TAG_STG:
						CoCmd_TEXT((EVE_LCD_WIDTH/2),(EVE_LCD_HIGH/2),25,OPT_CENTERX,"Strong UI, not finished");
						break;
					case M_TAG_PWR:
						CoCmd_TEXT((EVE_LCD_WIDTH/2),(EVE_LCD_HIGH/2),25,OPT_CENTERX,"Power off UI, not finished");
						break;
					default:
						i = sizeof(hdl_btm);
						break;
				}
				/* only one item would be select at a time */
				break;
			}
		}
		/* any selected item would case break out from 'for' before i reach to MAX
		 * if not, nothing be selected */
		if (i == sizeof(hdl_btm)) {
			/* middle 2 icon */
			HAL_CmdBufIn(TAG(M_TAG_AIR));
			HAL_CmdBufIn(BITMAP_HANDLE(M_MID_HDL));
			HAL_CmdBufIn(CELL(M_MID_AIR_CELL));
			HAL_CmdBufIn(VERTEX2F(M_MID_AIR_X*EVE_PIXEL_UNIT,M_MID_Y*EVE_PIXEL_UNIT));
			HAL_CmdBufIn(TAG(M_TAG_NAV));
			HAL_CmdBufIn(CELL(M_MID_NAV_CELL));
			HAL_CmdBufIn(VERTEX2F(M_MID_NAV_X*EVE_PIXEL_UNIT,M_MID_Y*EVE_PIXEL_UNIT));
		}

		/* bottom 5 icon */
		HAL_CmdBufIn(TAG(M_TAG_ALM));
		HAL_CmdBufIn(BITMAP_HANDLE(hdl_btm[M_TAG_ALM-M_TAG_ALM]));
		HAL_CmdBufIn(CELL(M_BTM_ALM_CELL));
		HAL_CmdBufIn(VERTEX2F(M_BTM_ALM_X*EVE_PIXEL_UNIT,M_BTM_Y*EVE_PIXEL_UNIT));

		HAL_CmdBufIn(TAG(M_TAG_LGT));
		HAL_CmdBufIn(BITMAP_HANDLE(hdl_btm[M_TAG_LGT-M_TAG_ALM]));
		HAL_CmdBufIn(CELL(M_BTM_LGT_CELL));
		HAL_CmdBufIn(VERTEX2F(M_BTM_LGT_X*EVE_PIXEL_UNIT,M_BTM_Y*EVE_PIXEL_UNIT));
		
		HAL_CmdBufIn(TAG(M_TAG_WEK));
		HAL_CmdBufIn(BITMAP_HANDLE(hdl_btm[M_TAG_WEK-M_TAG_ALM]));
		HAL_CmdBufIn(CELL(M_BTM_WEK_CELL));
		HAL_CmdBufIn(VERTEX2F(M_BTM_WEK_X*EVE_PIXEL_UNIT,M_BTM_Y*EVE_PIXEL_UNIT));
	
		HAL_CmdBufIn(TAG(M_TAG_STG));
		HAL_CmdBufIn(BITMAP_HANDLE(hdl_btm[M_TAG_STG-M_TAG_ALM]));
		HAL_CmdBufIn(CELL(M_BTM_STG_CELL));
		HAL_CmdBufIn(VERTEX2F(M_BTM_STG_X*EVE_PIXEL_UNIT,M_BTM_Y*EVE_PIXEL_UNIT));
	
		HAL_CmdBufIn(TAG(M_TAG_PWR));
		HAL_CmdBufIn(BITMAP_HANDLE(hdl_btm[M_TAG_PWR-M_TAG_ALM]));
		HAL_CmdBufIn(CELL(M_BTM_PWR_CELL));
		HAL_CmdBufIn(VERTEX2F(M_BTM_PWR_X*EVE_PIXEL_UNIT,M_BTM_Y*EVE_PIXEL_UNIT));
		
		/* no tag on the text */
		HAL_CmdBufIn(TAG_MASK(0));
		
		HAL_CmdBufIn(BITMAP_HANDLE(M_BTMt_HDL));
		HAL_CmdBufIn(CELL(M_BTM_ALM_CELL));
		HAL_CmdBufIn(VERTEX2F(M_BTMt_ALM_X*EVE_PIXEL_UNIT,M_BTMt_Y*EVE_PIXEL_UNIT));

		HAL_CmdBufIn(CELL(M_BTM_LGT_CELL));
		HAL_CmdBufIn(VERTEX2F(M_BTMt_LGT_X*EVE_PIXEL_UNIT,M_BTMt_Y*EVE_PIXEL_UNIT));
		
		HAL_CmdBufIn(CELL(M_BTM_WEK_CELL));
		HAL_CmdBufIn(VERTEX2F(M_BTMt_WEK_X*EVE_PIXEL_UNIT,M_BTMt_Y*EVE_PIXEL_UNIT));
	
		HAL_CmdBufIn(CELL(M_BTM_STG_CELL));
		HAL_CmdBufIn(VERTEX2F(M_BTMt_STG_X*EVE_PIXEL_UNIT,M_BTMt_Y*EVE_PIXEL_UNIT));
	
		HAL_CmdBufIn(CELL(M_BTM_PWR_CELL));
		HAL_CmdBufIn(VERTEX2F(M_BTMt_PWR_X*EVE_PIXEL_UNIT,M_BTMt_Y*EVE_PIXEL_UNIT));

		HAL_CmdBufIn(END());
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);

		if (appGP.appIndex == I_LOCK) {
			if (alpha == 0) {
				f_loaded = 0;
				screen_clean();
				return;		
			}
			alpha -= ALPHA_STEP;
			continue;
		}

		tag = HAL_Read8(REG_TOUCH_TAG);
		switch (tag) {
			case M_TAG_LCK:
				appGP.appIndex = I_LOCK;
				break;
			case M_TAG_ALM:
			case M_TAG_LGT:
			case M_TAG_WEK:
			case M_TAG_STG:
			case M_TAG_PWR:
				for (i = 0; i < sizeof(hdl_btm); i++) {
					if (i != tag-M_TAG_ALM) {
						hdl_btm[i] = M_BTM_HDL;
					} else {
						FTDELAY(VIBERATE_REMOVE);
						if (hdl_btm[i] == M_BTM1_HDL) {
							hdl_btm[i] = M_BTM_HDL;
						} else {
							hdl_btm[i] = M_BTM1_HDL;
						}
					}
				}
				break;
			default:
				break;
		}
	}
}

#define SETDEFAULT HAL_CmdBufIn(BITMAP_TRANSFORM_A(256)); \
	HAL_CmdBufIn(BITMAP_TRANSFORM_B(0)); \
	HAL_CmdBufIn(BITMAP_TRANSFORM_C(0)); \
	HAL_CmdBufIn(BITMAP_TRANSFORM_D(0)); \
	HAL_CmdBufIn(BITMAP_TRANSFORM_E(256)); \
	HAL_CmdBufIn(BITMAP_TRANSFORM_F(0))

FTVOID screen_clock (FTU32 para)
{
#define CLK_1_P        ROOT_PATH"lierda\\clk_1.raw"
#define CLK_2_P        ROOT_PATH"lierda\\clk_2.raw"
#define CLK_H_P        ROOT_PATH"lierda\\clk_h.raw"
#define CLK_M_P        ROOT_PATH"lierda\\clk_m.raw"
#define CLK_RAM RAM_G
#define CLK_1_W   57
#define CLK_1_H   335
#define CLK_2_W   427
#define CLK_2_H   29

#define CLK_1_X   (EVE_LCD_WIDTH/2-CLK_2_W/2-CLK_1_W)
#define CLK_1_Y   (EVE_LCD_HIGH/2-CLK_1_H/2)
#define CLK_2_X   (EVE_LCD_WIDTH/2-CLK_2_W/2)
#define CLK_2_Y   (CLK_1_Y-CLK_2_H/2)

#define CLK_M_W   267
#define CLK_M_H   267
#define CLK_M_X   (EVE_LCD_WIDTH/2-CLK_M_W/2)
#define CLK_M_Y   (EVE_LCD_HIGH/4-15)

#define CLK_H_W   178
#define CLK_H_H   178
#define CLK_H_X   (EVE_LCD_WIDTH/2-CLK_H_W/2)
#define CLK_H_Y   (EVE_LCD_HIGH/4+28)

#define CLK_ANGLE (360/60)
	enum bmp_hdl_ {
		CLK_1_HDL = 0,
		CLK_2_HDL,
		CLK_H_HDL,
		CLK_M_HDL,
		CLK_HDL_MAX
	};
	bmpHDR_st bmp_header[CLK_HDL_MAX] = {
		      {CLK_1_P, 0,0,ARGB4, 0,0, CLK_1_W, CLK_1_H},
		      {CLK_2_P, 0,0,ARGB4, 0,0, CLK_2_W, CLK_2_H},
		      {CLK_H_P, 0,0,ARGB4, 0,0, CLK_H_W, CLK_H_H},
		      {CLK_M_P, 0,0,ARGB4, 0,0, CLK_M_W, CLK_M_H},
	};
	FTU8 alpha = 255;
	static FTU32 f_loaded = 0, r_h = 0, r_m = 0;
	
	/* only load the file once */
	if (f_loaded == 0) {
		/* load bitmap resources data into EVE */
		if(APP_OK != appBmpToRamG(CLK_1_HDL, CLK_RAM, bmp_header, CLK_HDL_MAX)){
			DBGPRINT;
			return;
		}
		f_loaded = 1;
	}

	while (1) {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(COLOR_BAKGRD_CLR);
		HAL_CmdBufIn(CLEAR(1,1,1));

		HAL_CmdBufIn(COLOR_A(alpha));
		HAL_CmdBufIn(BEGIN(BITMAPS));
		/* left and top part of clock */
		HAL_CmdBufIn(BITMAP_HANDLE(CLK_1_HDL));
		HAL_CmdBufIn(VERTEX2F(CLK_1_X*EVE_PIXEL_UNIT,CLK_1_Y*EVE_PIXEL_UNIT));
		HAL_CmdBufIn(BITMAP_HANDLE(CLK_2_HDL));
		HAL_CmdBufIn(VERTEX2F(CLK_2_X*EVE_PIXEL_UNIT,CLK_2_Y*EVE_PIXEL_UNIT));
		HAL_CmdBufIn(END());

		/* rotate left 180 degree put at right */
		HAL_CmdBufIn(BEGIN(BITMAPS));
		CoCmd_LOADIDENTITY;	      
		CoCmd_TRANSLATE(CLK_1_W/2*EVE_TRANSFORM_MAX,CLK_1_H/2*EVE_TRANSFORM_MAX);
		CoCmd_ROTATE(180*EVE_TRANSFORM_MAX/360);
		CoCmd_TRANSLATE(-CLK_1_W/2*EVE_TRANSFORM_MAX,-CLK_1_H/2*EVE_TRANSFORM_MAX);
		CoCmd_SETMATRIX;
		HAL_CmdBufIn(BITMAP_HANDLE(CLK_1_HDL));
		HAL_CmdBufIn(VERTEX2F((CLK_2_X+CLK_2_W)*EVE_PIXEL_UNIT,(CLK_1_Y)*EVE_PIXEL_UNIT));
		SETDEFAULT;
		HAL_CmdBufIn(END());

		/* rotate top 180 degree put at bottom */
		HAL_CmdBufIn(BEGIN(BITMAPS));
		CoCmd_LOADIDENTITY;	      
		CoCmd_TRANSLATE(CLK_2_W/2*EVE_TRANSFORM_MAX,CLK_2_H/2*EVE_TRANSFORM_MAX);
		CoCmd_ROTATE(180*EVE_TRANSFORM_MAX/360);
		CoCmd_TRANSLATE(-CLK_2_W/2*EVE_TRANSFORM_MAX,-CLK_2_H/2*EVE_TRANSFORM_MAX);
		CoCmd_SETMATRIX;
		HAL_CmdBufIn(BITMAP_HANDLE(CLK_2_HDL));
		HAL_CmdBufIn(VERTEX2F(CLK_2_X*EVE_PIXEL_UNIT,(CLK_1_Y+CLK_1_H)*EVE_PIXEL_UNIT));
		SETDEFAULT;
		HAL_CmdBufIn(END());

		/* minutes */
		HAL_CmdBufIn(BEGIN(BITMAPS));
		CoCmd_LOADIDENTITY;
		CoCmd_TRANSLATE(CLK_M_W/2*EVE_TRANSFORM_MAX,CLK_M_H/2*EVE_TRANSFORM_MAX);
		CoCmd_ROTATE(r_m*EVE_TRANSFORM_MAX/360);
		CoCmd_TRANSLATE(-CLK_M_W/2*EVE_TRANSFORM_MAX,-CLK_M_H/2*EVE_TRANSFORM_MAX);
		CoCmd_SETMATRIX;
		HAL_CmdBufIn(BITMAP_HANDLE(CLK_M_HDL));
		HAL_CmdBufIn(VERTEX2F(CLK_M_X*EVE_PIXEL_UNIT,CLK_M_Y*EVE_PIXEL_UNIT));
		SETDEFAULT;
		HAL_CmdBufIn(END());
	
		/* hour */
		HAL_CmdBufIn(BEGIN(BITMAPS));
		CoCmd_LOADIDENTITY;
		CoCmd_TRANSLATE(CLK_H_W/2*EVE_TRANSFORM_MAX,CLK_H_H/2*EVE_TRANSFORM_MAX);
		CoCmd_ROTATE(r_h*EVE_TRANSFORM_MAX/360);
		CoCmd_TRANSLATE(-CLK_H_W/2*EVE_TRANSFORM_MAX,-CLK_H_H/2*EVE_TRANSFORM_MAX);
		CoCmd_SETMATRIX;
		HAL_CmdBufIn(BITMAP_HANDLE(CLK_H_HDL));
		HAL_CmdBufIn(VERTEX2F(CLK_H_X*EVE_PIXEL_UNIT,CLK_H_Y*EVE_PIXEL_UNIT));
		SETDEFAULT;
		HAL_CmdBufIn(END());
	
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
		/* clock simulator */
		if (r_m >= 360) {
			r_m = CLK_ANGLE;
			if (r_h >= 360) {
				r_h = CLK_ANGLE;
			} else {
				r_h += CLK_ANGLE;
			}
		} else {
			r_m += CLK_ANGLE;
		}
		/* screen switching effect */
		if (appGP.appIndex != I_LOCK) {
			if (TOUCHED) {
				appGP.appIndex = I_LOCK;
				continue;
			}	
		} else {
			alpha -= ALPHA_STEP;
			if (alpha == 0) {
				f_loaded = 0;
				screen_clean();
				return;
			}
			continue;
		}
	}
}

AppFunc APPS_UI[] = {
	screen_lock,
	screen_main,
	screen_clock
};

