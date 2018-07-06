/* 
    Hardware Abstract Layer for EVE 
    Author: Hawk
    Email : hawk.gao@ftdichip.com	
    Date  : 2013/May
 */

#ifndef _EVE_HAL_H_
#define _EVE_HAL_H_

#define WVGA_WIDTH  800
#define WVGA_HIGH   480

#define WQVGA_WIDTH 480
#define WQVGA_HIGH  272

#define QVGA_WIDTH  320
#define QVGA_HIGH   240

#define HVGA_WIDTH  320
#define HVGA_HIGH   480

#if defined(LCD_WVGA)
#define EVE_LCD_WIDTH WVGA_WIDTH
#define EVE_LCD_HIGH  WVGA_HIGH
#elif defined(LCD_QVGA)
#define EVE_LCD_WIDTH QVGA_WIDTH
#define EVE_LCD_HIGH  QVGA_HIGH
#elif defined(LCD_HVGA)
#define EVE_LCD_WIDTH HVGA_WIDTH
#define EVE_LCD_HIGH  HVGA_HIGH
#else
#define EVE_LCD_WIDTH WQVGA_WIDTH
#define EVE_LCD_HIGH  WQVGA_HIGH
#endif

#define EVE_ID_REG            (0xC0001)
#define EVE_ID              (0x7C)
#define EVE_CMD_WAIT_MAX    50
#define EVE_DLP_WAIT_MAX    100
#define EVE_TOUCH_THRESHOLD 1800
#define EVE_PIXEL_UNIT      (16)

#define EVE_DLP_SIZE          (8*1024)
#define REG_FLAG_CLN          (0xFFFFFFFF)
#define EVE_TRACK_VALUE_MAX (0xFFFF)
#define EVE_TRANSFORM_MAX   (0x10000)
#define FTU32_LEN             4
#define FTU16_LEN             2
#define FTU8_LEN              1

#define EVE_SPI_DUMMY       (0x04)

typedef enum {
    EVE_SSPI = 0,
    EVE_DSPI,
    EVE_QSPI
}EVE_SPI_NUM;
typedef enum EVE_CMD_EXE_st {
    CMD_BUF_START = 0,
    CMD_BUF,
    CMD_BUF_END
} EVE_CMD_EXE;
typedef struct EVE_LCD_st {
    FT16 Width;
    FT16 Height;
    FT16 HCycle;
    FT16 HOffset;
    FT16 HSync0;
    FT16 HSync1;
    FT16 VCycle;
    FT16 VOffset;
    FT16 VSync0;
    FT16 VSync1;
    FTU8 PCLK;
    FT8 Swizzle;
    FT8 PCLKPol;
    FT16 Cspread;
    FT16 Dither;	
} EVE_LCD;

#define CoCmd_TRACK(x, y, w, h, t)  HAL_CmdBufIn(CMD_TRACK); \
                                    HAL_CmdBufIn((FT32)(y)<<16|(FT32)(x)); \
                                    HAL_CmdBufIn((FTU32)(h)<<16|(FTU32)(w)); \
                                    HAL_CmdBufIn((FTU32)(t))

#define CoCmd_SETBASE(base)         HAL_CmdBufIn(CMD_SETBASE); \
                                    HAL_CmdBufIn(base)

#define CoCmd_NUMBER(x, y, f, o, n) HAL_CmdBufIn(CMD_NUMBER); \
                                    HAL_CmdBufIn((FT32)(y)<<16|(FT32)(x)); \
                                    HAL_CmdBufIn((FTU32)(o)<<16|(FTU32)(f)); \
                                    HAL_CmdBufIn((FT32)n)

FTVOID CoCmd_TEXT(FTU32 x, FTU32 y, FTU32 font, FTU32 opt, FTC8 * s, ...);

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

#define CoCmd_BGCOLOR(c)            HAL_CmdBufIn(CMD_BGCOLOR); \
                                    HAL_CmdBufIn((FTU32)(c))

#define CoCmd_FGCOLOR(c)            HAL_CmdBufIn(CMD_FGCOLOR); \
                                    HAL_CmdBufIn((FTU32)(c))

#define CoCmd_LOADIDENTITY          HAL_CmdBufIn(CMD_LOADIDENTITY)

#define CoCmd_TRANSLATE(x, y)       HAL_CmdBufIn(CMD_TRANSLATE); \
                                    HAL_CmdBufIn((FT32)(x)); \
                                    HAL_CmdBufIn((FT32)(y))

#define CoCmd_ROTATE(a)             HAL_CmdBufIn(CMD_ROTATE); \
                                    HAL_CmdBufIn((FT32)(a))

#define CoCmd_ROMFONT(hdl, font) HAL_CmdBufIn(CMD_ROMFONT); \
                                    HAL_CmdBufIn(hdl); \
                                    HAL_CmdBufIn(font)

#define CoCmd_SCALE(x, y)           HAL_CmdBufIn(CMD_SCALE); \
                                    HAL_CmdBufIn((FT32)(x)); \
                                    HAL_CmdBufIn((FT32)(y))

#define CoCmd_SETMATRIX             HAL_CmdBufIn(CMD_SETMATRIX)

#define CoCmd_GRADIENT(x0, y0, rgb0, x1, y1, rgb1) HAL_CmdBufIn(CMD_GRADIENT); \
                                    HAL_CmdBufIn((FTU32)(y0)<<16|(FTU32)(x0)); \
                                    HAL_CmdBufIn((FTU32)(rgb0)); \
                                    HAL_CmdBufIn((FTU32)(y1)<<16|(FTU32)(x1)); \
                                    HAL_CmdBufIn((FTU32)(rgb1))

#define CoCmd_VIDEOSTART            HAL_CmdBufIn(CMD_VIDEOSTART)

#define CoCmd_VIDEOFRAME(disp, flag)         HAL_CmdBufIn(CMD_VIDEOFRAME); \
                                    HAL_CmdBufIn(disp); \
                                    HAL_CmdBufIn(flag)

#define TOUCHED                     (!(HAL_Read16(REG_CTOUCH_TOUCH0_XY)&0x8000))

#if defined(DEF_81X) || defined(DEF_BT81X)
#define CoCmd_SETFONT(f, s, pf)     HAL_CmdBufIn(BITMAP_HANDLE((FTU32)(f))); \
                                    HAL_CmdBufIn(BITMAP_SOURCE(((FT_Gpu_Fonts_t *)(pf))->PointerToFontGraphicsData)); \
                                    HAL_CmdBufIn(BITMAP_LAYOUT(((FT_Gpu_Fonts_t *)(pf))->FontBitmapFormat, \
                                    ((FT_Gpu_Fonts_t *)(pf))->FontLineStride,((FT_Gpu_Fonts_t *)(pf))->FontHeightInPixels)); \
                                    HAL_CmdBufIn(BITMAP_LAYOUT_H((((FT_Gpu_Fonts_t *)(pf))->FontLineStride)>>10, \
                                    (((FT_Gpu_Fonts_t *)(pf))->FontHeightInPixels)>>9)); \
                                    HAL_CmdBufIn(BITMAP_SIZE(NEAREST,BORDER,BORDER,((FT_Gpu_Fonts_t *)(pf))->FontWidthInPixels, \
                                    ((FT_Gpu_Fonts_t *)(pf))->FontHeightInPixels)); \
                                    HAL_CmdBufIn(BITMAP_SIZE_H((((FT_Gpu_Fonts_t *)(pf))->FontWidthInPixels)>>9, \
                                    (((FT_Gpu_Fonts_t *)(pf))->FontHeightInPixels)>>9)); \
                                    HAL_CmdBufIn(CMD_SETFONT); \
                                    HAL_CmdBufIn((FTU32)(f)); \
                                    HAL_CmdBufIn((FTU32)(s))
#else
#define CoCmd_SETFONT(f, s, pf)     HAL_CmdBufIn(BITMAP_HANDLE((FTU32)(f))); \
                                    HAL_CmdBufIn(BITMAP_SOURCE(((FT_Gpu_Fonts_t *)(pf))->PointerToFontGraphicsData)); \
                                    HAL_CmdBufIn(BITMAP_LAYOUT(((FT_Gpu_Fonts_t *)(pf))->FontBitmapFormat, \
                                    ((FT_Gpu_Fonts_t *)(pf))->FontLineStride,((FT_Gpu_Fonts_t *)(pf))->FontHeightInPixels)); \
                                    HAL_CmdBufIn(BITMAP_SIZE(NEAREST,BORDER,BORDER,((FT_Gpu_Fonts_t *)(pf))->FontWidthInPixels, \
                                    ((FT_Gpu_Fonts_t *)(pf))->FontHeightInPixels)); \
                                    HAL_CmdBufIn(CMD_SETFONT); \
                                    HAL_CmdBufIn((FTU32)(f)); \
                                    HAL_CmdBufIn((FTU32)(s))
#endif

#define CoCmd_MEMCPY(des,src,len)   HAL_CmdBufIn(CMD_MEMCPY); \
                                    HAL_CmdBufIn(des); \
                                    HAL_CmdBufIn(src); \
                                    HAL_CmdBufIn(len)

#define CoCmd_MEMSET(ptr,val,len)   HAL_CmdBufIn(CMD_MEMSET); \
                                    HAL_CmdBufIn(ptr); \
                                    HAL_CmdBufIn(val); \
                                    HAL_CmdBufIn(len)

#define CoCmd_MEMZERO(ptr,len)      HAL_CmdBufIn(CMD_MEMZERO); \
                                    HAL_CmdBufIn(ptr); \
                                    HAL_CmdBufIn(len)

#define CoCmd_MEMCRC(ptr,len,ret)   HAL_CmdBufIn(CMD_MEMCRC); \
                                    HAL_CmdBufIn(ptr); \
                                    HAL_CmdBufIn(len); \
                                    HAL_CmdBufIn(ret)

#define CoCmd_APPEND(addr, nums)    HAL_CmdBufIn(CMD_APPEND); \
                                    HAL_CmdBufIn(addr); \
                                    HAL_CmdBufIn(nums)

#if defined(DEF_81X) || defined(DEF_BT81X)
#define CoCmd_SETROTATE(r)          HAL_CmdBufIn(CMD_SETROTATE); \
                                    HAL_CmdBufIn(r)

#define CoCmd_SETBITMAP(s, f, w, h) HAL_CmdBufIn(CMD_SETBITMAP); \
                                    HAL_CmdBufIn(s); \
                                    HAL_CmdBufIn(((FT32)(w)<<16)|((f) & 0xFFFF)); \
                                    HAL_CmdBufIn(h)

#define CoCmd_ANIMFRAME(x, y, o, f) HAL_CmdBufIn(CMD_ANIMFRAME); \
                                    HAL_CmdBufIn(((FT32)(y)<<16)|((x) & 0xFFFF)); \
                                    HAL_CmdBufIn(o); \
                                    HAL_CmdBufIn(f)

#endif

FTVOID HAL_Cfg ( FTU8 cfg );
FTU8 HAL_Read8 ( FTU32 addr );
FTU32 HAL_Read8Buff ( FTU32 addr, FTU8 *buff, FTU32 len );
FTU16 HAL_Read16 ( FTU32 addr );
FTU32 HAL_Read32 ( FTU32 addr );
FTVOID HAL_Write8 ( FTU32 addr, FTU8 data );
FTVOID HAL_Write8Src ( FTU32 addr, FTU8 *src, FTU32 len );
FTVOID HAL_Write16 ( FTU32 addr, FTU16 data );
FTVOID HAL_Write32 ( FTU32 addr, FTU32 data );
FTU32 HAL_CalResultAddr (FTVOID);
FTVOID HAL_CoReset (FTVOID);
FTVOID HAL_CmdWait (FTU16 CmdWrAddr);
FTVOID HAL_DlWait (FTVOID);
FTU32 HAL_EVELoopMemWr (FTU32 pDes, FTU32 desNow, FTU32 desLen, FTU8 * inSrc, FTU32 inLen);
FTVOID HAL_CmdBufIn (FTU32 Cmd);
FTVOID HAL_CmdBufInStr (FTC8 *pstr);
FTVOID HAL_DlpBufIn (FTU32 Cmd);
FTVOID HAL_BufToReg (FTU32 reg, FTU32 padNum);
FTVOID HAL_CmdToReg (FTU32 Cmd);
FTVOID HAL_McuCmdBufInit (FTVOID);
FTU32 HAL_CmdBufSize (FTVOID);
FTVOID HAL_CmdExeNow(FTU32 * pCL, FTU32 l);
#endif

