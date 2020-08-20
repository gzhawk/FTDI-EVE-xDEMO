#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <time.h>

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
#define ROOT_PATH "..\\..\\..\\res\\"
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
#include "vc_mpsse.h"
#elif defined(VC_FT4222)
#include "vc_ft4222.h"
#elif defined(VC_EMULATOR)
#include "vc_emu.h"
#else
#error "no VC SPI channel be selected"
#endif

FTVOID vc_dumy_print (char *p);
FTVOID vc_apps_sys_dummy (FTU32 para);
FTVOID HAL_restore_cdata (FTC8 *dataPath, FTU8 *p);
FTU8 HAL_is_tag_vaild (FTC8 *dataPath);
FTVOID HAL_vaild_tag (FTVOID);
FTVOID HAL_invaild_tag (FTC8 *dataPath);
FTVOID HAL_save_cdata (FTC8 *dataPath, FTU8 *p);
FTVOID HAL_ili9488 (FTVOID);
FTVOID HAL_preparation (FTVOID);
FTU32 HAL_WriteSrcToDes (FTU32 handle, FTU32 src, FTU32 des, FTU32 len);
FTU8 * HAL_LoopMemMalloc (FTU32 handle, FTU32 src, FTU32 len);
FTVOID HAL_LoopMemRead (FTU32 handle, FTU8 **ppbuf, FTU32 len);
FTVOID HAL_LoopMemFree (FTU32 buf);
FTU32 HAL_SegFileOpen (FTU8 *path);
FTU32 HAL_SegFileSize (FTU32 handle);
FTVOID HAL_SegFileClose (FTU32 handle);
FTU8 HAL_ZlibCompressed (FTU32 handle, FTU32 src);
FTU8 UTF8_Pump (FTU8 *utf8_file, FTU8 * p, FTU32 utf8_num);
