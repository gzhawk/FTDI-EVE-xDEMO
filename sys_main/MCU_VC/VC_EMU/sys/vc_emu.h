
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

extern BT8XXEMU_Emulator *gEmulator;

#define BTFLASH_DATA_FILE   L"..\\..\\..\\res\\eveflh\\eveflh.bin"
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
FTVOID HAL_speed_up (FTU32 type);
FTVOID HAL_PwdCyc ( FTU8 OnOff );
FTVOID HAL_SpiInit ( FTVOID );
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
