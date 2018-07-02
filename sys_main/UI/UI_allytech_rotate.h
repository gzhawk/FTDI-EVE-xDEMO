/* 
    Demo for AllyTech:
    1. for better background gradient effect
    2. for faster needle speed
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Apr
*/
#define AUTO_FILL 0

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
#define SHADOW_INX ROOT_PATH"allyrotate\\shadow-inx.bin"
#define SHADOW_LUT ROOT_PATH"allyrotate\\shadow-lut.bin"
/* partical needle effect */
#define PN_INX ROOT_PATH"allyrotate\\ndl-inx.bin"
#define PN_LUT ROOT_PATH"allyrotate\\ndl-lut.bin"

/* small number */
#define NUM_SMALL ROOT_PATH"allyrotate\\num-s.bin"

#define PIC_NUM     (6)
#define I_CUT1      (0)
#define I_CUT2      (1)
#define I_CUT3      (2)
#define I_SHADOW    (3)
#define I_NDL       (4)
#define I_NUM_S     (5)

#define CUTED_X     (0)
#define CUTED_M_X   (800/2-1)

#define STOP_PEROID (50)
#define NUM_BLUE    128//(64)

#define SHADOW_X    (42)
#define SHADOW_Y    (352)

#define SPEED_HALF  80//(40)
#define SPEED_START (0)
#define SPEED_END   (160)//(80)
#define SPEED_DELAY (50)

#define TST_NUM_H   (348)

FTU32 call_start = 0;
bmpHDR_st bmp_header[PIC_NUM] = {
    {CUT1_INX,   CUT1_LUT,   0,PALETTED8,   0,0,400,84},
    {CUT2_INX,   CUT2_LUT,   0,PALETTED8,   0,0,400,266},
    {CUT3_INX,   CUT3_LUT,   0,PALETTED8,   0,0,800,130},
    {SHADOW_INX, SHADOW_LUT, 0,PALETTED8,   0,0,701,200},
	{PN_INX,     PN_LUT,     0,PALETTED8,   0,0,6,56},
    {NUM_SMALL,  0,          0,ARGB4,       0,0,10,140},
};
typedef struct s_edge_str_ {
    FTU16 H_x;
    FTU16 L_x;
} s_edge_str_st;
s_edge_str_st shadow_edge[SPEED_HALF+1] = {
#if AUTO_FILL
/*0*/
{144,232},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},
/*10*/
{186,254},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},
/*20*/
{224,287},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},
/*30*/
{259,305},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},
/*40*/
{288,327},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},
/*50*/
{320,346},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},
/*60*/
{346,367},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},
/*70*/
{374,383},{0,0},{0,0},{0,0},{0,0},
{0,0},{0,0},{0,0},{0,0},{0,0},
#else
/*0*/
{144,232},{148,234},{152,236},{156,238},{160,240},
{164,242},{168,244},{172,246},{176,248},{180,250},
/*10*/
{186,254},{190,257},{194,260},{198,263},{202,266},
{206,269},{210,272},{214,275},{218,278},{222,281},
/*20*/
{224,287},{228,289},{232,291},{236,293},{240,295},
{244,297},{248,299},{252,301},{256,303},{260,305},
/*30*/
{259,305},{262,307},{265,309},{268,311},{271,313},
{274,315},{277,317},{280,319},{283,321},{286,323},
/*40*/
{288,327},{291,329},{294,331},{297,333},{300,335},
{303,337},{306,339},{309,341},{312,343},{315,345},
/*50*/
{320,346},{323,348},{326,350},{329,352},{332,354},
{335,356},{338,358},{341,360},{344,362},{347,364},
/*60*/
{346,367},{349,369},{352,371},{355,373},{358,375},
{361,377},{364,379},{367,381},{370,383},{373,385},
/*70*/
{374,383},{377,385},{380,387},{383,389},{386,391},
{389,393},{392,395},{395,397},{398,399},{401,401},
#endif
/*80*/
{400,404}
};

typedef struct spd_n_ {
    FTU16 X;
    FTU16 O_Y;
    FTU16 scale;
    FT16 angle;
} spd_n_st;
spd_n_st speed_needle[SPEED_HALF+1] = {
#if AUTO_FILL
/*0*/
{165,25,118,-40},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
/*10*/
{198,19,115,-34},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
/*20*/
{231,14,112,-30},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
/*30*/
{263,10,111,-24},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
/*40*/
{291,8,106,-20},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
/*50*/
{320,5,106,-13},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
/*60*/
{347,5,104,-10},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
/*70*/
{374,4,103,-5},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
#else
/*0*/
{165,25,118,-40},{168,25,118,-40},{171,25,118,-40},{174,25,118,-40},{177,25,118,-40},
{180,24,118,-39},{183,23,118,-38},{186,22,118,-37},{189,21,117,-36},{192,20,116,-35},
/*10*/
{198,19,115,-34},{201,19,115,-34},{204,19,115,-34},{207,19,115,-34},{210,19,115,-34},
{213,19,115,-34},{216,18,115,-34},{219,17,115,-33},{222,16,114,-32},{225,15,113,-31},
/*20*/
{231,14,112,-30},{234,14,112,-30},{237,14,112,-30},{240,14,112,-30},{243,14,112,-30},
{246,14,112,-29},{249,14,112,-28},{252,13,112,-27},{255,12,112,-26},{258,11,112,-25},
/*30*/
{263,10,111,-24},{266,10,111,-24},{269,10,111,-24},{272,10,111,-24},{275,10,111,-24},
{278,10,111,-24},{281,10,110,-24},{284,10,109,-23},{287,10,108,-22},{290,9,107,-21},
/*40*/
{291,8,106,-20},{294,8,106,-20},{297,8,106,-20},{300,8,106,-20},{303,8,106,-19},
{306,8,106,-18},{309,8,106,-17},{312,8,106,-16},{315,7,106,-15},{318,6,106,-14},
/*50*/
{320,5,106,-13},{323,5,106,-13},{326,5,106,-13},{329,5,106,-13},{332,5,106,-13},
{335,5,106,-13},{338,5,106,-13},{341,5,106,-13},{344,5,106,-12},{347,5,105,-11},
/*60*/
{347,5,104,-10},{350,5,104,-10},{353,5,104,-10},{356,5,104,-10},{359,5,104,-10},
{362,5,104,-10},{365,5,104,-9},{368,5,104,-8},{371,5,104,-7},{374,5,104,-6},
/*70*/
{374,4,103,-5},{377,4,103,-5},{380,4,103,-5},{383,4,103,-5},{386,4,103,-5},
{389,4,103,-5},{392,4,103,-4},{395,4,103,-3},{398,4,103,-2},{401,4,103,-1},
#endif
/*80*/
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
        return 400+(400-p[SPEED_HALF - (spd - SPEED_HALF)].L_x)+4;
    } else {
        return p[spd].L_x;
    }
}
FTU16 getSpdX(FTU32 spd, spd_n_st *p)
{
    if (spd > SPEED_HALF) {
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
FTVOID prePal8(FTVOID)
{
    HAL_CmdBufIn(COLOR_MASK(0,0,0,1));//0
    HAL_CmdBufIn(RETURN());//1

    HAL_CmdBufIn(COLOR_MASK(0,0,1,0));//2
    HAL_CmdBufIn(RETURN());//3

    HAL_CmdBufIn(COLOR_MASK(0,1,0,0));//4
    HAL_CmdBufIn(RETURN());//5

    HAL_CmdBufIn(BLEND_FUNC(DST_ALPHA, ONE_MINUS_DST_ALPHA));//6
    HAL_CmdBufIn(COLOR_MASK(1,0,0,0));//7
    HAL_CmdBufIn(RETURN());//8

    HAL_CmdBufIn(SAVE_CONTEXT());//9
    HAL_CmdBufIn(BLEND_FUNC(ONE, ZERO));//10
    HAL_CmdBufIn(RETURN());//11

	HAL_CmdBufIn(CLEAR(1,0,0));//12
    HAL_CmdBufIn(RESTORE_CONTEXT());//13
    HAL_CmdBufIn(RETURN());//14
}
/* 
in order to optimize the call speed
build up some call structure
insdie the display list
the call_start was from "offset" value in ally_rotate()
in real practice, instead of cutting the calling peices to this little
you may use a larger part of calling peices display list
for even better performance
*/
FTVOID dispPal8 (FTU32 X, FTU32 Y, FTU32 PalSrc, FTU32 hdl, FTU32 cell)
{
    /* every thing after this commands would not display
       if not use save/restore context */
    //HAL_CmdBufIn(SAVE_CONTEXT());
    //HAL_CmdBufIn(BLEND_FUNC(ONE, ZERO));
    //HAL_CmdBufIn(COLOR_MASK(0,0,0,1));
	HAL_CmdBufIn(CALL(call_start+9));
	HAL_CmdBufIn(CALL(call_start));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 3));
    HAL_CmdBufIn(BITMAP_HANDLE(hdl));
    HAL_CmdBufIn(CELL(cell));
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

    //HAL_CmdBufIn(BLEND_FUNC(DST_ALPHA, ONE_MINUS_DST_ALPHA));
    //HAL_CmdBufIn(COLOR_MASK(1,0,0,0));
	HAL_CmdBufIn(CALL(call_start+6));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 2));
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

    //HAL_CmdBufIn(COLOR_MASK(0,1,0,0));
	HAL_CmdBufIn(CALL(call_start+4));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 1));
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

    //HAL_CmdBufIn(COLOR_MASK(0,0,1,0));
	HAL_CmdBufIn(CALL(call_start+2));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 0));
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

    /* while using previous stencil function need to add this */
	//HAL_CmdBufIn(COLOR_MASK(0,0,0,1));
	//HAL_CmdBufIn(CLEAR(1,0,0));
    //HAL_CmdBufIn(RESTORE_CONTEXT());
	HAL_CmdBufIn(CALL(call_start));
	HAL_CmdBufIn(CALL(call_start+12));
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

    /* for debug */
    if (SPEED_START == SPEED_END) {
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
    FTDELAY(SPEED_DELAY);
}
/* pX and pY is just for debug purpose */
FTVOID draw_needle (FTU32 spd, spd_n_st *sp_ndl, FTU16 *pX, FTU16 *pY)
{
    FTU16 X,Y,scale,offset = bmp_header[I_NDL].high*2/3;
    FT16 angle;
    /* 
     you may put the calculated result of X,Y,Scale,Angle
     in array for even better performance
     here just leave it for run time for better code understanding
     */
    X = getSpdX(spd, sp_ndl)-offset-3;
    /* pX and pY is just for debug purpose */
    *pX = getSpdX(spd, sp_ndl)-6;
    Y = SHADOW_Y + getSpdOY(spd, sp_ndl);
    /* pX and pY is just for debug purpose */
    *pY = Y-14;
    scale = getSpdScale(spd, sp_ndl);
    angle = getSpdAngle(spd, sp_ndl);

    HAL_CmdBufIn(SAVE_CONTEXT());
	CoCmd_LOADIDENTITY;
    CoCmd_TRANSLATE(offset*EVE_TRANSFORM_MAX, 0);
	CoCmd_ROTATE(angle*EVE_TRANSFORM_MAX/360);
    CoCmd_SCALE(EVE_TRANSFORM_MAX,scale*EVE_TRANSFORM_MAX/100);
	CoCmd_SETMATRIX;
	/* speed needle */
	dispPal8(X,Y,bmp_header[I_NDL].lut_src,I_NDL,0);
    HAL_CmdBufIn(RESTORE_CONTEXT());
}
/* 
  I was too lazy to create some other
  variable to help define gap less than
  10, so use an "never" happen gap number
  to take a special mark for this situation
 */
#define S_MARK 800
FT16 getGap(FT16 left, FT16 right)
{
    FT16 gap = left - right,gapX = gap>=0?1:0;
    
    gap *= gap >= 0?1:(-1);
    if (gap < 10) {
        if (gap == 0) {
            return 0;
        }
        gap += S_MARK;
    } else {
        if (gap%10 >= 5) {
            gap = gap/10 + 1;
        } else {
            gap = gap/10;
        }
    }
    return gap*(gapX?1:(-1));
}
FTVOID setGap(FT16 *pLeft, FT16 right, FT16 gap, FTU16 start, FTU16 index)
{
    FTU8 gapX = gap>=0?1:0;

    gap *= gap >= 0?1:(-1);
    if (S_MARK < gap) {
        if ((index - start) >= 10 - (gap - S_MARK)) {
            *pLeft = right+(gapX?1:(-1));
        } else {
            *pLeft = right;
        }
    } else {
        gap *= gapX?1:(-1);
        *pLeft = (right>0)?(right+gap):(right-gap);
    }
}
FTVOID fill10edge(FTVOID)
{
    FTU16 i,j;
    FT16 gap1,gap2,gap3,gap4;

    printf("\n/*Edge*/");
    for (i = 0; i < (SPEED_HALF-10+1); i += 10) {
        gap1 = getGap(shadow_edge[i+10].H_x,shadow_edge[i].H_x);
        gap2 = getGap(shadow_edge[i+10].L_x,shadow_edge[i].L_x);
        printf("\n/*%d*/\n",i);
        printf("{%d,%d},",shadow_edge[i].H_x,shadow_edge[i].L_x);
        for (j = i+1; j < i+10; j++) {
            if (j == i+5) {
                printf("\n");
            }
            setGap((FT16 *)&(shadow_edge[j].H_x),shadow_edge[j-1].H_x,gap1,i+1,j);
            printf("{%d,",shadow_edge[j].H_x);

            setGap((FT16 *)&(shadow_edge[j].L_x),shadow_edge[j-1].L_x,gap2,i+1,j);
            printf("%d},",shadow_edge[j].L_x);
        }
    }
    printf("\n/*%d*/\n",i);
    printf("{%d,%d}",shadow_edge[i].H_x,shadow_edge[i].L_x);

    printf("\n/*Needle*/");
    for (i = 0; i < (SPEED_HALF-10+1); i += 10) {
        gap1 = getGap(speed_needle[i+10].X,speed_needle[i].X);
        gap2 = getGap(speed_needle[i+10].O_Y,speed_needle[i].O_Y);
        gap3 = getGap(speed_needle[i+10].scale,speed_needle[i].scale);
        gap4 = getGap(speed_needle[i+10].angle,speed_needle[i].angle);
        printf("\n/*%d*/\n",i);
        printf("{%d,%d,%d,%d},",speed_needle[i].X,speed_needle[i].O_Y,
                               speed_needle[i].scale,speed_needle[i].angle);
        for (j = i+1; j < i+10; j++) {
            if (j == i+5) {
                printf("\n");
            }
            setGap((FT16 *)&(speed_needle[j].X),speed_needle[j-1].X,gap1,i+1,j);
            printf("{%d,",speed_needle[j].X);
            setGap((FT16 *)&(speed_needle[j].O_Y),speed_needle[j-1].O_Y,gap2,i+1,j);
            printf("%d,",speed_needle[j].O_Y);
            setGap((FT16 *)&(speed_needle[j].scale),speed_needle[j-1].scale,gap3,i+1,j);
            printf("%d,",speed_needle[j].scale);
            setGap(&(speed_needle[j].angle),speed_needle[j-1].angle,gap4,i+1,j);
            printf("%d},",speed_needle[j].angle);
        }
    }
    printf("\n/*%d*/\n",i);
    printf("{%d,%d,%d,%d}",speed_needle[i].X,speed_needle[i].O_Y,
            speed_needle[i].scale,speed_needle[i].angle);
}
FTU32 ally_fixed_background (FTVOID)
{
	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));

    HAL_CmdBufIn(BEGIN(BITMAPS));
    
    /* background: cut1 */
    dispPal8(CUTED_X,0,bmp_header[I_CUT1].lut_src, I_CUT1, 0);
    /* background: vertical mirror cut1 */
    HAL_CmdBufIn(SAVE_CONTEXT());
    CoCmd_LOADIDENTITY;	      
    CoCmd_TRANSLATE(800/4*EVE_TRANSFORM_MAX,0);
    CoCmd_SCALE(-1*EVE_TRANSFORM_MAX,1*EVE_TRANSFORM_MAX);
    CoCmd_TRANSLATE(-800/4*EVE_TRANSFORM_MAX,0);
    CoCmd_SETMATRIX;
    dispPal8(CUTED_M_X,0,bmp_header[I_CUT1].lut_src, I_CUT1, 0);
    HAL_CmdBufIn(RESTORE_CONTEXT());
    
    /* background: cut2 */
    dispPal8(CUTED_X,bmp_header[I_CUT1].high,bmp_header[I_CUT2].lut_src, I_CUT2, 0);
    /* background: vertical mirror cut2 */
    HAL_CmdBufIn(SAVE_CONTEXT());
    CoCmd_LOADIDENTITY;
    CoCmd_TRANSLATE(800/4*EVE_TRANSFORM_MAX,0);
    CoCmd_SCALE(-1*EVE_TRANSFORM_MAX,1*EVE_TRANSFORM_MAX);
    CoCmd_TRANSLATE(-800/4*EVE_TRANSFORM_MAX,0);
    CoCmd_SETMATRIX;
    dispPal8(CUTED_M_X,bmp_header[I_CUT1].high,bmp_header[I_CUT2].lut_src, I_CUT2, 0);
    HAL_CmdBufIn(RESTORE_CONTEXT());

    HAL_CmdBufIn(STENCIL_OP(INCR,INCR));
    HAL_CmdBufIn(BEGIN(BITMAPS));
    /* background: cut3 */
    dispPal8(CUTED_X,bmp_header[I_CUT1].high+bmp_header[I_CUT2].high,bmp_header[I_CUT3].lut_src, I_CUT3, 0);

#if AUTO_FILL
    CoCmd_TEXT(400,240,23,OPT_CENTERX,"auto filling parameter");
#else
    CoCmd_TEXT(400,240,23,OPT_CENTERX,"manual filling parameter");
#endif
    HAL_CmdBufIn(COLOR_MASK(0,0,0,0));
    HAL_CmdBufIn(BEGIN(EDGE_STRIP_L));

    HAL_BufToReg(RAM_CMD,0);

    return HAL_Read32(REG_CMD_DL);
}
FTVOID ally_realtime_background (FTU32 speed)
{
    FTU16 x,y;

    /* shadow left edge strip */
    HAL_CmdBufIn(VERTEX2F(getHX(speed,shadow_edge)*EVE_PIXEL_UNIT,SHADOW_Y*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(VERTEX2F(getLX(speed,shadow_edge)*EVE_PIXEL_UNIT,(SHADOW_Y+100)*EVE_PIXEL_UNIT));

    HAL_CmdBufIn(BEGIN(BITMAPS));
    HAL_CmdBufIn(STENCIL_FUNC(GEQUAL,5,255));
	/* shadow background */
    HAL_CmdBufIn(BEGIN(BITMAPS));
    if (NUM_BLUE >= speed) {
	    dispPal8(SHADOW_X,SHADOW_Y,bmp_header[I_SHADOW].lut_src,I_SHADOW,0);
    } else {
	    dispPal8(SHADOW_X,SHADOW_Y,bmp_header[I_SHADOW].lut_src,I_SHADOW,1);
    }
    HAL_CmdBufIn(COLOR_MASK(1,1,1,1));
    HAL_CmdBufIn(STENCIL_FUNC(ALWAYS,0,255));

    draw_needle(speed, speed_needle, &x, &y);

    /* small number for test multi bitmap overlap */
    HAL_CmdBufIn(BITMAP_HANDLE(I_NUM_S));

    HAL_CmdBufIn(CELL(speed/100));
    HAL_CmdBufIn(VERTEX2F((x-bmp_header[I_NUM_S].wide)*EVE_PIXEL_UNIT,
                          y*EVE_PIXEL_UNIT));

    HAL_CmdBufIn(CELL((speed/10)%10));
    HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT,
                          y*EVE_PIXEL_UNIT));

    HAL_CmdBufIn(CELL(speed%10));
    HAL_CmdBufIn(VERTEX2F((x+bmp_header[I_NUM_S].wide)*EVE_PIXEL_UNIT,
                          y*EVE_PIXEL_UNIT));

    HAL_CmdBufIn(DISPLAY());
}
FTU32 ally_get_bitmap_tail(bmpHDR_st *p, FTU32 n)
{
    FTU32 i,l;

    for (i = 0, l = 0; i < n; i++) {
        if (p[i].path) {
            l += p[i].len;
        }
        if (p[i].path_lut) {
            l += p[i].len_lut;
        }
    }
    return l;
}
FTVOID ally_special_treatment(FTVOID)
{
    /* special treatment for number bitmap to use cell */
    HAL_DlpBufIn(BITMAP_HANDLE(I_NUM_S));
    HAL_DlpBufIn(BITMAP_LAYOUT(bmp_header[I_NUM_S].format,appGetLinestride(bmp_header[I_NUM_S]),bmp_header[I_NUM_S].high/10));
#ifdef DEF_81X
    HAL_DlpBufIn(BITMAP_LAYOUT_H(appGetLinestride(bmp_header[I_NUM_S]) >> 10,(bmp_header[I_NUM_S].high/10)>>9));
#endif       
    HAL_DlpBufIn(BITMAP_SIZE(NEAREST,BORDER,BORDER,bmp_header[I_NUM_S].wide,(bmp_header[I_NUM_S].high/10)));
#ifdef DEF_81X
    HAL_DlpBufIn(BITMAP_SIZE_H(bmp_header[I_NUM_S].wide >> 9,(bmp_header[I_NUM_S].high/10)>>9));
#endif 
    /* special treatment for shadow bitmap to use cell */
    HAL_DlpBufIn(BITMAP_HANDLE(I_SHADOW));
    HAL_DlpBufIn(BITMAP_LAYOUT(bmp_header[I_SHADOW].format,appGetLinestride(bmp_header[I_SHADOW]),bmp_header[I_SHADOW].high/2));
#ifdef DEF_81X
    HAL_DlpBufIn(BITMAP_LAYOUT_H(appGetLinestride(bmp_header[I_SHADOW]) >> 10,(bmp_header[I_SHADOW].high/2)>>9));
#endif       
    HAL_DlpBufIn(BITMAP_SIZE(NEAREST,BORDER,BORDER,bmp_header[I_SHADOW].wide,(bmp_header[I_SHADOW].high/2)));
#ifdef DEF_81X
    HAL_DlpBufIn(BITMAP_SIZE_H(bmp_header[I_SHADOW].wide >> 9,(bmp_header[I_SHADOW].high/2)>>9));
#endif         
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
}
FTVOID ally_rotate (FTU32 para)
{
	static FTU32 len_fDL1 = 0, addr_RAMG1,
                 len_fDL2 = 0, addr_RAMG2,
				 skip_it = 0;
    FTU32 offset;

	/* run it once */
	if (!len_fDL1) {
		/* load bitmap resources data into EVE */
		if(APP_OK != appBmpToRamG(0, RAM_G, bmp_header, PIC_NUM)){
			DBGPRINT;
			return;
		}

		/* do some special treatment for image rotate */
        ally_special_treatment();
#if AUTO_FILL
		/* automatically fill the needle moving track data */
        fill10edge();
#endif

		/* calculate the offset in RAM_G for previous generated display list */
        addr_RAMG1 = ally_get_bitmap_tail(bmp_header, PIC_NUM);
	}

	/* run it twice */
	if (!skip_it) {
		/* generate display list for 'never change' part of backgroudn */
        len_fDL1 = ally_fixed_background();

		/* copy the display list into specific offset in RAM_G */
        CoCmd_MEMCPY(addr_RAMG1,RAM_DL,len_fDL1);

		/* execute above command */
        HAL_BufToReg(RAM_CMD,0);
	} else {
        HAL_CmdBufIn(CMD_DLSTART);
        CoCmd_APPEND(addr_RAMG1,len_fDL1);
    }
    
	/* run it every times */
    ally_realtime_background(*(FTU32 *)para);

	/* run it twice */
	if (!skip_it) {
		/* execute above command */
        HAL_BufToReg(RAM_CMD,0);

        offset = HAL_Read32(REG_CMD_DL);
        prePal8();
		/* execute above command */
        HAL_BufToReg(RAM_CMD,0);

        len_fDL2 = HAL_Read32(REG_CMD_DL) - offset;
        addr_RAMG2 = addr_RAMG1+len_fDL1;

		/* copy the display list into specific offset in RAM_G */
        CoCmd_MEMCPY(addr_RAMG2,RAM_DL+offset,len_fDL2);

		/* execute above command */
        HAL_BufToReg(RAM_CMD,0);
    } else {
        CoCmd_APPEND(addr_RAMG2,len_fDL2);
    }
    
	/* the first times coming down is to get the offset
	   the data in first time run is wrong
	   need to run it again with right call_start*/
    if (!call_start) {
        /* what CALL needs is instruction offset
           one instruction 4 bytes*/
        call_start = offset/FTU32_LEN;
	    appGP.appIndex = 1;
        return;
	}
	
	if (!skip_it) {
		skip_it = 1;
	}

	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);
	appGP.appIndex = 0;
	
	return;
}

AppFunc APPS_UI[] = {
    allytech_speed,
	ally_rotate
};

