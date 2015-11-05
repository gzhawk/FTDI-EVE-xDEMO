/* 
    Hardware Abstract Layer for FT800 
    Author: Hawk
    Email : hawk.gao@ftdichip.com	
    Date  : 2013/May
*/

#ifndef _FT800_HAL_H_
#define _FT800_HAL_H_

#define WVGA_WIDTH  800
#define WVGA_HIGH   480
#define WQVGA_WIDTH 480
#define WQVGA_HIGH  272
#define QVGA_WIDTH  320
#define QVGA_HIGH   240
#define HVGA_WIDTH  320
#define HVGA_HIGH   480
#if defined(LCD_WVGA)
#define FT800_LCD_WIDTH WVGA_WIDTH
#define FT800_LCD_HIGH  WVGA_HIGH
#elif defined(LCD_QVGA)
#define FT800_LCD_WIDTH QVGA_WIDTH
#define FT800_LCD_HIGH  QVGA_HIGH
#elif defined(LCD_HVGA)
#define FT800_LCD_WIDTH HVGA_WIDTH
#define FT800_LCD_HIGH  HVGA_HIGH
#else
#define FT800_LCD_WIDTH WQVGA_WIDTH
#define FT800_LCD_HIGH  WQVGA_HIGH
#endif

#define EVE_ID_REG            0xC0001
#define FT800_ID              0x7C
#define FT800_CMD_WAIT_MAX    50
#define FT800_DLP_WAIT_MAX    100
#define FT800_TOUCH_THRESHOLD 1800
#define FT800_PIXEL_UNIT      (16)

#define EVE_DLP_SIZE        (8*1024)
#define REG_FLAG_CLN       (0xFFFFFFFF)
#define FT800_TRACK_VALUE_MAX (0xFFFF)
#define FT800_TRANSFORM_MAX   (0x10000)
#define FTU32_LEN             4
#define FTU16_LEN             2
#define FTU8_LEN              1

typedef enum {
	EVE_SSPI = 0,
	EVE_DSPI,
	EVE_QSPI
}EVE_SPI_NUM;

#define FT800_SPI_DUMMY       0x04

typedef enum FT800_CMD_EXE_st {
	CMD_BUF_START = 0,
	CMD_BUF,
	CMD_BUF_END
} FT800_CMD_EXE;
typedef struct FT800_LCD_st {
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
} FT800_LCD;
FTVOID HAL_Get_EVE_ID (FTVOID);
FTVOID HAL_Cfg ( FTU8 cfg );
FTU8 HAL_Read8 ( FTU32 addr );
FTU16 HAL_Read16 ( FTU32 addr );
FTU32 HAL_Read32 ( FTU32 addr );
FTVOID HAL_Write8 ( FTU32 addr, FTU8 data );
FTVOID HAL_Write8Src ( FTU32 addr, FTU8 *src, FTU32 len );
FTVOID HAL_Write16 ( FTU32 addr, FTU16 data );
FTVOID HAL_Write32 ( FTU32 addr, FTU32 data );
FTU32 HAL_CalResultAddr (FTVOID);
FTVOID HAL_CmdWait (FTU16 CmdWrAddr);
FTVOID HAL_DlWait (FTVOID);
FTU32 HAL_EVELoopMemWr (FTU32 pDes, FTU32 desNow, FTU32 desLen, FTU8 * inSrc, FTU32 inLen);
FTVOID HAL_CmdBufIn (FTU32 Cmd);
FTVOID HAL_CmdBufInStr (FTC8 *pstr);
FTVOID HAL_DlpBufIn (FTU32 Cmd);
FTVOID HAL_BufToReg (FTU32 reg, FTU32 padNum);
FTVOID HAL_CmdToReg (FTU32 Cmd);
FTVOID HAL_Path_Config (FTVOID);
FTVOID HAL_FT800_Active (FTVOID);
FTVOID HAL_FT800_CoreReset (FTVOID);
FTVOID HAL_FT800_Clock (FTVOID);
FTVOID HAL_FT800_PowerCycle ( FTU8 OnOff );
FTVOID HAL_FT800_GPIOConfig (FTVOID);
FTVOID HAL_FT800_TouchConfig (FTVOID);
FTVOID HAL_FT800_EndDisp ( FTVOID );
FTVOID HAL_FT800_BootupDisp (FTU32 count);
FTVOID HAL_FT800_LCDConfig ( FTVOID );
FTVOID HAL_FT800_SetSPI (FTU32 type);
#endif

