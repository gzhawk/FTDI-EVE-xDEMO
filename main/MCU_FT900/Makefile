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
LIBFS=./sys
LIB=-L$(LIB9XX) -L$(LIBFS) -L"../UI/EVE" -L"../" -lc -lfatfs -lstub -lft900 

XSRC= $(LIBFS)/ft900_xdemo.c \
	  ../main.c \
	  ../UI/EVE/EVE_APP.c \
	  ../UI/EVE/EVE_HAL.c 

IMGDIR=./img

all:
	$(CC) $(CFLAGS) -I$(LIBFS) -I$(LIBINC) $(XSRC) $(LIB) -Wl,--entry=_start -o $(IMGDIR)/$(XDEMO).elf
	$(SIZE) --format=berkeley -x $(IMGDIR)/$(XDEMO).elf
	$(CP) --oformat binary $(IMGDIR)/$(XDEMO).elf -o $(IMGDIR)/$(XDEMO).bin

