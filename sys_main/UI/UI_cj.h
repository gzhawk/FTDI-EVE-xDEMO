/* 
    Sample code for Chuangjian
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2015/Nov
*/

enum eTag_ {
	eT_SWT = 1,
	eT_PWM,
	eT_SWT_LOBBY,
	eT_PWM1_SW = eT_SWT_LOBBY, /* whole new screen, reuse the tag */
	eT_SWT_BED,
	eT_PWM2_SW = eT_SWT_BED, /* whole new screen, reuse the tag */
	eT_SWT_BED1,
	eT_PWM3_SW = eT_SWT_BED1, /* whole new screen, reuse the tag */
	eT_SWT_BED2,
	eT_SWT_KITCHEN,
	eT_SWT_DINNER,
	eT_SWT_WC,
	eT_PWM1_SLDR,
	eT_PWM2_SLDR,
	eT_PWM3_SLDR,
	eT_MAX
} eTag;

#define PATH_SWT ROOT_PATH"cj\\home.jpg"
#define PATH_PWM ROOT_PATH"cj\\motor.jpg"

/* make sure FIFOSIZE larger than JPG file size
   and less than (1024-LCD_WIDTH*LCD_HIGH*2/1024) */
#define CJ_FIFOSIZE        (150*1024)
#define CJ_FIFOADDR        (FT800_RAMG_SIZE - CJ_FIFOSIZE)

#define BUTN_SWT_X 0
#define BUTN_PWM_X (FT800_LCD_WIDTH/2)
#define BUTN_Y 0
#define BUTN_W (FT800_LCD_WIDTH/2)
#define BUTN_H (FT800_LCD_HIGH/16)
#define BUTN_F (30)

#define SWT_PIC_W 714
#define SWT_PIC_H 418
#define SWT_LED_KD_D_X 182
#define SWT_LED_KD_K_X 100
#define SWT_LED_KD_Y 248
#define SWT_LED_WC_X 150
#define SWT_LED_WC_Y 350
#define SWT_LED_LOBBY_X 366
#define SWT_LED_LOBBY_Y 112
#define SWT_LED_BED_X 493
#define SWT_LED_BED_Y 344
#define SWT_LED_BED1_X 100
#define SWT_LED_BED1_Y 85
#define SWT_LED_BED2_X 600
#define SWT_LED_BED2_Y 170
#define SWT_LED_RA (8)

#define PWM_MOTO_GAP (FT800_LCD_WIDTH/6)
#define PWM_LINE_GAP (FT800_LCD_WIDTH/3)
#define PWM_COLOR    HAL_CmdBufIn(COLOR_RGB(109, 115, 115))
#define PWM_SWITCH_BG CoCmd_BGCOLOR((109<<16)|(115<<8)|115)
#define PWM_F (23)
#define PWM_SLIDER_H (FT800_LCD_HIGH/32)
#define PWM_SLIDER_W (PWM_LINE_GAP/3*2)
#define PWM_SLIDER_GAP ((PWM_LINE_GAP - PWM_SLIDER_W)/2)
#define PWM_RATIO_NUM_Y (BUTN_H*3)
#define PWM_RATIO_F (21)
#define PWM_RATIO_GAP(a) ((PWM_SLIDER_W*(a))/100)

#define RATIO_MARK   20
#define RATIO_MARK_H 150
#define RATIO_WAVE_ON 50
#define WAVE_HIGH_Y (PWM_RATIO_NUM_Y+RATIO_WAVE_ON)
#define WAVE_LOW_Y (PWM_RATIO_NUM_Y+RATIO_MARK_H)
#define WAVE_1_X (PWM_SLIDER_GAP+PWM_RATIO_GAP(motor_pwm.pwm1.duty_ratio))
#define WAVE_2_X (PWM_LINE_GAP+PWM_SLIDER_GAP+PWM_RATIO_GAP(motor_pwm.pwm2.duty_ratio))
#define WAVE_3_X (PWM_LINE_GAP*2+PWM_SLIDER_GAP+PWM_RATIO_GAP(motor_pwm.pwm3.duty_ratio))

#if defined(FT9XXEV)
#define GPIO_INIT_STAT 1
#define GPIO_SWT_LOBBY 18
#define GPIO_SWT_BED   17
#define GPIO_SWT_BED1  15
#define GPIO_SWT_BED2  13
#define GPIO_SWT_KITCHEN  11
#define GPIO_SWT_DINNER   6
#define GPIO_SWT_WC    9

#define GPIO_PWM_1   56
#define GPIO_PWM_2   57
#define GPIO_PWM_3   58
#endif

#define SWT_LED_COLOR_OFF HAL_CmdBufIn(COLOR_RGB(0, 0, 0))
#define SWT_LED_COLOR_ON HAL_CmdBufIn(COLOR_RGB(255, 255, 0))

typedef struct home_sw {
	FTU8 lobby;
	FTU8 bed;
	FTU8 bed1;
	FTU8 bed2;
	FTU8 kitchen;
	FTU8 dinner;
	FTU8 wc;
} home_sw_st;

home_sw_st home_sw = {0};

#define PWM_PIC_W 100
#define PWM_PIC_H 73

typedef struct motor_ {
	FTU32 motor_off_on;
	FTU8 duty_ratio;
} motor_st;

typedef struct motor_pwm_ {
	motor_st pwm1;
	motor_st pwm2;
	motor_st pwm3;
} motor_pwm_st;

motor_pwm_st motor_pwm = {{0,10},{0,50},{0,80}};
#define PWM_COUNT_INIT 100 //16bits
#define PWM_PRE_INIT 1 //8bits

FTU8 swt_pwm = 0;

FTVOID cj_gpio_init (FTVOID)
{
/* currently, this demo's GPIO functon only works in FT9XX */
#if defined(FT9XXEV)
	gpio_function(GPIO_SWT_LOBBY, pad_gpio18);
	gpio_function(GPIO_SWT_BED, pad_gpio17);
	gpio_function(GPIO_SWT_BED1, pad_gpio15);
	gpio_function(GPIO_SWT_BED2, pad_gpio13);
	gpio_function(GPIO_SWT_KITCHEN, pad_gpio11);
	gpio_function(GPIO_SWT_DINNER, pad_gpio6);
	gpio_function(GPIO_SWT_WC, pad_gpio9);

	gpio_dir(GPIO_SWT_LOBBY, pad_dir_output);
	gpio_dir(GPIO_SWT_BED, pad_dir_output);
	gpio_dir(GPIO_SWT_BED1, pad_dir_output);
	gpio_dir(GPIO_SWT_BED2, pad_dir_output);
	gpio_dir(GPIO_SWT_KITCHEN, pad_dir_output);
	gpio_dir(GPIO_SWT_DINNER, pad_dir_output);
	gpio_dir(GPIO_SWT_WC, pad_dir_output);

	gpio_write(GPIO_SWT_LOBBY, GPIO_INIT_STAT);
	gpio_write(GPIO_SWT_BED, GPIO_INIT_STAT);
	gpio_write(GPIO_SWT_BED1, GPIO_INIT_STAT);
	gpio_write(GPIO_SWT_BED2, GPIO_INIT_STAT);
	gpio_write(GPIO_SWT_KITCHEN, GPIO_INIT_STAT);
	gpio_write(GPIO_SWT_DINNER, GPIO_INIT_STAT);
	gpio_write(GPIO_SWT_WC, GPIO_INIT_STAT);
#endif
}


FTVOID cj_swt_gpio (FTVOID)
{
/* currently, this demo's GPIO functon only works in FT9XX */
#if defined(FT9XXEV)
	gpio_write(GPIO_SWT_LOBBY, home_sw.lobby);
	gpio_write(GPIO_SWT_BED, home_sw.bed);
	gpio_write(GPIO_SWT_BED1, home_sw.bed1);
	gpio_write(GPIO_SWT_BED2, home_sw.bed2);
	gpio_write(GPIO_SWT_KITCHEN, home_sw.kitchen);
	gpio_write(GPIO_SWT_DINNER, home_sw.dinner);
	gpio_write(GPIO_SWT_WC, home_sw.wc);
#endif
}

FTVOID cj_swt_one_led (FTU8 off_on, FTU8 ra)
{
	if (off_on) {
		SWT_LED_COLOR_ON;
	} else {
		SWT_LED_COLOR_OFF;
	}

	HAL_CmdBufIn(POINT_SIZE(ra*FT800_PIXEL_UNIT));
}

FTVOID cj_swt_sw (FTU8 Tag)
{
	switch (Tag) {
		case eT_SWT_LOBBY:
			home_sw.lobby = home_sw.lobby?0:1;
			break;
		case eT_SWT_BED:
			home_sw.bed = home_sw.bed?0:1;
			break;
		case eT_SWT_BED1:
			home_sw.bed1 = home_sw.bed1?0:1;
			break;
		case eT_SWT_BED2:
			home_sw.bed2 = home_sw.bed2?0:1;
			break;
		case eT_SWT_KITCHEN:
			home_sw.kitchen = home_sw.kitchen?0:1;
			break;
		case eT_SWT_DINNER:
			home_sw.dinner = home_sw.dinner?0:1;
			break;
		case eT_SWT_WC:
			home_sw.wc = home_sw.wc?0:1;
			break;
		default:
			break;
	}
}

FTVOID cj_swt_led (FTU32 x, FTU32 y, FTU8 Tag)
{
#define SWT_LED_SPEED 10
	static FTU8 Ra = SWT_LED_RA;
	static FTU32 speed = 0;
	
	cj_swt_sw(Tag);

	HAL_CmdBufIn(BEGIN(FTPOINTS));
	HAL_CmdBufIn(TAG_MASK(1));
	HAL_CmdBufIn(TAG(eT_SWT_LOBBY));
	cj_swt_one_led(home_sw.lobby,home_sw.lobby?Ra:SWT_LED_RA);
	HAL_CmdBufIn(VERTEX2F((x+SWT_LED_LOBBY_X)*FT800_PIXEL_UNIT,
						  (y+SWT_LED_LOBBY_Y)*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(TAG(eT_SWT_BED));
	cj_swt_one_led(home_sw.bed,home_sw.bed?Ra:SWT_LED_RA);
	HAL_CmdBufIn(VERTEX2F((x+SWT_LED_BED_X)*FT800_PIXEL_UNIT,
						  (y+SWT_LED_BED_Y)*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(TAG(eT_SWT_BED1));
	cj_swt_one_led(home_sw.bed1,home_sw.bed1?Ra:SWT_LED_RA);
	HAL_CmdBufIn(VERTEX2F((x+SWT_LED_BED1_X)*FT800_PIXEL_UNIT,
						  (y+SWT_LED_BED1_Y)*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(TAG(eT_SWT_BED2));
	cj_swt_one_led(home_sw.bed2,home_sw.bed2?Ra:SWT_LED_RA);
	HAL_CmdBufIn(VERTEX2F((x+SWT_LED_BED2_X)*FT800_PIXEL_UNIT,
						  (y+SWT_LED_BED2_Y)*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(TAG(eT_SWT_KITCHEN));
	cj_swt_one_led(home_sw.kitchen,home_sw.kitchen?Ra:SWT_LED_RA);
	HAL_CmdBufIn(VERTEX2F((x+SWT_LED_KD_K_X)*FT800_PIXEL_UNIT,
						  (y+SWT_LED_KD_Y)*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(TAG(eT_SWT_DINNER));
	cj_swt_one_led(home_sw.dinner,home_sw.dinner?Ra:SWT_LED_RA);
	HAL_CmdBufIn(VERTEX2F((x+SWT_LED_KD_D_X)*FT800_PIXEL_UNIT,
						  (y+SWT_LED_KD_Y)*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(TAG(eT_SWT_WC));
	cj_swt_one_led(home_sw.wc,home_sw.wc?Ra:SWT_LED_RA);
	HAL_CmdBufIn(VERTEX2F((x+SWT_LED_WC_X)*FT800_PIXEL_UNIT,
						  (y+SWT_LED_WC_Y)*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(TAG_MASK(0));
	HAL_CmdBufIn(END());

	if (SWT_LED_SPEED == speed++) {
		if (Ra++ == 2*SWT_LED_RA) {
			Ra = SWT_LED_RA;
		}
		speed = 0;
	}
}

FTVOID cj_swt (FTU8 Tag)
{
	FTU32 x = ((FT800_LCD_WIDTH-SWT_PIC_W)/2), y = ((FT800_LCD_HIGH-SWT_PIC_H)/2);

	HAL_CmdBufIn(BITMAP_HANDLE(0));
	HAL_CmdBufIn(BITMAP_SOURCE(RAM_G));
	HAL_CmdBufIn(BITMAP_LAYOUT(RGB565, (SWT_PIC_W * 2), SWT_PIC_H));
	HAL_CmdBufIn(BITMAP_LAYOUT_H((SWT_PIC_W * 2) >> 10, SWT_PIC_H >> 9)); 
	HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, SWT_PIC_W, SWT_PIC_H));
	HAL_CmdBufIn(BITMAP_SIZE_H(SWT_PIC_W>>9, SWT_PIC_H>>9));
	HAL_CmdBufIn(BEGIN(BITMAPS));
	HAL_CmdBufIn(VERTEX2F(x*FT800_PIXEL_UNIT, y*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(END());

	cj_swt_led(x, y, Tag);
	
	cj_swt_gpio();
}

FTVOID cj_pwm_compare (FTU8 num, FTU8 ratio)
{
#if defined(FT9XXEV)
	pwm_compare(num,(PWM_COUNT_INIT/100)*ratio);
#endif
}

FTVOID cj_pwm_init (FTVOID)
{
/* currently, this demo's PWM functon only works in FT9XX */
#if defined(FT9XXEV)
    /* Enable the PWM subsystem */
    sys_enable(sys_device_pwm);
    
    /* Set up pins for PWM */
    gpio_function(GPIO_PWM_1, pad_pwm0);
    gpio_function(GPIO_PWM_2, pad_pwm1);
    gpio_function(GPIO_PWM_3, pad_pwm2);
    /* Initialise the PWM Subsystem... */
    pwm_init(PWM_PRE_INIT,PWM_COUNT_INIT,0);
    /* Set up PWM*/
    pwm_levels(0,pwm_state_high,pwm_restore_enable);
    pwm_levels(1,pwm_state_high,pwm_restore_enable);
    pwm_levels(2,pwm_state_high,pwm_restore_enable);
    /* Set Compare value */
	cj_pwm_compare(0,motor_pwm.pwm1.duty_ratio);
	cj_pwm_compare(1,motor_pwm.pwm2.duty_ratio);
	cj_pwm_compare(2,motor_pwm.pwm3.duty_ratio);

    /* Enable PWM... */
    pwm_enable(); 

#endif
}
FTVOID cj_pwm_toggle(FTU8 off_on, FTU8 a, FTU8 b)
{
#if defined(FT9XXEV)
    if (off_on) {
        pwm_add_toggle(a,b);
    } else {
        pwm_remove_toggle(a,b);
    }
#endif
}
FTVOID cj_pwm_sw (FTU8 Tag)
{
#define TOGGLE_ON 65535
	switch (Tag) {
		case eT_PWM1_SW:
			if (motor_pwm.pwm1.motor_off_on) {
				motor_pwm.pwm1.motor_off_on = 0;
				cj_pwm_toggle(0,0,0);
			} else {
				motor_pwm.pwm1.motor_off_on = TOGGLE_ON;
				cj_pwm_toggle(1,0,0);
			}
			break;
		case eT_PWM2_SW:
			if (motor_pwm.pwm2.motor_off_on) {
				motor_pwm.pwm2.motor_off_on = 0;
				cj_pwm_toggle(0,1,1);
			} else {
				motor_pwm.pwm2.motor_off_on = TOGGLE_ON;
				cj_pwm_toggle(1,1,1);
			}
			break;
		case eT_PWM3_SW:
			if (motor_pwm.pwm3.motor_off_on) {
				motor_pwm.pwm3.motor_off_on = 0;
				cj_pwm_toggle(0,2,2);
			} else {
				motor_pwm.pwm3.motor_off_on = TOGGLE_ON;
				cj_pwm_toggle(1,2,2);
			}
			break;
		default:
			break;
	}
}

FTVOID cj_pwm_motor (FTVOID)
{
	/* icon */
	HAL_CmdBufIn(BITMAP_HANDLE(0));
	HAL_CmdBufIn(BITMAP_SOURCE(RAM_G));
	HAL_CmdBufIn(BITMAP_LAYOUT(RGB565, (PWM_PIC_W * 2), PWM_PIC_H));
	HAL_CmdBufIn(BITMAP_LAYOUT_H((PWM_PIC_W * 2) >> 10, PWM_PIC_H >> 9)); 
	HAL_CmdBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, PWM_PIC_W, PWM_PIC_H));
	HAL_CmdBufIn(BITMAP_SIZE_H(PWM_PIC_W>>9, PWM_PIC_H>>9));
	HAL_CmdBufIn(BEGIN(BITMAPS));
	HAL_CmdBufIn(VERTEX2F((PWM_MOTO_GAP-PWM_PIC_W)*FT800_PIXEL_UNIT,(FT800_LCD_HIGH-PWM_PIC_H)*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F((PWM_MOTO_GAP*3-PWM_PIC_W)*FT800_PIXEL_UNIT,(FT800_LCD_HIGH-PWM_PIC_H)*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F((PWM_MOTO_GAP*5-PWM_PIC_W)*FT800_PIXEL_UNIT,(FT800_LCD_HIGH-PWM_PIC_H)*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(END());

	/* switch */
	HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
	PWM_SWITCH_BG;
	HAL_CmdBufIn(TAG_MASK(1));
	HAL_CmdBufIn(TAG(eT_PWM1_SW));
	CoCmd_TOGGLE(PWM_MOTO_GAP+30,FT800_LCD_HIGH - BUTN_H-10,PWM_MOTO_GAP/2,
				 PWM_F,0,motor_pwm.pwm1.motor_off_on,"Start""\xFF""   Stop");
	HAL_CmdBufIn(TAG(eT_PWM2_SW));
	CoCmd_TOGGLE(PWM_MOTO_GAP*3+30,FT800_LCD_HIGH - BUTN_H-10,PWM_MOTO_GAP/2,
				 PWM_F,0,motor_pwm.pwm2.motor_off_on,"Start""\xFF""   Stop");
	HAL_CmdBufIn(TAG(eT_PWM3_SW));
	CoCmd_TOGGLE(PWM_MOTO_GAP*5+30,FT800_LCD_HIGH - BUTN_H-10,PWM_MOTO_GAP/2,
				 PWM_F,0,motor_pwm.pwm3.motor_off_on,"Start""\xFF""   Stop");
	HAL_CmdBufIn(TAG_MASK(0));
}

FTVOID cj_pwm_number (FTVOID)
{
	PWM_COLOR;
	/* zone number */
	CoCmd_NUMBER(PWM_MOTO_GAP,BUTN_H,BUTN_F,OPT_CENTERX,1);
	CoCmd_NUMBER(PWM_MOTO_GAP*3,BUTN_H,BUTN_F,OPT_CENTERX,2);
	CoCmd_NUMBER(PWM_MOTO_GAP*5,BUTN_H,BUTN_F,OPT_CENTERX,3);
	
	/* duty ratio */
	CoCmd_NUMBER(WAVE_1_X,PWM_RATIO_NUM_Y,PWM_RATIO_F,OPT_CENTERX,motor_pwm.pwm1.duty_ratio);
	CoCmd_NUMBER(WAVE_2_X,PWM_RATIO_NUM_Y,PWM_RATIO_F,OPT_CENTERX,motor_pwm.pwm2.duty_ratio);
	CoCmd_NUMBER(WAVE_3_X,PWM_RATIO_NUM_Y,PWM_RATIO_F,OPT_CENTERX,motor_pwm.pwm3.duty_ratio);
}

FTVOID cj_pwm_lines (FTVOID)
{
	PWM_COLOR;

	/* seperated line */
	HAL_CmdBufIn(LINE_WIDTH(1 * FT800_PIXEL_UNIT));
	HAL_CmdBufIn(BEGIN(LINES));
	HAL_CmdBufIn(VERTEX2F(PWM_LINE_GAP*FT800_PIXEL_UNIT,BUTN_H*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(PWM_LINE_GAP*FT800_PIXEL_UNIT,FT800_LCD_HIGH*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(PWM_LINE_GAP*2*FT800_PIXEL_UNIT,BUTN_H*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(PWM_LINE_GAP*2*FT800_PIXEL_UNIT,FT800_LCD_HIGH*FT800_PIXEL_UNIT));

	/* wave form */
	/* wave form 1.1 high*/
	HAL_CmdBufIn(VERTEX2F(PWM_SLIDER_GAP*FT800_PIXEL_UNIT, WAVE_HIGH_Y*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(WAVE_1_X*FT800_PIXEL_UNIT, WAVE_HIGH_Y*FT800_PIXEL_UNIT));

	/* wave form 1.1 low*/
	HAL_CmdBufIn(VERTEX2F(WAVE_1_X*FT800_PIXEL_UNIT, WAVE_LOW_Y*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F((PWM_SLIDER_GAP+PWM_SLIDER_W)*FT800_PIXEL_UNIT, WAVE_LOW_Y*FT800_PIXEL_UNIT));

	/* wave form 2.1 high*/
	HAL_CmdBufIn(VERTEX2F((PWM_LINE_GAP+PWM_SLIDER_GAP)*FT800_PIXEL_UNIT,
						  WAVE_HIGH_Y*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(WAVE_2_X*FT800_PIXEL_UNIT, WAVE_HIGH_Y*FT800_PIXEL_UNIT));

	/* wave form 2.1 low*/
	HAL_CmdBufIn(VERTEX2F(WAVE_2_X*FT800_PIXEL_UNIT, WAVE_LOW_Y*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F((PWM_LINE_GAP+PWM_SLIDER_GAP+PWM_SLIDER_W)*FT800_PIXEL_UNIT,
						  WAVE_LOW_Y*FT800_PIXEL_UNIT));

	/* wave form 3.1 high*/
	HAL_CmdBufIn(VERTEX2F((PWM_LINE_GAP*2+PWM_SLIDER_GAP)*FT800_PIXEL_UNIT,
						  (PWM_RATIO_NUM_Y+RATIO_WAVE_ON)*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(WAVE_3_X*FT800_PIXEL_UNIT, (PWM_RATIO_NUM_Y+RATIO_WAVE_ON)*FT800_PIXEL_UNIT));

	/* wave form 3.1 low*/
	HAL_CmdBufIn(VERTEX2F(WAVE_3_X*FT800_PIXEL_UNIT, (PWM_RATIO_NUM_Y+RATIO_MARK_H)*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F((PWM_LINE_GAP*2+PWM_SLIDER_GAP+PWM_SLIDER_W)*FT800_PIXEL_UNIT,
						  (PWM_RATIO_NUM_Y+RATIO_MARK_H)*FT800_PIXEL_UNIT));

	/* duty ratio */
	HAL_CmdBufIn(COLOR_RGB(255, 0, 0));
	HAL_CmdBufIn(VERTEX2F(WAVE_1_X*FT800_PIXEL_UNIT,
						  (PWM_RATIO_NUM_Y+RATIO_MARK)*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(WAVE_1_X*FT800_PIXEL_UNIT,
						  (PWM_RATIO_NUM_Y+RATIO_MARK_H)*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(VERTEX2F(WAVE_2_X*FT800_PIXEL_UNIT,
						  (PWM_RATIO_NUM_Y+RATIO_MARK)*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(WAVE_2_X*FT800_PIXEL_UNIT,
						  (PWM_RATIO_NUM_Y+RATIO_MARK_H)*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(VERTEX2F(WAVE_3_X*FT800_PIXEL_UNIT,
						  (PWM_RATIO_NUM_Y+RATIO_MARK)*FT800_PIXEL_UNIT));
	HAL_CmdBufIn(VERTEX2F(WAVE_3_X*FT800_PIXEL_UNIT,
						  (PWM_RATIO_NUM_Y+RATIO_MARK_H)*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(END());
}

FTVOID cj_pwm_slider (FTVOID)
{
	PWM_COLOR;
	HAL_CmdBufIn(TAG_MASK(1));
	if (!motor_pwm.pwm1.motor_off_on) {
		HAL_CmdBufIn(TAG(eT_PWM1_SLDR));
		CoCmd_SLIDER(PWM_SLIDER_GAP, FT800_LCD_HIGH/3*2, PWM_SLIDER_W, PWM_SLIDER_H, 0, motor_pwm.pwm1.duty_ratio, 100);
		CoCmd_TRACK(PWM_SLIDER_GAP, FT800_LCD_HIGH/3*2, PWM_SLIDER_W, 1, eT_PWM1_SLDR);
	} else {
		CoCmd_TEXT(PWM_LINE_GAP/2, FT800_LCD_HIGH/3*2, PWM_F, OPT_CENTERX, "Duty Ratio (1%~99%)");
	}
	if (!motor_pwm.pwm2.motor_off_on) {
		HAL_CmdBufIn(TAG(eT_PWM2_SLDR));
		CoCmd_SLIDER(PWM_LINE_GAP+PWM_SLIDER_GAP, FT800_LCD_HIGH/3*2, PWM_SLIDER_W, PWM_SLIDER_H, 0, motor_pwm.pwm2.duty_ratio, 100);
		CoCmd_TRACK(PWM_LINE_GAP+PWM_SLIDER_GAP, FT800_LCD_HIGH/3*2, PWM_SLIDER_W, 1, eT_PWM2_SLDR);
	} else {
		CoCmd_TEXT(PWM_LINE_GAP+PWM_LINE_GAP/2, FT800_LCD_HIGH/3*2, PWM_F, OPT_CENTERX, "Duty Ratio (1%~99%)");
	}
	if (!motor_pwm.pwm3.motor_off_on) {
		HAL_CmdBufIn(TAG(eT_PWM3_SLDR));
		CoCmd_SLIDER(PWM_LINE_GAP*2+PWM_SLIDER_GAP, FT800_LCD_HIGH/3*2, PWM_SLIDER_W, PWM_SLIDER_H, 0, motor_pwm.pwm3.duty_ratio, 100);
		CoCmd_TRACK(PWM_LINE_GAP*2+PWM_SLIDER_GAP, FT800_LCD_HIGH/3*2, PWM_SLIDER_W, 1, eT_PWM3_SLDR);
	} else {
		CoCmd_TEXT(PWM_LINE_GAP*2+PWM_LINE_GAP/2, FT800_LCD_HIGH/3*2, PWM_F, OPT_CENTERX, "Duty Ratio (1%~99%)");
	}
	HAL_CmdBufIn(TAG_MASK(0));

}

FTVOID cj_pwm_ratio(FTVOID)
{
#define TRACK_MAX 65536
#define RATIO_UNIT (TRACK_MAX/100)
	FTU32 trk_tag = HAL_Read32(REG_TRACKER), trk_value;

	trk_value = (trk_tag  >> 16)&0xFFFF;
	trk_tag &= 0xFFFF;
	trk_value = trk_value/RATIO_UNIT;
	if (trk_value == 0) {
		trk_value = 1;
	} else if (trk_value >= 100) {
		trk_value = 99;
	}

	switch (trk_tag) {
		case eT_PWM1_SLDR:
			motor_pwm.pwm1.duty_ratio = trk_value;
			cj_pwm_compare(0, trk_value);
			break;
		case eT_PWM2_SLDR:
			motor_pwm.pwm2.duty_ratio = trk_value;
			cj_pwm_compare(1, trk_value);
			break;
		case eT_PWM3_SLDR:
			motor_pwm.pwm3.duty_ratio = trk_value;
			cj_pwm_compare(2, trk_value);
			break;
		default:
			break;
	}
}

FTVOID cj_pwm (FTU8 Tag)
{
	cj_pwm_sw(Tag);

	cj_pwm_ratio();

	cj_pwm_motor();

	cj_pwm_number();

	cj_pwm_lines();

	cj_pwm_slider();
}

FTU32 cj_mfifoJpegWrite (FTU32 mfifo_addr, FTU32 mfifo_size,FTU32 disp_addr,FTU32 opt,FTU32 resHDL, FTU32 file_len)
{
	FTU32 mfifo_rd, mfifo_wr;

	/* mfifo size should not less than file len ,
	 * mfifo address must different with display area in RAMG */
	if ((mfifo_size < file_len) || 
		((disp_addr >= mfifo_addr) && 
		 (disp_addr <= (mfifo_addr + mfifo_size)))) {
		return 0;
	}

	/* set the MEDIAFIFO space in RAMG*/
	CoCmd_MEDIAFIFO(mfifo_addr, mfifo_size);

	HAL_BufToReg(RAM_CMD, 0);

	/* write data to RAM_G */
	appResToDes(resHDL, mfifo_addr, 0, file_len, resWrEve);

	HAL_Write32(REG_MEDIAFIFO_WRITE, file_len);

	CoCmd_LOADIMAGE(disp_addr,opt);

	HAL_BufToReg(RAM_CMD, 0);

	/* wait till previous action finished */
	do {
		mfifo_rd = HAL_Read32(REG_MEDIAFIFO_READ); 
		mfifo_wr = HAL_Read32(REG_MEDIAFIFO_WRITE);
	} while (mfifo_rd != mfifo_wr);
	

	return file_len;
}

FTVOID cj_JPEGToRamG(FTU8 flag)
{
	FTU32 resHDL, Len;
	static FTU8 pre = 0xFF;

	/* avoid load the same jpeg again */
	if (pre != flag) {
		pre = flag;
	} else {
		return;
	}

	if (flag) {
		resHDL = appResOpen((FTU8 *)PATH_PWM);
	} else {
		resHDL = appResOpen((FTU8 *)PATH_SWT);
	}

	if (resHDL == 0) {
		DBGPRINT;
	}

	Len = appResSize(resHDL);
	if (FT800_RAMG_SIZE < RAM_G + Len) {
		appResClose(resHDL);
		DBGPRINT;
	}

	cj_mfifoJpegWrite(CJ_FIFOADDR,CJ_FIFOSIZE,RAM_G,OPT_MEDIAFIFO,resHDL,Len);

	appResClose(resHDL);
}

/* avoid intermittent touch */
FTVOID cj_sensitive (FTU8 *pDelayTag, FTU8 tag)
{
#define PRESS_SENSITIVE 50
	static FTU32 delay[2] = {eT_MAX,PRESS_SENSITIVE};

	if (eT_SWT_LOBBY <= tag && tag <= eT_SWT_WC) {
		if (delay[0] == tag && delay[1]) {
			*pDelayTag = 0;
		} else 	if (*pDelayTag != tag && 0xFF != tag && tag) {
			*pDelayTag = tag;
			delay[0] = tag;
			delay[1] = PRESS_SENSITIVE;
		}
	} else {
		*pDelayTag = tag;
	}

	if (eT_SWT_LOBBY <= delay[0] && delay[0] <= eT_SWT_WC && delay[1]) {
		delay[1]--;
	} else {
		delay[0] = 0;
		delay[1] = 0;
	}
}
FTVOID cj_main (FTU32 para)
{
	FTU8 tag = HAL_Read8(REG_TOUCH_TAG);
	/* avoid chaos display on JPEG, during screen exchange */
	static FTU8 clear = 0, delay_tag = 0;

	if (clear == 0) {
		cj_JPEGToRamG(swt_pwm);
	}

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255,255,255));
	HAL_CmdBufIn(CLEAR(1,1,1));

	if (clear == 0) {
		if (swt_pwm) {
			cj_pwm(delay_tag);
		} else {
			cj_swt(delay_tag);
		}
	}

	HAL_CmdBufIn(TAG_MASK(1));
	HAL_CmdBufIn(TAG(eT_SWT));
	if (swt_pwm) {
		HAL_CmdBufIn(COLOR_RGB(0, 0, 0));
	} else {
		HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
	}
	CoCmd_BUTTON(BUTN_SWT_X, BUTN_Y, BUTN_W, BUTN_H, BUTN_F, swt_pwm?0:OPT_FLAT, "DEMO: Switch");
	HAL_CmdBufIn(TAG(eT_PWM));
	if (swt_pwm) {
		HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
	} else {
		HAL_CmdBufIn(COLOR_RGB(0, 0, 0));
	}
	CoCmd_BUTTON(BUTN_PWM_X, BUTN_Y, BUTN_W, BUTN_H, BUTN_F, swt_pwm?OPT_FLAT:0, "DEMO: PWM");
	HAL_CmdBufIn(TAG_MASK(0));

	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);

	if ((eT_PWM == delay_tag) && (swt_pwm == 0)) {
		swt_pwm = 1;
		clear = 1;
	} else if ((eT_SWT == delay_tag) && (swt_pwm == 1)) {
		swt_pwm = 0;
		clear = 1;
	} else {
		clear = 0;
	}

	cj_sensitive(&delay_tag,tag);
}

FTVOID cj_init (FTU32 para)
{
	cj_gpio_init();
	
	cj_pwm_init();
	
	appGP.appIndex = 1;
}
AppFunc APPS_UI[] = {
	cj_init,
	cj_main,
	/* Leave this NULL at the buttom of this array */
	NULL
};

