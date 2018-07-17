#include "platform.h"

#if defined(UI_BOMS_HID)
#include "vc2012emu_xdemo_boms_hid.c"
#endif

STATIC FTVOID rdStart ( FTU32 addr )
{
    FT8XXEMU_cs(1);
    FT8XXEMU_transfer((FTU8)(addr >> 16));
    FT8XXEMU_transfer((FTU8)(addr >> 8));
    FT8XXEMU_transfer((FTU8)addr);
    FT8XXEMU_transfer(0);
}

STATIC FTVOID wrStart ( FTU32 addr )
{
    FT8XXEMU_cs(1);
    FT8XXEMU_transfer((FTU8)(addr >> 16) | 0x80);
    FT8XXEMU_transfer((FTU8)(addr >> 8));
    FT8XXEMU_transfer((FTU8)addr);
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

FTVOID HAL_Cfg ( FTU8 cfg )
{
    FT8XXEMU_transfer(cfg);
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

FTVOID vc2012emu_apps_sys_dummy (FTU32 para)
{
/* do nothing */
}

FTVOID vc2012emu_dumy_print (char *p)
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
		FTPRINT("\nvc2012emu_save_cdata: fail to open file");
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
    /* Do nothing */
}

FTVOID HAL_SpiInit ( FTVOID )
{
    //do nothing
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
