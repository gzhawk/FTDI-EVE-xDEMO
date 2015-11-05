/* 
    Sample to play the audio function 
    Author: Hawk
    Email : hawk.gao@ftdichip.com	
	Date  : 2013/May
*/
#ifdef DEMO_AUDIO
#define FT800_AUD_BLOCK_SIZE  (5*1024UL)
/* WINDOW SIZE has to base on BLOCK SIZE */
#define FT800_AUD_WINDOW_SIZE (3*FT800_AUD_BLOCK_SIZE)
#define FT800_AUD_MEM_ADDR    RAM_G

typedef struct app_para_st {
	FTU32 appIndex;
	FTU32 appPara;
}app_para_t;

app_para_t appGP = {0};
#define WAV_HEAD_LEN  44
#define WAV_HEAD_DLEN 40
#define WAV_HEAD_PCM  20
#define AUD_END_RETRY 3

FTU8 parseWAV (audio_hd_t *paudio_hd)
{
	FTU8 pbuf[WAV_HEAD_LEN];
	FTU32 resHDL;

	resHDL = appResOpen(paudio_hd->path);
	if(0 == resHDL){  
		DBGPRINT;
		paudio_hd->dsize = 0;
		appResClose(resHDL);
		return 0;
	}

	appResToDes(resHDL,(FTU32)pbuf,0,WAV_HEAD_LEN,resWrBuf);
	
	if ('R' == pbuf[0] &&
		'I' == pbuf[1] &&
		'F' == pbuf[2] &&
		'F' == pbuf[3]) {
		/* should be WAV file, but need to be PCM encoded */
		if (1 == pbuf[WAV_HEAD_PCM]) {
			paudio_hd->dsize = *(FTU32 *)&pbuf[WAV_HEAD_DLEN];
			paudio_hd->type = LINEAR_SAMPLES;
			paudio_hd->freq = *(FTU16 *)&pbuf[WAV_HEAD_LEN];
			appResClose(resHDL);
			return 1;
		} else {
			DBGPRINT;
			paudio_hd->dsize = 0;
			appResClose(resHDL);
			return 0;
		}
	} else {
		/* should be RAW file */
		paudio_hd->dsize = appResSize(resHDL);
		/* type and freq should be filled outside */
		appResClose(resHDL);
		return 0;
	}
}
/* 
 * change the FT800_AUD_MEM_ADDR and FT800_AUD_WINDOW_SIZE
 * to limit the audio playing space be used in RAM_G
 * when you have your own assignment in RAM_G
 * but pay attentation on the MAX RAM_G is FT800_RAMG_SIZE
 */
FTU32 audPlay(audio_hd_t *paudio_hd, AppFunc disp_func)
{
	FTU8 loop;
	FTU32 rdaddr = 0, wraddr = FT800_AUD_MEM_ADDR;
	FTU32 fBlock = 0, audioLen;
	FTU32 resHDL;
	
	parseWAV(paudio_hd);
	if(0 == paudio_hd->dsize){  
		DBGPRINT;
		return 0;
	}

	resHDL = appResOpen(paudio_hd->path);
	
	loop = (paudio_hd->dsize > FT800_AUD_WINDOW_SIZE)?1:0;
	audioLen = (paudio_hd->dsize < FT800_AUD_WINDOW_SIZE)?paudio_hd->dsize:FT800_AUD_WINDOW_SIZE;

	/* stop the previous playing just for save */		
	HAL_Write32(REG_PLAYBACK_LENGTH,0);
	HAL_Write8(REG_PLAYBACK_PLAY,0);
	/* setup the audio related reg */
	HAL_Write32(REG_PLAYBACK_START,FT800_AUD_MEM_ADDR);
	HAL_Write32(REG_PLAYBACK_LENGTH,audioLen);
	HAL_Write16(REG_PLAYBACK_FREQ,paudio_hd->freq);
	HAL_Write8(REG_PLAYBACK_FORMAT,paudio_hd->type);
	HAL_Write8(REG_PLAYBACK_LOOP, loop);
	HAL_Write8(REG_VOL_PB,paudio_hd->vol);	

	HAL_Write8(REG_GPIO_DIR,0xff);		
	HAL_Write8(REG_GPIO,0xff);
	HAL_Write8(REG_VOL_SOUND,paudio_hd->vol);
	while ( paudio_hd->dsize ) {
		fBlock = (paudio_hd->dsize < FT800_AUD_BLOCK_SIZE) ? paudio_hd->dsize : FT800_AUD_BLOCK_SIZE;

		appResToDes(resHDL,wraddr,paudio_hd->index,fBlock,resWrEve);

		if (0 == paudio_hd->index) {
			/* start to play the sound only once */
			HAL_Write8(REG_PLAYBACK_PLAY,1);
		}
		
		wraddr += fBlock;
		paudio_hd->index += fBlock;
		paudio_hd->dsize -= fBlock;
		rdaddr = HAL_Read16(REG_PLAYBACK_READPTR);

		disp_func((FTU32)paudio_hd);

		if (!loop) {
			/* single track, go out waiting */
			break;
		}

		/*
        +: new going to play
        -: played
        #: old going to play
        */
		if (rdaddr > wraddr) {
            /*+++W---R###*/
			while (rdaddr > wraddr) {
				/* wait till '#' finish */
				rdaddr = HAL_Read16(REG_PLAYBACK_READPTR);
			}
			if (!paudio_hd->dsize) {
				/* last track */
				HAL_Write8(REG_PLAYBACK_LOOP, 0);
				HAL_Write32(REG_PLAYBACK_LENGTH,wraddr - FT800_AUD_MEM_ADDR);
				break;
			}
		} else {
            /*---R+++W---*/
			if (paudio_hd->dsize) {
				/* 
				 * filling the new data after half of saved data be used
				 * so keep waiting till half space left
				 */
				while ((wraddr - rdaddr) > fBlock/2) {
					rdaddr = HAL_Read16(REG_PLAYBACK_READPTR);
				}
			} else {
				/* last track */
				HAL_Write8(REG_PLAYBACK_LOOP, 0);
				HAL_Write32(REG_PLAYBACK_LENGTH,wraddr - rdaddr);
				break;
			}
		}

		/* limit the audio playing space in FT800_AUD_WINDOW_SIZE */
		if ((wraddr - FT800_AUD_MEM_ADDR) >= FT800_AUD_WINDOW_SIZE) {
			wraddr = FT800_AUD_MEM_ADDR;
		}
	
		disp_func((FTU32)paudio_hd);

		if (TOUCHED) {
			while (TOUCHED) {
				HAL_Write32(REG_PLAYBACK_LENGTH,0);
				HAL_Write8(REG_PLAYBACK_PLAY,0);
			}

			appResClose(resHDL);
			return 1;
		}
	}

	/* reuse loop to see if the readptr never changed */
	loop = 0;
	while ((rdaddr < wraddr) && (rdaddr != FT800_AUD_MEM_ADDR)) {
		if (rdaddr == HAL_Read16(REG_PLAYBACK_READPTR)) {
			if (loop++ >= AUD_END_RETRY) break;
		}
		rdaddr = HAL_Read16(REG_PLAYBACK_READPTR);
	}

	HAL_Write32(REG_PLAYBACK_LENGTH,0);
	HAL_Write8(REG_PLAYBACK_PLAY,0);
	
	appResClose(resHDL);
	return 1;
}

FTVOID screenShow (FTU32 para)
{
	audio_hd_t *paud = (audio_hd_t *)para;

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255,255,255));
	HAL_CmdBufIn(CLEAR(1,1,1));
	HAL_CmdBufIn(COLOR_RGB(0,0xFF,0));
	if (paud->dsize == 0) {
		CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,23,OPT_CENTER,paud->path);
	} else {
		CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2,23,OPT_CENTER,paud->path);
		CoCmd_TEXT(FT800_LCD_WIDTH/2-80,FT800_LCD_HIGH/2+30,23,OPT_CENTER,"Length:");
		CoCmd_NUMBER(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/2+30,23,OPT_CENTER,paud->dsize);
	}
	HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_CMD,0);

	if (paud->dsize == 0) {
		FTDELAY(1000);
	}
}

FTVOID playaudio (FTU32 cashPara)
{
	audio_hd_t raw_hd = {
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
		(FTU8 *)ROOT_PATH"aud\\Chopin.raw",
#else
		(FTU8 *)ROOT_PATH"Chopin.raw",
#endif
		0,0,
		LINEAR_SAMPLES,
		8000,
		50	
	};
/* some WAV should be able to play directly
 * but the WAV file I got is not that good
 * so not play it, just leave the code here
 * for any one who may try by themselves
 */
#if 0
	audio_hd_t wav_hd = {
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
		(FTU8 *)ROOT_PATH"aud\\Chopin.wav",
#else
		(FTU8 *)ROOT_PATH"Chopin.wav",
#endif
		0,0,0,0,//program would set parameter later
		AUD_VOL
	};
#endif
	appGP.appIndex = 1;
	appGP.appPara = 0;

	screenShow((FTU32)&raw_hd);
	if (audPlay(&raw_hd, screenShow)) {
		appGP.appIndex = 0;
	}
/*	
	screenShow((FTU32)&wav_hd);
	if (audPlay(&wav_hd, screenShow)) {
		appGP.appIndex = 0;
	}
*/
}

AppFunc Apps[] = {
	playaudio,
	/* Leave this NULL at the buttom of this array */
	NULL
};
#endif

