
#include "stm32_xdemo.h"

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
typedef struct wrFuncPara_ {
	unsigned int res;
	unsigned int Src;
	unsigned int Des;
	unsigned int len;
}wrFuncPara;
register char * stack_ptr asm("sp");
unsigned int TimingDelay;
unsigned short VirtAddVarTab[NB_OF_VAR];
unsigned int FILE_SADDR = 0;

FTVOID stm32_apps_sys_dummy (FTU32 para)
{

}
void stm32_dumy_print(char * p)
{
/* do nothing */
}
void stm32getfileaddr (void)
{
	/* for STM32F407VG internal flash start from 0x8000000 */
	unsigned int p = 0x08000000;
	while (p < 0x080FFFFF) {
		if (FILE_MAGIC == *(unsigned int *)p) {
			FILE_SADDR = p;
			return;
		}
		p += FILE_SEARCH_BLOCK;
	}
}
unsigned int stm32f4filesize(unsigned int p)
{
	wrFuncPara *handle = (wrFuncPara *)p;

	return *(unsigned int *)(handle->res-FILE_LENL);
}
unsigned int stm32f4fileopen(unsigned char *path, unsigned int p)
{
	wrFuncPara *pwfp = (wrFuncPara *)p;
	unsigned int i, num, addr;

	if (FILE_MAGIC != *(unsigned int *)FILE_SADDR) {
		DBGPRINT;
		return 0;
	}

	num = *(unsigned int *)(FILE_SADDR+FILE_MGCL);
	addr = (FILE_SADDR+FILE_MGCL+FILE_NUML);
	
	for (i = 0; i < num; i++) {
		if (stm32f4_is_match(path,(unsigned char *)addr)) {
			if (pwfp) {
				pwfp->res = addr + FILE_NAMEL+FILE_LENL;
			}
			return (unsigned int)pwfp;
		} else {
			addr += (*(unsigned int *)(addr+FILE_NAMEL) + FILE_NAMEL + FILE_LENL);
		}
	}
	
	return 0;
}
unsigned int stm32f4Random(unsigned int r)
{
	while(!RNG_GetFlagStatus(RNG_FLAG_DRDY));
	return (RNG_GetRandomNumber()%(r));
}

void stm32f4Delay(unsigned int nTime)
{ 
	TimingDelay = nTime;

	while(TimingDelay != 0);
}

void stm32f4Timing(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
void stm32f4LED (void)
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
void stm32f4Init(void)
{
	unsigned int i;
	
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
void stm32f4SPI1Init (unsigned short Prescaler)
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
#define SPI_BaudRatePrescaler_2         ((unsigned short)0x0000) //  42 MHz      21 MHZ
#define SPI_BaudRatePrescaler_4         ((unsigned short)0x0008) //  21 MHz      10.5 MHz
#define SPI_BaudRatePrescaler_8         ((unsigned short)0x0010) //  10.5 MHz    5.25 MHz
#define SPI_BaudRatePrescaler_16        ((unsigned short)0x0018) //  5.25 MHz    2.626 MHz  
#define SPI_BaudRatePrescaler_32        ((unsigned short)0x0020) //  2.626 MHz   1.3125 MHz
#define SPI_BaudRatePrescaler_64        ((unsigned short)0x0028) //  1.3125 MHz  656.25 KHz
#define SPI_BaudRatePrescaler_128       ((unsigned short)0x0030) //  656.25 KHz  328.125 KHz
#define SPI_BaudRatePrescaler_256       ((unsigned short)0x0038) //  328.125 KHz 164.06 KHz
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

unsigned char STM32_SPISend(unsigned char data)
{
	while( !SPI_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
	SPI_SendData(SPI1, data);
	while( !SPI_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	return (unsigned char)SPI_ReceiveData(SPI1);
}

unsigned int stm32f4_len (unsigned char *str)
{
	unsigned int i = 0;

	while (str[i] != '\0') {
		i++;
	}

	return i;
}
unsigned int stm32f4_is_match (unsigned char *path, unsigned char *name)
{
	if (stm32f4_len(path) != stm32f4_len(name) ) {
		return 0;
	}

	return memcmp(path,name,stm32f4_len(path))?0:1;
}
void stm32f4_invaild_tag (void)
{
	unsigned short i;

	for (i = CALD_TAG_ADDR; i < CALD_TAG_LEN/2; i++) {
		EE_WriteVariable(i, 0);
	}
}
unsigned char stm32f4_is_tag_vaild (void)
{
	FTU8 i, tag[] = CALD_TAG_DATA, tmp[CALD_TAG_LEN] = {0};

	for (i = 0; i < CALD_TAG_LEN; i+=2) {
		EE_ReadVariable((unsigned short)(CALD_TAG_ADDR+i), (unsigned short *)&tmp[i]);
		if (tag[i] != tmp[i] || tag[i+1] != tmp[i+1]) {
			return 0;
		}
	}

	return 1;
}
void stm32f4_save_cdata (unsigned short* p)
{
	unsigned short i,j;

	for (i = 0, j = 0; i < EVE_CAL_PARA_NUM; i++) {
		j = i*4;
		EE_WriteVariable((CALD_TAG_ADDR+j/2),p[j/2]);
		EE_WriteVariable((CALD_TAG_ADDR+j/2+1),p[j/2+1]);
	}
}
void stm32f4_restore_cdata (unsigned short* p)
{
	unsigned char i;

	for (i = 0;i < EVE_CAL_PARA_NUM;i+=2) {
		EE_ReadVariable((unsigned short)(CALD_TAG_ADDR+CALD_TAG_LEN+i*2), (unsigned short *)(p+i*2));
		EE_ReadVariable((unsigned short)(CALD_TAG_ADDR+CALD_TAG_LEN+i*2+1), (unsigned short *)(p+i*2+1));
	}
}
void stm32f4_vaild_tag (void)
{
	unsigned char tag[] = CALD_TAG_DATA;
	unsigned int i;

	for (i = 0; i < CALD_TAG_LEN/2; i++) {
		EE_WriteVariable(CALD_TAG_ADDR+i,*(unsigned short *)(tag+i));;
	}
}

