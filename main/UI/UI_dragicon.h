/* 
    Demo to show how to impliment:
		1. drag&drop icon
		2. animation icon
    Author: Hawk
	Email : hawk.gao@ftdichip.com
	Date  : 2013/Dec
*/

#define MENU_FONT 23
#define TOGGLE_FONT 18
#define TOGGLE_DSKP_W   55
#define TOGGLE_DSKP_X   (FT800_LCD_WIDTH - TOGGLE_DSKP_W - 15)
#define TOGGLE_DSKP_Y   8
#define ICON_W 32
#define ICON_H 32
#define ICON_AMP 8
#define ICON_AMP_UNIT 1
#define ICON_ALPHA 50
#define ICON_CLASS 2
#define ICON_CLASS_ITEMS 4
#define ICON_NUM   (ICON_CLASS*ICON_CLASS_ITEMS)
#define ICON_ZOOM_U 4
#define ICON_ZOOM_D 5
/* TOUCH_COUNT_XXX numbers depand on your CPU speed */
#define TOUCH_COUNT_ACTIVE 50
#define CLR_BRD CLEAR_COLOR_RGB(203,222,182)
#define GRAD_BKG CoCmd_GRADIENT(0,0,0xCBDEB6,0,FT800_LCD_HIGH,0)

#define CLOSET_W_DEFAULT  (3*ICON_W)
#define CLOSET_W_TAG      (FT800_LCD_WIDTH/32)//the tag area of closet, at the very left side of closet
#define CLOSET_W_BAR      (FT800_LCD_WIDTH/16)//the very right side of closet, color is RGB_BAR

#define CLOSET_X       (CLOSET_W_TAG+(CLOSET_W_DEFAULT)/2)
#define CLOSET_Y(i)    ((FT800_LCD_HIGH/5)*((i)+1))

#define RGB_TITLE COLOR_RGB(0xFF,0xFF,0xFF)
#define RGB_TRANS COLOR_RGB(200, 200, 200)
#define RGB_PLACE COLOR_RGB(170, 170, 170)
#define RGB_BAR   COLOR_RGB(0xFF,0xFF,0)

#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(FT9XXEV)
#define PATH_TRANS ROOT_PATH"dragicon\\trans.raw"
#define PATH_PLACE ROOT_PATH"dragicon\\place.raw"
#else
#define PATH_TRANS ROOT_PATH"trans.raw"
#define PATH_PLACE ROOT_PATH"place.raw"

#define DESKTOP_TAG_ADDR (CALD_TAG_ADDR+CALD_TAG_LEN+FT800_CAL_PARA_NUM*FTU32_LEN)
#define DESKTOP_TAG_DATA "DSKP"
#define DESKTOP_TAG_LEN  4
/* 
 * DSKP  : 4 bytes
 * number: 1 bytes
 * handle: 1 bytes
 * cell  : 1 bytes
 * handle: 1 bytes
 * cell  : 1 bytes
 ...
*/

#endif

bmpHDR_st bmpHD[ICON_CLASS] = {
	{PATH_TRANS,	0,0,ARGB4,	0,0,	ICON_W,	ICON_H},
	{PATH_PLACE,	0,0,ARGB4,	0,0,	ICON_W,	ICON_H}
};

typedef enum actHDL_ {
	HDL_NONE = 0,
	HDL_TRANS,
	HDL_PLACE
	/* no more than 15 */
} actHDL_e;

typedef enum tEvent_ {
	T_NON = 100, /* no event on screen */
	T_CLK,       /* obj be click */ 
	T_DRG        /* obj be dragged */ 
} tEvent_e;

typedef enum tTag_ {
	TG_TAG_TRANS = 1, /* closet tag */
	TG_TAG_PLACE,     /* closet tag */
	TG_BAR,           /* closet bar */
	TG_TOGGLE,        /* desktop toggle */
	TG_TRANS_0,       /* icon */
	TG_TRANS_1,       /* icon */
	TG_TRANS_2,       /* icon */
	TG_TRANS_3,       /* icon */
	TG_PLACE_0,       /* icon */
	TG_PLACE_1,       /* icon */
	TG_PLACE_2,       /* icon */
	TG_PLACE_3,       /* icon */
} tTag_e;

typedef struct icon_ {
	FTU8      handle;
	FTU8      cell;
	FTU16     X;
	FTU16     Y;
}icon_st;

typedef struct desktop_node {
	icon_st * pdesktop_list;
	struct desktop_node * prev;
	struct desktop_node * next;
}desktop_node_st;

typedef struct closet_list_ {
	FTU8 active;
	FTU8 alpha;
	icon_st list[ICON_CLASS_ITEMS];
} closet_list_st;

closet_list_st closet_list = {0};

FTU16 g_closet_line = CLOSET_W_BAR;

desktop_node_st *phead = NULL;

icon_st icon_select = {HDL_NONE,0,0,0};

FTU16 desktop_grid[ICON_CLASS_ITEMS*ICON_CLASS][2] = {{0},{0}};

typedef struct list_st_ {
	icon_st icon_data;
	desktop_node_st desktop_data;
} list_st;
list_st list_buf[ICON_CLASS_ITEMS*ICON_CLASS] = {0};

/* dynamic generate each icon location(X,Y)
   based on the icon numbers on the screen */
FTINDEF FTVOID genGrid (FTU8 num)
{
	FTU32 i,w,h;

	for (i = 0; i < num; i++) {
		if (num <= ICON_CLASS_ITEMS) {
			h = (FT800_LCD_HIGH - 2*ICON_H)/2;
			w = (FT800_LCD_WIDTH-CLOSET_W_TAG)/(num+1);
			desktop_grid[i][0] = w * (i+1);
			desktop_grid[i][1] = h;
		} else {
			h = (FT800_LCD_HIGH - 2*ICON_H)/3;
			if (i < ICON_CLASS_ITEMS) {
				w = (FT800_LCD_WIDTH-CLOSET_W_TAG)/(ICON_CLASS_ITEMS+1);
				desktop_grid[i][0] = w * (i+1);
				desktop_grid[i][1] = h;
			} else {
				w = (FT800_LCD_WIDTH-CLOSET_W_TAG)/(num - ICON_CLASS_ITEMS + 1);
				desktop_grid[i][0] = w * ((i%ICON_CLASS_ITEMS)+1);
				desktop_grid[i][1] = 2*h;
			}
		}
	}
}

/* 0: not in target; 1: in target */
FTINDEF FTU8 inTarget (icon_st *p, icon_st *target)
{
	if (target->handle == p->handle
		&& target->cell == p->cell
		&& target->X-ICON_W/2 <= p->X 
		&& target->X+ICON_W/2 >= p->X
		&& target->Y-ICON_H/2 <= p->Y
		&& target->Y+ICON_H/2 >= p->Y) {
		return 1;			
	}		

	return 0;
}

FTINDEF FTU8 inCloset (icon_st *p)
{
	FTU8 i;

	if (closet_list.active) {
		for (i = 0; i < ICON_CLASS_ITEMS; i++) {
			if (inTarget(p, &closet_list.list[i])) {
				return 1;
			}
		}
	}

	return 0;
}

FTINDEF desktop_node_st * list_create (icon_st * p)
{
	icon_st *tmp_i = NULL;
	desktop_node_st *tmp_n = NULL;
	FTU8 i = (p->handle-HDL_TRANS)*ICON_CLASS_ITEMS+p->cell;

	tmp_i = &list_buf[i].icon_data;
	tmp_n = &list_buf[i].desktop_data;

	tmp_i->handle = p->handle;
	tmp_i->cell   = p->cell;
	tmp_i->X      = p->X;
	tmp_i->Y      = p->Y;

	tmp_n->pdesktop_list = tmp_i;
	tmp_n->prev = tmp_n;
	tmp_n->next = NULL;

	return tmp_n;
}

FTINDEF FTVOID desktop_list_destory_tag (FTVOID)
{
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(FT9XXEV)

#elif defined(STM32F4)

#else
	EEPROM.write(DESKTOP_TAG_ADDR,0xFF);
#endif
}

#if !defined(MSVC2010EXPRESS) && !defined(MSVC2012EMU) && !defined(STM32F4) && !defined(FT9XXEV)
FTINDEF FTU8 desktop_list_check_tag (FTVOID)
{
	FTU8 tag[] = DESKTOP_TAG_DATA;
	FTU32 addr = DESKTOP_TAG_ADDR, i;

	for (i = 0, addr = DESKTOP_TAG_ADDR; i < DESKTOP_TAG_LEN; i++, addr++) {
		if (tag[i] != EEPROM.read(addr)) {
			return 0;
		}
	}

	return 1;
}
#endif

FTINDEF desktop_node_st * desktop_list_search (icon_st *p, FTU8 *pindex)
{
	desktop_node_st *tmp = phead;
	
	*pindex = 0;
	while (tmp) {
		*pindex += 1;
		if (tmp->pdesktop_list->handle == p->handle
			&& tmp->pdesktop_list->cell == p->cell) {
			return tmp;
		}
		tmp = tmp->next;
	}
	
	return NULL;
}

FTINDEF FTVOID desktop_list_save (FTVOID)
{
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(FT9XXEV)

#elif defined(STM32F4)

#else
	desktop_node_st *tmp = phead;
	FTU8 i = 0, j;
	FTU8 tag[] = DESKTOP_TAG_DATA;
	FTU32 addr = DESKTOP_TAG_ADDR+DESKTOP_TAG_LEN+1;


	while (tmp) {
		EEPROM.write(addr,(FTU8)tmp->pdesktop_list->handle);
		EEPROM.write(addr+1,(FTU8)tmp->pdesktop_list->cell);
		i++;
		addr += 2;
		tmp = tmp->next;
	}
	if (i) {
		addr = DESKTOP_TAG_ADDR;
		for (j = 0; j < DESKTOP_TAG_LEN; j++, addr++) {
			EEPROM.write(addr,tag[j]);
		}
		EEPROM.write(addr,i);
	}
#endif
}

/* 0: not found, 1: deleted */
FTINDEF FTU8 desktop_list_delete (icon_st * p, FTU8 save)
{
	FTU8 i = 0;
	desktop_node_st *tmp = desktop_list_search(p, &i);

	/* check if already existed */
	if (tmp == NULL) {
		return 0;
	}

	if (tmp->next) {
		if (tmp->prev == tmp) {
			/* if the deleting node is the first node */
			phead = tmp->next;
			phead->prev = phead;
		} else {
			tmp->prev->next = tmp->next;
			tmp->next->prev = tmp->prev;
		}
	} else {
		if (tmp->prev == tmp) {
			/* if it's a last node, assign a NULL to head pointer */
			phead = NULL;
		} else {
			tmp->prev->next = NULL;
		}
	}
	tmp->pdesktop_list = NULL;
	tmp = NULL;
	if (save) {
		desktop_list_save();
	}

	return 1;
}

/* 0: success, 1: error, 2: list full 3: node exsited */
FTINDEF FTU8 desktop_list_insert_end (icon_st * p, FTU8 save)
{
	desktop_node_st *tmp = NULL;
	FTU8 count;

	if (phead) {
		/* check if already existed */
		tmp = desktop_list_search(p,&count);
		if (tmp) {
			return 3;
		}

		/* check if list full */
		if (count >= ICON_NUM) {
			return 2;
		}

		/* go to list end */
		tmp = phead;
		while (tmp->next) {
			tmp = tmp->next;
		}
		/* insert to end */
		tmp->next = list_create(p);
		if (tmp->next == NULL) {
			return 1;
		}
		tmp->next->prev = tmp;
	} else {
		phead = list_create(p);
		if (phead == NULL) {
			return 1;
		}
	}
	if (tmp == NULL) {
		tmp = phead;
	} else {
		tmp = tmp->next;
	}

	if (save) {
		desktop_list_save();
	}

	return 0;
}

FTINDEF FTU8 desktop_list_read (FTVOID)
{
#if defined(MSVC2010EXPRESS) || defined(MSVC2012EMU) || defined(FT9XXEV)
	return 0;
#elif defined(STM32F4)
	return 0;
#else
	icon_st node;
	FTU32 addr = DESKTOP_TAG_ADDR + DESKTOP_TAG_LEN, i;

	/* check tag */
	if(0 == desktop_list_check_tag()) {
		return 0;
	}

	/* desktop icon numbers */
	i = EEPROM.read(addr++);
	while (i) {
		node.handle = EEPROM.read(addr);
		node.cell = EEPROM.read(addr+1);
		if (node.handle > HDL_PLACE || node.cell >= ICON_CLASS_ITEMS) {
			/* some thing wrong in data, make it invalid */
			EEPROM.write(DESKTOP_TAG_ADDR, 0);
			return 0;
		}
		desktop_list_insert_end(&node, 0);
		i--;
		addr += 2;
	}

	return 1;
#endif
}

FTINDEF FTVOID desktop_list_align (FTVOID)
{
	desktop_node_st *tmp = phead;
	FTU8 i = 0;

	while (tmp) {
		tmp = tmp->next;
		++i;
	}

	if (i) {
		genGrid(i);
	}

	tmp = phead;
	i = 0;
	while (tmp) {
		tmp->pdesktop_list->X = desktop_grid[i][0];
		tmp->pdesktop_list->Y = desktop_grid[i][1];
		tmp = tmp->next;
		i++;
	}
}


FTINDEF FTVOID defaultMatrix (FTVOID)
{
	HAL_CmdBufIn(BITMAP_TRANSFORM_A(256));
	HAL_CmdBufIn(BITMAP_TRANSFORM_B(0));
	HAL_CmdBufIn(BITMAP_TRANSFORM_C(0));
	HAL_CmdBufIn(BITMAP_TRANSFORM_D(0));
	HAL_CmdBufIn(BITMAP_TRANSFORM_E(256));
	HAL_CmdBufIn(BITMAP_TRANSFORM_F(0));
}

FTINDEF FTVOID rects (FTU32 x0, FTU32 y0, FTU32 x1, FTU32 y1, FTU32 rgb)
{
	HAL_CmdBufIn(BEGIN(RECTS)); 
	HAL_CmdBufIn(rgb); 
	HAL_CmdBufIn(VERTEX2F((x0)*FT800_PIXEL_UNIT,(y0)*FT800_PIXEL_UNIT)); 
	HAL_CmdBufIn(VERTEX2F((x1)*FT800_PIXEL_UNIT,(y1)*FT800_PIXEL_UNIT)); 
	HAL_CmdBufIn(END());
}

FTINDEF FTVOID Swing (icon_st *i)
{
	static FT8 R = 1;

	/* remove the previous setting effect e.g. color */
	HAL_CmdBufIn(COLOR_RGB(0xFF,0xFF,0xFF));
	HAL_CmdBufIn(BEGIN(BITMAPS));
	CoCmd_LOADIDENTITY;
	CoCmd_TRANSLATE(ICON_W/2*FT800_TRANSFORM_MAX,ICON_H/2*FT800_TRANSFORM_MAX);
	CoCmd_ROTATE(R*FT800_TRANSFORM_MAX/360);
	CoCmd_TRANSLATE(-ICON_W/2*FT800_TRANSFORM_MAX,-ICON_H/2*FT800_TRANSFORM_MAX);
	CoCmd_SETMATRIX;
	/* display in ICON central */
	HAL_CmdBufIn(BITMAP_HANDLE(i->handle));
	HAL_CmdBufIn(CELL(i->cell));
	HAL_CmdBufIn(VERTEX2F((i->X-ICON_W/2)*FT800_PIXEL_UNIT,(i->Y-ICON_H/2)*FT800_PIXEL_UNIT));
	defaultMatrix();
	HAL_CmdBufIn(END());
	if (R > 0) {
		R = R > ICON_AMP?(-1*ICON_AMP_UNIT):R + ICON_AMP_UNIT;
	} else {
		R = R < (-1*ICON_AMP)?ICON_AMP_UNIT:R - ICON_AMP_UNIT;
	}
}

FTINDEF FTVOID fillCloset (FTU8 hdl)
{
	FTU8 i;

	for (i = 0; i < ICON_CLASS_ITEMS; i++) {
		closet_list.list[i].X = CLOSET_X;
		closet_list.list[i].Y = CLOSET_Y(i);
		closet_list.list[i].handle = hdl;
		closet_list.list[i].cell = i;
	}
}

FTINDEF FTVOID displayCloset (icon_st *p, tEvent_e event, FTU8 Touch)
{
	FTU32 i = g_closet_line-CLOSET_W_BAR;

	/* for un-initial closet list only */
	if (closet_list.list[0].handle == HDL_NONE) {
		fillCloset(HDL_TRANS);
	}

	/* closet tag */
	HAL_CmdBufIn(TAG_MASK(1));
	HAL_CmdBufIn(TAG(TG_TAG_TRANS));
	rects(0,0,CLOSET_W_TAG,FT800_LCD_HIGH/ICON_CLASS,RGB_TRANS);
	HAL_CmdBufIn(TAG(TG_TAG_PLACE));
	rects(0,FT800_LCD_HIGH/ICON_CLASS,CLOSET_W_TAG,FT800_LCD_HIGH,RGB_PLACE);
	HAL_CmdBufIn(TAG_MASK(0));
	/* closet space */
	if (closet_list.list[0].handle == HDL_TRANS) {
		rects(CLOSET_W_TAG,0,i,FT800_LCD_HIGH,RGB_TRANS);
	} else {
		rects(CLOSET_W_TAG,0,i,FT800_LCD_HIGH,RGB_PLACE);
	}

	/* show closet background */
	HAL_CmdBufIn(RGB_TITLE);
	CoCmd_TEXT(i/2,0,MENU_FONT,OPT_CENTERX,"Closet");

	/* icon show out effect */
	if (0xFF != closet_list.alpha) {
		closet_list.alpha += 1;
	}
	/* remove the previous setting color */
	HAL_CmdBufIn(COLOR_RGB(0xFF,0xFF,0xFF));
	/* closet icon */
	HAL_CmdBufIn(TAG_MASK(1));
	HAL_CmdBufIn(BEGIN(BITMAPS));
	for (i = 0; i < ICON_CLASS_ITEMS; i++) {
		if (event == T_DRG) {
			/* make dragging icon in closet transparent */
			if (Touch && closet_list.list[i].handle == p->handle 
					&& closet_list.list[i].cell == p->cell) {
				HAL_CmdBufIn(COLOR_A(ICON_ALPHA));
			} else {
				HAL_CmdBufIn(COLOR_A(closet_list.alpha));
			}
		} else {
			HAL_CmdBufIn(COLOR_A(closet_list.alpha));
		}
		if (closet_list.list[i].handle == HDL_TRANS) {
			HAL_CmdBufIn(TAG(TG_TRANS_0+i));
		} else {
			HAL_CmdBufIn(TAG(TG_PLACE_0+i));
		}
		if (Touch && event == T_CLK && inTarget(p, &closet_list.list[i])) {
			CoCmd_LOADIDENTITY;
			CoCmd_TRANSLATE(ICON_W/2*FT800_TRANSFORM_MAX,ICON_H/2*FT800_TRANSFORM_MAX);
			CoCmd_SCALE(FT800_TRANSFORM_MAX/ICON_ZOOM_D*ICON_ZOOM_U,FT800_TRANSFORM_MAX/ICON_ZOOM_D*ICON_ZOOM_U);
			CoCmd_TRANSLATE(-ICON_W/2*FT800_TRANSFORM_MAX,-ICON_H/2*FT800_TRANSFORM_MAX);
			CoCmd_SETMATRIX;
		}
		HAL_CmdBufIn(BITMAP_HANDLE(closet_list.list[i].handle));
		HAL_CmdBufIn(CELL(closet_list.list[i].cell));
		HAL_CmdBufIn(VERTEX2F((closet_list.list[i].X-ICON_W/2)*FT800_PIXEL_UNIT,(closet_list.list[i].Y-ICON_H/2)*FT800_PIXEL_UNIT));
		defaultMatrix();
	}

	/* keep others normal alpha */
	HAL_CmdBufIn(COLOR_A(255));
	HAL_CmdBufIn(END());
	HAL_CmdBufIn(TAG_MASK(0));
}

FTINDEF FTVOID displayDesktop (icon_st *p, tEvent_e event, FTU8 Touch)
{
	desktop_node_st *tmp = phead;
	
	HAL_CmdBufIn(RGB_TITLE);
	CoCmd_TEXT(g_closet_line+(FT800_LCD_WIDTH-g_closet_line)/2,0,MENU_FONT,OPT_CENTERX,"Desktop");

	if (tmp) {
		HAL_CmdBufIn(TAG_MASK(1));
		HAL_CmdBufIn(BEGIN(BITMAPS));
		do {
			/* just in case the desktop icon be draw to closet area */
			if (closet_list.active && (g_closet_line >= tmp->pdesktop_list->X)) {
		
			} else {
				if (tmp->pdesktop_list->handle == HDL_TRANS) {
					HAL_CmdBufIn(TAG(TG_TRANS_0+tmp->pdesktop_list->cell));
				} else {
					HAL_CmdBufIn(TAG(TG_PLACE_0+tmp->pdesktop_list->cell));
				}
				if (Touch && event == T_CLK && inTarget(p, tmp->pdesktop_list)) {
					CoCmd_LOADIDENTITY;
					CoCmd_TRANSLATE(ICON_W/2*FT800_TRANSFORM_MAX,ICON_H/2*FT800_TRANSFORM_MAX);
					CoCmd_SCALE(FT800_TRANSFORM_MAX/ICON_ZOOM_D*ICON_ZOOM_U,FT800_TRANSFORM_MAX/ICON_ZOOM_D*ICON_ZOOM_U);
					CoCmd_TRANSLATE(-ICON_W/2*FT800_TRANSFORM_MAX,-ICON_H/2*FT800_TRANSFORM_MAX);
					CoCmd_SETMATRIX;	
				}
				HAL_CmdBufIn(BITMAP_HANDLE(tmp->pdesktop_list->handle));
				HAL_CmdBufIn(CELL(tmp->pdesktop_list->cell));
				HAL_CmdBufIn(VERTEX2F((tmp->pdesktop_list->X-ICON_W/2)*FT800_PIXEL_UNIT,(tmp->pdesktop_list->Y-ICON_H/2)*FT800_PIXEL_UNIT));
				defaultMatrix();
			}
			tmp = tmp->next;			
		} while (tmp);
		HAL_CmdBufIn(END());
		HAL_CmdBufIn(TAG_MASK(0));
	}
}

FTINDEF FTVOID displayBar (FTVOID)
{
	/* display closet bar */
	HAL_CmdBufIn(TAG_MASK(1));
	HAL_CmdBufIn(TAG(TG_BAR));
	rects(g_closet_line-CLOSET_W_BAR,0,g_closet_line,FT800_LCD_HIGH,RGB_BAR);
	HAL_CmdBufIn(END());
	HAL_CmdBufIn(TAG_MASK(0));
}

#if !defined(MSVC2010EXPRESS) && !defined(MSVC2012EMU) && !defined(FT9XXEV)
FTINDEF FTVOID displayToggle (FTU8 state)
{
	/*TODO: actuall, the deperator should be 0xFF "Unsave/xffSave"
	 * but I just can not make it work, find the root case later*/
	FTU8 str[] = "Unsave   Save";
	/* display closet bar */
	HAL_CmdBufIn(TAG_MASK(1));
	CoCmd_BGCOLOR(0xC8C8C8);
	CoCmd_FGCOLOR(0x969696);
	HAL_CmdBufIn(TAG(TG_TOGGLE));
	CoCmd_TOGGLE(TOGGLE_DSKP_X,TOGGLE_DSKP_Y,TOGGLE_DSKP_W,TOGGLE_FONT,0,state?0xFFFF:0,str);
	HAL_CmdBufIn(END());
	HAL_CmdBufIn(TAG_MASK(0));
}
#endif
/* 1: finish display, 0: keep looping */
FTINDEF FTU8 displayItems (icon_st *p, FTU8 Touch, FTU8 *pSave)
{
	FTU8 tmp;
	static tEvent_e lastEvent = T_NON;
	static FTU8 lastObj = 0;
	static FTU32 count = 0;

	if (lastEvent != T_DRG) {
		if (Touch) {
			lastObj = HAL_Read8(REG_TOUCH_TAG);
			if (TG_TAG_TRANS <= lastObj && TG_TOGGLE >= lastObj) {
				lastEvent = T_CLK;
			} else if (TG_TRANS_0 <= lastObj && TG_PLACE_3 >= lastObj) {
				lastEvent = T_CLK;
				if (++count > TOUCH_COUNT_ACTIVE) {
					/* if the desktop has it
					 * and the continue touch is happend in closet
					 * treat it as click */
					if (desktop_list_search(p, &tmp) && inCloset(p)) {
						lastEvent = T_CLK;
					} else {
						lastEvent = T_DRG;
					}
				}
			} else {
				if (p->X > g_closet_line) {
					closet_list.active = 0;
					closet_list.alpha = 0;
					g_closet_line = CLOSET_W_BAR;
				}
			}
			p->handle = (lastObj < TG_PLACE_0)?HDL_TRANS:HDL_PLACE;
			p->cell = (lastObj < TG_PLACE_0)?(lastObj - TG_TRANS_0):(lastObj - TG_PLACE_0);
		} else {
			if (TG_TOGGLE == lastObj) {
				if (*pSave == 0) {
					*pSave = 1;
					desktop_list_save();
				} else {
					*pSave = 0;
					desktop_list_destory_tag();
				}
			} else if (TG_TRANS_0 <= lastObj && TG_PLACE_3 >= lastObj) {
				if (count < TOUCH_COUNT_ACTIVE) {
					lastEvent = T_CLK;
				}
			}
			count = 0;
		}
	}

	if (closet_list.active) {
		/* show icon in closet */
		displayCloset(p, lastEvent, Touch);
	}

	/* show icon on desktop */
	displayDesktop(p, lastEvent, Touch);

#if !defined(MSVC2010EXPRESS) && !defined(MSVC2012EMU) && !defined(FT9XXEV)
	/* show desktop toggle */
	displayToggle(*pSave);
#endif

	/* show bar */
	displayBar();

	/* has to be put at the end to make swing icon show in the front */
	if (lastEvent == T_DRG) {
		if (Touch) {
			if (p->X > g_closet_line) {
				desktop_list_delete(p, *pSave);
			}
			Swing(p);
		} else {
			if (p->X > g_closet_line) {
				desktop_list_insert_end(p, *pSave);
			}
			lastEvent = T_NON;
			lastObj = 0;
			desktop_list_align();
		}
	} else if (lastEvent == T_CLK && Touch == 0) {
			switch (lastObj) {
				case TG_BAR:
					/* closet bar judgement */
					closet_list.active = closet_list.active?0:1;
					if (closet_list.active) {
						g_closet_line = CLOSET_W_DEFAULT+CLOSET_W_TAG+CLOSET_W_BAR;
					} else {
						g_closet_line = CLOSET_W_BAR;
						closet_list.alpha = 0;
					}
					break;
				case TG_TAG_TRANS:
					/* closet trans tag */
					fillCloset(HDL_TRANS);
					break;
				case TG_TAG_PLACE:
					/* closet place tag */
					fillCloset(HDL_PLACE);
					break;
				case TG_TRANS_0:
				case TG_TRANS_1:
				case TG_TRANS_2:
				case TG_TRANS_3:
					/* trans icon */
					icon_select.handle = HDL_TRANS;
					icon_select.cell = lastObj - TG_TRANS_0;
					lastEvent = T_NON;
					lastObj = 0;
					count = 0;
					g_closet_line = CLOSET_W_BAR;
					closet_list.active = 0;
					closet_list.alpha = 0;
					return 1;
				case TG_PLACE_0:
				case TG_PLACE_1:
				case TG_PLACE_2:
				case TG_PLACE_3:
					/* place icon */
					icon_select.handle = HDL_PLACE;
					icon_select.cell = lastObj - TG_PLACE_0;
					lastEvent = T_NON;
					lastObj = 0;
					count = 0;					
					g_closet_line = CLOSET_W_BAR;
					closet_list.active = 0;
					closet_list.alpha = 0;
					return 1;
				default:
					break;
			}
			lastEvent = T_NON;
			lastObj = 0;
	}

	return 0;
}

FTVOID subScreen (FTU32 para)
{
	static FTU8 ifLoaded = 0;
	FTU16 i = FT800_LCD_WIDTH/5*2, j = 0;
	if (ifLoaded == 0) {
		/* load bitmap resources data into FT800 */
		if(APP_OK != appBmpToRamG(HDL_TRANS, RAM_G, &bmpHD[0], ICON_CLASS)){
			DBGPRINT;
			return;
		}
		ifLoaded = 1;
	}

	while (0 == TOUCHED) {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(CLR_BRD);
		HAL_CmdBufIn(CLEAR(1,1,1));
		GRAD_BKG;

		CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/5,MENU_FONT,OPT_CENTERX,"SubScreen");
		HAL_CmdBufIn(BEGIN(BITMAPS));
		HAL_CmdBufIn(BITMAP_HANDLE(icon_select.handle));
		HAL_CmdBufIn(CELL(icon_select.cell));
		HAL_CmdBufIn(VERTEX2F((i-ICON_W/2)*FT800_PIXEL_UNIT,(FT800_LCD_HIGH/2-ICON_W/2)*FT800_PIXEL_UNIT));
		HAL_CmdBufIn(END());
		CoCmd_TEXT(FT800_LCD_WIDTH/2,FT800_LCD_HIGH/5*4,MENU_FONT,OPT_CENTERX,"Tap any place in screen to return");

		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
		if (j) {
			if (--i == FT800_LCD_WIDTH/5*2) {
				j = 0;
			}
		} else {
			if (++i == FT800_LCD_WIDTH/5*3) {
				j = 1;
			}
		}
	}

	while (TOUCHED) {
		/* after release the press, 
			then change to other UI */
	}
	/* return back */
	appGP.appIndex = 0;
	appGP.appPara = 0;

	return;
}

FTVOID dragIcon (FTU32 para)
{
	icon_st iconLocation;
	FTU32 tmp;
	FTU16 X, Y;
	FTU8 dskpSave;
	static FTU8 ifLoaded = 0;

	/* never mind, it's for debug,
	 * this part just for this routine jump out the outside caller 
	 * when error happen */
	appGP.appIndex = 1;
	appGP.appPara = 0;
#if defined(STM32F4)
	appGP.appIndex = 2;
	return;
#endif
	if (ifLoaded == 0) {
		/* load bitmap resources data into FT800 */
		if(APP_OK != appBmpToRamG(HDL_TRANS, RAM_G, &bmpHD[0], ICON_CLASS)){
			DBGPRINT;
			return;
		}
	}

	/* read the saved desktop data from storage */
	dskpSave = desktop_list_read();
	if (dskpSave) {
		desktop_list_align();
	}

	while (1) {
		HAL_CmdBufIn(CMD_DLSTART);
		HAL_CmdBufIn(CLR_BRD);
		HAL_CmdBufIn(CLEAR(1,1,1));
		GRAD_BKG;
		tmp = HAL_Read32(REG_TOUCH_SCREEN_XY);
		X = (tmp>>16)&0xFFFF;
		Y = tmp&0xFFFF;
		if (Y <= FT800_LCD_HIGH && X <= FT800_LCD_WIDTH) {
			iconLocation.Y = Y;
			iconLocation.X = X;

			if(displayItems(&iconLocation,1, &dskpSave)){
				return;
			}
		} else {
			if(displayItems(&iconLocation,0, &dskpSave)){
				return;
			}
		}

		HAL_CmdBufIn(DISPLAY());
		HAL_CmdBufIn(CMD_SWAP);
		HAL_BufToReg(RAM_CMD,0);
	}
}

AppFunc Apps[] = {
	dragIcon,
	subScreen,
	/* Leave this NULL at the buttom of this array */
	NULL
};

