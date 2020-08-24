
#include <direct.h>
#include <io.h>
#include "bt8xxemu.h"

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

FT32 emu_main (FTVOID);

#define FTMAIN FTVOID mcu (BT8XXEMU_Emulator *sender, FTVOID *context) 
#define FTDUMMY FT32 main(FT32 argc,FT8 *argv[]) {return emu_main();}
#define SYS_END BT8XXEMU_stop(gEmulator)

FTMAIN; /* declaration of "mcu" for "emu_main" used */

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
