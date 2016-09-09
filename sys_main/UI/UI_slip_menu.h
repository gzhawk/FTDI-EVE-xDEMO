/* 
    Sample code to show how to make slip menu
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Sep
*/

//FTU8 MENU_LEFT[] = ROOT_PATH"slip_m\\1a.raw";
//FTU8 MENU_LEFT[] = ROOT_PATH"slip_m\\1a.bin";
FTU8 MENU_LEFT[] = ROOT_PATH"slip_m\\1a_inx.bin";
FTU8 MENU_LEFT_[] = ROOT_PATH"slip_m\\1a_lut.bin";
//FTU8 MENU_1[] = ROOT_PATH"slip_m\\2a.raw";
//FTU8 MENU_1[] = ROOT_PATH"slip_m\\2a.bin";
FTU8 MENU_1[] = ROOT_PATH"slip_m\\2a_inx.bin";
FTU8 MENU_1_[] = ROOT_PATH"slip_m\\2a_lut.bin";
//FTU8 MENU_2[] = ROOT_PATH"slip_m\\3a.raw";
//FTU8 MENU_2[] = ROOT_PATH"slip_m\\3a.bin";
FTU8 MENU_2[] = ROOT_PATH"slip_m\\3a_inx.bin";
FTU8 MENU_2_[] = ROOT_PATH"slip_m\\3a_lut.bin";
//FTU8 MENU_3[] = ROOT_PATH"slip_m\\4a.raw";
//FTU8 MENU_3[] = ROOT_PATH"slip_m\\4a.bin";
FTU8 MENU_3[] = ROOT_PATH"slip_m\\4a_inx.bin";
FTU8 MENU_3_[] = ROOT_PATH"slip_m\\4a_lut.bin";
//FTU8 MENU_4[] = ROOT_PATH"slip_m\\5a.raw";
//FTU8 MENU_4[] = ROOT_PATH"slip_m\\5a.bin";
FTU8 MENU_4[] = ROOT_PATH"slip_m\\5a_inx.bin";
FTU8 MENU_4_[] = ROOT_PATH"slip_m\\5a_lut.bin";
//FTU8 MENU_RIGHT[] = ROOT_PATH"slip_m\\6a.raw";
//FTU8 MENU_RIGHT[] = ROOT_PATH"slip_m\\6a.bin";
FTU8 MENU_RIGHT[] = ROOT_PATH"slip_m\\6a_inx.bin";
FTU8 MENU_RIGHT_[] = ROOT_PATH"slip_m\\6a_lut.bin";

FTU8 MENU_SUB[] = ROOT_PATH"slip_m\\0_inx.bin";
FTU8 MENU_SUB_[] = ROOT_PATH"slip_m\\0_lut.bin";

#define MENU_MAIN_NUM  6 //left, 1, 2, 3, 4, right
#define MENU_NUM       '8' //total menu number
#define MENU_HDL_START 0
#define MENU_RAM_START RAM_G
#define MENU_W         200
#define MENU_W_        800
#define MENU_H         480
#define MENU_DENO      100
#define MENU_END_L     (-200)
#define MENU_END_R     (MENU_END_L+(MENU_MAIN_NUM-1)*MENU_W)

#if (defined(MSVC2010EXPRESS) || defined(MSVC2012EMU))
#define UPDATE_PATH_INDEX 17
#else
#define UPDATE_PATH_INDEX 7
#endif

#define SCALE_PRESSED     95
#define SCALE_RELEASED    100

typedef enum SUB_MENU_ {
    E_MENU_M = 0,
    E_MENU_SUB,
    E_MENU_ERR,
}SUB_MENU_E;

typedef struct menu_prop_ {
    bmpHDR_st menuInfo;
    FT16  X;
    FTU8  inRAM;
    FTU8  scale; //100 = 100%, 95 = 95%
} menu_prop_st;

FT16 pos_anchor[MENU_MAIN_NUM] = {
    MENU_END_L,
    (MENU_END_L+MENU_W),
    (MENU_END_L+2*MENU_W),
    (MENU_END_L+3*MENU_W),
    (MENU_END_L+4*MENU_W),
    MENU_END_R
};

menu_prop_st main_menu[MENU_MAIN_NUM+1] = {
    {{(FTC8 *)MENU_LEFT , (FTC8 *)MENU_LEFT_ , 
      (MENU_RAM_START+(MENU_W*MENU_H)),          
      //RGB332, (MENU_W*MENU_H), 1024, MENU_W,  MENU_H},
      PALETTED565, (MENU_W*MENU_H), 1024, MENU_W,  MENU_H},
    MENU_END_L,           0,100},

    {{(FTC8 *)MENU_1    , (FTC8 *)MENU_1_    , 
      (MENU_RAM_START+2*(MENU_W*MENU_H)+1024),   
      //RGB332, (MENU_W*MENU_H), 1024, MENU_W,  MENU_H},
      PALETTED565, (MENU_W*MENU_H), 1024, MENU_W,  MENU_H},
    (MENU_END_L+MENU_W),  0,100},

    {{(FTC8 *)MENU_2    , (FTC8 *)MENU_2_    , 
      (MENU_RAM_START+3*(MENU_W*MENU_H)+2*1024), 
      //RGB332, (MENU_W*MENU_H), 1024, MENU_W,  MENU_H},
      PALETTED565, (MENU_W*MENU_H), 1024, MENU_W,  MENU_H},
    (MENU_END_L+2*MENU_W),0,100},

    {{(FTC8 *)MENU_3    , (FTC8 *)MENU_3_    , 
      (MENU_RAM_START+4*(MENU_W*MENU_H)+3*1024), 
      //RGB332, (MENU_W*MENU_H), 1024, MENU_W,  MENU_H},
      PALETTED565, (MENU_W*MENU_H), 1024, MENU_W,  MENU_H},
    (MENU_END_L+3*MENU_W),0,100},

    {{(FTC8 *)MENU_4    , (FTC8 *)MENU_4_    , 
      (MENU_RAM_START+5*(MENU_W*MENU_H)+4*1024), 
      //RGB332, (MENU_W*MENU_H), 1024, MENU_W,  MENU_H},
      PALETTED565, (MENU_W*MENU_H), 1024, MENU_W,  MENU_H},
    (MENU_END_L+4*MENU_W),0,100},

    {{(FTC8 *)MENU_RIGHT, (FTC8 *)MENU_RIGHT_, 
      (MENU_RAM_START+6*(MENU_W*MENU_H)+5*1024), 
      //RGB332, (MENU_W*MENU_H), 1024, MENU_W,  MENU_H},
      PALETTED565, (MENU_W*MENU_H), 1024, MENU_W,  MENU_H},
    MENU_END_R,           0,100},

    {{(FTC8 *)MENU_SUB,   (FTC8 *)MENU_SUB_,   
      (MENU_RAM_START+6*(MENU_W*MENU_H)+6*1024+(MENU_W_*MENU_H)), 
      PALETTED565, (MENU_W_*MENU_H),1024, MENU_W_, MENU_H},
    0,                    1,100}
};

FTVOID menu_load_test (menu_prop_st *p)
{
    if (p->X > MENU_END_R+MENU_W) {
        p->X -= MENU_MAIN_NUM*MENU_W;
        p->inRAM = 0;
    } else if (p->X < MENU_END_L-MENU_W) {
        p->X += MENU_MAIN_NUM*MENU_W;
        p->inRAM = 0;
    }
}

FTU8 menu_nunber (FT16 X)
{
    FTU8 i;

    for (i = 0; i < MENU_MAIN_NUM; i++) {
        if (main_menu[i].X <= X && (main_menu[i].X + (FT16)main_menu[i].menuInfo.wide) > X) {
            break;
        }
    }

    return i;
}

FTU8 menu_anchor_index (FT16 X)
{
    FTU8 i;
    
    for (i = 0; i < MENU_MAIN_NUM; i++) {
        if (pos_anchor[i] == X) {
            break;
        }
    }

    return i;
}

FTVOID menu_anchor (FTVOID)
{
    FTU8 i, j;
    menu_prop_st *p = &main_menu[0];

    for (i = 0; i < MENU_MAIN_NUM; i++) {
        if ((pos_anchor[i]-MENU_W/2) < p->X && (pos_anchor[i]+MENU_W/2) > p->X) {
            p->X = pos_anchor[i];
            break;   
        }
    }

    for (j = 1, i += 1; j < MENU_MAIN_NUM; j++,i++) {
        if (i >= MENU_MAIN_NUM) {
            i = 0;
        }
        main_menu[j].X = pos_anchor[i];
    }
}

FTVOID menu_new_path (FTU8 i)
{
    FTU8 n;
	FT16 w = (FT16)main_menu[i].menuInfo.wide;

    if (main_menu[i].X <= MENU_END_L+w) {
        if (i >= (MENU_MAIN_NUM-1)) {
            n = main_menu[0].menuInfo.path[UPDATE_PATH_INDEX] - 1;
        } else {
            n = main_menu[i+1].menuInfo.path[UPDATE_PATH_INDEX] - 1;
        }

        if (n < '1') {
            *(FTU8 *)(main_menu[i].menuInfo.path + UPDATE_PATH_INDEX) = MENU_NUM;
            *(FTU8 *)(main_menu[i].menuInfo.path_lut + UPDATE_PATH_INDEX) = MENU_NUM;
        } else {
            *(FTU8 *)(main_menu[i].menuInfo.path + UPDATE_PATH_INDEX) = n;
            *(FTU8 *)(main_menu[i].menuInfo.path_lut + UPDATE_PATH_INDEX) = n;
        }
    } else if (main_menu[i].X >= MENU_END_R-w) {
        if (i <= 0) {
            n = main_menu[MENU_MAIN_NUM-1].menuInfo.path[UPDATE_PATH_INDEX] + 1;
        } else {
            n = main_menu[i-1].menuInfo.path[UPDATE_PATH_INDEX] + 1;
        }

        if (n > MENU_NUM) {
            *(FTU8 *)(main_menu[i].menuInfo.path + UPDATE_PATH_INDEX) = '1';
            *(FTU8 *)(main_menu[i].menuInfo.path_lut + UPDATE_PATH_INDEX) = '1';
        } else {
            *(FTU8 *)(main_menu[i].menuInfo.path + UPDATE_PATH_INDEX) = n;
            *(FTU8 *)(main_menu[i].menuInfo.path_lut + UPDATE_PATH_INDEX) = n;
        }
    }
}

FTU8 menu_load (FTU8 i, FTU8 init)
{
    FTU32 ram_addr = main_menu[i].menuInfo.lut_src - main_menu[i].menuInfo.len;
    
    if(!main_menu[i].inRAM){
        if (APP_OK != appLoadBmp(ram_addr,&(main_menu[i].menuInfo),1) ) {
            return 0;
        }
        main_menu[i].inRAM = 1;
    }

    if (init) {
        appUI_FillBmpDL(MENU_HDL_START+i, ram_addr, &(main_menu[i].menuInfo), 1);
    }
    
    return 1;
}

FTU8 menu_init (FTVOID)
{
	FTU8 i;
    
    for (i = 0; i < MENU_MAIN_NUM+1; i++) {
        menu_load(i, 1);
    }

    return 1;
}

FTU8 menu_detect (FTU32 *pMainMenuIndex)
{
#define RELEASED     (0x8000&nowTouch)
#define DELTA_L      (-3)
#define DELTA_R      (3)
    static FT16 lastTouch = 0, drag = 0;
    FT16 nowTouch = 0xFFFF&(HAL_Read32(REG_TOUCH_SCREEN_XY)>>16),
         delta = 0;
    FTU8 i;

    /* never touch */
    if (RELEASED && !lastTouch) {
        return E_MENU_M;
    }

    /* first touch */
	if (!lastTouch) {
		lastTouch = nowTouch;
        return E_MENU_M;
	}

	if (RELEASED) {
        /* touched, and released */
        menu_anchor();
	} else {
        /* touched, and drag */
		delta = nowTouch - lastTouch;
		lastTouch = nowTouch;
        /* ignore the tiny movement */
        if (delta > DELTA_L && delta < DELTA_R) {
            delta = 0;
        }
        /* 
         once set it to drag
         only release can reset it
         */
        if (!drag && delta) {
            drag = 1;
        }
	}

    for (i = 0; i < MENU_MAIN_NUM; i++) {
		if (RELEASED) {
            main_menu[i].scale = RELEASED?SCALE_RELEASED:SCALE_PRESSED;
		} else {
			main_menu[i].scale = SCALE_PRESSED;
            if (delta) {
                main_menu[i].X += delta;
                menu_load_test(&main_menu[i]);
            }
		}

        if (!main_menu[i].inRAM) {
            menu_new_path(i);
            menu_load(i, 0);
        }
    }

    i = E_MENU_M;

    if (RELEASED) {
        if (!drag) {
            *pMainMenuIndex = menu_nunber(lastTouch);
            if (*pMainMenuIndex == MENU_MAIN_NUM) {
                /* main menu index not found */
                i = E_MENU_M;
            } else {
                i = E_MENU_SUB;
                main_menu[MENU_MAIN_NUM].inRAM = 0;
            }
        }
        lastTouch = 0;
        delta = 0;
        drag = 0;
    }

    return i;
}

FTVOID menu_disp (FTU8 index)
{
    FTU32 addr = main_menu[index].menuInfo.lut_src,
          X = (main_menu[index].X)*FT800_PIXEL_UNIT;

    HAL_CmdBufIn(BITMAP_HANDLE(MENU_HDL_START+index));
    HAL_CmdBufIn(CELL(0));
    /* TODO: impliment palette 8 later */
    if (main_menu[index].menuInfo.format == PALETTED565 ||
        main_menu[index].menuInfo.format == PALETTED4444) {
        HAL_CmdBufIn(PALETTE_SOURCE(addr));
    }
    HAL_CmdBufIn(VERTEX2F(X,0));
}

FTVOID menu_disp_scale (FTU8 index)
{
    FTU16 w = main_menu[index].menuInfo.wide,
          h = main_menu[index].menuInfo.high;

    HAL_CmdBufIn(SAVE_CONTEXT());
    CoCmd_LOADIDENTITY;
    CoCmd_TRANSLATE(w/2*FT800_TRANSFORM_MAX,h/2*FT800_TRANSFORM_MAX);
    CoCmd_SCALE(FT800_TRANSFORM_MAX/MENU_DENO*main_menu[index].scale,
                FT800_TRANSFORM_MAX/MENU_DENO*main_menu[index].scale);
    CoCmd_TRANSLATE(w/(-2)*FT800_TRANSFORM_MAX,h/(-2)*FT800_TRANSFORM_MAX);
    CoCmd_SETMATRIX;

    menu_disp(index);

    HAL_CmdBufIn(RESTORE_CONTEXT());
}

FTVOID menu_disp_inside (FTU8 index)
{
    FTU16 X = main_menu[index].X+main_menu[index].menuInfo.wide/2;

    HAL_CmdBufIn(SAVE_CONTEXT());
    HAL_CmdBufIn(COLOR_RGB(255,255,255));
    CoCmd_NUMBER(X,0,31,OPT_RIGHTX,index);
    HAL_CmdBufIn(COLOR_RGB(((MENU_MAIN_NUM == index)?255:0),((MENU_MAIN_NUM == index)?0:255),0));
    CoCmd_TEXT(X,FT800_LCD_HIGH/2,((MENU_MAIN_NUM == index)?31:22),OPT_CENTER,
                 main_menu[index].menuInfo.path);
    HAL_CmdBufIn(RESTORE_CONTEXT());
}

FTVOID submenu_detect (FTU8 sub, FTU8 *p_ing)
{
#define RELEASED     (0x8000&nowTouch)
#define DECREASE     (20)
    static FT16 lastTouch = 0;
    FT16 nowTouch = 0xFFFF&(HAL_Read32(REG_TOUCH_SCREEN_XY)>>16);
    FTU8 i,j;
   
    appGP.appIndex = E_MENU_SUB;
    
    if (!(*p_ing)) {
        if (RELEASED) {
            if (lastTouch) {
                /* set the main menu position back to visable */
                for (i = menu_anchor_index(main_menu[sub].X)+1,j=sub+1; 
                     ; i++,j++) {
                    if (i >= MENU_MAIN_NUM) {
                        i = 0;
                    }
                    if (j >= MENU_MAIN_NUM) {
                        j = 0;
                    }
					if (j == sub) {
						break;
					}
                    main_menu[j].X = pos_anchor[i];
                }
                *p_ing = 1;
                appGP.appIndex = E_MENU_M;
                lastTouch = 0;
                main_menu[MENU_MAIN_NUM].inRAM = 0;
            }
            main_menu[MENU_MAIN_NUM].scale = SCALE_RELEASED;
        } else {
            lastTouch = nowTouch;
            main_menu[MENU_MAIN_NUM].scale = SCALE_PRESSED;
        }
    } else {
        for (i = 0; i < MENU_MAIN_NUM; i++) {
            if (i == sub) {
                continue;
            }
            if (main_menu[i].X <= MENU_END_L || main_menu[i].X >= MENU_END_R) {
                continue;
            }

            if (main_menu[i].X < main_menu[sub].X) {
                main_menu[i].X -= DECREASE;
            } else {
                main_menu[i].X += DECREASE;
            }
        } 
        for (i = 0; i < MENU_MAIN_NUM; i++) {
            if (main_menu[i].X <= MENU_END_L || 
                main_menu[i].X >= MENU_END_R || 
                i == sub) {
                continue;
            }
            return;
        }
        *p_ing = 0;
    }
}

FTVOID submenu_disp (FTU32 para)
{
    FTU8 i;
    static FTU8 processing = 1;
    
    appGP.appIndex = E_MENU_ERR;

    if (!main_menu[MENU_MAIN_NUM].inRAM) {
        *(FTU8 *)(main_menu[MENU_MAIN_NUM].menuInfo.path + UPDATE_PATH_INDEX) =
                  main_menu[para].menuInfo.path[UPDATE_PATH_INDEX];
        *(FTU8 *)(main_menu[MENU_MAIN_NUM].menuInfo.path_lut + UPDATE_PATH_INDEX) =
                  main_menu[para].menuInfo.path[UPDATE_PATH_INDEX];
        menu_load(MENU_MAIN_NUM, 0);
    }
	
    HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));
	
    HAL_CmdBufIn(BEGIN(BITMAPS));
    if (processing) {
        menu_disp(MENU_MAIN_NUM);
        menu_disp_inside(MENU_MAIN_NUM);
        for (i = 0; i < MENU_MAIN_NUM;i++) {
            if (i == para) {
                /* don't display the selected one */
                continue;
            }
            menu_disp(i);
            menu_disp_inside(i);
        }
    } else {
        menu_disp_scale(MENU_MAIN_NUM);
        menu_disp_inside(MENU_MAIN_NUM);
    }

	HAL_CmdBufIn(END());
    
    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
    HAL_BufToReg(RAM_CMD,0);

    submenu_detect(para, &processing);
}

FTVOID menu_main (FTU32 para)
{
	static FTU8 inited = 0;
    FTU8 i;

	/* never mind, it's for debug,
	 * this part just for this routine 
     * jump out the outside caller 
     * when error happen */
	appGP.appIndex = E_MENU_ERR;
	appGP.appPara = 0;

	if (!inited) {
		inited = menu_init();
	}

    appGP.appIndex = menu_detect(&(appGP.appPara));

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(0,0,0));
	HAL_CmdBufIn(CLEAR(1,1,1));
	
    HAL_CmdBufIn(BEGIN(BITMAPS));
    for (i = 0; i < MENU_MAIN_NUM; i++) {
        if (main_menu[i].scale == SCALE_PRESSED) {
            menu_disp_scale(i);
        } else {
            menu_disp(i);
        }

        menu_disp_inside(i);
    }
	HAL_CmdBufIn(END());
    
    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
    HAL_BufToReg(RAM_CMD,0);
}

AppFunc APPS_UI[] = {
	menu_main,
    submenu_disp,
	/* Leave this NULL at the buttom of this array */
	NULL
};

