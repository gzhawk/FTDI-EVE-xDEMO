#include "platform.h"

FTVOID FT4222_CloseAll ( FTVOID )
{
    if (ftHdl_spi) {
        FT_Close(ftHdl_spi);
    }

    if (ftHdl_gpio) {
        FT_Close(ftHdl_gpio);
    }
}

/* as for QSPI write, 
   no API to send the address first, then send data
   it has to be send together
   so use '* addrNdata' to bring the 'address + data' into routine
   but 'datalen' only means the data len*/
FTVOID ft4222_spi_wt (FTU8 * addrNdata, FTU16 datalen)
{
	FTU32 useless32;
	FTU16 useless16;
    FTU8  useless8;

    if (1 == spi_type) {
        FT4222_SPIMaster_SingleWrite(ftHdl_spi, addrNdata, SPI_TXADDR_LEN+datalen,
                &useless16,1);
    } else {
        FT4222_SPIMaster_MultiReadWrite(ftHdl_spi, &useless8, addrNdata, 
                                        0, SPI_TXADDR_LEN+datalen, 0,
                                        &useless32);
    }
}
FTVOID ft4222_spi_rd (FTU8 * addrNdata, FTU16 datalen)
{
	FTU32 useless32;
	FTU16 useless16;
    FTU8  addrbuf[SPI_RXADDR_MLEN] = {0};

    if (1 == spi_type) {
        FT4222_SPIMaster_SingleWrite(ftHdl_spi, addrNdata, SPI_RXADDR_SLEN,
                &useless16,0);

        FT4222_SPIMaster_SingleRead(ftHdl_spi, addrNdata, datalen,
                &useless16,1);
    } else {
        memcpy(addrbuf, addrNdata, SPI_TXADDR_LEN);

        FT4222_SPIMaster_MultiReadWrite(ftHdl_spi, addrNdata, addrbuf, 
                                        0, SPI_RXADDR_MLEN, datalen,
                                        &useless32);
    }
}

FTVOID rdStart ( FTU32 addr )
{
    /* do nothing */
}

FTVOID wrStart ( FTU32 addr )
{
    /* do nothing */
}

FTVOID HAL_Cfg ( FTU8 cfg )
{
    FTU8 tmp[SPI_TXADDR_LEN] = {0};

    tmp[0] = cfg;

    ft4222_spi_wt(tmp, 0);
}

FTVOID HAL_Cfg3 ( FTU32 cfg3 )
{
    FTU8 tmp[SPI_TXADDR_LEN] = {0};

    tmp[0] = cfg3;
    tmp[1] = cfg3>>8;
    tmp[2] = cfg3>>16;

    ft4222_spi_wt(tmp, 0);
}

FTVOID HAL_Write8 ( FTU32 addr, FTU8 data )
{
    FTU8 tmp[SPI_TXADDR_LEN+1] = {0};

    tmp[0] = (FTU8)(0x80 | (addr >> 16));
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;
    tmp[3] = data;
    
    ft4222_spi_wt(tmp, 1);
}

FTVOID HAL_Write8Src ( FTU32 addr, FTU8 *src, FTU32 len )
{
    /* 
       the QSPI read/write API
       do not have holding CS option
       it can only send the addree + data once
       the malloc space for src already shift 3 byte
       while doing the malloc, check HAL_LoopMemMalloc
       for detail
     */
    FTU8 *p = src - 3;

    p[0] = (FTU8)(0x80 | (addr >> 16));
    p[1] = (FTU8)(addr >> 8);
    p[2] = (FTU8)addr;

    ft4222_spi_wt(p, len);
}

FTVOID HAL_Write16 ( FTU32 addr, FTU16 data )
{
    FTU8 tmp[SPI_TXADDR_LEN+2] = {0};

    tmp[0] = (FTU8)(0x80 | (addr >> 16));
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;
    tmp[3] = (FTU8)(data & 0xff);
    tmp[4] = (FTU8)(data >> 8);
    
    ft4222_spi_wt(tmp, 2);
}

FTVOID HAL_Write32 ( FTU32 addr, FTU32 data )
{
    FTU8 tmp[SPI_TXADDR_LEN+4] = {0};

    tmp[0] = (FTU8)(0x80 | (addr >> 16));
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;
    tmp[3] = (FTU8)data & 0xff;
    tmp[4] = (FTU8)(data >> 8);
    tmp[5] = (FTU8)(data >> 16);
    tmp[6] = (FTU8)(data >> 24);

    
    ft4222_spi_wt(tmp, 4);
}

FTU8 HAL_Read8 ( FTU32 addr )
{
    FTU8 tmp[SPI_RXADDR_MLEN] = {0};

    tmp[0] = (FTU8)(addr >> 16);
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;
    ft4222_spi_rd(tmp, 1);

    return tmp[0];
}

FTU32 HAL_Read8Buff ( FTU32 addr, FTU8 *buff, FTU32 len )
{
    buff[0] = (FTU8)(addr >> 16);
    buff[1] = (FTU8)(addr >> 8);
    buff[2] = (FTU8)addr;
    ft4222_spi_rd(buff, len);

    return len;
}

FTU16 HAL_Read16 ( FTU32 addr )
{
    FTU8 tmp[SPI_RXADDR_MLEN] = {0};

    tmp[0] = (FTU8)(addr >> 16);
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;
    ft4222_spi_rd(tmp, 2);

    return (tmp[0]|(tmp[1]<<8));
}

FTU32 HAL_Read32 ( FTU32 addr )
{
    FTU8 tmp[SPI_RXADDR_MLEN] = {0};

    tmp[0] = (FTU8)(addr >> 16);
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;
    ft4222_spi_rd(tmp, 4);

    return (tmp[0]|(tmp[1]<<8)|(tmp[2]<<16)|(tmp[3]<<24));
}

FTVOID HAL_speed_up (FTU32 type)
{
    FT4222_STATUS stat;

    stat = FT4222_SPIMaster_SetLines(ftHdl_spi, type);
    while (FT4222_OK != stat) {
 		FTPRINT("\nHAL_speed_up: fail to speed up spi");
		/* Endless loop for error*/;	
    }
    spi_type = type;
}

FTVOID HAL_PwdCyc ( FTU8 OnOff )
{
    FT4222_GPIO_Write(ftHdl_gpio, GPIO_PORT0, OnOff?0:1);
    FTDELAY(20);
    
    FT4222_GPIO_Write(ftHdl_gpio, GPIO_PORT0, OnOff?1:0);
    FTDELAY(20);
}

FTVOID HAL_SpiInit ( FTVOID )
{
    FT_STATUS stat;
    FT_HANDLE hdl;
    FTU32 devs, i, flag, type, ID, LocID;
    FT8 ser[16], des[64];
    GPIO_Dir gpio_dir[4] = {GPIO_OUTPUT,
                            GPIO_INPUT,
                            GPIO_INPUT,
                            GPIO_INPUT};

    stat = FT_CreateDeviceInfoList(&devs);
	while (FT_OK != stat) {
		FTPRINT("\nHAL_SpiInit: fail to create channel");
		/* Endless loop for error*/;	
	}
	ftHdl_spi = NULL;
	ftHdl_gpio = NULL;
    for (i = 0; i < devs; i++) {
            stat = FT_GetDeviceInfoDetail(i, &flag, &type, &ID, &LocID,
                                          ser, des, &hdl);
            while (FT_OK != stat) {
                FTPRINT("\nHAL_SpiInit: fail to get channel detail");
                /* Endless loop for error*/;	
            }

            if (!strcmp(des, "FT4222 A") && !ftHdl_spi) {
				/* only set the first found */
                stat = FT_OpenEx(des, FT_OPEN_BY_DESCRIPTION, &ftHdl_spi);
                while (FT_OK != stat) {
                    FTPRINT("\nHAL_SpiInit: fail to open spi channel");
                    /* Endless loop for error*/;	
                }
            } else if (!strcmp(des, "FT4222 B") && !ftHdl_gpio) {
				/* only set the first found */
                stat = FT_OpenEx(des, FT_OPEN_BY_DESCRIPTION, &ftHdl_gpio);
                while (FT_OK != stat) {
                    FTPRINT("\nHAL_SpiInit: fail to open gpio channel");
                    /* Endless loop for error*/;	
                }
            } else {
                continue;
            }

    }

    while (!ftHdl_spi || !ftHdl_gpio) {
        FT4222_CloseAll();
        FTPRINT("\nHAL_SpiInit: fail to get channel handle");
        /* Endless loop for error*/;	
    }

    stat = FT_SetTimeouts(ftHdl_spi, SPI_TIMEOUT_R, SPI_TIMEOUT_W);
    while (FT_OK != stat) {
        FT4222_CloseAll();
        FTPRINT("\nHAL_SpiInit: fail to set timeout");
        /* Endless loop for error*/;	
    }

    stat = FT_SetLatencyTimer(ftHdl_spi, SPI_LATENCY);
    while (FT_OK != stat) {
        FT4222_CloseAll();
        FTPRINT("\nHAL_SpiInit: fail to set latency");
        /* Endless loop for error*/;	
    }

    stat = FT4222_SetClock(ftHdl_spi, SPI_SYSCLK);
    while (FT_OK != stat) {
        FT4222_CloseAll();
        FTPRINT("\nHAL_SpiInit: fail to set sys clk");
        /* Endless loop for error*/;	
    }

    stat = FT4222_SPIMaster_Init(ftHdl_spi, SPI_IO_SINGLE,
                                   SPI_SYSDIV, CLK_IDLE_LOW,
                                   CLK_LEADING, SPI_PIN_CS);
    while (FT_OK != stat) {
        FT4222_CloseAll();
        FTPRINT("\nHAL_SpiInit: fail to init spi master");
        /* Endless loop for error*/;	
    }

    stat = FT4222_SPI_SetDrivingStrength(ftHdl_spi, DS_4MA, DS_4MA, DS_4MA);
    while (FT_OK != stat) {
        FT4222_CloseAll();
        FTPRINT("\nHAL_SpiInit: fail to set driving strength");
        /* Endless loop for error*/;	
    }

    FTDELAY(20);

    stat = FT4222_SetSuspendOut(ftHdl_gpio, FALSE);
    while (FT_OK != stat) {
        FT4222_CloseAll();
        FTPRINT("\nHAL_SpiInit: fail to set gpio suspend");
        /* Endless loop for error*/;	
    }

    stat = FT4222_SetWakeUpInterrupt(ftHdl_gpio, FALSE);
    while (FT_OK != stat) {
        FT4222_CloseAll();
        FTPRINT("\nHAL_SpiInit: fail to set gpio interrupt");
        /* Endless loop for error*/;	
    }

    stat = FT4222_GPIO_Init(ftHdl_gpio, gpio_dir);
    while (FT_OK != stat) {
        FT4222_CloseAll();
        FTPRINT("\nHAL_SpiInit: fail to set gpio direction");
        /* Endless loop for error*/;	
    }

	/* at the very first stage, spi has to be single spi */
	spi_type = 1;
}
