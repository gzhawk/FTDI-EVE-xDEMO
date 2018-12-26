#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <time.h>

#if defined(VC_MPSSE)
#include "libMPSSE_spi.h"
#endif

#if defined(VC_FT4222)
#include "ftd2xx.h"
#include "libFT4222.h"
#endif

#if defined(VC_EMULATOR)
#include <direct.h>
#include <io.h>
#if defined(_MSC_VER)
typedef unsigned __int8 uint8_t;
typedef signed __int8 int8_t;
typedef unsigned __int16 uint16_t;
typedef signed __int16 int16_t;
typedef unsigned __int32 uint32_t;
typedef signed __int32 int32_t;
typedef unsigned __int64 uint64_t;
typedef signed __int64 int64_t;
#endif
typedef uint32_t argb8888;
#include "bt8xxemu_Emulator.h"
#endif

typedef const char     FTC8;
typedef signed char    FT8;
typedef unsigned char  FTU8;
typedef signed short   FT16;
typedef unsigned short FTU16;
typedef signed int     FT32;
typedef unsigned int   FTU32;
typedef signed long    FT64;
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

#define STATIC static

#ifdef DBG_PRINT
#define FTPRINT     printf
#define DBGPRINT    appUI_DbgPrint(__FUNCTION__,__LINE__)
#define SYS_INIT    printf("\r\nVer: %s",APPS_VER)
#else
#define FTPRINT     vc2012emu_dumy_print
#define DBGPRINT
#define SYS_INIT 
#endif

#define FTRANDOM(M) (rand()%(M))

#define EVE_CAL_PARA_NUM    (6)

#define APPS_SYS vc_apps_sys_dummy

#if defined(VC_MPSSE)
#define SPI_CLK_15M    15000000
#define SPI_L_TIMER    2
#define SPI_INIDIR_IN  0
#define SPI_INIVAL_LOW 0
#define SPI_ENDDIR_IN  0
#define SPI_ENDVAL_LOW 0
#define SPI_TXCMD_LEN  3
#define SPI_RXCMD_LEN  4

FT_HANDLE ftHandle;
#endif

#if defined(VC_FT4222)
#define SPI_TIMEOUT_R  5000
#define SPI_TIMEOUT_W  5000
#define SPI_LATENCY    2

#define SPI_SYSCLK     SYS_CLK_80
#define SPI_SYSDIV     CLK_DIV_4
#define SPI_PIN_CS     1

#define SPI_TXADDR_LEN  3
#define SPI_RXADDR_SLEN 4
#define SPI_RXADDR_MLEN 5

FT_HANDLE ftHdl_spi;
FT_HANDLE ftHdl_gpio;
FTU8 spi_type;
#endif

#if defined(VC_MPSSE) || defined(VC_FT4222)
#define FTMAIN FT32 main (FT32 argc,FT8 *argv[]) 
#define FTDUMMY
#define SYS_END    return 0
#endif

#if defined(VC_EMULATOR)
extern BT8XXEMU_Emulator *gEmulator;

#define BTFLASH_DATA_FILE   L"..\\..\\res\\eveflh\\eveflh.bin"
#define BTFLASH_DEVICE_TYPE L"mx25lemu"
#define BTFLASH_SIZE        (16 * 1024 * 1024)

#if defined(DEF_BT81X)
#define EVEMODE BT8XXEMU_EmulatorBT815
#elif defined(DEF_81X)
#define EVEMODE BT8XXEMU_EmulatorFT810
#else
#define EVEMODE BT8XXEMU_EmulatorFT800
#endif

#define FTMAIN FTVOID mcu (BT8XXEMU_Emulator *sender, void *context) 
#define FTDUMMY \
                int main(FT32 argc,FT8 *argv[]) \
                { \
                  BT8XXEMU_Flash *flash = NULL; \
                  BT8XXEMU_FlashParameters flashParams; \
				  BT8XXEMU_EmulatorParameters params; \
                  \
                  BT8XXEMU_Flash_defaults(BT8XXEMU_VERSION_API, &flashParams); \
                  wcscpy(flashParams.DeviceType, BTFLASH_DEVICE_TYPE); \
                  wcscpy(flashParams.DataFilePath, BTFLASH_DATA_FILE); \
                  flashParams.SizeBytes = BTFLASH_SIZE; \
                  flashParams.StdOut = 1; \
                  flash = BT8XXEMU_Flash_create(BT8XXEMU_VERSION_API, &flashParams); \
                  \
				  BT8XXEMU_defaults(BT8XXEMU_VERSION_API, &params, EVEMODE); \
				  params.Flags |= BT8XXEMU_EmulatorEnableStdOut; \
				  params.Main = mcu; \
				  params.Flash = flash; \
				  BT8XXEMU_run(BT8XXEMU_VERSION_API, &gEmulator, &params); \
                  BT8XXEMU_stop(gEmulator); \
                  BT8XXEMU_destroy(gEmulator); \
                  BT8XXEMU_Flash_destroy(flash); \
                  \
				  return 0; \
			    }
#define SYS_END BT8XXEMU_stop(gEmulator)

FTVOID FT8XXEMU_cs(FT8 i);
FTU32 FT8XXEMU_transfer(FTU32 data);
#endif

FTVOID vc_dumy_print (char *p);
FTVOID vc_apps_sys_dummy (FTU32 para);

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
FTVOID HAL_Cfg3 ( FTU32 cfg3 );
FTVOID HAL_Write8 ( FTU32 addr, FTU8 data );
FTVOID HAL_Write8Src ( FTU32 addr, FTU8 *src, FTU32 len );
FTVOID HAL_Write16 ( FTU32 addr, FTU16 data );
FTVOID HAL_Write32 ( FTU32 addr, FTU32 data );
FTU8 HAL_Read8 ( FTU32 addr );
FTU32 HAL_Read8Buff ( FTU32 addr, FTU8 *buff, FTU32 len );
FTU16 HAL_Read16 ( FTU32 addr );
FTU32 HAL_Read32 ( FTU32 addr );

FTU8 UTF8_Pump (FTU8 *utf8_file, FTU8 * p, FTU32 utf8_num);
