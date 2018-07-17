#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include "libMPSSE_spi.h"

typedef const char     FTC8;
typedef signed char    FT8;
typedef unsigned char  FTU8;
typedef signed short   FT16;
typedef unsigned short FTU16;
typedef signed int     FT32;
typedef unsigned int   FTU32;
typedef unsigned long  FTU64;
#define FTVOID void

#define USED_CMD_BUF

#define PROGMEM 
#define FLASH8 FT8
#define FLASHU8 FTU8
#define FLASHRD8(a) (*(FTU8 *)(a))

#define FTDELAY(mS)    Sleep(mS)
/* actually it's a fake function */
#define FTUDELAY(mS)    Sleep(mS/1000)
#define ROOT_PATH "..\\..\\res\\"
#define CDATA_PATH ROOT_PATH"cdata.bin"
#define FTMAIN FT32 main (FT32 argc,FT8 *argv[]) 
#define FTDUMMY

#define STATIC static

#ifdef DBG_PRINT
#define FTPRINT     printf
#define DBGPRINT    appUI_DbgPrint(__FUNCTION__,__LINE__)
#define SYS_INIT    printf("\r\nVer: %s",APPS_VER)
#else
#define FTPRINT     vc2010_dumy_print
#define DBGPRINT
#define SYS_INIT
#endif


#define FTRANDOM(M) (rand()%(M))

#define EVE_CAL_PARA_NUM    (6)

#define SPI_CLK_12M 12000000
#define SPI_CLK_15M 15000000
#define SPI_L_TIMER 2
#define SPI_INIDIR_IN 0x00000000
#define SPI_INIDIR_OUT 0x00000001
#define SPI_INIVAL_LOW 0x00000000
#define SPI_INIVAL_HIGH 0x00000100
#define SPI_ENDDIR_IN 0x00000000
#define SPI_ENDDIR_OUT 0x00010000
#define SPI_ENDVAL_LOW 0x00000000
#define SPI_ENDDIR_HIGH 0x01000000
#define SPI_TXCMD_LEN 3
#define SPI_RXCMD_LEN 4

#define APPS_SYS vc2010_apps_sys_dummy

#define SYS_END    return 0

FT_HANDLE ftHandle;
FTVOID vc2010_dumy_print(char *p);
FTVOID vc2010_apps_sys_dummy (FTU32 para);

FTVOID HAL_restore_cdata (FTC8 *dataPath, FTU8 *p);
FTU8 HAL_is_tag_vaild (FTC8 *dataPath);
FTVOID HAL_vaild_tag (FTVOID);
FTVOID HAL_invaild_tag (FTC8 *dataPath);
FTVOID HAL_save_cdata (FTC8 *dataPath, FTU8 *p);
FTVOID HAL_ili9488 (FTVOID);
FTVOID HAL_speed_up (FTU32 type);
FTVOID HAL_PwdCyc ( FTU8 OnOff );
FTVOID HAL_SpiInit ( FTVOID );
FTVOID HAL_preparation (FTVOID);
FTU32 HAL_WriteSrcToDes (FTU32 handle, FTU32 src, FTU32 des, FTU32 len);
FTU8 * HAL_LoopMemMalloc (FTU32 handle, FTU32 src, FTU32 len);
FTVOID HAL_LoopMemRead (FTU32 handle, FTU8 **ppbuf, FTU32 len);
FTVOID HAL_LoopMemFree (FTU32 buf);
FTU32 HAL_SegFileOpen (FTU8 *path);
FTU32 HAL_SegFileSize (FTU32 handle);
FTVOID HAL_SegFileClose (FTU32 handle);
FTU8 HAL_ZlibCompressed (FTU32 handle, FTU32 src);

FTVOID HAL_Cfg ( FTU8 cfg );
FTVOID HAL_Write8 ( FTU32 addr, FTU8 data );
FTVOID HAL_Write8Src ( FTU32 addr, FTU8 *src, FTU32 len );
FTVOID HAL_Write16 ( FTU32 addr, FTU16 data );
FTVOID HAL_Write32 ( FTU32 addr, FTU32 data );
FTU8 HAL_Read8 ( FTU32 addr );
FTU32 HAL_Read8Buff ( FTU32 addr, FTU8 *buff, FTU32 len );
FTU16 HAL_Read16 ( FTU32 addr );
FTU32 HAL_Read32 ( FTU32 addr );

