
#define TST_CMD_LEN 6
#define TST_MSG_LEN 4

#ifdef APPS_SYS
#undef APPS_SYS
#define APPS_SYS arduino_apps_sys
#endif

FTU8 arduino_tst_msg[TST_CMD_LEN+TST_MSG_LEN+1] = {0xAA,6,0xBA,0,2,2,0,0,0,0,0};

FTU8 arduino_tst_crc (FTU8 * p)
{
	FTU32 i;
	FTU8 crc;

	for (i = 1, crc = 0; i < (TST_CMD_LEN+TST_MSG_LEN+1); i++) {
		crc += p[i];
	}

	return (~crc+1);
}

FTVOID arduino_apps_sys (FTU32 para)
{
	FTU32 i;
    static FTU32 j = 0xAB;

    if (j == para) {
        return;
    }

	if (para) {
		arduino_tst_msg[TST_CMD_LEN] = 1;
	} else {
		arduino_tst_msg[TST_CMD_LEN] = 0;
	}

	for (i = 0; i < (TST_CMD_LEN+TST_MSG_LEN+1); i++) {
		FTPRINT(arduino_tst_msg[i]);
	}

	FTPRINT(arduino_tst_crc(arduino_tst_msg));

    j = para;
}
