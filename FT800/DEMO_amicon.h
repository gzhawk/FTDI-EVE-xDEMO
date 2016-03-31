/* 
    Demo created by Born:
    Author: Born Jiang
	Email : born.jiang@ftdichip.com
	Date  : 2014/Feb
*/
#ifdef DEMO_AMICON
#include <math.h>

typedef struct app_para_st {
	FTU32 appIndex;
	FTU32 appPara;
}app_para_t;

app_para_t appGP = {0};

typedef struct halo_para_st {
	FTU32 r;
	FT32 linewidth;
	FTU32 start_angle;
	FTU32 end_angle;
	FTU32 rgb_r;
	FTU32 rgb_g;
	FTU32 rgb_b;
}halo_para_t;

halo_para_t haloPara[18] = {
	{55,2,45,135,128,0,0},
	{75,2,45,135,0,128,0},
	{95,2,45,135,0,0,128},

	{65,2,0,120,128,0,0},
	{65,2,120,240,0,128,0},
	{65,2,240,360,0,0,128},

	{55,2,45,135,128,0,0},
	{55,2,225,315,128,0,0},
	{75,2,45,135,0,128,0},
	{75,2,225,315,128,0,0},
	{95,2,45,135,0,0,128},
	{95,2,225,315,128,0,0},

	{64,5,0,360,220,220,220},
	{71,2,0,360,255,255,255},
	{77,5,0,360,220,220,220},
	{83,2,0,360,25,255,255},
	{89,5,0,360,220,220,20},
	{96,2,0,360,255,25,255},
};


//FTU8 count=0;
//FTU8 longtouchTag=0;

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
#define PATH_LEN 50
#define PATH_AM_ICON ROOT_PATH"ami\\g\\gd0"
#define PATH_FIRE_F1 ROOT_PATH"ami\\fire\\f1.raw"
#define PATH_FIRE_F2 ROOT_PATH"ami\\fire\\f2.raw"
#define PATH_FIRE_J1 ROOT_PATH"ami\\fire\\j1.raw"
#define PATH_FIRE_J2 ROOT_PATH"ami\\fire\\j2.raw"
#define PATH_FIRE_P1 ROOT_PATH"ami\\fire\\p1.raw"
#define PATH_FIRE_P2 ROOT_PATH"ami\\fire\\p2.raw"
#define PATH_FIRE_Q1 ROOT_PATH"ami\\fire\\q1.raw"
#define PATH_FIRE_Q2 ROOT_PATH"ami\\fire\\q2.raw"

#else
#define PATH_LEN 10
#define PATH_AM_ICON ROOT_PATH"gd0"
#define PATH_FIRE_F1 ROOT_PATH"f1.raw"
#define PATH_FIRE_F2 ROOT_PATH"f2.raw"
#define PATH_FIRE_J1 ROOT_PATH"j1.raw"
#define PATH_FIRE_J2 ROOT_PATH"j2.raw"
#define PATH_FIRE_P1 ROOT_PATH"p1.raw"
#define PATH_FIRE_P2 ROOT_PATH"p2.raw"
#define PATH_FIRE_Q1 ROOT_PATH"q1.raw"
#define PATH_FIRE_Q2 ROOT_PATH"q2.raw"
#endif

//#define BKGND_BNUM 64 /* do not use too much number, or Arduino would run out of memory */
#define BKGND_TLT_FONT 25
#define BKGND_TLT_LEN 20
//#define BKGND_VALUE_1 (BKGND_BNUM/4)
//#define BKGND_VALUE_2 (BKGND_BNUM/2)

#define BKGND_BLOB_MSPEED 1
#define BKGND_BLOB_GRAD_BKG CoCmd_GRADIENT(0,0,0xCBDEB6,0,FT800_LCD_HIGH,0)

#define ft_random(x)		(rand() % (x))
#define DIR_MAX 6

#define PI 3.141592
#define R 20
#define ZX 0
#define ZY 0


typedef enum icon_grp {
	AMICON_HDL_FIRST = 0,
	AMICON_HDL_SECOND,
	/* leave it at the buttom */
	BKGND_ICON_GROUP
} icon_grp_e;

bmpHDR_st bmpHD[BKGND_ICON_GROUP] = {
	{PATH_AM_ICON,	0,0,ARGB4,	0,0,	140,	140},
};

typedef enum fire_icon_grp {
	FIRE_HDL_F1 = 0,
	FIRE_HDL_F2,
	FIRE_HDL_J1,
	FIRE_HDL_J2,
	FIRE_HDL_P1,
	FIRE_HDL_P2,
	FIRE_HDL_Q1,
	FIRE_HDL_Q2,

	/* leave it at the buttom */
	FIRE_ICON_GROUP
} fire_icon_grp_e;


bmpHDR_st FireBmpHD[FIRE_ICON_GROUP] = {
	{PATH_FIRE_F1,  0,0,ARGB4,  0,0,  128,    128},
	{PATH_FIRE_F2,  0,0,ARGB4,  0,0,  128,    128},
	{PATH_FIRE_J1,  0,0,ARGB4,  0,0,  128,    128},
	{PATH_FIRE_J2,  0,0,ARGB4,  0,0,  128,    128},
	{PATH_FIRE_P1,  0,0,ARGB4,  0,0,  128,    128},
	{PATH_FIRE_P2,  0,0,ARGB4,  0,0,  128,    128},
	{PATH_FIRE_Q1,  0,0,ARGB4,  0,0,  128,    128},
	{PATH_FIRE_Q2,  0,0,ARGB4,  0,0,  128,    128}
};
bmpHDR_st temp_bmpHD[1];


typedef struct coor_ {
	FT16 x, y;
	FT16 xOffset, yOffset;
} coor_st;

coor_st Fire_Coor[FIRE_ICON_GROUP] = {
	{5,80,0,0},
	{5,80,0,0},
	{120,80,0,0},
	{120,80,0,0},
	{235,80,0,0},
	{235,80,0,0},
	{350,80,0,0},
	{350,80,0,0}
};


#define BKGND_TLT_FONT 25
#define BKGND_TLT_LEN 20

struct {
	//operation_e op;
	FTU8 tlt[BKGND_TLT_LEN];
} ctrl_a[DIR_MAX] = {
	{"fire ICON"},
	{"rotate ICON"},
	{"tremble ICON"},
	{"dynamic ICON"},
	{"halo ICON"},
	{"Picture overly"},
};

FTU8 gIndex = 0;
//FTC8 filePath1[PATH_LEN]={0};
FTU8 filePath[PATH_LEN]={0};

appRet_en BmpLayout(FTU32 bmpHdl, FTU32 ramgAddr, bmpHDR_st *pbmpHD)
{
	//FTU32 i, src;

	HAL_DlpBufIn(BITMAP_HANDLE(bmpHdl));
	HAL_DlpBufIn(BITMAP_SOURCE(ramgAddr));
		/* only know the listed format's linestride, others need to find out my ourself */
	if (ARGB2 == pbmpHD[0].format || RGB332 == pbmpHD[0].format || PALETTED == pbmpHD[0].format) {
		HAL_DlpBufIn(BITMAP_LAYOUT(pbmpHD[0].format,pbmpHD[0].wide,pbmpHD[0].high));
	} else {
		HAL_DlpBufIn(BITMAP_LAYOUT(pbmpHD[0].format,pbmpHD[0].wide*2,pbmpHD[0].high));
	}
		/* don't know the different between NEAREST and BILINEAR, here just use NEAREST */
	HAL_DlpBufIn(BITMAP_SIZE(NEAREST,BORDER,BORDER,pbmpHD[0].wide,pbmpHD[0].high));

	HAL_DlpBufIn(DISPLAY());
	HAL_BufToReg(RAM_DL,0);

	return APP_OK;
}

FTVOID getCircleXYany(FT32 zx,FT32 zy,halo_para_t *para)
{
	double rxs=0,rys=0,rxe=0,rye=0; 
	//int r = 100;
	double temp_angle = 0;
	rxe = para[0].r * 16;
	rye = 0;
	rxs = para[0].r * 16;
	rys = 0;
//	if(r<0 || r>MAX_R || (end_angle - start_angle)>360){
//		return;
//	}
	temp_angle =para[0].start_angle;
	//HAL_CmdBufIn(CMD_DLSTART);
	//HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
	//HAL_CmdBufIn(CLEAR(1,1,1));
	//BKGND_BLOB_GRAD_BKG;
    rxs = para[0].r * 16 * cos(2*PI*temp_angle/360);
	rys = para[0].r * 16 * sin(2*PI*temp_angle/360);
	HAL_CmdBufIn(COLOR_MASK(1,1,1,1));
	HAL_CmdBufIn(COLOR_RGB(para[0].rgb_r, para[0].rgb_g, para[0].rgb_b));
    HAL_CmdBufIn(LINE_WIDTH(para[0].linewidth * 16));
	HAL_CmdBufIn(BEGIN(LINES));
	while(1){
	    //printf("rx = %f ry = %f \n",rxs,rys);
	    temp_angle = temp_angle + 5;
	    if(temp_angle>para[0].end_angle) break;
	    rxe = para[0].r * 16 * cos(2*PI*temp_angle/360);
	    rye = para[0].r * 16 * sin(2*PI*temp_angle/360);	
		HAL_CmdBufIn(VERTEX2F(zx*16 + (int)rxs,(zy*16 - (int)rys)));
		HAL_CmdBufIn(VERTEX2F(zx*16 + (int)rxe,(zy*16 - (int)rye)));
		rxs = rxe;
		rys = rye;
	}
	HAL_CmdBufIn(COLOR_MASK(0,0,0,0));
	
	
/*	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);*/	
}

FTVOID getCircleXYany1(FTU32 r,FT32 zx,FT32 zy,FT32 linewidth,FTU32 start_angle,FTU32 end_angle,FTU32 rgb_r,FTU32 rgb_g,FTU32 rgb_b)
{
	double rxs=0,rys=0,rxe=0,rye=0; 
	//int r = 100;
	double temp_angle = 0;
	rxe = r*16;
	rye = 0;
	rxs = r*16;
	rys = 0;
//	if(r<0 || r>MAX_R || (end_angle - start_angle)>360){
//		return;
//	}
	temp_angle = start_angle;
	//HAL_CmdBufIn(CMD_DLSTART);
	//HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
	//HAL_CmdBufIn(CLEAR(1,1,1));
	//BKGND_BLOB_GRAD_BKG;
    rxs = r * 16 * cos(2*PI*temp_angle/360);
	rys = r * 16 * sin(2*PI*temp_angle/360);
	HAL_CmdBufIn(COLOR_MASK(1,1,1,1));
	HAL_CmdBufIn(COLOR_RGB(rgb_r, rgb_g, rgb_b));
    HAL_CmdBufIn(LINE_WIDTH(linewidth * 16));
	HAL_CmdBufIn(BEGIN(LINES));
	while(1){
	    //printf("rx = %f ry = %f \n",rxs,rys);
	    temp_angle = temp_angle + 3;
	    if(temp_angle>end_angle) break;
	    rxe = r * 16 * cos(2*PI*temp_angle/360);
	    rye = r * 16 * sin(2*PI*temp_angle/360);	
		HAL_CmdBufIn(VERTEX2F(zx*16 + (int)rxs,(zy*16 - (int)rys)));
		HAL_CmdBufIn(VERTEX2F(zx*16 + (int)rxe,(zy*16 - (int)rye)));
		rxs = rxe;
		rys = rye;
	}
	HAL_CmdBufIn(COLOR_MASK(0,0,0,0));
	
	
/*	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);*/	
}

FTVOID draw_halo(FT32 zx,FT32 zy,FTU8 type,FTU8 timetouch){
    FTU32 count=0;
	switch(type){
	case 0:
		{
			if(timetouch<15){
				getCircleXYany(zx,zy,&haloPara[0]);
			}
			else if(timetouch<30){
				getCircleXYany(zx,zy,&haloPara[0]);
				getCircleXYany(zx,zy,&haloPara[1]);
			}
			else if(timetouch<45){
				getCircleXYany(zx,zy,&haloPara[0]);
				getCircleXYany(zx,zy,&haloPara[1]);
				getCircleXYany(zx,zy,&haloPara[2]);
			}
		}
		break;
	case 1:
		{
			if(timetouch<15){
				getCircleXYany(zx,zy,&haloPara[3]);
			}
			else if(timetouch<30){
				getCircleXYany(zx,zy,&haloPara[3]);
				getCircleXYany(zx,zy,&haloPara[4]);
			}
			else if(timetouch<45){
				getCircleXYany(zx,zy,&haloPara[3]);
				getCircleXYany(zx,zy,&haloPara[4]);
				getCircleXYany(zx,zy,&haloPara[5]);
			}
		}
		break;
	case 2:
		{
			if(timetouch<15){
				getCircleXYany(zx,zy,&haloPara[6]);
				getCircleXYany(zx,zy,&haloPara[7]);
			}
			else if(timetouch<30){
				getCircleXYany(zx,zy,&haloPara[6]);
				getCircleXYany(zx,zy,&haloPara[7]);
				getCircleXYany(zx,zy,&haloPara[8]);
				getCircleXYany(zx,zy,&haloPara[9]);
			}
			else if(timetouch<45){
				getCircleXYany(zx,zy,&haloPara[6]);
				getCircleXYany(zx,zy,&haloPara[7]);
				getCircleXYany(zx,zy,&haloPara[8]);
				getCircleXYany(zx,zy,&haloPara[9]);
				getCircleXYany(zx,zy,&haloPara[10]);
				getCircleXYany(zx,zy,&haloPara[11]);
			}
		}
		break;
	case 3:
		{
			if(timetouch<10){
				getCircleXYany(zx,zy,&haloPara[12]);
			}
			else if(timetouch<20){
				getCircleXYany(zx,zy,&haloPara[12]);
				getCircleXYany(zx,zy,&haloPara[13]);
			}
			else if(timetouch<30){
				getCircleXYany(zx,zy,&haloPara[12]);
				getCircleXYany(zx,zy,&haloPara[13]);
				getCircleXYany(zx,zy,&haloPara[14]);
			}
			else if(timetouch<40){
				getCircleXYany(zx,zy,&haloPara[12]);
				getCircleXYany(zx,zy,&haloPara[13]);
				getCircleXYany(zx,zy,&haloPara[14]);
				getCircleXYany(zx,zy,&haloPara[15]);
			}
			else if(timetouch<50){
				getCircleXYany(zx,zy,&haloPara[12]);
				getCircleXYany(zx,zy,&haloPara[13]);
				getCircleXYany(zx,zy,&haloPara[14]);
				getCircleXYany(zx,zy,&haloPara[15]);
				getCircleXYany(zx,zy,&haloPara[16]);
			}
		}
		break;
	default:
		break;
	}

}

FTINDEF FTVOID defaultMatrix (FTVOID)
{
	HAL_CmdBufIn(BITMAP_TRANSFORM_A(256));
	HAL_CmdBufIn(BITMAP_TRANSFORM_B(0));
	HAL_CmdBufIn(BITMAP_TRANSFORM_C(0));
	HAL_CmdBufIn(BITMAP_TRANSFORM_D(0));
	HAL_CmdBufIn(BITMAP_TRANSFORM_E(256));
	HAL_CmdBufIn(BITMAP_TRANSFORM_F(0));
}

FTINDEF FTVOID downFireBitmap()
{
	static FTU8 ifLoaded = 0;
	FTU8 i, j;

	if (ifLoaded == 0) {
		/* load bitmap resources data into FT800 */
		if(APP_OK != appBmpToRamG(FIRE_HDL_F1, RAM_G, &FireBmpHD[0], FIRE_ICON_GROUP)){
			DBGPRINT;
			return;
		}
		ifLoaded = 1;	    	
	}
}

FTINDEF FTVOID downRotateBitmap()
{
	static FTU8 ifLoaded = 0;
	FTU8 i, j;

	if (ifLoaded == 0) {
		/* load bitmap resources data into FT800 */
		if(APP_OK != appBmpToRamG(FIRE_HDL_F1, RAM_G, &FireBmpHD[0], FIRE_ICON_GROUP)){
			DBGPRINT;
			return;
		}
		ifLoaded = 1;	    	
	}
}

FTINDEF FTVOID downHaloBitmap()
{
	static FTU8 ifLoaded = 0;
	FTU8 i, j;

	if (ifLoaded == 0) {
		/* load bitmap resources data into FT800 */
		if(APP_OK != appBmpToRamG(FIRE_HDL_F1, RAM_G, &FireBmpHD[0], FIRE_ICON_GROUP)){
			DBGPRINT;
			return;
		}
		ifLoaded = 1;	    	
	}
}

FTINDEF FTVOID downTrembleBitmap()
{
	static FTU8 ifLoaded = 0;
	FTU8 i, j;

	if (ifLoaded == 0) {
		/* load bitmap resources data into FT800 */
		if(APP_OK != appBmpToRamG(FIRE_HDL_F1, RAM_G, &FireBmpHD[0], FIRE_ICON_GROUP)){
			DBGPRINT;
			return;
		}
		ifLoaded = 1;	    	
	}
}

FTINDEF FTVOID drawFireBitmap()
{
	FTU8 i,t;
	t = HAL_Read8(REG_TOUCH_TAG);
    printf("tag = %d\n",t);
	if(t==0){
		for(i=0;i<FIRE_ICON_GROUP/2;i++){
            HAL_CmdBufIn(TAG_MASK(1));
			HAL_CmdBufIn(BEGIN(BITMAPS));
			HAL_CmdBufIn(TAG(2*i+1));
		    HAL_CmdBufIn(VERTEX2II(Fire_Coor[2*i].x,Fire_Coor[2*i].y,2*i,0));
			HAL_CmdBufIn(TAG_MASK(0));
		}
	}
	else{
		for(i=0;i<FIRE_ICON_GROUP/2;i++){
			if(i == (t-1)/2){
                HAL_CmdBufIn(TAG_MASK(1));
			    HAL_CmdBufIn(BEGIN(BITMAPS));
			    HAL_CmdBufIn(TAG(2*i+1));
		        HAL_CmdBufIn(VERTEX2II(Fire_Coor[2*i+1].x,Fire_Coor[2*i+1].y,2*i+1,0));
			    HAL_CmdBufIn(TAG_MASK(0));
			}
			else{
                HAL_CmdBufIn(TAG_MASK(1));
			    HAL_CmdBufIn(BEGIN(BITMAPS));
			    HAL_CmdBufIn(TAG(2*i+1));
		        HAL_CmdBufIn(VERTEX2II(Fire_Coor[2*i].x,Fire_Coor[2*i].y,2*i,0));
			    HAL_CmdBufIn(TAG_MASK(0));
			}
		}	
	}
	HAL_CmdBufIn(END());
	
}


FTINDEF FTVOID drawHaloBitmap()
{
	FTU8 i,t;
	static FTU8 touch_time=0;
	t = HAL_Read8(REG_TOUCH_TAG);
	if(t==0){
		touch_time = 0;
		for(i=0;i<FIRE_ICON_GROUP/2;i++){
            HAL_CmdBufIn(TAG_MASK(1));
			HAL_CmdBufIn(BEGIN(BITMAPS));
			HAL_CmdBufIn(TAG(2*i+1));
		    HAL_CmdBufIn(VERTEX2II(Fire_Coor[2*i].x,Fire_Coor[2*i].y,2*i,0));
			HAL_CmdBufIn(TAG_MASK(0));
		}
	}
	else{
		for(i=0;i<FIRE_ICON_GROUP/2;i++){
			if(i == (t-1)/2){
				draw_halo(Fire_Coor[2*i].x+FireBmpHD[2*i].wide/2,Fire_Coor[2*i].y+FireBmpHD[2*i].high/2,i,touch_time);
			}
			HAL_CmdBufIn(COLOR_MASK(1,1,1,1));
			HAL_CmdBufIn(COLOR_RGB(255,255,255));
            HAL_CmdBufIn(TAG_MASK(1));
			HAL_CmdBufIn(BEGIN(BITMAPS));
			HAL_CmdBufIn(TAG(2*i+1));
		    HAL_CmdBufIn(VERTEX2II(Fire_Coor[2*i].x,Fire_Coor[2*i].y,2*i,0));
			HAL_CmdBufIn(TAG_MASK(0));
		}
		touch_time++;
		if(touch_time>60)
			touch_time = 0;
		//printf("touch_time = %d\n",touch_time);
	}
	HAL_CmdBufIn(END());
	
}

FTINDEF FTVOID trembleBitmap()
{
	FTU8 i,t;
	t = HAL_Read8(REG_TOUCH_TAG);
    printf("tag = %d\n",t);
	if(t==0){
		for(i=0;i<FIRE_ICON_GROUP/2;i++){
            HAL_CmdBufIn(TAG_MASK(1));
			HAL_CmdBufIn(BEGIN(BITMAPS));
			HAL_CmdBufIn(TAG(2*i+1));
		    HAL_CmdBufIn(VERTEX2II(Fire_Coor[2*i].x,Fire_Coor[2*i].y,2*i,0));
			HAL_CmdBufIn(TAG_MASK(0));
		}
	}
	else{
		for(i=0;i<FIRE_ICON_GROUP/2;i++){
			if(i == (t-1)/2){
                HAL_CmdBufIn(TAG_MASK(1));
			    HAL_CmdBufIn(BEGIN(BITMAPS));
			    HAL_CmdBufIn(TAG(2*i+1));
		        HAL_CmdBufIn(VERTEX2II(Fire_Coor[2*i+1].x + ft_random(5),Fire_Coor[2*i+1].y + ft_random(10),2*i,0));
			    HAL_CmdBufIn(TAG_MASK(0));
			}
			else{
                HAL_CmdBufIn(TAG_MASK(1));
			    HAL_CmdBufIn(BEGIN(BITMAPS));
			    HAL_CmdBufIn(TAG(2*i+1));
		        HAL_CmdBufIn(VERTEX2II(Fire_Coor[2*i].x,Fire_Coor[2*i].y,2*i,0));
			    HAL_CmdBufIn(TAG_MASK(0));
			}
		}	
	}
	HAL_CmdBufIn(END());
	
}

FTINDEF FTVOID rotateBitmap()
{
	FTU8 i,t;
	static FTU32 rangle=90;
	t = HAL_Read8(REG_TOUCH_TAG);
    printf("tag = %d\n",t);
	if(t==0){
		for(i=0;i<FIRE_ICON_GROUP/2;i++){
            HAL_CmdBufIn(TAG_MASK(1));
			HAL_CmdBufIn(BEGIN(BITMAPS));
			HAL_CmdBufIn(TAG(2*i+1));
		    HAL_CmdBufIn(VERTEX2II(Fire_Coor[2*i].x,Fire_Coor[2*i].y,2*i,0));
			HAL_CmdBufIn(TAG_MASK(0));
		}
	}
	else{
		for(i=0;i<FIRE_ICON_GROUP/2;i++){
			if(i == (t-1)/2){
				HAL_CmdBufIn(TAG_MASK(1));
	            HAL_CmdBufIn(BEGIN(BITMAPS));
				HAL_CmdBufIn(TAG(2*i+1));
	            CoCmd_LOADIDENTITY;	      
				CoCmd_TRANSLATE(FireBmpHD[2*i].wide/2*FT800_TRANSFORM_MAX,FireBmpHD[2*i].high/2*FT800_TRANSFORM_MAX);
	            CoCmd_ROTATE(rangle*FT800_TRANSFORM_MAX/360);
				CoCmd_TRANSLATE(-FireBmpHD[2*i].wide/2*FT800_TRANSFORM_MAX,-FireBmpHD[2*i].high/2*FT800_TRANSFORM_MAX);
	            CoCmd_SETMATRIX;
	            /* display in ICON central */
	            HAL_CmdBufIn(VERTEX2II(Fire_Coor[2*i].x,Fire_Coor[2*i].y,2*i,0));
				HAL_CmdBufIn(TAG_MASK(0));
	            defaultMatrix();
	            HAL_CmdBufIn(END());
				rangle = rangle + 10;
		    }
			else{
                HAL_CmdBufIn(TAG_MASK(1));
			    HAL_CmdBufIn(BEGIN(BITMAPS));
			    HAL_CmdBufIn(TAG(2*i+1));
		        HAL_CmdBufIn(VERTEX2II(Fire_Coor[2*i].x,Fire_Coor[2*i].y,2*i,0));
			    HAL_CmdBufIn(TAG_MASK(0));
			}
		}	
	}
	HAL_CmdBufIn(END());
	
}

FTVOID getFile(FTU8 index)
{
    FTU8 i;
	FTU8 buffer[4] = {0};
	i = index;
	if(i<10){
		sprintf((char*)buffer , "00%d", i );
	}
	else if(i<100){
		sprintf((char*)buffer , "0%d", i );
	}
	else if(i<=142){
		sprintf((char*)buffer , "%d", i );
	}
		//printf("path = %s\n",p);
		sprintf((char*)filePath,"%s%s.raw",(char*)bmpHD[0].path,buffer);
		temp_bmpHD[0] = bmpHD[0];
		temp_bmpHD[0].path = (FTC8*)filePath;
}


FTU8 bmpMap(FTU32 dspHDL,FTU8 *map)
{
	FTU32 flen = 0; 
	if(*map==0){
		BmpLayout(AMICON_HDL_FIRST,RAM_G,&temp_bmpHD[0]);
		BmpLayout(AMICON_HDL_SECOND,RAM_G+39200,&temp_bmpHD[0]);
	    *map = 1;
	}

	if(dspHDL==AMICON_HDL_FIRST)   
	{
		flen = appFileToRamG(temp_bmpHD[0].path,RAM_G,1,0,0);
		if (flen == 0) {
		    DBGPRINT;
			return 0;
		}
	}
	else if(dspHDL==AMICON_HDL_SECOND)
	{
		flen = appFileToRamG(temp_bmpHD[0].path,RAM_G+39200,1,0,0);
		if (flen == 0) {
			DBGPRINT;
			return 0;
		}
	}
	return flen;
}


FTVOID fire_show (FTU32 para)
{
	FTU16 high = 0, flag = 0, count = 0;
	FTU8 init = 1,i=0;

		
	downFireBitmap();
	while (255 != HAL_Read8(REG_TOUCH_TAG)) {
	/* move up and down the title */
		if (++count >= BKGND_BLOB_MSPEED) {
			if (flag) {
				high--;
				flag = (high<=0)?0:1;
			} else {
				high++;
				flag = (high>=(FT800_LCD_HIGH-32))?1:0;
			}
			count = 0;
		}
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
		HAL_CmdBufIn(CLEAR(1,1,1));
		BKGND_BLOB_GRAD_BKG;
	
		drawFireBitmap();
		gIndex = 0;
		CoCmd_TEXT(FT800_LCD_WIDTH/2,high,BKGND_TLT_FONT,OPT_CENTERX,(FT8 *)ctrl_a[gIndex].tlt);	
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
		init = 0;
	}
	while (255==HAL_Read8(REG_TOUCH_TAG)) {
		/* hold till release */
	}

	appGP.appIndex++;
	//gIndex = 0;

	return;
}


FTVOID tremble_show (FTU32 para)
{
	FTU16 high = 0, flag = 0, count = 0;
	FTU8 init = 1,i=0;

		
	downTrembleBitmap();
	while (255 != HAL_Read8(REG_TOUCH_TAG)) {
		// move up and down the title 
		if (++count >= BKGND_BLOB_MSPEED) {
			if (flag) {
				high--;
				flag = (high<=0)?0:1;
			} else {
				high++;
				flag = (high>=(FT800_LCD_HIGH-32))?1:0;
			}
			count = 0;
		}
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
		HAL_CmdBufIn(CLEAR(1,1,1));
		BKGND_BLOB_GRAD_BKG;
	
		trembleBitmap();		
		gIndex = 2;		
		CoCmd_TEXT(FT800_LCD_WIDTH/2,high,BKGND_TLT_FONT,OPT_CENTERX,(FT8 *)ctrl_a[gIndex].tlt);
		
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
		init = 0;
	}
	while (255==HAL_Read8(REG_TOUCH_TAG)) {
		// hold till release 
	}

	appGP.appIndex++;
	//gIndex = 0;

	return;
}

FTVOID rotate_show (FTU32 para)
{
	FTU16 high = 0, flag = 0, count = 0;
	FTU8 init = 1,i=0,x;

		
	downRotateBitmap();
	while (255 != HAL_Read8(REG_TOUCH_TAG)) {
		// move up and down the title 	
		if (++count >= BKGND_BLOB_MSPEED) {
			if (flag) {
				high--;
				flag = (high<=0)?0:1;
			} else {
				high++;
				flag = (high>=(FT800_LCD_HIGH-32))?1:0;
			}
			count = 0;
		}
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
		HAL_CmdBufIn(CLEAR(1,1,1));
		BKGND_BLOB_GRAD_BKG;
	
		rotateBitmap();		
		gIndex = 1;
		CoCmd_TEXT(FT800_LCD_WIDTH/2,high,BKGND_TLT_FONT,OPT_CENTERX,(FT8 *)ctrl_a[gIndex].tlt);
		
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
		init = 0;

	}
	while (255==HAL_Read8(REG_TOUCH_TAG)) {
		// hold till release 
	}

	appGP.appIndex++;
	//gIndex = 0;

	return;
}




FTINDEF FTVOID drawGIF(FTU32 para)  
{
	FTU16 high = 0, flag = 0, count = 0;
	FTU8 init = 1,i=1,x;
	FTU32 flen=0,hdl=0;
	FTU8 t,ifMaped = 0;
	hdl = AMICON_HDL_FIRST;
	while (0 == TOUCHED) {
		// move up and down the title 
		if (++count >= BKGND_BLOB_MSPEED) {
			if (flag) {
				high--;
				flag = (high<=0)?0:1;
			} else {
				high++;
				flag = (high>=(FT800_LCD_HIGH-32))?1:0;
			}
			count = 0;
		}
        if(i>=122){
			i = 1;
		}
		i++;
		//printf("i=%d\n",i);
		getFile(i);
		if(bmpMap(hdl,&ifMaped)==0)
			DBGPRINT;

		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(CLEAR_COLOR_RGB(50,50,50));
		HAL_CmdBufIn(CLEAR(1,1,1));

	    HAL_CmdBufIn(BEGIN(BITMAPS));	
        HAL_CmdBufIn(VERTEX2II(0,0,hdl,0));
		gIndex = 3;
		CoCmd_TEXT(FT800_LCD_WIDTH/2,high,BKGND_TLT_FONT,OPT_CENTERX,(FT8 *)ctrl_a[gIndex].tlt);

		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(END());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
		if(hdl==AMICON_HDL_FIRST)
			hdl = AMICON_HDL_SECOND;
		else
			hdl = AMICON_HDL_FIRST;	
	}
	while (TOUCHED) {
		// hold till release 
	}

	//appGP.appIndex = 0;
	//gIndex = 0;

	return;
}



FTVOID drawHalo_show (FTU32 para)
{
	FTU16 high = 0, flag = 0, count = 0;
	FTU8 i=0;

		
	downHaloBitmap();
	while (255 != HAL_Read8(REG_TOUCH_TAG)) {
	/* move up and down the title */
		if (++count >= BKGND_BLOB_MSPEED) {
			if (flag) {
				high--;
				flag = (high<=0)?0:1;
			} else {
				high++;
				flag = (high>=(FT800_LCD_HIGH-32))?1:0;
			}
			count = 0;
		}
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
		HAL_CmdBufIn(CLEAR(1,1,1));
		BKGND_BLOB_GRAD_BKG;
		drawHaloBitmap();
		gIndex = 4;
		CoCmd_TEXT(FT800_LCD_WIDTH/2,high,BKGND_TLT_FONT,OPT_CENTERX,(FT8 *)ctrl_a[gIndex].tlt);	
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
	}
	while (255==HAL_Read8(REG_TOUCH_TAG)) {
		/* hold till release */
	}
	appGP.appIndex++;
	//gIndex = 0;
	return;
}

/*
 * actually, all background_x is able to be put in one routine and change by each touch
 * but here, just show you how to use Apps to switch between each sub routine
 * so, we seperate into some sub routine
 */
AppFunc Apps[] = {
	rotate_show,
	tremble_show,
	fire_show,
	drawHalo_show,
	drawGIF,
	/* Leave this NULL at the buttom of this array */
	NULL
};
#endif

