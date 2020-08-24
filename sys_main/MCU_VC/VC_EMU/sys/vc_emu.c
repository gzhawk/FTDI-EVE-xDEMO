#include "platform.h"

BT8XXEMU_Emulator *gEmulator = NULL;

STATIC FTVOID EMU_cs_en(FTVOID)
{
	BT8XXEMU_chipSelect(gEmulator, 1);
}

STATIC FTVOID EMU_cs_dis(FTVOID)
{
	BT8XXEMU_chipSelect(gEmulator, 0);
}

FTU8 EMU_transfer(FTU8 data)
{
    return BT8XXEMU_transfer(gEmulator, data);
}

FTVOID rdStart ( FTU32 addr )
{
    EMU_cs_en();
    EMU_transfer((FTU8)(addr >> 16));
    EMU_transfer((FTU8)(addr >> 8));
    EMU_transfer((FTU8)addr);
    EMU_transfer(0);
}

FTVOID wrStart ( FTU32 addr )
{
    EMU_cs_en();
    EMU_transfer((FTU8)(addr >> 16) | 0x80);
    EMU_transfer((FTU8)(addr >> 8));
    EMU_transfer((FTU8)addr);
}

FTVOID HAL_Cfg ( FTU8 cfg )
{
    EMU_transfer(cfg);
}

FTVOID HAL_Cfg3 ( FTU32 cfg3 )
{
    EMU_transfer((FTU8)cfg3);
    EMU_transfer((FTU8)(cfg3>>8));
    EMU_transfer((FTU8)(cfg3>>16));
}

FTVOID HAL_Write8 ( FTU32 addr, FTU8 data )
{
    wrStart(addr);
    EMU_transfer(data);
    EMU_cs_dis();
}

FTVOID HAL_Write8Src ( FTU32 addr, FTU8 *src, FTU32 len )
{
    FTU32 i;

    wrStart(addr);
    for (i = 0; i < len; i++) {
        EMU_transfer(src[i]);
    }
    EMU_cs_dis();
}

FTVOID HAL_Write16 ( FTU32 addr, FTU16 data )
{
    wrStart(addr);

    EMU_transfer((FTU8)(data&0xFF));
    EMU_transfer((FTU8)((data>>8)&0xFF));

    EMU_cs_dis();
}

FTVOID HAL_Write32 ( FTU32 addr, FTU32 data )
{
    wrStart(addr);

    EMU_transfer((FTU8)(data&0xFF));
    EMU_transfer((FTU8)((data>>8)&0xFF));
    EMU_transfer((FTU8)((data>>16)&0xFF));
    EMU_transfer((FTU8)((data>>24)&0xFF));

    EMU_cs_dis();
}

FTU8 HAL_Read8 ( FTU32 addr )
{
    FTU8 tmp;

    rdStart(addr);

    tmp = EMU_transfer(0);

    EMU_cs_dis();

    return tmp;
}

FTU32 HAL_Read8Buff ( FTU32 addr, FTU8 *buff, FTU32 len )
{
    FTU32 tmp = len;
    FTU8 *p = buff;

    rdStart(addr);

    while (tmp--) {
        *p = EMU_transfer(0);
        p++;
    }

    EMU_cs_dis();

    return len;
}

FTU16 HAL_Read16 ( FTU32 addr )
{
    FTU16 tmp = 0;

    rdStart(addr);

    tmp = (FTU16)EMU_transfer(0);
    tmp |= (FTU16)EMU_transfer(0) << 8;

    EMU_cs_dis();

    return tmp;
}

FTU32 HAL_Read32 ( FTU32 addr )
{
    FTU32 tmp = 0;

    rdStart(addr);

    tmp = (FTU32)EMU_transfer(0);
    tmp |= (FTU32)EMU_transfer(0) << 8;
    tmp |= (FTU32)EMU_transfer(0) << 16;
    tmp |= (FTU32)EMU_transfer(0) << 24;

    EMU_cs_dis();

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
    /* do nothing, reuse it to print debug information */
    FTPRINT("\nEmulator version: %s",(char *)BT8XXEMU_version());
    FTPRINT("\nEmulator chip   : %x",EVEMODE);
}

FT32 emu_main (FTVOID)
{
    BT8XXEMU_Flash *flash = NULL;
    BT8XXEMU_FlashParameters flashParams;
    BT8XXEMU_EmulatorParameters params;

    BT8XXEMU_Flash_defaults(BT8XXEMU_VERSION_API, &flashParams);

    wcscpy(flashParams.DeviceType, BTFLASH_DEVICE_TYPE);
    wcscpy(flashParams.DataFilePath, BTFLASH_DATA_FILE);
    flashParams.SizeBytes = BTFLASH_SIZE;
    flashParams.StdOut = 1;
    
    flash = BT8XXEMU_Flash_create(BT8XXEMU_VERSION_API, &flashParams);

    BT8XXEMU_defaults(BT8XXEMU_VERSION_API, &params, EVEMODE);

    params.Flags |= BT8XXEMU_EmulatorEnableStdOut;
    params.Main = mcu;
    params.Flash = flash;
    
    BT8XXEMU_run(BT8XXEMU_VERSION_API, &gEmulator, &params);

    BT8XXEMU_stop(gEmulator);
    BT8XXEMU_destroy(gEmulator);
    BT8XXEMU_Flash_destroy(flash);

    return 0;
}
