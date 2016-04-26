/* 
    Demo for AllyTech:
    1. for better background gradient effect
    2. for faster needle speed
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Apr
*/
#ifdef DEMO_ALLY_ROTATE

/*background: upper cut*/
#define CUT1_INX ROOT_PATH"allyrotate\\cut1-inx.bin"
#define CUT1_LUT ROOT_PATH"allyrotate\\cut1-lut.bin"
/*background: middle cut*/
#define CUT2_INX ROOT_PATH"allyrotate\\cut2-inx.bin"
#define CUT2_LUT ROOT_PATH"allyrotate\\cut2-lut.bin"
/*background: bottom cut*/
#define CUT3_INX ROOT_PATH"allyrotate\\cut3-inx.bin"
#define CUT3_LUT ROOT_PATH"allyrotate\\cut3-lut.bin"
/* full needle shadow effect */
#define FN_B_INX ROOT_PATH"allyrotate\\blue-inx.bin"
#define FN_B_LUT ROOT_PATH"allyrotate\\blue-lut.bin"
#define FN_R_INX ROOT_PATH"allyrotate\\red-inx.bin"
#define FN_R_LUT ROOT_PATH"allyrotate\\red-lut.bin"
/* partical needle effect */
#define PN_INX ROOT_PATH"allyrotate\\ndl-inx.bin"
#define PN_LUT ROOT_PATH"allyrotate\\ndl-lut.bin"

#define PIC_NUM     (6)
#define I_CUT1      (0)
#define I_CUT2      (1)
#define I_CUT3      (2)
#define I_BLUE      (3)
#define I_RED       (4)
#define I_NDL       (5)
#define STOP_PEROID (50)
#define NUM_BLUE    (64)
#define SHADOW_Y    (354)
#define SPEED_HALF  (40)
#define SPEED_START (0)
#define SPEED_END   (80)
#define CUTED_X     (0)
#define CUTED_M_X   (800/2-1)

bmpHDR_st bmp_header[PIC_NUM] = {
    {CUT1_INX,   CUT1_LUT,   0,PALETTED8,   0,0,400,84},
    {CUT2_INX,   CUT2_LUT,   0,PALETTED8,   0,0,400,266},
    {CUT3_INX,   CUT3_LUT,   0,PALETTED8,   0,0,800,130},
    {FN_B_INX,   FN_B_LUT,   0,PALETTED8,   0,0,716,100},
	{FN_R_INX,   FN_R_LUT,   0,PALETTED8,   0,0,716,100},//for shadow swap
	{PN_INX,     PN_LUT,     0,PALETTED8,   0,0,6,56},
};
typedef struct s_edge_str_ {
    FTU16 H_x;
    FTU16 L_x;
} s_edge_str_st;
s_edge_str_st shadow_edge[SPEED_HALF+1] = {
/*0*/
{144,232},{152,237},{160,242},{168,247},{176,252},
{184,257},{192,262},{200,267},{208,272},{216,277},
/*10*/
{224,287},{230,291},{236,295},{242,299},{248,303},
{254,307},{260,311},{266,315},{272,319},{278,323},
/*20*/
{288,327},{293,331},{298,335},{303,339},{308,343},
{313,347},{318,351},{323,355},{328,359},{333,363},
/*30*/
{346,367},{351,370},{356,373},{361,376},{366,379},
{371,382},{376,385},{381,388},{386,391},{391,394},
/*40*/
{400,404}
};

typedef struct spd_n_ {
    FTU16 X;
    FTU16 O_Y;
    FTU16 scale;
    FT16 angle;
} spd_n_st;

spd_n_st speed_needle[SPEED_HALF+1] = {
/*0*/
{168,29,110,-40},{174,28,110,-39},{180,27,110,-38},{186,26,110,-37},{192,25,110,-36},
{198,23,110,-35},{204,22,110,-34},{210,20,110,-33},{216,19,110,-32},{222,18,110,-31},
/*10*/
{234,17,107,-30},{239,16,107,-29},{244,15,107,-28},{249,14,107,-27},{254,13,107,-26},
{259,12,107,-25},{264,11,107,-24},{269,11,107,-23},{274,10,107,-22},{279,10,107,-21},
/*20*/
{290,11,105,-20},{295,10,105,-19},{300,9,105,-18},{305,8,105,-17},{310,7,104,-16},
{315,7,104,-15},{320,6,104,-14},{325,6,104,-13},{330,6,104,-12},{335,6,104,-11},
/*30*/
{347,6,103,-10},{352,5,103,-9},{357,5,103,-8},{362,4,103,-7},{367,4,103,-6},
{372,4,103,-5},{377,4,103,-4},{382,4,103,-3},{387,4,103,-2},{392,4,103,-1},
/*40*/
{400,3,102,0}
};
FTU32 g_speed = 0;
FTU16 getHX(FTU32 spd, s_edge_str_st *p)
{
    if (spd > SPEED_HALF) {
        return 400+(400-p[SPEED_HALF - (spd - SPEED_HALF)].H_x);
    } else {
        return p[spd].H_x;
    }
}
FTU16 getLX(FTU32 spd, s_edge_str_st *p)
{
    if (spd > SPEED_HALF) {
        /* 
           due to bitmap not 100% mirror
           in right side, 
           edge strip a little bit more
           to cover the tail of needle
         */
        return 400+(400-p[SPEED_HALF - (spd - SPEED_HALF)].L_x)+9;
    } else {
        return p[spd].L_x;
    }
}
FTU16 getSpdX(FTU32 spd, spd_n_st *p)
{
    if (spd > 40) {
        return 400+(400-p[SPEED_HALF - (spd - SPEED_HALF)].X);
    } else {
        return p[spd].X;
    }
}
FTU16 getSpdOY(FTU32 spd, spd_n_st *p)
{
    if (spd > SPEED_HALF) {
        /* 
           due to bitmap not 100% mirror
           in right side, 
           rise up a little bit more
           to prevent the tail of needle 
           exceed the bottom line
         */
        return p[SPEED_HALF - (spd - SPEED_HALF)].O_Y-3;
    } else {
        return p[spd].O_Y;
    }
}
FTU16 getSpdScale(FTU32 spd, spd_n_st *p)
{
    if (spd > SPEED_HALF) {
        return p[SPEED_HALF - (spd - SPEED_HALF)].scale;
    } else {
        return p[spd].scale;
    }
}
FT16 getSpdAngle(FTU32 spd, spd_n_st *p)
{
    if (spd > SPEED_HALF) {
        return -1*p[SPEED_HALF - (spd - SPEED_HALF)].angle;
    } else {
        return p[spd].angle;
    }
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
    HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

    HAL_CmdBufIn(BLEND_FUNC(DST_ALPHA, ONE_MINUS_DST_ALPHA));
    HAL_CmdBufIn(COLOR_MASK(1,0,0,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 2));
    HAL_CmdBufIn(BITMAP_HANDLE(hdl));
    HAL_CmdBufIn(CELL(cell));
    HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

    HAL_CmdBufIn(COLOR_MASK(0,1,0,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 1));
    HAL_CmdBufIn(BITMAP_HANDLE(hdl));
    HAL_CmdBufIn(CELL(cell));
    HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

    HAL_CmdBufIn(COLOR_MASK(0,0,1,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 0));
    HAL_CmdBufIn(BITMAP_HANDLE(hdl));
    HAL_CmdBufIn(CELL(cell));
    HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

    /* while using previous stencil function need to add this */
	HAL_CmdBufIn(COLOR_MASK(0,0,0,1));
	HAL_CmdBufIn(CLEAR(1,0,0));

    HAL_CmdBufIn(RESTORE_CONTEXT());
}
FTVOID allytech_speed (FTU32 para)
{
    static FTU8 nFlag = 0;

	appGP.appPara = (FTU32)&g_speed;
	appGP.appIndex = 1;
	if (para) {
		g_speed = *(FTU32 *)para;
	} else {
		/* at the very beginning */
		g_speed = SPEED_START;
		return;
	}

    /* moving forward or backward control */
	if (g_speed == SPEED_END) {
        nFlag = 1;
	} else if (g_speed == SPEED_START) {
        nFlag = 0;
	}

    if (nFlag == 1) {
		g_speed--;
    } else {
        g_speed++;
    }
}
FTVOID draw_needle (FTU32 spd, spd_n_st *sp_ndl)
{
    FTU16 X,Y,scale,offset = bmp_header[I_NDL].high*2/3;
    FT16 angle;
    /* 
     you may put the calculated result of X,Y,Scale,Angle
     in array for even better performance
     here just leave it for run time for better code understanding
     */
    X = getSpdX(spd, sp_ndl)-offset-3;
    Y = SHADOW_Y + getSpdOY(spd, sp_ndl);
    scale = getSpdScale(spd, sp_ndl);
    angle = getSpdAngle(spd, sp_ndl);

    HAL_CmdBufIn(SAVE_CONTEXT());
	CoCmd_LOADIDENTITY;
    CoCmd_TRANSLATE(offset*FT800_TRANSFORM_MAX, 0);
	CoCmd_ROTATE(angle*FT800_TRANSFORM_MAX/360);
    CoCmd_SCALE(FT800_TRANSFORM_MAX,scale*FT800_TRANSFORM_MAX/100);
	CoCmd_SETMATRIX;
	/* speed needle */
	dispPal8(X,Y,bmp_header[I_NDL].lut_src,I_NDL,0);
    HAL_CmdBufIn(RESTORE_CONTEXT());
}
/* 
 This part not in running function
 just be used to generate some data 
 for easy fill in array during debugging
 */
FTVOID fill10edge(FTVOID)
{
    FTU16 i,j;
    FT16 gap1,gap2,gap3,gap4;

    printf("\n/*Edge*/");
    for (i = 0; i < 31; i += 10) {
        gap1 = (shadow_edge[i+10].H_x - shadow_edge[i].H_x)/10;
        gap2 = (shadow_edge[i+10].L_x - shadow_edge[i].L_x)/10;
        printf("\n/*%d*/\n",i);
        printf("{%d,%d},",shadow_edge[i].H_x,shadow_edge[i].L_x);
        for (j = i+1; j < i+10; j++) {
            if (j == i+5) {
                printf("\n");
            }
            shadow_edge[j].H_x = shadow_edge[j-1].H_x+gap1;
            printf("{%d,",shadow_edge[j].H_x);
            shadow_edge[j].L_x = shadow_edge[j-1].L_x+gap2;
            printf("%d},",shadow_edge[j].L_x);
        }
    }
    printf("\n/*%d*/\n",i);
    printf("{%d,%d}",shadow_edge[i].H_x,shadow_edge[i].L_x);

    printf("\n/*Needle*/");
    for (i = 0; i < 31; i += 10) {
        gap1 = (speed_needle[i+10].X - speed_needle[i].X)/10;
        gap2 = (speed_needle[i].O_Y - speed_needle[i+10].O_Y )/10;
        gap3 = (speed_needle[i].scale - speed_needle[i+10].scale )/10;
        gap4 = (speed_needle[i].angle - speed_needle[i+10].angle )/10;
        printf("\n/*%d*/\n",i);
        printf("{%d,%d,%d,%d},",speed_needle[i].X,speed_needle[i].O_Y,
                               speed_needle[i].scale,speed_needle[i].angle);
        for (j = i+1; j < i+10; j++) {
            if (j == i+5) {
                printf("\n");
            }
            speed_needle[j].X = speed_needle[j-1].X+gap1;
            printf("{%d,",speed_needle[j].X);
            speed_needle[j].O_Y = speed_needle[j-1].O_Y-gap2;
            printf("%d,",speed_needle[j].O_Y);
            speed_needle[j].scale = speed_needle[j-1].scale-gap3;
            printf("%d,",speed_needle[j].scale);
            speed_needle[j].angle = speed_needle[j-1].angle-gap4;
            printf("%d},",speed_needle[j].angle);
        }
    }
    printf("\n/*%d*/\n",i);
    printf("{%d,%d,%d,%d}",speed_needle[i].X,speed_needle[i].O_Y,
            speed_needle[i].scale,speed_needle[i].angle);
}
FTVOID ally_rotate (FTU32 para)
{
	static FTU8 init = 0;
    FTU32 speed = *(FTU32 *)para;

	/* 
      this part just for this routine jump out 
      the outside caller when error happen 
     */
	appGP.appIndex = 2;

	if (!init) {
		/* load bitmap resources data into FT800 */
		if(APP_OK != appBmpToRamG(0, RAM_G, bmp_header, PIC_NUM)){
			DBGPRINT;
			return;
		}
        /* special treatment for rotate bitmap
           need to use BILINEAR
           high > wide: use high in both side
           high < wide: use wide in both side*/
        HAL_DlpBufIn(BITMAP_HANDLE(I_NDL));
        HAL_DlpBufIn(BITMAP_SIZE(BILINEAR,BORDER,BORDER,bmp_header[I_NDL].high*2,bmp_header[I_NDL].high*2));
#ifdef DEF_81X
        HAL_DlpBufIn(BITMAP_SIZE_H((bmp_header[I_NDL].high*2) >> 9,(bmp_header[I_NDL].high*2)>>9));
#endif         
        HAL_DlpBufIn(DISPLAY());
        HAL_BufToReg(RAM_DL,0);
        init = 1;
	}

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));

    HAL_CmdBufIn(STENCIL_OP(INCR,INCR));
    HAL_CmdBufIn(BEGIN(BITMAPS));
    
    /* background: cut1 */
    dispPal8(CUTED_X,0,bmp_header[I_CUT1].lut_src, I_CUT1, 0);
    /* background: vertical mirror cut1 */
    HAL_CmdBufIn(SAVE_CONTEXT());
    CoCmd_LOADIDENTITY;	      
    CoCmd_TRANSLATE(800/4*FT800_TRANSFORM_MAX,0);
    CoCmd_SCALE(-1*FT800_TRANSFORM_MAX,1*FT800_TRANSFORM_MAX);
    CoCmd_TRANSLATE(-800/4*FT800_TRANSFORM_MAX,0);
    CoCmd_SETMATRIX;
    dispPal8(CUTED_M_X,0,bmp_header[I_CUT1].lut_src, I_CUT1, 0);
    HAL_CmdBufIn(RESTORE_CONTEXT());
    
    /* background: cut2 */
    dispPal8(CUTED_X,bmp_header[I_CUT1].high,bmp_header[I_CUT2].lut_src, I_CUT2, 0);
    /* background: vertical mirror cut2 */
    HAL_CmdBufIn(SAVE_CONTEXT());
    CoCmd_LOADIDENTITY;	      
    CoCmd_TRANSLATE(800/4*FT800_TRANSFORM_MAX,0);
    CoCmd_SCALE(-1*FT800_TRANSFORM_MAX,1*FT800_TRANSFORM_MAX);
    CoCmd_TRANSLATE(-800/4*FT800_TRANSFORM_MAX,0);
    CoCmd_SETMATRIX;
    dispPal8(CUTED_M_X,bmp_header[I_CUT1].high,bmp_header[I_CUT2].lut_src, I_CUT2, 0);
    HAL_CmdBufIn(RESTORE_CONTEXT());

    HAL_CmdBufIn(END());

    HAL_CmdBufIn(STENCIL_OP(INCR,INCR));
    HAL_CmdBufIn(BEGIN(BITMAPS));
    /* background: cut3 */
    dispPal8(CUTED_X,bmp_header[I_CUT1].high+bmp_header[I_CUT2].high,bmp_header[I_CUT3].lut_src, I_CUT3, 0);
	
    /* shadow left edge strip */
    HAL_CmdBufIn(COLOR_MASK(0,0,0,0));
    HAL_CmdBufIn(BEGIN(EDGE_STRIP_L));
    HAL_CmdBufIn(VERTEX2F(getHX(speed,shadow_edge)*FT800_PIXEL_UNIT,SHADOW_Y*FT800_PIXEL_UNIT));
    HAL_CmdBufIn(VERTEX2F(getLX(speed,shadow_edge)*FT800_PIXEL_UNIT,(SHADOW_Y+100)*FT800_PIXEL_UNIT));

    HAL_CmdBufIn(BEGIN(BITMAPS));
    HAL_CmdBufIn(STENCIL_FUNC(GEQUAL,5,255));
	/* shadow background */
    HAL_CmdBufIn(BEGIN(BITMAPS));
    if (NUM_BLUE >= speed) {
	    dispPal8((FT800_LCD_WIDTH-bmp_header[I_BLUE].wide)/2,SHADOW_Y,bmp_header[I_BLUE].lut_src,I_BLUE,0);
    } else {
	    dispPal8((FT800_LCD_WIDTH-bmp_header[I_RED].wide)/2,SHADOW_Y,bmp_header[I_RED].lut_src,I_RED,0);
    }
    HAL_CmdBufIn(COLOR_MASK(1,1,1,1));

    draw_needle(speed, speed_needle);

    HAL_CmdBufIn(END());

    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
	
	appGP.appIndex = 0;
	
	return;
}

AppFunc Apps[] = {
    allytech_speed,
	ally_rotate,
	/* Leave this NULL at the buttom of this array */
	NULL
};

#endif


