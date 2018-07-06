/* 
    Hardware Abstract Layer for EVE 
    Author: Hawk
    Email : hawk.gao@ftdichip.com	
    Date  : 2013/Oct
 */
#include "platform.h"
#include "EVE_Gpu.h"
#include "EVE_HAL.h"
#include "EVE_APP.h"

FTU32 * mcuCMDBuf = 0;
FTU32 mcuCMDBufSize = 0;
FTU32 mcuCMDindex = 0; 
FTU32 mcuDLindex = 0;

#if defined(USED_CMD_BUF)
FTU8 mem_pool[USE_STATIC_MEM_LEN+FTU32_LEN] = {0};
#endif

STATIC FTVOID rdStart ( FTU32 addr )
{
#ifdef MSVC2010EXPRESS
    FTU8 tmp[SPI_RXCMD_LEN] = {0};
    FTU32 send;

    tmp[0] = (FTU8)(addr >> 16);
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;
    tmp[3] = 0; /* dummy required by MPSSE */

    SPI_Write(ftHandle,tmp,SPI_RXCMD_LEN,&send,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE);
#elif defined(MSVC2012EMU) || defined(MSVC2017EMU)
    FT8XXEMU_cs(1);
    FT8XXEMU_transfer((FTU8)(addr >> 16));
    FT8XXEMU_transfer((FTU8)(addr >> 8));
    FT8XXEMU_transfer((FTU8)addr);
    FT8XXEMU_transfer(0);
#elif defined(STM32F4)
    EVE_CS_LOW;
    STM32_SPISend((FTU8)(addr >> 16));
    STM32_SPISend((FTU8)(addr >> 8));
    STM32_SPISend((FTU8)addr);
    STM32_SPISend(0); //Dummy byte to generate read clock
#elif defined(FT9XXEV)
    FTU8 tmp[FT9XX_SPI_RXLEN] = {0};

    tmp[0] = (FTU8)(addr >> 16);
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;

    FT9XX_CS_LOW;
    spi_writen(SPIM,tmp,FT9XX_SPI_RXLEN);
#else
    digitalWrite(EVE_SPI_CS, LOW);
    SPI.transfer((FTU8)(addr >> 16));
    SPI.transfer(highByte(addr));
    SPI.transfer(lowByte(addr));

    SPI.transfer(0); //Dummy Read Byte
#endif
}

STATIC FTVOID wrStart ( FTU32 addr )
{
#ifdef MSVC2010EXPRESS
    FTU8 tmp[SPI_TXCMD_LEN] = {0};
    FTU32 send;

    tmp[0] = (FTU8)(0x80 | (addr >> 16));
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;

    SPI_Write(ftHandle,tmp,SPI_TXCMD_LEN,&send,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE);
#elif defined(MSVC2012EMU) || defined(MSVC2017EMU)
    FT8XXEMU_cs(1);
    FT8XXEMU_transfer((FTU8)(addr >> 16) | 0x80);
    FT8XXEMU_transfer((FTU8)(addr >> 8));
    FT8XXEMU_transfer((FTU8)addr);
#elif defined(STM32F4)
    EVE_CS_LOW;
    STM32_SPISend((FTU8)(0x80 | (addr >> 16)));
    STM32_SPISend((FTU8)(addr >> 8));
    STM32_SPISend((FTU8)addr);
#elif defined(FT9XXEV)
    FTU8 tmp[FT9XX_SPI_TXLEN] = {0};

    tmp[0] = (FTU8)(0x80 | (addr >> 16));
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;

    FT9XX_CS_LOW;
    spi_writen(SPIM,tmp,FT9XX_SPI_TXLEN);
#else
    digitalWrite(EVE_SPI_CS, LOW);
    SPI.transfer(0x80 | (addr >> 16));
    SPI.transfer(highByte(addr));
    SPI.transfer(lowByte(addr));
#endif
}
FTVOID HAL_CoReset (FTVOID)
{
#if defined(DEF_BT81X)
	FTU16 p_addr = HAL_Read16(PATCH_ADDR);
#endif	

    /*
       Bit 2 - 0 :
       Bit 0 for coprocessor engine,
       Bit 1 for touch engine,
       Bit 2 for audio engine.
       Write 1 to reset the corresponding engine.
       Write 0 to go back normal working status.
     */

	/* Set REG_CPURESET to 1, to hold the coprocessor in the reset condition */
    HAL_Write8(REG_CPURESET, 1);
    while (!HAL_Read8(REG_CPURESET));

    /* Set REG_CMD_READ and REG_CMD_WRITE to zero */
    HAL_Write32(REG_CMD_READ,0);
    HAL_Write32(REG_CMD_WRITE,0);
	HAL_Write32(REG_CMD_DL, 0);
    
    mcuCMDBuf[mcuCMDBufSize/FTU32_LEN] = REG_FLAG_CLN;
    mcuCMDindex = 0; 
        
    /* j1 will set the pclk to 0 for that error case */
	HAL_Write32(REG_PCLK, 2);

	/* Set REG_CPURESET to 0, to restart the coprocessor */
    HAL_Write8(REG_CPURESET, 0);
    while (HAL_Read8(REG_CPURESET));

#if defined(DEF_BT81X)
    HAL_Write16(PATCH_ADDR, p_addr);
#endif
}
STATIC FTU32 cmdWait (FTVOID)
{
    while (HAL_Read32(REG_CMD_WRITE) != HAL_Read32(REG_CMD_READ)) {
        if (0xFFF == HAL_Read32(REG_CMD_READ)) {
            FTPRINT("\nco-processor error, reset.");
            UI_END();
            return 0;
        }
    }
    
    return HAL_Read32(REG_CMD_READ);
}

STATIC FTVOID dlpWait (FTVOID)
{
    while (HAL_Read8(REG_DLSWAP)) ;

    mcuDLindex = 0;
}

FTVOID HAL_Write8 ( FTU32 addr, FTU8 data )
{
#ifdef MSVC2010EXPRESS
    FTU8 tmp[SPI_TXCMD_LEN+FTU8_LEN] = {0};
    FTU32 send;

    tmp[0] = (FTU8)(0x80 | (addr >> 16));
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;
    tmp[3] = data;

    SPI_Write(ftHandle,tmp,SPI_TXCMD_LEN+FTU8_LEN,&send,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
#elif defined(MSVC2012EMU) || defined(MSVC2017EMU)
    wrStart(addr);
    FT8XXEMU_transfer(data);
    FT8XXEMU_cs(0);
#elif defined(STM32F4)
    wrStart(addr);

    STM32_SPISend(data);

    while( SPI_GetFlagStatus(EVE_SPI, SPI_I2S_FLAG_BSY));
    EVE_CS_HIGH;
#elif defined(FT9XXEV)
    wrStart(addr);

    spi_writen(SPIM,&data,1);

    FT9XX_CS_HIGH;
#else
    wrStart(addr);

    SPI.transfer(data);

    digitalWrite(EVE_SPI_CS, HIGH);
#endif
}
FTVOID HAL_Write8Src ( FTU32 addr, FTU8 *src, FTU32 len )
{
    FTU32 i;

    wrStart(addr);
#ifdef MSVC2010EXPRESS
    SPI_Write(ftHandle,src,len,&i,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
#elif defined(MSVC2012EMU) || defined(MSVC2017EMU)
    for (i = 0; i < len; i++) {
        FT8XXEMU_transfer(src[i]);
    }
    FT8XXEMU_cs(0);
#elif defined(STM32F4)
    for (i = 0; i < len; i++) {
        STM32_SPISend(src[i]);
    }

    while( SPI_GetFlagStatus(EVE_SPI, SPI_I2S_FLAG_BSY));
    EVE_CS_HIGH;
#elif defined(FT9XXEV)
    i = len; /* just to remove compile warning */
    spi_writen(SPIM,src,i);

    FT9XX_CS_HIGH;
#else
    for (i = 0; i < len; i++) {
        SPI.transfer(src[i]);
    }

    digitalWrite(EVE_SPI_CS, HIGH);	
#endif
}
FTVOID HAL_Write16 ( FTU32 addr, FTU16 data )
{
#ifdef MSVC2010EXPRESS
    FTU8 tmp[SPI_TXCMD_LEN+FTU16_LEN] = {0};
    FTU32 send;

    tmp[0] = (FTU8)(0x80 | (addr >> 16));
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;
    tmp[3] = (FTU8)(data & 0xff);
    tmp[4] = (FTU8)(data >> 8);

    SPI_Write(ftHandle,tmp,SPI_TXCMD_LEN+FTU16_LEN,&send,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
#elif defined(MSVC2012EMU) || defined(MSVC2017EMU)
    wrStart(addr);

    FT8XXEMU_transfer((FTU8)data&0xFF);
    FT8XXEMU_transfer((FTU8)(data>>8)&0xFF);

    FT8XXEMU_cs(0);
#elif defined(STM32F4)
    wrStart(addr);

    STM32_SPISend((FTU8)data&0xFF);
    STM32_SPISend((FTU8)(data>>8)&0xFF);

    while( SPI_GetFlagStatus(EVE_SPI, SPI_I2S_FLAG_BSY));
    EVE_CS_HIGH;
#elif defined(FT9XXEV)
    wrStart(addr);

    spi_writen(SPIM,(FTU8 *)&data,FTU16_LEN);

    FT9XX_CS_HIGH;
#else
    wrStart(addr);

    SPI.transfer((FTU8)data&0xFF);
    SPI.transfer((FTU8)(data>>8)&0xFF);

    digitalWrite(EVE_SPI_CS, HIGH);
#endif
}
FTVOID HAL_Write32 ( FTU32 addr, FTU32 data )
{
#ifdef MSVC2010EXPRESS
    FTU8 tmp[SPI_TXCMD_LEN+FTU32_LEN] = {0};
    FTU32 send;

    tmp[0] = (FTU8)(0x80 | (addr >> 16));
    tmp[1] = (FTU8)(addr >> 8);
    tmp[2] = (FTU8)addr;
    tmp[3] = (FTU8)data & 0xff;
    tmp[4] = (FTU8)(data >> 8);
    tmp[5] = (FTU8)(data >> 16);
    tmp[6] = (FTU8)(data >> 24);

    SPI_Write(ftHandle,tmp,SPI_TXCMD_LEN+FTU32_LEN,&send,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
#elif defined(MSVC2012EMU) || defined(MSVC2017EMU)
    wrStart(addr);

    FT8XXEMU_transfer((FTU8)data&0xFF);
    FT8XXEMU_transfer((FTU8)(data>>8)&0xFF);
    FT8XXEMU_transfer((FTU8)(data>>16)&0xFF);
    FT8XXEMU_transfer((FTU8)(data>>24)&0xFF);

    FT8XXEMU_cs(0);
#elif defined(STM32F4)
    wrStart(addr);

    STM32_SPISend((FTU8)data&0xFF);
    STM32_SPISend((FTU8)(data>>8)&0xFF);
    STM32_SPISend((FTU8)(data>>16)&0xFF);
    STM32_SPISend((FTU8)(data>>24)&0xFF);

    while( SPI_GetFlagStatus(EVE_SPI, SPI_I2S_FLAG_BSY));
    EVE_CS_HIGH;
#elif defined(FT9XXEV)
    wrStart(addr);

    spi_writen(SPIM,(FTU8 *)&data,FTU32_LEN);

    FT9XX_CS_HIGH;
#else
    wrStart(addr);

    SPI.transfer((FTU8)data&0xFF);
    SPI.transfer((FTU8)(data>>8)&0xFF);
    SPI.transfer((FTU8)(data>>16)&0xFF);
    SPI.transfer((FTU8)(data>>24)&0xFF);

    digitalWrite(EVE_SPI_CS, HIGH);
#endif
}

FTVOID HAL_Cfg ( FTU8 cfg )
{
#ifdef MSVC2010EXPRESS
    FTU32 send;
    FTU8 tmp[SPI_TXCMD_LEN] = {0};

    tmp[0] = cfg;

    SPI_Write(ftHandle,tmp,SPI_TXCMD_LEN,&send, 
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
#elif defined(MSVC2012EMU) || defined(MSVC2017EMU)
    FT8XXEMU_transfer(cfg);
#elif defined(STM32F4)
    EVE_CS_LOW;

    STM32_SPISend(cfg);

    STM32_SPISend(0);

    STM32_SPISend(0);

    while( SPI_GetFlagStatus(EVE_SPI, SPI_I2S_FLAG_BSY));
    EVE_CS_HIGH;
#elif defined(FT9XXEV)
    FTU8 tmp[FT9XX_SPI_TXLEN] = {0};
    tmp[0] = cfg;
    tmp[1] = 0;
    tmp[2] = 0;

    FT9XX_CS_LOW;
    spi_writen(SPIM,tmp,FT9XX_SPI_TXLEN);
    FT9XX_CS_HIGH;
#else
    digitalWrite(EVE_SPI_CS, LOW);
    SPI.transfer(cfg);
    SPI.transfer(0);
    SPI.transfer(0);
    digitalWrite(EVE_SPI_CS, HIGH);
#endif
}


FTU8 HAL_Read8 ( FTU32 addr )
{
#ifdef MSVC2010EXPRESS
    FTU8 tmp[SPI_RXCMD_LEN] = {0};
    FTU32 recv;

    rdStart(addr);

    SPI_Read(ftHandle,tmp,FTU8_LEN,&recv,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);

    return tmp[0];
#elif defined(MSVC2012EMU) || defined(MSVC2017EMU)
    FTU8 tmp;

    rdStart(addr);

    tmp = FT8XXEMU_transfer(0);

    FT8XXEMU_cs(0);

    return tmp;
#elif defined(STM32F4)
    FTU8 tmp;

    rdStart(addr);
    tmp = STM32_SPISend(0);

    EVE_CS_HIGH;

    return tmp;
#elif defined(FT9XXEV)
    FTU8 tmp[4] = {0};
    rdStart(addr);
    spi_readn(SPIM,tmp,spi_dummy+FTU8_LEN);
    FT9XX_CS_HIGH;
    return tmp[spi_dummy];
#else
    FTU8 tmp;

    rdStart(addr);

    tmp = SPI.transfer(0);

    digitalWrite(EVE_SPI_CS, HIGH);

    return tmp;
#endif
}
FTU32 HAL_Read8Buff ( FTU32 addr, FTU8 *buff, FTU32 len )
{
#ifdef MSVC2010EXPRESS
    FTU32 recv;

    rdStart(addr);

    SPI_Read(ftHandle,buff,len,&recv,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);

    return recv;
#elif defined(MSVC2012EMU) || defined(MSVC2017EMU)
    FTU32 tmp = len;
    FTU8 *p = buff;

    rdStart(addr);

    while (tmp--) {
        *p = FT8XXEMU_transfer(0);
        p++;
    }

    FT8XXEMU_cs(0);

    return len;
#elif defined(STM32F4)
    FTU32 tmp = len;
    FTU8 *p = buff;

    rdStart(addr);

    while (tmp--) {
        *p = STM32_SPISend(0);
        p++;
    }

    EVE_CS_HIGH;

    return len;
#elif defined(FT9XXEV)
    rdStart(addr);

    spi_readn(SPIM,buff,spi_dummy+len);
    
    FT9XX_CS_HIGH;
    
    return len;
#else
    FTU32 tmp = len;
    FTU8 *p = buff;

    rdStart(addr);

    while (tmp--) {
        *p = SPI.transfer(0);
        p++;
    }

    digitalWrite(EVE_SPI_CS, HIGH);

    return len;
#endif
}
FTU16 HAL_Read16 ( FTU32 addr )
{
#ifdef MSVC2010EXPRESS
    FTU8 tmp[SPI_RXCMD_LEN] = {0};
    FTU32 recv;

    rdStart(addr);

    SPI_Read(ftHandle,tmp,FTU16_LEN,&recv,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);

    return (tmp[0]|(tmp[1]<<8));
#elif defined(MSVC2012EMU) || defined(MSVC2017EMU)
    FTU16 tmp = 0;

    rdStart(addr);

    tmp = (FTU16)FT8XXEMU_transfer(0);
    tmp |= (FTU16)FT8XXEMU_transfer(0) << 8;

    FT8XXEMU_cs(0);

    return tmp;
#elif defined(STM32F4)
    FTU16 tmp = 0;

    rdStart(addr);

    /* force type definition is critical while running on different platform */
    tmp = (FTU16)STM32_SPISend(0);
    tmp |= (FTU16)STM32_SPISend(0) << 8;

    while( SPI_GetFlagStatus(EVE_SPI, SPI_I2S_FLAG_BSY));
    EVE_CS_HIGH;

    return tmp;
#elif defined(FT9XXEV)
    FTU8 tmp[4] = {0};
    rdStart(addr);
    spi_readn(SPIM,(FTU8 *)&tmp,spi_dummy+FTU16_LEN);
    FT9XX_CS_HIGH;

    return tmp[spi_dummy]| \
        (tmp[spi_dummy+1]<<8);
#else
    FTU16 tmp = 0;

    rdStart(addr);

    /* force type definition is critical while running on different platform */
    tmp = (FTU16)SPI.transfer(0);
    tmp |= (FTU16)SPI.transfer(0) << 8;

    digitalWrite(EVE_SPI_CS, HIGH);

    return tmp;
#endif
}

FTU32 HAL_Read32 ( FTU32 addr )
{
#ifdef MSVC2010EXPRESS
    FTU8 tmp[SPI_RXCMD_LEN+FTU32_LEN] = {0};
    FTU32 recv;

    rdStart(addr);

    SPI_Read(ftHandle,tmp,FTU32_LEN,&recv,
            SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | 
            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);

    return (tmp[0]|(tmp[1]<<8)|(tmp[2]<<16)|(tmp[3]<<24));
#elif defined(MSVC2012EMU) || defined(MSVC2017EMU)
    FTU32 tmp = 0;

    rdStart(addr);

    tmp = (FTU32)FT8XXEMU_transfer(0);
    tmp |= (FTU32)FT8XXEMU_transfer(0) << 8;
    tmp |= (FTU32)FT8XXEMU_transfer(0) << 16;
    tmp |= (FTU32)FT8XXEMU_transfer(0) << 24;

    FT8XXEMU_cs(0);

    return tmp;
#elif defined(STM32F4)
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
#elif defined(FT9XXEV)
    FTU8 tmp[8] = {0};
    rdStart(addr);
    spi_readn(SPIM,(FTU8 *)&tmp,spi_dummy+FTU32_LEN);
    FT9XX_CS_HIGH;

    return tmp[spi_dummy]| \
        (tmp[spi_dummy+1]<<8)| \
        (tmp[spi_dummy+2]<<16)| \
        (tmp[spi_dummy+3]<<24);
#else
    FTU32 tmp = 0;

    rdStart(addr);

    /* force type definition is critical while running on different platform */
    tmp = (FTU32)SPI.transfer(0);
    tmp |= (FTU32)SPI.transfer(0) << 8;
    tmp |= (FTU32)SPI.transfer(0) << 16;
    tmp |= (FTU32)SPI.transfer(0) << 24;

    digitalWrite(EVE_SPI_CS, HIGH);

    return tmp;
#endif
}

FTU32 HAL_CalResultAddr (FTVOID)
{
    if (mcuCMDBuf) {
        return RAM_CMD + mcuCMDindex - FTU32_LEN;
    } else {	
        return RAM_CMD + HAL_Read32(REG_CMD_WRITE) - FTU32_LEN;
    }
}

FTVOID HAL_McuCmdBufInit (FTVOID)
{
    mcuDLindex = 0;
    FTPRINT("\nCMD buffer: ");
#if defined(USED_CMD_BUF)
    FTPRINT("yes");
    mcuCMDBuf = (FTU32 *)&mem_pool[0];
    mcuCMDBufSize = USE_STATIC_MEM_LEN;
    mcuCMDBuf[mcuCMDBufSize/FTU32_LEN] = REG_FLAG_CLN;
    mcuCMDindex = 0; 
#else
    FTPRINT("no");
    mcuCMDBuf = 0;
    mcuCMDBufSize = 0;
    mcuCMDindex = 0;
#endif
}

FTU32 HAL_CmdBufSize (FTVOID)
{
    return mcuCMDBufSize;
}

/*
 * SampleApp smart way to find the cmd fifo free space is:
 * (used space) = ((mcu/eve writen pointer) - (eve executed pointer)) & ((eve cmd fifo length) - 1);
 * (free space) = ((eve cmd fifo length) - 4) - (used space); 
 * refer to Ft_Gpu_Cmdfifo_Freespace in SampleApp for the detail
 * I like my stupid way, so keep using this way
   |---------desLen-------|
   |>>>>>>!>>>>>>>>>>>>>>>|
   pDes  desNow        pDes+desLen

   the loop space may be located in EVE's cmd buffer or EVE's RAM_G
   so make the address as input variable
 */
FTU32 HAL_EVELoopMemWr (FTU32 pDes, FTU32 desNow, FTU32 desLen, FTU8 * inSrc, FTU32 inLen)
{
    FTU32 l;

    if (inLen >= desLen) {
        FTPRINT("\neve loop wr: inLen >= desLen");
        return desNow;
    } else if (inLen == 0) {
        FTPRINT("\neve loop wr: inLen == 0");
        return desNow;
    }

    if (desNow + inLen > desLen) {
        l = desLen - desNow;
        HAL_Write8Src(pDes+desNow,inSrc,l);
        HAL_Write8Src(pDes,&inSrc[l],inLen-l);
        return inLen - l;
    } else {
        HAL_Write8Src(pDes+desNow,inSrc,inLen);
        if (desNow + inLen != desLen) {
            return desNow + inLen;
        } else {
            return 0;
        }
    }
}

FTVOID HAL_CmdToReg (FTU32 Cmd)
{
    FTU32 w = HAL_Read32(REG_CMD_WRITE);

    HAL_Write32(RAM_CMD + w,Cmd);
    w += FTU32_LEN;
    if (w >= CMDBUF_SIZE) {
        w = 0;
    }
    HAL_Write16(REG_CMD_WRITE,(FTU16)w);
}

FTVOID HAL_CmdBufIn (FTU32 Cmd)
{
    if (mcuCMDBuf) {
        if (mcuCMDBufSize <= mcuCMDindex) {
            FTPRINT("\nmcu cmd buf: mcuCMDBufSize <= mcuCMDindex");
            return;
        }
        /* mcuCMDBuf would be used both DLP and CMD for the buffering
           set the CMD tag, to indicate CMD is using
           when tag not match, previous command may using DLP*/
        if (REG_FLAG_CLN == mcuCMDBuf[mcuCMDBufSize/FTU32_LEN]) {
            mcuCMDBuf[mcuCMDBufSize/FTU32_LEN] = RAM_CMD;
        } else if (mcuCMDBuf[mcuCMDBufSize/FTU32_LEN] != RAM_CMD) {
            FTPRINT("\nmcu cmd buf: tag != RAM_CMD");
            return;
        }
        mcuCMDBuf[mcuCMDindex/FTU32_LEN] = Cmd;
        mcuCMDindex += FTU32_LEN;
        if (mcuCMDindex == mcuCMDBufSize) {

            HAL_Write16(REG_CMD_WRITE,
                    (FTU16)HAL_EVELoopMemWr(RAM_CMD,HAL_Read32(REG_CMD_WRITE),
                        CMDBUF_SIZE,(FTU8 *)mcuCMDBuf,
                        mcuCMDBufSize));

            mcuCMDindex = 0;
        }
    } else {
        HAL_CmdToReg(Cmd);
    }
}
/* 
 * It's not smart at all
 * you may refer to SampleApp for better solution
 * I'll leave my stupid routine here
 */
FTVOID HAL_CmdBufInStr (FTC8 *pstr)
{
    FTU32 i,tmp = 0;
    /*
     * one of the reason I'm doing this is:
     * there are two loop buffer needs to be handled
     * 1. mcu cmd buf
     * 2. eve cmd buf
     * both of them got and 'loop free space' need to be consider
     * before doing any continous filling
     * and for string, string become another 'buffer' space
     * 3. string buf
     * when continous filling into whatever mcu buffer or eve buffer
     * to prevent too many 'if there are still free space ahead'
     * judgement in program, I use make the action to 4 byte a shot
     */
    for (i = 0; pstr[i] != '\0'; i++) {
        switch (i%FTU32_LEN) {
            case 0:
                /* 
                 * force type definition is critical 
                 * while running on different platform 
                 */
                tmp = (FTU32)pstr[i];
                continue;
            case 1:
                tmp |= (FTU32)pstr[i]<<8;
                continue;
            case 2:
                tmp |= (FTU32)pstr[i]<<16;
                continue;
            case 3:
                tmp |= (FTU32)pstr[i]<<24;
                break;
            default:
                break;
        }

        HAL_CmdBufIn(tmp);

        /* 
         make sure:
         1. not 4 byte align length str would have '\0' with the tmp
         2. 4 byte align length str would have '\0'
         once it break from the while loop
         */
        tmp = 0;
    }

    /*
     when the str length is :
     1. 4 byte align, here is to give '\0'
     2. none 4 byte align, here is to give the tmp with '\0'
     */
    HAL_CmdBufIn(tmp);	

    return;
}
FTU8 COUNT_ARGS(FTC8 * str)
{
	FTU8 count = 0;
	FTC8 *tmp = str;

	tmp = strstr(tmp, "%");
	while (tmp)
	{
		if (*(tmp + 1) == '%') {
			tmp += 2;
		}
		else {
			count++;
			tmp++;
		}
		tmp = strstr(tmp, "%");
	}
	return count;
}
FTVOID CoCmd_TEXT(FTU32 x, FTU32 y, FTU32 font, FTU32 opt, FTC8 * s, ...)
{
	va_list args;
	FTU8 i, num=0;
	va_start(args, s);
	
#if defined(DEF_BT81X)
    //Only check % characters if option OPT_FORMAT is set 
	num = (opt & OPT_FORMAT) ? (COUNT_ARGS(s)) : (0); 
#endif
	HAL_CmdBufIn(CMD_TEXT);
	HAL_CmdBufIn((((FTU32)y << 16) | (x & 0xffff)));
	HAL_CmdBufIn((((FTU32)opt << 16) | (font & 0xffff)));
	HAL_CmdBufInStr(s);
	for (i = 0; i < num; i++)
	{
	    HAL_CmdBufIn((FTU32)va_arg(args, FTU32));
	}
	va_end(args);
}
FTVOID HAL_DlpBufIn (FTU32 Dlp)
{
    if (mcuCMDBuf) {
        if (EVE_DLP_SIZE <= (mcuDLindex + mcuCMDindex)) {
            FTPRINT("\nmcu dlp buf: EVE_DLP_SIZE <= index");
            return;
        }
        /* mcuCMDBuf would be used both DLP and CMD for the buffering
           set the DLP tag, to indicate DLP is using
           when tag not match, previous command may using CMD*/
        if (REG_FLAG_CLN == mcuCMDBuf[mcuCMDBufSize/FTU32_LEN]) {
            mcuCMDBuf[mcuCMDBufSize/FTU32_LEN] = RAM_DL;
        } else if (mcuCMDBuf[mcuCMDBufSize/FTU32_LEN] != RAM_DL) {
            FTPRINT("\nmcu dlp buf: tag != RAM_DL");
            return;
        }
        mcuCMDBuf[mcuCMDindex/FTU32_LEN] = Dlp;
        mcuCMDindex += FTU32_LEN;
        if (mcuCMDindex == mcuCMDBufSize) {
            HAL_Write8Src(RAM_DL+mcuDLindex,
                    (FTU8 *)mcuCMDBuf,mcuCMDBufSize);
            mcuCMDindex = 0;
            mcuDLindex += mcuCMDBufSize;
        }
    } else {
        HAL_Write32(RAM_DL+mcuDLindex,Dlp);
        mcuDLindex += FTU32_LEN;
    }
}
FTVOID HAL_CmdWait (FTU16 CmdWrAddr)
{
    HAL_Write16(REG_CMD_WRITE, CmdWrAddr);
    cmdWait();
}
FTVOID HAL_DlWait (FTVOID)
{
    HAL_Write32(REG_DLSWAP,DLSWAP_FRAME);
    dlpWait();
}
FTVOID HAL_BufToReg (FTU32 reg, FTU32 padNum)
{
    if (reg == RAM_CMD) {
        if (mcuCMDBuf) {
            if (mcuCMDBuf[mcuCMDBufSize/FTU32_LEN] != reg) {
                FTPRINT("\nmcu to eve: cmd tag invalid");
                return;
            }
			/* sometime cmd buffer already flush into eve in HAL_CmdBufIn
			   so judge the mcuCMDindex before doing the flush */
			if (mcuCMDindex) {
				HAL_CmdWait((FTU16)HAL_EVELoopMemWr(reg,HAL_Read32(REG_CMD_WRITE),
							CMDBUF_SIZE,(FTU8 *)mcuCMDBuf,
							mcuCMDindex));
			}
        } else {
            HAL_CmdWait((FTU16)HAL_Read32(REG_CMD_WRITE));
        }
    } else {
        if (mcuCMDBuf) {
            if (mcuCMDBuf[mcuCMDBufSize/FTU32_LEN] != reg) {
                FTPRINT("\nmcu to eve: dlp tag invalid");
                return;
            }
			/* sometime cmd buffer already flush into eve in HAL_DlpBufIn
			   so judge the mcuCMDindex before doing the flush */
			if (mcuCMDindex) {
				HAL_Write8Src(reg+mcuDLindex,(FTU8 *)mcuCMDBuf,mcuCMDindex);
			}
        }
        HAL_DlWait();
    }

    if (mcuCMDBuf) {
        mcuCMDBuf[mcuCMDBufSize/FTU32_LEN] = REG_FLAG_CLN;
        mcuCMDindex = 0;
    }
}
FTVOID HAL_CmdExeNow(FTU32 * pCL, FTU32 l)
{
    FTU32 i = 0;
    
    /* make sure command buffer clean */
    HAL_CmdWait((FTU16)HAL_Read32(REG_CMD_WRITE));

    while (i < l) {
	    HAL_CmdToReg(pCL[i]);
        i++;
    }

    /* make sure command successful executed */
    HAL_CmdWait((FTU16)HAL_Read32(REG_CMD_WRITE));
}

