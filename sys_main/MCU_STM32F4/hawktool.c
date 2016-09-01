/* the very first file storage starting flag 
 * storage format should be:
 * FILE_SADDR       0x557EE7AA (4bytes)
 * FILE_SADDR+04    file number(4bytes)
 * FILE_SADDR+08    file1 name (8bytes)
 * FILE_SADDR+16    file1 len  (4bytes)
 * FILE_SADDR+20    file1 data (YYbytes)
 * FILE_SADDR+20+YY file2 name (8bytes)
 * FILE_SADDR+20+YY file2 len  (4bytes)
 * FILE_SADDR+20+YY file2 data (KKbytes)
 */
#include <stdio.h>
#include <windows.h>
#define BYTESxALIGN(x,l) (((l)%x)?((l)+(x-(l)%x)):(l))
#define F_MAGIC 0x557EE7AA
#define F_MAGL  4
#define F_NUML  4
#define F_NAMEL 8
#define F_LENL  4
#define SKIP_LEN 2
#define F_IMG_BLK 512
#define F_TMP_N "temp____file"
#define F_PAK_N "package.bin"
#define F_IMG_N "xdemo.bin"
#define BYTES4ALIGN(l) (BYTESxALIGN(4,l))
#define BYTES512ALIGN(l) (BYTESxALIGN(512,l))
int getFile (unsigned char** ppbuf, char * pname)
{
	FILE * pFile;
	int l;
	
	pFile = fopen (pname,"rb");
	if (pFile!=NULL)
	{
		fseek (pFile , 0 , SEEK_END);
		l = ftell (pFile);
		rewind (pFile);
		*ppbuf = (unsigned char*)malloc(l);
		if (*ppbuf) {
			fread (*ppbuf,1,l,pFile);
		}
		fclose (pFile);

		return *ppbuf?l:0;
	}

	return 0;
}
int getName (unsigned char** ppbuf, char* pname, int *buf_len)
{
	int i = 0, j = 0, l = *buf_len;
	unsigned char *p = *ppbuf;

	if (*buf_len == 0) {
		return 0;
	}
	while (i <= (l-SKIP_LEN)) {
		/* don't include *.exe and *.bin file */
		if (p[i] == 0xD && p[i+1] == 0xA) {
			if (p[i-1] == 'e' && 
				p[i-2] == 'x' && 
				p[i-3] == 'e' && 
				p[i-4] == '.') {
				l -= (i+SKIP_LEN);
				if (l == 0) {
					*buf_len = 0;
					return 0;
				}
				p += (i+SKIP_LEN);
				i = 0;
				continue;
			}

			if (p[i-1] == 'n' && 
				p[i-2] == 'i' && 
				p[i-3] == 'b' && 
				p[i-4] == '.') {
				l -= (i+SKIP_LEN);
				if (l == 0) {
					*buf_len = 0;
					return 0;
				}
				p += (i+SKIP_LEN);
				i = 0;
				continue;
			}

			if (F_NAMEL < i) {
				l -= (i+SKIP_LEN);
				if (l == 0) {
					*buf_len = 0;
					return 0;
				}
				p += (i+SKIP_LEN);
				i = 0;			
				continue;
			} else {
				j = 0xFF;
				break;
			}
		}
		i++;
	}

	if (j == 0xFF) {
		for (j = 0; j < i && j < F_NAMEL; j++) {
			pname[j] = p[j];
		}
	
		*buf_len = (l - i - SKIP_LEN);
		*ppbuf = (p + i + SKIP_LEN);
		return i;
	} else {
		return 0;
	}
}
int main (void)
{
	FILE * pFile;
	int l, x, y, l1;
	unsigned char * p, *buf, *buf1, header[512]; 
	char name[F_NAMEL] = {0};
	system("del /q "F_TMP_N);
	system("del /q "F_PAK_N);
	system("dir /b > "F_TMP_N);

	l = getFile(&p, F_TMP_N);
	if (l)
	{
		for (buf = p, memset(name,0,F_NAMEL), x = getName(&buf, name, &l), y = 0; 
			 x;
			 memset(name,0,F_NAMEL),x = getName(&buf, name, &l)) {
			if (y == 0) {
				l1 = getFile(&buf1, name);
				if (l1) {
					pFile = fopen (F_PAK_N,"wb+");
					if (pFile!=NULL) {
						*(unsigned int *)header = F_MAGIC;
						memcpy((unsigned char *)(header+F_MAGL+F_NUML),name,F_NAMEL);
						*(unsigned int *)(header+F_MAGL+F_NUML+F_NAMEL) = BYTES4ALIGN(l1);
						fwrite (header, 1, F_MAGL+F_NUML+F_NAMEL+F_LENL, pFile);
						fwrite (buf1, 1, l1, pFile);
						l1 = (BYTES4ALIGN(l1) - l1);
						if (l1) {
							memset(header,0xFF,l1);
							fwrite (header, 1, l1, pFile);
						}
						fclose(pFile);
						y++;
					}
					free(buf1);
				}
			} else {
				l1 = getFile(&buf1, name);
				if (l1) {
					pFile = fopen (F_PAK_N,"ab+");
					if (pFile!=NULL) {
						memcpy(header,name,F_NAMEL);
						*(unsigned int *)(header+F_NAMEL) = BYTES4ALIGN(l1);
						fwrite (header, 1, F_NAMEL+F_LENL, pFile);
						fwrite (buf1, 1, l1, pFile);
						l1 = (BYTES4ALIGN(l1) - l1);
						if (l1) {
							memset(header,0xFF,l1);
							fwrite (header, 1, l1, pFile);
						}
						fclose(pFile);
						y++;
					}
					free(buf1);
				}
			}
		}
		pFile = fopen (F_PAK_N,"rb+");
		if (pFile!=NULL) {
			fseek (pFile, F_MAGL, SEEK_SET);
			fwrite ((unsigned char *)&y, 1, F_NUML, pFile);
			fclose(pFile);
		}
		free(p);
	}
	system("del /q "F_TMP_N);

	/* pack the package.bin into image */
	pFile = fopen (F_IMG_N,"ab+");
	if (pFile!=NULL) {
		fseek (pFile , 0 , SEEK_END);
		l = ftell (pFile);
		l1 = getFile(&p, F_PAK_N);
		if (l1)
		{
			l = (BYTES512ALIGN(l) - l);
			if (l) {
				memset(header,0xFF,l);
				fwrite (header, 1, l, pFile);
			}
			fwrite (p, 1, l1, pFile);
			free(p);
			system("del /q "F_PAK_N);
		}
		fclose(pFile);
	}
	return 0;
}

