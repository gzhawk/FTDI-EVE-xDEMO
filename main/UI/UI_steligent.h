/* 
    Demo for Steligent's UI
    Author: Hawk Gao
	Email : hawk.gao@ftdichip.com
	Date  : Oct/2015
*/

/* for multi touch and track touch use */
FTU8 trigger = 0;
FTU8 pre_tag = 0;
FTU32 key_data = 0;

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(FT9XXEV)
#define LOGO1_PATH ROOT_PATH"steligent\\logo1.raw"
#define LOGO2_PATH ROOT_PATH"steligent\\logo2.raw"
#define TRI_PATH ROOT_PATH"steligent\\tri.raw"
#else
#define LOGO1_PATH ROOT_PATH"logo1.raw"
#define LOGO2_PATH ROOT_PATH"logo2.raw"
#define TRI_PATH ROOT_PATH"tri.raw"
#endif
#define TOGGLE_TRUE (65535)
#define LOGO_GAP    10
#define LOGO_NUM    2
#define LOGO_HANDLE 0
#define LOGO1_W     32
#define LOGO1_H     32
#define LOGO1_X     10
#define LOGO1_Y     10
#define LOGO2_W     105
#define LOGO2_H     27
#define LOGO2_X     (LOGO1_X+LOGO1_W+LOGO_GAP)
#define LOGO2_Y     (15)

#define TRI_W       9
#define TRI_H       5
#define TRI_GAP     5

#define TOP_FONT      (22)
#define TOP_UP      (15)
#define TOP_VER_X      (180)
#define TOP_FORM_X      (256)
#define TOP_FORM_Y      (5)
#define TOP_FORM_W      (262)
#define TOP_FORM_H      (32)
#define TOP_YEAR_X      (634)
#define TOP_MONTH_X      (662)
#define TOP_DAY_X      (690)
#define TOP_HOUR_X      (734)
#define TOP_MINUTE_X      (760)
#define TOP_SECOND_X      (784)
#define TOP_SEL_NUM      (27)
#define TOP_SEL_LEN      (15)

#define CHANX_MAX    (5)
#define CHANX_COL_BAR (COLOR_RGB(75, 75, 75))
#define CHANX_COL_BY (COLOR_RGB(198, 198, 198))
#define CHANX_COL_PR (COLOR_RGB(158, 158, 158))
#define CHANX_COL_TOG CoCmd_FGCOLOR((255<<16)|(255<<8)|255)
#define CHANX_COL_TOG1 CoCmd_BGCOLOR((1<<16)|(58<<8)|106)
#define CHANX_COL_TOG2 CoCmd_BGCOLOR((53<<16)|(123<<8)|26)
#define CHANX_COL_TOG3 CoCmd_BGCOLOR((158<<16)|(158<<8)|158)
#define CHANX_W      (390)
#define CHANX_LINE_W (3)
#define CHANX_BAR_H  (28)
#define CHANX_BAR_F  (23)
#define CHANX_LINE   (90)
#define CHANX_BODY_F (22)
#define CHANX_BODY_H (126)
#define CHANX_TITLE1 (76)
#define CHANX_TITLE2 (368)
#define CHANX_LED    (320)
#define CHANX_LED_RA (8)
#define CHANX_PARA_U  (12)
#define CHANX_PARA_H  (25)
#define CHANX_PARA_FH (16)
#define CHANX_PARA_L  (90)
#define CHANX_PARA_W  (90)
#define CHANX_PARA_R  (280)
#define CHANX_BUTN_LN (90)
#define CHANX_BUTN_W  (84)
#define CHANX_BUTN_H  (28)
#define CHANX_BUTN_GAP ((CHANX_W-4*CHANX_BUTN_W)/5)

#define CHANX_KEY_W (2*CHANX_PARA_W)
#define CHANX_KEY_H (CHANX_BUTN_H-5)
#define CHANX_KEY_COL CoCmd_FGCOLOR((75<<16)|(75<<8)|75)

#define CHANA_COL_BT CoCmd_FGCOLOR((158<<16)|(158<<8)|158)
#define CHANA_COL_BT1 CoCmd_FGCOLOR((1<<16)|(58<<8)|106)
#define CHANA_COL_BT2 CoCmd_FGCOLOR((53<<16)|(123<<8)|26)
#define CHANA_W      (2*CHANX_W+3)
#define CHANA_BODY_H (76)
#define CHANA_PARA_GAP ((CHANA_W-8*CHANX_PARA_W)/9)
#define CHANA_BUTN_LN (40)
#define CHANA_BUTN_GAP ((CHANA_W-4*CHANX_BUTN_W)/5)

#define TOUCH_SINGLE_SENSITIVE 200 /* change this value depends on real board test */
#define TOUCH_MULTI_SENSITIVE 180 /* change this value depends on real board test */

#define DATA_NOT_EQUAL 0xFF

enum en_touch_type {
	TOUCH_NONE = 1,
	TOUCH_SINGLE,
	TOUCH_MULTI,
	TOUCH_TRACK
};
typedef struct chanX_xy_ {
	FTU16 X;
	FTU16 Y;
	FT8 str[10];
} chanX_xy_st;
chanX_xy_st chanX_list[CHANX_MAX] = {
	{10,48,"Chan 1"},
	{404,48,"Chan 2"},
	{10,210,"Chan 3"},
	{404,210,"Chan 4"},
	{10,370,"Chan All"}
};
typedef struct top_time_ {
	FTU16 year;
	FTU8 month;
	FTU8 day;
	FTU8 hour;
	FTU8 minute;
	FTU8 second;
} top_time_st;
top_time_st top_time = {
	2015,6,24,18,19,20	
};
FT8 ver_str[] = "Ver 2.0";
FT8 top_str[TOP_SEL_NUM][TOP_SEL_LEN] = {
	{"1.25000 Gbps"},
	{"2.12500 Gbps"},
	{"4.25000 Gbps"},
	{"5.00000 Gbps"},
	{"6.00000 Gbps"},
	{"6.25000 Gbps"},
	{"8.00000 Gbps"},
	{"8.50000 Gbps"},
	{"9.00000 Gbps"},
	{"9.50000 Gbps"},
	{"9.95328 Gbps"},
	{"10.00000 Gbps"},
	{"10.31250 Gbps"},
	{"10.51785 Gbps"},
	{"10.70000 Gbps"},
	{"10.70922 Gbps"},
	{"11.10000 Gbps"},
	{"11.31700 Gbps"},
	{"11.50000 Gbps"},
	{"12.00000 Gbps"},
	{"12.50000 Gbps"},
	{"12.80000 Gbps"},
	{"13.00000 Gbps"},
	{"13.50000 Gbps"},
	{"14.00000 Gbps"},
	{"14.02500 Gbps"},
	{"14.50000 Gbps"},
};
#define POL_SEL_NUM 2
#define POL_SEL_LEN 7
FT8 polarity_str[POL_SEL_NUM][POL_SEL_LEN] = {
	{"Normal"},
	{"Invert"}
};
#define PRBS_SEL_NUM 7
#define PRBS_SEL_LEN 7
FT8 prbs_str[PRBS_SEL_NUM][PRBS_SEL_LEN] = {
	{"2^7-1"},
	{"2^9-1"},
	{"2^10-1"},
	{"2^11-1"},
	{"2^15-1"},
	{"2^23-1"},
	{"2^31-1"}
};
enum TAG_MainUI {
	eT_M_TOP = 1,
	
	eT_M_C1_PRBS,
	eT_M_C1_POLY,
	eT_M_C1_GT,
	eT_M_C1_AMP,
	eT_M_C1_PPG,
	eT_M_C1_BERT,

	eT_M_C2_PRBS,
	eT_M_C2_POLY,
	eT_M_C2_GT,
	eT_M_C2_AMP,
	eT_M_C2_PPG,
	eT_M_C2_BERT,

	eT_M_C3_PRBS,
	eT_M_C3_POLY,
	eT_M_C3_GT,
	eT_M_C3_AMP,
	eT_M_C3_PPG,
	eT_M_C3_BERT,

	eT_M_C4_PRBS,
	eT_M_C4_POLY,
	eT_M_C4_GT,
	eT_M_C4_AMP,
	eT_M_C4_PPG,
	eT_M_C4_BERT, //25

	eT_M_CA_PRBS,
	eT_M_CA_POLY,
	eT_M_CA_GT,
	eT_M_CA_AMP,
	eT_M_CA_PPG_1,
	eT_M_CA_PPG_0,
	eT_M_CA_BERT_1,
	eT_M_CA_BERT_0,
	
	eT_M_MAX, /* 34 use it as track area tag */
	/* 
	 should not larger than '0'(40)
	 since it would be used in KEY...
	 */
};

#define eCHANX_OFFSET (eT_M_C2_PRBS - eT_M_C1_PRBS)

typedef struct chan_x_data_ {
	FTU8 prbs;
	FTU16 polarity;
	FTU32 gating_time;
	FTU32 amplitude;
	FTU32 error_count;
	FTU32 error_ratio;
	FTU16 ppg;
	FTU16 bert;
} chan_x_data_st;
typedef struct data_ {
	FTU8 data_top;
	chan_x_data_st chan_data[CHANX_MAX];
} data_st;
data_st ui_data;
FTU8 led_color[CHANX_MAX-1] = {0,1,0,1};
FTVOID touch_top (FTVOID)
{
#define TRACK_MAX_VAL 65535
#define ONE_MOVE_VAL (TRACK_MAX_VAL/16)
	FTU16 x = (TOP_FORM_X);
	FTU16 y = (TOP_FORM_Y+TOP_FORM_H+3);
	FTU32 t_track, d_track;
	static FTU16 D1 = 0;
	FTU8 top = ui_data.data_top, top1,top2,top3,top4;

	if (trigger == 0) {
		FTDELAY(TOUCH_SINGLE_SENSITIVE);
		trigger = 1;
	} else {
		t_track = HAL_Read32(REG_TRACKER);
		d_track = (t_track >> 16)&0xFFFF;
		t_track &= 0xFFFF;

		if (t_track == eT_M_MAX) {
			if (D1 == 0) {
				D1 = d_track;
			} else if (d_track < D1) {
				if (D1 - d_track > ONE_MOVE_VAL) {
					top = (top == (TOP_SEL_NUM-1))?0:(top + 1);
					D1 = 0;
				}
			} else if (d_track >= D1) {
				if (d_track - D1 > ONE_MOVE_VAL) {
					top = (top == 0)?(TOP_SEL_NUM-1):(top - 1);
					D1 = 0;
				}
			}
		} else {
			D1 = 0;
		}
	}

	top1 = (top == (TOP_SEL_NUM-1))?0:(top + 1);
	top2 = (top1 == (TOP_SEL_NUM-1))?0:(top1 + 1);
	top3 = (top2 == (TOP_SEL_NUM-1))?0:(top2 + 1);
	top4 = (top3 == (TOP_SEL_NUM-1))?0:(top3 + 1);
	ui_data.data_top = top;

	HAL_CmdBufIn(TAG_MASK(1));
	HAL_CmdBufIn(TAG(eT_M_MAX));
	HAL_CmdBufIn(BEGIN(RECTS));
	HAL_CmdBufIn(CHANX_COL_PR);
	HAL_CmdBufIn(VERTEX2F(x*FT800_PIXEL_UNIT,y*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F((x+TOP_FORM_W)*FT800_PIXEL_UNIT,(y+4*TOP_FORM_H)*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(END());
	CoCmd_TRACK(x,y,1,4*TOP_FORM_H,eT_M_MAX);
	HAL_CmdBufIn(TAG_MASK(0));

	HAL_CmdBufIn(COLOR_RGB(255,255,255));
	CoCmd_TEXT(x,y,TOP_FONT,0,&top_str[top1][0]);
	CoCmd_TEXT(x,y+TOP_FORM_H,TOP_FONT,0,&top_str[top2][0]);
	CoCmd_TEXT(x,y+2*TOP_FORM_H,TOP_FONT,0,&top_str[top3][0]);
	CoCmd_TEXT(x,y+3*TOP_FORM_H,TOP_FONT,0,&top_str[top4][0]);
}
FTVOID touch_prbs (FTU8 Tag)
{
	FTU8 chanx = (Tag - eT_M_C1_PRBS)/eCHANX_OFFSET,
	     prbs = ui_data.chan_data[chanx].prbs, 
		 prbs1,prbs2,prbs3,prbs4;
	FTU16 x,y;
	FTU32 t_track, d_track;
	static FTU16 D1 = 0;

	if (chanx < CHANX_MAX-1) {
		x = (chanX_list[chanx].X+CHANX_PARA_L+CHANX_LINE_W);
		y = (chanX_list[chanx].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH+3);
	} else {
		x = (chanX_list[chanx].X+2*CHANA_PARA_GAP+CHANX_PARA_W+CHANX_LINE_W);
		y = (chanX_list[chanx].Y+CHANX_BAR_H+CHANX_PARA_U-3);
	}

	if (trigger == 0) {
		FTDELAY(TOUCH_SINGLE_SENSITIVE);
		trigger = 1;
	} else {
		t_track = HAL_Read32(REG_TRACKER);
		d_track = (t_track >> 16)&0xFFFF;
		t_track &= 0xFFFF;

		if (t_track == eT_M_MAX) {
			if (D1 == 0) {
				D1 = d_track;
			} else if (d_track < D1) {
				if (D1 - d_track > ONE_MOVE_VAL) {
					prbs = (prbs == (PRBS_SEL_NUM-1))?0:(prbs + 1);
					D1 = 0;
				}
			} else if (d_track >= D1) {
				if (d_track - D1 > ONE_MOVE_VAL) {
					prbs = (prbs == 0)?(PRBS_SEL_NUM-1):(prbs - 1);
					D1 = 0;
				}
			}
		} else {
			D1 = 0;
		}
	}


	HAL_CmdBufIn(TAG_MASK(1));
	HAL_CmdBufIn(TAG(eT_M_MAX));
	HAL_CmdBufIn(BEGIN(RECTS));
	HAL_CmdBufIn(CHANX_COL_PR);
	if (chanx < CHANX_MAX-1) {
		HAL_CmdBufIn(VERTEX2F(x*FT800_PIXEL_UNIT,y*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((x+CHANX_PARA_W)*FT800_PIXEL_UNIT,(y+3+4*CHANX_PARA_FH)*FT800_PIXEL_UNIT));
		CoCmd_TRACK(x,y,1,4*CHANX_PARA_FH,eT_M_MAX);
	} else {
		HAL_CmdBufIn(VERTEX2F(x*FT800_PIXEL_UNIT,(y-3-4*CHANX_PARA_FH)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((x+CHANX_PARA_W)*FT800_PIXEL_UNIT,y*FT800_PIXEL_UNIT));
		CoCmd_TRACK(x,(y-3-4*CHANX_PARA_FH),1,4*CHANX_PARA_FH,eT_M_MAX);
	}
	HAL_CmdBufIn(TAG_MASK(0));
	HAL_CmdBufIn(END());
	
	HAL_CmdBufIn(COLOR_RGB(255,255,255));
	if (chanx < CHANX_MAX-1) {
		prbs1 = (prbs == PRBS_SEL_NUM-1)?0:(prbs + 1);
		prbs2 = (prbs1 == PRBS_SEL_NUM-1)?0:(prbs1 + 1);
		prbs3 = (prbs2 == PRBS_SEL_NUM-1)?0:(prbs2 + 1);
		prbs4 = (prbs3 == PRBS_SEL_NUM-1)?0:(prbs3 + 1);
		ui_data.chan_data[chanx].prbs = prbs;

		CoCmd_TEXT(x,y+CHANX_PARA_FH/2,CHANX_BODY_F-1,OPT_CENTERY,&prbs_str[prbs1][0]);
		CoCmd_TEXT(x,y+CHANX_PARA_FH+CHANX_PARA_FH/2,CHANX_BODY_F-1,OPT_CENTERY,&prbs_str[prbs2][0]);
		CoCmd_TEXT(x,y+2*CHANX_PARA_FH+CHANX_PARA_FH/2,CHANX_BODY_F-1,OPT_CENTERY,&prbs_str[prbs3][0]);
		CoCmd_TEXT(x,y+3*CHANX_PARA_FH+CHANX_PARA_FH/2,CHANX_BODY_F-1,OPT_CENTERY,&prbs_str[prbs4][0]);
	} else {
		prbs1 = (prbs == 0)?(PRBS_SEL_NUM-1):(prbs - 1);
		prbs2 = (prbs1 == 0)?(PRBS_SEL_NUM-1):(prbs1 - 1);
		prbs3 = (prbs2 == 0)?(PRBS_SEL_NUM-1):(prbs2 - 1);
		prbs4 = (prbs3 == 0)?(PRBS_SEL_NUM-1):(prbs3 - 1);
		ui_data.chan_data[0].prbs = prbs;
		ui_data.chan_data[1].prbs = prbs;
		ui_data.chan_data[2].prbs = prbs;
		ui_data.chan_data[3].prbs = prbs;
		ui_data.chan_data[4].prbs = prbs;

		CoCmd_TEXT(x,y-3*CHANX_PARA_FH-CHANX_PARA_FH/2,CHANX_BODY_F-1,OPT_CENTERY,&prbs_str[prbs4][0]);
		CoCmd_TEXT(x,y-2*CHANX_PARA_FH-CHANX_PARA_FH/2,CHANX_BODY_F-1,OPT_CENTERY,&prbs_str[prbs3][0]);
		CoCmd_TEXT(x,y-CHANX_PARA_FH-CHANX_PARA_FH/2,CHANX_BODY_F-1,OPT_CENTERY,&prbs_str[prbs2][0]);
		CoCmd_TEXT(x,y-CHANX_PARA_FH/2,CHANX_BODY_F-1,OPT_CENTERY,&prbs_str[prbs1][0]);
	}

}
FTVOID touch_all_polarity (FTU8 *pTag)
{
	FTU16 polarity = ui_data.chan_data[4].polarity;
	FTU32 x,Tx,y,Ty;

	x = (chanX_list[4].X+4*CHANA_PARA_GAP+3*CHANX_PARA_W+CHANX_LINE_W);
	y = (chanX_list[4].Y+CHANX_BAR_H+CHANX_PARA_U-3);

	if (trigger == 0) {
		FTDELAY(TOUCH_SINGLE_SENSITIVE);
		trigger = 1;
	} else {
		Ty = HAL_Read32(REG_CTOUCH_TOUCH0_XY);
		if (!(Ty&0x8000)) {
			FTDELAY(TOUCH_SINGLE_SENSITIVE);
			Tx = Ty>>16;
			Tx &= 0x7FFF;
			Ty &= 0x7FFF;
			trigger = 0;
			*pTag = 0;

			if (Tx >= x && Tx <= (x+CHANX_PARA_W)) {
				if (Ty >= (y-2*CHANX_PARA_FH-3) && Ty < (y-CHANX_PARA_FH-3)) {
					polarity = 0;
				} else if (Ty >= (y-CHANX_PARA_FH-3) && Ty <= (y-3)) {
					polarity = 1;
				}
			}
		}
	}

	HAL_CmdBufIn(BEGIN(RECTS));
	HAL_CmdBufIn(CHANX_COL_PR);
	HAL_CmdBufIn(VERTEX2F(x*FT800_PIXEL_UNIT,(y-2*CHANX_PARA_FH-3)*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F((x+CHANX_PARA_W)*FT800_PIXEL_UNIT,y*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(END());

	HAL_CmdBufIn(COLOR_RGB(255,255,255));
	CoCmd_TEXT(x,y-CHANX_PARA_FH-CHANX_PARA_FH/2,CHANX_BODY_F-1,OPT_CENTERY,&polarity_str[0][0]);
	CoCmd_TEXT(x,y-CHANX_PARA_FH/2,CHANX_BODY_F-1,OPT_CENTERY,&polarity_str[1][0]);
	
	if (DATA_NOT_EQUAL != polarity) {
		CoCmd_TEXT((chanX_list[4].X+4*CHANA_PARA_GAP+3*CHANX_PARA_W+CHANX_LINE_W),
				(chanX_list[4].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH/2),
				CHANX_BODY_F-1,OPT_CENTERY,&polarity_str[ui_data.chan_data[4].polarity][0]);
		ui_data.chan_data[0].polarity = (polarity == 1)?TOGGLE_TRUE:polarity;
		ui_data.chan_data[1].polarity = (polarity == 1)?TOGGLE_TRUE:polarity;
		ui_data.chan_data[2].polarity = (polarity == 1)?TOGGLE_TRUE:polarity;
		ui_data.chan_data[3].polarity = (polarity == 1)?TOGGLE_TRUE:polarity;
		ui_data.chan_data[4].polarity = polarity;
	}
}
FTU8 touch_keyboard (FTU16 x, FTU16 y, FTU8 *pTag)
{
	FTU8 keys = 0;

	/* keyboard background */
	HAL_CmdBufIn(BEGIN(RECTS));
	HAL_CmdBufIn(LINE_WIDTH(CHANX_LINE_W * FT800_PIXEL_UNIT));
	HAL_CmdBufIn(CHANX_COL_PR);
	HAL_CmdBufIn(VERTEX2F(x*FT800_PIXEL_UNIT,(y-CHANX_KEY_H)*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F((x+CHANX_KEY_W)*FT800_PIXEL_UNIT,(y+5*CHANX_KEY_H+5)*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(END());

	/* keyboard keys */
	CHANX_KEY_COL;
	HAL_CmdBufIn(COLOR_RGB(0,0,0));
	HAL_CmdBufIn(LINE_WIDTH(1 * FT800_PIXEL_UNIT));
	if (TOUCHED) {
		keys = HAL_Read8(REG_TOUCH_TAG);
	}

	CoCmd_KEY(x,y,CHANX_KEY_W,CHANX_KEY_H,CHANX_BAR_F,keys,"123");
	CoCmd_KEY(x,y+CHANX_KEY_H,CHANX_KEY_W,CHANX_KEY_H,CHANX_BAR_F,keys,"456");
	CoCmd_KEY(x,y+2*CHANX_KEY_H,CHANX_KEY_W,CHANX_KEY_H,CHANX_BAR_F,keys,"789");
	CoCmd_KEY(x,y+3*CHANX_KEY_H,CHANX_KEY_W,CHANX_KEY_H,CHANX_BAR_F,keys,"0<");
	CoCmd_KEY(x,y+4*CHANX_KEY_H,CHANX_KEY_W,CHANX_KEY_H,CHANX_BAR_F,keys,"NY");

	HAL_CmdBufIn(COLOR_RGB(255,255,255));
	CoCmd_NUMBER(x,(y-CHANX_KEY_H/2),CHANX_BODY_F+1,OPT_CENTERY,key_data);

	if ((keys >= '0' && keys <= '9') || 
		keys == '<' || keys == 'N' || keys == 'Y') {
		FTDELAY(TOUCH_MULTI_SENSITIVE);
		if (keys >= '0' && keys <= '9') {
			keys -= '0';
			if (key_data != 0 && key_data < 100000000) {
				key_data *= 10;
				key_data += keys;
			} else {
				key_data = keys;	
			}
		} else if (keys == '<') {
			key_data /= 10;	
		} else {
			trigger = 0;
			*pTag = 0;
			if (keys == 'Y') {
				return 1;
			}
		}
	}
	return 0;
}
FTVOID touch_gt (FTU8* pTag)
{
	FTU8 chanx = (*pTag - eT_M_C1_GT)/eCHANX_OFFSET;
	FTU16 x,y;
	
	if (chanx < CHANX_MAX-1) {
		x = ((chanX_list[chanx].X+CHANX_PARA_L+CHANX_PARA_W-CHANX_LINE_W)-CHANX_KEY_W);
		y = (chanX_list[chanx].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_H+CHANX_PARA_FH);
	} else {
		x = ((chanX_list[chanx].X+6*CHANA_PARA_GAP+6*CHANX_PARA_W-CHANX_LINE_W)-CHANX_KEY_W);
		y = ((chanX_list[chanx].Y+CHANX_BAR_H+CHANX_PARA_U)-5*CHANX_KEY_H-5);
	}

	if (touch_keyboard(x,y,pTag)) {
		if (chanx < CHANX_MAX-1) {
			ui_data.chan_data[chanx].gating_time = key_data;
		} else {
			ui_data.chan_data[0].gating_time = key_data;
			ui_data.chan_data[1].gating_time = key_data;
			ui_data.chan_data[2].gating_time = key_data;
			ui_data.chan_data[3].gating_time = key_data;
			ui_data.chan_data[4].gating_time = key_data;
		}
		key_data = 0;
	}
}
FTVOID touch_amp (FTU8 *pTag)
{
	FTU8 chanx = (*pTag - eT_M_C1_AMP)/eCHANX_OFFSET;
	FTU16 x,y;

	if (chanx < CHANX_MAX-1) {
		x = ((chanX_list[chanx].X+CHANX_PARA_R+CHANX_PARA_W-CHANX_LINE_W)-CHANX_KEY_W);
		y = (chanX_list[chanx].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_H+CHANX_PARA_FH);
	} else {
		x = ((chanX_list[chanx].X+8*CHANA_PARA_GAP+8*CHANX_PARA_W-CHANX_LINE_W)-CHANX_KEY_W);
		y = ((chanX_list[chanx].Y+CHANX_BAR_H+CHANX_PARA_U)-5*CHANX_KEY_H-5);
	}
	
	if (touch_keyboard(x,y,pTag)) {
		if (chanx < CHANX_MAX-1) {
			ui_data.chan_data[chanx].amplitude = key_data;
		} else {
			ui_data.chan_data[0].amplitude = key_data;
			ui_data.chan_data[1].amplitude = key_data;
			ui_data.chan_data[2].amplitude = key_data;
			ui_data.chan_data[3].amplitude = key_data;
			ui_data.chan_data[4].amplitude = key_data;
		}
		key_data = 0;
	}
}
FTVOID touch_toggle_polarity (FTU8 *pTag)
{
	FTU8 i = (*pTag - eT_M_C1_POLY)/eCHANX_OFFSET;

	if (ui_data.chan_data[i].polarity == 0) {
		ui_data.chan_data[i].polarity = TOGGLE_TRUE;
	} else {
		ui_data.chan_data[i].polarity = 0;
	}

	*pTag = 0;
	FTDELAY(TOUCH_SINGLE_SENSITIVE);
}
FTVOID touch_toggle_ppg (FTU8 *pTag)
{
	FTU8 i = (*pTag - eT_M_C1_PPG)/eCHANX_OFFSET;

	if (ui_data.chan_data[i].ppg == 0) {
		ui_data.chan_data[i].ppg = TOGGLE_TRUE;
	} else {
		ui_data.chan_data[i].ppg = 0;
	}

	*pTag = 0;
	FTDELAY(TOUCH_SINGLE_SENSITIVE);
}
FTVOID touch_toggle_bert (FTU8 *pTag)
{
	FTU8 i = (*pTag - eT_M_C1_BERT)/eCHANX_OFFSET;

	if (ui_data.chan_data[i].bert == 0) {
		ui_data.chan_data[i].bert = TOGGLE_TRUE;
	} else {
		ui_data.chan_data[i].bert = 0;
	}

	*pTag = 0;
	FTDELAY(TOUCH_SINGLE_SENSITIVE);
}
FTVOID touch_button (FTU8 *pTag)
{
	switch (*pTag) {
		case eT_M_CA_PPG_1:
			ui_data.chan_data[0].ppg = TOGGLE_TRUE;
			ui_data.chan_data[1].ppg = TOGGLE_TRUE;
			ui_data.chan_data[2].ppg = TOGGLE_TRUE;
			ui_data.chan_data[3].ppg = TOGGLE_TRUE;
			break;
		case eT_M_CA_PPG_0:
			ui_data.chan_data[0].ppg = 0;
			ui_data.chan_data[1].ppg = 0;
			ui_data.chan_data[2].ppg = 0;
			ui_data.chan_data[3].ppg = 0;
			break;
		case eT_M_CA_BERT_1:
			ui_data.chan_data[0].bert = TOGGLE_TRUE;
			ui_data.chan_data[1].bert = TOGGLE_TRUE;
			ui_data.chan_data[2].bert = TOGGLE_TRUE;
			ui_data.chan_data[3].bert = TOGGLE_TRUE;
			break;
		case eT_M_CA_BERT_0:
			ui_data.chan_data[0].bert = 0;
			ui_data.chan_data[1].bert = 0;
			ui_data.chan_data[2].bert = 0;
			ui_data.chan_data[3].bert = 0;
			break;
		default:
			break;
	}

	*pTag = 0;
	FTDELAY(TOUCH_SINGLE_SENSITIVE);
}
FTU8 touch_get_type (FTU8 tag)
{
	switch (tag) {
		case eT_M_C1_POLY:
		case eT_M_C2_POLY:
		case eT_M_C3_POLY:
		case eT_M_C4_POLY:

		case eT_M_C1_PPG:
		case eT_M_C2_PPG:
		case eT_M_C3_PPG:
		case eT_M_C4_PPG:

		case eT_M_C1_BERT:
		case eT_M_C2_BERT:
		case eT_M_C3_BERT:
		case eT_M_C4_BERT:

		case eT_M_CA_PPG_1:
		case eT_M_CA_PPG_0:
		case eT_M_CA_BERT_1:
		case eT_M_CA_BERT_0:
			return TOUCH_SINGLE;
		case eT_M_CA_POLY:

		case eT_M_C1_GT:
		case eT_M_C2_GT:
		case eT_M_C3_GT:
		case eT_M_C4_GT:
		case eT_M_CA_GT:

		case eT_M_C1_AMP:
		case eT_M_C2_AMP:
		case eT_M_C3_AMP:
		case eT_M_C4_AMP:
		case eT_M_CA_AMP:
			return TOUCH_MULTI;
		case eT_M_TOP:

		case eT_M_C1_PRBS:
		case eT_M_C2_PRBS:
		case eT_M_C3_PRBS:
		case eT_M_C4_PRBS:
		case eT_M_CA_PRBS:
			return TOUCH_TRACK;
		default:
			return TOUCH_NONE;
	}
}
FTVOID touch_control (FTU8 tag)
{
	FTU8 type = touch_get_type(tag);

	switch (type) {
		case TOUCH_NONE:
			break;
		case TOUCH_TRACK:
			if (trigger && pre_tag == tag) {
				trigger = 0;
				key_data = 0;
				pre_tag = 0;
				FTDELAY(TOUCH_SINGLE_SENSITIVE);
				return;
			} else if (!trigger) {
				pre_tag = tag;
			}
			break;
		case TOUCH_MULTI:
		case TOUCH_SINGLE:
			if (pre_tag != tag) {
				trigger = 0;
				key_data = 0;
			}
			pre_tag = tag;
			break;
		default:
			break;
	}
	switch (pre_tag) {
		/* track touch action */
		case eT_M_TOP:
			touch_top();
			return;
		case eT_M_C1_PRBS:
		case eT_M_C2_PRBS:
		case eT_M_C3_PRBS:
		case eT_M_C4_PRBS:
		case eT_M_CA_PRBS:
			touch_prbs(pre_tag);
			return;
		/* multi touch action */
		case eT_M_CA_POLY:
			touch_all_polarity(&pre_tag);
			return;
		case eT_M_C1_GT:
		case eT_M_C2_GT:
		case eT_M_C3_GT:
		case eT_M_C4_GT:
		case eT_M_CA_GT:
			touch_gt(&pre_tag);
			return;
		case eT_M_C1_AMP:
		case eT_M_C2_AMP:
		case eT_M_C3_AMP:
		case eT_M_C4_AMP:
		case eT_M_CA_AMP:
			touch_amp(&pre_tag);
			return;
		/* single touch action */
		case eT_M_C1_POLY:
		case eT_M_C2_POLY:
		case eT_M_C3_POLY:
		case eT_M_C4_POLY:
			touch_toggle_polarity(&pre_tag);
			return;
		case eT_M_C1_PPG:
		case eT_M_C2_PPG:
		case eT_M_C3_PPG:
		case eT_M_C4_PPG:
			touch_toggle_ppg(&pre_tag);
			return;
		case eT_M_C1_BERT:
		case eT_M_C2_BERT:
		case eT_M_C3_BERT:
		case eT_M_C4_BERT:
			touch_toggle_bert(&pre_tag);
			return;
		case eT_M_CA_PPG_1:
		case eT_M_CA_PPG_0:
		case eT_M_CA_BERT_1:
		case eT_M_CA_BERT_0:
			touch_button(&pre_tag);
			return;
		default:
			break;
	}

}

FTU8 bootup_show (FTVOID)
{
#define L1_X (FT800_LCD_WIDTH/2-(LOGO1_W+LOGO2_W+LOGO_GAP)/2)
#define L1_Y (FT800_LCD_HIGH/2-LOGO1_H/2)
#define L2_X (FT800_LCD_WIDTH/2+(LOGO1_W+LOGO2_W+LOGO_GAP)/2-LOGO2_W)
#define L2_Y (L1_Y+5)
#define LOGO_SPEED 10

	static FTU32 x1 = L1_X, y1 = 0, x2 = L2_X, y2 = FT800_LCD_HIGH, ret = 1;

	if (ret) {
		if (y1 <= L1_Y || y2 >= L2_Y) {
			y1 += LOGO_SPEED;
			y2 -= LOGO_SPEED;
			ret = 1;
		} else {
			y1 = L1_Y;
			y2 = L2_Y;
			ret = 0;
		}
	}

	HAL_CmdBufIn(BITMAP_HANDLE(LOGO_HANDLE));
	HAL_CmdBufIn(CELL(0));
	HAL_CmdBufIn(VERTEX2F(x1*FT800_PIXEL_UNIT,
	                      y1*FT800_PIXEL_UNIT));
	
	HAL_CmdBufIn(BITMAP_HANDLE(LOGO_HANDLE+1));
	HAL_CmdBufIn(CELL(0));
	HAL_CmdBufIn(VERTEX2F(x2*FT800_PIXEL_UNIT,
	                      y2*FT800_PIXEL_UNIT));

	return ret;
}
FTVOID bootup_ui (FTU32 para)
{
#define LOGO_HOLD 800
	bmpHDR_st bmp_header[LOGO_NUM+1] = {
		{LOGO1_PATH,0,0,ARGB4,0,0,LOGO1_W,LOGO1_H},
		{LOGO2_PATH,0,0,ARGB4,0,0,LOGO2_W,LOGO2_H},
		{TRI_PATH,  0,0,ARGB4,0,0,TRI_W,  TRI_H}
	};
	static FTU8 flag = 0;
	FTU8 ret = 0;

	/* only load the file once */
	if (flag == 0) {
		/* load bitmap resources data into FT800 */
		if(APP_OK != appBmpToRamG(LOGO_HANDLE, RAM_G, bmp_header, LOGO_NUM+1)){
			DBGPRINT;
			return;
		}
		flag = 1;
	}

	do {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
		HAL_CmdBufIn(CLEAR(1,1,1));
		HAL_CmdBufIn(BEGIN(BITMAPS));

		ret = bootup_show();

		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);

	} while (ret);

	FTDELAY(LOGO_HOLD);

	appGP.appIndex = 1;
}
FTVOID chanX_body_form (FTU8 chan_x)
{
	HAL_CmdBufIn(LINE_WIDTH(1 * FT800_PIXEL_UNIT));
	if (chan_x == 4) {
		/* bottom */
		HAL_CmdBufIn(CHANX_COL_PR);
		HAL_CmdBufIn(TAG_MASK(1));
		HAL_CmdBufIn(TAG(eT_M_CA_PRBS));
		
		HAL_CmdBufIn(BEGIN(RECTS));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+2*CHANA_PARA_GAP+CHANX_PARA_W+CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+2*CHANA_PARA_GAP+2*CHANX_PARA_W-CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH)*FT800_PIXEL_UNIT));

		HAL_CmdBufIn(TAG(eT_M_CA_POLY));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+4*CHANA_PARA_GAP+3*CHANX_PARA_W+CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+4*CHANA_PARA_GAP+4*CHANX_PARA_W-CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH)*FT800_PIXEL_UNIT));
		
		HAL_CmdBufIn(TAG(eT_M_CA_GT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+6*CHANA_PARA_GAP+5*CHANX_PARA_W+CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+6*CHANA_PARA_GAP+6*CHANX_PARA_W-CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH)*FT800_PIXEL_UNIT));

		HAL_CmdBufIn(TAG(eT_M_CA_AMP));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+8*CHANA_PARA_GAP+7*CHANX_PARA_W+CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+8*CHANA_PARA_GAP+8*CHANX_PARA_W-CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(END());
		HAL_CmdBufIn(TAG_MASK(0));

		HAL_CmdBufIn(COLOR_RGB(255,255,255));
		if (ui_data.chan_data[1].prbs == ui_data.chan_data[0].prbs &&
			ui_data.chan_data[2].prbs == ui_data.chan_data[1].prbs &&
			ui_data.chan_data[3].prbs == ui_data.chan_data[2].prbs) {
			CoCmd_TEXT((chanX_list[chan_x].X+2*CHANA_PARA_GAP+CHANX_PARA_W+CHANX_LINE_W),
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH/2),
					CHANX_BODY_F-1,OPT_CENTERY,&prbs_str[ui_data.chan_data[chan_x].prbs][0]);
		} else {
			ui_data.chan_data[chan_x].prbs = 0;
		}

		if (ui_data.chan_data[1].polarity == ui_data.chan_data[0].polarity &&
			ui_data.chan_data[2].polarity == ui_data.chan_data[1].polarity &&
			ui_data.chan_data[3].polarity == ui_data.chan_data[2].polarity) {
			ui_data.chan_data[chan_x].polarity = (ui_data.chan_data[0].polarity == TOGGLE_TRUE)?1:0;
			CoCmd_TEXT((chanX_list[chan_x].X+4*CHANA_PARA_GAP+3*CHANX_PARA_W+CHANX_LINE_W),
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH/2),
					CHANX_BODY_F-1,OPT_CENTERY,&polarity_str[ui_data.chan_data[chan_x].polarity][0]);
		} else {
			ui_data.chan_data[chan_x].polarity = DATA_NOT_EQUAL;
		}

		if (ui_data.chan_data[1].gating_time == ui_data.chan_data[0].gating_time &&
			ui_data.chan_data[2].gating_time == ui_data.chan_data[1].gating_time &&
			ui_data.chan_data[3].gating_time == ui_data.chan_data[2].gating_time) {
			CoCmd_NUMBER((chanX_list[chan_x].X+6*CHANA_PARA_GAP+5*CHANX_PARA_W+CHANX_LINE_W),
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH/2),
					CHANX_BODY_F-1,OPT_CENTERY,ui_data.chan_data[chan_x].gating_time);
		} else {
			ui_data.chan_data[chan_x].gating_time = 0;
		}

		if (ui_data.chan_data[1].amplitude == ui_data.chan_data[0].amplitude &&
			ui_data.chan_data[2].amplitude == ui_data.chan_data[1].amplitude &&
			ui_data.chan_data[3].amplitude == ui_data.chan_data[2].amplitude) {
			CoCmd_NUMBER((chanX_list[chan_x].X+8*CHANA_PARA_GAP+7*CHANX_PARA_W+CHANX_LINE_W),
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH/2),
					CHANX_BODY_F-1,OPT_CENTERY,ui_data.chan_data[chan_x].amplitude);
		} else {
			ui_data.chan_data[chan_x].amplitude = 0;
		}

	} else {
		/* left */
		HAL_CmdBufIn(CHANX_COL_PR);
		HAL_CmdBufIn(TAG_MASK(1));
		if (trigger && eT_M_TOP == pre_tag && chan_x == 1) {
			/* Top would go down to chan 2 PRBS parts, disable it */
			HAL_CmdBufIn(TAG_MASK(0));
		}
		HAL_CmdBufIn(TAG(eT_M_C1_PRBS+chan_x*eCHANX_OFFSET));
		HAL_CmdBufIn(BEGIN(RECTS));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_PARA_L+CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_PARA_L+CHANX_PARA_W-CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(TAG_MASK(1));
		if (trigger && TOUCH_TRACK == touch_get_type(pre_tag)) {
			/* PRBS would go down to GT part, disable GT tag */
			HAL_CmdBufIn(TAG_MASK(0));
		}
		HAL_CmdBufIn(TAG(eT_M_C1_GT+chan_x*eCHANX_OFFSET));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_PARA_L+CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_H+2)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_PARA_L+CHANX_PARA_W-CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_H+CHANX_PARA_FH)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(TAG_MASK(0));

		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_PARA_L+CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+2*CHANX_PARA_H+2)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_PARA_L+CHANX_PARA_W-CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+2*CHANX_PARA_H+CHANX_PARA_FH)*FT800_PIXEL_UNIT));

		/* right */
		/*
		HAL_CmdBufIn(TAG_MASK(1));
		HAL_CmdBufIn(TAG(eT_M_C1_POLY+chan_x*eCHANX_OFFSET));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_PARA_R+CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_PARA_R+CHANX_PARA_W-CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(TAG_MASK(0));
		*/
		HAL_CmdBufIn(TAG_MASK(1));
		if (trigger && eT_M_TOP == pre_tag && chan_x == 0) {
			/* Top would go down to chan 1 AMP parts, disable it */
			HAL_CmdBufIn(TAG_MASK(0));
		}

		HAL_CmdBufIn(TAG(eT_M_C1_AMP+chan_x*eCHANX_OFFSET));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_PARA_R+CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_H+2)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_PARA_R+CHANX_PARA_W-CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_H+CHANX_PARA_FH)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(TAG_MASK(0));

		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_PARA_R+CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+2*CHANX_PARA_H+2)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_PARA_R+CHANX_PARA_W-CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+2*CHANX_PARA_H+CHANX_PARA_FH)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(END());

		HAL_CmdBufIn(COLOR_RGB(255,255,255));
		CoCmd_TEXT((chanX_list[chan_x].X+CHANX_PARA_L+CHANX_LINE_W),
		           (chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH/2),
				   CHANX_BODY_F-1,OPT_CENTERY,&prbs_str[ui_data.chan_data[chan_x].prbs][0]);

		CoCmd_NUMBER((chanX_list[chan_x].X+CHANX_PARA_L+CHANX_LINE_W),
		           (chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_H+2+CHANX_PARA_FH/2),
				   CHANX_BODY_F-1,OPT_CENTERY,ui_data.chan_data[chan_x].gating_time);

		CoCmd_NUMBER((chanX_list[chan_x].X+CHANX_PARA_L+CHANX_LINE_W),
		           (chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+2*CHANX_PARA_H+2+CHANX_PARA_FH/2),
				   CHANX_BODY_F-1,OPT_CENTERY,ui_data.chan_data[chan_x].error_count);
		/*
		CoCmd_TEXT((chanX_list[chan_x].X+CHANX_PARA_R+CHANX_LINE_W),
		           (chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH/2),
				   CHANX_BODY_F-1,OPT_CENTERY,&polarity_str[ui_data.chan_data[chan_x].polarity][0]);
		*/

		CoCmd_NUMBER((chanX_list[chan_x].X+CHANX_PARA_R+CHANX_LINE_W),
		           (chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_H+2+CHANX_PARA_FH/2),
				   CHANX_BODY_F-1,OPT_CENTERY,ui_data.chan_data[chan_x].amplitude);

		CoCmd_NUMBER((chanX_list[chan_x].X+CHANX_PARA_R+CHANX_LINE_W),
		           (chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+2*CHANX_PARA_H+2+CHANX_PARA_FH/2),
				   CHANX_BODY_F-1,OPT_CENTERY,ui_data.chan_data[chan_x].error_ratio);

	}
}
FTVOID chanX_body_label (FTU8 chan_x)
{
	HAL_CmdBufIn(COLOR_RGB(0, 0, 0));
	if (chan_x == 4) {
		CoCmd_TEXT(chanX_list[chan_x].X+CHANA_PARA_GAP+CHANX_PARA_W,chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U,
				CHANX_BODY_F,OPT_RIGHTX,"PRBS");
		CoCmd_TEXT(chanX_list[chan_x].X+3*CHANA_PARA_GAP+3*CHANX_PARA_W,chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U,
				CHANX_BODY_F,OPT_RIGHTX,"Polarity");
		CoCmd_TEXT(chanX_list[chan_x].X+5*CHANA_PARA_GAP+5*CHANX_PARA_W,chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U,
				CHANX_BODY_F,OPT_RIGHTX,"Gating Time");
		CoCmd_TEXT(chanX_list[chan_x].X+7*CHANA_PARA_GAP+7*CHANX_PARA_W,chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U,
				CHANX_BODY_F,OPT_RIGHTX,"Amplitude");
	} else {
		CoCmd_TEXT(chanX_list[chan_x].X+CHANX_PARA_L,chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U,
				CHANX_BODY_F,OPT_RIGHTX,"PRBS");
		CoCmd_TEXT(chanX_list[chan_x].X+CHANX_PARA_L,chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_H,
				CHANX_BODY_F,OPT_RIGHTX,"Gating Time");
		CoCmd_TEXT(chanX_list[chan_x].X+CHANX_PARA_L,chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+2*CHANX_PARA_H,
				CHANX_BODY_F,OPT_RIGHTX,"Error Count");
		CoCmd_TEXT(chanX_list[chan_x].X+CHANX_PARA_R,chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U,
				CHANX_BODY_F,OPT_RIGHTX,"Polarity");
		CoCmd_TEXT(chanX_list[chan_x].X+CHANX_PARA_R,chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_H,
				CHANX_BODY_F,OPT_RIGHTX,"Amplitude");
		CoCmd_TEXT(chanX_list[chan_x].X+CHANX_PARA_R,chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+2*CHANX_PARA_H,
				CHANX_BODY_F,OPT_RIGHTX,"Error Ratio");
	}
}
FTVOID chanX_body_line (FTU8 chan_x)
{
	HAL_CmdBufIn(BEGIN(LINES));
	HAL_CmdBufIn(CHANX_COL_PR);
	HAL_CmdBufIn(LINE_WIDTH(1 * FT800_PIXEL_UNIT));
	if (chan_x == 4) {
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+2)*FT800_PIXEL_UNIT,
				(chanX_list[chan_x].Y+CHANX_BAR_H+CHANA_BUTN_LN)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANA_W-2)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANA_BUTN_LN)*FT800_PIXEL_UNIT));
	} else {
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+2)*FT800_PIXEL_UNIT,
				(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_BUTN_LN)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_W-2)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_BUTN_LN)*FT800_PIXEL_UNIT));
	}
	HAL_CmdBufIn(END());
}
FTVOID chanX_body_button (FTU8 chan_x, FTU8 chan_tag)
{
	HAL_CmdBufIn(TAG_MASK(1));
	HAL_CmdBufIn(COLOR_RGB(0, 0, 0));
	if (chan_tag == eT_M_CA_PPG_1) {
		CHANA_COL_BT;
	} else {
		CHANA_COL_BT1;
	}
	HAL_CmdBufIn(TAG(eT_M_CA_PPG_1));
	CoCmd_BUTTON(chanX_list[chan_x].X+CHANA_BUTN_GAP, chanX_list[chan_x].Y+CHANX_BAR_H+CHANA_BUTN_LN+3,
			CHANX_BUTN_W,CHANX_BUTN_H,CHANX_BODY_F,OPT_FLAT,"Start PPG");
	if (chan_tag == eT_M_CA_PPG_0) {
		CHANA_COL_BT;
	} else {
		CHANA_COL_BT1;
	}
	HAL_CmdBufIn(TAG(eT_M_CA_PPG_0));
	CoCmd_BUTTON(chanX_list[chan_x].X+2*CHANA_BUTN_GAP+CHANX_BUTN_W, chanX_list[chan_x].Y+CHANX_BAR_H+CHANA_BUTN_LN+3,
			CHANX_BUTN_W,CHANX_BUTN_H,CHANX_BODY_F,OPT_FLAT,"Stop PPG");
	if (chan_tag == eT_M_CA_BERT_1) {
		CHANA_COL_BT;
	} else {
		CHANA_COL_BT2;
	}
	HAL_CmdBufIn(TAG(eT_M_CA_BERT_1));
	CoCmd_BUTTON(chanX_list[chan_x].X+3*CHANA_BUTN_GAP+2*CHANX_BUTN_W, chanX_list[chan_x].Y+CHANX_BAR_H+CHANA_BUTN_LN+3,
			CHANX_BUTN_W,CHANX_BUTN_H,CHANX_BODY_F,OPT_FLAT,"Start BERT");
	if (chan_tag == eT_M_CA_BERT_0) {
		CHANA_COL_BT;
	} else {
		CHANA_COL_BT2;
	}
	HAL_CmdBufIn(TAG(eT_M_CA_BERT_0));
	CoCmd_BUTTON(chanX_list[chan_x].X+4*CHANA_BUTN_GAP+3*CHANX_BUTN_W, chanX_list[chan_x].Y+CHANX_BAR_H+CHANA_BUTN_LN+3,
			CHANX_BUTN_W,CHANX_BUTN_H,CHANX_BODY_F,OPT_FLAT,"Stop BERT");

	HAL_CmdBufIn(TAG_MASK(0));
}
FTVOID chanX_body_toggle (FTU8 chan_x)
{
	HAL_CmdBufIn(TAG_MASK(1));
	if (trigger && eT_M_CA_PRBS == pre_tag && chan_x == 2) {
		/* chan all PRBS would go up to chan 3 toggle parts, disable it */
		HAL_CmdBufIn(TAG_MASK(0));
	}
	HAL_CmdBufIn(COLOR_RGB(0, 0, 0));
	CHANX_COL_TOG;

	CHANX_COL_TOG1;
	HAL_CmdBufIn(TAG(eT_M_C1_PPG+chan_x*eCHANX_OFFSET));
	CoCmd_TOGGLE(chanX_list[chan_x].X+CHANX_W/4-CHANX_BUTN_W/2,chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_BUTN_LN+CHANX_BUTN_H/2,
			CHANX_BUTN_W,CHANX_BODY_F,OPT_FLAT,ui_data.chan_data[chan_x].ppg,"Stop PPG""\xFF""   Start PPG");

	CHANX_COL_TOG2;
	HAL_CmdBufIn(TAG(eT_M_C1_BERT+chan_x*eCHANX_OFFSET));
	CoCmd_TOGGLE(chanX_list[chan_x].X+CHANX_W/4*3-CHANX_BUTN_W/2,chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_BUTN_LN+CHANX_BUTN_H/2,
			CHANX_BUTN_W,CHANX_BODY_F,OPT_FLAT,ui_data.chan_data[chan_x].bert,"Stop BERT""\xFF""   Start BERT");
	HAL_CmdBufIn(TAG_MASK(1));
	if (trigger && eT_M_TOP == pre_tag && chan_x == 0) {
		/* Top would go down to chan 1 toggle parts, disable it */
		HAL_CmdBufIn(TAG_MASK(0));
	}

	HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
	CHANX_COL_TOG3;
	HAL_CmdBufIn(TAG(eT_M_C1_POLY+chan_x*eCHANX_OFFSET));
	CoCmd_TOGGLE((chanX_list[chan_x].X+CHANX_PARA_R+CHANX_LINE_W+10),
	             (chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH/4),
				 CHANX_PARA_W-30,CHANX_BODY_F-1,OPT_FLAT,ui_data.chan_data[chan_x].polarity,"Normal""\xFF""   Invert");
	HAL_CmdBufIn(TAG_MASK(0));
}
FTVOID chanX_body (FTU8 chan_x, FTU8 chan_tag)
{
	HAL_CmdBufIn(BEGIN(RECTS));
	HAL_CmdBufIn(CHANX_COL_BY);
	HAL_CmdBufIn(LINE_WIDTH(CHANX_LINE_W * FT800_PIXEL_UNIT));
	if (chan_x == 4) {
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANA_W-CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANA_BODY_H)*FT800_PIXEL_UNIT));
	} else {
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_W-CHANX_LINE_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_BODY_H)*FT800_PIXEL_UNIT));
	}
	HAL_CmdBufIn(END());

	chanX_body_form(chan_x);

	chanX_body_label(chan_x);

	chanX_body_line(chan_x);

	if (chan_x == 4) {
		chanX_body_button(chan_x, chan_tag);
	} else {
		chanX_body_toggle(chan_x);
	}
}
FTVOID chanX_bar (FTU8 chan_x, FTU8 chan_tag, FTU8 led_red)
{
	HAL_CmdBufIn(BEGIN(RECTS));
	HAL_CmdBufIn(CHANX_COL_BAR);
	HAL_CmdBufIn(LINE_WIDTH(1 * FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(chanX_list[chan_x].X*FT800_PIXEL_UNIT,
	                      chanX_list[chan_x].Y*FT800_PIXEL_UNIT));
	if (chan_x == 4) {
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANA_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H)*FT800_PIXEL_UNIT));
	} else {
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_W)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H)*FT800_PIXEL_UNIT));
	}
	HAL_CmdBufIn(END());

	/* text */
	HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
	if (chan_x == 4) {
		CoCmd_TEXT(chanX_list[chan_x].X+CHANX_TITLE1,chanX_list[chan_x].Y+CHANX_BAR_H/6,
				CHANX_BAR_F,OPT_RIGHTX,chanX_list[chan_x].str);
	} else {
		CoCmd_TEXT(chanX_list[chan_x].X+CHANX_TITLE1,chanX_list[chan_x].Y+CHANX_BAR_H/6,
				CHANX_BAR_F,OPT_RIGHTX,chanX_list[chan_x].str);
		HAL_CmdBufIn(COLOR_RGB(0, 0, 0));
		CoCmd_TEXT(chanX_list[chan_x].X+CHANX_TITLE2,chanX_list[chan_x].Y+CHANX_BAR_H/6,
				CHANX_BAR_F,OPT_RIGHTX,"LOS");

		/* led */
		HAL_CmdBufIn(BEGIN(FTPOINTS));
		if (led_red) {
			HAL_CmdBufIn(COLOR_RGB(255, 0, 0));
		} else {
			HAL_CmdBufIn(COLOR_RGB(0, 255, 0));
		}
		HAL_CmdBufIn(POINT_SIZE(CHANX_LED_RA*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_LED)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H/2)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(END());
	}

}
FTVOID chanX_triangle (FTU8 chan_x)
{
	HAL_CmdBufIn(BEGIN(BITMAPS));
	HAL_CmdBufIn(BITMAP_HANDLE(LOGO_HANDLE+2));
	HAL_CmdBufIn(CELL(0));
	if (chan_x == 4) {
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+2*CHANA_PARA_GAP+2*CHANX_PARA_W-CHANX_LINE_W-TRI_W-TRI_GAP)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH/2)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+4*CHANA_PARA_GAP+4*CHANX_PARA_W-CHANX_LINE_W-TRI_W-TRI_GAP)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH/2)*FT800_PIXEL_UNIT));
	} else {
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_PARA_L+CHANX_PARA_W-CHANX_LINE_W-TRI_W-TRI_GAP)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH/2)*FT800_PIXEL_UNIT));
		/*
		HAL_CmdBufIn(VERTEX2F((chanX_list[chan_x].X+CHANX_PARA_R+CHANX_PARA_W-CHANX_LINE_W-TRI_W-TRI_GAP)*FT800_PIXEL_UNIT,
					(chanX_list[chan_x].Y+CHANX_BAR_H+CHANX_PARA_U+CHANX_PARA_FH/2)*FT800_PIXEL_UNIT));
		*/
	}
	HAL_CmdBufIn(END());
}
FTVOID chanX_ui (FTU8 chan_x, FTU8 chan_tag, FTU8 led_red)
{
	chanX_body(chan_x, chan_tag);

	chanX_bar(chan_x, chan_tag, led_red);
	
	chanX_triangle(chan_x);
}
FTVOID top_ui (FTVOID)
{
	HAL_CmdBufIn(SAVE_CONTEXT());
	/* Top, middle form */
	HAL_CmdBufIn(BEGIN(RECTS));
	HAL_CmdBufIn(CHANX_COL_PR);
	HAL_CmdBufIn(TAG_MASK(1));
	HAL_CmdBufIn(TAG(eT_M_TOP));
	HAL_CmdBufIn(VERTEX2F(TOP_FORM_X*FT800_PIXEL_UNIT,TOP_FORM_Y*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F((TOP_FORM_X+TOP_FORM_W)*FT800_PIXEL_UNIT,(TOP_FORM_Y+TOP_FORM_H)*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(TAG_MASK(0));
	HAL_CmdBufIn(END());
	HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
	CoCmd_TEXT(TOP_FORM_X,TOP_FORM_Y+TOP_FORM_H/2,TOP_FONT+1,OPT_CENTERY,&top_str[ui_data.data_top][0]);


	/* Top, version */
	HAL_CmdBufIn(CHANX_COL_PR);
	CoCmd_TEXT(TOP_VER_X,TOP_UP,TOP_FONT+1,0,ver_str);
	
	/* Top, date and time */
	CoCmd_NUMBER(TOP_YEAR_X,TOP_UP,TOP_FONT,OPT_RIGHTX,top_time.year);
	CoCmd_TEXT(TOP_YEAR_X+3,TOP_UP-3,TOP_FONT+1,0,"-");
	CoCmd_NUMBER(TOP_MONTH_X,TOP_UP,TOP_FONT,OPT_RIGHTX,top_time.month);
	CoCmd_TEXT(TOP_MONTH_X+3,TOP_UP-3,TOP_FONT+1,0,"-");
	CoCmd_NUMBER(TOP_DAY_X,TOP_UP,TOP_FONT,OPT_RIGHTX,top_time.day);
	CoCmd_NUMBER(TOP_HOUR_X,TOP_UP,TOP_FONT,OPT_RIGHTX,top_time.hour);
	CoCmd_TEXT(TOP_HOUR_X+3,TOP_UP-3,TOP_FONT+1,0,":");
	CoCmd_NUMBER(TOP_MINUTE_X,TOP_UP,TOP_FONT,OPT_RIGHTX,top_time.minute);
	CoCmd_TEXT(TOP_MINUTE_X+3,TOP_UP-3,TOP_FONT+1,0,":");
	CoCmd_NUMBER(TOP_SECOND_X,TOP_UP,TOP_FONT,OPT_RIGHTX,top_time.second);

	HAL_CmdBufIn(RESTORE_CONTEXT());

	/* Top, logo */
	HAL_CmdBufIn(BEGIN(BITMAPS));
	HAL_CmdBufIn(BITMAP_HANDLE(LOGO_HANDLE));
	HAL_CmdBufIn(CELL(0));
	HAL_CmdBufIn(VERTEX2F(LOGO1_X*FT800_PIXEL_UNIT,
	                      LOGO1_Y*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(BITMAP_HANDLE(LOGO_HANDLE+1));
	HAL_CmdBufIn(CELL(0));
	HAL_CmdBufIn(VERTEX2F(LOGO2_X*FT800_PIXEL_UNIT,
	                      LOGO2_Y*FT800_PIXEL_UNIT));

	/* Top, black triangle */
	HAL_CmdBufIn(BITMAP_HANDLE(LOGO_HANDLE+2));
	HAL_CmdBufIn(CELL(0));
	HAL_CmdBufIn(VERTEX2F((TOP_FORM_X+TOP_FORM_W-TRI_W-TRI_GAP)*FT800_PIXEL_UNIT,
	                      (TOP_FORM_Y+TOP_FORM_H/2)*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(END());
}

FTVOID data_handle ()
{
	static FTU8 i = 0;
	if (i++ == 20) {
		ui_data.chan_data[0].error_count = FTRANDOM(1000);
		ui_data.chan_data[0].error_ratio = FTRANDOM(100);
		ui_data.chan_data[1].error_count = FTRANDOM(1000);
		ui_data.chan_data[1].error_ratio = FTRANDOM(100);
		ui_data.chan_data[2].error_count = FTRANDOM(1000);
		ui_data.chan_data[2].error_ratio = FTRANDOM(100);
		ui_data.chan_data[3].error_count = FTRANDOM(1000);
		ui_data.chan_data[3].error_ratio = FTRANDOM(100);
		i = 0;
	}
}
FTVOID main_ui (FTU32 para)
{
	static FTU8 tag = 0;

	data_handle();


	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));

	top_ui();
	if (tag == eT_M_C1_GT || pre_tag == eT_M_C1_GT ||
		tag == eT_M_C1_PRBS || pre_tag == eT_M_C1_PRBS ||
		tag == eT_M_C1_AMP || pre_tag == eT_M_C1_AMP) {
		chanX_ui(0,tag,led_color[0]);
	} else if (tag == eT_M_C2_GT || pre_tag == eT_M_C2_GT ||
		tag == eT_M_C2_PRBS || pre_tag == eT_M_C2_PRBS ||
		tag == eT_M_C2_AMP || pre_tag == eT_M_C2_AMP) {
		chanX_ui(1,tag,led_color[1]);
	} else if (tag == eT_M_C3_GT || pre_tag == eT_M_C3_GT ||
		tag == eT_M_C3_PRBS || pre_tag == eT_M_C3_PRBS ||
		tag == eT_M_C3_AMP || pre_tag == eT_M_C3_AMP) {
		chanX_ui(2,tag,led_color[2]);
	} else if (tag == eT_M_C4_GT || pre_tag == eT_M_C4_GT ||
		tag == eT_M_C4_PRBS || pre_tag == eT_M_C4_PRBS ||
		tag == eT_M_C4_AMP || pre_tag == eT_M_C4_AMP) {
		chanX_ui(3,tag,led_color[3]);
	} else if (tag == eT_M_CA_GT || pre_tag == eT_M_CA_GT ||
		tag == eT_M_CA_PRBS || pre_tag == eT_M_CA_PRBS ||
		tag == eT_M_CA_AMP || pre_tag == eT_M_CA_AMP) {
		chanX_ui(4,tag,0);
	} else if (!(tag == eT_M_TOP || pre_tag == eT_M_TOP)) {
		chanX_ui(0,tag,led_color[0]);
		chanX_ui(1,tag,led_color[1]);
		chanX_ui(2,tag,led_color[2]);
		chanX_ui(3,tag,led_color[3]);
		chanX_ui(4,tag,0);
	}

	touch_control(tag);

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
	
	tag = HAL_Read8(REG_TOUCH_TAG);
	appGP.appIndex = 1;
}
AppFunc Apps[] = {
	/* System bootup logo show */
	bootup_ui,
	main_ui,

	/* Leave this NULL at the buttom of this array */
	NULL
};

