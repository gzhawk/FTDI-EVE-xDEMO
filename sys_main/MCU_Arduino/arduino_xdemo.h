#include <SPI.h>
#include <EEPROM.h>
#ifdef FT800_PRINT
#include <Serial.h>
#endif

#ifndef NULL
#define NULL 0
#endif

#define PLUS_PRO

typedef const char     FTC8;
typedef char           FT8;
typedef unsigned char  FTU8;
typedef signed int     FT16;
typedef unsigned int   FTU16;
typedef signed long    FT32;
typedef unsigned long  FTU32;
#define FTVOID void

#define FLASH8 prog_char
#define FLASHU8 prog_uchar
#define FLASHRD8 pgm_read_byte_near

#define FTPRINT        Serial.print
#define FTDELAY(mS)    delay(mS)
#define FTEND
#define ROOT_PATH 
#define CDATA_PATH NULL
#define FTMAIN FTVOID setup () 
#define FTDUMMY FTVOID loop () {}

#define CALD_TAG_ADDR 0
#define CALD_TAG_DATA "CALD"
#define CALD_TAG_LEN  4

#define FTINDEF static inline

#ifdef FT800_PRINT
#define DBGPRINT FTPRINT("\r\n");FTPRINT("Error ");FTPRINT(__FUNCTION__); \
	             FTPRINT(':');FTPRINT(__LINE__)
/* 
 * if you want to see the serial output
 * the delay is not necessary
 * only wait a while for you to manually open serial mornitor 
 * in Arduino IDE, to see the whole debug output from the beginning
 */
#define FTPREINIT Serial.begin(9600);FTDELAY(5000); \
	              FTPRINT("\r\n");FTPRINT("Ver: ");FTPRINT(FT800_VER)
#else
#define DBGPRINT
#define FTPREINIT 
#endif

#define FTRANDOM(M) random(M)

#define FTEMU

#define FTRES Reader

#define FTIOCNTRL arduino_simugpio

#ifdef PLUS_PRO
#define FT800_SPI_CS (9) //VM800P, Arduino Pro
#define FT800_SDC_CS (8)   //VM800P, Arduino Pro
#else
#define FT800_SPI_CS (10)  //Arduino Uno (Sparkfun: RedBoard)
#define FT800_SDC_CS (8)   //Sparkfun: microSD Shield
#endif

#define FT800_PD_N (4)

#define FT800_CAL_PARA_NUM    (6)
#define SDC_NAME_LEN     8
#define SDC_NAME_EXT_LEN 3
#define SDC_DBG          1
#define SDC_SECTOR_SIZE  512L
#define FAT16            0
#define FAT32            1

#ifndef APPS_SYS
#define APPS_SYS arduino_apps_sys_dummy
#endif

struct dirent {
	FT8 name[SDC_NAME_LEN];
	FT8 ext[SDC_NAME_EXT_LEN];
	FTU8 attribute;
	FTU8 reserved[8];
	FTU16 cluster_hi;  // FAT32 only
	FTU16 time;
	FTU16 date;
	FTU16 cluster;
	FTU32 size;
};

class sdcard {
	public:
		FTVOID sel() { digitalWrite(pin, LOW); }
		FTVOID desel() {
			digitalWrite(pin, HIGH);
			SPI.transfer(0xff); // force DO release
		}
		FTVOID sd_delay(FTU8 n) {
			while (n--)
				SPI.transfer(0xff);
		}

		FTVOID cmd(FTU8 cmd, FTU32 lba = 0, FTU8 crc = 0x95) {

			sel();
			SPI.transfer(0xff);
			SPI.transfer(0x40 | cmd);
			SPI.transfer(0xff & (lba >> 24));
			SPI.transfer(0xff & (lba >> 16));
			SPI.transfer(0xff & (lba >> 8));
			SPI.transfer(0xff & (lba));
			SPI.transfer(crc);
			SPI.transfer(0xff);
		}

		FTU8 R1() 
		{     // read response R1
			FTU8 r;

			while ((r = SPI.transfer(0xff)) & 0x80);
			desel();
			SPI.transfer(0xff);  
			return r;
		}

		FTU8 sdR3(FTU32 &ocr) {  // read response R3
			FTU32 r;
			while ((r = SPI.transfer(0xff)) & 0x80)
				;
			for (FTU8 i = 4; i; i--)
				ocr = (ocr << 8) | SPI.transfer(0xff);

			SPI.transfer(0xff);

			desel();
			return r;
		}

		FTU8 sdR7() {  // read response R3
			FTU32 r;
			while ((r = SPI.transfer(0xff)) & 0x80)
				;
			for (FTU8 i = 4; i; i--)
				SPI.transfer(0xff);
			desel();

			return r;
		}

		FTVOID appcmd(FTU8 cc, FTU32 lba = 0) {
			cmd(55); R1();
			cmd(cc, lba);
		}

		FTU8 begin(FTU8 p)
		{
			FTU16 cts=2000;
			pin = p;


			pinMode(pin, OUTPUT);
			SPI.begin();
			SPI.setClockDivider(SPI_CLOCK_DIV64);
			desel();

			delay(10);      // wait for boot
			sd_delay(10);   // deselected, 80 pulses

			FTU8 r1,r;
			do
			{       // reset, enter idle
				cmd(0);
				do
				{      
					if(cts>0) cts--; 
					else 
					{
						desel();
						return  0;
					}
				}while ((r = SPI.transfer(0xff)) & 0x80);
				desel();
				SPI.transfer(0xff);
				r1 = r;
			}while(r1!=1);

			FTU8 sdhc = 0;
			cmd(8, 0x1aa, 0x87);
			r1 = sdR7();
			sdhc = (r1 == 1);

			while (1) {
				appcmd(41, sdhc ? (1UL << 30) : 0); // card init
				r1 = R1();
				if ((r1 & 1) == 0)
					break;
				delay(100);
			}

			if (sdhc) {
				cmd(58);
				FTU32 OCR;
				sdR3(OCR);
				ccs = 1UL & (OCR >> 30);
			} else {
				ccs = 0;
			}

			FTU8 type_code = rd(0x1be + 0x4);
			switch (type_code) {
				default:
					type = FAT16;
					break;
				case 0x0b:
				case 0x0c:
					type = FAT32;
					break;
			}

			o_partition = SDC_SECTOR_SIZE * rd4(0x1be + 0x8);
			sectors_per_cluster = rd(o_partition + 0xd);
			reserved_sectors = rd2(o_partition + 0xe);
			cluster_size = SDC_SECTOR_SIZE * sectors_per_cluster;


			if (type == FAT16) {
				max_root_dir_entries = rd2(o_partition + 0x11);
				sectors_per_fat = rd2(o_partition + 0x16);
				o_fat = o_partition + SDC_SECTOR_SIZE * reserved_sectors;
				o_root = o_fat + (2 * SDC_SECTOR_SIZE * sectors_per_fat);
				// data area starts with cluster 2, so offset it here
				o_data = o_root + (max_root_dir_entries * 32L) - (2L * cluster_size); 
			} else {
				FTU32 sectors_per_fat = rd4(o_partition + 0x24);
				root_dir_first_cluster = rd4(o_partition + 0x2c);
				FTU32 fat_begin_lba = (o_partition >> 9) + reserved_sectors;
				FTU32 cluster_begin_lba = (o_partition >> 9) + reserved_sectors + (2 * sectors_per_fat);

				o_fat = SDC_SECTOR_SIZE * fat_begin_lba;
				o_root = (SDC_SECTOR_SIZE * (cluster_begin_lba + (root_dir_first_cluster - 2) * sectors_per_cluster));
				o_data = (SDC_SECTOR_SIZE * (cluster_begin_lba - 2 * sectors_per_cluster));
			}
			return 1;
		}
		FTVOID cmd17(FTU32 off) {
			if (ccs)
				cmd(17, off >> 9);
			else
				cmd(17, off & ~511L);
			R1();
			sel();
			while (SPI.transfer(0xff) != 0xfe)
				;
		}
		FTVOID rdn(FTU8 *d, FTU32 off, FTU16 n) {
			cmd17(off);
			FTU16 i;
			FTU16 bo = (off & 511);
			for (i = 0; i < bo; i++)
				SPI.transfer(0xff);
			for (i = 0; i < n; i++)
				*d++ = SPI.transfer(0xff);
			for (i = 0; i < (514 - bo - n); i++)
				SPI.transfer(0xff);
			desel();
		}

		FTU32 rd4(FTU32 off) {
			FTU32 r;
			rdn((FTU8*)&r, off, sizeof(r));
			return r;
		}

		FTU16 rd2(FTU32 off) {
			FTU16 r;
			rdn((FTU8*)&r, off, sizeof(r));
			return r;
		}

		FTU8 rd(FTU32 off) {
			FTU8 r;
			rdn((FTU8*)&r, off, sizeof(r));
			return r;
		}
		FTU8 pin;
		FTU8 ccs;

		FTU8 type;
		FTU16 sectors_per_cluster;
		FTU16 reserved_sectors;
		FTU16 max_root_dir_entries;
		FTU16 sectors_per_fat;
		FTU16 cluster_size;
		FTU32 root_dir_first_cluster;

		// These are all linear addresses, hence the o_ prefix
		FTU32 o_partition;
		FTU32 o_fat;
		FTU32 o_root;
		FTU32 o_data;
};

extern sdcard SD;

FTVOID dos83(FTU8 dst[SDC_NAME_LEN+SDC_NAME_EXT_LEN], FT8 *ps);

class Reader {
	public:
		short openfile(FT8 *filename) {
			FT16 i = 0;
			FTU8 dosname[SDC_NAME_LEN+SDC_NAME_EXT_LEN];
			dirent de;

			dos83(dosname, filename);
			do {
				SD.rdn((FTU8*)&de, SD.o_root + i * 32, sizeof(de));
				if (0 == memcmp(de.name, dosname, SDC_NAME_LEN+SDC_NAME_EXT_LEN)) {
					begin(de);
					return 1;
				}
				i++;
			} while (de.name[0]);
			return 0;
		}

		FTVOID begin(dirent &de) {
			size = de.size;
			cluster = de.cluster;
			if (SD.type == FAT32)
				cluster |= ((FT32)de.cluster_hi << 16);
			sector = 0;
			offset = 0;
		}
		FTVOID nextcluster() {
			if (SD.type == FAT16)
				cluster = SD.rd2(SD.o_fat + 2 * cluster);
			else
				cluster = SD.rd4(SD.o_fat + 4 * cluster);
		}
		FTVOID skipcluster() {
			nextcluster();
			offset += SD.cluster_size;
		}
		FTVOID skipsector() {
			if (sector == SD.sectors_per_cluster) {
				sector = 0;
				nextcluster();
			}
			sector++;
			offset += SDC_SECTOR_SIZE;
		}
		FTVOID seek(FTU32 o) {
			while (offset < o) {
				if ((sector == SD.sectors_per_cluster) && ((o - offset) > (FT32)SD.cluster_size))
					skipcluster();
				else
					skipsector();
			}
		}
		FTVOID readsector() {
			if (sector == SD.sectors_per_cluster) {
				sector = 0;
				nextcluster();
			}
			FTU32 off = SD.o_data + ((FT32)SD.cluster_size * cluster) + (SDC_SECTOR_SIZE * sector);
			SD.cmd17(off & ~(SDC_SECTOR_SIZE-1));
			sector++;
			offset += SDC_SECTOR_SIZE;
		}
		FTVOID readsector(FTU8 *dst) {
			readsector();
			for (FT16 i = 0; i < SDC_SECTOR_SIZE; i++) {
				*dst++ = SPI.transfer(0xff);
			}
			SPI.transfer(0xff);   // consume CRC
			SPI.transfer(0xff);
			SD.desel();
		}
		FTU32 cluster;
		FTU32 offset;
		FTU32 size;
		FTU8 sector;
};
#define TST_CMD_LEN 6
#define TST_MSG_LEN 4

FTVOID arduino_dumy_print (char * p);
FTVOID arduino_simugpio(FTU8 flag);
FTVOID arduino_sdcardInit (FTVOID);
FTU8 arduino_is_tag_vaild (FTVOID);
FTVOID arduino_invaild_tag (FTVOID);
FTVOID arduino_save_cdata (FTU8 *p);
FTVOID arduino_restore_cdata (FTU8 *p);
FTVOID arduino_vaild_tag (FTVOID);
FTVOID arduino_apps_sys_dummy (FTU32 para);
