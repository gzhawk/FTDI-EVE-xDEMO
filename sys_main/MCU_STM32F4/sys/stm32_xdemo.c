#include "platform.h"

#define XTAL            (168000000UL)    /* Oscillator frequency             */
#define SYSCLK 4*XTAL
/* the very first file storage starting flag 
 * storage format should be:
 * FILE_SADDR       0x557EE7AA (4bytes)
 * FILE_SADDR+04    file number(4bytes)
 * FILE_SADDR+08    file1 name (8bytes)
 * FILE_SADDR+16    file1 len  (4bytes)
 * FILE_SADDR+20    file1 data (YYbytes)
 * FILE_SADDR+20+YY file2 name (8bytes)
 * FILE_SADDR+20+YY file2 len  (4bytes)
 * FILE_SADDR+20+YY file2 data (KKbytes)
 * */
#define FILE_MAGIC 0x557EE7AA 
#define FILE_MGCL  4
#define FILE_NUML  4
#define FILE_NAMEL 8
#define FILE_LENL  4
#define FILE_SEARCH_BLOCK 512

FTU16 VirtAddVarTab[NB_OF_VAR];
FTU32 TimingDelay;
FTU32 FILE_SADDR = 0;
FTU32 gfile_addr = 0;

register char * stack_ptr asm("sp");

STATIC FTVOID rdStart ( FTU32 addr )
{
    EVE_CS_LOW;
    STM32_SPISend((FTU8)(addr >> 16));
    STM32_SPISend((FTU8)(addr >> 8));
    STM32_SPISend((FTU8)addr);
    STM32_SPISend(0); //Dummy byte to generate read clock
}

STATIC FTVOID wrStart ( FTU32 addr )
{
    EVE_CS_LOW;
    STM32_SPISend((FTU8)(0x80 | (addr >> 16)));
    STM32_SPISend((FTU8)(addr >> 8));
    STM32_SPISend((FTU8)addr);
}

FTVOID HAL_Write8 ( FTU32 addr, FTU8 data )
{
    wrStart(addr);

    STM32_SPISend(data);

    while( SPI_GetFlagStatus(EVE_SPI, SPI_I2S_FLAG_BSY));
    EVE_CS_HIGH;
}

FTVOID HAL_Write8Src ( FTU32 addr, FTU8 *src, FTU32 len )
{
    FTU32 i;

    wrStart(addr);

    for (i = 0; i < len; i++) {
        STM32_SPISend(src[i]);
    }

    while( SPI_GetFlagStatus(EVE_SPI, SPI_I2S_FLAG_BSY));
    EVE_CS_HIGH;
}

FTVOID HAL_Write16 ( FTU32 addr, FTU16 data )
{
    wrStart(addr);

    STM32_SPISend((FTU8)data&0xFF);
    STM32_SPISend((FTU8)(data>>8)&0xFF);

    while( SPI_GetFlagStatus(EVE_SPI, SPI_I2S_FLAG_BSY));
    EVE_CS_HIGH;
}

FTVOID HAL_Write32 ( FTU32 addr, FTU32 data )
{
    wrStart(addr);

    STM32_SPISend((FTU8)data&0xFF);
    STM32_SPISend((FTU8)(data>>8)&0xFF);
    STM32_SPISend((FTU8)(data>>16)&0xFF);
    STM32_SPISend((FTU8)(data>>24)&0xFF);

    while( SPI_GetFlagStatus(EVE_SPI, SPI_I2S_FLAG_BSY));
    EVE_CS_HIGH;
}

FTVOID HAL_Cfg ( FTU8 cfg )
{
    EVE_CS_LOW;

    STM32_SPISend(cfg);

    STM32_SPISend(0);

    STM32_SPISend(0);

    while( SPI_GetFlagStatus(EVE_SPI, SPI_I2S_FLAG_BSY));
    EVE_CS_HIGH;
}

FTU8 HAL_Read8 ( FTU32 addr )
{
    FTU8 tmp;

    rdStart(addr);
    tmp = STM32_SPISend(0);

    EVE_CS_HIGH;

    return tmp;
}

FTU32 HAL_Read8Buff ( FTU32 addr, FTU8 *buff, FTU32 len )
{
    FTU32 tmp = len;
    FTU8 *p = buff;

    rdStart(addr);

    while (tmp--) {
        *p = STM32_SPISend(0);
        p++;
    }

    EVE_CS_HIGH;

    return len;
}

FTU16 HAL_Read16 ( FTU32 addr )
{
    FTU16 tmp = 0;

    rdStart(addr);

    /* force type definition is critical while running on different platform */
    tmp = (FTU16)STM32_SPISend(0);
    tmp |= (FTU16)STM32_SPISend(0) << 8;

    while( SPI_GetFlagStatus(EVE_SPI, SPI_I2S_FLAG_BSY));
    EVE_CS_HIGH;

    return tmp;
}

FTU32 HAL_Read32 ( FTU32 addr )
{
    FTU32 tmp = 0;

    rdStart(addr);

    /* force type definition is critical while running on different platform */
    tmp = (FTU32)STM32_SPISend(0);
    tmp |= (FTU32)STM32_SPISend(0) << 8;
    tmp |= (FTU32)STM32_SPISend(0) << 16;
    tmp |= (FTU32)STM32_SPISend(0) << 24;

    while( SPI_GetFlagStatus(EVE_SPI, SPI_I2S_FLAG_BSY));
    EVE_CS_HIGH;

    return tmp;
}

FTVOID stm32_apps_sys_dummy (FTU32 para)
{
/* do nothing */
}

FTVOID stm32_dumy_print(char * p)
{
/* do nothing */
}

FTVOID stm32getfileaddr (FTVOID)
{
	/* for STM32F407VG internal flash start from 0x8000000 */
	FTU32 p = 0x08000000;
	while (p < 0x080FFFFF) {
		if (FILE_MAGIC == *(FTU32 *)p) {
			FILE_SADDR = p;
			return;
		}
		p += FILE_SEARCH_BLOCK;
	}
}

FTU32 stm32f4Random(FTU32 r)
{
	while(!RNG_GetFlagStatus(RNG_FLAG_DRDY));
	return (RNG_GetRandomNumber()%(r));
}

FTVOID stm32f4Delay(FTU32 nTime)
{ 
	TimingDelay = nTime;

	while(TimingDelay != 0);
}

FTVOID stm32f4Timing(FTVOID)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

FTVOID stm32f4LED (FTVOID)
{
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);

	STM_EVAL_LEDOff(LED3);
	STM_EVAL_LEDOff(LED4);
	STM_EVAL_LEDOff(LED6);
	STM_EVAL_LEDOff(LED5);

	STM_EVAL_LEDOn(LED4);
	STM_EVAL_LEDOn(LED5);
	stm32f4Delay(500);
	STM_EVAL_LEDOff(LED4);
	STM_EVAL_LEDOff(LED5);
	stm32f4Delay(500);
	STM_EVAL_LEDOn(LED4);
	STM_EVAL_LEDOn(LED5);
	stm32f4Delay(500);
	STM_EVAL_LEDOff(LED4);
	STM_EVAL_LEDOff(LED5);
	stm32f4Delay(500);

}

FTVOID stm32f4Init(FTVOID)
{
	FTU32 i;
	
	if (SysTick_Config(SYSCLK / 5000))
	{ 
		/* Capture error */ 
		while (1);
	}

	stm32getfileaddr();

	/* Random number generator init */
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);

	RNG_Cmd(ENABLE);
	
	/* EEPROM Init */
	for (i = 0; i < NB_OF_VAR; i++) {
		VirtAddVarTab[i] = i;
	}

	EE_Init();
	
	stm32f4LED();
}

FTVOID stm32f4SPI1Init (FTU16 Prescaler)
{
	SPI_InitTypeDef spi_st;
	GPIO_InitTypeDef gpio_st;

	/* Init SCK, MISO, MOSI */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/*
	 * (STM32F4 Discovery)
	 * SCK : PA5 pin30 
	 * MISO: PA6 pin31
	 * MOSI: PA7 pin32
	 */
	gpio_st.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	gpio_st.GPIO_Mode  = GPIO_Mode_AF;
	gpio_st.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_st.GPIO_OType = GPIO_OType_PP;
	gpio_st.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA, &gpio_st);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	/* Init SPI1 CS */
	GPIOD->BSRRL |= GPIO_Pin_15;

	/* Init SPI1 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	spi_st.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	spi_st.SPI_Mode              = SPI_Mode_Master;
	spi_st.SPI_DataSize          = SPI_DataSize_8b;
	spi_st.SPI_NSS               = SPI_NSS_Soft;
	spi_st.SPI_FirstBit          = SPI_FirstBit_MSB;
	/* SPI Mode 0: CPOL-0, CPHA-0 */
	spi_st.SPI_CPOL              = SPI_CPOL_Low;
	spi_st.SPI_CPHA              = SPI_CPHA_1Edge;
/*
   Peripherial Clock 42MHz SPI2 SPI3
   Peripherial Clock 84MHz SPI1                                SPI1        SPI2/3
#define SPI_BaudRatePrescaler_2         ((FTU16)0x0000) //  42 MHz      21 MHZ
#define SPI_BaudRatePrescaler_4         ((FTU16)0x0008) //  21 MHz      10.5 MHz
#define SPI_BaudRatePrescaler_8         ((FTU16)0x0010) //  10.5 MHz    5.25 MHz
#define SPI_BaudRatePrescaler_16        ((FTU16)0x0018) //  5.25 MHz    2.626 MHz  
#define SPI_BaudRatePrescaler_32        ((FTU16)0x0020) //  2.626 MHz   1.3125 MHz
#define SPI_BaudRatePrescaler_64        ((FTU16)0x0028) //  1.3125 MHz  656.25 KHz
#define SPI_BaudRatePrescaler_128       ((FTU16)0x0030) //  656.25 KHz  328.125 KHz
#define SPI_BaudRatePrescaler_256       ((FTU16)0x0038) //  328.125 KHz 164.06 KHz
*/
	spi_st.SPI_BaudRatePrescaler = Prescaler;

	SPI_Init(SPI1, &spi_st);

	SPI_Cmd(SPI1, ENABLE);	
}

caddr_t _sbrk(int incr)
{
	extern char end asm("end");
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0)
		heap_end = &end;

	prev_heap_end = heap_end;
	if (heap_end + incr > stack_ptr)
	{
		return (caddr_t) -1;
	}

	heap_end += incr;

	return (caddr_t) prev_heap_end;
}

FTU8 STM32_SPISend(FTU8 data)
{
	while( !SPI_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
	SPI_SendData(SPI1, data);
	while( !SPI_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	return (FTU8)SPI_ReceiveData(SPI1);
}

FTU32 stm32f4_len (FTU8 *str)
{
	FTU32 i = 0;

	while (str[i] != '\0') {
		i++;
	}

	return i;
}

FTU32 stm32f4_is_match (FTU8 *path, FTU8 *name)
{
	if (stm32f4_len(path) != stm32f4_len(name) ) {
		return 0;
	}

	return memcmp(path,name,stm32f4_len(path))?0:1;
}

FTVOID HAL_invaild_tag (FTC8 *dataPath)
{
	FTU16 i;

	for (i = CALD_TAG_ADDR; i < CALD_TAG_LEN/2; i++) {
		EE_WriteVariable(i, 0);
	}
}

FTU8 HAL_is_tag_vaild (FTC8 *dataPath)
{
	FTU8 i, tag[] = CALD_TAG_DATA, tmp[CALD_TAG_LEN] = {0};

	for (i = 0; i < CALD_TAG_LEN; i+=2) {
		EE_ReadVariable((FTU16)(CALD_TAG_ADDR+i), (FTU16 *)&tmp[i]);
		if (tag[i] != tmp[i] || tag[i+1] != tmp[i+1]) {
			return 0;
		}
	}

	return 1;
}

FTVOID HAL_save_cdata (FTC8 *dataPath, FTU8 *p)
{
	FTU16 i,j;

	for (i = 0, j = 0; i < EVE_CAL_PARA_NUM; i++) {
		j = i*4;
		EE_WriteVariable((CALD_TAG_ADDR+j/2),p[j/2]);
		EE_WriteVariable((CALD_TAG_ADDR+j/2+1),p[j/2+1]);
	}
}

FTVOID HAL_restore_cdata (FTC8 *dataPath, FTU8 *p)
{
	FTU8 i;

	for (i = 0;i < EVE_CAL_PARA_NUM;i+=2) {
		EE_ReadVariable((FTU16)(CALD_TAG_ADDR+CALD_TAG_LEN+i*2), (FTU16 *)(p+i*2));
		EE_ReadVariable((FTU16)(CALD_TAG_ADDR+CALD_TAG_LEN+i*2+1), (FTU16 *)(p+i*2+1));
	}
}

FTVOID HAL_vaild_tag (FTVOID)
{
	FTU8 tag[] = CALD_TAG_DATA;
	FTU32 i;

	for (i = 0; i < CALD_TAG_LEN/2; i++) {
		EE_WriteVariable(CALD_TAG_ADDR+i,*(FTU16 *)(tag+i));;
	}
}

FTVOID HAL_ili9488 (FTVOID)
{
//do nothing;
}

FTVOID HAL_speed_up (FTU32 type)
{
//do nothing;
}

FTVOID HAL_PwdCyc ( FTU8 OnOff )
{
#define PWC_DELAY 20
    /* Share the PD13/LED3 pin with PD pin of EVE */
    STM_EVAL_LEDOff(LED3);
    FTDELAY(PWC_DELAY);

    STM_EVAL_LEDOn(LED3);
    FTDELAY(PWC_DELAY);
}

FTVOID HAL_SpiInit ( FTVOID )
{
    /* 
       the SPI clock shall not exceed 11MHz before system clock is enabled. 
       After system clock is properly enabled, 
       the SPI clock is allowed to go up to 30MHz.	
     */
    stm32f4SPI1Init(SPI_BaudRatePrescaler_8);
}

FTVOID HAL_preparation (FTVOID)
{
    //do nothing
}

FTU32 HAL_WriteSrcToDes (FTU32 handle, FTU32 src, FTU32 des, FTU32 len)
{
    FTU32 i;

    for (i = 0; i < len ; i++) {
        *(FTU8 *)(des+i) = *(FTU8 *)(handle+src+i);
    }
    
    return i;
}

FTU8 * HAL_LoopMemMalloc (FTU32 handle, FTU32 src, FTU32 len)
{
    return (FTU8 *)handle+src;
}

FTVOID HAL_LoopMemRead (FTU32 handle, FTU8 **ppbuf, FTU32 len)
{
    *ppbuf += len;
}

FTVOID HAL_LoopMemFree (FTU32 buf)
{
    // do nothing
}

FTU32 HAL_SegFileOpen (FTU8 *path)
{
	FTU32 i, num, addr;

	if (FILE_MAGIC != *(FTU32 *)FILE_SADDR) {
		return 0;
	}

	num = *(FTU32 *)(FILE_SADDR+FILE_MGCL);
	addr = (FILE_SADDR+FILE_MGCL+FILE_NUML);
	
	for (i = 0; i < num; i++) {
		if (stm32f4_is_match(path,(FTU8 *)addr)) {
			gfile_addr = addr + FILE_NAMEL+FILE_LENL;
			return gfile_addr;
		} else {
			addr += (*(FTU32 *)(addr+FILE_NAMEL) + FILE_NAMEL + FILE_LENL);
		}
	}
	
	return 0;
}

FTU32 HAL_SegFileSize (FTU32 handle)
{
	return *(FTU32 *)(handle-FILE_LENL);
}

FTVOID HAL_SegFileClose (FTU32 handle)
{
    // do nothing
}

FTU8 HAL_ZlibCompressed (FTU32 handle, FTU32 src)
{
    FTU8 header[2] = {0};

    header[0] = *(FTU8 *)(handle+src);
    header[1] = *(FTU8 *)(handle+src+1);
    /*
       zLib compression header
       +---+---+
       |CMF|FLG|
       +---+---+

       78 01 - No Compression/low
       78 9C - Default Compression
       78 DA - Best Compression 
     */
    if (header[0] == 0x78 && header[1] == 0x9C) {
        return 1;
    } else {
        return 0;
    }
}
