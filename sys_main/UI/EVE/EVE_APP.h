/* 
    Applicaton, Integrated function for EVE
    Author: Hawk
    Date  : 2013/Oct
 */

#ifndef _EVE_APP_H_
#define _EVE_APP_H_



/* Attention, so far as I know, Arduino platform may force the number to 0
 * if you define the number larger than system allow
 * e.g. in Mege328 system (no external memory) MAX should not larger than 32K
 * UL define may be used when you have to use a number larger than 32K */
#if defined(DEF_81X) || defined(DEF_BT81X)
#define EVE_RAMG_SIZE       (1*1024*1024UL)
#else
#define EVE_RAMG_SIZE       (256*1024UL)
#endif
/* 0~15, 15 can only be used when no GRADIENT, BUTTON, KEYS be used 
 * at current display list */
#define EVE_BMP_EXT_HANDLE  (16) 
#define EVE_APP_ERR_CODE    (0xFF00UL) 

#define EVE_PIC_MEM_ADDR    RAM_G
#define EVE_FONT_INTERNAL   (16)

#define TYPE_ZLIB           (0xFFFFFFFF)
/* 
there would be three kinds of flash file location mark:
"Z_FLASH:addr:len"      all kinds of compressed format
"ASTC_FLASH:addr:len"   original ASTC format
"FLASH:addr:len"        all other none ASTC format
*/
#define TYPE_Z_FLASH          (0xFFFFFFFE)
#define TYPE_ASTC_FLASH       (0xFFFFFFFD)
#define TYPE_FLASH            (0xFFFFFFFC)
#define L_Z_FLASH             7
#define L_ASTC_FLASH          10
#define L_FLASH               5
#define MARK_ADDR             '@'
#define MARK_LEN              ':'

#if defined(DEF_BT81X)
#define EVE_DBG_BUF_LEN       (128)
#define EVE_FLHUPDATE_LEN     (4*1024)
#define EVE_FLHBLOCK_LEN      (10*EVE_FLHUPDATE_LEN)
/* user may need to modify it base on it's own RAM partition */
#define EVE_FLHUPDATE_ADDR    (EVE_RAMG_SIZE - EVE_FLHUPDATE_LEN)
#else
#define EVE_DBG_BUF_LEN       (50)
#endif
typedef struct bmpHDR_ {
    FTU32 format;
    FTU32 wide;
    FTU32 high;
    FTU32 info; /*address to memory which store more information
                  of image such as file path, file length, etc.*/
}bmpHDR_st;
typedef struct ImgInfo_ {
    FTC8  *path;
    FTU32 handle;
    FTU32 len;
    FTU32 addr; /* address in EVE or Flash */
}ImgInfo_st;
typedef struct ImgInfoPal_ {
    FTC8  *path_inx;
    FTC8  *path_lut;
    FTU32 handle;
    FTU32 len_inx;
    FTU32 len_lut;
    FTU32 addr_inx; /* address in EVE or Flash */
    FTU32 addr_lut; /* address in EVE or Flash */
}ImgInfoPal_st;
typedef struct ImgInfoDXT1_ {
    FTU32 addr; /* address in EVE or Flash, 
                   consider b0,b1,c0,c1 should
                   be continuous stored */
    FTU32 handle;
    FTC8  *path_c0; /* the memory sequence should be */
    FTU32 len_c0;   /* c0,c1,b0,b1                   */
    FTC8  *path_c1;
    FTU32 len_c1;
    FTC8  *path_b0;
    FTU32 len_b0;
    FTC8  *path_b1;
    FTU32 len_b1;
}ImgInfoDXT1_st;
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
    APP_ERR_CALC    = EVE_APP_ERR_CODE,
    APP_ERR_NULL,
    APP_ERR_OPEN,
    APP_ERR_HDL_EXC,
    APP_ERR_LEN,
} appRet_en;

typedef FTVOID (* AppFunc) (FTU32);

FTU8 appFileExist (FTC8 *path);
FTU32 appFileToRamG (FTC8 *path, FTU32 inAddr, FTU8 chkExceed, FTU8 *outAddr, FTU32 outLen);
appRet_en appWaitCal (FTVOID);
FTVOID appUI_FillBmpDL(FTU32 bmpHdl, bmpHDR_st *pbmpHD, FTU32 nums);
appRet_en appLoadBmp(FTU32 ramgAddr, bmpHDR_st *pbmpHD, FTU32 nums);
appRet_en appBmpToRamG(FTU32 bmpHdl, FTU32 ramgAddr, bmpHDR_st *pbmpHD, FTU32 nums);
FTU32 appGetLinestride(bmpHDR_st *bmpHD);
FTVOID appUI_DbgPrint (FTC8 *p_fname, FTU32 fline);
FTU32 appEveCRC(FTU32 eve_addr, FTU32 len);
FTVOID appEveZERO(FTU32 eve_addr, FTU32 len);
FTU8 appFlashPath (FTC8 *path, FTU32 *len);
FTVOID SegmentOperation (FTU32 handle, FTU32 src, FTU32 des, FTU32 len, FTU8 toCoPro);
FTVOID appDispDXT1 (FTU8 startHdl, FT16 X, FT16 Y);
#if defined(DEF_81X) || defined(DEF_BT81X)
FTVOID appDispPalette8 (FTU32 X, FTU32 Y, FTU32 PalSrc, FTU32 hdl, FTU32 cell);
#endif
#if defined(DEF_BT81X)
FTVOID resWrFlash (FTU32 para);
FTU8 appFlashSetFull(FTVOID);
FTVOID appFlashErase(FTVOID);
FTU8 appFlashToEVE(FTU32 flash_addr, FTU32 eve_addr, FTU32 len);
FTU8 appFlashUpdate(FTU32 f_addr, FTU32 e_addr, FTU32 len);
FTU32 appFlashVerify(FTU8 *golden_file, FTU32 flash_addr);
FTU32 appFlashProgProgress(FTU8 *f_file, FTU32 f_addr, FTU32 block, FTU8 update);
FTU32 appFlashAddr(FTC8 *path);
FTU32 appFlashLen(FTC8 *path);
FTVOID appFlashUnzip(FTC8 *path, FTU32 src);
#endif

FTVOID UI_INIT (FTVOID);
FTVOID UI_END (FTVOID);

#endif
