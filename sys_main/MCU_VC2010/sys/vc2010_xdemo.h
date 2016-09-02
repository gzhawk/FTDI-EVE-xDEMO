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
#define ROOT_PATH "..\\..\\res\\"
#define CDATA_PATH ROOT_PATH"cdata.bin"
#define FTMAIN FT32 main (FT32 argc,FT8 *argv[]) 
#define FTDUMMY

#define FTINDEF static

#ifdef FT800_PRINT
#define FTPRINT     printf
#define DBGPRINT    printf("\r\nError %s:%d",__FUNCTION__,__LINE__)
#define SYS_INIT   printf("\r\nVer: %s",FT800_VER)
#else
#define FTPRINT     vc2010_dumy_print
#define DBGPRINT
#define SYS_INIT
#endif


#define FTRANDOM(M) (rand()%(M))

#define FTRES FILE*

#define FTIOCNTRL
#define FT800_CAL_PARA_NUM    (6)

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
void vc2010_spi_init(void);
FTVOID vc2010_invaild_tag (FTC8 *dataPath);
FTU8 vc2010_is_tag_vaild (FTC8 *dataPath);
FTVOID vc2010_save_cdata (FTC8 *dataPath, FTU8 *p);
FTVOID vc2010_restore_cdata (FTC8 *dataPath, FTU8 *p);
FTVOID vc2010_vaild_tag (FTVOID);
FTVOID vc2010_dumy_print(char *p);
FTVOID vc2010_apps_sys_dummy (FTU32 para);
