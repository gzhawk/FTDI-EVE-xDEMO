/* 
    Demo for Coobot's UI
    Author: Hawk Gao
	Email : hawk.gao@ftdichip.com
	Date  : May/2014
*/

enum speed_e {
	SPD_WRTE = 0,
	SPD_READ,
	SPD_FILL,
	SPD_MAX
};

enum tag_index_e {
	T_PRE = 0,
	T_NOW,
	T_MAX
};

enum coobot_e {
	TAG_DATA = 1,
	TAG_UPDT,
	TAG_STAR,
	TAG_STOP,
	TAG_OK,
	TAG_CL
};

typedef struct coobot_st {
	FTU8 tag[T_MAX];
	FTU8 set;
	FT32 speed[SPD_MAX];
	FTU32 dc_v;
	FTU32 current;
	FTU32 power;
	FTU32 err_code;
} coobot_t;

#define DEMO_SPEED  80

#define DEFAULT_SPD (-8765)
#define DEFAULT_DCV 12
#define DEFAULT_CUR 500
#define DEFAULT_PWR 40
#define DEFAULT_ERR 12

#define MAX_SPD 100000
#define MAX_DCV 100
#define MAX_CUR 1000
#define MAX_PWR 1000
#define MAX_ERR 1000

#define PUB_FONT    21 
#define BUT_FONT    23 
#define NUM_FONT    25 

#define T_COLOR     COLOR_RGB(0,0,0xFF)
#define F_COLOR     COLOR_RGB(208,208,191)
#define W_COLOR     COLOR_RGB(0,0,0)
#define S_COLOR     COLOR_RGB(0xFF,0xFF,0xFF)
#define START_COLOR COLOR_RGB(0,0xFF,0)
#define STOP_COLOR  COLOR_RGB(0xFF,0,0)
#define BK_COLOR    CLEAR_COLOR_RGB(236,233,216)
#define BK_COLOR_BOOT    CLEAR_COLOR_RGB(0,0,0)
#define FG_COLR_0   (0xF3F3EF)
#define FG_COLR_1   (0xD7D7CA)

#define DISP_W      100
#define DISP_H      30
#define DISP_G      10
#define DISP_F_H    (30+5*(DISP_H+DISP_G))

#define PUB_F_Y        30
#define PUB_T_Y        (PUB_F_Y-8)

#define INFO_F_X       240
#define INFO_T_X       (INFO_F_X+10)
#define INFO_T_Y       (PUB_F_Y-8)
#define INFO_F_W       (50+DISP_W+70)
#define INFO_IN_X      (INFO_T_X+10)
#define INFO_DSP_X     (INFO_IN_X+50)
#define INFO_DSP_UNT_G 5
#define INFO_DSP_SPD_Y (PUB_F_Y+20)
#define INFO_DSP_DCV_Y (INFO_DSP_SPD_Y+DISP_H+DISP_G)
#define INFO_DSP_CUR_Y (INFO_DSP_DCV_Y+DISP_H+DISP_G)
#define INFO_DSP_PWR_Y (INFO_DSP_CUR_Y+DISP_H+DISP_G)
#define INFO_DSP_ERR_Y (INFO_DSP_PWR_Y+DISP_H+DISP_G)
#define INFO_IN_SPD_Y  (PUB_F_Y+30)
#define INFO_IN_DCV_Y  (INFO_IN_SPD_Y+DISP_H+DISP_G)
#define INFO_IN_CUR_Y  (INFO_IN_DCV_Y+DISP_H+DISP_G)
#define INFO_IN_PWR_Y  (INFO_IN_CUR_Y+DISP_H+DISP_G)
#define INFO_IN_ERR_Y  (INFO_IN_PWR_Y+DISP_H+DISP_G)

#define SPD_F_X      20
#define SPD_T_X      (SPD_F_X+10)
#define SPD_F_W      (40+DISP_W+60)
#define SPD_IN_X     (SPD_T_X+10)
#define SPD_IN_SPD_Y (PUB_F_Y+20)
#define SPD_DATA_Y   (SPD_IN_SPD_Y+10)
#define SPD_IN_RPM_Y (SPD_DATA_Y+5)
#define SPD_IN_B_W   (DISP_W+30)
#define SPD_IN_B_H   (DISP_H+10)
#define SPD_IN_B_U_Y (SPD_DATA_Y+SPD_IN_B_H+DISP_G)
#define SPD_IN_B_S_Y (SPD_IN_B_U_Y+SPD_IN_B_H+DISP_G)
#define SPD_IN_B_P_Y (SPD_IN_B_S_Y+SPD_IN_B_H+DISP_G)

#define FILLING_X      (FT800_LCD_WIDTH/3/2)
#define FILLING_XX     (FT800_LCD_WIDTH-FILLING_X)
#define FILLING_Y      (FT800_LCD_HIGH/4/2)
#define FILLING_YY     (FT800_LCD_HIGH-FILLING_Y)
#define FILLING_DSP_YY (FILLING_Y+(FILLING_YY-FILLING_Y)/5)
#define FILLING_KEY_W  ((FILLING_XX-FILLING_X)/4*3)
#define FILLING_KEY_H  ((FILLING_YY-FILLING_Y)/5)
#define FILLING_K1_Y   FILLING_DSP_YY
#define FILLING_K4_Y   (FILLING_K1_Y+FILLING_KEY_H)
#define FILLING_K7_Y   (FILLING_K4_Y+FILLING_KEY_H)
#define FILLING_K0_Y   (FILLING_K7_Y+FILLING_KEY_H)
#define FILLING_BTN_W  ((FILLING_XX-FILLING_X)/4)
#define FILLING_BTN_H  ((FILLING_YY-FILLING_Y)/5*4/2)
#define FILLING_BTN_X  (FILLING_X+FILLING_KEY_W)
#define FILLING_BTN_Y1 FILLING_DSP_YY
#define FILLING_BTN_Y2 (FILLING_BTN_Y1+FILLING_BTN_H)
#define FILLING_K1_3(x) ((x)>='1'?((x)<='3'?(x):0):0)
#define FILLING_K4_6(x) ((x)>='4'?((x)<='6'?(x):0):0)
#define FILLING_K7_9(x) ((x)>='7'?((x)<='9'?(x):0):0)
#define FILLING_K0_M(x) ((x)!='0'?((x)!='-'?0:(x)):(x))

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
#define PATH_ICON_1 ROOT_PATH"coobot\\logo.raw"
#define PATH_ICON_2 ROOT_PATH"coobot\\logo.raw"
#define PATH_ICON_3 ROOT_PATH"coobot\\logo.raw"
#define PATH_ICON_4 ROOT_PATH"coobot\\logo.raw"
#else
#define PATH_ICON_1 ROOT_PATH"logo.raw"
#define PATH_ICON_2 ROOT_PATH"logo.raw"
#define PATH_ICON_3 ROOT_PATH"logo.raw"
#define PATH_ICON_4 ROOT_PATH"logo.raw"
#endif

#define ICON_NUM 1

#define LOGO_ZOOM  50

bmpHDR_st bmpLogoHD[ICON_NUM] = {
	{PATH_ICON_1, 0,0,ARGB1555, 0,0, 211, 221},
};

coobot_t UIData = {
	{0,0},
	0,
	{DEFAULT_SPD,DEFAULT_SPD,0},
	DEFAULT_DCV,
	DEFAULT_CUR,
	DEFAULT_PWR,
	DEFAULT_ERR
};

FTINDEF FTVOID rect_line (FTU32 X, FTU32 Y, FTU32 XX, FTU32 YY)
{
	HAL_CmdBufIn(F_COLOR);
	HAL_CmdBufIn(BEGIN(LINES));	
	HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(XX*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,YY*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(XX*FT800_PIXEL_UNIT,YY*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(XX*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(XX*FT800_PIXEL_UNIT,YY*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,YY*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(END());	
}

FTINDEF FTVOID coobot_speed (FTVOID)
{
	HAL_CmdBufIn(T_COLOR);
	CoCmd_TEXT(SPD_T_X,PUB_T_Y,PUB_FONT,OPT_CENTERY,"Speed Control");
	
	rect_line(SPD_F_X,PUB_F_Y,(SPD_F_X+SPD_F_W),(PUB_F_Y+DISP_F_H));

	HAL_CmdBufIn(W_COLOR);
	CoCmd_TEXT(SPD_IN_X,SPD_IN_SPD_Y,PUB_FONT,OPT_CENTERY,"Speed Reference");

	HAL_CmdBufIn(S_COLOR);
	HAL_CmdBufIn(TAG_MASK(1));	
	HAL_CmdBufIn(TAG(TAG_DATA));
	HAL_CmdBufIn(BEGIN(RECTS));	
	HAL_CmdBufIn(VERTEX2F(SPD_IN_X*FT800_PIXEL_UNIT,SPD_DATA_Y*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F((SPD_IN_X+DISP_W)*FT800_PIXEL_UNIT,(SPD_DATA_Y+DISP_H)*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(END());
	HAL_CmdBufIn(TAG_MASK(0));	

	rect_line(SPD_IN_X,SPD_DATA_Y,(SPD_IN_X+DISP_W),(SPD_DATA_Y+DISP_H));
	
	HAL_CmdBufIn(W_COLOR);
	CoCmd_TEXT((SPD_IN_X+DISP_W+5),SPD_IN_RPM_Y,PUB_FONT,0,"RPM");

	CoCmd_NUMBER(SPD_IN_X+DISP_W,SPD_DATA_Y,NUM_FONT,OPT_RIGHTX|OPT_SIGNED,UIData.speed[SPD_WRTE]);

	HAL_CmdBufIn(TAG_MASK(1));	
	HAL_CmdBufIn(TAG(TAG_UPDT));
	if (TAG_UPDT == UIData.tag[T_NOW]) {
		CoCmd_FGCOLOR(FG_COLR_1);
	} else {
		CoCmd_FGCOLOR(FG_COLR_0);
	}
	CoCmd_BUTTON(SPD_IN_X,SPD_IN_B_U_Y,SPD_IN_B_W,SPD_IN_B_H,BUT_FONT,0,"Update Speed");
	
	HAL_CmdBufIn(TAG(TAG_STAR));
	if (TAG_STAR == UIData.tag[T_NOW]) {
		CoCmd_FGCOLOR(FG_COLR_1);
	} else {
		CoCmd_FGCOLOR(FG_COLR_0);
	}
	HAL_CmdBufIn(START_COLOR);
	if (UIData.tag[T_PRE] != TAG_STAR) {
		CoCmd_BUTTON(SPD_IN_X,SPD_IN_B_S_Y,SPD_IN_B_W,SPD_IN_B_H,BUT_FONT,0,"Start");
	} else {
		CoCmd_BUTTON(SPD_IN_X,SPD_IN_B_S_Y,SPD_IN_B_W,SPD_IN_B_H,BUT_FONT,OPT_FLAT,"Start");
	}
	
	HAL_CmdBufIn(TAG(TAG_STOP));
	if (TAG_STOP == UIData.tag[T_NOW]) {
		CoCmd_FGCOLOR(FG_COLR_1);
	} else {
		CoCmd_FGCOLOR(FG_COLR_0);
	}
	HAL_CmdBufIn(STOP_COLOR);
	if (UIData.tag[T_PRE] != TAG_STAR) {
		CoCmd_BUTTON(SPD_IN_X,SPD_IN_B_P_Y,SPD_IN_B_W,SPD_IN_B_H,BUT_FONT,OPT_FLAT,"Stop");
	} else {
		CoCmd_BUTTON(SPD_IN_X,SPD_IN_B_P_Y,SPD_IN_B_W,SPD_IN_B_H,BUT_FONT,0,"Stop");
	}
	HAL_CmdBufIn(TAG_MASK(0));	
	
	HAL_CmdBufIn(W_COLOR);
}

FTINDEF FTVOID coobot_info (FTVOID)
{
	HAL_CmdBufIn(T_COLOR);
	CoCmd_TEXT(INFO_T_X,INFO_T_Y,PUB_FONT,OPT_CENTERY,"Status Infomation");

	rect_line(INFO_F_X,PUB_F_Y,(INFO_F_X+INFO_F_W),(PUB_F_Y+DISP_F_H));

	rect_line(INFO_DSP_X,INFO_DSP_SPD_Y,(INFO_DSP_X+DISP_W),(INFO_DSP_SPD_Y+DISP_H));

	rect_line(INFO_DSP_X,INFO_DSP_DCV_Y,(INFO_DSP_X+DISP_W),(INFO_DSP_DCV_Y+DISP_H));

	rect_line(INFO_DSP_X,INFO_DSP_CUR_Y,(INFO_DSP_X+DISP_W),(INFO_DSP_CUR_Y+DISP_H));

	rect_line(INFO_DSP_X,INFO_DSP_PWR_Y,(INFO_DSP_X+DISP_W),(INFO_DSP_PWR_Y+DISP_H));

	rect_line(INFO_DSP_X,INFO_DSP_ERR_Y,(INFO_DSP_X+DISP_W),(INFO_DSP_ERR_Y+DISP_H));

	HAL_CmdBufIn(W_COLOR);
	CoCmd_TEXT(INFO_IN_X,INFO_IN_SPD_Y,PUB_FONT,0,"Speed");
	CoCmd_TEXT(INFO_DSP_X+DISP_W+INFO_DSP_UNT_G,INFO_IN_SPD_Y,PUB_FONT,0,"RPM");
	CoCmd_TEXT(INFO_IN_X,INFO_IN_DCV_Y,PUB_FONT,0,"DC Volt");
	CoCmd_TEXT(INFO_DSP_X+DISP_W+INFO_DSP_UNT_G,INFO_IN_DCV_Y,PUB_FONT,0,"V");
	CoCmd_TEXT(INFO_IN_X,INFO_IN_CUR_Y,PUB_FONT,0,"Current");
	CoCmd_TEXT(INFO_DSP_X+DISP_W+INFO_DSP_UNT_G,INFO_IN_CUR_Y,PUB_FONT,0,"A");
	CoCmd_TEXT(INFO_IN_X,INFO_IN_PWR_Y,PUB_FONT,0,"Power");
	CoCmd_TEXT(INFO_DSP_X+DISP_W+INFO_DSP_UNT_G,INFO_IN_PWR_Y,PUB_FONT,0,"W");
	CoCmd_TEXT(INFO_IN_X,INFO_IN_ERR_Y,PUB_FONT,0,"ErrCode");
	
	CoCmd_NUMBER(INFO_DSP_X+DISP_W,INFO_DSP_SPD_Y,NUM_FONT,OPT_RIGHTX|OPT_SIGNED,UIData.speed[SPD_READ]);
	CoCmd_NUMBER(INFO_DSP_X+DISP_W,INFO_DSP_DCV_Y,NUM_FONT,OPT_RIGHTX,UIData.dc_v);
	CoCmd_NUMBER(INFO_DSP_X+DISP_W,INFO_DSP_CUR_Y,NUM_FONT,OPT_RIGHTX,UIData.current);
	CoCmd_NUMBER(INFO_DSP_X+DISP_W,INFO_DSP_PWR_Y,NUM_FONT,OPT_RIGHTX,UIData.power);
	CoCmd_NUMBER(INFO_DSP_X+DISP_W,INFO_DSP_ERR_Y,NUM_FONT,OPT_RIGHTX,UIData.err_code);
}

FTINDEF FTU8 isFilling(FTU8 tag)
{
	switch (tag) {
		case TAG_DATA:
		case TAG_OK:
		case TAG_CL:
		case '-':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return 1;
		default:
			return 0;
	}
}

FTINDEF FTVOID filling_handle (FTU8 data)
{
	static FTU8 init = 1;
	FT32 speed = UIData.speed[SPD_FILL], tmp;
	
	if (init) {
		speed = 0;
		init = 0;
	}

	if (data >= '0' && data <= '9') {
		tmp = data - '0';
		if (speed == 0) {
			speed = tmp;
		} else {
			speed *= 10;
			speed += tmp;
		}
	} else if (data == '-') {
		if (speed >= 0) {
			speed *= -1;
		}
	} else {
		if (data == TAG_CL) {
			speed = 0;
		}
		init = 1;
	}

	if (MAX_SPD <= speed || (MAX_SPD*(-1)) >= speed) {
		speed = UIData.speed[SPD_FILL];
	}

	UIData.speed[SPD_FILL] = speed;
}

FTINDEF FTVOID coobot_filling (FTVOID)
{
	/* make filling form appear only when tap the filling area */
	if (isFilling(UIData.tag[T_PRE])) {

		/* make it be recorded only once for single click */
		if (UIData.set) {
			filling_handle(UIData.tag[T_PRE]);
			UIData.set = 0;
		}
		
		HAL_CmdBufIn(F_COLOR);
		rect_line(FILLING_X-1,FILLING_Y-1,FILLING_XX+1,FILLING_YY+1);
		HAL_CmdBufIn(BEGIN(RECTS));	
		HAL_CmdBufIn(S_COLOR);
		HAL_CmdBufIn(VERTEX2F(FILLING_X*FT800_PIXEL_UNIT,FILLING_Y*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(VERTEX2F(FILLING_XX*FT800_PIXEL_UNIT,FILLING_DSP_YY*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(W_COLOR);
		CoCmd_NUMBER(FILLING_XX,FILLING_Y,NUM_FONT,OPT_RIGHTX|OPT_SIGNED,UIData.speed[SPD_FILL]);
		HAL_CmdBufIn(END());

		HAL_CmdBufIn(TAG_MASK(1));
		CoCmd_FGCOLOR(FG_COLR_0);
		CoCmd_BGCOLOR(FG_COLR_1);
		CoCmd_KEY(FILLING_X,FILLING_K1_Y,FILLING_KEY_W,FILLING_KEY_H,BUT_FONT,FILLING_K1_3(UIData.tag[T_NOW]),"123");
		CoCmd_KEY(FILLING_X,FILLING_K4_Y,FILLING_KEY_W,FILLING_KEY_H,BUT_FONT,FILLING_K4_6(UIData.tag[T_NOW]),"456");
		CoCmd_KEY(FILLING_X,FILLING_K7_Y,FILLING_KEY_W,FILLING_KEY_H,BUT_FONT,FILLING_K7_9(UIData.tag[T_NOW]),"789");
		CoCmd_KEY(FILLING_X,FILLING_K0_Y,FILLING_KEY_W,FILLING_KEY_H,BUT_FONT,FILLING_K0_M(UIData.tag[T_NOW]),"0-");
		
		HAL_CmdBufIn(TAG(TAG_OK));
		if (TAG_OK == UIData.tag[T_NOW]) {
			CoCmd_FGCOLOR(FG_COLR_1);
		} else {
			CoCmd_FGCOLOR(FG_COLR_0);
		}
		CoCmd_BUTTON(FILLING_BTN_X,FILLING_BTN_Y1,FILLING_BTN_W,FILLING_BTN_H,BUT_FONT,0,"OK");
		HAL_CmdBufIn(TAG(TAG_CL));
		if (TAG_CL == UIData.tag[T_NOW]) {
			CoCmd_FGCOLOR(FG_COLR_1);
		} else {
			CoCmd_FGCOLOR(FG_COLR_0);
		}
		CoCmd_BUTTON(FILLING_BTN_X,FILLING_BTN_Y2,FILLING_BTN_W,FILLING_BTN_H,BUT_FONT,0,"Cancel");
		HAL_CmdBufIn(TAG_MASK(0));	
	}
}

FTVOID coobot_data_handle (FTU32 para)
{
	FTU8 touch;
	static FTU8 count = 0;

	touch = HAL_Read8(REG_TOUCH_TAG);

	if (UIData.tag[T_PRE] == TAG_OK || UIData.tag[T_PRE] == TAG_CL) {
		/* set the filling data into write area */
		UIData.speed[SPD_WRTE] = (UIData.tag[T_PRE] == TAG_OK)?UIData.speed[SPD_FILL]:UIData.speed[SPD_WRTE];
		UIData.tag[T_PRE] = 0;
	}

	if (touch == 0 && UIData.tag[T_NOW] != 0) {
		UIData.tag[T_PRE] = UIData.tag[T_NOW];
		UIData.set = 1;
	}

	if (UIData.tag[T_PRE] == TAG_UPDT && UIData.set) {
		/* Just a demo to make the display 'looks like' setting data */
		UIData.speed[SPD_READ] = UIData.speed[SPD_WRTE];
		UIData.set = 0;
	}

	if ((UIData.tag[T_PRE] == TAG_STOP && touch == TAG_STOP) || 
	    (UIData.tag[T_PRE] == TAG_UPDT && touch == TAG_STOP) || 
		(UIData.tag[T_PRE] == TAG_STAR && touch == TAG_STAR)) {
		/* control the disable area */
		UIData.tag[T_NOW] = 0;
	} else {
		UIData.tag[T_NOW] = touch;
	}

	if (count++ == DEMO_SPEED && UIData.tag[T_PRE] == TAG_STAR) {
		/* Just a demo to make the display 'looks like' reading data */
		UIData.speed[SPD_READ]++;
		UIData.dc_v            = FTRANDOM(MAX_DCV);
		UIData.current         = FTRANDOM(MAX_CUR);
		UIData.power           = FTRANDOM(MAX_PWR);
		UIData.err_code        = FTRANDOM(MAX_ERR);
		count = 0;
	}
	
	appGP.appIndex++;
}

FTINDEF FTVOID coobot_Zoomin (FTU8 handle, FT32 X, FT32 Y, FT32 W, FT32 H)
{
	FT32 step;
	
	/* animation for logo zoom in */
	for (step = 1; step <= LOGO_ZOOM; step++) {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(BK_COLOR_BOOT);
		HAL_CmdBufIn(CLEAR(1,1,1));

		/* bitmap area setting */
		HAL_CmdBufIn(BEGIN(BITMAPS));
		CoCmd_LOADIDENTITY;
		CoCmd_TRANSLATE(W/2*FT800_TRANSFORM_MAX,H/2*FT800_TRANSFORM_MAX);
		CoCmd_SCALE(FT800_TRANSFORM_MAX/LOGO_ZOOM*step,FT800_TRANSFORM_MAX/LOGO_ZOOM*step);
		CoCmd_TRANSLATE(-W/2*FT800_TRANSFORM_MAX,-H/2*FT800_TRANSFORM_MAX);
		CoCmd_SETMATRIX;
		HAL_CmdBufIn(VERTEX2II(X,Y,handle,0));

		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
	}
}

FTINDEF FTVOID coobot_Gradient(FTVOID)
{
	FT32 Y;

	/* animation for background gradient */
	for (Y = 0; Y < FT800_LCD_HIGH; Y += 10) {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(BK_COLOR);
		HAL_CmdBufIn(CLEAR(1,1,1));
		CoCmd_GRADIENT(0,0,0xECE9D8,0,Y,0);
		
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
	}
}

FTVOID coobot_startup (FTU32 para)
{	
	/* for input number transfer between subroutine */
	appGP.appPara = (FTU32)&UIData;
#if !defined(STM32F4)
	/* load bitmap resources data into FT800 */
	if(APP_OK != appBmpToRamG(0, 0, bmpLogoHD, ICON_NUM)) {
		DBGPRINT;
		return;
	}

	FTDELAY(1000);
	
	coobot_Zoomin(0, FT800_LCD_WIDTH/2-211/2, FT800_LCD_HIGH/2-221/2, 211, 221);
	
	coobot_Gradient();
	appGP.appIndex = 1;
#endif
	return;
}
FTVOID coobot_ui (FTU32 para)
{
	/* main part use to display the UI */
	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(BK_COLOR);
	HAL_CmdBufIn(CLEAR(1,1,1));

	/* display the left part "speed" */
	coobot_speed();

	/* display the right part "information" */
	coobot_info();

	/* display the filling form */
	coobot_filling();

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);

	appGP.appIndex = 1;
}

AppFunc APPS_UI[] = {
	/* Show the startup LOGO */
	coobot_startup,
	/* Handling the data */
	coobot_data_handle,
	/* Display on LCD */
	coobot_ui
};

