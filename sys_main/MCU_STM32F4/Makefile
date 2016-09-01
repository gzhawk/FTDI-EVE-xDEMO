# 
# It's a makefile for FT800 demo for STM32 MCU
# The reason we use  "script" way in stead of using a "IDE" (Keil, Tasking, TrueSTUDIO, Eclipse, etc.) is
# try to provide user more flexiblity, to expand what they want to do, or make them easy porting
# to the environment they used to play on, and make the build environment independent
#

CC=arm-none-eabi-gcc
CP=arm-none-eabi-objcopy
DP=arm-none-eabi-objdump
SIZE=arm-none-eabi-size

CFLAGS=-Wall -mcpu=cortex-m4 -mthumb -mlittle-endian 
XDEF=-D STM32F4 
XDEF+=-D USE_STDPERIPH_DRIVER

XDEMO=xdemo

LIBDIR=./lib
FILEDIR=./sys
IMGDIR=./img/
LIBSDIR=$(LIBDIR)/src
LIBINC=$(LIBDIR)/inc
XDIR=..
UIDIR=../UI
EVEDIR=../UI/EVE

LDFILE=$(LIBDIR)/stm32_flash.ld

STFILE=$(LIBDIR)/startup_stm32f4xx.s

LIBSRC=$(LIBSDIR)/stm32f4xx_it.c \
	   $(LIBSDIR)/stm32f4xx_flash.c \
	   $(LIBSDIR)/eeprom.c \
	   $(LIBSDIR)/stm32f4xx_rcc.c \
	   $(LIBSDIR)/stm32f4xx_gpio.c \
	   $(LIBSDIR)/stm32f4xx_spi.c \
	   $(LIBSDIR)/stm32f4xx_rng.c \
	   $(LIBSDIR)/system_stm32f4xx.c \
	   $(LIBSDIR)/stm32f4_discovery.c \
	   $(LIBSDIR)/stm32f4xx_exti.c \
	   $(LIBSDIR)/stm32f4xx_syscfg.c \
	   $(LIBSDIR)/misc.c 

XSRC=$(FILEDIR)/stm32_xdemo.c \
     $(XDIR)/sys_main.c \
	 $(XDIR)/UI/EVE/EVE_APP.c \
	 $(XDIR)/UI/EVE/EVE_HAL.c 

all:
	$(CC) -I$(XDIR) -I$(UIDIR) -I$(EVEDIR) -I$(FILEDIR) $(CFLAGS) -T$(LDFILE) -I$(LIBINC) -I$(LIBDIR) $(XDEF) $(LIBSRC) $(XSRC) $(STFILE) -o $(IMGDIR)$(XDEMO).elf
	$(CP) -O binary $(IMGDIR)$(XDEMO).elf $(IMGDIR)$(XDEMO).bin
	$(SIZE) $(IMGDIR)$(XDEMO).elf

#	$(CP) -O ihex $(IMGDIR)$(XDEMO).elf $(IMGDIR)$(XDEMO).hex
#	$(DP) -St $(IMGDIR)$(XDEMO).elf > $(IMGDIR)$(XDEMO).lst




