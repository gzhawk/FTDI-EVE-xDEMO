/* 
    Applicaton, Integrated function for FT800
    Author: Hawk
    Email : hawk.gao@ftdichip.com	
    Date  : 2013/Oct
*/
#include "FT800_platform.h"
#include "FT800_Gpu.h"
#include "FT800_HAL.h"
#include "FT800_APP.h"

#define FONT_CAL 27
#define FORCE_WAIT_COUNT 5
#define FORCE_WAIT_TIME 500

typedef struct wrFuncPara_ {
	FTRES res;
	FTU32 Src;
	FTU32 Des;
	FTU32 len;
}wrFuncPara;

wrFuncPara fPara = {0};

#if !defined(STM32F4)&&!defined(MSVC2010EXPRESS)&&!defined(MSVC2012EMU)
FTU8 file_buff[FT800_BLOCK_SIZE] = {0};
#endif

#if defined(CAL_NEEDED)
FTINDEF appRet_en appCalCmd (FTU8 font, FTC8 *str1, FTC8 *str2)
{
	FTU32 ret_addr;

	HAL_CmdBufIn(CMD_DLSTART);

	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));
	HAL_CmdBufIn(COLOR_RGB(255,255,255));

	CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,font,OPT_CENTER,str1);
	CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/3*2,font,OPT_CENTER,(str2 == NULL)?"NULL":str2);
	HAL_CmdBufIn(CMD_CALIBRATE);
	HAL_CmdBufIn(0);
	ret_addr = HAL_CalResultAddr();
	HAL_BufToReg(RAM_CMD,0);
	
	return (0 != HAL_Read32(ret_addr))?APP_OK:APP_ERR_CALC;
}

FTINDEF appRet_en appCalForce (FTC8 *dataPath)
{
	FTU32 CData[FT800_CAL_PARA_NUM] = {0},
		  reg = REG_TOUCH_TRANSFORM_A, i = 0;

	/* make the tag part invalid */
#if defined(STM32F4)
	stm32f4_invaild_tag();
#elif defined(MSVC2012EMU)
	vc2012emu_invaild_tag(dataPath);
#elif defined(MSVC2010EXPRESS)
	vc2010_invaild_tag(dataPath);
#elif defined(FT9XXEV)
	ft9xx_invaild_tag(dataPath);
#else
	arduino_invaild_tag();
#endif
	
	/* do calibration to get the CData */
#if defined(STM32F4)
	if (APP_OK == appCalCmd(FONT_CAL,(FTC8 *)"Tap the flashing point (STM32F4)",dataPath)) {
#elif defined(MSVC2012EMU)
	if (APP_OK == appCalCmd(FONT_CAL,(FTC8 *)"Tap the flashing point (VC2012Emu)",dataPath)) {
#elif defined(MSVC2010EXPRESS)
	if (APP_OK == appCalCmd(FONT_CAL,(FTC8 *)"Tap the flashing point (VC2010)",dataPath)) {
#elif defined(FT9XXEV)
	if (APP_OK == appCalCmd(FONT_CAL,(FTC8 *)"Tap the flashing point (FT9XX)",dataPath)) {
#else
	if (APP_OK == appCalCmd(FONT_CAL,(FTC8 *)"Tap the flashing point (Arduino)",dataPath)) {
#endif
		for (i = 0; i < FT800_CAL_PARA_NUM; reg+=FTU32_LEN,i++) {
			CData[i] = HAL_Read32(reg);
		}
		/* save the CData */
#if defined(STM32F4)
		stm32f4_save_cdata((FTU16 *)CData);
#elif defined(MSVC2012EMU)
		vc2012emu_save_cdata(dataPath, (FTU8 *)CData);
#elif defined(MSVC2010EXPRESS)
		vc2010_save_cdata(dataPath, (FTU8 *)CData);
#elif defined(FT9XXEV)
		ft9xx_save_cdata(&FT9xxFile,dataPath, (FTU8 *)CData);
#else
		arduino_save_cdata((FTU8 *)CData);
#endif

		/* make the tag part valid */
#if defined(STM32F4)
		stm32f4_vaild_tag();
#elif defined(MSVC2012EMU)
		vc2012emu_vaild_tag();
#elif defined(MSVC2010EXPRESS)
		vc2010_vaild_tag();
#elif defined(FT9XXEV)
		ft9xx_vaild_tag();
#else
		arduino_vaild_tag();
#endif
	}
	return APP_OK;
}
#endif

FTINDEF appRet_en appCal (FTU8 force, FTC8 *dPath)
{
#if defined(CAL_NEEDED)
	FTU32 CData[FT800_CAL_PARA_NUM] = {0};
	FTU32 reg = REG_TOUCH_TRANSFORM_A, i = 0;

	FTPRINT("\r\n");
	FTPRINT("FT800 calibration");
#if defined(MSVC2012EMU) || defined(MSVC2010EXPRESS) || defined(FT9XXEV)
	if (force || (dPath == NULL)) {
#else
	if (force) {
#endif
		return appCalForce(dPath);
	}

#if defined(STM32F4)
	if (!stm32f4_is_tag_vaild()) {
#elif defined(MSVC2012EMU)
	if (!vc2012emu_is_tag_vaild(dPath)) {
#elif defined(MSVC2010EXPRESS)
	if (!vc2010_is_tag_vaild(dPath)) {
#elif defined(FT9XXEV)
	if (!ft9xx_is_tag_vaild(&FT9xxFile,dPath)) {
#else
	if (!arduino_is_tag_vaild()) {
#endif
		return appCalForce(dPath);
	}

#if defined(STM32F4)
	stm32f4_restore_cdata((FTU16 *)CData);
#elif defined(MSVC2012EMU)
	vc2012emu_restore_cdata(dPath, (FTU8 *)CData);
#elif defined(MSVC2010EXPRESS)
	vc2010_restore_cdata(dPath, (FTU8 *)CData);
#elif defined(FT9XXEV)
	ft9xx_restore_cdata(&FT9xxFile,dPath, (FTVOID *)CData);
#else
	arduino_restore_cdata((FTU8 *)CData);
#endif
	while (reg <= REG_TOUCH_TRANSFORM_F) {
		HAL_Write32(reg, CData[i]);
		i++;
		reg += FTU32_LEN;
	}
#endif
	return APP_OK;
}

appRet_en appWaitCal (FTVOID)
{
	FTU8 i = 0;
	appRet_en ret;

	/* Cal would judge whether it should do the calibration or not  */
	do {
		/* keep doing the calibration until it success */
		ret = appCal(i,CDATA_PATH);
	} while (ret != APP_OK);
	
	/* wait for a while, 
	 * let user decide if need to force calibration
	 * or see some debug information */
	i = FORCE_WAIT_COUNT;
	while (!TOUCHED) {
		HAL_FT800_BootupDisp(i);

		FTDELAY(FORCE_WAIT_TIME);
		if (0 == --i) {
			break;
		}
	}

	/* deciede to do the calibration or not depend on user's input */
	if (i) {
		/* into the calibration, after press released */
		while (TOUCHED) {
			FTDELAY(FORCE_WAIT_TIME);
		}
		do {
			/* keep doing the calibration until it success */
			ret = appCal(i,CDATA_PATH);
		} while (ret != APP_OK);
	}

	return APP_OK; 
}
/* 
 * call back routine for multi-platform
 * read data from Src (MCU, EVE, Flash, SD Card) to Des (MCU, EVE, Flash, SD Card)
 * at a time 
 * set the length as return
 */
FTVOID resWrBuf (FTU32 para)
{
	wrFuncPara *wfp = (wrFuncPara *) para;
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
	if (wfp->len > 0) {
		fseek(wfp->res,wfp->Src,SEEK_SET);
		fread((FTVOID *)wfp->Des,1,wfp->len,wfp->res);
	}
#elif defined(STM32F4)
	FTU32 i;
	for (i = 0; i < wfp->len ; i++) {
		*(FTU8 *)(wfp->Des+i) = *(FTU8 *)(wfp->res+wfp->Src+i);
	}
#elif defined(FT9XXEV)
	FTU32 l;
	if (wfp->len > 0) {
		f_lseek(wfp->res,wfp->Src);
		f_read(wfp->res, (FTVOID *)wfp->Des,wfp->len,&l);
	}
#else/* Arduino */
	FTU32 i,l;
	FTU8 p[SDC_SECTOR_SIZE] = {0};

	/* only accept increasly read
	 * make sure not over read the previous reading */
	if (wfp->res.offset > wfp->Src) {
		wfp->len = 0;
		DBGPRINT;
		return;
	}

	for (l = 0, i = wfp->Src%SDC_SECTOR_SIZE; l < wfp->len; ) {
		wfp->res.readsector(p);
		for (; i < SDC_SECTOR_SIZE && l < wfp->len; i++,l++) {
			*(FTU8 *)(wfp->Des+l) = p[i];
		}
		i = 0;
	}
		
#endif
}
/* 
 * call back routine for multi-platform
 * read data from Src (MCU, EVE, Flash, SD Card) to Des (EVE-CMD, EVE-DLP, EVE-RAM_G)
 * block by block
 * set the length as return
 */
FTVOID resWrEve (FTU32 para)
{
	wrFuncPara *wfp = (wrFuncPara *) para;
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
	/* make sure the data length into EVE is 4bytes aligned */
	FTU32 l = BYTES4ALIGN(wfp->len);
	FTU8 * p = 0;

	p = (FTU8 *)malloc(l);
	if (p == 0) {
		DBGPRINT;
		return;
	}
	/* clean up the buffer, in case need to set '0' to tail */
	memset(p,0,l);
	fseek(wfp->res,wfp->Src,SEEK_SET);
	/* only read real length of file */
	fread(p,1,wfp->len,wfp->res);
	/* read the 4 bytes align length */
	appBlockWrite(wfp->Des, p, l, FT800_BLOCK_SIZE);
	wfp->len = l;
	free(p);
#elif defined(STM32F4)
	/* make sure the data length into EVE is 4bytes aligned */
	FTU32 l = BYTES4ALIGN(wfp->len);
	/* read the 4 bytes align length */
	(FTVOID)appBlockWrite(wfp->Des, (FTU8 *)(wfp->res+wfp->Src), l, FT800_BLOCK_SIZE);
	wfp->len = l;
#else/* Arduino and FT9XXEV */
	/* make sure the data length into EVE is 4bytes aligned */
	FTU32 i,j,l = FT800_BLOCK_SIZE,x = BYTES4ALIGN(wfp->len);
	FTU8 * p = 0;
	p = file_buff;

#if !defined(FT9XXEV)
	/* only accept increasly read
	 * make sure not over read the previous reading */
	if (wfp->res.offset > wfp->Src) {
		DBGPRINT;
		wfp->len = 0;
		return;
	}
#endif

	for (i = 0; i < x; ) {
#if defined(FT9XXEV)
		f_lseek(wfp->res,wfp->Src+i);
		/* '&j' is useless, just re-use j to fill up the f_read para */
		f_read(wfp->res, (FTVOID *)p,l,&j);
#else
		wfp->res.readsector(p);
#endif
		l = ((x - i) > FT800_BLOCK_SIZE)?FT800_BLOCK_SIZE:(x - i);
		HAL_Write8Src(wfp->Des+i, p, l);
		i += l;
	}
		
#endif
}
/* 
 * call back routine for multi-platform
 * read data from Src (MCU, EVE, Flash, SD Card) to Des (EVE-CMD)
 * in loop buffer in EVE (CMD buffer)
 * set the length as return
 */
FTVOID resWrEveCmd (FTU32 para)
{
	wrFuncPara *wfp = (wrFuncPara *) para;

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
	/* make sure the data length into EVE is 4bytes aligned */
	FTU32 l = BYTES4ALIGN(wfp->len);
	FTU8 * p = 0;

	p = (FTU8 *)malloc(l);
	if (p == 0) {
		DBGPRINT;
		wfp->len = 0;
		return;
	}
	fseek(wfp->res,wfp->Src,SEEK_SET);
	fread(p,1,wfp->len,wfp->res);
	wfp->len = HAL_EVELoopMemWr(RAM_CMD, wfp->Des, CMDBUF_SIZE, p, l);
	free(p);
#elif defined(STM32F4)
	/* make sure the data length into EVE is 4bytes aligned */
	FTU32 l = BYTES4ALIGN(wfp->len);

	wfp->len = HAL_EVELoopMemWr(RAM_CMD, wfp->Des, CMDBUF_SIZE, (FTU8 *)(wfp->res+wfp->Src), l);
#else/* Arduino and FT9XXEV */
	FTU32 i,j,l = FT800_BLOCK_SIZE,x = BYTES4ALIGN(wfp->len);
	FTU8 * p = 0;
	p = file_buff;

#if !defined(FT9XXEV)
	/* only accept increasly read
	 * make sure not over read the previous reading */
	if (wfp->res.offset > wfp->Src) {
		DBGPRINT;
		/* for EVE CMD write purpose */
		wfp->len = wfp->Des;
		return;
	}
#endif

	for (i = 0; i < x; ) {
#if defined(FT9XXEV)
		f_lseek(wfp->res,wfp->Src+i);
		/* '&j' is useless, just re-use j to fill up the f_read para */
		f_read(wfp->res, (FTVOID *)p,l,&j);
#else
		wfp->res.readsector(p);
#endif
		l = ((x - i) > FT800_BLOCK_SIZE)?FT800_BLOCK_SIZE:(x - i);
		wfp->Des = HAL_EVELoopMemWr(RAM_CMD, wfp->Des, CMDBUF_SIZE, p, l);
		i += l;
	}
	/* for EVE CMD write purpose */
	wfp->len = wfp->Des;
#endif
}

FTU32 appResOpen (FTU8 *path)
{
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
	wrFuncPara *pwfp = 0;
	FILE *pFile;

	if(NULL == path)
	{  
		DBGPRINT;
		return 0;
	}
	
	pFile = fopen((FTC8 *)path,"rb");
	if(NULL == pFile)
	{  
		DBGPRINT;
		return 0;
	}

	pwfp = &fPara;
	if (pwfp) {
		pwfp->res = pFile;
	}

	return (FTU32)pwfp;
#elif defined(STM32F4)
	return stm32f4fileopen(path, (FTU32)&fPara);
#elif defined(FT9XXEV)
	wrFuncPara *pwfp = 0;
	if (FR_OK != f_open(&FT9xxFile, (const TCHAR*)path, FA_READ)) {
		DBGPRINT;
		return 0;
	}
	pwfp = &fPara;
	if (pwfp) {
		pwfp->res = &FT9xxFile;
	}

	return (FTU32)pwfp;
#else/* Arduino */
	wrFuncPara *pwfp = 0;
	Reader sdC;
	
	if (0 == sdC.openfile((FT8 *)path)) {
		DBGPRINT;
		return 0;
	}

	pwfp = &fPara;
	if (pwfp) {
		pwfp->res = sdC;
	}

	return (FTU32)pwfp;
#endif
}
FTU32 appResSize (FTU32 resHDL)
{
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
	wrFuncPara *para = (wrFuncPara *)resHDL;
	FTU32 size;
	
	fseek(para->res,0,SEEK_END);
	size = ftell(para->res);
	fseek(para->res,0,SEEK_SET);

	return BYTES4ALIGN(size);
#elif defined(STM32F4)
	return BYTES4ALIGN(stm32f4filesize(resHDL));
#elif defined(FT9XXEV)
	wrFuncPara *para = (wrFuncPara *)resHDL;
	return BYTES4ALIGN(f_size(para->res));
#else/* Arduino */
	wrFuncPara *para = (wrFuncPara *)resHDL;
	return BYTES4ALIGN(para->res.size);
#endif
}
FTU32 appResToDes (FTU32 resHDL, FTU32 Des, FTU32 Src, FTU32 len, AppFunc writeFunc)
{
	wrFuncPara *para = (wrFuncPara *)resHDL;

	if (writeFunc) {
		para->Src = Src;
		para->Des = Des;
		para->len = len;
		writeFunc(resHDL);
		/* writeFunc may change the len so use para after writeFunc executed */
		return para->len;
	}

	return 0;
}
FTVOID appResClose (FTU32 resHDL)
{
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
	wrFuncPara *para = (wrFuncPara *)resHDL;
	fclose(para->res);
#elif defined(STM32F4)

#elif defined(FT9XXEV)
	wrFuncPara *para = (wrFuncPara *)resHDL;
	f_close(para->res);
#else/* Arduino */

#endif
}

/* the chkExceed flag provid the flexibility while using puzzle overlap function */
FTU32 appFileToRamG (FTC8 *path, FTU32 inAddr, FTU8 chkExceed, FTU8 *outAddr, FTU32 outLen)
{
	FTU32 Len, resHDL;

	resHDL = appResOpen((FTU8 *)path);
	if (resHDL == 0) {
		DBGPRINT;
		return 0;
	}

	Len = appResSize(resHDL);
	if (chkExceed && (FT800_RAMG_SIZE < inAddr + Len)) {
		appResClose(resHDL);
		DBGPRINT;
		return 0;
	}
		
	appResToDes(resHDL,inAddr,0,Len,resWrEve);

	if (outAddr) {
		appResToDes(resHDL,(FTU32)outAddr,0,outLen,resWrBuf);
	}

	appResClose(resHDL);

	return Len;
}

appRet_en appBmpToRamG(FTU32 bmpHdl, FTU32 ramgAddr, bmpHDR_st *pbmpHD, FTU32 nums)
{
	FTU32 i, src;

	if (nums > FT800_BMP_EXT_HANDLE || bmpHdl >= FT800_BMP_EXT_HANDLE) {
		DBGPRINT;
		return APP_ERR_HDL_EXC;
	}

	/* you may use other way to copy BMP file to RAM_G
	 * if use this application, 
	 * better under the condition of co-processor finished previous operation*/
	//HAL_BufToReg(RAM_CMD,0);

	for (i = 0, src = ramgAddr; i < nums; i++) {
		pbmpHD[i].len = appFileToRamG(pbmpHD[i].path,src,1,0,0);
		if (pbmpHD[i].len == 0) {
			DBGPRINT;
			return APP_ERR_LEN;
		}
		src += pbmpHD[i].len;
	}

	for (i = 0, src = ramgAddr; i < nums; i++) {
		HAL_DlpBufIn(BITMAP_HANDLE(i+bmpHdl));
		HAL_DlpBufIn(BITMAP_SOURCE(src));
		if (ARGB2 == pbmpHD[i].format || RGB332 == pbmpHD[i].format || PALETTED == pbmpHD[i].format || L8 == pbmpHD[i].format) {
			HAL_DlpBufIn(BITMAP_LAYOUT(pbmpHD[i].format,pbmpHD[i].wide,pbmpHD[i].high));
		} else {
			HAL_DlpBufIn(BITMAP_LAYOUT(pbmpHD[i].format,pbmpHD[i].wide*2,pbmpHD[i].high));
		}
		/* don't know the different between NEAREST and BILINEAR, here just use NEAREST */
		HAL_DlpBufIn(BITMAP_SIZE(NEAREST,BORDER,BORDER,pbmpHD[i].wide,pbmpHD[i].high));
		src += pbmpHD[i].len;
	}

	HAL_DlpBufIn(DISPLAY());
	HAL_BufToReg(RAM_DL,0);

	return APP_OK;
}

FTVOID appBlockWrite (FTU32 des, FTU8 *psrc, FTU32 len, FTU32 block)
{
	FTU32 i = 0,l = (len >= block)?block:len;
	while (l) {
		HAL_Write8Src(des+i,psrc+i,l);
		i += l;
		l = len - i;
		l = (l >= block)?block:l;
	}
}


