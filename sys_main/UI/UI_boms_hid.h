/* 
    Training project code on display
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2016/Aug
*/

#define RET_W    80
#define RET_H    40

test_rate_t rate_data = {0};
#define DISK_LIST_LEN 3
FTU8 diskList[DISK_LIST_LEN][3] = {"G:","E:","F:"};
#define PACK_LIST_LEN 3
FTU32 packList[PACK_LIST_LEN] = {512, 1024, 1024*5};

FTVOID touch_detect (FTU8 *p, FTU8 t)
{
    static FTU8 i = 0, j = 0;

    if (t == '<' ||
        t == '>' ||
        t == '{' ||
        t == '}' ||
        t == 'W' ||
        t == 'R' ||
        t == 'G' ||
        t == 'S') {
        if (*p != t && *p == 0) {
            *p = t;
        } else {
            appGP.appIndex = UI_MAIN;
        }
        return;
    }

    /* 
       only trigger the action 
       1. after press
       AND
       2. be released 
     */
    if (*p &&
        t != '<' &&
        t != '>' &&
        t != '{' &&
        t != '}' &&
        t != 'W' &&
        t != 'R' &&
        t != 'G' &&
        t != 'S') {

        switch (*p) {
            case '<':
                if (!i) {
                    i = DISK_LIST_LEN - 1;
                } else {
                    i--;
                }
                rate_data.disk[0] = diskList[i][0];
                break;
            case '>':
                if (i == DISK_LIST_LEN - 1) {
                    i = 0;
                } else {
                    i++;
                }
                rate_data.disk[0] = diskList[i][0];
                break;
            case '{':
                if(!j) {
                    j = PACK_LIST_LEN - 1;
                } else {
                    j--;
                }
                rate_data.size = packList[j];
                break;
            case '}':
                if (j == PACK_LIST_LEN - 1) {
                    j = 0;
                } else {
                    j++;
                }
                rate_data.size = packList[j];
                break;
            case 'G':
                rate_data.start = 1;
                rate_data.low = 0;
                rate_data.high = 0;
                appGP.appPara = (FTU32)&rate_data;
                break;
            case 'S':
                rate_data.start = 0;
                rate_data.low = 0;
                rate_data.high = 0;
                break;
            case 'W':
				/* when press W means switch to R */
                rate_data.read = 1;
                break;
            case 'R':
				/* when press R means switch to W */
                rate_data.read = 0;
                break;
            default:
                break;
        }
        *p = 0;
    }
}

FTVOID test_boms (FTU32 para)
{
#define TEST_FONT    25
#define TEST_LOW_X   50
#define TEST_HIGH_X  450
#define TEST_STATUS_X (EVE_LCD_WIDTH/2)
#define TEST_STATUS_Y (EVE_LCD_HIGH/2)
#define TEST_TXT_GAP 200
#define TEST_KEY_W   40
#define TEST_KEY_H   40
#define TEST_DSK_Y   (EVE_LCD_HIGH*2/3)
#define TEST_PAK_Y   TEST_DSK_Y
#define TEST_ACT_Y   (EVE_LCD_HIGH-TEST_KEY_H)
    static FTU8 pressed = 0, inited = 0;

    if (!inited) {
        strcpy_s(rate_data.disk,TEST_DISK_STR_LEN,&diskList[0][0]);
        strcpy_s(rate_data.status,TEST_DISK_STA_LEN,"init");
        rate_data.size = packList[0];
        inited = 1;
    }

    touch_detect(&pressed, HAL_Read8(REG_TOUCH_TAG));

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255,255,255));
	HAL_CmdBufIn(CLEAR(1,1,1));
    
    HAL_CmdBufIn(COLOR_RGB(0,74,111));

    if (rate_data.read) {
        CoCmd_TEXT(TEST_LOW_X, EVE_LCD_HIGH/3, TEST_FONT, OPT_CENTERY, "Low (MB/s):");
        CoCmd_TEXT(TEST_HIGH_X, EVE_LCD_HIGH/3, TEST_FONT, OPT_CENTERY, "High (MB/s):");
    } else {
        CoCmd_TEXT(TEST_LOW_X, EVE_LCD_HIGH/3, TEST_FONT, OPT_CENTERY, "Low (KB/s):");
        CoCmd_TEXT(TEST_HIGH_X, EVE_LCD_HIGH/3, TEST_FONT, OPT_CENTERY, "High (KB/s):");
    }

    CoCmd_NUMBER(TEST_LOW_X+TEST_TXT_GAP, EVE_LCD_HIGH/3, TEST_FONT, OPT_CENTERY, rate_data.low);
    CoCmd_NUMBER(TEST_HIGH_X+TEST_TXT_GAP, EVE_LCD_HIGH/3, TEST_FONT, OPT_CENTERY, rate_data.high);
   
    CoCmd_TEXT(TEST_LOW_X, TEST_DSK_Y, TEST_FONT, 0, "Disk");
    CoCmd_TEXT(TEST_HIGH_X, TEST_DSK_Y, TEST_FONT, 0, "PackSize");
    
    CoCmd_TEXT(TEST_STATUS_X, TEST_STATUS_Y, TEST_FONT-2, OPT_CENTER, rate_data.status);

    if (rate_data.start) {
        HAL_CmdBufIn(COLOR_RGB(0,74,111));
        CoCmd_TEXT(TEST_LOW_X, TEST_DSK_Y+TEST_KEY_H, TEST_FONT, 0, rate_data.disk);
        CoCmd_NUMBER(TEST_HIGH_X, TEST_DSK_Y+TEST_KEY_H, TEST_FONT, 0, rate_data.size);

        if (rate_data.read) {
            CoCmd_TEXT(EVE_LCD_WIDTH/2-TEST_KEY_W*3, TEST_ACT_Y, TEST_FONT, 0, "Read");
        } else {
            CoCmd_TEXT(EVE_LCD_WIDTH/2-TEST_KEY_W*3, TEST_ACT_Y, TEST_FONT, 0, "Write");
        }

        HAL_CmdBufIn(COLOR_RGB(255,0,0));
        CoCmd_KEY((EVE_LCD_WIDTH)/2, TEST_ACT_Y, TEST_KEY_W*3, TEST_KEY_H, TEST_FONT, pressed, "S");
    } else {
        HAL_CmdBufIn(COLOR_RGB(255,255,255));
        CoCmd_KEY(TEST_LOW_X, TEST_DSK_Y+TEST_KEY_H, TEST_KEY_W, TEST_KEY_H, TEST_FONT, pressed, "<");
        CoCmd_KEY(TEST_LOW_X+TEST_KEY_W*2, TEST_DSK_Y+TEST_KEY_H, TEST_KEY_W, TEST_KEY_H, TEST_FONT, pressed, ">");
        
        CoCmd_KEY(TEST_HIGH_X, TEST_DSK_Y+TEST_KEY_H, TEST_KEY_W, TEST_KEY_H, TEST_FONT, pressed, "{");
        CoCmd_KEY(TEST_HIGH_X+TEST_KEY_W*3, TEST_DSK_Y+TEST_KEY_H, TEST_KEY_W, TEST_KEY_H, TEST_FONT, pressed, "}");
        
        if (rate_data.read) {
            CoCmd_KEY((EVE_LCD_WIDTH)/2-TEST_KEY_W*3, TEST_ACT_Y, TEST_KEY_W*3, TEST_KEY_H, TEST_FONT, pressed, "R");
        } else {
            CoCmd_KEY((EVE_LCD_WIDTH)/2-TEST_KEY_W*3, TEST_ACT_Y, TEST_KEY_W*3, TEST_KEY_H, TEST_FONT, pressed, "W");
        }

        HAL_CmdBufIn(COLOR_RGB(0,74,111));
        CoCmd_TEXT(TEST_LOW_X+TEST_KEY_W, TEST_DSK_Y+TEST_KEY_H, TEST_FONT, 0, rate_data.disk);
        CoCmd_NUMBER(TEST_HIGH_X+TEST_KEY_W, TEST_DSK_Y+TEST_KEY_H, TEST_FONT, 0, rate_data.size);

        HAL_CmdBufIn(COLOR_RGB(0,255,0));
        CoCmd_KEY((EVE_LCD_WIDTH)/2, TEST_ACT_Y, TEST_KEY_W*3, TEST_KEY_H, TEST_FONT, pressed, "G");
    }


    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
    HAL_BufToReg(RAM_CMD,0);
    
    appGP.appIndex = UI_MAIN;
}

AppFunc APPS_UI[] = {
	test_boms
};
#elif defined(FT9XXEV)

data_pool_st dataPool = {0};

typedef enum KB_MODE_ {
    KB_MODE_NON_CAP = 0,
    KB_MODE_IS_CAP,
    KB_MODE_NUM_SYM,
}KB_MODE;
#define RET_FONT 25
#define KB_DEL   0x7F
#define KB_ENT   0x0D
#define KEYS_IN_KB(x) (((x) >= ' ' && (x) <= '~'))
/* 
    only trigger the action 
    1. after press
    AND
    2. be released 
 */
FTVOID kb_touch (FTU8 *p, FTU8 t, FTU32 i, FTU8 *m)
{
    if (KEYS_IN_KB(t)) {
        if (*p != t && *p == 0) {
            *p = t;
        }
    } else {
        while (*p) {
            if (KEYS_IN_KB(*p)) {
                if (*p == '^') {
                    if (*m == KB_MODE_NON_CAP) {
                        *m = KB_MODE_IS_CAP;
                        break;
                    } else if (*m == KB_MODE_IS_CAP) {
                        *m = KB_MODE_NON_CAP;
                        break;
                    }
                }

                if (*p == '*') {
                    if (*m == KB_MODE_NON_CAP) {
                        *m = KB_MODE_NUM_SYM;
                        break;
                    } else if (*m == KB_MODE_IS_CAP) {
                        *m = KB_MODE_NUM_SYM;
                        break;
                    }
                }

                if (*p == 'C') {
                    if (*m == KB_MODE_NUM_SYM) {
                        *m = KB_MODE_NON_CAP;
                        break;
                    }
                }

                if (DATA4UI == dataPool.direction) {
                    dataPool.type = i;
                    dataPool.direction = DATA4MCU;
                    switch (*p) {
                        case '-':
                            if (*m == KB_MODE_NON_CAP || *m == KB_MODE_IS_CAP) {
                                dataPool.data[0] = KB_DEL;
                            } else {
                                dataPool.data[0] = *p;
                            }
                            break;
                        case '_':
                            if (*m == KB_MODE_NON_CAP || *m == KB_MODE_IS_CAP) {
                                dataPool.data[0] = ' ';
                            } else {
                                dataPool.data[0] = *p;
                            }
                            break;
                        case '>':
                            if (*m == KB_MODE_NON_CAP || *m == KB_MODE_IS_CAP) {
                                dataPool.data[0] = KB_ENT;
                            } else {
                                dataPool.data[0] = *p;
                            }
                            break;
                        case 'B':
                            if (*m == KB_MODE_NUM_SYM) {
                                dataPool.data[0] = KB_DEL;
                            } else {
                                dataPool.data[0] = *p;
                            }
                            break;
                        case 'S':
                            if (*m == KB_MODE_NUM_SYM) {
                                dataPool.data[0] = ' ';
                            } else {
                                dataPool.data[0] = *p;
                            }
                            break;
                        case 'E':
                            if (*m == KB_MODE_NUM_SYM) {
                                dataPool.data[0] = KB_ENT;
                            } else {
                                dataPool.data[0] = *p;
                            }
                            break;
                        default:
                            dataPool.data[0] = *p;
                            break;
                    }
                }
            }
            break;
        }
        *p = 0;
    }

	/* if not change to next UI, make it stay in this UI */
	if (appGP.appIndex != UI_KB2PC) {
		appGP.appIndex = i;
	}
}
FTVOID simple_keyboard_ui (FTU32 ui_index)
{
#define SIM_KB_FONT  25
#define SIM_KB_KEY_W 50
#define SIM_KB_KEY_H 50
#define SIM_KB_GAP   50
#define SIM_KB_L1_Y  (EVE_LCD_HIGH/2)
#define SIM_KB_L2_Y  (EVE_LCD_HIGH/2+SIM_KB_KEY_H)
#define SIM_KB_L3_Y  (EVE_LCD_HIGH/2+2*SIM_KB_KEY_H)
#define SIM_KB_L4_Y  (EVE_LCD_HIGH/2+3*SIM_KB_KEY_H)
    static FTU8 pressed = 0, mode = KB_MODE_NON_CAP, inited = 0;

    /* 
     * assign the data exchange mempool
     * to the exchange variable
     */
    if (!inited) {
        appGP.appPara = (FTU32)&dataPool;
        dataPool.type = appGP.appIndex;
        dataPool.direction = DATA4UI;
        inited = 1;
    }

    kb_touch(&pressed, HAL_Read8(REG_TOUCH_TAG), ui_index, &mode);

	HAL_CmdBufIn(CMD_DLSTART);
	HAL_CmdBufIn(CLEAR_COLOR_RGB(255,255,255));
	HAL_CmdBufIn(CLEAR(1,1,1));
    
    switch (mode) {
        case KB_MODE_NON_CAP:
            CoCmd_KEY(SIM_KB_GAP, SIM_KB_L1_Y, SIM_KB_KEY_W*14, SIM_KB_KEY_H, SIM_KB_FONT, pressed, "abcdefghijklmn");
            CoCmd_KEY(SIM_KB_GAP+SIM_KB_KEY_W, SIM_KB_L3_Y, SIM_KB_KEY_W*4, SIM_KB_KEY_H, SIM_KB_FONT, pressed, "*");
            CoCmd_KEY(SIM_KB_GAP+SIM_KB_KEY_W*5, SIM_KB_L3_Y, SIM_KB_KEY_W*4, SIM_KB_KEY_H, SIM_KB_FONT, pressed, "_");
            if (ui_index == UI_KB2PC) {
                CoCmd_KEY(SIM_KB_GAP, SIM_KB_L2_Y, SIM_KB_KEY_W*14, SIM_KB_KEY_H, SIM_KB_FONT, pressed, "^opqrstuvwxyz-");
                CoCmd_KEY(SIM_KB_GAP+SIM_KB_KEY_W*9, SIM_KB_L3_Y, SIM_KB_KEY_W*4, SIM_KB_KEY_H, SIM_KB_FONT, pressed, ">");
            } else {
                CoCmd_KEY(SIM_KB_GAP, SIM_KB_L2_Y, SIM_KB_KEY_W*14, SIM_KB_KEY_H, SIM_KB_FONT, pressed, "^opqrstuvwxyz");
            }
            break;
        case KB_MODE_IS_CAP:
            CoCmd_KEY(SIM_KB_GAP, SIM_KB_L1_Y, SIM_KB_KEY_W*14, SIM_KB_KEY_H, SIM_KB_FONT, pressed, "ABCDEFGHIJKLMN");
            CoCmd_KEY(SIM_KB_GAP, SIM_KB_L2_Y, SIM_KB_KEY_W*14, SIM_KB_KEY_H, SIM_KB_FONT, pressed, "^OPQRSTUVWXYZ-");
            CoCmd_KEY(SIM_KB_GAP+SIM_KB_KEY_W, SIM_KB_L3_Y, SIM_KB_KEY_W*4, SIM_KB_KEY_H, SIM_KB_FONT, pressed, "*");
            CoCmd_KEY(SIM_KB_GAP+SIM_KB_KEY_W*5, SIM_KB_L3_Y, SIM_KB_KEY_W*4, SIM_KB_KEY_H, SIM_KB_FONT, pressed, "_");
            if (ui_index == UI_KB2PC) {
                CoCmd_KEY(SIM_KB_GAP, SIM_KB_L2_Y, SIM_KB_KEY_W*14, SIM_KB_KEY_H, SIM_KB_FONT, pressed, "^OPQRSTUVWXYZ-");
                CoCmd_KEY(SIM_KB_GAP+SIM_KB_KEY_W*9, SIM_KB_L3_Y, SIM_KB_KEY_W*4, SIM_KB_KEY_H, SIM_KB_FONT, pressed, ">");
            } else {
                CoCmd_KEY(SIM_KB_GAP, SIM_KB_L2_Y, SIM_KB_KEY_W*14, SIM_KB_KEY_H, SIM_KB_FONT, pressed, "^OPQRSTUVWXYZ");
            }
            break;
        case KB_MODE_NUM_SYM:
            CoCmd_KEY(SIM_KB_GAP, SIM_KB_L1_Y, SIM_KB_KEY_W*14, SIM_KB_KEY_H, SIM_KB_FONT, pressed, "~`1234567890-_");
            CoCmd_KEY(SIM_KB_GAP, SIM_KB_L2_Y, SIM_KB_KEY_W*14, SIM_KB_KEY_H, SIM_KB_FONT, pressed, "!@#$%^&*()?+=");
            CoCmd_KEY(SIM_KB_GAP, SIM_KB_L3_Y, SIM_KB_KEY_W*14, SIM_KB_KEY_H, SIM_KB_FONT, pressed, "[]{}\";:<>,.\'/\\");
            if (ui_index == UI_KB2PC) {
                CoCmd_KEY(SIM_KB_GAP+SIM_KB_KEY_W*4+SIM_KB_KEY_W/2, SIM_KB_L4_Y, SIM_KB_KEY_W*5, SIM_KB_KEY_H, SIM_KB_FONT, pressed, "CB|SE");
            } else {
                CoCmd_KEY(SIM_KB_GAP+SIM_KB_KEY_W*4+SIM_KB_KEY_W/2, SIM_KB_L4_Y, SIM_KB_KEY_W*5, SIM_KB_KEY_H, SIM_KB_FONT, pressed, "C|S");
            }
            break;
        default:
            break;
    }

    HAL_CmdBufIn(COLOR_RGB(0,74,111));
    if (ui_index == UI_KB2PC) {
        CoCmd_TEXT(EVE_LCD_WIDTH/2, EVE_LCD_HIGH/5, SIM_KB_FONT, OPT_CENTER, "Open a TXT file on PC, press the key to check");
        if (mode == KB_MODE_NON_CAP || mode == KB_MODE_IS_CAP) {
            CoCmd_TEXT(EVE_LCD_WIDTH/2, EVE_LCD_HIGH/3, SIM_KB_FONT, OPT_CENTER, "Shift[^] Backspace[-] NumSym[*] Space[_] Enter[>]");
        } else {
            CoCmd_TEXT(EVE_LCD_WIDTH/2, EVE_LCD_HIGH/3, SIM_KB_FONT, OPT_CENTER, "Backspace[B] Char[C] Space[S] Enter[E]");
        }
    } else {
        CoCmd_TEXT(EVE_LCD_WIDTH/2, EVE_LCD_HIGH/5, SIM_KB_FONT, OPT_CENTER, "Press the key to write into test file in SD card");
        if (mode == KB_MODE_NON_CAP || mode == KB_MODE_IS_CAP) {
            CoCmd_TEXT(EVE_LCD_WIDTH/2, EVE_LCD_HIGH/3, SIM_KB_FONT, OPT_CENTER, "Shift[^] NumSym[*] Space[_]");
        } else {
            CoCmd_TEXT(EVE_LCD_WIDTH/2, EVE_LCD_HIGH/3, SIM_KB_FONT, OPT_CENTER, "Char[C] Space[S]");
        }
    }

    HAL_CmdBufIn(DISPLAY());
	HAL_CmdBufIn(CMD_SWAP);
    HAL_BufToReg(RAM_CMD,0);
}

FTVOID keyboard_to_pc (FTU32 para)
{
    simple_keyboard_ui(UI_KB2PC);
}

AppFunc APPS_UI[] = {
	keyboard_to_pc
};

