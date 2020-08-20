#include "platform.h"

FTVOID rdStart ( FTU32 addr )
{
    FTU8 tmp[SPI_RXCMD_LEN] = {0};
    FTU32 send;

    tmp[0] = (FTU8)(addr >> 16);
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;
    tmp[3] = 0; /* dummy required by MPSSE */

    SPI_Write(ftHandle,tmp,SPI_RXCMD_LEN,&send,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE);
}

FTVOID wrStart ( FTU32 addr )
{
    FTU8 tmp[SPI_TXCMD_LEN] = {0};
    FTU32 send;

    tmp[0] = (FTU8)(0x80 | (addr >> 16));
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;

    SPI_Write(ftHandle,tmp,SPI_TXCMD_LEN,&send,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE);
}

FTVOID HAL_Cfg ( FTU8 cfg )
{
    FTU32 send;
    FTU8 tmp[SPI_TXCMD_LEN] = {0};

    tmp[0] = cfg;

    SPI_Write(ftHandle,tmp,SPI_TXCMD_LEN,&send, 
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
}

FTVOID HAL_Cfg3 ( FTU32 cfg3 )
{
    FTU32 send;
    FTU8 tmp[SPI_TXCMD_LEN] = {0};

    tmp[0] = cfg3;
    tmp[1] = cfg3>>8;
    tmp[2] = cfg3>>16;

    SPI_Write(ftHandle,tmp,SPI_TXCMD_LEN,&send, 
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
}

FTVOID HAL_Write8 ( FTU32 addr, FTU8 data )
{
    FTU32 send;

    wrStart(addr);
    SPI_Write(ftHandle,&data,1,&send,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
}

FTVOID HAL_Write8Src ( FTU32 addr, FTU8 *src, FTU32 len )
{
    FTU32 i;

    wrStart(addr);
    SPI_Write(ftHandle,src,len,&i,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
}

FTVOID HAL_Write16 ( FTU32 addr, FTU16 data )
{
    FTU8 tmp[2] = {0};
    FTU32 send;

    tmp[0] = (FTU8)(data & 0xff);
    tmp[1] = (FTU8)(data >> 8);

    wrStart(addr);
    SPI_Write(ftHandle,tmp,2,&send,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES |
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
}

FTVOID HAL_Write32 ( FTU32 addr, FTU32 data )
{
    FTU8 tmp[4] = {0};
    FTU32 send;

    tmp[0] = (FTU8)data & 0xff;
    tmp[1] = (FTU8)(data >> 8);
    tmp[2] = (FTU8)(data >> 16);
    tmp[3] = (FTU8)(data >> 24);

    wrStart(addr);
    SPI_Write(ftHandle,tmp,4,&send,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES |
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
}

FTU8 HAL_Read8 ( FTU32 addr )
{
    FTU8 tmp[SPI_RXCMD_LEN] = {0};
    FTU32 recv;

    rdStart(addr);

    SPI_Read(ftHandle,tmp,1,&recv,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);

    return tmp[0];
}

FTU32 HAL_Read8Buff ( FTU32 addr, FTU8 *buff, FTU32 len )
{
    FTU32 recv;

    rdStart(addr);

    SPI_Read(ftHandle,buff,len,&recv,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);

    return recv;
}

FTU16 HAL_Read16 ( FTU32 addr )
{
    FTU8 tmp[SPI_RXCMD_LEN] = {0};
    FTU32 recv;

    rdStart(addr);

    SPI_Read(ftHandle,tmp,2,&recv,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);

    return (tmp[0]|(tmp[1]<<8));
}

FTU32 HAL_Read32 ( FTU32 addr )
{
    FTU8 tmp[SPI_RXCMD_LEN+4] = {0};
    FTU32 recv;

    rdStart(addr);

    SPI_Read(ftHandle,tmp,4,&recv,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);

    return (tmp[0]|(tmp[1]<<8)|(tmp[2]<<16)|(tmp[3]<<24));
}

FTVOID HAL_speed_up (FTU32 type)
{
    /* do nothing */
}

FTVOID HAL_PwdCyc ( FTU8 OnOff )
{
    FT_WriteGPIO(ftHandle, 0xBB, OnOff?0x08:0x88);
    FTDELAY(20);

    FT_WriteGPIO(ftHandle, 0xBB, OnOff?0x88:0x08);
    FTDELAY(20);
}

FTVOID HAL_SpiInit ( FTVOID )
{
    ChannelConfig cConf;
	FT_DEVICE_LIST_INFO_NODE devList;
	FTU32 i;

    Init_libMPSSE();

	SPI_GetNumChannels(&i);
	while (i == 0) {
		FTPRINT("\nHAL_SpiInit: no channel");
		/* Endless loop for error*/;	
	}
	
	i = SPI_GetChannelInfo(0,&devList);
	while (FT_OK != i) {
		FTPRINT("\nHAL_SpiInit: no channel info");
		/* Endless loop for error*/;	
	}

	i = SPI_OpenChannel(0,&ftHandle);
	while (FT_OK != i) {
		FTPRINT("\nHAL_SpiInit: fail to open channel");
		/* Endless loop for error*/;	
	}
	
	cConf.ClockRate = SPI_CLK_15M;
	cConf.LatencyTimer= SPI_L_TIMER;
	cConf.configOptions = SPI_CONFIG_OPTION_MODE0 | 
		                  SPI_CONFIG_OPTION_CS_DBUS3 | 
						  SPI_CONFIG_OPTION_CS_ACTIVELOW;
	cConf.Pin = SPI_INIDIR_IN |
		        SPI_INIVAL_LOW |
		        SPI_ENDDIR_IN |
		        SPI_ENDVAL_LOW;

	i = SPI_InitChannel(ftHandle,&cConf);
	while (FT_OK != i) {
		FTPRINT("\nHAL_SpiInit: fail to init channel");
		/* Endless loop for error*/;	
	}
}
