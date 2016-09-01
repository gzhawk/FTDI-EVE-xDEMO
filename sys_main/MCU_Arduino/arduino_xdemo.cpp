#include "arduino_xdemo.h"

sdcard SD;

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


FTU8 arduino_tst_msg[TST_CMD_LEN+TST_MSG_LEN+1] = {0xAA,6,0xBA,0,2,2,0,0,0,0,0};

FTU8 arduino_tst_crc (FTU8 * p)
{
	FTU32 i;
	FTU8 crc;

	for (i = 1, crc = 0; i < (TST_CMD_LEN+TST_MSG_LEN+1); i++) {
		crc += p[i];
	}

	return (~crc+1);
}

FTVOID arduino_simugpio(FTU8 flag)
{
	FTU32 i;

	if (flag == 0) {
		arduino_tst_msg[TST_CMD_LEN] = 0;
	} else {
		arduino_tst_msg[TST_CMD_LEN] = 1;
	}

	for (i = 0; i < (TST_CMD_LEN+TST_MSG_LEN+1); i++) {
		FTPRINT(arduino_tst_msg[i]);
		//FTPRINT(',');
	}

	FTPRINT(arduino_tst_crc(arduino_tst_msg));
}

FTVOID arduino_sdcardInit (FTVOID)
{
	pinMode(FT800_SDC_CS, OUTPUT);
	digitalWrite(FT800_SDC_CS, HIGH);

	FTDELAY(100);

	while(SD.begin(FT800_SDC_CS) == 0)
	{
		FTPRINT("\r\n");
		FTPRINT("No SD Card be found!");
		FTPRINT("\r\n");
		FTDELAY(1000);
		/* Endless loop for error */;
	}
}

FTU8 arduino_is_tag_vaild (FTVOID)
{
	FTU8 i, tag[] = CALD_TAG_DATA;

	for (i = 0; i < CALD_TAG_LEN; i++) {
		if (tag[i] != EEPROM.read(CALD_TAG_ADDR+i)) {
			return 0;
		}
	}

	return 1;
}

FTVOID arduino_invaild_tag (FTVOID)
{
	FTU16 i;

	for (i = CALD_TAG_ADDR; i < CALD_TAG_LEN; i++) {
		EEPROM.write(i,0);
	}
}

FTVOID arduino_save_cdata (FTU8 *p)
{
	FTU16 i, j;

	for (i = 0, j = 0; i < FT800_CAL_PARA_NUM; i++) {
		j = i*4;
		EEPROM.write((CALD_TAG_ADDR+CALD_TAG_LEN)+j,p[j]);
		EEPROM.write((CALD_TAG_ADDR+CALD_TAG_LEN)+j+1,p[j+1]);
		EEPROM.write((CALD_TAG_ADDR+CALD_TAG_LEN)+j+2,p[j+2]);
		EEPROM.write((CALD_TAG_ADDR+CALD_TAG_LEN)+j+3,p[j+3]);
	}
}

FTVOID arduino_restore_cdata (FTU8 *p)
{
	FTU16 i;

	for (i = 0;i < FT800_CAL_PARA_NUM;i++) {
		p[i*4] = EEPROM.read((CALD_TAG_ADDR+CALD_TAG_LEN)+i*4);
		p[i*4+1] = EEPROM.read((CALD_TAG_ADDR+CALD_TAG_LEN)+i*4+1);
		p[i*4+2] = EEPROM.read((CALD_TAG_ADDR+CALD_TAG_LEN)+i*4+2);
		p[i*4+3] = EEPROM.read((CALD_TAG_ADDR+CALD_TAG_LEN)+i*4+3);
	}
}

FTVOID arduino_vaild_tag (FTVOID)
{
	FTU8 tag[] = CALD_TAG_DATA;
	FTU16 i;

	for (i = 0; i < CALD_TAG_LEN; i++) {
		EEPROM.write(CALD_TAG_ADDR+i,tag[i]);
	}
}

