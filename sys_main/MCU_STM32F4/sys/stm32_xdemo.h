
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

#define CALD_TAG_ADDR 0
#define CALD_TAG_DATA "CALD"
#define CALD_TAG_LEN  4

#define FTRANDOM stm32f4Random

#define FTRES FTU32

#define EVE_CAL_PARA_NUM    (6)

#define EVE_SPI SPI1
#define EVE_SPI_RCC RCC_AHB1Periph_GPIOD
#define EVE_SPI_GPIO GPIOD
#define EVE_SPI_CS GPIO_Pin_15
#define EVE_CS_HIGH (EVE_SPI_GPIO->BSRRL |= EVE_SPI_CS)
#define EVE_CS_LOW (EVE_SPI_GPIO->BSRRH |= EVE_SPI_CS)

#define APPS_SYS stm32_apps_sys_dummy

#define SYS_END  return 0

extern unsigned int FILE_SADDR;
void stm32_dumy_print(char * p);
void stm32f4Init(void);
void stm32f4Delay(unsigned int nTime);
void stm32f4Timing(void);
unsigned int stm32f4Random(unsigned int r);
void stm32f4SPI1Init (unsigned short Prescaler);
unsigned char STM32_SPISend(unsigned char data);
unsigned int stm32f4_is_match (unsigned char *path, unsigned char *name);
unsigned int stm32f4fileopen(unsigned char *path, unsigned int p);
unsigned int stm32f4filesize(unsigned int p);
void stm32f4_invaild_tag (void);
unsigned char stm32f4_is_tag_vaild (void);
void stm32f4_save_cdata (unsigned short* p);
void stm32f4_restore_cdata (unsigned short* p);
void stm32f4_vaild_tag (void);
FTVOID stm32_apps_sys_dummy (FTU32 para);
