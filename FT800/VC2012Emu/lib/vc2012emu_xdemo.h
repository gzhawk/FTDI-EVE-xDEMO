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
#include "ft800emu_emulator.h"

#define USED_CMD_BUF

#define PROGMEM 
#define FLASH8 FT8
#define FLASHU8 FTU8
#define FLASHRD8(a) (*(FTU8 *)(a))

#define FTPRINT        printf
#define FTDELAY(mS)    Sleep(mS)
#define FTEND
#define ROOT_PATH "..\\..\\res\\"
#define CDATA_PATH ROOT_PATH"cdata.bin"
#define FTMAIN FTVOID setup () 
#define FTDUMMY FTVOID loop () {}

#define FTINDEF static

#ifdef FT800_PRINT
#define DBGPRINT FTPRINT("\r\nError %s:%d",__FUNCTION__,__LINE__)
#define FTPREINIT FTPRINT("\r\nVer: %s",FT800_VER)
#else
#define DBGPRINT
#define FTPREINIT 
#endif

#define FTRANDOM(M) (rand()%(M))

/* TODO: at this point I fixed use FT8XXEMU_EmulatorFT811 to the emulator, may improve it later */
#ifdef DEF_81X
#define FTEMU int main(FT32 argc,FT8 *argv[]) \
              { \
				  FT8XXEMU_EmulatorParameters params; \
				  FT8XXEMU_defaults(FT8XXEMU_VERSION_API, &params, FT8XXEMU_EmulatorFT810); \
				  params.Flags &= (~FT8XXEMU_EmulatorEnableDynamicDegrade & \
						           ~FT8XXEMU_EmulatorEnableRegPwmDutyEmulation); \
				  params.Setup = setup; \
				  params.Loop = loop; \
				  FT8XXEMU_run(FT8XXEMU_VERSION_API, &params); \
				  return 0; \
			  }
#else
#define FTEMU int main(FT32 argc,FT8 *argv[]) \
              { \
				  FT8XXEMU_EmulatorParameters params; \
				  FT8XXEMU_defaults(FT8XXEMU_VERSION_API, &params, FT8XXEMU_EmulatorFT800); \
				  params.Flags &= (~FT8XXEMU_EmulatorEnableDynamicDegrade & \
						           ~FT8XXEMU_EmulatorEnableRegPwmDutyEmulation); \
				  params.Setup = setup; \
				  params.Loop = loop; \
				  FT8XXEMU_run(FT8XXEMU_VERSION_API, &params); \
				  return 0; \
			  }
#endif

#define FTRES FILE*

#define FTIOCNTRL
#define FT800_CAL_PARA_NUM    (6)

FTVOID vc2012emu_invaild_tag (FTC8 *dataPath);
FTU8 vc2012emu_is_tag_vaild (FTC8 *dataPath);
FTVOID vc2012emu_save_cdata (FTC8 *dataPath, FTU8 *p);
FTVOID vc2012emu_restore_cdata (FTC8 *dataPath, FTU8 *p);
FTVOID vc2012emu_vaild_tag (FTVOID);

