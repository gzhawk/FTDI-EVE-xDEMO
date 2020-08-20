#include "platform.h"

/* the extra 3 is for SPI address, 
   it's a trick in FT4222 platform
   go FT4222 HAL_Write8Src for more detail*/
FTU8 file_buff[MCU_BLOCK_SIZE+3] = {0};

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
    if (len > MCU_BLOCK_SIZE) {
		FTPRINT("\nHAL_LoopMemMalloc: malloc exceeded");
    }
    /* check FT4222 HAL_Write8Src for the reason of offset 3 */
    return file_buff+3;
}

FTVOID HAL_LoopMemRead (FTU32 handle, FTU8 **ppbuf, FTU32 len)
{
    fread(*ppbuf, 1, len, (FILE *)handle);
}

FTVOID HAL_LoopMemFree (FTU32 buf)
{
    /* do nothing */
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
FTU8 UTF8_Pump (FTU8 *utf8_file, FTU8 * p, FTU32 utf8_num)
{
#define UTF8_LEN 3
    /* it just a test purpuse routine, NOT for common use */
    static FTU32 index = 0;
	static FILE *pF = NULL;
    FTU8 header[UTF8_LEN] = {0}, *tmp = p;
    FTU32 i,rd;

    if (!pF) {
        pF = fopen(utf8_file,"rb");
        if(NULL == pF) {
            FTPRINT("\nUTF8_Pump: file open error");
            return 0;
        }

        fread(header, UTF8_LEN, 1, pF);

        if (header[0] != 0xEF || 
            header[1] != 0xBB || 
            header[2] != 0xBF) {
            FTPRINT("\nUTF8_Pump: not utf-8 file");
            return 0;
        }

        index = UTF8_LEN;
    }

    for (i = 0; i < utf8_num; i++) {
		rd = fread(tmp, UTF8_LEN, 1, pF);
        if (!rd) {
            fseek(pF, UTF8_LEN, SEEK_SET);
		    fread(tmp, UTF8_LEN, 1, pF);
            index = UTF8_LEN;
            FTPRINT("\nUTF8_Pump: read from head again");
        }
        //this test TXT file use '0x0D,0x0A' to seperate each UTF8 char
        index += UTF8_LEN + 2;
        tmp += UTF8_LEN;
        fseek(pF, index, SEEK_SET);
    }
    tmp = '\0';

    return 1;
}
