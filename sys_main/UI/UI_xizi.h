/* 
    Sample to play dxt1 function for XIZI
    Author: 
	Email : 
	Date  : 2014/Jan
*/

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
#define PATH_SCN1_C0 ROOT_PATH"xizi\\1-background_c0.raw"
#define PATH_SCN1_C1 ROOT_PATH"xizi\\1-background_c1.raw"
#define PATH_SCN1_B0 ROOT_PATH"xizi\\1-background_b0.raw"
#define PATH_SCN1_B1 ROOT_PATH"xizi\\1-background_b1.raw"

#define PATH_NUMBERS ROOT_PATH"xizi\\2-numbers-n-arrows-pal.bin"
#define PATH_SHADOW ROOT_PATH"xizi\\3-shadow-l8.bin"
#define PATH_BRANCHD ROOT_PATH"xizi\\4-down-branch-l4.bin"
#define PATH_BRANCHU ROOT_PATH"xizi\\5-up-branch-l4.bin"
#define PATH_FULL ROOT_PATH"xizi\\6-full-l8.bin"
#define PATH_PARK ROOT_PATH"xizi\\7-park-l8.bin"
#define PATH_LEAF ROOT_PATH"xizi\\8-leaf-pal.bin"
#define PATH_PALETTE ROOT_PATH"xizi\\9-palette.bin"
#else
#define PATH_SCN1_C0 ROOT_PATH"bk0_c0.raw"
#define PATH_SCN1_C1 ROOT_PATH"bk0_c1.raw"
#define PATH_SCN1_B0 ROOT_PATH"bk0_b0.raw"
#define PATH_SCN1_B1 ROOT_PATH"bk0_b1.raw"

#define PATH_SCN2_C0 ROOT_PATH"bk1_c0.raw"
#define PATH_SCN2_C1 ROOT_PATH"bk1_c1.raw"
#define PATH_SCN2_B0 ROOT_PATH"bk1_b0.raw"
#define PATH_SCN2_B1 ROOT_PATH"bk1_b1.raw"
#endif

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU)
#define DXT1_PATH_LEN   (30+5)
#define DXT1_PATH_INDEX_RUN (17+5)
#else
#define DXT1_PATH_LEN   (8+1+3+1)
#define DXT1_PATH_INDEX_RUN (2)
#endif

#define DXT1_BLOCK_NUMS (4)
#define DXT1_START_ADDR RAM_G
#define DXT1_START_HDL  (0)
#define DXT1_HDL_NUMS   (2)
#define DXT1_TLT_FONT   (24)

#define MOVING_WINDOWS 272

typedef struct dxt1_path_ {
	FTU8 path_c0[DXT1_PATH_LEN];
	FTU8 path_c1[DXT1_PATH_LEN];
	FTU8 path_b0[DXT1_PATH_LEN];
	FTU8 path_b1[DXT1_PATH_LEN];
} dxt1_path_st;

dxt1_path_st ScnPath = {
	
	PATH_SCN1_C0,
	PATH_SCN1_C1,
	PATH_SCN1_B0,
	PATH_SCN1_B1
	
};

typedef struct moving_ {
	dxt1_path_st *path;
	FTU32 addr;
	FTU32 hdl;
	FT16 X;
	FT16 Y;
	FTU16 W;
	FTU16 H;
}moving_st;
FTVOID BlockWrite (FTU32 des, FTU8 *psrc, FTU32 len, FTU32 block)
{
	FTU32 i = 0,l = (len >= block)?block:len;
	while (l) {
		HAL_Write8Src(des+i,psrc+i,l);
		i += l;
		l = len - i;
		l = (l >= block)?block:l;
	}
}
FTU32 file_add_gram(FTC8 *path, FTU32 inAddr, FTU32 sadd, FTU32 len)
{
	FILE *fp;
	unsigned char *buf;

	fp = fopen(path, "rb");
	fseek(fp, sadd, SEEK_SET);
	buf = (unsigned char *)malloc(len);
	fread(buf, 1, len, fp);
	fclose(fp);
	BlockWrite(inAddr, buf, len, MCU_BLOCK_SIZE);
	free(buf);
	return len;
}
FTINDEF FTU32 dxt1File (dxt1_path_st *path, FTU32 addrRAM)
{
	FTU32 len, addr = addrRAM;

	len = appFileToRamG((FTC8 *)path->path_c0,addr,0,0,0);
	if (len == 0) {
		DBGPRINT;
		return len;
	}
	addr += len;
	len = appFileToRamG((FTC8 *)path->path_c1,addr,0,0,0);
	if (len == 0) {
		DBGPRINT;
		return len;
	}
	addr += len;
	len = appFileToRamG((FTC8 *)path->path_b0,addr,0,0,0);
	if (len == 0) {
		DBGPRINT;
		return len;
	}
	addr += len;
	len = appFileToRamG((FTC8 *)path->path_b1,addr,0,0,0);
	if (len == 0) {
		DBGPRINT;
		return len;
	}
	addr += len;

	return (addr - addrRAM);
}

FTINDEF FTVOID dxt1BitmapInfo (FTU8 startHdl, FTU32 startAddr, FTU16 W, FTU16 H)
{
	HAL_DlpBufIn(BITMAP_HANDLE(startHdl+1));
	HAL_DlpBufIn(BITMAP_SOURCE(startAddr));
	HAL_DlpBufIn(BITMAP_LAYOUT(RGB565, W/DXT1_BLOCK_NUMS*2, H/DXT1_BLOCK_NUMS));
	HAL_DlpBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, W, H));

	HAL_DlpBufIn(BITMAP_HANDLE(startHdl));
	HAL_DlpBufIn(BITMAP_SOURCE(startAddr + 2*(W/DXT1_BLOCK_NUMS*2*H/DXT1_BLOCK_NUMS)));
	HAL_DlpBufIn(BITMAP_LAYOUT(L1, W/DXT1_BLOCK_NUMS/2, H));
	HAL_DlpBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, W, H));
}

FTINDEF FTVOID dxt1FormatInfo (FTU8 startHdl, FT16 X, FT16 Y)
{
	HAL_CmdBufIn(BLEND_FUNC(ONE,ZERO));
	HAL_CmdBufIn(COLOR_A(0x55));
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl));
	HAL_CmdBufIn(CELL(0));
	HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(BLEND_FUNC(ONE,ONE));
	HAL_CmdBufIn(COLOR_A(0xAA));
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl));
	HAL_CmdBufIn(CELL(1));
	HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(COLOR_MASK(1,1,1,0));

	CoCmd_LOADIDENTITY;
	CoCmd_SCALE(4*FT800_TRANSFORM_MAX, 4*FT800_TRANSFORM_MAX);
	CoCmd_SETMATRIX;

	HAL_CmdBufIn(BLEND_FUNC(DST_ALPHA,ZERO));
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl+1));
	HAL_CmdBufIn(CELL(1));
	HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(BLEND_FUNC(ONE_MINUS_DST_ALPHA,ONE));
	HAL_CmdBufIn(BITMAP_HANDLE(startHdl+1));
	HAL_CmdBufIn(CELL(0));
	HAL_CmdBufIn(VERTEX2F(X*FT800_PIXEL_UNIT,Y*FT800_PIXEL_UNIT));

	HAL_CmdBufIn(END());
}

FTINDEF FTVOID valueChg (moving_st *pm)
{
	FTU8 i = pm->path->path_c0[DXT1_PATH_INDEX_RUN] - '0';

	i += 2;
	i %= 4;
	i += '0';

	pm->Y = -272;

	pm->path->path_c0[DXT1_PATH_INDEX_RUN] = i;
	pm->path->path_c1[DXT1_PATH_INDEX_RUN] = i;
	pm->path->path_b0[DXT1_PATH_INDEX_RUN] = i;
	pm->path->path_b1[DXT1_PATH_INDEX_RUN] = i;
}

FTINDEF FTVOID DispPic (moving_st *pm1)
{

	HAL_CmdBufIn(SAVE_CONTEXT());
	dxt1BitmapInfo(pm1->hdl, pm1->addr, pm1->W, pm1->H);

	HAL_CmdBufIn(BEGIN(BITMAPS));

	dxt1FormatInfo(pm1->hdl,pm1->X,pm1->Y);
	HAL_CmdBufIn(RESTORE_CONTEXT());

	HAL_CmdBufIn(END());

}

FTU32 file_add_gram(FTC8 *path, FTU32 inAddr, FTU32 sadd, FTU32 len);
FTVOID playdxt1 (FTU32 para)
{
	FTU32 len, windows = 0;
	moving_st m1 = {&ScnPath,DXT1_START_ADDR,DXT1_START_HDL,0,0,480,272};
	FTU32 count = 0;
	FTU32 number0_add;
	FTU32 number1_add;
	FTU32 i;
	FTU32 x, y;

FTU16 leaves[30][2] = 
{
	{424, 7},
	{368, 22},
	{409, 37},
	{442, 40},
	{485, 30},
	{415, 47},
	{402, 51},
	{412, 56},
	{452, 56},
	{452, 56},
	{439, 73},
	{435, 83},
	{435, 83},
	{469, 84},
	{407, 121},
	{443, 136},
	{435, 139},
	{400, 144},
	{411, 158},
	{416, 160},
	{452, 171},
	{452, 171},
	{457, 184},
	{457, 184},
	{449, 244},
	{442, 258},
	{480, 246},
	{454, 254},
	{454, 254},
	{452, 262}
};
	/* just for debug */
	appGP.appIndex = 1;
	appGP.appPara = 0;

	/* pre-stored the first two screen information */
	len = dxt1File(&ScnPath,m1.addr);
	if (len == 0) {
		DBGPRINT;
		return;
	}

	//HAL_CmdBufIn(CMD_DLSTART);


	dxt1BitmapInfo(m1.hdl, m1.addr, m1.W, m1.H);

	HAL_DlpBufIn(BITMAP_HANDLE(2));
	HAL_DlpBufIn(BITMAP_SOURCE(len));
	HAL_DlpBufIn(BITMAP_LAYOUT(PALETTED, 173, 128));
	HAL_DlpBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, 173, 128));
	number0_add = len;
	len += file_add_gram((FTC8 *)PATH_NUMBERS,len + RAM_G,0,44288);	

	HAL_DlpBufIn(BITMAP_HANDLE(3));
	HAL_DlpBufIn(BITMAP_SOURCE(len));
	HAL_DlpBufIn(BITMAP_LAYOUT(PALETTED, 173, 128));
	HAL_DlpBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, 173, 128));
	number1_add = len;
	len += file_add_gram((FTC8 *)PATH_NUMBERS,len + RAM_G,0,44288);	

	HAL_DlpBufIn(BITMAP_HANDLE(4));
	HAL_DlpBufIn(BITMAP_SOURCE(len));
	HAL_DlpBufIn(BITMAP_LAYOUT(PALETTED, 173, 128));
	HAL_DlpBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, 173, 128));

	len += file_add_gram((FTC8 *)PATH_NUMBERS,len + RAM_G,243584,44288);

	HAL_DlpBufIn(BITMAP_HANDLE(5));
	HAL_DlpBufIn(BITMAP_SOURCE(len));
	HAL_DlpBufIn(BITMAP_LAYOUT(L8, 37, 140));
	HAL_DlpBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, 37, 140));

	len += appFileToRamG((FTC8 *)PATH_SHADOW ,len + RAM_G,0,0,0);

	HAL_DlpBufIn(BITMAP_HANDLE(6));
	HAL_DlpBufIn(BITMAP_SOURCE(len));
	HAL_DlpBufIn(BITMAP_LAYOUT(L4, 41, 206));
	HAL_DlpBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, 82, 206));

	len += appFileToRamG((FTC8 *)PATH_BRANCHD,len + RAM_G,0,0,0);

	HAL_DlpBufIn(BITMAP_HANDLE(7));
	HAL_DlpBufIn(BITMAP_SOURCE(len));
	HAL_DlpBufIn(BITMAP_LAYOUT(L4, 57, 272));
	HAL_DlpBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, 113, 272));

	len += appFileToRamG((FTC8 *)PATH_BRANCHU,len + RAM_G,0,0,0);

	HAL_DlpBufIn(BITMAP_HANDLE(8));
	HAL_DlpBufIn(BITMAP_SOURCE(len));
	HAL_DlpBufIn(BITMAP_LAYOUT(L8, 31, 44));
	HAL_DlpBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, 31, 44));

	len += appFileToRamG((FTC8 *)PATH_FULL,len + RAM_G,0,0,0);
	len += appFileToRamG((FTC8 *)PATH_PARK,len + RAM_G,0,0,0);

	HAL_DlpBufIn(BITMAP_HANDLE(9));
	HAL_DlpBufIn(BITMAP_SOURCE(len));
	HAL_DlpBufIn(BITMAP_LAYOUT(PALETTED, 49, 44));
	HAL_DlpBufIn(BITMAP_SIZE(NEAREST, BORDER, BORDER, 49, 44));

	len += appFileToRamG((FTC8 *)PATH_LEAF,len + RAM_G,0,0,0);
	appFileToRamG((FTC8 *)PATH_PALETTE, RAM_PAL,0,0,0);

	HAL_DlpBufIn(DISPLAY());
	HAL_DlpBufIn(CMD_SWAP);
	HAL_BufToReg(RAM_DL,0);
	do {
		count ++;

		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(CLEAR_COLOR_RGB(0xFF,0xFF,0xFF));
		HAL_CmdBufIn(CLEAR(1,1,1));
		//DispPic(&m1);
		//background
		HAL_CmdBufIn(SAVE_CONTEXT());
		HAL_CmdBufIn(BEGIN(BITMAPS));

		HAL_CmdBufIn(BLEND_FUNC(ONE,ZERO));
		HAL_CmdBufIn(COLOR_A(0x55));
		HAL_CmdBufIn(BITMAP_HANDLE(m1.hdl));
		HAL_CmdBufIn(CELL(0));
		HAL_CmdBufIn(VERTEX2F(0, 0));

		HAL_CmdBufIn(BLEND_FUNC(ONE,ONE));
		HAL_CmdBufIn(COLOR_A(0xAA));
		HAL_CmdBufIn(BITMAP_HANDLE(m1.hdl));
		HAL_CmdBufIn(CELL(1));
		HAL_CmdBufIn(VERTEX2F(0, 0));

		HAL_CmdBufIn(COLOR_MASK(1,1,1,0));

		CoCmd_LOADIDENTITY;
		CoCmd_SCALE(4*FT800_TRANSFORM_MAX, 4*FT800_TRANSFORM_MAX);
		CoCmd_SETMATRIX;

		HAL_CmdBufIn(BLEND_FUNC(DST_ALPHA,ZERO));
		HAL_CmdBufIn(BITMAP_HANDLE(m1.hdl + 1));
		HAL_CmdBufIn(CELL(1));
		HAL_CmdBufIn(VERTEX2F(0, 0));

		HAL_CmdBufIn(BLEND_FUNC(ONE_MINUS_DST_ALPHA,ONE));
		HAL_CmdBufIn(BITMAP_HANDLE(m1.hdl + 1));
		HAL_CmdBufIn(CELL(0));
		HAL_CmdBufIn(VERTEX2F(0, 0));

		HAL_CmdBufIn(RESTORE_CONTEXT());
		HAL_CmdBufIn(END());

		HAL_CmdBufIn(BEGIN(BITMAPS));
		HAL_CmdBufIn(COLOR_RGB(135, 135, 135));
		HAL_CmdBufIn(BITMAP_HANDLE(6));
		HAL_CmdBufIn(VERTEX2F(0, 66 * 16));
		HAL_CmdBufIn(END());


		HAL_CmdBufIn(BEGIN(BITMAPS));
		HAL_CmdBufIn(COLOR_RGB(172, 172, 172));
		HAL_CmdBufIn(BITMAP_HANDLE(7));
		HAL_CmdBufIn(VERTEX2F(367 * 16, 0));
		HAL_CmdBufIn(END());

		HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
		//end background
		for (i=0; i<30; i++)
		{
			x = leaves[i][0] * 16;
			y = leaves[i][1] * 16;

			HAL_CmdBufIn(COLOR_RGB(255, 255, 255));
			HAL_CmdBufIn(BEGIN(BITMAPS));
			HAL_CmdBufIn(BITMAP_HANDLE(9));
			HAL_CmdBufIn(SAVE_CONTEXT());
			HAL_CmdBufIn(VERTEX2F(x, y));
			HAL_CmdBufIn(RESTORE_CONTEXT());
			HAL_CmdBufIn(END());
		}

		//arrow
		HAL_CmdBufIn(BEGIN(BITMAPS));
		HAL_CmdBufIn(BITMAP_HANDLE(4));
		HAL_CmdBufIn(CELL(count & 1));
		HAL_CmdBufIn(VERTEX2F(270 * 16, 136 * 16));
		HAL_CmdBufIn(END());
		HAL_CmdBufIn(CELL(0));
		//end arrow

		//floor
		file_add_gram((FTC8 *)PATH_NUMBERS,RAM_G + number0_add,22144 * ((count % 40) / 10),22144);	
		file_add_gram((FTC8 *)PATH_NUMBERS,RAM_G + number1_add,22144 * ((count % 40) % 10),22144);	

		HAL_CmdBufIn(BEGIN(BITMAPS));
		HAL_CmdBufIn(BITMAP_HANDLE(2));
		HAL_CmdBufIn(CELL(0));
		HAL_CmdBufIn(VERTEX2F(85*16, 136*16));
		HAL_CmdBufIn(END());

		HAL_CmdBufIn(CELL(0));
		HAL_CmdBufIn(COLOR_RGB(83, 43, 43));
		HAL_CmdBufIn(BEGIN(BITMAPS));
		HAL_CmdBufIn(BITMAP_HANDLE(5));
		HAL_CmdBufIn(VERTEX2F(70*16, 136*16));
		HAL_CmdBufIn(VERTEX2F(70*16, 9*16));
		HAL_CmdBufIn(END());
		HAL_CmdBufIn(COLOR_RGB(255, 255, 255));


		HAL_CmdBufIn(BEGIN(BITMAPS));
		HAL_CmdBufIn(BITMAP_HANDLE(3));
		HAL_CmdBufIn(CELL(0));
		HAL_CmdBufIn(VERTEX2F(85*16, 9*16));
		HAL_CmdBufIn(END());



		//end floor

		//status 
		HAL_CmdBufIn(BEGIN(BITMAPS));
		HAL_CmdBufIn(COLOR_A(0xFF));
		HAL_CmdBufIn(COLOR_RGB(60, 46, 39));
		HAL_CmdBufIn(BITMAP_HANDLE(8));
		HAL_CmdBufIn(CELL(count & 1));
		HAL_CmdBufIn(VERTEX2F(258 * 16, 144 * 16));
		HAL_CmdBufIn(END());
		//end status


		HAL_CmdBufIn(CELL(0));
		
		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
		FTDELAY(500);
	} while (1);
}

AppFunc APPS_UI[] = {
	playdxt1,
	/* Leave this NULL at the buttom of this array */
	NULL
};

