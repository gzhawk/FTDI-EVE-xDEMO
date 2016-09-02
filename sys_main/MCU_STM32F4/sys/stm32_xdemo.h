
#include <stdint.h>
#include <stdlib.h>
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

#define FTEND          return 0
#define FTMAIN int main (FTVOID) 
#define FTDUMMY FTVOID _exit(FT32 status) {while(1);}

#define FTINDEF static

#define ROOT_PATH 
#define CDATA_PATH NULL
#ifdef FT800_PRINT
#define FTPRINT   stm32f4Print 
#define DBGPRINT  stm32f4Dbg()
#else
#define FTPRINT stm32_dumy_print
#define DBGPRINT
#endif
#define FTPREINIT stm32f4Init()
#define FTDELAY   stm32f4Delay

#define CALD_TAG_ADDR 0
#define CALD_TAG_DATA "CALD"
#define CALD_TAG_LEN  4

#define FTRANDOM stm32f4Random

#define FTEMU

#define FTRES FTU32

#define FTIOCNTRL(a) stm32f4simu_gpio(a)

#define FT800_CAL_PARA_NUM    (6)

#define FT800_SPI SPI1
#define FT800_SPI_RCC RCC_AHB1Periph_GPIOD
#define FT800_SPI_GPIO GPIOD
#define FT800_SPI_CS GPIO_Pin_15
#define FT800_CS_HIGH (FT800_SPI_GPIO->BSRRL |= FT800_SPI_CS)
#define FT800_CS_LOW (FT800_SPI_GPIO->BSRRH |= FT800_SPI_CS)

#define APPS_SYS stm32_apps_sys_dummy

extern unsigned int FILE_SADDR;
void stm32_dumy_print(char * p);
void stm32f4Print(char * p);
void stm32f4Init(void);
void stm32f4Delay(unsigned int nTime);
void stm32f4Timing(void);
unsigned int stm32f4Random(unsigned int r);
void stm32f4Dbg(void);
void stm32f4SPI1Init (unsigned short Prescaler);
unsigned char STM32_SPISend(unsigned char data);
void stm32f4simu_gpio (unsigned char status);
unsigned int stm32f4_is_match (unsigned char *path, unsigned char *name);
unsigned int stm32f4fileopen(unsigned char *path, unsigned int p);
unsigned int stm32f4filesize(unsigned int p);
void stm32f4_invaild_tag (void);
unsigned char stm32f4_is_tag_vaild (void);
void stm32f4_save_cdata (unsigned short* p);
void stm32f4_restore_cdata (unsigned short* p);
void stm32f4_vaild_tag (void);
FTVOID stm32_apps_sys_dummy (FTU32 para);
