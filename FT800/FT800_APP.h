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
/* use MARCO instead of subroutine to make improve program efficency (no stack in/out) */
#define CoCmd_TRACK(x, y, w, h, t)  HAL_CmdBufIn(CMD_TRACK); \
                                    HAL_CmdBufIn((FT32)(y)<<16|(FT32)(x)); \
                                    HAL_CmdBufIn((FTU32)(h)<<16|(FTU32)(w)); \
                                    HAL_CmdBufIn((FTU32)(t))

#define CoCmd_NUMBER(x, y, f, o, n) HAL_CmdBufIn(CMD_NUMBER); \
                                    HAL_CmdBufIn((FT32)(y)<<16|(FT32)(x)); \
                                    HAL_CmdBufIn((FTU32)(o)<<16|(FTU32)(f)); \
                                    HAL_CmdBufIn((FT32)n)

#define CoCmd_TEXT(x, y, f, o, p)   HAL_CmdBufIn(CMD_TEXT); \
                                    HAL_CmdBufIn((FT32)(y)<<16|(FT32)(x)); \
                                    HAL_CmdBufIn((FTU32)(o)<<16|(FTU32)(f)); \
									HAL_CmdBufInStr((FTC8 *)(p))

#define CoCmd_SLIDER(x, y, w, h, o, v, r)	HAL_CmdBufIn(CMD_SLIDER); \
                                           	HAL_CmdBufIn((FT32)(y)<<16|(FT32)(x)); \
                                           	HAL_CmdBufIn((FTU32)(h)<<16|(FTU32)(w)); \
                                           	HAL_CmdBufIn((FTU32)(v)<<16|(FTU32)(o)); \
                                           	HAL_CmdBufIn((FTU32)(r))

#define CoCmd_PROGRESS(x, y, w, h, o, v, r)	HAL_CmdBufIn(CMD_PROGRESS); \
                                           	HAL_CmdBufIn((FT32)(y)<<16|(FT32)(x)); \
                                           	HAL_CmdBufIn((FTU32)(h)<<16|(FTU32)(w)); \
                                           	HAL_CmdBufIn((FTU32)(v)<<16|(FTU32)(o)); \
                                           	HAL_CmdBufIn((FTU32)(r))

#define CoCmd_GAUGE(x, y, r, o, ma, mi, v, ra)	HAL_CmdBufIn(CMD_GAUGE); \
                                              	HAL_CmdBufIn((FT32)(y)<<16|(FT32)(x)); \
                                              	HAL_CmdBufIn((FTU32)(o)<<16|(FTU32)(r)); \
                                              	HAL_CmdBufIn((FTU32)(mi)<<16|(FTU32)(ma)); \
                                              	HAL_CmdBufIn((FTU32)(ra)<<16|(FTU32)(v))

#define CoCmd_BUTTON(x, y, w, h, f, o, p)     HAL_CmdBufIn(CMD_BUTTON); \
                                              HAL_CmdBufIn((FT32)(y)<<16|(FT32)(x)); \
                                              HAL_CmdBufIn((FTU32)(h)<<16|(FTU32)(w)); \
                                              HAL_CmdBufIn((FTU32)(o)<<16|(FTU32)(f)); \
											  HAL_CmdBufInStr((FTC8 *)(p))

#define CoCmd_KEY(x, y, w, h, f, o, p)        HAL_CmdBufIn(CMD_KEYS); \
                                              HAL_CmdBufIn((FT32)(y)<<16|(FT32)(x)); \
                                              HAL_CmdBufIn((FTU32)(h)<<16|(FTU32)(w)); \
                                              HAL_CmdBufIn((FTU32)(o)<<16|(FTU32)(f)); \
											  HAL_CmdBufInStr((FTC8 *)(p))

#define CoCmd_TOGGLE(x, y, w, f, o, s, p)     HAL_CmdBufIn(CMD_TOGGLE); \
                                              HAL_CmdBufIn((FT32)(y)<<16|(FT32)(x)); \
                                              HAL_CmdBufIn((FTU32)(f)<<16|(FTU32)(w)); \
                                              HAL_CmdBufIn((FTU32)(s)<<16|(FTU32)(o)); \
											  HAL_CmdBufInStr((FTC8 *)(p))

#define CoCmd_CLOCK(x, y, r, o, h, m, s, ms)  HAL_CmdBufIn(CMD_CLOCK); \
                                              HAL_CmdBufIn((FT32)(y)<<16|(FT32)(x)); \
                                              HAL_CmdBufIn((FTU32)(o)<<16|(FTU32)(r)); \
                                              HAL_CmdBufIn((FTU32)(m)<<16|(FTU32)(h)); \
                                              HAL_CmdBufIn((FTU32)(ms)<<16|(FTU32)(s))

#define CoCmd_SPINNER(x, y, stl, scl)         HAL_CmdBufIn(CMD_SPINNER); \
                                              HAL_CmdBufIn((FT32)(y)<<16|(FT32)(x)); \
                                              HAL_CmdBufIn((FTU32)(stl)<<16|(FTU32)(scl))

#define CoCmd_MEDIAFIFO(addr, size)           HAL_CmdBufIn(CMD_MEDIAFIFO); \
                                              HAL_CmdBufIn(addr); \
                                              HAL_CmdBufIn(size)

#define CoCmd_LOADIMAGE(addr, opt)            HAL_CmdBufIn(CMD_LOADIMAGE); \
                                              HAL_CmdBufIn(addr); \
                                              HAL_CmdBufIn(opt)

#define CoCmd_BGCOLOR(c)         HAL_CmdBufIn(CMD_BGCOLOR); \
                                 HAL_CmdBufIn((FTU32)(c))

#define CoCmd_FGCOLOR(c)         HAL_CmdBufIn(CMD_FGCOLOR); \
                                 HAL_CmdBufIn((FTU32)(c))

#define CoCmd_LOADIDENTITY       HAL_CmdBufIn(CMD_LOADIDENTITY)

#define CoCmd_TRANSLATE(x, y)    HAL_CmdBufIn(CMD_TRANSLATE); \
                                 HAL_CmdBufIn((FT32)(x)); \
                                 HAL_CmdBufIn((FT32)(y))

#define CoCmd_ROTATE(a)          HAL_CmdBufIn(CMD_ROTATE); \
                                 HAL_CmdBufIn((FT32)(a))

#define CoCmd_ROMFONT(hdl, font) HAL_CmdBufIn(CMD_ROMFONT); \
                                 HAL_CmdBufIn(hdl); \
                                 HAL_CmdBufIn(font)

#define CoCmd_SCALE(x, y)        HAL_CmdBufIn(CMD_SCALE); \
                                 HAL_CmdBufIn((FT32)(x)); \
                                 HAL_CmdBufIn((FT32)(y))

#define CoCmd_SETMATRIX          HAL_CmdBufIn(CMD_SETMATRIX)

#define CoCmd_GRADIENT(x0, y0, rgb0, x1, y1, rgb1) HAL_CmdBufIn(CMD_GRADIENT); \
                                                   HAL_CmdBufIn((FTU32)(y0)<<16|(FTU32)(x0)); \
                                                   HAL_CmdBufIn((FTU32)(rgb0)); \
                                                   HAL_CmdBufIn((FTU32)(y1)<<16|(FTU32)(x1)); \
                                                   HAL_CmdBufIn((FTU32)(rgb1))

#define CoCmd_VIDEOSTART         HAL_CmdBufIn(CMD_VIDEOSTART)

#define CoCmd_VIDEOFRAME(disp, flag)         HAL_CmdBufIn(CMD_VIDEOFRAME); \
                                             HAL_CmdBufIn(disp); \
                                             HAL_CmdBufIn(flag)

#define TOUCHED                  (!(HAL_Read16(REG_CTOUCH_TOUCH0_XY)&0x8000))

#define CoCmd_SETFONT(f, s, pf)  HAL_CmdBufIn(BITMAP_HANDLE((FTU32)(f))); \
                                 HAL_CmdBufIn(BITMAP_SOURCE(((FT_Gpu_Fonts_t *)(pf))->PointerToFontGraphicsData)); \
                                 HAL_CmdBufIn(BITMAP_LAYOUT(((FT_Gpu_Fonts_t *)(pf))->FontBitmapFormat,((FT_Gpu_Fonts_t *)(pf))->FontLineStride,((FT_Gpu_Fonts_t *)(pf))->FontHeightInPixels)); \
                                 HAL_CmdBufIn(BITMAP_SIZE(NEAREST,BORDER,BORDER,((FT_Gpu_Fonts_t *)(pf))->FontWidthInPixels,((FT_Gpu_Fonts_t *)(pf))->FontHeightInPixels)); \
                                 HAL_CmdBufIn(CMD_SETFONT); \
                                 HAL_CmdBufIn((FTU32)(f)); \
                                 HAL_CmdBufIn((FTU32)(s))
FTVOID resWrBuf (FTU32 para);
FTVOID resWrEve (FTU32 para);
FTVOID resWrEveCmd (FTU32 para);
FTU32 appFileToRamG (FTC8 *path, FTU32 inAddr, FTU8 chkExceed, FTU8 *outAddr, FTU32 outLen);
appRet_en appWaitCal (FTVOID);
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

#endif
