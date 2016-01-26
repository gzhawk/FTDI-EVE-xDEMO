/* 
    Demo created by Born:
    Author: Born Jiang
	Email : born.jiang@ftdichip.com
	Date  : 2014/Feb
*/
#ifdef DEMO_DIAL
#include <math.h>
#define PI 3.1415926

#define BKGND_BLOB_GRAD_BKG CoCmd_GRADIENT(0,0,0xCBDEB6,0,FT800_LCD_HIGH,0)

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
#define PATH_BK_ICON ROOT_PATH"dial\\L8_H400.raw"
#define PATH_BK_ICON1 ROOT_PATH"dial\\L8_H200.raw"
#define PATH_BK_ICON2 ROOT_PATH"dial\\L8_V200.raw"
#define PATH_BK_ICON3 ROOT_PATH"dial\\L8_V400.raw"
#define PATH_ARROW_ICON ROOT_PATH "dial\\NEXT.raw"
#else
#define PATH_BK_ICON ROOT_PATH"L8_H400.raw"
#define PATH_BK_ICON1 ROOT_PATH"L8_H200.raw"
#define PATH_BK_ICON2 ROOT_PATH"L8_V200.raw"
#define PATH_BK_ICON3 ROOT_PATH"L8_V400.raw"
#define PATH_ARROW_ICON ROOT_PATH "NEXT.raw"
#endif

//you can modify these paramaters
#define TRACK_TOP_LEFT_X 30     //track left top x
#define TRACK_TOP_LEFT_Y 100     //track left top y
#define TRACK_TOP_LEFT_X1 220     //track left top x
#define TRACK_TOP_LEFT_Y1 20     //track left top y
#define TRACKTAG 10             //track TAG  

#define TRACK_HIGH0 40           //track high
#define TRACK_HIGH1 70           //track high
#define TRACK_LEN0 400           //this paramater same as the shadow bitmap length(L8_400x1.png),when WINDOW_ANGLE is 180, the length is arc diameter 
#define TRACK_LEN1 200           //this paramater same as the shadow bitmap length(L8_200x1.png),when WINDOW_ANGLE is 180, the length is arc diameter 

#define TRACK_HIGH2 200           //this paramater same as the shadow bitmap length(H8_400x1.png),when WINDOW_ANGLE is 180, the length is arc diameter 
#define TRACK_HIGH3 400           //this paramater same as the shadow bitmap length(H8_200x1.png),when WINDOW_ANGLE is 180, the length is arc diameter 
#define TRACK_LEN3 50             //track width
#define TRACK_LEN3 70             //track width
#define TRACK_LEN4 140             //track width

#define WINDOW_ANGLE0 180        //TRACK_LEN map arc's angle, max is 180, min>0,always is 180
#define WINDOW_ANGLE1 180        //TRACK_LEN map arc's angle, max is 180, min>0,always is 180

#define INTERVAL_ANGLE0 30       //ervry roll string interval angle
#define INTERVAL_ANGLE1 40       //ervry roll string interval angle
#define INTERVAL_ANGLE2 50       //ervry roll string interval angle
#define INTERVAL_ANGLE3 20       //ervry roll string interval angle

#define SCALEFACTOR0 20          //the paramater is the result of testing, modify this marco to adjust turn speed
#define SCALEFACTOR1 100          //the paramater is the result of testing, modify this marco to adjust turn speed

#define ROLL_FONT0 23                //common font on the scroll
#define SELECT_ROLL_FONT0 24         //selected font on the scroll
#define ROLL_FONT1 24                //common font on the scroll
#define SELECT_ROLL_FONT1 25         //selected font on the scroll


#define SHADOW_COLOR_R0 64
#define SHADOW_COLOR_G0 64
#define SHADOW_COLOR_B0 64

#define SHADOW_COLOR_R1 96
#define SHADOW_COLOR_G1 72
#define SHADOW_COLOR_B1 4

#define SHADOW_COLOR_R2 23
#define SHADOW_COLOR_G2 40
#define SHADOW_COLOR_B2 4

//these paramaters as bleow don't need to modify
#define WINDOW_MIDDLE_ANGLE 90  //Track middle map arc's angle,always is 90
#define WINDOW_MIDDLE_X (TRACK_TOP_LEFT_X)+(TRACK_LEN/2)      
#define WINDOW_MIDDLE_Y (TRACK_TOP_LEFT_Y)+(TRACK_HIGH/2)
#define WINDOW_START_ANGLE (WINDOW_MIDDLE_ANGLE)-(WINDOW_ANGLE)/2   //don't need to modidy
#define WINDOW_END_ANGLE (WINDOW_MIDDLE_ANGLE)+(WINDOW_ANGLE)/2     //don't need to modidy

#define HORIZONTAL 1 
#define VERTICAL 0

//these are next button paramates
#define NEXT_TOP_LEFT_X 400
#define NEXT_TOP_LEFT_Y 200
#define NEXTTAG 1
#define NEXT_HIGH 64
#define NEXT_WIDTH 64

typedef struct app_para_st {
	FTU32 appIndex;
	FTU32 appPara;
}app_para_t;

app_para_t appGP = {0};
FTU8 gIndex=0;

typedef enum icon_hdl {
	BACKGROUND_HDL0 = 0,
	H400_HDL,
	H200_HDL,
	V200_HDL,
	V400_HDL,
	NEXT_HDL,
	/* leave it at the buttom */
	BKGND_ICON_GROUP
} icon_hdl_e;

typedef struct BMPpara_ {
	bmpHDR_st bmpHD;
	FTU32 filter;
	FTU32 wrapx;
	FTU32 wrapy;
}BMP_para;

BMP_para bmpSetting[BKGND_ICON_GROUP] = {
	{{PATH_BK_ICON,	    0,0,L8,	    0,0,	TRACK_LEN0, 1         },NEAREST,BORDER,REPEAT},
	{{PATH_BK_ICON1,	0,0,L8,	    0,0,	TRACK_LEN1, 1         },NEAREST,BORDER,REPEAT},
	{{PATH_BK_ICON2,	0,0,L8,	    0,0,	1,          TRACK_HIGH2},NEAREST,REPEAT,BORDER},
	{{PATH_BK_ICON3,	0,0,L8,	    0,0,	1,          TRACK_HIGH3},NEAREST,REPEAT,BORDER},
	{{PATH_ARROW_ICON,	0,0,ARGB4,	0,0,	64,         64        },NEAREST,BORDER,BORDER}
};


/* Structure used for scroller implementation */
/* Only two samples are used for velocity calculation */
/* all the pixel precision are interms of 1/16ths */
typedef struct Ft_App_Scroller{
  FT32 limitflag;			//limit flag type - 0 means limit, 1 means rollover reserved paramater,don't need modify
  FT32 PrevTagValue;		//save the roll touch tag, init is 0. don't need modify
  float roll_angle;			//Sliding distance change to angle, init is 0,don't need modify
  FT32 roll_arc_len;		//Sliding distance,don't need modify
  FT32 trackprev;			//save the x,y of last touch, init is 0,don't need modify
  FTU8 verticalORhorizontal;//Vertical OR Horizontal
  FTU32 rgb_r;				//shadow color r,you can modify it
  FTU32 rgb_g;				//shadow color g,you can modify it
  FTU32 rgb_b;				//shadow color b,you can modify it
  FTU32 left_top_x;         //left top x,you can modify it
  FTU32 left_top_y;         //left top y,you can modify it
  FTU8 hdl;                 //handle of the shadow,you can modify it
  FTU32 len;                //this paramater same as the shadow bitmap length(L8_400x1.png),you need modify it if shadow bitmap length is change
  FTU32 high;               //track high ,you can modify it
  FT32 TagValue;			//tag value of track - 0 means no tag value (pressumed)
  FT32 ScaleFactor;         //this parameter is result of testing ,you can modify it and try, must >0
  float interval_angle;		//ervry roll string interval angle,you can modify it
  float window_angle;		 //pScroll->len map arc's angle, max is 180, min>0,always is 180,you can modify it
  float window_middle_angle;//Track middle map arc's angle,always is 90
  FTU8  roll_font;          //common font on the scroll
  FTU8  sel_roll_font;      //selected font on the scroll
  FTU8  display_num;        //display str num
  float window_start_angle;	//window start angle
  float window_end_angle;	//window end angle
  FTU32 window_middle_x;    //display window center x, don't need to modify it,init is 0
  FTU32 window_middle_y;    //display window center y, don't need to modify it,init is 0
  FT32 r;					//virtual arc circle radius
} Ft_App_Scroller_t;

Ft_App_Scroller_t xScroll;

#define SCROLL_NUM1 5
Ft_App_Scroller_t aScroll1[SCROLL_NUM1] = {
	{0,0,0,0,0,HORIZONTAL,SHADOW_COLOR_R0,SHADOW_COLOR_G0,SHADOW_COLOR_B0,TRACK_TOP_LEFT_X,TRACK_TOP_LEFT_Y,H400_HDL,TRACK_LEN0,TRACK_HIGH0,TRACKTAG,SCALEFACTOR0,INTERVAL_ANGLE0,WINDOW_ANGLE0,90,ROLL_FONT0,SELECT_ROLL_FONT0,0,0,0,0,0},
	{0,0,0,0,0,HORIZONTAL,SHADOW_COLOR_R0,SHADOW_COLOR_G0,SHADOW_COLOR_B0,TRACK_TOP_LEFT_X,TRACK_TOP_LEFT_Y,H400_HDL,TRACK_LEN0,TRACK_HIGH0,TRACKTAG,SCALEFACTOR1,INTERVAL_ANGLE0,WINDOW_ANGLE0,90,ROLL_FONT0,SELECT_ROLL_FONT0,0,0,0,0,0},
	{0,0,0,0,0,HORIZONTAL,SHADOW_COLOR_R1,SHADOW_COLOR_G1,SHADOW_COLOR_B1,TRACK_TOP_LEFT_X,TRACK_TOP_LEFT_Y,H400_HDL,TRACK_LEN0,TRACK_HIGH0,TRACKTAG,SCALEFACTOR0,INTERVAL_ANGLE1,WINDOW_ANGLE1,90,ROLL_FONT0,SELECT_ROLL_FONT0,0,0,0,0,0},
	{0,0,0,0,0,HORIZONTAL,SHADOW_COLOR_R1,SHADOW_COLOR_G1,SHADOW_COLOR_B1,TRACK_TOP_LEFT_X,TRACK_TOP_LEFT_Y,H400_HDL,TRACK_LEN0,TRACK_HIGH1,TRACKTAG,SCALEFACTOR0,INTERVAL_ANGLE1,WINDOW_ANGLE0,90,ROLL_FONT1,SELECT_ROLL_FONT1,0,0,0,0,0},
    {0,0,0,0,0,HORIZONTAL,SHADOW_COLOR_R2,SHADOW_COLOR_G2,SHADOW_COLOR_B2,TRACK_TOP_LEFT_X,TRACK_TOP_LEFT_Y,H200_HDL,TRACK_LEN1,TRACK_HIGH0,TRACKTAG,SCALEFACTOR0,INTERVAL_ANGLE1,WINDOW_ANGLE0,90,ROLL_FONT0,SELECT_ROLL_FONT0,0,0,0,0,0}
};
#define SCROLL_NUM2 1
Ft_App_Scroller_t aScroll2[SCROLL_NUM2] = {
	{0,0,0,0,0,HORIZONTAL,SHADOW_COLOR_R0,SHADOW_COLOR_G0,SHADOW_COLOR_B0,TRACK_TOP_LEFT_X,TRACK_TOP_LEFT_Y,H400_HDL,TRACK_LEN0,TRACK_HIGH0,TRACKTAG,SCALEFACTOR0,INTERVAL_ANGLE2,WINDOW_ANGLE0,90,ROLL_FONT0,SELECT_ROLL_FONT0,0,0,0,0,0}
};

#define SCROLL_NUM3 1
Ft_App_Scroller_t aScroll3[SCROLL_NUM3] = {
	{0,0,0,0,0,VERTICAL,SHADOW_COLOR_R2,SHADOW_COLOR_G2,SHADOW_COLOR_B2,TRACK_TOP_LEFT_X1,TRACK_TOP_LEFT_Y1,V200_HDL,TRACK_LEN4,TRACK_HIGH2,TRACKTAG,SCALEFACTOR0,INTERVAL_ANGLE0,WINDOW_ANGLE0,90,ROLL_FONT0,SELECT_ROLL_FONT0,0,0,0,0,0}
};

#define SCROLL_NUM4 3
Ft_App_Scroller_t aScroll4[SCROLL_NUM4] = {
	{0,0,0,0,0,VERTICAL,SHADOW_COLOR_R0,SHADOW_COLOR_G0,SHADOW_COLOR_B0,TRACK_TOP_LEFT_X1,TRACK_TOP_LEFT_Y1,V200_HDL,TRACK_LEN3,TRACK_HIGH2,TRACKTAG,SCALEFACTOR0,INTERVAL_ANGLE1,WINDOW_ANGLE0,90,ROLL_FONT0,SELECT_ROLL_FONT0,0,0,0,0,0},
	{0,0,0,0,0,VERTICAL,SHADOW_COLOR_R1,SHADOW_COLOR_G1,SHADOW_COLOR_B1,TRACK_TOP_LEFT_X1,TRACK_TOP_LEFT_Y1,V200_HDL,TRACK_LEN3,TRACK_HIGH2,TRACKTAG,SCALEFACTOR0,INTERVAL_ANGLE0,WINDOW_ANGLE0,90,ROLL_FONT0,SELECT_ROLL_FONT0,0,0,0,0,0},
	{0,0,0,0,0,VERTICAL,SHADOW_COLOR_R2,SHADOW_COLOR_G2,SHADOW_COLOR_B2,TRACK_TOP_LEFT_X1,TRACK_TOP_LEFT_Y1,V200_HDL,TRACK_LEN3,TRACK_HIGH2,TRACKTAG,SCALEFACTOR1,INTERVAL_ANGLE3,WINDOW_ANGLE0,90,ROLL_FONT0,SELECT_ROLL_FONT0,0,0,0,0,0}
};

typedef struct context_ {
	char *str;
	float str_angle;
}context;

#define DIAL_CONTEX_NUM_1 36
context context_data1[DIAL_CONTEX_NUM_1] = {
	{"A1",0},
	{"B1",0},
	{"C1",0},
	{"FTDI",0},
	{"E",0},
	{"F",0},
	{"G",0},
	{"H",0},
	{"I",0},
	{"J",0},
	{"K",0},
	{"L",0},
	{"M",0},
	{"N",0},
	{"O",0},
	{"P",0},
	{"Q",0},
	{"R",0},
	{"S",0},
	{"T",0},
	{"U",0},
	{"V",0},
	{"W",0},
	{"X",0},
	{"Y",0},
	{"Z",0},
	{"0",0},
	{"1",0},
	{"2",0},
	{"3",0},
	{"4",0},
	{"5",0},
	{"6",0},
	{"7",0},
	{"8",0},
	{"9",0}
};

#define DISPLAY_NUM 20    //in order suitable different parameter, define Large enough array size, actually you can define as below
//#define DISPLAY_NUM ((((WINDOW_ANGLE)/2)/(INTERVAL_ANGLE))*2)+1
typedef struct display_context_ {
	FT32 context_index;
	context dis_context;
	FT32 display_x;
	FT32 display_y;
	FTU8 isOnMiddle;
}display_context;
display_context arrayDisplay[DISPLAY_NUM];
 
#define DIAL_CONTEX_NUM_2 12

context context_data2[DIAL_CONTEX_NUM_2] = {
	{"January",0},
	{"February",0},
	{"March",0},
	{"April",0},
	{"May",0},
	{"June",0},
	{"July",0},
	{"August",0},
	{"September",0},
	{"October",0},
	{"November",0},
	{"December",0}
};

FTVOID ScrollerInit0(Ft_App_Scroller_t *pScroll,Ft_App_Scroller_t *initScroll,display_context *array_Display,FTU8 *display_num,context *context_data)
{
	float start_angle;
	FTU8 i=0,len;
	*pScroll = *initScroll;
	pScroll->window_middle_x = pScroll->left_top_x + pScroll->len/2;   
	pScroll->window_middle_y = pScroll->left_top_y + pScroll->high/2;  
	pScroll->window_end_angle = pScroll->window_middle_angle + pScroll->window_angle/2;   //min start angle
	pScroll->window_start_angle = pScroll->window_middle_angle - pScroll->window_angle/2; //max end angle
	if(pScroll->verticalORhorizontal == HORIZONTAL)
	    pScroll->r = pScroll->len/2 * sin((float)((pScroll->window_middle_angle-pScroll->window_start_angle)*PI/180));  //the track map the arc radius
	else
		pScroll->r = pScroll->high/2 * sin((float)((pScroll->window_middle_angle-pScroll->window_start_angle)*PI/180));  //the track map the arc radius
	start_angle = (float)(pScroll->window_middle_angle - ((FTU8)(pScroll->window_middle_angle / pScroll->interval_angle))*pScroll->interval_angle);  //init first roll string angle 
	len = ((FTU8)((pScroll->window_angle/2)/pScroll->interval_angle))*2+1;
	//len = (((pScroll->window_angle/2)/pScroll->interval_angle))*2+1;
	*display_num = len; 
	if(*display_num > DISPLAY_NUM)
	{
		printf("error: DISPLAY_NUM is too small \n");
		while(1){};
	}
	for(i=0;i<len;i++){
		(array_Display+i)->context_index = i;
		(array_Display+i)->dis_context.str = (context_data+i)->str;
		(array_Display+i)->dis_context.str_angle = start_angle + pScroll->interval_angle*i;
	}
}

appRet_en dialBmpToRamG(FTU32 bmpHdl, FTU32 ramgAddr, BMP_para *pbmpHD,FTU32 width,FTU32 high)
{
	FTU32 i, src;
	src = ramgAddr;
	if (bmpHdl >= FT800_BMP_EXT_HANDLE) {
		DBGPRINT;
		return APP_ERR_HDL_EXC;
	}
	pbmpHD[0].bmpHD.len = appFileToRamG(pbmpHD[0].bmpHD.path,src,1,0,0);
	if (pbmpHD[0].bmpHD.len == 0) {
		DBGPRINT;
		return APP_ERR_LEN;
	}
    HAL_CmdBufIn(BITMAP_HANDLE(bmpHdl));
	HAL_CmdBufIn(BITMAP_SOURCE(src));
	/* only know the listed format's linestride, others need to find out my ourself */
	if (ARGB2 == pbmpHD[0].bmpHD.format || RGB332 == pbmpHD[0].bmpHD.format || PALETTED == pbmpHD[0].bmpHD.format || L8 == pbmpHD[0].bmpHD.format) {
		HAL_CmdBufIn(BITMAP_LAYOUT(pbmpHD[0].bmpHD.format,pbmpHD[0].bmpHD.wide,pbmpHD[0].bmpHD.high));
	} else {
		HAL_CmdBufIn(BITMAP_LAYOUT(pbmpHD[0].bmpHD.format,pbmpHD[0].bmpHD.wide*2,pbmpHD[0].bmpHD.high));
	}
		/* don't know the different between NEAREST and BILINEAR, here just use NEAREST */
	//HAL_CmdBufIn(BITMAP_SIZE(pbmpHD[0].filter,pbmpHD[0].wrapx,pbmpHD[0].wrapy,pbmpHD[0].bmpHD.wide,high));
    HAL_CmdBufIn(BITMAP_SIZE(pbmpHD[0].filter,pbmpHD[0].wrapx,pbmpHD[0].wrapy,width,high));
	return APP_OK;
}

FTINDEF FTVOID downBKBitmap(Ft_App_Scroller_t *pScroll)
{
	static FTU8 ifLoaded = 0;
	FTU8 i, j;
	FTU32 addr=0;
	    addr = RAM_G;
	//if (ifLoaded == 0) {
		/* load bitmap resources data into FT800 */
		if(APP_OK != dialBmpToRamG(H400_HDL, addr, &bmpSetting[0],pScroll->len,pScroll->high)){
			DBGPRINT;
			return;
		}
		addr = addr + 1024;
		if(APP_OK != dialBmpToRamG(H200_HDL, addr, &bmpSetting[1],pScroll->len,pScroll->high)){
			DBGPRINT;
			return;
		}
		addr = addr + 1024;
		if(APP_OK != dialBmpToRamG(V200_HDL, addr, &bmpSetting[2],pScroll->len,pScroll->high)){
			DBGPRINT;
			return;
		}
		addr = addr + 1024;
		if(APP_OK != dialBmpToRamG(V400_HDL, addr, &bmpSetting[3],pScroll->len,pScroll->high)){
			DBGPRINT;
			return;
		}
		addr = addr + 1024;
		if(APP_OK != dialBmpToRamG(NEXT_HDL, addr, &bmpSetting[4],NEXT_WIDTH,NEXT_HIGH)){
			DBGPRINT;
			return;
		}
	//	ifLoaded = 1;	    	
	//}
}

FTVOID drawContext(Ft_App_Scroller_t *pScroll,display_context *array_Display,FTU8 len)
{
	FTU8 i=0;
	FTU32 display_x=0,display_y=0,display_y_roll=0,temp,display_y_sel=0;
	if(pScroll->verticalORhorizontal == HORIZONTAL){
		for(i=0;i<len;i++){
			(array_Display+i)->display_y = pScroll->window_middle_y;
			if((array_Display+i)->dis_context.str_angle <= pScroll->window_end_angle && (array_Display+i)->dis_context.str_angle >= pScroll->window_start_angle){
				if((array_Display+i)->dis_context.str_angle < pScroll->window_middle_angle)
					display_x = pScroll->window_middle_x - (FTU32)(sin((pScroll->window_middle_angle - (array_Display+i)->dis_context.str_angle)*PI/180) * pScroll->r);
				else
					display_x = pScroll->window_middle_x + (FTU32)(sin(((array_Display+i)->dis_context.str_angle - pScroll->window_middle_angle)*PI/180) * pScroll->r);
				(array_Display+i)->display_x = display_x;
				//if((array_Display+i)->dis_context.str_angle > pScroll->window_middle_angle - pScroll->interval_angle/2 && (array_Display+i)->dis_context.str_angle < pScroll->window_middle_angle + pScroll->interval_angle/2){
				if((array_Display+i)->dis_context.str_angle > pScroll->window_middle_angle - 10 && (array_Display+i)->dis_context.str_angle < pScroll->window_middle_angle + 10)
					(array_Display+i)->isOnMiddle = 1;
				else
					(array_Display+i)->isOnMiddle = 0;
			}
		}
	}
	if(pScroll->verticalORhorizontal == VERTICAL){
		for(i=0;i<len;i++){
			(array_Display+i)->display_x = pScroll->window_middle_x;
			if((array_Display+i)->dis_context.str_angle <= pScroll->window_end_angle && (array_Display+i)->dis_context.str_angle >= pScroll->window_start_angle){
				if((array_Display+i)->dis_context.str_angle < pScroll->window_middle_angle)
					display_y = pScroll->window_middle_y - (FTU32)(sin((pScroll->window_middle_angle - (array_Display+i)->dis_context.str_angle)*PI/180) * pScroll->r);				
				else
					display_y = pScroll->window_middle_y + (FTU32)(sin(((array_Display+i)->dis_context.str_angle - pScroll->window_middle_angle)*PI/180) * pScroll->r);
				(array_Display+i)->display_y = display_y;
				//if((array_Display+i)->dis_context.str_angle > pScroll->window_middle_angle - pScroll->interval_angle/2 && (array_Display+i)->dis_context.str_angle < pScroll->window_middle_angle + pScroll->interval_angle/2){
				if((array_Display+i)->dis_context.str_angle > pScroll->window_middle_angle - 10 && (array_Display+i)->dis_context.str_angle < pScroll->window_middle_angle + 10)
					(array_Display+i)->isOnMiddle = 1;				
				else
					(array_Display+i)->isOnMiddle = 0;			
			}
		}
	}
//	modifyDisplay_xy(pScroll,&array_Display[0],len);
	for(i=0;i<len;i++){
		if((array_Display+i)->isOnMiddle==1){
			HAL_CmdBufIn(COLOR_MASK(1,1,1,1));
			HAL_CmdBufIn(COLOR_RGB(255,0,0));  
			CoCmd_TEXT((array_Display+i)->display_x,(array_Display+i)->display_y,pScroll->sel_roll_font,OPT_CENTER,(FT8 *)(array_Display+i)->dis_context.str);
			HAL_CmdBufIn(COLOR_MASK(0,0,0,0));
		}
		else{
			HAL_CmdBufIn(COLOR_MASK(1,1,1,1));
			HAL_CmdBufIn(COLOR_RGB(64,64,64));
			CoCmd_TEXT((array_Display+i)->display_x,(array_Display+i)->display_y,pScroll->roll_font,OPT_CENTER,(FT8 *)(array_Display+i)->dis_context.str);
			HAL_CmdBufIn(COLOR_MASK(0,0,0,0));
		}
	}
}

FTVOID drawRectangle(Ft_App_Scroller_t *pScroll)
{
	//roll Rectangle
	HAL_CmdBufIn(COLOR_MASK(1,1,1,1));
	HAL_CmdBufIn(COLOR_RGB(64,64,64));
	HAL_CmdBufIn(LINE_WIDTH(1*16));
	HAL_CmdBufIn(BEGIN(RECTS));
	HAL_CmdBufIn(STENCIL_OP(KEEP,INCR));
	HAL_CmdBufIn(CLEAR(0,1,0));
	HAL_CmdBufIn(TAG_MASK(1));
	HAL_CmdBufIn(TAG(TRACKTAG));
	HAL_CmdBufIn(TAG_MASK(0));
	HAL_CmdBufIn(COLOR_RGB(223,223,223));
	HAL_CmdBufIn(VERTEX2F((pScroll->left_top_x+1)*16,(pScroll->left_top_y+1)*16));
	HAL_CmdBufIn(VERTEX2F((pScroll->left_top_x + pScroll->len-2)*16,(pScroll->left_top_y + pScroll->high-2)*16));
	HAL_CmdBufIn(COLOR_RGB(0,0,0));
	HAL_CmdBufIn(COLOR_A(255));
	HAL_CmdBufIn(STENCIL_FUNC(EQUAL,1,255));
	HAL_CmdBufIn(STENCIL_OP(KEEP,KEEP));	
}

FTVOID drawNext()
{
	HAL_CmdBufIn(COLOR_MASK(1,1,1,1));
	HAL_CmdBufIn(COLOR_RGB(255,255,255));
	HAL_CmdBufIn(COLOR_A(255));
	HAL_CmdBufIn(BEGIN(BITMAPS));
	HAL_CmdBufIn(TAG_MASK(1));
	HAL_CmdBufIn(TAG(NEXTTAG));
	HAL_CmdBufIn(VERTEX2II(NEXT_TOP_LEFT_X,NEXT_TOP_LEFT_Y,NEXT_HDL,0));
	HAL_CmdBufIn(TAG_MASK(0));	
}

FTVOID drawShadow(Ft_App_Scroller_t *pScroll)
{
	//roll shadow
	HAL_CmdBufIn(COLOR_MASK(1,1,1,1));
	HAL_CmdBufIn(STENCIL_FUNC(ALWAYS,0,255));
	HAL_CmdBufIn(COLOR_RGB(xScroll.rgb_r,xScroll.rgb_g,xScroll.rgb_b));
	HAL_CmdBufIn(COLOR_A(255));
	HAL_CmdBufIn(BEGIN(BITMAPS));
	HAL_CmdBufIn(TAG_MASK(1));
	HAL_CmdBufIn(VERTEX2II(pScroll->left_top_x,pScroll->left_top_y,pScroll->hdl,0));
	HAL_CmdBufIn(TAG_MASK(0));	
}

/* API to moniter and track the scroller - change touch move distance to angle */
FTVOID ScrollerRun(Ft_App_Scroller_t *pScroll)
{
	FT32 change_len;
	FT16 currtrack;
	FTU8 TagValue;
	FTU32 Touch_XY;
	TagValue = HAL_Read8(REG_TOUCH_TAG);
	Touch_XY = HAL_Read32(REG_TOUCH_TAG_XY);
	if(pScroll->verticalORhorizontal == HORIZONTAL){
        currtrack = (FT16)(Touch_XY>>16);
	}
	else{
		currtrack = (FT16)(Touch_XY);
	}
	/* only get the x value of the track as only linear movement is been tracked */
	if((TagValue == pScroll->TagValue) && (pScroll->TagValue == pScroll->PrevTagValue) && (-32768 != currtrack) && (-32768 != pScroll->trackprev))
	{
		/* update the velocity value */
		pScroll->roll_arc_len = (FT32)((pScroll->trackprev - currtrack)<<4);//making 1/16ths pixel precision
	}
	else
	{
		change_len = max(1, abs(pScroll->roll_arc_len) >> 5);
		if (pScroll->roll_arc_len < 0)
		{
			pScroll->roll_arc_len += change_len;
		}
		if (pScroll->roll_arc_len > 0)
		{
			pScroll->roll_arc_len -= change_len;
		}
	}
	pScroll->roll_angle = -(float)(pScroll->roll_arc_len * 360) / ((float)(pScroll->r * 2 * PI)*pScroll->ScaleFactor);
	pScroll->PrevTagValue = TagValue;
	pScroll->trackprev = currtrack;
}
/* API to moniter and track the scroller - change touch move distance to angle */
FTVOID ScrollerRun1(Ft_App_Scroller_t *pScroll)
{
	FT32 change_len;
	FT16 currtrack;
	FTU8 TagValue;
	FTU32 Touch_XY;
	TagValue = HAL_Read8(REG_TOUCH_TAG);
	Touch_XY = HAL_Read32(REG_TOUCH_TAG_XY);

	/* only get the x value of the track as only linear movement is been tracked */
	currtrack = (FT16)(Touch_XY>>16);
	if((TagValue == pScroll->TagValue) && (pScroll->TagValue == pScroll->PrevTagValue) && (-32768 != currtrack) && (-32768 != pScroll->trackprev))
	{
		/* update the velocity value */
		pScroll->roll_arc_len = (FT32)((pScroll->trackprev - currtrack)<<4);//making 1/16ths pixel precision
	}
	else
	{
		change_len = max(1, abs(pScroll->roll_arc_len) >> 5);
		if (pScroll->roll_arc_len < 0)
		{
			pScroll->roll_arc_len += change_len;
		}
		if (pScroll->roll_arc_len > 0)
		{
			pScroll->roll_arc_len -= change_len;
		}
	}
	pScroll->roll_angle = -(float)(pScroll->roll_arc_len * 360) / ((float)(pScroll->r * 2 * PI)*pScroll->ScaleFactor);
	pScroll->PrevTagValue = TagValue;
	pScroll->trackprev = currtrack;
}

FTVOID displayMove(Ft_App_Scroller_t *pScroll,display_context *array_Display,FTU8 len,context *context_data,FTU8 context_len)
{
	FTU8 i=0,slen=0;
	for(i=0;i<len;i++){
		(array_Display+i)->dis_context.str_angle = (array_Display+i)->dis_context.str_angle + pScroll->roll_angle;
	}

	//while(1){
	if(pScroll->roll_angle>0.00001){
		if((array_Display+0)->dis_context.str_angle - pScroll->interval_angle > pScroll->window_start_angle ){
			for(i=len-1;i>0;i--){
				*(array_Display+i) = *(array_Display+i-1);
			}
			if((array_Display+1)->context_index==0){
				(array_Display+0)->context_index = context_len-1;
			}else{
                (array_Display+0)->context_index = (array_Display+1)->context_index - 1;
			}
			(array_Display+0)->dis_context.str = (context_data + (array_Display+0)->context_index) ->str;
			(array_Display+0)->dis_context.str_angle = (array_Display+1)->dis_context.str_angle - pScroll->interval_angle;
	    }
	}
	if(pScroll->roll_angle<-0.00001){
		if((array_Display+len-1)->dis_context.str_angle + pScroll->interval_angle < pScroll->window_end_angle){
			for(i=0;i<len-1;i++){
				*(array_Display+i) = *(array_Display+i+1);
			}
			if((array_Display+len-2)->context_index==context_len-1){
				(array_Display+len-1)->context_index = 0;
			}else{
				(array_Display+len-1)->context_index = (array_Display+len-2)->context_index + 1;
			}
			(array_Display+len-1)->dis_context.str = (context_data +(array_Display+len-1)->context_index)->str;
			(array_Display+len-1)->dis_context.str_angle = (array_Display+len-2)->dis_context.str_angle + pScroll->interval_angle;
	    }
	}
}

FTVOID Scroller_show (FTU32 para)
{
	FTU8 diaplay_array_len=0;
	ScrollerInit0(&xScroll,&aScroll1[gIndex],&arrayDisplay[0],&diaplay_array_len,&context_data1[0]);
	while (NEXTTAG != HAL_Read8(REG_TOUCH_TAG)) {
		HAL_CmdBufIn(CMD_DLSTART);
		downBKBitmap(&xScroll);
		HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
		HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
		HAL_CmdBufIn(CLEAR(1,1,1));
		HAL_CmdBufIn(CLEAR_COLOR_RGB(64,64,64));
		BKGND_BLOB_GRAD_BKG;		
		drawNext();
	    drawRectangle(&xScroll);
		drawContext(&xScroll,&arrayDisplay[0],diaplay_array_len);
		ScrollerRun(&xScroll);
		displayMove(&xScroll,&arrayDisplay[0],diaplay_array_len,&context_data1[0],DIAL_CONTEX_NUM_1);
		drawShadow(&xScroll);
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
	}
	while (NEXTTAG==HAL_Read8(REG_TOUCH_TAG)) {
		/* hold till release */
	}
	gIndex++;
	if(gIndex>SCROLL_NUM1-1){
	    appGP.appIndex++;
		gIndex = 0;
	}
	return;
}

FTVOID Scroller_show1 (FTU32 para)
{
	FTU8 diaplay_array_len=0;
	ScrollerInit0(&xScroll,&aScroll2[gIndex],&arrayDisplay[0],&diaplay_array_len,&context_data2[0]);
	while (NEXTTAG != HAL_Read8(REG_TOUCH_TAG)) {
		HAL_CmdBufIn(CMD_DLSTART);
		downBKBitmap(&xScroll);
		HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
		HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
		HAL_CmdBufIn(CLEAR(1,1,1));
		HAL_CmdBufIn(CLEAR_COLOR_RGB(64,64,64));
		BKGND_BLOB_GRAD_BKG;		
		drawNext();
	    drawRectangle(&xScroll);	
		ScrollerRun(&xScroll);
		displayMove(&xScroll,&arrayDisplay[0],diaplay_array_len,&context_data2[0],DIAL_CONTEX_NUM_2);
		drawContext(&xScroll,&arrayDisplay[0],diaplay_array_len);
		drawShadow(&xScroll);
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
	}
	while (NEXTTAG==HAL_Read8(REG_TOUCH_TAG)) {
		/* hold till release */
	}
	gIndex++;
	if(gIndex>SCROLL_NUM2-1){
	    appGP.appIndex++;
		gIndex = 0;
	}
	return;
}

FTVOID Scroller_show2 (FTU32 para)
{
	FTU8 diaplay_array_len=0;
	ScrollerInit0(&xScroll,&aScroll3[gIndex],&arrayDisplay[0],&diaplay_array_len,&context_data2[0]);
	while (NEXTTAG != HAL_Read8(REG_TOUCH_TAG)) {
		HAL_CmdBufIn(CMD_DLSTART);
		downBKBitmap(&xScroll);
		HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
		HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
		HAL_CmdBufIn(CLEAR(1,1,1));
		HAL_CmdBufIn(CLEAR_COLOR_RGB(64,64,64));
		BKGND_BLOB_GRAD_BKG;		
		drawNext();
	    drawRectangle(&xScroll);	
		ScrollerRun(&xScroll);
		displayMove(&xScroll,&arrayDisplay[0],diaplay_array_len,&context_data2[0],DIAL_CONTEX_NUM_2);
		drawContext(&xScroll,&arrayDisplay[0],diaplay_array_len);
		drawShadow(&xScroll);
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
	}
	while (NEXTTAG==HAL_Read8(REG_TOUCH_TAG)) {
		/* hold till release */
	}
	gIndex++;
	if(gIndex>SCROLL_NUM3-1){
	    appGP.appIndex++;
		gIndex = 0;
	}
	return;
}

FTVOID Scroller_show3 (FTU32 para)
{
	FTU8 diaplay_array_len=0;
	ScrollerInit0(&xScroll,&aScroll4[gIndex],&arrayDisplay[0],&diaplay_array_len,&context_data1[0]);
	while (NEXTTAG != HAL_Read8(REG_TOUCH_TAG)) {
		HAL_CmdBufIn(CMD_DLSTART);
		downBKBitmap(&xScroll);
		HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
		HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
		HAL_CmdBufIn(CLEAR(1,1,1));
		HAL_CmdBufIn(CLEAR_COLOR_RGB(64,64,64));
		BKGND_BLOB_GRAD_BKG;		
		drawNext();
	    drawRectangle(&xScroll);	
		ScrollerRun(&xScroll);
		displayMove(&xScroll,&arrayDisplay[0],diaplay_array_len,&context_data1[0],DIAL_CONTEX_NUM_1);
		drawContext(&xScroll,&arrayDisplay[0],diaplay_array_len);
		drawShadow(&xScroll);
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
	}
	while (NEXTTAG==HAL_Read8(REG_TOUCH_TAG)) {
		/* hold till release */
	}
	gIndex++;
	if(gIndex>SCROLL_NUM4-1){
	    appGP.appIndex=0;
		gIndex = 0;
	}
	return;
}

/*
 * actually, all background_x is able to be put in one routine and change by each touch
 * but here, just show you how to use Apps to switch between each sub routine
 * so, we seperate into some sub routine
 */
AppFunc Apps[] = {
	Scroller_show,
	Scroller_show1,
	Scroller_show2,
	Scroller_show3,
	/* Leave this NULL at the buttom of this array */
	NULL
};


#endif

