#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "ft900.h"
#include "ff.h"
typedef const char     FTC8;
typedef signed char    FT8;
typedef unsigned char  FTU8;
typedef short          FT16;
typedef unsigned short FTU16;
typedef int            FT32;
typedef unsigned int   FTU32;
typedef long           FT64;
typedef unsigned long  FTU64;
#define FTVOID void

#define USED_CMD_BUF

#define FTMAIN int main (FTVOID) 
#define FTDUMMY

#define STATIC static

#define ROOT_PATH 
#define CDATA_PATH "cdata.bin"

#ifdef DBG_PRINT
/* if stdio.h be included, printf is usable, 
   but uart_puts would cost less image space */
#define FTPRINT(a) uart_puts(UART0,a) 
#define DBGPRINT appUI_DbgPrint(__FUNCTION__,__LINE__)
#else
#define FTPRINT ft9xx_dumy_print
#define DBGPRINT
#endif

#define SYS_INIT ft9xx_int_print(APPS_VER)
#define FTDELAY   delayms

#define CALD_TAG_ADDR 0
#define CALD_TAG_DATA "CALD"
#define CALD_TAG_LEN  4

#define FTRANDOM(M) (rand()%(M))

#define FTRES FIL* 

#define EVE_CAL_PARA_NUM (6)

#define FT9XX_UART0_RX 49
#define FT9XX_UART0_TX 48
#define FT9XX_PRESCALER 1

#define FT9XX_SPI_SCK  27
#define FT9XX_SPI_CHIP  0
#define FT9XX_SPI_CS_P 28
#define FT9XX_SPI_MOSI 29
#define FT9XX_SPI_MOSI_2 31
#define FT9XX_SPI_MOSI_3 32
#define FT9XX_SPI_MISO 30

#define FT9XX_SPI_TXLEN 3
#define FT9XX_SPI_RXLEN 3

#define FT9XX_PD       43

#define FT9XX_TST      54/* MM900EV1A CN3 pin3 use it for bootup debug when no UART available*/

#define FT9XX_SD_CLK  (19)
#define FT9XX_SD_CMD  (20)
#define FT9XX_SD_DAT3 (21)
#define FT9XX_SD_DAT2 (22)
#define FT9XX_SD_DAT1 (23)
#define FT9XX_SD_DAT0 (24)
#define FT9XX_SD_CD   (25)
#define FT9XX_SD_WP   (26)

#define FT9XX_ILI9488_CS   (33)
#define FT9XX_ILI9488_DCX  (34)

#define FT9XX_CS_HIGH gpio_write(FT9XX_SPI_CS_P, 1)//spi_close(SPIM,FT9XX_SPI_CHIP)
#define FT9XX_CS_LOW  gpio_write(FT9XX_SPI_CS_P, 0)//spi_open(SPIM,FT9XX_SPI_CHIP)

#define FT9XX_ILI9488_CS_HIGH gpio_write(FT9XX_ILI9488_CS, 1)
#define FT9XX_ILI9488_CS_LOW  gpio_write(FT9XX_ILI9488_CS, 0)

#define FT9XX_ILI9488_DCX_HIGH gpio_write(FT9XX_ILI9488_DCX, 1)
#define FT9XX_ILI9488_DCX_LOW  gpio_write(FT9XX_ILI9488_DCX, 0)

#define APPS_SYS ft9xx_apps_sys_dummy

#if defined(UI_BOMS_HID)
#include "ft900_xdemo_boms_hid.h" 
#endif

#define SYS_END  while (1) {APPS_SYS(appGP.appPara);} \
                 return 0

/* default 1 dummy byte */
unsigned char spi_dummy;
FATFS FT9xxFatFs;
FIL FT9xxFile;
FTVOID ft9xx_dumy_print (char *p);
void ft9xx_init_ili9488 (void);
void ft9xx_init (void);
void ft9xx_spi_init (unsigned char spi_type, unsigned int spi_div);
void ft9xx_int_print (char *p);
void ft9xx_sdc_init (void);
unsigned char ft9xx_is_tag_vaild (FIL *f_hdl, const char *dPath);
void ft9xx_invaild_tag (const char *dataPath);
void ft9xx_save_cdata (FIL *f_hdl, const char *dataPath, unsigned char *p);
void ft9xx_restore_cdata (FIL *f_hdl, const char *dataPath, void *p);
void ft9xx_vaild_tag (void);
FTVOID ft9xx_apps_sys_dummy (FTU32 para);

