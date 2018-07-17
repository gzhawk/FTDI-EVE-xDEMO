
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include "stm32f4xx_spi.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_flash.h"
#include "stm32f4xx_rng.h"
#include "stm32f4_discovery.h"
#include "eeprom.h"

typedef char         FTC8;
typedef int8_t       FT8;
typedef uint8_t      FTU8;
typedef int16_t      FT16;
typedef uint16_t     FTU16;
typedef int32_t      FT32;
typedef uint32_t     FTU32;
#define FTVOID void

#define USED_CMD_BUF

#define FTMAIN int main (FTVOID) 
#define FTDUMMY FTVOID _exit(FT32 status) {while(1);}

#define STATIC static

#define ROOT_PATH 
#define CDATA_PATH NULL

#define FTPRINT stm32_dumy_print

#ifdef DBG_PRINT
#define DBGPRINT appUI_DbgPrint(__FUNCTION__,__LINE__)
#else
#define DBGPRINT
#endif

#define SYS_INIT stm32f4Init()
#define FTDELAY   stm32f4Delay
/* actually it's a fake function */
#define FTUDELAY(mS)   stm32f4Delay(mS/1000) 

#define CALD_TAG_ADDR 0
#define CALD_TAG_DATA "CALD"
#define CALD_TAG_LEN  4

#define FTRANDOM stm32f4Random

#define EVE_CAL_PARA_NUM    (6)

#define EVE_SPI SPI1
#define EVE_SPI_RCC RCC_AHB1Periph_GPIOD
#define EVE_SPI_GPIO GPIOD
#define EVE_SPI_CS GPIO_Pin_15
#define EVE_CS_HIGH (EVE_SPI_GPIO->BSRRL |= EVE_SPI_CS)
#define EVE_CS_LOW (EVE_SPI_GPIO->BSRRH |= EVE_SPI_CS)

#define APPS_SYS stm32_apps_sys_dummy

#define SYS_END  return 0

FTVOID stm32_dumy_print(char * p);
FTVOID stm32f4Init(FTVOID);
FTVOID stm32f4Delay(FTU32 nTime);
FTVOID stm32f4Timing(FTVOID);
FTU32 stm32f4Random(FTU32 r);
FTVOID stm32f4SPI1Init (FTU16 Prescaler);
FTU8 STM32_SPISend(FTU8 data);
FTU32 stm32f4_is_match (FTU8 *path, FTU8 *name);
FTVOID stm32_apps_sys_dummy (FTU32 para);

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

