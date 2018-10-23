/* 
    Sample code to show how to make slip menu
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Sep
*/

//FTU8 MENU_0[] = ROOT_PATH"slip_m\\1a.raw";
//FTU8 MENU_0[] = ROOT_PATH"slip_m\\1a.bin";
FTU8 MENU_0[] = ROOT_PATH"slip_m\\1a_inx.bin";
//FTU8 MENU_0[] = ROOT_PATH"slip_m\\1a_inx.raw";

//FTU8 MENU_1[] = ROOT_PATH"slip_m\\2a.raw";
//FTU8 MENU_1[] = ROOT_PATH"slip_m\\2a.bin";
FTU8 MENU_1[] = ROOT_PATH"slip_m\\2a_inx.bin";
//FTU8 MENU_1[] = ROOT_PATH"slip_m\\2a_inx.raw";

//FTU8 MENU_2[] = ROOT_PATH"slip_m\\3a.raw";
//FTU8 MENU_2[] = ROOT_PATH"slip_m\\3a.bin";
FTU8 MENU_2[] = ROOT_PATH"slip_m\\3a_inx.bin";
//FTU8 MENU_2[] = ROOT_PATH"slip_m\\3a_inx.raw";

//FTU8 MENU_3[] = ROOT_PATH"slip_m\\4a.raw";
//FTU8 MENU_3[] = ROOT_PATH"slip_m\\4a.bin";
FTU8 MENU_3[] = ROOT_PATH"slip_m\\4a_inx.bin";
//FTU8 MENU_3[] = ROOT_PATH"slip_m\\4a_inx.raw";

//FTU8 MENU_4[] = ROOT_PATH"slip_m\\5a.raw";
//FTU8 MENU_4[] = ROOT_PATH"slip_m\\5a.bin";
FTU8 MENU_4[] = ROOT_PATH"slip_m\\5a_inx.bin";
//FTU8 MENU_4[] = ROOT_PATH"slip_m\\5a_inx.raw";

//FTU8 MENU_5[] = ROOT_PATH"slip_m\\6a.raw";
//FTU8 MENU_5[] = ROOT_PATH"slip_m\\6a.bin";
FTU8 MENU_5[] = ROOT_PATH"slip_m\\6a_inx.bin";
//FTU8 MENU_5[] = ROOT_PATH"slip_m\\6a_inx.raw";

FTU8 MENU_SUB[] = ROOT_PATH"slip_m\\0_inx.bin";
//FTU8 MENU_SUB[] = ROOT_PATH"slip_m\\0_inx.raw";

FTU8 MENU_0_[] = ROOT_PATH"slip_m\\1a_lut.bin";
FTU8 MENU_1_[] = ROOT_PATH"slip_m\\2a_lut.bin";
FTU8 MENU_2_[] = ROOT_PATH"slip_m\\3a_lut.bin";
FTU8 MENU_3_[] = ROOT_PATH"slip_m\\4a_lut.bin";
FTU8 MENU_4_[] = ROOT_PATH"slip_m\\5a_lut.bin";
FTU8 MENU_5_[] = ROOT_PATH"slip_m\\6a_lut.bin";
FTU8 MENU_SUB_[] = ROOT_PATH"slip_m\\0_lut.bin";

#define MENU_MAIN_NUM  6
#define MENU_NUM_END   '8'
#define MENU_NUM_START '1'
#define MENU_HDL_START 0
#define MENU_RAM_START RAM_G
#define MENU_W         200
#define MENU_W_        800
#define MENU_H         480
#define MENU_DENO      100
#define MENU_END_L     (-200)
#define MENU_END_R     (MENU_END_L+(MENU_MAIN_NUM-1)*MENU_W)

#if defined(FILESYS_USED) 
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
    {{(FTC8 *)MENU_0 , (FTC8 *)MENU_0_ , 
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

    {{(FTC8 *)MENU_5, (FTC8 *)MENU_5_, 
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

    /* get the menu index from pressed location */
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
    
    /* get the anchor index from pressed location */
    for (i = 0; i < MENU_MAIN_NUM; i++) {
        if (pos_anchor[i] == X) {
            break;
        }
    }

    return i;
}

/* set all the menu back to it's original offset position */
FTVOID menu_anchor (FTVOID)
{
    FTU8 i, j;
    menu_prop_st *p = &main_menu[0];

    /* use index 0 to search the anchor */
    for (i = 0; i < MENU_MAIN_NUM; i++) {
        if ((pos_anchor[i]-MENU_W/2) < p->X && (pos_anchor[i]+MENU_W/2) > p->X) {
            p->X = pos_anchor[i];
            break;   
        }
    }

    /* set the rest (start from 1) to the end base on anchor */
    for (j = 1, i += 1; j < MENU_MAIN_NUM; j++,i++) {
        if (i >= MENU_MAIN_NUM) {
            i = 0;
        }
        main_menu[j].X = pos_anchor[i];
    }
}

/* 
 two loop need to be handled, and make it sync:
 1. menu number loop
 2. sub menu number loop
 */
FTVOID menu_new_path (FTU8 i)
{
    FTU8 n;
	FT16 w = (FT16)main_menu[i].menuInfo.wide;

    if (main_menu[i].X <= MENU_END_L+w) {
        /* 1. menu number loop */
        if (i >= (MENU_MAIN_NUM-1)) {
            n = main_menu[0].menuInfo.path[UPDATE_PATH_INDEX] - 1;
        } else {
            n = main_menu[i+1].menuInfo.path[UPDATE_PATH_INDEX] - 1;
        }

        /* 2. sub menu number loop */
        if (n < MENU_NUM_START) {
            *(FTU8 *)(main_menu[i].menuInfo.path + UPDATE_PATH_INDEX) = MENU_NUM_END;
            if (main_menu[i].menuInfo.format == PALETTED565) {
                *(FTU8 *)(main_menu[i].menuInfo.path_lut + UPDATE_PATH_INDEX) = MENU_NUM_END;
            }
        } else {
            *(FTU8 *)(main_menu[i].menuInfo.path + UPDATE_PATH_INDEX) = n;
            if (main_menu[i].menuInfo.format == PALETTED565) {
                *(FTU8 *)(main_menu[i].menuInfo.path_lut + UPDATE_PATH_INDEX) = n;
            }
        }
    } else if (main_menu[i].X >= MENU_END_R-w) {
        /* 1. menu number loop */
        if (i <= 0) {
            n = main_menu[MENU_MAIN_NUM-1].menuInfo.path[UPDATE_PATH_INDEX] + 1;
        } else {
            n = main_menu[i-1].menuInfo.path[UPDATE_PATH_INDEX] + 1;
        }

        /* 2. sub menu number loop */
        if (n > MENU_NUM_END) {
            *(FTU8 *)(main_menu[i].menuInfo.path + UPDATE_PATH_INDEX) = MENU_NUM_START;
            if (main_menu[i].menuInfo.format == PALETTED565) {
                *(FTU8 *)(main_menu[i].menuInfo.path_lut + UPDATE_PATH_INDEX) = MENU_NUM_START;
            }
        } else {
            *(FTU8 *)(main_menu[i].menuInfo.path + UPDATE_PATH_INDEX) = n;
            if (main_menu[i].menuInfo.format == PALETTED565) {
                *(FTU8 *)(main_menu[i].menuInfo.path_lut + UPDATE_PATH_INDEX) = n;
            }
        }
    }
}

FTU8 menu_load (FTU8 i, FTU8 init)
{
    FTU32 ram_addr = main_menu[i].menuInfo.lut_src - main_menu[i].menuInfo.len;
    
    if(!main_menu[i].inRAM){
        /* load the new menu/submenu, base on the index */
        if (APP_OK != appLoadBmp(ram_addr,&(main_menu[i].menuInfo),1) ) {
            return 0;
        }
        main_menu[i].inRAM = 1;
    }

    if (init) {
        /* set the bitmap global variable */
        appUI_FillBmpDL(MENU_HDL_START+i, ram_addr, &(main_menu[i].menuInfo), 1);
    }
    
    return 1;
}

FTU8 menu_init (FTVOID)
{
	FTU8 i;
    
    for (i = 0; i < MENU_MAIN_NUM+1; i++) {
        if(!menu_load(i, 1)){
            DBGPRINT;
            return 0;
        }
    }

    return 1;
}

FTU8 menu_move (FTU8 M_index, FT16 move, FTU8 release)
{
    FTU8 i;
    /* in single touch, the first location is wrong, ignore it */
    static FTU8 ignore = 1;

    for (i = 0; i < MENU_MAIN_NUM; i++) {
        if (release) {
            ignore = 1;
			if (main_menu[i].scale == SCALE_RELEASED) {
				continue;
			} else {
			    main_menu[i].scale = SCALE_RELEASED;
            }
            /* do it only once, below routine would handle all menu */
            if (i == 0) {
                menu_anchor();
            }
        } else {
            if (move) {
                /* user drag or inertia move */
                ignore = 1;
                main_menu[i].scale = SCALE_PRESSED;
                main_menu[i].X += move;
                menu_load_test(&main_menu[i]);
            } else {
                /* single click */
                if (M_index == i) {
                    if (ignore) {
                        ignore = 0;
                        continue;
                    }
                    main_menu[i].scale = SCALE_PRESSED;
                }
            }
        }

        if (!main_menu[i].inRAM) {
            menu_new_path(i);
            if(!menu_load(i, 0)){
                DBGPRINT;
                return 0;
            }
        }
    }

    return 1;
}

/* 
 instead of using out side loop count (WAIT)
 you may use timer to have more accurate result
 */
FT16 menu_slip_inertia (FT16 tX)
{
#define WAIT (8)
    static FT16 lastX = 0, 
                prevX = 0, 
                wait = 0,
                inertia = 0;

    if (0x8000&tX) {
        /* 
         to count the last inertia, what we need is "the last pair (prevX, lastX)"
         the last entry of this routine before release only go "prevX" or "lastX"
         the "inertia" would be used till the next release entry this routine
         */
        if (!prevX && !lastX) {
            inertia = 0;
		}
        wait = 0;
		lastX = 0;
		prevX = 0;
    } else {
        if (!prevX) {
            prevX = tX;
        } else {
            if (lastX) {
                lastX = 0;
                prevX = tX;
            }
            if (++wait >= WAIT) {
                wait = 0;
                lastX = tX;
                inertia = lastX - prevX;
            }
        }
    }
    return inertia;
}

FT16 menu_slip_press (FT16 press, FTU8 *pMenu, FTU32 *pMenuIndex)
{
#define DELTA_L        (-3)
#define DELTA_R        (3)

    static FT16 last_press = 0, drag = 0;
    FT16 delta = 0;
	FTU8 unpress = (0x8000&press)?1:0;

    *pMenu = E_MENU_M;

    /* never touch */
    if (unpress && !last_press) {
        return delta;
    }

    /* first touch */
	if (!last_press) {
		last_press = press;
        return delta;
	}

    /* tell the menu index for single click */
    *pMenuIndex = menu_nunber(last_press);

    if (unpress) {
        if (!drag) {
            if (*pMenuIndex != MENU_MAIN_NUM) {
                *pMenu = E_MENU_SUB;
                main_menu[MENU_MAIN_NUM].inRAM = 0;
            }
        }
        last_press = 0;
        drag = 0;
    } else {
        /* touched, and drag */
		delta = press - last_press;
		last_press = press;
        /* ignore the tiny movement */
        if (delta > DELTA_L && delta < DELTA_R) {
            delta = 0;
        }
        /* 
         once set it to drag
         only unpress can reset it
         */
        if (!drag && delta) {
            drag = 1;
        }
	}

    return delta;
}

FTU8 menu_detect (FTU32 *pMenuIndex)
{
#define STEP (10)

    FT16 Touch = 0xFFFF&(HAL_Read32(REG_TOUCH_SCREEN_XY)>>16);
	FTU8 ui = E_MENU_M, released = (0x8000&Touch)?1:0;
    FT16 D_press = menu_slip_press(Touch,&ui,pMenuIndex);
    FT16 D_inertia = menu_slip_inertia(Touch);
    static FT16 inertiaing = 0;

    if (inertiaing) {
        if (!released) {
            /* any touch during the inertia process would stop the inertia */
            inertiaing = 0;
        } else {
            if(menu_move(*pMenuIndex,inertiaing, 0)) {
                if (inertiaing > 0) {
                    inertiaing -= STEP;
                    if (inertiaing <= 0) {
                        /* avoid endless loop to blow routine */
                        inertiaing = 0;
                    }
                } else {
                    inertiaing += STEP;
                    if (inertiaing >= 0) {
                        /* avoid endless loop to above routine */
                        inertiaing = 0;
                    }
                }
            } else {
                return E_MENU_ERR;
            }
        }
    } else {
        /* only release and fast enough movement can trigger the inertia */
        if (released && D_inertia) {
            inertiaing = D_inertia;
        } else {
            if(!menu_move(*pMenuIndex,D_press, released)){
                return E_MENU_ERR;
            }
        }
    }

    return ui;
}

FTVOID menu_disp (FTU8 index)
{
    FTU32 addr = main_menu[index].menuInfo.lut_src,
          X = (main_menu[index].X)*EVE_PIXEL_UNIT;

    HAL_CmdBufIn(BITMAP_HANDLE(MENU_HDL_START+index));
    HAL_CmdBufIn(CELL(0));
    if (main_menu[index].menuInfo.format == PALETTED565) {
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
    CoCmd_TRANSLATE(w/2*EVE_TRANSFORM_MAX,h/2*EVE_TRANSFORM_MAX);
    CoCmd_SCALE(EVE_TRANSFORM_MAX/MENU_DENO*main_menu[index].scale,
                EVE_TRANSFORM_MAX/MENU_DENO*main_menu[index].scale);
    CoCmd_TRANSLATE(w/(-2)*EVE_TRANSFORM_MAX,h/(-2)*EVE_TRANSFORM_MAX);
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
    CoCmd_TEXT(X,EVE_LCD_HIGH/2,((MENU_MAIN_NUM == index)?31:22),OPT_CENTER,
                 main_menu[index].menuInfo.path);
    HAL_CmdBufIn(RESTORE_CONTEXT());
}

FTVOID submenu_detect (FTU8 sub, FTU8 *p_ing)
{
#define SUB_RELEASED (0x8000&nowTouch)
#define DECREASE     (20)
    static FT16 lastTouch = 0;
    FT16 nowTouch = 0xFFFF&(HAL_Read32(REG_TOUCH_SCREEN_XY)>>16);
    FTU8 i,j;
   
    appGP.appIndex = E_MENU_SUB;
    
    if (!(*p_ing)) {
        /* coming from submenu normal display period */
        if (SUB_RELEASED) {
            if (lastTouch) {
                /*
                 before going back to main menu
                 set the main menu position back to visable 
                 */
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
        /* coming from main menu, need a "moving none selected menu away" effect */
        for (i = 0; i < MENU_MAIN_NUM; i++) {
            if (i == sub) {
                /* ignore selected one */
                continue;
            }
            if (main_menu[i].X <= MENU_END_L || main_menu[i].X >= MENU_END_R) {
                /* ignore invisable two */
                continue;
            }

            /* 
             "moving none selected menu away" effect
             */
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
            /* 
             keep entry this routine
             till all valid "none selected" menu invisable
             */
            return;
        }
        *p_ing = 0;
    }
}

FTVOID submenu_disp (FTU32 para)
{
    FTU8 i;
    static FTU8 processing = 1;
    
    if (!main_menu[MENU_MAIN_NUM].inRAM) {
        *(FTU8 *)(main_menu[MENU_MAIN_NUM].menuInfo.path + UPDATE_PATH_INDEX) =
                  main_menu[para].menuInfo.path[UPDATE_PATH_INDEX];
        if (main_menu[MENU_MAIN_NUM].menuInfo.format == PALETTED565) {
            *(FTU8 *)(main_menu[MENU_MAIN_NUM].menuInfo.path_lut + UPDATE_PATH_INDEX) =
                      main_menu[para].menuInfo.path[UPDATE_PATH_INDEX];
        }
        if(!menu_load(MENU_MAIN_NUM, 0)){
            DBGPRINT;
            return;
        }
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
                /* don't display the selected main menu */
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

	if (!inited) {
		inited = menu_init();
        if (!inited) {
            return;
        }
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
    submenu_disp
};

