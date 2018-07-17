#include "platform.h"

#if defined(UI_LITTLESWAN)
#include "arduino_xdemo_littleswan.h" 
#endif

FTU8 file_buff[MCU_BLOCK_SIZE] = {0};

sdcard SD;


STATIC FTVOID rdStart ( FTU32 addr )
{
    digitalWrite(EVE_SPI_CS, LOW);
    SPI.transfer((FTU8)(addr >> 16));
    SPI.transfer(highByte(addr));
    SPI.transfer(lowByte(addr));

    SPI.transfer(0); //Dummy Read Byte
}

STATIC FTVOID wrStart ( FTU32 addr )
{
    digitalWrite(EVE_SPI_CS, LOW);
    SPI.transfer(0x80 | (addr >> 16));
    SPI.transfer(highByte(addr));
    SPI.transfer(lowByte(addr));
}

FTVOID HAL_Write8 ( FTU32 addr, FTU8 data )
{
    wrStart(addr);

    SPI.transfer(data);

    digitalWrite(EVE_SPI_CS, HIGH);
}

FTVOID HAL_Write8Src ( FTU32 addr, FTU8 *src, FTU32 len )
{
    FTU32 i;

    wrStart(addr);

    for (i = 0; i < len; i++) {
        SPI.transfer(src[i]);
    }

    digitalWrite(EVE_SPI_CS, HIGH);	
}

FTVOID HAL_Write16 ( FTU32 addr, FTU16 data )
{
    wrStart(addr);

    SPI.transfer((FTU8)data&0xFF);
    SPI.transfer((FTU8)(data>>8)&0xFF);

    digitalWrite(EVE_SPI_CS, HIGH);
}

FTVOID HAL_Write32 ( FTU32 addr, FTU32 data )
{
    wrStart(addr);

    SPI.transfer((FTU8)data&0xFF);
    SPI.transfer((FTU8)(data>>8)&0xFF);
    SPI.transfer((FTU8)(data>>16)&0xFF);
    SPI.transfer((FTU8)(data>>24)&0xFF);

    digitalWrite(EVE_SPI_CS, HIGH);
}

FTVOID HAL_Cfg ( FTU8 cfg )
{
    digitalWrite(EVE_SPI_CS, LOW);
    SPI.transfer(cfg);
    SPI.transfer(0);
    SPI.transfer(0);
    digitalWrite(EVE_SPI_CS, HIGH);
}

FTU8 HAL_Read8 ( FTU32 addr )
{
    FTU8 tmp;

    rdStart(addr);

    tmp = SPI.transfer(0);

    digitalWrite(EVE_SPI_CS, HIGH);

    return tmp;
}

FTU32 HAL_Read8Buff ( FTU32 addr, FTU8 *buff, FTU32 len )
{
    FTU32 tmp = len;
    FTU8 *p = buff;

    rdStart(addr);

    while (tmp--) {
        *p = SPI.transfer(0);
        p++;
    }

    digitalWrite(EVE_SPI_CS, HIGH);

    return len;
}

FTU16 HAL_Read16 ( FTU32 addr )
{
    FTU16 tmp = 0;

    rdStart(addr);

    /* force type definition is critical while running on different platform */
    tmp = (FTU16)SPI.transfer(0);
    tmp |= (FTU16)SPI.transfer(0) << 8;

    digitalWrite(EVE_SPI_CS, HIGH);

    return tmp;
}

FTU32 HAL_Read32 ( FTU32 addr )
{
    FTU32 tmp = 0;

    rdStart(addr);

    /* force type definition is critical while running on different platform */
    tmp = (FTU32)SPI.transfer(0);
    tmp |= (FTU32)SPI.transfer(0) << 8;
    tmp |= (FTU32)SPI.transfer(0) << 16;
    tmp |= (FTU32)SPI.transfer(0) << 24;

    digitalWrite(EVE_SPI_CS, HIGH);

    return tmp;
}

FTVOID arduino_apps_sys_init(FTVOID)
{
#ifdef DBG_PRINT
    Serial.begin(9600);
    /* 
     * the delay is only for giving you some time
     * to manually open serial mornitor (e.g. Arduino IDE's, or 3th party's)
     * you may remove/modify it.
     */
    FTDELAY(5000);
    FTPRINT("\r\n");
    FTPRINT("Ver: ");
    FTPRINT(APPS_VER);
#endif
}

FTVOID arduino_apps_sys_dummy (FTU32 para)
{

}

FTVOID arduino_dumy_print (char * p)
{
/* do nothing */
}

FTVOID dos83(FTU8 dst[SDC_NAME_LEN+SDC_NAME_EXT_LEN], FT8 *ps)
{
	FTU8 i = 0;
	while (*ps) {
		if (*ps != '.')
			dst[i++] = toupper(*ps);
		else {
			while (i < SDC_NAME_LEN)
				dst[i++] = ' ';
		}
		ps++;
	}
	while (i < (SDC_NAME_LEN+SDC_NAME_EXT_LEN))
		dst[i++] = ' ';
}

FTVOID arduino_sdcardInit (FTVOID)
{
	pinMode(EVE_SDC_CS, OUTPUT);
	digitalWrite(EVE_SDC_CS, HIGH);

	FTDELAY(100);

	while(SD.begin(EVE_SDC_CS) == 0)
	{
		FTPRINT("\r\n");
		FTPRINT("No SD Card be found!");
		FTPRINT("\r\n");
		FTDELAY(1000);
		/* Endless loop for error */;
	}
}

FTU8 HAL_is_tag_vaild (FTC8 *dPath)
{
	FTU8 i, tag[] = CALD_TAG_DATA;

	for (i = 0; i < CALD_TAG_LEN; i++) {
		if (tag[i] != EEPROM.read(CALD_TAG_ADDR+i)) {
			return 0;
		}
	}

	return 1;
}

FTVOID HAL_invaild_tag (FTC8 *dataPath)
{
	FTU16 i;

	for (i = CALD_TAG_ADDR; i < CALD_TAG_LEN; i++) {
		EEPROM.write(i,0);
	}
}

FTVOID HAL_save_cdata (FTC8 *dataPath, FTU8 *p)
{
	FTU16 i, j;

	for (i = 0, j = 0; i < EVE_CAL_PARA_NUM; i++) {
		j = i*4;
		EEPROM.write((CALD_TAG_ADDR+CALD_TAG_LEN)+j,p[j]);
		EEPROM.write((CALD_TAG_ADDR+CALD_TAG_LEN)+j+1,p[j+1]);
		EEPROM.write((CALD_TAG_ADDR+CALD_TAG_LEN)+j+2,p[j+2]);
		EEPROM.write((CALD_TAG_ADDR+CALD_TAG_LEN)+j+3,p[j+3]);
	}
}

FTVOID HAL_restore_cdata (FTC8 *dataPath, FTU8 *p)
{
	FTU16 i;

	for (i = 0;i < EVE_CAL_PARA_NUM;i++) {
		p[i*4] = EEPROM.read((CALD_TAG_ADDR+CALD_TAG_LEN)+i*4);
		p[i*4+1] = EEPROM.read((CALD_TAG_ADDR+CALD_TAG_LEN)+i*4+1);
		p[i*4+2] = EEPROM.read((CALD_TAG_ADDR+CALD_TAG_LEN)+i*4+2);
		p[i*4+3] = EEPROM.read((CALD_TAG_ADDR+CALD_TAG_LEN)+i*4+3);
	}
}

FTVOID HAL_vaild_tag (FTVOID)
{
	FTU8 tag[] = CALD_TAG_DATA;
	FTU16 i;

	for (i = 0; i < CALD_TAG_LEN; i++) {
		EEPROM.write(CALD_TAG_ADDR+i,tag[i]);
	}
}

FTVOID HAL_ili9488 (FTVOID)
{
//do nothing;
}

FTVOID FTUDELAY(FTU32 uS)
{
    delayMicroseconds(uS);
}

FTVOID HAL_speed_up (FTU32 type)
{
//do nothing;
}

FTVOID HAL_PwdCyc ( FTU8 OnOff )
{
#define PWC_DELAY 20
    digitalWrite(EVE_PD_N, OnOff?LOW:HIGH);
    FTDELAY(PWC_DELAY);

    digitalWrite(EVE_PD_N, OnOff?HIGH:LOW);
    FTDELAY(PWC_DELAY);
    /* 
     * I was not quite understand why a read action is needed
     * but for VM801Plus module, it seems like a critical action 
     */
    HAL_Read16(0);
}

FTVOID HAL_SpiInit ( FTVOID )
{
    /* 
    the SPI clock shall not exceed 11MHz before system clock is enabled. 
    After system clock is properly enabled, 
    the SPI clock is allowed to go up to 30MHz.	
    
    spi sck = system clock/DIVx, 16MHz/2 = 8MHz*/
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
}

FTVOID HAL_preparation (FTVOID)
{
    /* set the GPIO pin */
    pinMode(EVE_SPI_CS, OUTPUT);
    digitalWrite(EVE_SPI_CS, HIGH);

    pinMode(EVE_PD_N, OUTPUT);
    digitalWrite(EVE_PD_N, HIGH);

    arduino_sdcardInit();
}

FTU32 HAL_WriteSrcToDes (FTU32 handle, FTU32 src, FTU32 des, FTU32 len)
{
    FTU32 i,l;
    FTU8 p[SDC_SECTOR_SIZE] = {0};
    Reader sdC;

    /* only accept forward read
     * make sure not backward over read 
     * the previous finished read info */
    if (sdC.offset > src) {
        FTPRINT("\nHAL_WriteSrcToDes: backward reading");
        return 0;
    }

    for (l = 0, i = src%SDC_SECTOR_SIZE; l < len; ) {
        sdC.readsector(p);
        for (; i < SDC_SECTOR_SIZE && l < len; i++,l++) {
            *(FTU8 *)(des+l) = p[i];
        }
        i = 0;
    }

    return l;
}

FTU8 * HAL_LoopMemMalloc (FTU32 handle, FTU32 src, FTU32 len)
{
    return file_buff;
}

FTVOID HAL_LoopMemRead (FTU32 handle, FTU8 **ppbuf, FTU32 len)
{
    Reader sdC;

    sdC.readsector(*ppbuf);
}

FTVOID HAL_LoopMemFree (FTU32 buf)
{
    // do nothing
}

FTU32 HAL_SegFileOpen (FTU8 *path)
{
    Reader sdC;

    if (!sdC.openfile((FT8 *)path)) {
        FTPRINT("\nHAL_LoopMemOpen: file open error");
        return 0;
    }

    return 1;
}

FTU32 HAL_SegFileSize (FTU32 handle)
{
    Reader sdC;
    
    return BYTES4ALIGN(sdC.size);
}

FTVOID HAL_SegFileClose (FTU32 handle)
{
    // do nothing
}

FTU8 HAL_ZlibCompressed (FTU32 handle, FTU32 src)
{
    FTU8 header[512] = {0};
    Reader sdC;

    sdC.readsector(header);
    sdC.seek(src);
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
