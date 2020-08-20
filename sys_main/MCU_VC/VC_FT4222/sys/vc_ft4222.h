
#include "ftd2xx.h"
#include "libFT4222.h"

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
