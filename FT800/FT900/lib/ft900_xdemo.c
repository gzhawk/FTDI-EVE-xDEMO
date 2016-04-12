
#include "FT800_platform.h"

#if defined(DEF_TIMER)
extern FTVOID timerISR(FTVOID);
#endif

FTVOID ft9xx_gpio_test (FTU8 flag)
{
	if (flag) {
		gpio_write(FT9XX_TST, 0);
		delayms(100);
		gpio_write(FT9XX_TST, 1);
		delayms(100);
		gpio_write(FT9XX_TST, 0);
		delayms(100);
		gpio_write(FT9XX_TST, 1);
	} else {
		gpio_write(FT9XX_TST, 0);
		delayms(50);
		gpio_write(FT9XX_TST, 1);
		delayms(50);
		gpio_write(FT9XX_TST, 0);
		delayms(50);
		gpio_write(FT9XX_TST, 1);
	}
}
FTVOID ft9xx_ili9488_cmd (FTU8 *pdata, FTU32 num)
{
	FT9XX_ILI9488_CS_LOW;
	FT9XX_ILI9488_DCX_LOW;

	spi_writen(SPIM,pdata,1);
	
	FT9XX_ILI9488_DCX_HIGH;

	spi_writen(SPIM,pdata+1,num-1);

	FT9XX_ILI9488_CS_HIGH;
}
FTVOID ft9xx_init_ili9488 (FTVOID)
{
	FTU8 tmp[5] = {0};

	tmp[0] = 1;
	tmp[1] = 0;
	ft9xx_ili9488_cmd(tmp,2);
	usleep(120);

	tmp[0] = 0x2a;
	tmp[1] = 0;
	tmp[2] = 0;
	tmp[3] = 1;
	tmp[4] = 0x3f;
	ft9xx_ili9488_cmd(tmp,5);
	tmp[0] = 0x2b;
	tmp[0] = 0;
	tmp[1] = 0;
	tmp[2] = 1;
	tmp[3] = 0xdf;
	ft9xx_ili9488_cmd(tmp,5);
	tmp[0] = 0xb1;
	tmp[1] = 0xb0;
	ft9xx_ili9488_cmd(tmp,2);
	tmp[0] = 0xf7;
	tmp[1] = 0xa9;
	tmp[2] = 0x51;
	tmp[3] = 0x2c;
	tmp[4] = 0x82;
	ft9xx_ili9488_cmd(tmp,5);
	tmp[0] = 0xb0;
	tmp[1] = 2;
	ft9xx_ili9488_cmd(tmp,2);
	tmp[0] = 0x3a;
	tmp[1] = 0x66;
	ft9xx_ili9488_cmd(tmp,2);
	tmp[0] = 0xe9;
	tmp[1] = 0;
	ft9xx_ili9488_cmd(tmp,2);
	tmp[0] = 0x53;
	tmp[1] = 0x2c;
	ft9xx_ili9488_cmd(tmp,2);
	tmp[0] = 0x36;
	tmp[1] = 0x48;
	ft9xx_ili9488_cmd(tmp,2);
	tmp[0] = 0xb6;
	tmp[1] = 0x30;
	tmp[2] = 2;
	tmp[3] = 0x3b;
	ft9xx_ili9488_cmd(tmp,4);
	tmp[0] = 0x11;
	tmp[1] = 0;
	ft9xx_ili9488_cmd(tmp,2);
	usleep(120);

	tmp[0] = 0x29;
	tmp[1] = 0;
	ft9xx_ili9488_cmd(tmp,2);
	tmp[0] = 0x2c;
	tmp[1] = 0xb0;
	ft9xx_ili9488_cmd(tmp,2);
}
FTVOID ft9xx_int_print (char *p)
{
	sys_enable(sys_device_uart0);
	gpio_function(FT9XX_UART0_TX, pad_uart0_txd); /* UART0 TXD */
	gpio_function(FT9XX_UART0_RX, pad_uart0_rxd); /* UART0 RXD */
	uart_open(UART0,                  /* Device */
			FT9XX_PRESCALER,          /* Prescaler = 1 */
			UART_DIVIDER_9600_BAUD,
			uart_data_bits_8,         /* No. Data Bits */
			uart_parity_none,         /* Parity */
			uart_stop_bits_1);        /* No. Stop Bits */

	uart_puts(UART0,"\n");
	uart_puts(UART0,p);
	uart_puts(UART0,"\n");
}
FTVOID ft9xx_int_timer (FTVOID)
{
#if defined(DEF_TIMER)
/* The Timer Prescaler will divide the 100MHz Master clock down to 2kHz */
#define TIMER_PRESCALER (50000)
#define TIMER_M_SEC     (100000/TIMER_PRESCALER)
#define TIMER_SEC       (1000*TIMER_M_SEC)
#define TIMER_PRI       (17) //why this number? I don't know, copy from sample.
    /* Enable Timers... */
    sys_enable(sys_device_timer_wdt);
    /* Set up the Timer tick to be 0.5 ms... */
    timer_prescaler(TIMER_PRESCALER);
    /* Set up Timer A to be triggered 1 second */
    timer_init(timer_select_a,              /* Device */
               TIMER_SEC,                   /* Initial Value */
               timer_direction_down,        /* Count Direction */
               timer_prescaler_select_on,   /* Prescaler Select */
               timer_mode_continuous);      /* Timer Mode */

    /* Enable the timers... */
    timer_enable_interrupt(timer_select_a);

    /* Register the interrupt... */
    interrupt_attach(interrupt_timers, TIMER_PRI, timerISR);

    /* Start all the timers at the same time... */
    timer_start(timer_select_a);
#endif
}
FTVOID ft9xx_init (FTVOID)
{
	interrupt_enable_globally();

	/* set and pull HIGH the PD pin of EVE */
	gpio_function(FT9XX_PD, pad_gpio43);
	gpio_dir(FT9XX_PD, pad_dir_output);
	gpio_write(FT9XX_PD, 1);	

	/* connect a LED in this GPIO to see 
	 * if the MCU run here */
	gpio_function(FT9XX_TST, pad_gpio54);
	gpio_dir(FT9XX_TST, pad_dir_output);
	gpio_write(FT9XX_TST, 1);

	/* set all the possible functional GPIO to HIGH
	   for better initial status*/
	gpio_function(FT9XX_ILI9488_CS, pad_gpio33);
	gpio_function(FT9XX_SPI_CS_P, pad_gpio28);
	gpio_function(FT9XX_ILI9488_DCX, pad_gpio34);

	gpio_dir(FT9XX_ILI9488_CS, pad_dir_output);
	gpio_dir(FT9XX_SPI_CS_P, pad_dir_output);
	gpio_dir(FT9XX_ILI9488_DCX, pad_dir_output);

	gpio_write(FT9XX_ILI9488_CS, 1);
	gpio_write(FT9XX_SPI_CS_P, 1);
	gpio_write(FT9XX_ILI9488_DCX, 1);

    ft9xx_int_timer();
}

FTVOID ft9xx_spi_init (FTU8 spi_type, FTU32 spi_div)
{
	/* limitation check */
	if (spi_type == 0 || spi_type == 3 || spi_type > 4) {
		uart_puts(UART0,"\n");
		uart_puts(UART0,"SPI type error");
		return;	
	} else if (spi_type == 4 && spi_div < 8) {
		uart_puts(UART0,"\n");
		uart_puts(UART0,"QSPI div too small");
		return;	
	}

	/* receive dummy byte */
	if (spi_type != 1) {
		spi_dummy = 2;
	} else {
		spi_dummy = 1;
	}

	/* basic SPI init */
	sys_enable(sys_device_spi_master);
	/* in order to use the SPI in multiple way (not only for EVE)
	   do not use FT900 integrated CS (pad_spim_ss0),
	   do the CS action by GPIO in other place */
	gpio_function(FT9XX_SPI_SCK, pad_spim_sck);
	gpio_function(FT9XX_SPI_MOSI, pad_spim_mosi);
	gpio_function(FT9XX_SPI_MISO, pad_spim_miso);

	gpio_dir(FT9XX_SPI_SCK, pad_dir_output);
	gpio_dir(FT9XX_SPI_MOSI, pad_dir_output);
	gpio_dir(FT9XX_SPI_MISO, pad_dir_input);

	/* QSPI init */
	if (spi_type == 4) {
		gpio_function(FT9XX_SPI_MOSI_2, pad_spim_io2);
		gpio_function(FT9XX_SPI_MOSI_3, pad_spim_io3);
		gpio_dir(FT9XX_SPI_MOSI_2, pad_dir_output);
		gpio_dir(FT9XX_SPI_MOSI_3, pad_dir_output);
	}

	/* enable SPI: host, mode0, speed */
	spi_init(SPIM, spi_dir_master, spi_mode_0, spi_div);

	/* DSPI, QSPI setting */
	if (spi_type != 1) {
		spi_option(SPIM,spi_option_fifo_size,64);
		spi_option(SPIM,spi_option_fifo,1);
		spi_option(SPIM,spi_option_fifo_receive_trigger,1);
	}

	/* spi bus width */
	spi_option(SPIM,spi_option_bus_width,spi_type);

	/* set all output pins HIGH */
	if (spi_type == 4) {
		gpio_write(FT9XX_SPI_MOSI_2, 1);
		gpio_write(FT9XX_SPI_MOSI_3, 1);
	}
	gpio_write(FT9XX_SPI_SCK, 1);
	gpio_write(FT9XX_SPI_MOSI, 1);
}

FTVOID ft9xx_sdc_init (FTVOID)
{
#define SDC_TRY_RUN 10
#define SDC_TRY_WAIT 10
	FTU8 i = 0;

	sys_enable(sys_device_sd_card);	
	sdhost_init();

	gpio_function(FT9XX_SD_CLK, pad_sd_clk); 
	gpio_pull(FT9XX_SD_CLK, pad_pull_none);
	gpio_function(FT9XX_SD_CMD, pad_sd_cmd); 
	gpio_pull(FT9XX_SD_CMD, pad_pull_pullup);
	gpio_function(FT9XX_SD_DAT3, pad_sd_data3); 
	gpio_pull(FT9XX_SD_DAT3, pad_pull_pullup);
    gpio_function(FT9XX_SD_DAT2, pad_sd_data2); 
	gpio_pull(FT9XX_SD_DAT2, pad_pull_pullup);
	gpio_function(FT9XX_SD_DAT1, pad_sd_data1); 
	gpio_pull(FT9XX_SD_DAT1, pad_pull_pullup);
	gpio_function(FT9XX_SD_DAT0, pad_sd_data0); 
	gpio_pull(FT9XX_SD_DAT0, pad_pull_pullup);
	gpio_function(FT9XX_SD_CD, pad_sd_cd); 
	gpio_pull(FT9XX_SD_CD, pad_pull_pullup);
	gpio_function(FT9XX_SD_WP, pad_sd_wp); 
	gpio_pull(FT9XX_SD_WP, pad_pull_pullup);

	while ((SDC_TRY_RUN != ++i) && (sdhost_card_detect() != SDHOST_CARD_INSERTED)) {
		delayms(SDC_TRY_WAIT);
		uart_puts(UART0,".");
	}

	if(i < SDC_TRY_RUN) {
		if (f_mount(&FT9xxFatFs, "", 0) != FR_OK) {
			uart_puts(UART0,"\nMount Disk Fail");
		} else {
			/* this lib seems not set the fs_type 
			   even it successful mounted */
			if (FT9xxFatFs.fs_type) { 
				uart_puts(UART0,"\nMount Disk Success");
			} else {
				uart_puts(UART0,"\nDisk Found");
			}
		}
	} else {
		uart_puts(UART0,"\nNo SD Card");
	}
		
	return;
}
/* a stub function for libfatfs.a */
FTU64 get_fattime (FTVOID)
{
	return 0;
}

FTVOID ft9xx_invaild_tag (FTC8 *dataPath)
{
	uart_puts(UART0,"\ninvalid the tag");
	f_unlink((const TCHAR*)dataPath);
}

FTU8 ft9xx_is_tag_vaild (FIL *f_hdl, FTC8 *dPath)
{
	if (FR_OK != f_open(f_hdl, (const TCHAR*)dPath, FA_READ)) {
		uart_puts(UART0,"\ntag invalid");
		return 0;
	}
	f_close(f_hdl);
	return 1;
}

FTVOID ft9xx_save_cdata (FIL *f_hdl, FTC8 *dataPath, FTU8 *p)
{
	FTU32 i;

	/* TODO: use FA_CREATE_NEW would make it works,
	   but I just not able to write data to the file
	   so just use FA_WRITE make it return error not create file*/
	if (FR_OK != f_open(f_hdl, (const TCHAR*)dataPath, FA_WRITE)) {
		uart_puts(UART0,"\nfail to open c data file");
		/* don't stop the bootup, 
		   if only can not open file */
		return;
	}

	f_write(f_hdl, (FTVOID *)p,FT800_CAL_PARA_NUM*4,&i);
	f_close(f_hdl);
}

FTVOID ft9xx_restore_cdata (FIL *f_hdl, FTC8 *dPath, FTVOID *p)
{
	FTU32 i;

	f_open(f_hdl, (const TCHAR*)dPath, FA_READ);
	f_read(f_hdl, p,FT800_CAL_PARA_NUM*4,&i);
	f_close(f_hdl);
}

FTVOID ft9xx_vaild_tag (FTVOID)
{
	/* already save it to file, the file is the 'tag' */
	/* give some time to file system to finish operation */
	delayms(100);
}

