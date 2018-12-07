/* 
    Sample code to show how to display QRCode
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Aug
*/

#if defined(VC_EMULATOR)
#error "copy res/scan/eveflh.bin to res/eveflh, then comment this line"
#elif defined(VC_MPSSE) || defined(VC_FT4222)
#error "program res/scan/eveflh.bin to eve-connected-flash, then comment this line"
#endif

#define ANIM_ADDR    783552
#define ANIM_MAX     180 
#define ANIM_W       512
#define ANIM_H       512
#define QR_PATH      "ASTC_EVEFLH@785024"
#define QR_W         200
#define QR_H         200
#define QR_TXT_PATH  "ASTC_EVEFLH@825024"
#define QR_TXT_W     172
#define QR_TXT_H     24
#define COUNT        100
#define HDL_START    0

ImgInfo_st info_hdr[] = {
    {QR_PATH,    0,0,0},
    {QR_TXT_PATH,0,0,0},
};

bmpHDR_st bmp_hdr[] = {
    {COMPRESSED_RGBA_ASTC_4x4_KHR, QR_W,     QR_H,     (FTU32)&info_hdr[0]},
    {COMPRESSED_RGBA_ASTC_4x4_KHR, QR_TXT_W, QR_TXT_H, (FTU32)&info_hdr[1]},
};

FTVOID loop_frame(FTU32 *pframe, FTU32 max)
{
    if (*pframe >= max-1) {
        *pframe = 0;
    } else {
        *pframe = *pframe + 1;
    }
}
FTU8 load_resources(bmpHDR_st *p_res, FTU8 num)
{
    if (appEVEFLHSetFull()) {
        FTPRINT("\neveflh error occur");
        return 1;
    }

    if(appBmpToRamG(HDL_START, RAM_G, p_res, num)){
        FTPRINT("\nload bitmap error occur");
        return 1;
    }

    return 0;
}
FTVOID scan_1(FTU32 para)
{
    static FTU8 load = 1, press = 0, count = 0;
#if !defined(CAL_NEEDED)
    static FTU8 tick = 0;
#endif

    if (load) {
        if(load_resources(bmp_hdr, sizeof(bmp_hdr)/sizeof(bmpHDR_st))) {
            return;
        }
        load = 0;
    }

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255, 255, 255));
	HAL_CmdBufIn(CLEAR(1, 1, 1));
    
    HAL_CmdBufIn(BEGIN(BITMAPS));
    
    HAL_CmdBufIn(CELL(0));
    HAL_CmdBufIn(BITMAP_HANDLE(HDL_START));
    HAL_CmdBufIn(VERTEX2F((EVE_LCD_WIDTH/2 - QR_W/2)*EVE_PIXEL_UNIT,(EVE_LCD_HIGH/2 - QR_H/2)*EVE_PIXEL_UNIT));
    if (press) {
        HAL_CmdBufIn(BITMAP_HANDLE(HDL_START+1));
        HAL_CmdBufIn(CELL(0));
        HAL_CmdBufIn(VERTEX2F((EVE_LCD_WIDTH/2 - QR_TXT_W/2)*EVE_PIXEL_UNIT,(EVE_LCD_HIGH/2 - QR_TXT_H/2)*EVE_PIXEL_UNIT));
        ++count;
    }

	HAL_CmdBufIn(END());
	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

	HAL_BufToReg(RAM_CMD, 0);
    
#if !defined(CAL_NEEDED)
    if (++tick == COUNT) {
#else
    while (TOUCHED) {
#endif
        press = 1;
    }

    if (count == COUNT) {
	    appGP.appIndex = 1;
        press = 0;
        count = 0;
#if !defined(CAL_NEEDED)
        tick = 0;
#endif
    } else {
	    appGP.appIndex = 0;
    }
}
FTVOID scan_2(FTU32 para)
{
	static FTU32 frm = 0;
#if !defined(CAL_NEEDED)
    static FTU8 tick = 0;
#endif

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255, 255, 255));
	HAL_CmdBufIn(CLEAR(1, 1, 1));
    
    CoCmd_ANIMFRAME((EVE_LCD_WIDTH/2), (EVE_LCD_HIGH/2), ANIM_ADDR, frm);
    
    loop_frame(&frm,ANIM_MAX);

	HAL_CmdBufIn(END());
	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);

	HAL_BufToReg(RAM_CMD, 0);
    
    appGP.appIndex = 1;

#if !defined(CAL_NEEDED)
    if (++tick == 2*COUNT) {
        tick = 0;
#else
    while (TOUCHED) {
#endif

	    appGP.appIndex = 0;
    }
}

AppFunc APPS_UI[] = {
	scan_1,
    scan_2,
};

