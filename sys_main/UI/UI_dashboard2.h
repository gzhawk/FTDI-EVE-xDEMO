/* 
    Sample code to show how to use rotatearound for the dashboard
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2018/Dec
*/

#define PATH_BKG      ROOT_PATH"dashboard2\\bkg.raw"
#define PATH_NDL      ROOT_PATH"dashboard2\\ndl.raw"

typedef enum hdl_ {
    HDL_NEEDLE  = 0,
    HDL_BKGND,
    RAW_NUM = HDL_BKGND + 1,
} hdl_enum;

#define NDL_START 238
#define NDL_END   124

#define NDL_W     64
#define NDL_H     232

#define BKG_W     480
#define BKG_H     396

#define BKG_X     (EVE_LCD_WIDTH/2 - BKG_W/2)
#define BKG_Y      0

#define NDL_X     (EVE_LCD_WIDTH/2 - NDL_W/2 - NDL_H)
#define NDL_Y     (EVE_LCD_HIGH/2 - NDL_H*4/5)

ImgInfo_st img_info[] = {
    {PATH_NDL,0,0,0},
    {PATH_BKG,0,0,0},
};
bmpHDR_st bmp_header[RAW_NUM] = {
    {COMPRESSED_RGBA_ASTC_8x8_KHR,NDL_W,NDL_H,(FTU32)&img_info[HDL_NEEDLE]},
    {COMPRESSED_RGBA_ASTC_4x4_KHR,BKG_W,BKG_H,(FTU32)&img_info[HDL_BKGND]},
};

FTVOID dashboard2 (FTU32 para)
{
    static FTU8 flag = 0;
    static FTU32 i=NDL_START;

    /* only load the file once */
    if (flag == 0) {
        /* load bitmap resources data into EVE */
        if(APP_OK != appBmpToRamG(HDL_NEEDLE, RAM_G, bmp_header, RAW_NUM)){
            DBGPRINT;
            return;
        }
        /* when doing the none-square image, 
           bilinear always give a better effect,
           while nearest often show jagger
           so reset the bitmap_size info 
           
           the reason why use HIGH 
           assume HIGH > WIDE, if your image is WIDE > HIGH,
           then use WIDE instead of HIGH
           the reason why multiple 2 is
           when image rotate 90", and 270", the space is HIGH*2
           give the rotation the larger appearance space 
           you may also set it ZERO for the largest */
        appBilinearModify(HDL_NEEDLE, bmp_header[HDL_NEEDLE].high*2);
        flag = 1;
    }

    HAL_CmdBufIn(CMD_DLSTART);
    HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
    HAL_CmdBufIn(CLEAR(1,1,1));

    HAL_CmdBufIn(BEGIN(BITMAPS));

    /* display background */
    HAL_CmdBufIn(BITMAP_HANDLE(img_info[HDL_BKGND].handle));
    HAL_CmdBufIn(VERTEX2F(BKG_X*EVE_PIXEL_UNIT,BKG_Y*EVE_PIXEL_UNIT));
    
    /* display rotated needle */
    HAL_CmdBufIn(BITMAP_HANDLE(img_info[HDL_NEEDLE].handle));
    HAL_CmdBufIn(SAVE_CONTEXT());
    CoCmd_LOADIDENTITY;
    /* have to do the offset to make it fully display */
    CoCmd_TRANSLATE((bmp_header[HDL_NEEDLE].high)*EVE_TRANSFORM_MAX,0);
    CoCmd_ROTATEAROUND(bmp_header[HDL_NEEDLE].wide/2,bmp_header[HDL_NEEDLE].high*4/5,i*EVE_TRANSFORM_MAX/360,EVE_TRANSFORM_MAX);
    CoCmd_SETMATRIX;
    
    HAL_CmdBufIn(VERTEX2F(NDL_X*EVE_PIXEL_UNIT,NDL_Y*EVE_PIXEL_UNIT));
    HAL_CmdBufIn(RESTORE_CONTEXT());

    HAL_CmdBufIn(END());
    HAL_CmdBufIn(DISPLAY());
    HAL_CmdBufIn(CMD_SWAP);

    HAL_BufToReg(RAM_CMD,0);

    /* needle auto running */
    if (flag == 1) {
        i++;
		if (i == 360) {
			i = 0;
		} else if (i == NDL_END) {
            flag = 2;
        }
    }
    
    if (flag == 2) {
        i--;
		if (i == 0) {
            i = 359;
		} else if (i == NDL_START) {
            flag = 1;
        }
    }
    FTDELAY(10);
    appGP.appIndex = 0;
}

AppFunc APPS_UI[] = {
    dashboard2
};

