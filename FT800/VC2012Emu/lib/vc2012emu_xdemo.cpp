#include "vc2012emu_xdemo.h"

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
		DBGPRINT;
		/* don't stop the bootup, 
		   if only can not open file */
		return;
	}

	fwrite(p, 4, FT800_CAL_PARA_NUM, pF);
	fclose(pF);
}

FTVOID vc2012emu_restore_cdata (FTC8 *dataPath, FTU8 *p)
{
	FILE *pF;
	
	pF = fopen(dataPath,"rb");
	fread(p, 4, FT800_CAL_PARA_NUM, pF);
	fclose(pF);
}

FTVOID vc2012emu_vaild_tag (FTVOID)
{
	/* already save it to file, the file is the 'tag' */
	/* give some time to file system to finish operation */
	FTDELAY(100);
}

