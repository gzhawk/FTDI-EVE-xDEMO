
#include "..\..\FT800_platform.h"

FTVOID vc2010_dumy_print(char *p)
{
/*do nothing*/
}
FTVOID vc2010_spi_init(FTVOID)
{
    ChannelConfig cConf;
	FT_DEVICE_LIST_INFO_NODE devList;
	FTU32 i;

    Init_libMPSSE();

	SPI_GetNumChannels(&i);
	while (i == 0) {
		DBGPRINT;
		/* Endless loop for error*/;	
	}
	
	i = SPI_GetChannelInfo(0,&devList);
	while (FT_OK != i) {
		DBGPRINT;
		/* Endless loop for error*/;	
	}

	i = SPI_OpenChannel(0,&ftHandle);
	while (FT_OK != i) {
		DBGPRINT;
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
		DBGPRINT;
		/* Endless loop for error*/;	
	}
}

FTVOID vc2010_invaild_tag (FTC8 *dataPath)
{
	remove(dataPath);
}

FTU8 vc2010_is_tag_vaild (FTC8 *dataPath)
{
	FILE *pF;
	
	pF = fopen(dataPath,"rb");
	if(NULL == pF) {
		return 0;
	}
	fclose(pF);
	return 1;
}

FTVOID vc2010_save_cdata (FTC8 *dataPath, FTU8 *p)
{
	FILE *pF;
	
	pF = fopen(dataPath,"wb");
	if(NULL == pF) {
		DBGPRINT;
		/* don't stop the bootup, 
		   if only can not open file */
		return;
	}

	fwrite(p, 4, FT800_CAL_PARA_NUM, pF);
	fclose(pF);
}

FTVOID vc2010_restore_cdata (FTC8 *dataPath, FTU8 *p)
{
	FILE *pF;
	
	pF = fopen(dataPath,"rb");
	fread(p, 4, FT800_CAL_PARA_NUM, pF);
	fclose(pF);
}

FTVOID vc2010_vaild_tag (FTVOID)
{
	/* already save it to file, the file is the 'tag' */
	/* give some time to file system to finish operation */
	FTDELAY(100);
}


