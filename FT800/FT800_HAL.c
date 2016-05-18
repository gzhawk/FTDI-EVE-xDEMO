/* 
    Hardware Abstract Layer for FT800 
    Author: Hawk
    Email : hawk.gao@ftdichip.com	
    Date  : 2013/Oct
 */
#include "FT800_platform.h"
#include "FT800_Gpu.h"
#include "FT800_HAL.h"
#include "FT800_APP.h"

FTU8 EVE_ID = 0;
FTU32 * mcuCMDBuf = 0;
FTU32 mcuCMDBufSize = 0;
FTU32 mcuCMDindex = 0; 
FTU32 mcuDLindex = 0;

#if defined(USED_CMD_BUF)
FTU8 mem_pool[USE_STATIC_MEM_LEN+FTU32_LEN] = {0};
#endif

FTINDEF FTVOID rdStart ( FTU32 addr )
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
#elif defined(MSVC2012EMU)
    FT8XXEMU_cs(1);
    FT8XXEMU_transfer((FTU8)(addr >> 16));
    FT8XXEMU_transfer((FTU8)(addr >> 8));
    FT8XXEMU_transfer((FTU8)addr);
    FT8XXEMU_transfer(0);
#elif defined(STM32F4)
    FT800_CS_LOW;
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
    digitalWrite(FT800_SPI_CS, LOW);
    SPI.transfer((FTU8)(addr >> 16));
    SPI.transfer(highByte(addr));
    SPI.transfer(lowByte(addr));

    SPI.transfer(0); //Dummy Read Byte
#endif
}

FTINDEF FTVOID wrStart ( FTU32 addr )
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
#elif defined(MSVC2012EMU)
    FT8XXEMU_cs(1);
    FT8XXEMU_transfer((FTU8)(addr >> 16) | 0x80);
    FT8XXEMU_transfer((FTU8)(addr >> 8));
    FT8XXEMU_transfer((FTU8)addr);
#elif defined(STM32F4)
    FT800_CS_LOW;
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
    digitalWrite(FT800_SPI_CS, LOW);
    SPI.transfer(0x80 | (addr >> 16));
    SPI.transfer(highByte(addr));
    SPI.transfer(lowByte(addr));
#endif
}

FTINDEF FTU32 cmdWait (FTVOID)
{
    while (HAL_Read32(REG_CMD_WRITE) != HAL_Read32(REG_CMD_READ));
    
    return HAL_Read32(REG_CMD_READ);
}

FTINDEF FTVOID dlpWait (FTVOID)
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
#elif defined(MSVC2012EMU)
    wrStart(addr);
    FT8XXEMU_transfer(data);
    FT8XXEMU_cs(0);
#elif defined(STM32F4)
    wrStart(addr);

    STM32_SPISend(data);

    while( SPI_GetFlagStatus(FT800_SPI, SPI_I2S_FLAG_BSY));
    FT800_CS_HIGH;
#elif defined(FT9XXEV)
    wrStart(addr);

    spi_writen(SPIM,&data,1);

    FT9XX_CS_HIGH;
#else
    wrStart(addr);

    SPI.transfer(data);

    digitalWrite(FT800_SPI_CS, HIGH);
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
#elif defined(MSVC2012EMU)
    for (i = 0; i < len; i++) {
        FT8XXEMU_transfer(src[i]);
    }
    FT8XXEMU_cs(0);
#elif defined(STM32F4)
    for (i = 0; i < len; i++) {
        STM32_SPISend(src[i]);
    }

    while( SPI_GetFlagStatus(FT800_SPI, SPI_I2S_FLAG_BSY));
    FT800_CS_HIGH;
#elif defined(FT9XXEV)
    i = len; /* just to remove compile warning */
    spi_writen(SPIM,src,i);

    FT9XX_CS_HIGH;
#else
    for (i = 0; i < len; i++) {
        SPI.transfer(src[i]);
    }

    digitalWrite(FT800_SPI_CS, HIGH);	
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
#elif defined(MSVC2012EMU)
    wrStart(addr);

    FT8XXEMU_transfer((FTU8)data&0xFF);
    FT8XXEMU_transfer((FTU8)(data>>8)&0xFF);

    FT8XXEMU_cs(0);
#elif defined(STM32F4)
    wrStart(addr);

    STM32_SPISend((FTU8)data&0xFF);
    STM32_SPISend((FTU8)(data>>8)&0xFF);

    while( SPI_GetFlagStatus(FT800_SPI, SPI_I2S_FLAG_BSY));
    FT800_CS_HIGH;
#elif defined(FT9XXEV)
    wrStart(addr);

    spi_writen(SPIM,(FTU8 *)&data,FTU16_LEN);

    FT9XX_CS_HIGH;
#else
    wrStart(addr);

    SPI.transfer((FTU8)data&0xFF);
    SPI.transfer((FTU8)(data>>8)&0xFF);

    digitalWrite(FT800_SPI_CS, HIGH);
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
#elif defined(MSVC2012EMU)
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

    while( SPI_GetFlagStatus(FT800_SPI, SPI_I2S_FLAG_BSY));
    FT800_CS_HIGH;
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

    digitalWrite(FT800_SPI_CS, HIGH);
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
#elif defined(MSVC2012EMU)
    FT8XXEMU_transfer(cfg);
#elif defined(STM32F4)
    FT800_CS_LOW;

    STM32_SPISend(cfg);

    STM32_SPISend(0);

    STM32_SPISend(0);

    while( SPI_GetFlagStatus(FT800_SPI, SPI_I2S_FLAG_BSY));
    FT800_CS_HIGH;
#elif defined(FT9XXEV)
    FTU8 tmp[FT9XX_SPI_TXLEN] = {0};
    tmp[0] = cfg;
    tmp[1] = 0;
    tmp[2] = 0;

    FT9XX_CS_LOW;
    spi_writen(SPIM,tmp,FT9XX_SPI_TXLEN);
    FT9XX_CS_HIGH;
#else
    digitalWrite(FT800_SPI_CS, LOW);
    SPI.transfer(cfg);
    SPI.transfer(0);
    SPI.transfer(0);
    digitalWrite(FT800_SPI_CS, HIGH);
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
#elif defined(MSVC2012EMU)
    FTU8 tmp;

    rdStart(addr);

    tmp = FT8XXEMU_transfer(0);

    FT8XXEMU_cs(0);

    return tmp;
#elif defined(STM32F4)
    FTU8 tmp;

    rdStart(addr);
    tmp = STM32_SPISend(0);

    FT800_CS_HIGH;

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

    digitalWrite(FT800_SPI_CS, HIGH);

    return tmp;
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
#elif defined(MSVC2012EMU)
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

    while( SPI_GetFlagStatus(FT800_SPI, SPI_I2S_FLAG_BSY));
    FT800_CS_HIGH;

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

    digitalWrite(FT800_SPI_CS, HIGH);

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
#elif defined(MSVC2012EMU)
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

    while( SPI_GetFlagStatus(FT800_SPI, SPI_I2S_FLAG_BSY));
    FT800_CS_HIGH;

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

    digitalWrite(FT800_SPI_CS, HIGH);

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
        /* set the cmd tag */
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
 * I'll leave this stupid routine to show how fool I am :-)
 * one of the reason I'm doing this is:
 * there are two loop buffer needs to be handled
 * 1. mcu cmd buf
 * 2. eve cmd buf
 * both of them got and 'loop free space' need to be consider
 * before doing any continous filling
 * and for string, string become another 'buffer' space when
 * continous filling into what ever mcu buffer or eve buffer
 * to prevent too many 'if there are still free space ahead'
 * judgement in program, I use make the action to 4 byte a shot
 */
FTVOID HAL_CmdBufInStr (FTC8 *pstr)
{
    FTU32 i,tmp = 0;

    /* get the length by myslef to avoid different platform strlen 
     * may return the length include or not include '\0' */
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

        tmp = 0;
    }

    if (i%FTU32_LEN == 0) {
        HAL_CmdBufIn(0);
    } else {
        HAL_CmdBufIn(tmp);	
    }

    return;
}

FTVOID HAL_DlpBufIn (FTU32 Dlp)
{
    if (mcuCMDBuf) {
        if (EVE_DLP_SIZE <= (mcuDLindex + mcuCMDindex)) {
            FTPRINT("\nmcu dlp buf: EVE_DLP_SIZE <= index");
            return;
        }
        /* set the dlp tag */
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

            HAL_CmdWait((FTU16)HAL_EVELoopMemWr(reg,HAL_Read32(REG_CMD_WRITE),
                        CMDBUF_SIZE,(FTU8 *)mcuCMDBuf,
                        mcuCMDindex));
        } else {
            HAL_CmdWait((FTU16)HAL_Read32(REG_CMD_WRITE));
        }
    } else {
        if (mcuCMDBuf) {
            if (mcuCMDBuf[mcuCMDBufSize/FTU32_LEN] != reg) {
                FTPRINT("\nmcu to eve: dlp tag invalid");
                return;
            }

            HAL_Write8Src(reg+mcuDLindex,(FTU8 *)mcuCMDBuf,mcuCMDindex);
        }
        HAL_DlWait();
    }

    if (mcuCMDBuf) {
        mcuCMDBuf[mcuCMDBufSize/FTU32_LEN] = REG_FLAG_CLN;
        mcuCMDindex = 0;
    }
}

FTINDEF FTVOID HAL_SpiInit ( FTVOID )
{
    /* 
       the SPI clock shall not exceed 11MHz before system clock is enabled. 
       After system clock is properly enabled, 
       the SPI clock is allowed to go up to 30MHz.	
     */
#ifdef MSVC2010EXPRESS
    vc2010_spi_init();
#elif defined(MSVC2012EMU)
    //do nothing
#elif defined(STM32F4)
    stm32f4SPI1Init(SPI_BaudRatePrescaler_8);
#elif defined(FT9XXEV)
    /*spi sck = system clock/SPI_Div, 100MHz/16=6.25MHz*/
    ft9xx_spi_init(1,16);
#else
    /*spi sck = system clock/DIVx, 16MHz/2 = 8MHz*/
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
#endif
}
FTVOID HAL_Get_EVE_ID (FTVOID)
{
    EVE_ID = HAL_Read8(EVE_ID_REG);
}
FTVOID HAL_Path_Config ( FTVOID )
{
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
    //do nothing
#elif defined(STM32F4)
    //do nothing
#elif defined(FT9XXEV)
    ft9xx_init();

    ft9xx_sdc_init();
#else
    /* set the GPIO pin */
    pinMode(FT800_SPI_CS, OUTPUT);
    digitalWrite(FT800_SPI_CS, HIGH);

    pinMode(FT800_PD_N, OUTPUT);
    digitalWrite(FT800_PD_N, HIGH);

    arduino_sdcardInit();
#endif

    HAL_McuCmdBufInit();

    HAL_SpiInit();
}
FTVOID HAL_FT800_Active ( FTVOID )
{
    HAL_Cfg(FT_GPU_ACTIVE_M);
#ifdef DEF_81X
    /* 81X need more action for the active */
    HAL_Cfg(FT_GPU_ACTIVE_M);
#endif
    FTDELAY(50);
}

/* 
 * longer delay for CAP touch engin ready
 * for RES touch engin, 10 would be enough 
 */
#define CLK_DELAY 100
FTVOID HAL_FT800_Clock ( FTVOID )
{
    FTPRINT("\nOSC: ");
#if defined(TRIM_NEEDED)
    FTPRINT("internal");
#else
    FTPRINT("external");
    /* Set the clk to external clock */
    HAL_Cfg(FT_GPU_EXTERNAL_OSC);  
    FTDELAY(CLK_DELAY);
#endif
#ifndef DEF_81X
    /* default 48MHz, no need to config
    HAL_Cfg(FT_GPU_PLL_48M);  
    FTDELAY(CLK_DELAY);
    */
#endif
}

#define PWC_DELAY 20
FTVOID HAL_FT800_PowerCycle ( FTU8 OnOff )
{
#ifdef MSVC2010EXPRESS
    FT_WriteGPIO(ftHandle, 0xBB, OnOff?0x08:0x88);
    FTDELAY(PWC_DELAY);

    FT_WriteGPIO(ftHandle, 0xBB, OnOff?0x88:0x08);
    FTDELAY(PWC_DELAY);
#elif defined(MSVC2012EMU)
    /* Do nothing */
#elif defined(STM32F4)
    /* Share the PD13/LED3 pin with PD pin of EVE */
    STM_EVAL_LEDOff(LED3);
    FTDELAY(PWC_DELAY);

    STM_EVAL_LEDOn(LED3);
    FTDELAY(PWC_DELAY);
#elif defined(FT9XXEV)
    gpio_write(FT9XX_PD, OnOff?0:1);
    FTDELAY(PWC_DELAY);

    gpio_write(FT9XX_PD, OnOff?1:0);
    FTDELAY(PWC_DELAY);
#else
    digitalWrite(FT800_PD_N, OnOff?LOW:HIGH);
    FTDELAY(PWC_DELAY);

    digitalWrite(FT800_PD_N, OnOff?HIGH:LOW);
    FTDELAY(PWC_DELAY);
    /* 
     * I was not quite understand why a read action is needed
     * but for VM801Plus module, it seems like a critical action 
     */
    HAL_Read16(0);
#endif

}

FTVOID HAL_FT800_GPIOConfig ( FTVOID )
{
    /* set DISP to output, then enable the DISP */
#if defined(DEF_81X)
    HAL_Write16(REG_GPIOX_DIR,0x8000 | HAL_Read16(REG_GPIOX_DIR));
    HAL_Write16(REG_GPIOX,0x8000 | HAL_Read16(REG_GPIOX));
#else
    HAL_Write8(REG_GPIO_DIR,0x80 | HAL_Read8(REG_GPIO_DIR));
    HAL_Write8(REG_GPIO,0x80 | HAL_Read8(REG_GPIO));
#endif
}

/*
 * Add the FT81X support, with the CTP controller
 * you may change the delay time base on your own HW and CTP
 */
FTVOID HAL_FT800_TouchConfig ( FTVOID )
{
#if defined(DEF_CAP_NONMULTI) || defined(DEF_CAP_MULTI)
    FTDELAY(300);
    HAL_Write8(REG_CPURESET, 2);
    FTDELAY(300);
    HAL_Write16(REG_CYA_TOUCH,(HAL_Read16(REG_CYA_TOUCH) & 0x7fff));
    FTDELAY(300);
    HAL_Write8(REG_CPURESET, 0);
    FTDELAY(300);

#ifdef DEF_CAP_NONMULTI
    HAL_Write8(REG_CTOUCH_EXTENDED,CTOUCH_MODE_COMPATIBILITY);
#endif

#else
    /*
     * for this code, I consider "resistance touch" + "WVGA" 
     * means using FTDI WH70R, it use default touch threshold: 
     * the lightest touch will be accepted
     * you may change it depends on your real system
     */
#if !defined(LCD_WVGA)
    HAL_Write16(REG_TOUCH_RZTHRESH,FT800_TOUCH_THRESHOLD);
#endif
    HAL_Write16(REG_CYA_TOUCH,(HAL_Read16(REG_CYA_TOUCH) | 0x8000));
#endif
}
/*
 * Only when error happen
 * process would reach here
 */
FTVOID HAL_FT800_EndDisp ( FTVOID )
{
    while (1) {
        HAL_CmdBufIn(CMD_DLSTART);
        HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0,0));
        HAL_CmdBufIn(CLEAR(1,1,1));

        CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/3,24,
                OPT_CENTERX,"Missing correct files in SD card");
        CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/3*2,24,
                OPT_CENTERX,"or image not for this platform");

        HAL_CmdBufIn(DISPLAY());
        HAL_CmdBufIn(CMD_SWAP);
        HAL_BufToReg(RAM_CMD,0);
    }
}
FTVOID HAL_FT800_SetSPI (FTU32 type)
{
#ifdef DEF_81X
    if (type == 4) {
        HAL_Write8(REG_SPI_WIDTH, EVE_QSPI | FT800_SPI_DUMMY);
    } else if (type == 2) {
        HAL_Write8(REG_SPI_WIDTH, EVE_DSPI | FT800_SPI_DUMMY);
    } else {
        HAL_Write8(REG_SPI_WIDTH, EVE_SSPI);
    }
#endif

#if defined(FT9XXEV)
    /* use the highest speed of clk of SPI on MM900 module */
    if (type == 4) {
        ft9xx_spi_init(type,8);
    } else {
        ft9xx_spi_init(type,2);
    }
#endif
}
#define VER_FONT 25
#define CAL_FONT 20
#define CAL_WIDE 15
#define SYS_HANG (0)
FTVOID HAL_FT800_BootupDisp ( FTU32 count )
{
    static FTU8 init = 0;
    if (init == 0) {
        FTPRINT("\nBootup info");
        init = 1;
    }
    do {
        HAL_CmdBufIn(CMD_DLSTART);
        HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
        HAL_CmdBufIn(CLEAR(1,1,1));

        switch (EVE_ID) {
            case 0x10:
                CoCmd_TEXT(FT800_LCD_WIDTH/3,FT800_LCD_HIGH/4,
                        VER_FONT,OPT_CENTERX,"FT810");
                break;
            case 0x11:
                CoCmd_TEXT(FT800_LCD_WIDTH/3,FT800_LCD_HIGH/4,
                        VER_FONT,OPT_CENTERX,"FT811");
                break;
            case 0x12:
                CoCmd_TEXT(FT800_LCD_WIDTH/3,FT800_LCD_HIGH/4,
                        VER_FONT,OPT_CENTERX,"FT812");
                break;
            case 0x13:
                CoCmd_TEXT(FT800_LCD_WIDTH/3,FT800_LCD_HIGH/4,
                        VER_FONT,OPT_CENTERX,"FT813");
                break;
            default:
                /* only new FT81X able to read the EVE ID */
                CoCmd_TEXT(FT800_LCD_WIDTH/3,FT800_LCD_HIGH/4,
                        VER_FONT,OPT_CENTERX,"EVE Chip");
                break;
        }
#if defined(DEF_CAP_MULTI)
        CoCmd_TEXT(FT800_LCD_WIDTH/3*2,FT800_LCD_HIGH/4,
                VER_FONT,OPT_CENTERX,"CAP-M");
#elif defined(DEF_CAP_NONMULTI)
        CoCmd_TEXT(FT800_LCD_WIDTH/3*2,FT800_LCD_HIGH/4,
                VER_FONT,OPT_CENTERX,"CAP-NM");
#else
        CoCmd_TEXT(FT800_LCD_WIDTH/3*2,FT800_LCD_HIGH/4,
                VER_FONT,OPT_CENTERX,"RES");
#endif
        CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,
                VER_FONT,OPT_CENTERX,"Ver: "FT800_VER);
        if (mcuCMDBuf) {
            CoCmd_TEXT(FT800_LCD_WIDTH/2-10,FT800_LCD_HIGH/4*3,
                    CAL_FONT,OPT_CENTERX,"MCU CMD Buf: ");
            CoCmd_NUMBER(FT800_LCD_WIDTH/2+40,FT800_LCD_HIGH/4*3,
                    CAL_FONT,OPT_CENTERX,mcuCMDBufSize);
        }
#if defined(STM32F4)
        CoCmd_TEXT(FT800_LCD_WIDTH/2-30,FT800_LCD_HIGH/4*3+10,
                CAL_FONT,OPT_CENTERX,"File addr: ");
        CoCmd_NUMBER(FT800_LCD_WIDTH/2+40,FT800_LCD_HIGH/4*3+10,
                CAL_FONT,OPT_CENTERX,FILE_SADDR);
#endif
        if (SYS_HANG) {
            CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH - CAL_WIDE*2,
                    CAL_FONT,OPT_CENTERX,"system hange due to memory limit!");
        } else {
            CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH - CAL_WIDE*2,
                    CAL_FONT,OPT_CENTERX,"press and hold to force in calibration");
            CoCmd_NUMBER(FT800_LCD_WIDTH/2,FT800_LCD_HIGH - CAL_WIDE,
                    CAL_FONT,OPT_CENTERX,count);
        }

        HAL_CmdBufIn(DISPLAY());
        HAL_CmdBufIn(CMD_SWAP);
        HAL_BufToReg(RAM_CMD,0);
    } while (SYS_HANG);
}
#if defined(TRIM_NEEDED)
FTU32 HAL_FT800_GetEVEFrq (FTVOID)
{
    FTU32 t0, t1;
    FT32 r = 15625;

    t0 = HAL_Read32(REG_CLOCK); /* t0 read */
#if defined(FT9XXEV)
    __asm__
        (
         "   move.l  $r0,%0"
         "\n\t"
         "   mul.l   $r0,$r0,100"
         "\n\t"
         "1:"
         "\n\t"
         "   sub.l   $r0,$r0,3"
         "\n\t" /* Subtract the loop time = 4 cycles */
         "   cmp.l   $r0,0"
         "\n\t" /* Check that the counter is equal to 0 */
         "   jmpc    gt, 1b"
         "\n\t"
         /* Outputs */ :
         /* Inputs */  : "r"(r)
         /* Using */   : "$r0"
        );
#elif defined(STM32F4) || defined(MSVC2012EMU) || defined(MSVC2010EXPRESS)
    FTDELAY(r/1000);
#else
    delayMicroseconds(r);
#endif
    t1 = HAL_Read32(REG_CLOCK); /* t1 read */
    /* bitshift 6 places is the same as multiplying 64 */
    return ((t1 - t0) * 64); 
}
#endif
FTVOID HAL_FT800_ClkTrim ( FTVOID )
{
#if defined(TRIM_NEEDED)
#define LOW_FREQ_BOUND  58800000L//98% of 60Mhz
    FTU32 f;
    FTU8 i;
    /* Trim the internal clock by increase the REG_TRIM register
       till the measured frequency is within the acceptable range.*/
    for (i=0; (i < 31) && ((f= HAL_FT800_GetEVEFrq()) < LOW_FREQ_BOUND); i++)
    {
        /* increase the REG_TRIM register value 
           automatically increases the internal clock */
        HAL_Write8(REG_TRIM, i);  
    }
    /* Set the final frequency to be used for internal operations */
    HAL_Write32(REG_FREQUENCY,f);  
#endif
}
FTVOID HAL_FT800_LCDConfig ( FTVOID )
{
    /*
       Width Height 
       HCycle HOffset HSync0 HSync1 
       VCycle VOffset VSync0 VSync1 
       PCLK Swizzle PCLKPol Cspread Dither
     */
    FT800_LCD lcd = {FT800_LCD_WIDTH,FT800_LCD_HIGH, 
#if defined(LCD_WVGA)
        /* PCLK is critical,
           sometime it may need to set to larger number (2,3)
           some smaller number (set to 1), may case under run issue 
           while too much commands needs to be executed*/
        /* 
        this setting seems better than right side
        in real pratice of AllyTech project
         */
        1058,40,0,20,//928,88,0,48, 
        525,25,0,10,//525,32,0,3, 
        2,0,1,0,1};
#elif defined(LCD_QVGA)
        408,70,0,10, 
        263,13,0, 2, 
        8,2,0,1,1};
#elif defined(LCD_HVGA)
        400,40,0,10, 
        500,10,0, 5, 
        //4,2,1,1,1};
        5,2,1,1,1}; //for ME810A_HV35R pclk is 5
#else
        548,43,0,41, 
        292,12,0,10, 
        5,0,1,1,1};
#endif
    /* 
     * After recognizing the type of chip, 
     * perform the trimming if necessary 
     */
    HAL_FT800_ClkTrim();

    HAL_Write16(REG_HSIZE, lcd.Width);
    HAL_Write16(REG_VSIZE, lcd.Height);

    HAL_Write16(REG_HCYCLE, lcd.HCycle);
    HAL_Write16(REG_HOFFSET, lcd.HOffset);
    HAL_Write16(REG_HSYNC0, lcd.HSync0);
    HAL_Write16(REG_HSYNC1, lcd.HSync1);

    HAL_Write16(REG_VCYCLE, lcd.VCycle);
    HAL_Write16(REG_VOFFSET, lcd.VOffset);
    HAL_Write16(REG_VSYNC0, lcd.VSync0);
    HAL_Write16(REG_VSYNC1, lcd.VSync1);

    HAL_Write8(REG_SWIZZLE, lcd.Swizzle);
    HAL_Write8(REG_PCLK_POL, lcd.PCLKPol);

    HAL_Write16(REG_CSPREAD, lcd.Cspread);
    HAL_Write16(REG_DITHER, lcd.Dither);

    /* clear the screen */
    HAL_CmdBufIn(CMD_DLSTART);
    HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
    HAL_CmdBufIn(CLEAR(1,1,1));
    HAL_CmdBufIn(DISPLAY());
    HAL_CmdBufIn(CMD_SWAP);
    HAL_BufToReg(RAM_CMD,0);

    /* start to display */
    HAL_Write8(REG_PCLK,lcd.PCLK);

    /* set the backlight to highest */
    HAL_Write8(REG_PWM_DUTY,128);

    /* set the RGB output bit */
    /* 
        R:8,7,6
        G:5,4,3
        B:2,1,0
        default: 6x6x6
        0: 8x8x8
    HAL_Write32(REG_OUTBITS,0);
     */

    /* mute sound to avoid pop sound */
    HAL_Write16(REG_SOUND,0x0060);
    HAL_Write8(REG_PLAY,0x01);

#if defined(LCD_HVGA) && defined(FT9XXEV)
    /*
     * spi sck = system clock/SPI_Div, 100MHz/256=390KHz
     * ILI9488 looks like need 500KHz, 100MHz/128=781KHz, 
     * for FT900 no 200 can be selected 
     */
    ft9xx_spi_init(1,128);
    ft9xx_init_ili9488();
#endif
    /* set the SPI, DSPI, QSPI */
    HAL_FT800_SetSPI(EVE_SPI_TYPE); 
}

