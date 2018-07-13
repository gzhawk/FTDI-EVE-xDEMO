#include "platform.h"

#if defined(UI_BOMS_HID)
#include "vc2012emu_xdemo_boms_hid.c"
#endif

FTVOID vc2012emu_apps_sys_dummy (FTU32 para)
{
}
FTVOID vc2012emu_dumy_print (char *p)
{
/* do nothing */
}
FTVOID vc2012emu_invaild_tag (FTC8 *dataPath)
{
	remove(dataPath);
}

FTU8 vc2012emu_is_tag_vaild (FTC8 *dataPath)
{
	FILE *pF;
	
	pF = fopen(dataPath,"rb");
	if(NULL == pF) {
		return 0;
	}
	fclose(pF);
	return 1;
}

FTVOID vc2012emu_save_cdata (FTC8 *dataPath, FTU8 *p)
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

FTVOID vc2012emu_restore_cdata (FTC8 *dataPath, FTU8 *p)
{
	FILE *pF;
	
	pF = fopen(dataPath,"rb");
	fread(p, 4, EVE_CAL_PARA_NUM, pF);
	fclose(pF);
}

FTVOID vc2012emu_vaild_tag (FTVOID)
{
	/* already save it to file, the file is the 'tag' */
	/* give some time to file system to finish operation */
	FTDELAY(100);
}

