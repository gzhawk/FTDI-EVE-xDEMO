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

FTVOID HAL_Write8 ( FTU32 addr, FTU8 data )
{
    FTU8 tmp[SPI_TXCMD_LEN+1] = {0};
    FTU32 send;

    tmp[0] = (FTU8)(0x80 | (addr >> 16));
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;
    tmp[3] = data;

    SPI_Write(ftHandle,tmp,SPI_TXCMD_LEN+1,&send,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE | 
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
    FTU8 tmp[SPI_TXCMD_LEN+2] = {0};
    FTU32 send;

    tmp[0] = (FTU8)(0x80 | (addr >> 16));
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;
    tmp[3] = (FTU8)(data & 0xff);
    tmp[4] = (FTU8)(data >> 8);

    SPI_Write(ftHandle,tmp,SPI_TXCMD_LEN+2,&send,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
}

FTVOID HAL_Write32 ( FTU32 addr, FTU32 data )
{
    FTU8 tmp[SPI_TXCMD_LEN+4] = {0};
    FTU32 send;

    tmp[0] = (FTU8)(0x80 | (addr >> 16));
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;
    tmp[3] = (FTU8)data & 0xff;
    tmp[4] = (FTU8)(data >> 8);
    tmp[5] = (FTU8)(data >> 16);
    tmp[6] = (FTU8)(data >> 24);

    SPI_Write(ftHandle,tmp,SPI_TXCMD_LEN+4,&send,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE | 
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

FTVOID vc2010_apps_sys_dummy (FTU32 para)
{
/*do nothing*/
}

FTVOID vc2010_dumy_print(char *p)
{
/*do nothing*/
}

FTVOID HAL_invaild_tag (FTC8 *dataPath)
{
	remove(dataPath);
}

FTU8 HAL_is_tag_vaild (FTC8 *dataPath)
{
	FILE *pF;
	
	pF = fopen(dataPath,"rb");
	if(NULL == pF) {
		return 0;
	}
	fclose(pF);
	return 1;
}

FTVOID HAL_save_cdata (FTC8 *dataPath, FTU8 *p)
{
	FILE *pF;
	
	pF = fopen(dataPath,"wb");
	if(NULL == pF) {
		FTPRINT("\nvc2010_save_cdata: fail to open file");
		/* don't stop the bootup, 
		   if only can not open file */
		return;
	}

	fwrite(p, 4, EVE_CAL_PARA_NUM, pF);
	fclose(pF);
}

FTVOID HAL_restore_cdata (FTC8 *dataPath, FTU8 *p)
{
	FILE *pF;
	
	pF = fopen(dataPath,"rb");
	fread(p, 4, EVE_CAL_PARA_NUM, pF);
	fclose(pF);
}

FTVOID HAL_vaild_tag (FTVOID)
{
	/* already save it to file, the file is the 'tag' */
	/* give some time to file system to finish operation */
	FTDELAY(100);
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
    FT_WriteGPIO(ftHandle, 0xBB, OnOff?0x08:0x88);
    FTDELAY(PWC_DELAY);

    FT_WriteGPIO(ftHandle, 0xBB, OnOff?0x88:0x08);
    FTDELAY(PWC_DELAY);
}

FTVOID HAL_SpiInit ( FTVOID )
{
    /* 
       the SPI clock shall not exceed 11MHz before system clock is enabled. 
       After system clock is properly enabled, 
       the SPI clock is allowed to go up to 30MHz.	
     */

    ChannelConfig cConf;
	FT_DEVICE_LIST_INFO_NODE devList;
	FTU32 i;

    Init_libMPSSE();

	SPI_GetNumChannels(&i);
	while (i == 0) {
		FTPRINT("\nvc2010_spi_init: no channel");
		/* Endless loop for error*/;	
	}
	
	i = SPI_GetChannelInfo(0,&devList);
	while (FT_OK != i) {
		FTPRINT("\nvc2010_spi_init: no channel info");
		/* Endless loop for error*/;	
	}

	i = SPI_OpenChannel(0,&ftHandle);
	while (FT_OK != i) {
		FTPRINT("\nvc2010_spi_init: fail to open channel");
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
		FTPRINT("\nvc2010_spi_init: fail to init channel");
		/* Endless loop for error*/;	
	}
}

FTVOID HAL_preparation (FTVOID)
{
    //do nothing
}

FTU32 HAL_WriteSrcToDes (FTU32 handle, FTU32 src, FTU32 des, FTU32 len)
{
    fseek((FILE *)handle, src, SEEK_SET);

    return fread((FTVOID *)des, 1, len, (FILE *)handle);
}

FTU8 * HAL_LoopMemMalloc (FTU32 handle, FTU32 src, FTU32 len)
{
    return (FTU8 *)malloc(len);
}

FTVOID HAL_LoopMemRead (FTU32 handle, FTU8 **ppbuf, FTU32 len)
{
    fread(*ppbuf, 1, len, (FILE *)handle);
}

FTVOID HAL_LoopMemFree (FTU32 buf)
{
    free((FTVOID *)buf);
}

FTU32 HAL_SegFileOpen (FTU8 *path)
{
    FILE *pFile;

    if(NULL == path)
    {  
        FTPRINT("\nHAL_SegFileOpen: path null");
        return 0;
    }

    pFile = fopen((FTC8 *)path,"rb");
    if(NULL == pFile)
    {  
        FTPRINT("\nHAL_SegFileOpen: file open error");
        return 0;
    }

    return (FTU32)pFile;
}

FTU32 HAL_SegFileSize (FTU32 handle)
{
    FTU32 size;

    fseek((FILE*)handle,0,SEEK_END);

    size = ftell((FILE*)handle);
    
    fseek((FILE*)handle,0,SEEK_SET);

    return BYTES4ALIGN(size);
    //return (((size) + 3) & 0xFFFFFFFC);
}

FTVOID HAL_SegFileClose (FTU32 handle)
{
    fclose((FILE*)handle);
}

FTU8 HAL_ZlibCompressed (FTU32 handle, FTU32 src)
{
    FTU8 header[2] = {0};
    
    fread(header, 1, 2, (FILE*)handle);
    fseek((FILE*)handle, src, SEEK_SET);
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
