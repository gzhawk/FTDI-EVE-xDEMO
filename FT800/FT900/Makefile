# 
# It's a makefile for FT800 demo for FT9XX MCU
# The reason we use  "script" way in stead of using a "IDE" (FT900 Eclipse) is
# try to provide user more flexiblity, to expand what they want to do, or make them easy porting
# to the environment they used to play on, and make the build environment independent
#
XDEMO=xdemo

TOOLDIR="C:\Program Files (x86)\FTDI\FT90x Toolchain\Toolchain\tools\bin\"

CC=$(TOOLDIR)ft32-elf-gcc
CP=$(TOOLDIR)ft32-elf-ld
SIZE=$(TOOLDIR)ft32-elf-size

CFLAGS=-D FT9XXEV -Wall -O0 -ffunction-sections -Wl,--gc-sections -I. -I.. 

LIB9XX="C:\Program Files (x86)\FTDI\FT90x Toolchain\Toolchain\hardware\lib"
LIBINC="C:\Program Files (x86)\FTDI\FT90x Toolchain\Toolchain\hardware\include"
LIBFS=./lib
LIB=-L$(LIB9XX) -L$(LIBFS) -lc -lfatfs -lstub -lft900 

XSRC= $(LIBFS)/ft900_xdemo.c \
	  ../FT800.c \
	  ../FT800_APP.c \
	  ../FT800_HAL.c 

IMGDIR=./img

XDEMO:
	$(CC) $(CFLAGS) -I$(LIBFS) -I$(LIBINC) $(XSRC) $(LIB) -Wl,--entry=_start -o $(IMGDIR)/$(XDEMO).elf
	$(SIZE) --format=berkeley -x $(IMGDIR)/$(XDEMO).elf
	$(CP) --oformat binary $(IMGDIR)/$(XDEMO).elf -o $(IMGDIR)/$(XDEMO).bin

