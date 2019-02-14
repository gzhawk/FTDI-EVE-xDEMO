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

/* 
 it just for easy debug/implimentable of your evaluation
 once found the proper value in your system 
 recommanded to use calculated fixed number
 less multiple/division, better performance
 */
#define ECAR_PRECISE_SPEED  2
#define ECAR_PRECISE_PWR    2
#define ECAR_PRECISE_TRPM   10
/* 
 the division of 1 degree
 you MUST have the correct corresponding "sintbl"
*/
#define ECAR_PRECISE_CIRCLE 4

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
#define ECAR_OVERSPEED_SPD  (120*ECAR_PRECISE_SPEED)

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

#define ECAR_MIN_SPEED      (0*ECAR_PRECISE_SPEED)
#define ECAR_MIN_PWR        (-25*ECAR_PRECISE_PWR)
#define ECAR_MIN_TRPM       (-10*ECAR_PRECISE_TRPM)
#define ECAR_MIN_TKM        0
#define ECAR_MIN_AKM        0
#define ECAR_MIN_BKM        0
#define ECAR_MIN_VOT        0
#define ECAR_MIN_AMP        (-200)

#define ECAR_MAX_SPEED      (160*ECAR_PRECISE_SPEED)
#define ECAR_MAX_PWR        (125*ECAR_PRECISE_PWR)
#define ECAR_MAX_TRPM       (20*ECAR_PRECISE_TRPM)
#define ECAR_MAX_TKM        999999
#define ECAR_MAX_AKM        999999
#define ECAR_MAX_BKM        999999
#define ECAR_MAX_VOT        200
#define ECAR_MAX_AMP        200

#define ECAR_MARK_NUM       9

#define ECAR_WARNING_SPD    (80*ECAR_PRECISE_SPEED)

#define ECAR_AB_SPD         (80*ECAR_PRECISE_SPEED)

typedef enum ECAR_HDL_ { //bitmap handle
    ECAR_HDL_BOOT = 0,
    ECAR_HDL_BG   = 0,   //background
                         //in new screen, reuse handle 0
    ECAR_HDL_NEEDLE_0,   //speed needle
    ECAR_HDL_NEEDLE_1,   //power/trpm needle
    ECAR_HDL_NUM_C,      //number in the center (V/A) of dashboard
    ECAR_HDL_NUM_D,      //number in the down (A/B/Total trip) dashboard
    ECAR_HDL_OVERSPEED_B,//overspeed waning display in center
    ECAR_HDL_SPEED_B,    //speed number in big size
    ECAR_HDL_SPEED_N,    //speed number in normal size
    ECAR_HDL_WARNING,    //3 warning icon
    ECAR_HDL_XPATCH,     //cover the km/h mark when speed over 160km/h
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
/* 
 1. angle unit: 1/ECAR_PRECISE_CIRCLE degree
 2. the original sin data multiple 10000 (e.g. sin(1) = 0.0174)
    so the calulated result need to div 10000, in order to 
    make the calulation faster for MCU (bit shift faster than div)
    div the original sin data by 1.22 to come out a new sin table
    then the result only need to right shift 13 bits
    (2^13=8192, 10000/8192=1.22) 
 3. there is an easy way to generate the table by using EXCEL
    you may search it online, the way I'm using is
    by using this formular "ROUND(SIN(RADIANS(A1))/1.22*10000,0)"
    "A1" the cell which content the "angle" value
 4. in MCU, you may put this table in Flash instead of memory
    as long as your system has a fast accessing Flash mechanism
 */
FTU16 sintbl[] = 
{
0	,	36	,	72	,	107	,
143	,	179	,	215	,	250	,
286	,	322	,	358	,	393	,
429	,	465	,	500	,	536	,
572	,	607	,	643	,	679	,
714	,	750	,	786	,	821	,
857	,	892	,	928	,	963	,
999	,	1034	,	1070	,	1105	,
1141	,	1176	,	1212	,	1247	,
1282	,	1318	,	1353	,	1388	,
1423	,	1459	,	1494	,	1529	,
1564	,	1599	,	1634	,	1669	,
1704	,	1739	,	1774	,	1809	,
1844	,	1879	,	1913	,	1948	,
1983	,	2018	,	2052	,	2087	,
2121	,	2156	,	2190	,	2225	,
2259	,	2294	,	2328	,	2362	,
2396	,	2431	,	2465	,	2499	,
2533	,	2567	,	2601	,	2635	,
2669	,	2702	,	2736	,	2770	,
2803	,	2837	,	2871	,	2904	,
2937	,	2971	,	3004	,	3037	,
3071	,	3104	,	3137	,	3170	,
3203	,	3236	,	3268	,	3301	,
3334	,	3367	,	3399	,	3432	,
3464	,	3496	,	3529	,	3561	,
3593	,	3625	,	3657	,	3689	,
3721	,	3753	,	3785	,	3817	,
3848	,	3880	,	3911	,	3943	,
3974	,	4005	,	4036	,	4067	,
4098	,	4129	,	4160	,	4191	,
4222	,	4252	,	4283	,	4313	,
4344	,	4374	,	4404	,	4434	,
4464	,	4494	,	4524	,	4554	,
4584	,	4613	,	4643	,	4672	,
4701	,	4731	,	4760	,	4789	,
4818	,	4847	,	4876	,	4904	,
4933	,	4961	,	4990	,	5018	,
5046	,	5075	,	5103	,	5131	,
5158	,	5186	,	5214	,	5241	,
5269	,	5296	,	5323	,	5350	,
5378	,	5404	,	5431	,	5458	,
5485	,	5511	,	5538	,	5564	,
5590	,	5616	,	5642	,	5668	,
5694	,	5720	,	5745	,	5771	,
5796	,	5821	,	5846	,	5871	,
5896	,	5921	,	5946	,	5970	,
5995	,	6019	,	6043	,	6067	,
6091	,	6115	,	6139	,	6163	,
6186	,	6210	,	6233	,	6256	,
6279	,	6302	,	6325	,	6347	,
6370	,	6392	,	6415	,	6437	,
6459	,	6481	,	6503	,	6525	,
6546	,	6568	,	6589	,	6610	,
6631	,	6652	,	6673	,	6694	,
6714	,	6735	,	6755	,	6775	,
6795	,	6815	,	6835	,	6855	,
6874	,	6894	,	6913	,	6932	,
6951	,	6970	,	6989	,	7007	,
7026	,	7044	,	7063	,	7081	,
7099	,	7116	,	7134	,	7152	,
7169	,	7186	,	7203	,	7220	,
7237	,	7254	,	7271	,	7287	,
7303	,	7319	,	7336	,	7351	,
7367	,	7383	,	7398	,	7414	,
7429	,	7444	,	7459	,	7473	,
7488	,	7503	,	7517	,	7531	,
7545	,	7559	,	7573	,	7586	,
7600	,	7613	,	7626	,	7639	,
7652	,	7665	,	7678	,	7690	,
7702	,	7715	,	7727	,	7738	,
7750	,	7762	,	7773	,	7784	,
7796	,	7807	,	7817	,	7828	,
7839	,	7849	,	7859	,	7869	,
7879	,	7889	,	7899	,	7908	,
7917	,	7927	,	7936	,	7945	,
7953	,	7962	,	7970	,	7979	,
7987	,	7995	,	8002	,	8010	,
8018	,	8025	,	8032	,	8039	,
8046	,	8053	,	8059	,	8066	,
8072	,	8078	,	8084	,	8090	,
8096	,	8101	,	8107	,	8112	,
8117	,	8122	,	8127	,	8131	,
8136	,	8140	,	8144	,	8148	,
8152	,	8155	,	8159	,	8162	,
8166	,	8169	,	8171	,	8174	,
8177	,	8179	,	8181	,	8184	,
8185	,	8187	,	8189	,	8190	,
8192	,	8193	,	8194	,	8195	,
8195	,	8196	,	8196	,	8197	,
8197							
};
FT16 ecar_Qsin(FTU16 Angle)
{
    if (Angle >= 0 && Angle <= 90*ECAR_PRECISE_CIRCLE) {
        return sintbl[Angle];
    } else if (Angle > 90*ECAR_PRECISE_CIRCLE && Angle <= 180*ECAR_PRECISE_CIRCLE) {
        return sintbl[90*ECAR_PRECISE_CIRCLE - (Angle-90*ECAR_PRECISE_CIRCLE)];
    } else if (Angle > 180*ECAR_PRECISE_CIRCLE && Angle <= 270*ECAR_PRECISE_CIRCLE) {
        return 0 - sintbl[Angle-180*ECAR_PRECISE_CIRCLE];
    } else if (Angle > 270*ECAR_PRECISE_CIRCLE && Angle < 360*ECAR_PRECISE_CIRCLE) {
        return 0 - sintbl[90*ECAR_PRECISE_CIRCLE - (Angle-270*ECAR_PRECISE_CIRCLE)];
    } else {
        return 0;
    }
}
FT16 ecar_Qcos(FTU16 Angle)
{
    if (Angle >= 0 && Angle+90*ECAR_PRECISE_CIRCLE < 360*ECAR_PRECISE_CIRCLE) {
        return ecar_Qsin(Angle+90*ECAR_PRECISE_CIRCLE);
    } else if (Angle+90*ECAR_PRECISE_CIRCLE > 360*ECAR_PRECISE_CIRCLE && Angle < 360*ECAR_PRECISE_CIRCLE) {
        return ecar_Qsin(90*ECAR_PRECISE_CIRCLE + (Angle - 360*ECAR_PRECISE_CIRCLE));
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
        if ((spd >= ECAR_PRECISE_SPEED*spd_mark[i].spd_n1_low && spd <= ECAR_PRECISE_SPEED*spd_mark[i].spd_n1_high) || 
            (spd >= ECAR_PRECISE_SPEED*spd_mark[i].spd_n2_low && spd <= ECAR_PRECISE_SPEED*spd_mark[i].spd_n2_high)) {
            HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_SPEED_N));
            HAL_CmdBufIn(PALETTE_SOURCE(((ImgInfoPal_st *)(bmp_header[ECAR_INX_SPEED_N].info))->addr_lut));
            HAL_CmdBufIn(CELL(i));
            HAL_CmdBufIn(VERTEX2F(spd_mark[i].spd_x*EVE_PIXEL_UNIT,spd_mark[i].spd_y*EVE_PIXEL_UNIT));
        }
        
        if (spd >= ECAR_PRECISE_SPEED*spd_mark[i].spd_b_low && spd <= ECAR_PRECISE_SPEED*spd_mark[i].spd_b_high) {
            HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_SPEED_B));
            HAL_CmdBufIn(PALETTE_SOURCE(((ImgInfoPal_st *)(bmp_header[ECAR_INX_SPEED_B].info))->addr_lut));
            HAL_CmdBufIn(CELL(i));
            HAL_CmdBufIn(VERTEX2F(spd_mark[i].spd_x*EVE_PIXEL_UNIT,spd_mark[i].spd_y*EVE_PIXEL_UNIT));
        }
    }
}
static FTU32  ecar_x2a(FT16 x, FT16 x_min, FT16 x_max, FT16 a_start, FT16 a_end)
{
    FT16 a = (x - x_min);

    if (a < 0) {
        a = (-1)*a;
    }

    a = a_start + a*(a_end-a_start)/(x_max - x_min);

    return (a >= 360*ECAR_PRECISE_CIRCLE)?0:a;
}
FTVOID ecar_needle_speed(FT16 speed)
{
#define NEEDLE_SPD_R        (160)
#define NEEDLE_SPD_CENTER_X (10)
#define NEEDLE_SPD_CENTER_Y (5)
#define NEEDLE_SPD_OFFSET   (bmp_header[ECAR_INX_NEEDLE_0].high)
#define SPEED2ANGLE_START   (56*ECAR_PRECISE_CIRCLE)
#define SPEED2ANGLE_END     (302*ECAR_PRECISE_CIRCLE)

    FT16 x = (EVE_LCD_WIDTH/2-10)-NEEDLE_SPD_OFFSET,
         y = (EVE_LCD_HIGH/2+10)-NEEDLE_SPD_OFFSET,
         agl = ecar_x2a(speed,ECAR_MIN_SPEED,ECAR_MAX_SPEED,SPEED2ANGLE_START,SPEED2ANGLE_END);

    HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_NEEDLE_0));
    HAL_CmdBufIn(CELL(0));
    
    HAL_CmdBufIn(SAVE_CONTEXT());
    CoCmd_LOADIDENTITY;
    /* have to do the offset to make it fully display */
    CoCmd_TRANSLATE(NEEDLE_SPD_OFFSET*EVE_TRANSFORM_MAX, 
                    NEEDLE_SPD_OFFSET*EVE_TRANSFORM_MAX);
    CoCmd_ROTATEAROUND(NEEDLE_SPD_CENTER_X,
                       NEEDLE_SPD_CENTER_Y,
                       agl*EVE_TRANSFORM_MAX/(360*ECAR_PRECISE_CIRCLE),EVE_TRANSFORM_MAX);
    CoCmd_SETMATRIX;
    
    ecar_getXYfrmCenter(agl,NEEDLE_SPD_R,&x,&y);

    HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT,y*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(RESTORE_CONTEXT());
}
FTVOID ecar_needle_power(FT16 power)
{
#define NEEDLE_PWR_R        (260)
#define NEEDLE_PWR_CENTER_X (6)
#define NEEDLE_PWR_CENTER_Y (5)
#define NEEDLE_PWR_OFFSET   (bmp_header[ECAR_INX_NEEDLE_1].high)
#define POWER2ANGLE_START   (56*ECAR_PRECISE_CIRCLE)
#define POWER2ANGLE_END     (132*ECAR_PRECISE_CIRCLE)

    FT16 x = (EVE_LCD_WIDTH/2-10)-NEEDLE_PWR_OFFSET,
         y = (EVE_LCD_HIGH/2+10)-NEEDLE_PWR_OFFSET,
         agl = ecar_x2a(power,ECAR_MIN_PWR,ECAR_MAX_PWR,POWER2ANGLE_START,POWER2ANGLE_END);

    HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_NEEDLE_1));
    HAL_CmdBufIn(CELL(0));
    
    HAL_CmdBufIn(SAVE_CONTEXT());
    CoCmd_LOADIDENTITY;
    /* have to do the offset to make it fully display */
    CoCmd_TRANSLATE(NEEDLE_PWR_OFFSET*EVE_TRANSFORM_MAX, 
                    NEEDLE_PWR_OFFSET*EVE_TRANSFORM_MAX);
    CoCmd_ROTATEAROUND(NEEDLE_PWR_CENTER_X,
                       NEEDLE_PWR_CENTER_Y,
                       agl*EVE_TRANSFORM_MAX/(360*ECAR_PRECISE_CIRCLE),EVE_TRANSFORM_MAX);
    CoCmd_SETMATRIX;
    
    ecar_getXYfrmCenter(agl,NEEDLE_PWR_R,&x,&y);

    HAL_CmdBufIn(VERTEX2F(x*EVE_PIXEL_UNIT,y*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(RESTORE_CONTEXT());
}
FTVOID ecar_needle_trpm(FT16 trpm)
{
#define NEEDLE_TRPM_R        (260)
#define NEEDLE_TRPM_CENTER_X (6)
#define NEEDLE_TRPM_CENTER_Y (5)
#define NEEDLE_TRPM_OFFSET   (bmp_header[ECAR_INX_NEEDLE_1].high)
#define TRPM2ANGLE_START     (299*ECAR_PRECISE_CIRCLE)
#define TRPM2ANGLE_END       (228*ECAR_PRECISE_CIRCLE)

    FT16 x = (EVE_LCD_WIDTH/2-10)-NEEDLE_TRPM_OFFSET,
         y = (EVE_LCD_HIGH/2+10)-NEEDLE_TRPM_OFFSET,
         agl = ecar_x2a(trpm,ECAR_MIN_TRPM,ECAR_MAX_TRPM,TRPM2ANGLE_START,TRPM2ANGLE_END);

    HAL_CmdBufIn(BITMAP_HANDLE(ECAR_HDL_NEEDLE_1));
    HAL_CmdBufIn(CELL(0));
    
    HAL_CmdBufIn(SAVE_CONTEXT());
    CoCmd_LOADIDENTITY;
    /* have to do the offset to make it fully display */
    CoCmd_TRANSLATE(NEEDLE_PWR_OFFSET*EVE_TRANSFORM_MAX, 
                    NEEDLE_PWR_OFFSET*EVE_TRANSFORM_MAX);
    CoCmd_ROTATEAROUND(NEEDLE_PWR_CENTER_X,
                       NEEDLE_PWR_CENTER_Y,
                       agl*EVE_TRANSFORM_MAX/(360*ECAR_PRECISE_CIRCLE),EVE_TRANSFORM_MAX);
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

