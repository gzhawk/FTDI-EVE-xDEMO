/* 
    Sample code to show how to demostrate a E-Car dashboard
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2019/Jan
*/

#if defined(VC_EMULATOR)
#error "copy res/ecar/eveflh.bin to res/eveflh, then comment this line"
#elif defined(VC_MPSSE) || defined(VC_FT4222)
#error "program res/ecar/eveflh.bin to eve-connected-flash, then comment this line"
#endif

#define ECAR_BOOT        "EVEFLH@4096:122880"
#define ECAR_BG          "EVEFLH@9220096 : 384000"
#define ECAR_NEEDLE_0    "EVEFLH@9604096 : 832"
#define ECAR_NEEDLE_1    "EVEFLH@9604928 : 960"
#define ECAR_NUM_C       "EVEFLH@9605888 : 3584"
#define ECAR_NUM_C_      "EVEFLH@9609472 : 320"
#define ECAR_NUM_D       "EVEFLH@9609792 : 7104"
#define ECAR_NUM_D_      "EVEFLH@9616896 : 512"
#define ECAR_OVERSPEED_B "EVEFLH@9617408 : 44736"
#define ECAR_SPEED_B     "EVEFLH@9662144 : 12096"
#define ECAR_SPEED_B_    "EVEFLH@9674240 : 512"
#define ECAR_SPEED_N     "EVEFLH@9674752 : 7232"
#define ECAR_SPEED_N_    "EVEFLH@9681984 : 64"
#define ECAR_WARNING     "EVEFLH@9682048 : 4800"
#define ECAR_XPATCH      "EVEFLH@9686848 : 960"

#define ECAR_WIDTH          800
#define ECAR_HIGH           480

#define ECAR_BOOT_ADDR      4096
#define ECAR_BOOT_SLEN      122880
#define ECAR_BOOT_FMAX      75

#define ECAR_RAM_BOOT       RAM_G
#define ECAR_RAM_DASHBOARD  RAM_G

#define ECAR_NEEDLE_0_W     16
#define ECAR_NEEDLE_0_H     52
#define ECAR_NEEDLE_1_W     12
#define ECAR_NEEDLE_1_H     76

#define ECAR_OVERSPEED_W    228
#define ECAR_OVERSPEED_H    196
#define ECAR_OVERSPEED_SPD  120

#define ECAR_NUM_C_W        14
#define ECAR_NUM_C_H        23//23*11
#define ECAR_NUM_D_W        21
#define ECAR_NUM_D_H        26//26*13

#define ECAR_SPEED_B_W      48
#define ECAR_SPEED_B_H      28//28*9
#define ECAR_SPEED_N_W      40
#define ECAR_SPEED_N_H      20//20*9

#define ECAR_WARNING_N_W    40
#define ECAR_WARNING_N_H    40//40*3

#define ECAR_XPATCH_W       40
#define ECAR_XPATCH_H       24

#define ECAR_MIN_SPEED      0
#define ECAR_MIN_PWR        (-25)
#define ECAR_MIN_TRPM       (-10)
#define ECAR_MIN_TKM        0
#define ECAR_MIN_AKM        0
#define ECAR_MIN_BKM        0
#define ECAR_MIN_VOT        0
#define ECAR_MIN_AMP        (-200)

#define ECAR_MAX_SPEED      160
#define ECAR_MAX_PWR        125
#define ECAR_MAX_TRPM       20
#define ECAR_MAX_TKM        999999
#define ECAR_MAX_AKM        999999
#define ECAR_MAX_BKM        999999
#define ECAR_MAX_VOT        200
#define ECAR_MAX_AMP        200

#define ECAR_MARK_NUM       9

#define ECAR_WARNING_SPD    80

#define ECAR_AB_SPD         80

typedef enum ECAR_HDL_ {
    ECAR_HDL_BOOT = 0,
    ECAR_HDL_BG   = 0, //in new screen, reuse handle 0
    ECAR_HDL_NEEDLE_0,
    ECAR_HDL_NEEDLE_1,
    ECAR_HDL_NUM_C,
    ECAR_HDL_NUM_D,
    ECAR_HDL_OVERSPEED_B,
    ECAR_HDL_SPEED_B,
    ECAR_HDL_SPEED_N,
    ECAR_HDL_WARNING,
    ECAR_HDL_XPATCH,
    ECAR_HDL_MAX,
}ECAR_HDL_E;

ImgInfo_st info_header[] = {
    {ECAR_BOOT,0,0,0},
    {ECAR_BG,0,0,0},
    {ECAR_NEEDLE_0,0,0,0},
    {ECAR_NEEDLE_1,0,0,0},
    {ECAR_OVERSPEED_B,0,0,0},
    {ECAR_WARNING,0,0,0},
    {ECAR_XPATCH,0,0,0},
};
ImgInfoPal_st info_pal_header[] = {
    {ECAR_NUM_C,ECAR_NUM_C_,0,0,0,0,0},
    {ECAR_NUM_D,ECAR_NUM_D_,0,0,0,0,0},
    {ECAR_SPEED_B,ECAR_SPEED_B_,0,0,0,0,0},
    {ECAR_SPEED_N,ECAR_SPEED_N_,0,0,0,0,0},
};
bmpHDR_st bmp_header[] = {
    {COMPRESSED_RGBA_ASTC_10x5_KHR,ECAR_WIDTH,ECAR_HIGH,(FTU32)&info_header[0]},
    {COMPRESSED_RGBA_ASTC_4x4_KHR, ECAR_WIDTH,ECAR_HIGH,(FTU32)&info_header[1]},
    {COMPRESSED_RGBA_ASTC_4x4_KHR, ECAR_NEEDLE_0_W,ECAR_NEEDLE_0_H,(FTU32)&info_header[2]},
    {COMPRESSED_RGBA_ASTC_4x4_KHR, ECAR_NEEDLE_1_W,ECAR_NEEDLE_1_H,(FTU32)&info_header[3]},
    {PALETTED4444, ECAR_NUM_C_W,ECAR_NUM_C_H,(FTU32)&info_pal_header[0]},
    {PALETTED4444, ECAR_NUM_D_W,ECAR_NUM_D_H,(FTU32)&info_pal_header[1]},
    {COMPRESSED_RGBA_ASTC_4x4_KHR, ECAR_OVERSPEED_W,ECAR_OVERSPEED_H,(FTU32)&info_header[4]},
    {PALETTED4444, ECAR_SPEED_B_W,ECAR_SPEED_B_H,(FTU32)&info_pal_header[2]},
    {PALETTED4444, ECAR_SPEED_N_W,ECAR_SPEED_N_H,(FTU32)&info_pal_header[3]},
    {COMPRESSED_RGBA_ASTC_4x4_KHR, ECAR_WARNING_N_W,ECAR_WARNING_N_H,(FTU32)&info_header[5]},
    {COMPRESSED_RGBA_ASTC_4x4_KHR, ECAR_XPATCH_W,ECAR_XPATCH_H,(FTU32)&info_header[6]},
};
typedef enum ECAR_INX_ {
    ECAR_INX_BOOT = 0,
    ECAR_INX_BG,
    ECAR_INX_NEEDLE_0,
    ECAR_INX_NEEDLE_1,
    ECAR_INX_NUM_C,
    ECAR_INX_NUM_D,
    ECAR_INX_OVERSPEED_B,
    ECAR_INX_SPEED_B,
    ECAR_INX_SPEED_N,
    ECAR_INX_WARNING,
    ECAR_INX_XPATCH,
    ECAR_INX_MAX,
}ECAR_INX_E;
typedef struct spd_mark_ {
    FTU8 spd_n1_low;  //normal size range before big size range
    FTU8 spd_n1_high;
    FTU8 spd_b_low;   //big size range
    FTU8 spd_b_high;
    FTU8 spd_n2_low;  //normal size range after big size range
    FTU8 spd_n2_high;
    FTU16 spd_x;
    FTU16 spd_y;
} spd_mark_st;
/* instaed of using 0xFF, you may also use 
   ECAR_MAX_SPEED+1 for better understanding */
spd_mark_st spd_mark[ECAR_MARK_NUM] = {
    {0xFF,0xFF,0,18,0xFF,0xFF, 266, 339}, //0
    {0,8,9,38,39,40,           247, 254}, //20
    {15,29,30,59,60,65,        264, 167}, //40
    {30,49,50,80,81,85,        316, 116}, //60
    {55,70,71,110,111,113,     390,  91}, //80
    {78,99,100,124,125,130,    459, 112}, //100
    {99,119,120,141,142,144,   507, 172}, //120
    {119,132,133,159,0xFF,0xFF,528, 255}, //140
    {140,154,155,160,0xFF,0xFF,498, 335}, //160
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
};
FT16 ecar_Qsin(FTU16 Angle)
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
FT16 ecar_Qcos(FTU16 Angle)
{
    if (Angle >= 0 && Angle+90 <= 360) {
        return ecar_Qsin(Angle+90);
    } else if (Angle+90 > 360 && Angle <= 360) {
        return ecar_Qsin(90 + (Angle - 360));
    } else {
        return 1; 
    }
}
FTVOID ecar_getXYfrmCenter(FT16 Angle, FT16 Radius, FT16 *pX, FT16 *pY)
{
    *pX = *pX - ((Radius*ecar_Qsin(Angle)) >> 13);
    *pY = *pY + ((Radius*ecar_Qcos(Angle)) >> 13);
}

FTU8 ecar_boot_frame(FTU32 *pframe, FTU32 max)
{
    if (*pframe >= max-1) {
        return 1;
    } else {
        *pframe = *pframe + 1;
    }

    info_header->addr = ECAR_BOOT_ADDR + *pframe * ECAR_BOOT_SLEN;
    /* keep pumpping the data into the GRAM */
    appEVEFLHToEVE(info_header->addr, ECAR_RAM_BOOT, ECAR_BOOT_SLEN);
    
    return 0;
}

FTVOID ecar_boot(FTU32 para)
{
	static FTU8 first = 1;
	static FTU32 c = 1;

	if (first) {
		if(APP_OK != appBmpToRamG(ECAR_HDL_BG, ECAR_RAM_BOOT, bmp_header, 1)){
			DBGPRINT;
			return;
		}

        HAL_CmdBufIn(CMD_DLSTART);
        HAL_CmdBufIn(CLEAR_COLOR_RGB(255, 255, 255));
        HAL_CmdBufIn(CLEAR(1, 1, 1));

        HAL_CmdBufIn(BEGIN(BITMAPS));
        HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_BOOT));
        HAL_CmdBufIn(CELL(0));

        /* display one frame */
        HAL_CmdBufIn(VERTEX2F(0,0));


        HAL_CmdBufIn(END());
        HAL_CmdBufIn(DISPLAY());
        HAL_CmdBufIn(CMD_SWAP);

        HAL_BufToReg(RAM_CMD, 0);

		first = 0;
	} else {
        /* change the frame index to make it 'animation' */
        if (ecar_boot_frame(&c, ECAR_BOOT_FMAX)) {
            appClnScrn(0,0,0);
            /* boot process finished, go next */
	        appGP.appIndex = 1;
        } else {
            /* stay in boot process */
            appGP.appIndex = 0;
        }
        /* 
         the delay has two function
         1. for the delay of each frame (you may use other way to do it)
         2. for the delay of appClnScrn (occasionally blurred screen be shown without delay)
         */
        FTDELAY(100);
    }
}

FTVOID ecar_background(FTVOID)
{
    HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_BG));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(VERTEX2F(0,0));
}

FTVOID ecar_number_tkm(FT32 num)
{
#define NUM_TKM_X 430
#define NUM_TKM_Y 382
    FT32 tmp = num, x = NUM_TKM_X;

    HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_NUM_D));
    HAL_CmdBufIn(PALETTE_SOURCE(((ImgInfoPal_st *)(bmp_header[ECAR_INX_NUM_D].info))->addr_lut));
    
    if (tmp == 0) {
        HAL_CmdBufIn(CELL(0));
        HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT, NUM_TKM_Y*EVE_PIXEL_UNIT));
        return;
    }

    while (tmp) {
        HAL_CmdBufIn(CELL(tmp%10));
        HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT, NUM_TKM_Y*EVE_PIXEL_UNIT));
        tmp = tmp/10;
        x -= (bmp_header[ECAR_INX_NUM_D].wide - 3);
    }
}

FTVOID ecar_number_abkm(FT32 num, FTU8 AorB)
{
#define NUM_ABMK_X 324
#define NUM_ABKM_X 430
#define NUM_ABKM_Y 409
    FT32 tmp = num, x = NUM_ABKM_X;

    HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_NUM_D));
    HAL_CmdBufIn(PALETTE_SOURCE(((ImgInfoPal_st *)(bmp_header[ECAR_INX_NUM_D].info))->addr_lut));
   
    HAL_CmdBufIn(CELL(tmp%10));
    HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT, NUM_ABKM_Y*EVE_PIXEL_UNIT));
    x -= (bmp_header[ECAR_INX_NUM_D].wide - 15);
    tmp = tmp/10;
    HAL_CmdBufIn(CELL(12));
    HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT, NUM_ABKM_Y*EVE_PIXEL_UNIT));

    do {
        x -= (bmp_header[ECAR_INX_NUM_D].wide - 3);
        HAL_CmdBufIn(CELL(tmp%10));
        HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT, NUM_ABKM_Y*EVE_PIXEL_UNIT));
        tmp = tmp/10;
    } while (tmp);

    if (AorB) {
        HAL_CmdBufIn(CELL(11));
    } else {
        HAL_CmdBufIn(CELL(10));
    }
    HAL_CmdBufIn(VERTEX2F(NUM_ABMK_X*EVE_PIXEL_UNIT, NUM_ABKM_Y*EVE_PIXEL_UNIT));
}
FTVOID ecar_number_vot(FT32 vot)
{
#define NUM_VOT_X 405
#define NUM_VOT_Y 194
    FT32 tmp = vot, x = NUM_VOT_X;

    HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_NUM_C));
    HAL_CmdBufIn(PALETTE_SOURCE(((ImgInfoPal_st *)(bmp_header[ECAR_INX_NUM_C].info))->addr_lut));
    
    if (tmp == 0) {
        HAL_CmdBufIn(CELL(0));
        HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT, NUM_VOT_Y*EVE_PIXEL_UNIT));
        return;
    }

    while (tmp) {
        HAL_CmdBufIn(CELL(tmp%10));
        HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT, NUM_VOT_Y*EVE_PIXEL_UNIT));
        tmp = tmp/10;
        x -= bmp_header[ECAR_INX_NUM_C].wide;
    }
}
FTVOID ecar_number_amp(FT32 amp)
{
#define NUM_AMP_X 405
#define NUM_AMP_Y 294
    FT32 tmp = amp, x = NUM_AMP_X, n = 0;

    HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_NUM_C));
    HAL_CmdBufIn(PALETTE_SOURCE(((ImgInfoPal_st *)(bmp_header[ECAR_INX_NUM_C].info))->addr_lut));
    
    if (tmp == 0) {
        HAL_CmdBufIn(CELL(0));
        HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT, NUM_AMP_Y*EVE_PIXEL_UNIT));
        return;
    }

    if (tmp < 0) {
        n = 1;
        tmp = tmp * (-1);
    }

    while (tmp) {
        HAL_CmdBufIn(CELL(tmp%10));
        HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT, NUM_AMP_Y*EVE_PIXEL_UNIT));
        tmp = tmp/10;
        x -= bmp_header[ECAR_INX_NUM_C].wide;
    }

    if (n) {
        HAL_CmdBufIn(CELL(10));
        HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT, NUM_AMP_Y*EVE_PIXEL_UNIT));
    }
}
FTVOID ecar_speed(FT16 spd)
{
    FTU8 i;
  
    for (i = 0; i < ECAR_MARK_NUM; i++) {
        if ((spd >= spd_mark[i].spd_n1_low && spd <= spd_mark[i].spd_n1_high) || 
            (spd >= spd_mark[i].spd_n2_low && spd <= spd_mark[i].spd_n2_high)) {
            HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_SPEED_N));
            HAL_CmdBufIn(PALETTE_SOURCE(((ImgInfoPal_st *)(bmp_header[ECAR_INX_SPEED_N].info))->addr_lut));
            HAL_CmdBufIn(CELL(i));
            HAL_CmdBufIn(VERTEX2F(spd_mark[i].spd_x*EVE_PIXEL_UNIT,spd_mark[i].spd_y*EVE_PIXEL_UNIT));
        }
        
        if (spd >= spd_mark[i].spd_b_low && spd <= spd_mark[i].spd_b_high) {
            HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_SPEED_B));
            HAL_CmdBufIn(PALETTE_SOURCE(((ImgInfoPal_st *)(bmp_header[ECAR_INX_SPEED_B].info))->addr_lut));
            HAL_CmdBufIn(CELL(i));
            HAL_CmdBufIn(VERTEX2F(spd_mark[i].spd_x*EVE_PIXEL_UNIT,spd_mark[i].spd_y*EVE_PIXEL_UNIT));
        }
    }
}

static FTU32  ecar_spd2agl(FT16 spd)
{
#define SPEED2ANGLE_START (56)
#define SPEED2ANGLE_END   (302)
#define SPEED2ANGLE_RANGE (ECAR_MAX_SPEED - ECAR_MIN_SPEED)
/*
    speed from 0 ~ 160
    angle from start ~ end
    it's an internal function
    the speed would not exceed 160
    so not considering the 360 border
 */
    return (SPEED2ANGLE_START + spd*(SPEED2ANGLE_END-SPEED2ANGLE_START)/SPEED2ANGLE_RANGE);
}
FTVOID ecar_needle_speed(FT16 speed)
{
#define NEEDLE_SPD_R        (160)
#define NEEDLE_SPD_CENTER_X (10)
#define NEEDLE_SPD_CENTER_Y (5)
#define NEEDLE_SPD_OFFSET   (bmp_header[ECAR_INX_NEEDLE_0].high)

    FT16 x = (EVE_LCD_WIDTH/2-10)-NEEDLE_SPD_OFFSET,
         y = (EVE_LCD_HIGH/2+10)-NEEDLE_SPD_OFFSET,
         agl = ecar_spd2agl(speed);

    HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_NEEDLE_0));
    HAL_CmdBufIn(CELL(0));
    
    HAL_CmdBufIn(SAVE_CONTEXT());
    CoCmd_LOADIDENTITY;
    /* have to do the offset to make it fully display */
    CoCmd_TRANSLATE(NEEDLE_SPD_OFFSET*EVE_TRANSFORM_MAX, 
                    NEEDLE_SPD_OFFSET*EVE_TRANSFORM_MAX);
    CoCmd_ROTATEAROUND(NEEDLE_SPD_CENTER_X,
                       NEEDLE_SPD_CENTER_Y,
                       agl*EVE_TRANSFORM_MAX/360,EVE_TRANSFORM_MAX);
    CoCmd_SETMATRIX;
    
    ecar_getXYfrmCenter(agl,NEEDLE_SPD_R,&x,&y);

    HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT,y*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(RESTORE_CONTEXT());
}
static FTU32  ecar_pwr2agl(FT16 pwr)
{
#define POWER2ANGLE_START (56)
#define POWER2ANGLE_END   (132)
#define POWER2ANGLE_RANGE (ECAR_MAX_PWR - ECAR_MIN_PWR)
    return (POWER2ANGLE_START + (pwr - ECAR_MIN_PWR)*(POWER2ANGLE_END-POWER2ANGLE_START)/POWER2ANGLE_RANGE);
}

FTVOID ecar_needle_power(FT16 power)
{
#define NEEDLE_PWR_R        (260)
#define NEEDLE_PWR_CENTER_X (6)
#define NEEDLE_PWR_CENTER_Y (5)
#define NEEDLE_PWR_OFFSET   (bmp_header[ECAR_INX_NEEDLE_1].high)

    FT16 x = (EVE_LCD_WIDTH/2-10)-NEEDLE_PWR_OFFSET,
         y = (EVE_LCD_HIGH/2+10)-NEEDLE_PWR_OFFSET,
         agl = ecar_pwr2agl(power);

    HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_NEEDLE_1));
    HAL_CmdBufIn(CELL(0));
    
    HAL_CmdBufIn(SAVE_CONTEXT());
    CoCmd_LOADIDENTITY;
    /* have to do the offset to make it fully display */
    CoCmd_TRANSLATE(NEEDLE_PWR_OFFSET*EVE_TRANSFORM_MAX, 
                    NEEDLE_PWR_OFFSET*EVE_TRANSFORM_MAX);
    CoCmd_ROTATEAROUND(NEEDLE_PWR_CENTER_X,
                       NEEDLE_PWR_CENTER_Y,
                       agl*EVE_TRANSFORM_MAX/360,EVE_TRANSFORM_MAX);
    CoCmd_SETMATRIX;
    
    ecar_getXYfrmCenter(agl,NEEDLE_PWR_R,&x,&y);

    HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT,y*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(RESTORE_CONTEXT());
}
static FTU32  ecar_trpm2agl(FT16 trpm)
{
#define TRPM2ANGLE_START (299)
#define TRPM2ANGLE_END   (228)
#define TRPM2ANGLE_RANGE (ECAR_MAX_TRPM - ECAR_MIN_TRPM)
    FTU16 move = (trpm - ECAR_MIN_TRPM);

    if (move < 0) {
        move *= (-1);
    }
    return (TRPM2ANGLE_START - move*(TRPM2ANGLE_START-TRPM2ANGLE_END)/TRPM2ANGLE_RANGE);
}

FTVOID ecar_needle_trpm(FT16 trpm)
{
#define NEEDLE_TRPM_R        (260)
#define NEEDLE_TRPM_CENTER_X (6)
#define NEEDLE_TRPM_CENTER_Y (5)
#define NEEDLE_TRPM_OFFSET   (bmp_header[ECAR_INX_NEEDLE_1].high)

    FT16 x = (EVE_LCD_WIDTH/2-10)-NEEDLE_TRPM_OFFSET,
         y = (EVE_LCD_HIGH/2+10)-NEEDLE_TRPM_OFFSET,
         agl = ecar_trpm2agl(trpm);

    HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_NEEDLE_1));
    HAL_CmdBufIn(CELL(0));
    
    HAL_CmdBufIn(SAVE_CONTEXT());
    CoCmd_LOADIDENTITY;
    /* have to do the offset to make it fully display */
    CoCmd_TRANSLATE(NEEDLE_PWR_OFFSET*EVE_TRANSFORM_MAX, 
                    NEEDLE_PWR_OFFSET*EVE_TRANSFORM_MAX);
    CoCmd_ROTATEAROUND(NEEDLE_PWR_CENTER_X,
                       NEEDLE_PWR_CENTER_Y,
                       agl*EVE_TRANSFORM_MAX/360,EVE_TRANSFORM_MAX);
    CoCmd_SETMATRIX;
    
    ecar_getXYfrmCenter(agl,NEEDLE_TRPM_R,&x,&y);

    HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT,y*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(RESTORE_CONTEXT());
}
FTVOID ecar_overspeed(FTVOID)
{
#define OVERSPEED_X 287
#define OVERSPEED_Y 141
    HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_OVERSPEED_B));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(VERTEX2F(OVERSPEED_X*EVE_PIXEL_UNIT,OVERSPEED_Y*EVE_PIXEL_UNIT));
}
FTVOID ecar_xpatch(FTVOID)
{
#define XPATCH_X 495
#define XPATCH_Y 332
    HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_XPATCH));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(VERTEX2F(XPATCH_X*EVE_PIXEL_UNIT,XPATCH_Y*EVE_PIXEL_UNIT));
}
FTVOID ecar_warning(FTVOID)
{
#define WARNING_1_X 49
#define WARNING_1_Y 16
#define WARNING_2_X 753
#define WARNING_2_Y 185
#define WARNING_3_X 32
#define WARNING_3_Y 422

    HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_WARNING));
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(VERTEX2F(WARNING_1_X*EVE_PIXEL_UNIT,WARNING_1_Y*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(CELL(1));
    HAL_CmdBufIn(VERTEX2F(WARNING_2_X*EVE_PIXEL_UNIT,WARNING_2_Y*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(CELL(2));
    HAL_CmdBufIn(VERTEX2F(WARNING_3_X*EVE_PIXEL_UNIT,WARNING_3_Y*EVE_PIXEL_UNIT));
}

FTVOID ecar_simulat(FT32 *p, FT32 min, FT32 max)
{
    if (TOUCHED) {
        if (max > *p) {
            *p = *p + 1;
        }
    } else {
        if (min < *p) {
            *p = *p - 1;
        } 
    }
}

FTVOID ecar_dashboard(FTU32 para)
{
	static FT32 load = 1, 
                speed = ECAR_MIN_SPEED, 
                power = ECAR_MIN_PWR, 
                trpm = ECAR_MIN_TRPM,
                tkm = ECAR_MIN_TKM,
                akm = ECAR_MIN_AKM,
                bkm = ECAR_MIN_BKM,
                vot = ECAR_MIN_VOT,
                amp = ECAR_MIN_AMP;

	if (load) {
		if(APP_OK != appBmpToRamG(ECAR_HDL_BOOT, ECAR_RAM_DASHBOARD, &bmp_header[1], ECAR_HDL_MAX)){
			DBGPRINT;
			return;
		}

        appBilinearModify(ECAR_HDL_NEEDLE_0, bmp_header[ECAR_INX_NEEDLE_0].high*2);
        appBilinearModify(ECAR_HDL_NEEDLE_1, bmp_header[ECAR_INX_NEEDLE_1].high*2);
        load = 0;
	}
    HAL_CmdBufIn(CMD_DLSTART);
    HAL_CmdBufIn(CLEAR_COLOR_RGB(0, 0, 0));
    HAL_CmdBufIn(CLEAR(1, 1, 1));

    HAL_CmdBufIn(BEGIN(BITMAPS));

    ecar_background();
 
    ecar_number_tkm(tkm);

    if (speed > ECAR_AB_SPD) {
        ecar_number_abkm(bkm,1);
        ecar_simulat(&bkm, ECAR_MIN_BKM, ECAR_MAX_BKM);
    } else {
        ecar_number_abkm(akm,0);
        ecar_simulat(&akm, ECAR_MIN_AKM, ECAR_MAX_AKM);
    }

    if (speed > ECAR_WARNING_SPD) {
        ecar_warning();
    }

    if (speed > ECAR_OVERSPEED_SPD) {
        ecar_overspeed();
        ecar_xpatch();
    } else {
        ecar_number_vot(vot); 
        ecar_number_amp(amp);
    }
   
    ecar_speed(speed);
    ecar_needle_speed(speed);
    ecar_needle_power(power);
	ecar_needle_trpm(trpm);

    HAL_CmdBufIn(END());
    HAL_CmdBufIn(DISPLAY());
    HAL_CmdBufIn(CMD_SWAP);
    HAL_BufToReg(RAM_CMD, 0);

    ecar_simulat(&speed, ECAR_MIN_SPEED, ECAR_MAX_SPEED);
    ecar_simulat(&power, ECAR_MIN_PWR, ECAR_MAX_PWR);
    ecar_simulat(&trpm, ECAR_MIN_TRPM, ECAR_MAX_TRPM);
    ecar_simulat(&tkm, ECAR_MIN_TKM, ECAR_MAX_TKM);
    ecar_simulat(&vot, ECAR_MIN_VOT, ECAR_MAX_VOT);
    ecar_simulat(&amp, ECAR_MIN_AMP, ECAR_MAX_AMP);
    /* stay in dashboard process */
	appGP.appIndex = 1;
}

AppFunc APPS_UI[] = {
	ecar_boot,
    ecar_dashboard,
};

