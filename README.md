a. Everything here is ONLY for learning EVE's purpose, I DO NOT have
   any responsibility to any one or any organization, to their usage
   on any peices of my codes. I'll delete related parts, if any meterials
   have any license violate, just let me know.

b. The whole project structure may looks ugly because I was trying to merge
   all those platform in single project under "the more sharing code the better"
   requirement.

c. This series of software is developed base on the learning from SampleApp, 
   SampleApp would have full supported function, 
   and may relatively easy be supported by R&D 
   (since SampleApp was developed by them, while this series 
   is developed by FAE).

d. How to execute the project:
   
d.1 DO change the define in sys_main/platform.h based on each project's hardware
    EVE chip     : FT80X, FT81X, BT81X
    Touch control: Cap, Res
    LCD          : QVGA, HVGA, WQVGA, etc.
    SPI type     : SPI, DSPI, QSPI
    UI type      : define the specific project by using "#define UI_XXX"
                   for example: #define UI_EVEUI, #define UI_ECAR, etc.
                   ONLY define and run one example at a time
    
d.2 All the EVE related code (you may consider as UI code) are under the sys_main\UI:
    sys_main\UI\ARCHIVE   : archived code,very old project, need too much 
                            modify to make it work, I just keep it for myself
               \EVE       : EVE chip related function
               \SCREENSHOT: some project screenshot after running, just for reference
               \UI_xxx.h  : specific project UI related code

d.3 All the system function code are under the sys_main\MCU_XXX:
    Arduino platform: 
        *1 read the ReadMe.txt under the "sys_main\MCU_Arduino"

    FT900 platform  : 
        *1 read the ReadMe.txt.
        *2 type "make" under the "sys_main\MCU_FT900" to make image
        *3 type "python onewire.py loadflash xdemo.bin" under the "sys_main\MCU_FT900\img"
           to program the image into the FT900 platform

    STM32F4 platform:
        *1 read the ReadMe.txt
        *2 type "make" under the "sys_main\STM32F4" to make image
        *3 connect the Jlink between PC and Discovery run following command 
           to program the flash
        *3.1 device STM32F407VG
        *3.2 h
        *3.3 r
        *3.4 loadbin ./sys_main/MCU_STM32F4/img/xdemo.bin 0
        *3.5 r
        *3.6 g
    
    VC platform (first of all, install MSVC2019):
        *1 used PC as a MCU, and emulate EVE output on PC
        *1.1 go to "sys_main\MCU_VC\VC_EMU"
        *1.2 double click the VC_emulator.vcxproj
        *1.3 rebuild all
        *1.4 execute

        *2 used PC as a MCU, and connect EVE hardware via FT4222 device (SPI/DSPI/QSPI)
        *2.1 go to "sys_main\MCU_VC\VC_FT4222"
        *2.2 double click the VC_ft4222.vcxproj: 
        *2.3 rebuild all
        *2.4 connect the FT4222 device between PC and EVE hardware
        *2.5 execute

        *3 used PC as a MCU, and connect EVE hardware via MPSSE device (SPI)
        *3.1 go to "sys_main\MCU_VC\VC_VC_MPSSE"
        *3.2 double click the VC_mpsse.vcxproj: 
        *3.3 rebuild all
        *3.4 connect the MPSSE device between PC and EVE hardware
        *3.5 execute
