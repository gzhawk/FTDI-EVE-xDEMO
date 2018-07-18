#include "platform.h"

#if defined(VC_EMULATOR)
BT8XXEMU_Emulator *gEmulator = NULL;
#endif

FTVOID rdStart ( FTU32 addr )
{
#if defined(VC_MPSSE)
    FTU8 tmp[SPI_RXCMD_LEN] = {0};
    FTU32 send;

    tmp[0] = (FTU8)(addr >> 16);
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;
    tmp[3] = 0; /* dummy required by MPSSE */

    SPI_Write(ftHandle,tmp,SPI_RXCMD_LEN,&send,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE);
#elif defined(VC_EMULATOR)
    FT8XXEMU_cs(1);
    FT8XXEMU_transfer((FTU8)(addr >> 16));
    FT8XXEMU_transfer((FTU8)(addr >> 8));
    FT8XXEMU_transfer((FTU8)addr);
    FT8XXEMU_transfer(0);
#else
#endif
}

FTVOID wrStart ( FTU32 addr )
{
#if defined(VC_MPSSE)
    FTU8 tmp[SPI_TXCMD_LEN] = {0};
    FTU32 send;

    tmp[0] = (FTU8)(0x80 | (addr >> 16));
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;

    SPI_Write(ftHandle,tmp,SPI_TXCMD_LEN,&send,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE);
#elif defined(VC_EMULATOR)
    FT8XXEMU_cs(1);
    FT8XXEMU_transfer((FTU8)(addr >> 16) | 0x80);
    FT8XXEMU_transfer((FTU8)(addr >> 8));
    FT8XXEMU_transfer((FTU8)addr);
#else
#endif
}

FTVOID HAL_Cfg ( FTU8 cfg )
{
#if defined(VC_MPSSE)
    FTU32 send;
    FTU8 tmp[SPI_TXCMD_LEN] = {0};

    tmp[0] = cfg;

    SPI_Write(ftHandle,tmp,SPI_TXCMD_LEN,&send, 
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
#elif defined(VC_EMULATOR)
    FT8XXEMU_transfer(cfg);
#else
#endif
}

FTVOID HAL_Write8 ( FTU32 addr, FTU8 data )
{
#if defined(VC_MPSSE)
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
#elif defined(VC_EMULATOR)
    wrStart(addr);
    FT8XXEMU_transfer(data);
    FT8XXEMU_cs(0);
#else
#endif
}

FTVOID HAL_Write8Src ( FTU32 addr, FTU8 *src, FTU32 len )
{
#if defined(VC_MPSSE)
    FTU32 i;

    wrStart(addr);
    SPI_Write(ftHandle,src,len,&i,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
#elif defined(VC_EMULATOR)
    FTU32 i;

    wrStart(addr);
    for (i = 0; i < len; i++) {
        FT8XXEMU_transfer(src[i]);
    }
    FT8XXEMU_cs(0);
#else
#endif
}

FTVOID HAL_Write16 ( FTU32 addr, FTU16 data )
{
#if defined(VC_MPSSE)
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
#elif defined(VC_EMULATOR)
    wrStart(addr);

    FT8XXEMU_transfer((FTU8)data&0xFF);
    FT8XXEMU_transfer((FTU8)(data>>8)&0xFF);

    FT8XXEMU_cs(0);
#else
#endif
}

FTVOID HAL_Write32 ( FTU32 addr, FTU32 data )
{
#if defined(VC_MPSSE)
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
#elif defined(VC_EMULATOR)
    wrStart(addr);

    FT8XXEMU_transfer((FTU8)data&0xFF);
    FT8XXEMU_transfer((FTU8)(data>>8)&0xFF);
    FT8XXEMU_transfer((FTU8)(data>>16)&0xFF);
    FT8XXEMU_transfer((FTU8)(data>>24)&0xFF);

    FT8XXEMU_cs(0);
#else
#endif
}

FTU8 HAL_Read8 ( FTU32 addr )
{
#if defined(VC_MPSSE)
    FTU8 tmp[SPI_RXCMD_LEN] = {0};
    FTU32 recv;

    rdStart(addr);

    SPI_Read(ftHandle,tmp,1,&recv,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);

    return tmp[0];
#elif defined(VC_EMULATOR)
    FTU8 tmp;

    rdStart(addr);

    tmp = FT8XXEMU_transfer(0);

    FT8XXEMU_cs(0);

    return tmp;
#else
#endif
}

FTU32 HAL_Read8Buff ( FTU32 addr, FTU8 *buff, FTU32 len )
{
#if defined(VC_MPSSE)
    FTU32 recv;

    rdStart(addr);

    SPI_Read(ftHandle,buff,len,&recv,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);

    return recv;
#elif defined(VC_EMULATOR)
    FTU32 tmp = len;
    FTU8 *p = buff;

    rdStart(addr);

    while (tmp--) {
        *p = FT8XXEMU_transfer(0);
        p++;
    }

    FT8XXEMU_cs(0);

    return len;
#else
#endif
}

FTU16 HAL_Read16 ( FTU32 addr )
{
#if defined(VC_MPSSE)
    FTU8 tmp[SPI_RXCMD_LEN] = {0};
    FTU32 recv;

    rdStart(addr);

    SPI_Read(ftHandle,tmp,2,&recv,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);

    return (tmp[0]|(tmp[1]<<8));
#elif defined(VC_EMULATOR)
    FTU16 tmp = 0;

    rdStart(addr);

    tmp = (FTU16)FT8XXEMU_transfer(0);
    tmp |= (FTU16)FT8XXEMU_transfer(0) << 8;

    FT8XXEMU_cs(0);

    return tmp;
#else
#endif
}

FTU32 HAL_Read32 ( FTU32 addr )
{
#if defined(VC_MPSSE)
    FTU8 tmp[SPI_RXCMD_LEN+4] = {0};
    FTU32 recv;

    rdStart(addr);

    SPI_Read(ftHandle,tmp,4,&recv,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);

    return (tmp[0]|(tmp[1]<<8)|(tmp[2]<<16)|(tmp[3]<<24));
#elif defined(VC_EMULATOR)
    FTU32 tmp = 0;

    rdStart(addr);

    tmp = (FTU32)FT8XXEMU_transfer(0);
    tmp |= (FTU32)FT8XXEMU_transfer(0) << 8;
    tmp |= (FTU32)FT8XXEMU_transfer(0) << 16;
    tmp |= (FTU32)FT8XXEMU_transfer(0) << 24;

    FT8XXEMU_cs(0);

    return tmp;
#else
#endif
}

#if defined(VC_EMULATOR)
FTVOID FT8XXEMU_cs(FT8 i)
{
	BT8XXEMU_chipSelect(gEmulator, i);
}

FTU32 FT8XXEMU_transfer(FTU32 data)
{
    return BT8XXEMU_transfer(gEmulator, data);
}
#endif

FTVOID vc_apps_sys_dummy (FTU32 para)
{
    /* do nothing */
}

FTVOID vc_dumy_print (char *p)
{
    /* do nothing */
}

FTVOID HAL_ili9488 (FTVOID)
{
    /* do nothing */
}

FTVOID HAL_speed_up (FTU32 type)
{
    /* do nothing */
}

FTVOID HAL_PwdCyc ( FTU8 OnOff )
{
#if defined(VC_MPSSE)
    FT_WriteGPIO(ftHandle, 0xBB, OnOff?0x08:0x88);
    FTDELAY(20);

    FT_WriteGPIO(ftHandle, 0xBB, OnOff?0x88:0x08);
    FTDELAY(20);
#elif defined(VC_EMULATOR)
    /* do nothing */
#endif
}

FTVOID HAL_SpiInit ( FTVOID )
{
#if defined(VC_MPSSE)
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
#elif defined(VC_EMULATOR)
    /* do nothing */
#endif
}

FTVOID HAL_preparation (FTVOID)
{
    /* do nothing */
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
		FTPRINT("\nHAL_save_cdata: fail to open file");
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
