#include "platform.h"

BT8XXEMU_Emulator *gEmulator = NULL;

FTVOID FT8XXEMU_cs(FT8 i)
{
	BT8XXEMU_chipSelect(gEmulator, i);
}

FTU32 FT8XXEMU_transfer(FTU32 data)
{
    return BT8XXEMU_transfer(gEmulator, data);
}

FTVOID rdStart ( FTU32 addr )
{
    FT8XXEMU_cs(1);
    FT8XXEMU_transfer((FTU8)(addr >> 16));
    FT8XXEMU_transfer((FTU8)(addr >> 8));
    FT8XXEMU_transfer((FTU8)addr);
    FT8XXEMU_transfer(0);
}

FTVOID wrStart ( FTU32 addr )
{
    FT8XXEMU_cs(1);
    FT8XXEMU_transfer((FTU8)(addr >> 16) | 0x80);
    FT8XXEMU_transfer((FTU8)(addr >> 8));
    FT8XXEMU_transfer((FTU8)addr);
}

FTVOID HAL_Cfg ( FTU8 cfg )
{
    FT8XXEMU_transfer(cfg);
}

FTVOID HAL_Cfg3 ( FTU32 cfg3 )
{
    FT8XXEMU_transfer(cfg3);
    FT8XXEMU_transfer(cfg3>>8);
    FT8XXEMU_transfer(cfg3>>16);
}

FTVOID HAL_Write8 ( FTU32 addr, FTU8 data )
{
    wrStart(addr);
    FT8XXEMU_transfer(data);
    FT8XXEMU_cs(0);
}

FTVOID HAL_Write8Src ( FTU32 addr, FTU8 *src, FTU32 len )
{
    FTU32 i;

    wrStart(addr);
    for (i = 0; i < len; i++) {
        FT8XXEMU_transfer(src[i]);
    }
    FT8XXEMU_cs(0);
}

FTVOID HAL_Write16 ( FTU32 addr, FTU16 data )
{
    wrStart(addr);

    FT8XXEMU_transfer((FTU8)data&0xFF);
    FT8XXEMU_transfer((FTU8)(data>>8)&0xFF);

    FT8XXEMU_cs(0);
}

FTVOID HAL_Write32 ( FTU32 addr, FTU32 data )
{
    wrStart(addr);

    FT8XXEMU_transfer((FTU8)data&0xFF);
    FT8XXEMU_transfer((FTU8)(data>>8)&0xFF);
    FT8XXEMU_transfer((FTU8)(data>>16)&0xFF);
    FT8XXEMU_transfer((FTU8)(data>>24)&0xFF);

    FT8XXEMU_cs(0);
}

FTU8 HAL_Read8 ( FTU32 addr )
{
    FTU8 tmp;

    rdStart(addr);

    tmp = FT8XXEMU_transfer(0);

    FT8XXEMU_cs(0);

    return tmp;
}

FTU32 HAL_Read8Buff ( FTU32 addr, FTU8 *buff, FTU32 len )
{
    FTU32 tmp = len;
    FTU8 *p = buff;

    rdStart(addr);

    while (tmp--) {
        *p = FT8XXEMU_transfer(0);
        p++;
    }

    FT8XXEMU_cs(0);

    return len;
}

FTU16 HAL_Read16 ( FTU32 addr )
{
    FTU16 tmp = 0;

    rdStart(addr);

    tmp = (FTU16)FT8XXEMU_transfer(0);
    tmp |= (FTU16)FT8XXEMU_transfer(0) << 8;

    FT8XXEMU_cs(0);

    return tmp;
}

FTU32 HAL_Read32 ( FTU32 addr )
{
    FTU32 tmp = 0;

    rdStart(addr);

    tmp = (FTU32)FT8XXEMU_transfer(0);
    tmp |= (FTU32)FT8XXEMU_transfer(0) << 8;
    tmp |= (FTU32)FT8XXEMU_transfer(0) << 16;
    tmp |= (FTU32)FT8XXEMU_transfer(0) << 24;

    FT8XXEMU_cs(0);

    return tmp;
}

FTVOID HAL_speed_up (FTU32 type)
{
    /* do nothing */
}

FTVOID HAL_PwdCyc ( FTU8 OnOff )
{
    /* do nothing */
}

FTVOID HAL_SpiInit ( FTVOID )
{
    /* do nothing */
}
