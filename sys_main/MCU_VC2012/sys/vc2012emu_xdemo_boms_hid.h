#ifdef APPS_SYS
#undef APPS_SYS
#define APPS_SYS vc2012emu_apps_sys
#endif

typedef enum UI_INDEX_ {
    UI_MAIN = 0,
    UI_ERR
} UI_INDEX;

#define TEST_DISK_STR_LEN 3
#define TEST_DISK_STA_LEN 50

typedef struct test_rate_ {
    HANDLE threadHDL;
    FTU8 start;     /* stop the test or not */
    FTU8 read;     /* write test or read */
    FTU8 disk[TEST_DISK_STR_LEN];  /* test disk */
    FTU32 size;    /* test package size */
    FTU32 low;     /* lowest rate */
    FTU32 high;    /* highest rate */
    FTU8 status[TEST_DISK_STA_LEN];/* test status report */
}test_rate_t;

FTVOID vc2012emu_apps_sys (FTU32 para);

