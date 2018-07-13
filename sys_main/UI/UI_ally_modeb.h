/* 
    Sample code to show how to use sin/cos simulate a needle roate
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/May
*/

#define DISPBP_XNDL ROOT_PATH"allymodeb\\sNdl_inx.bin"
#define DISPBP_XNDL_LUT ROOT_PATH"allymodeb\\sNdl_lut.bin"
#define DISPBP_PALETTE8 ROOT_PATH"allymodeb\\BG_inx.bin"
#define DISPBP_PALETTE8_LUT ROOT_PATH"allymodeb\\BG_lut.bin"

#define RAW_NUM   (2)
#define HDL_START 0

#define NDL_START 73
#define NDL_END   264
#define NDL_X     400
#define NDL_Y     328
#define NDL_RADIUS 188

#define FAST_SIN

bmpHDR_st bmp_header[RAW_NUM] = {
    {DISPBP_XNDL,       DISPBP_XNDL_LUT,       0,PALETTED8,   0,0,23,112},
    {DISPBP_PALETTE8,   DISPBP_PALETTE8_LUT,   0,PALETTED8,   0,0,800,480},
};
FTU16 sintbl[] = 
{
/* 
   the original sin data multiple 10000 (e.g. sin(1) = 0.0174)
   so the calulated result need to div 10000, in order to 
   make the calulation faster for MCU (bit shift faster than div)
   div the original sin data by 1.22 to come out a new sin table
   then the result only need to right shift 13 bits
   (2^13=8192, 10000/8192=1.22) 
*/
#if defined(FAST_SIN)
    0   ,142 ,286 ,428 ,572 ,714 ,856 ,999 ,1140,
    1281,1422,1563,1704,1843,1982,2121,2259,2396,
    2532,2668,2803,2937,3070,3202,3333,3463,3593,
    3721,3848,3973,4098,4221,4343,4463,4583,4701,
    4818,4932,5046,5158,5268,5377,5484,5590,5694,
    5795,5895,5995,6090,6186,6278,6370,6459,6545,
    6631,6713,6795,6874,6950,7025,7098,7168,7236,
    7303,7367,7428,7487,7545,7600,7652,7702,7750,
    7795,7838,7879,7917,7953,7986,8017,8045,8072,
    8095,8117,8136,8151,8165,8177,8185,8191,8195,
    8196
#else
    0   , 174, 349, 523, 698, 872,1045,1219,1392,1564,
    1736,1908,2079,2249,2419,2588,2756,2924,3090,3256,
    3420,3584,3746,3907,4067,4226,4384,4540,4695,4848,
    5000,5150,5299,5446,5592,5736,5878,6018,6157,6293,
    6428,6561,6691,6820,6947,7071,7193,7314,7431,7547,
    7660,7772,7880,7986,8090,8191,8290,8387,8480,8571,
    8660,8746,8829,8910,8988,9063,9135,9205,9272,9336,
    9397,9455,9511,9563,9613,9659,9703,9744,9781,9816,
    9848,9877,9903,9926,9945,9962,9976,9986,9994,9998,
    10000
#endif
};
FT16 Qsin(FTU16 Angle)
{
    if (Angle >= 0 && Angle <= 90) {
        return sintbl[Angle];
    } else if (Angle > 90 && Angle <= 180) {
        return sintbl[90 - (Angle-90)];
    } else if (Angle > 180 && Angle <= 270) {
        return 0 - sintbl[Angle-180];
    } else if (Angle > 270 && Angle <= 360) {
        return 0 - sintbl[90 - (Angle-270)];
    } else {
        return 0;
    }
}
FT16 Qcos(FTU16 Angle)
{
    if (Angle >= 0 && Angle+90 <= 360) {
        return Qsin(Angle+90);
    } else if (Angle+90 > 360 && Angle <= 360) {
        return Qsin(90 + (Angle - 360));
    } else {
        return 1; 
    }
}
FTVOID getXYfrmCenter(FT16 Angle, FT16 *pX, FT16 *pY)
{
#if defined(FAST_SIN)
    *pX = *pX - (FT16)(((FT64)NDL_RADIUS*Qsin(Angle)) >> 13);
    *pY = *pY + (FT16)(((FT64)NDL_RADIUS*Qcos(Angle)) >> 13);
#else
    *pX = *pX - NDL_RADIUS*Qsin(Angle)/10000;
    *pY = *pY + NDL_RADIUS*Qcos(Angle)/10000;
#endif
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
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

    HAL_CmdBufIn(BLEND_FUNC(DST_ALPHA, ONE_MINUS_DST_ALPHA));
    HAL_CmdBufIn(COLOR_MASK(1,0,0,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 2));
    HAL_CmdBufIn(BITMAP_HANDLE(hdl));
    HAL_CmdBufIn(CELL(cell));
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

    HAL_CmdBufIn(COLOR_MASK(0,1,0,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 1));
    HAL_CmdBufIn(BITMAP_HANDLE(hdl));
    HAL_CmdBufIn(CELL(cell));
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));

    HAL_CmdBufIn(COLOR_MASK(0,0,1,0));
    HAL_CmdBufIn(PALETTE_SOURCE(PalSrc + 0));
    HAL_CmdBufIn(BITMAP_HANDLE(hdl));
    HAL_CmdBufIn(CELL(cell));
    HAL_CmdBufIn(VERTEX2F(X*EVE_PIXEL_UNIT,Y*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(RESTORE_CONTEXT());
}

FTVOID ally_modeb (FTU32 para)
{
    static FTU8 flag = 0;
    static FTU32 i=NDL_START;
    FT16 Nx=NDL_X, Ny=NDL_Y;

    /* only load the file once */
    if (flag == 0) {
        /* load bitmap resources data into EVE */
        if(APP_OK != appBmpToRamG(HDL_START, RAM_G, bmp_header, RAW_NUM)){
            DBGPRINT;
            return;
        }

        HAL_CmdBufIn(CMD_DLSTART);
        HAL_CmdBufIn(BITMAP_HANDLE(0));
        HAL_CmdBufIn(BITMAP_SIZE(BILINEAR,BORDER,BORDER,bmp_header[0].high*2,bmp_header[0].high*2));
        HAL_CmdBufIn(BITMAP_SIZE_H((bmp_header[0].high*2)>>9,(bmp_header[0].high*2)>>9));
        HAL_CmdBufIn(DISPLAY());
        HAL_CmdBufIn(CMD_SWAP);
        HAL_BufToReg(RAM_CMD,0);

        flag = 1;
    }

    HAL_CmdBufIn(CMD_DLSTART);
    HAL_CmdBufIn(CLEAR_COLOR_RGB(255,255,255));
    HAL_CmdBufIn(CLEAR(1,1,1));

    HAL_CmdBufIn(BEGIN(BITMAPS));

    /* display background */
    dispPal8(0,0,bmp_header[1].lut_src, 1, 0);

    /* display rotated needle */
    HAL_CmdBufIn(SAVE_CONTEXT());
    CoCmd_LOADIDENTITY;
    CoCmd_TRANSLATE((bmp_header[0].high)*EVE_TRANSFORM_MAX, 
            (bmp_header[0].high)*EVE_TRANSFORM_MAX);
    CoCmd_ROTATE(i*EVE_TRANSFORM_MAX/360);
    CoCmd_TRANSLATE((-1)*(bmp_header[0].wide/2)*EVE_TRANSFORM_MAX,0);
    CoCmd_SETMATRIX;

    getXYfrmCenter(i,&Nx,&Ny);
    Nx -= bmp_header[0].high;
    Ny -= bmp_header[0].high;
    dispPal8(Nx,Ny,bmp_header[0].lut_src, 0, 0);
    HAL_CmdBufIn(RESTORE_CONTEXT()); 

    HAL_CmdBufIn(END());
    HAL_CmdBufIn(DISPLAY());
    HAL_CmdBufIn(CMD_SWAP);

    HAL_BufToReg(RAM_CMD,0);

    /* needle auto running */
    if (i >= NDL_END) {
        flag = 2;
    } else if (i <= NDL_START) {
        flag = 1; 
    }

    if (flag == 1) {
        i++; 
    } else if (flag == 2) {
        i--;
    }

    FTDELAY(20);
    appGP.appIndex = 0;
}

AppFunc APPS_UI[] = {
    ally_modeb
};

