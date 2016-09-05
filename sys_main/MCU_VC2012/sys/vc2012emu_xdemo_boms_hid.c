
#define RUN_TIMES     50
#define RUN_TIMES_X   500
#define DISK_IDX      0
FTU8 tstFile[] =      "Z:\\rTest";
#define STATUS_INFO(p,str)   strcpy_s((char *)((p)->status),TEST_DISK_STA_LEN-1,str)

FTU8 vc2012emu_test_read (test_rate_t *p)
{
	FILE *pF;
    FTU32 tmp,i,j,l;
    FTU8 *mp = NULL;
    FTU64 t1, t2;

	if (!(p->start)) {
		return 1;
	}
    tstFile[DISK_IDX] = p->disk[0];

    STATUS_INFO(p,"read test start ...");
    /* write the file to disk */
    pF = fopen((char *)tstFile,"wb");
    if(!pF) {
        p->start = 0;
        STATUS_INFO(p,"fail to open read test file for write");
        return 1;
    }

    l = (p->size)*RUN_TIMES;
    mp = malloc(l);
    if (!mp) {
        p->start = 0;
        STATUS_INFO(p,"fail to malloc test data for read test");
        fclose(pF);
        remove((char *)tstFile);
        return 1;
    }
    /* set the buffer for write action */
    memset(mp,'W',l);
    for (i = 0; i < RUN_TIMES; i++) {
        fwrite(&mp[i*(p->size)], 1, p->size, pF);
    }
	fclose(pF);

    /* read the file from disk for the read test */
    pF = fopen((char *)tstFile,"r");
    if(!pF) {
        p->start = 0;
		free(mp);
		remove((char *)tstFile);
        STATUS_INFO(p,"fail to open read test file for read");
        return 1;
    }
    t1 = GetTickCount();
    /* 
     read far more faster than write
     read more times, or t1 would equal to t2
     */
    for (j = 0; j < RUN_TIMES_X; j++) {
        /* set the buffer to different for the read compare */
        memset(mp,'R',l);
        fseek(pF,0,SEEK_SET);
        for (i = 0; i < RUN_TIMES; i++) {
            fread(&mp[i*(p->size)], 1, p->size, pF);
        }
        /* only compare the first block */
        if ('W' != mp[p->size]) {
            STATUS_INFO(p,"read file data error");
            free(mp);
            fclose(pF);
            remove(tstFile);
            return 0;
        }
    }
    t2 = GetTickCount();

    free(mp);
    fclose(pF);
    remove((char *)tstFile);

    if (t2 >= t1) {
        t2 -= t1;
    } else {
        t2 += (0xFFFFFFFFFFFFFFFF - t1);
    }

    tmp = l*1000*RUN_TIMES_X/t2;
    tmp /= (1024*1024);

    if (0 == p->low || tmp <= p->low) {
        p->low = tmp;
    }

    if (0 == p->high || tmp > p->high) {
        p->high = tmp;
    }

    STATUS_INFO(p,"read test finished");
    return 0;
}
FTU8 vc2012emu_test_write (test_rate_t *p)
{
	FILE *pF;
    FTU32 tmp,i,l;
    FTU8 *mp = NULL;
    FTU64 t1, t2;

	if (!(p->start)) {
		return 1;
	}

    tstFile[DISK_IDX] = p->disk[0];

    STATUS_INFO(p,"write test start ...");

    pF = fopen((char *)tstFile,"wb");
    if(!pF) {
        p->start = 0;
        STATUS_INFO(p,"fail to open write test file for write");
        return 1;
    }

    l = (p->size)*RUN_TIMES;
    mp = malloc(l);
    if (!mp) {
        p->start = 0;
        STATUS_INFO(p,"fail to malloc test data for write test");
        fclose(pF);
        return 1;
    }
    memset(mp,0xAA,l);

    t1 = GetTickCount();
    for (i = 0; i < RUN_TIMES; i++) {
        fwrite(mp, 1, p->size, pF);
    }
    t2 = GetTickCount();

    free(mp);
    fclose(pF);

    remove((char *)tstFile);

    if (t2 >= t1) {
        t2 -= t1;
    } else {
        t2 += (0xFFFFFFFFFFFFFFFF - t1);
    }

    tmp = l*1000/t2;
    tmp /= 1024;

    if (0 == p->low || tmp <= p->low) {
        p->low = tmp;
    }

    if (0 == p->high || tmp > p->high) {
        p->high = tmp;
    }

    STATUS_INFO(p,"write test finished");
    return 0;
}
DWORD WINAPI vc2012emu_test_proc (test_rate_t *p)
{
    while (p) {
		if (!(p->start)) {
			p->high = 0;
			p->low = 0;
			continue;
		}
		if (!(p->read)) {
			while(!vc2012emu_test_write(p));
		} else {
			while(!vc2012emu_test_read(p));
		}
    }
    return 0;
}
FTVOID vc2012emu_apps_sys (FTU32 para)
{
    FTU32 threadRET = 0;
    test_rate_t *p = (test_rate_t *)para;
    static FTU32 dbg = 0;

    if (!p || (0 == p->size)) {
        return;
    }

    if (!(p->threadHDL)) {
        p->threadHDL = CreateThread( 
                NULL,                   // default security attributes
                0,                      // use default stack size  
                vc2012emu_test_proc,    // thread function name
                p,                      // argument to thread function 
                0,                      // use default creation flags 
                &threadRET);            // returns the thread identifier 
		if (!(p->threadHDL)) {
            STATUS_INFO(p,"fail to start thread");
			p->start = 0;
		}
    }
}

