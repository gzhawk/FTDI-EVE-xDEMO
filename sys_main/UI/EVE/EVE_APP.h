/* 
    Applicaton, Integrated function for FT800
    Author: Hawk
    Date  : 2013/Oct
 */

#ifndef _FT800_APP_H_
#define _FT800_APP_H_

/* you need to carfully consider the size of the block
 * base on your system available memory and data transfer buffering space 
 * WINDOWS: consider it as unlimited memory
 * FT9xx  : totally got 64K memory
 */
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(FT9XXEV)
#define MCU_BLOCK_SIZE      (10*1024)
#else
/* limited by sdcard sector size
 * should be the same as SDC_SECTOR_SIZE if sdcard.h be used */
#define MCU_BLOCK_SIZE      (512)
#endif

/* Attention, so far as I know, Arduino platform may force the number to 0
 * if you define the number larger than system allow
 * e.g. in Mege328 system (no external memory) MAX should not larger than 32K
 * UL define may be used when you have to use a number larger than 32K */
#ifdef DEF_81X
#define FT800_RAMG_SIZE       (1*1024*1024UL)
#else
#define FT800_RAMG_SIZE       (256*1024UL)
#endif
/* 0~15, 15 can only be used when no GRADIENT, BUTTON, KEYS be used 
 * at current display list */
#define FT800_BMP_EXT_HANDLE  (16) 
#define FT800_APP_ERR_CODE    (0xFF00UL) 

#define FT800_PIC_MEM_ADDR    RAM_G
#define FT800_FONT_INTERNAL   (16)

#define ZLIB_LEN              (0xFFFFFFFF)
typedef struct bmpHDR_ {
    FTC8  *path;
    FTC8  *path_lut; //for palette only
    FTU32 lut_src;   //for palette only
    FTU32 format;
    FTU32 len;
    FTU32 len_lut;   //for palette only
    FTU32 wide;
    FTU32 high;
}bmpHDR_st;
typedef struct audio_hd_st {
    FTU8 * path;
    FTU32 index;
    FTU32 dsize;
    FTU32 type;
    FTU16 freq;
    FTU8 vol;
}audio_hd_t;
typedef enum appRet_ {
    APP_OK          = 0,
    APP_ERR_CALC    = FT800_APP_ERR_CODE,
    APP_ERR_NULL,
    APP_ERR_OPEN,
    APP_ERR_HDL_EXC,
    APP_ERR_LEN,
} appRet_en;
/* 
 * application routine call back pointer
 * for easy modulize the code
 * and better application structure
 */
typedef FTVOID (* AppFunc) (FTU32);

FTVOID resWrBuf (FTU32 para);
FTVOID resWrEve (FTU32 para);
FTVOID resWrEveCmd (FTU32 para);
FTU32 appFileToRamG (FTC8 *path, FTU32 inAddr, FTU8 chkExceed, FTU8 *outAddr, FTU32 outLen);
appRet_en appWaitCal (FTVOID);
FTVOID FillBmpDL(FTU32 bmpHdl, FTU32 ramgAddr, bmpHDR_st *pbmpHD, FTU32 nums);
appRet_en appLoadBmp(FTU32 ramgAddr, bmpHDR_st *pbmpHD, FTU32 nums);
appRet_en appBmpToRamG(FTU32 bmpHdl, FTU32 ramgAddr, bmpHDR_st *pbmpHD, FTU32 nums);
FTU32 appResOpen (FTU8 *path);
FTU32 appResSize (FTU32 resHDL);
FTU32 appResToDes (FTU32 resHDL, FTU32 Des, FTU32 Src, FTU32 len, AppFunc writeFunc);
FTVOID appResClose (FTU32 resHDL);
FTU32 appGetLinestride(bmpHDR_st bmpHD);
#if !defined(STM32F4)&&!defined(MSVC2010EXPRESS)&&!defined(MSVC2012EMU)&&!defined(FT9XXEV)
FTVOID arduino_simugpio(FTU8 flag);
FTVOID arduino_sdcardInit (FTVOID);
#endif
FTVOID UI_INIT (FTVOID);
FTVOID UI_END (FTVOID);
#endif
