#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <direct.h>
#include <time.h>
#include <io.h>

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

#ifdef _MSC_VER
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
extern BT8XXEMU_Emulator *gEmulator;

#define BTFLASH_DATA_FILE   L"..\\..\\res\\flash\\bt81X.flash"
#define BTFLASH_DEVICE_TYPE L"mx25lemu"
#define BTFLASH_SIZE        (16 * 1024 * 1024)
#define USED_CMD_BUF

#define PROGMEM 
#define FLASH8 FT8
#define FLASHU8 FTU8
#define FLASHRD8(a) (*(FTU8 *)(a))

#define FTDELAY(mS)    Sleep(mS)
#define ROOT_PATH "..\\..\\res\\"
#define CDATA_PATH ROOT_PATH"cdata.bin"
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

#define EVEMODE BT8XXEMU_EmulatorBT815

#define FTRES FILE*

#define EVE_CAL_PARA_NUM    (6)

#define APPS_SYS vc2017emu_apps_sys_dummy

#define SYS_END BT8XXEMU_stop(gEmulator)

FTVOID vc2017emu_invaild_tag (FTC8 *dataPath);
FTU8 vc2017emu_is_tag_vaild (FTC8 *dataPath);
FTVOID vc2017emu_save_cdata (FTC8 *dataPath, FTU8 *p);
FTVOID vc2017emu_restore_cdata (FTC8 *dataPath, FTU8 *p);
FTVOID vc2017emu_vaild_tag (FTVOID);
FTVOID vc2017emu_dumy_print (char *p);
FTVOID vc2017emu_apps_sys_dummy (FTU32 para);

FTVOID FT8XXEMU_cs(FT8 i);
FTU32 FT8XXEMU_transfer(FTU32 data);
