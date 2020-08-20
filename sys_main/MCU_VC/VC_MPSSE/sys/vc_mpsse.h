
#include "libMPSSE_spi.h"

#define SPI_CLK_15M    15000000
#define SPI_L_TIMER    2
#define SPI_INIDIR_IN  0
#define SPI_INIVAL_LOW 0
#define SPI_ENDDIR_IN  0
#define SPI_ENDVAL_LOW 0
#define SPI_TXCMD_LEN  3
#define SPI_RXCMD_LEN  4

FT_HANDLE ftHandle;

#define FTMAIN FT32 main (FT32 argc,FT8 *argv[]) 
#define FTDUMMY
#define SYS_END    return 0

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

