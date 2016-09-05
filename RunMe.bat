@echo off
echo.
echo DO NOT run the bath file before you install the related application.
echo.
echo 1: it's FT9XX based platform sample/demo codes, 
echo purchase MM900EV1A + UMFTPD2A + UMFT81X to play it.
echo.
echo 2: it's Arduino (Atmel MCU) based platform sample/demo codes, 
echo you have to purchase a FT800PLUS, 
echo or purchase "Arduino Pro" + CCModule/BasicModule to play it.
echo.
echo 3: it's ARM (STM32) based platform sample/demo codes, 
echo the code was developed under the STM32F4-Discovery board, 
echo also, an extra CCModule/BasicModule is needed as a display part.
echo.
echo 4: it's a PC based platform (MS VC2010 Express) sample/demo codes, 
echo you have to have a MPSSE cable and CCModule/BasicModule
echo to play it.
echo.
echo 5: it's a PC based platform (MS VC2012) sample/demo codes, 
echo which you don't need hardware (cable/CCModule/BasicModule, 
echo etc.), it can emulate a LCD window which show the final effect 
echo of running result by FT800.
echo.
echo 6: quit
echo.

choice /C:123456

if errorlevel 6 goto SkipRunme
if errorlevel 5 goto VC2012
if errorlevel 4 goto VC2010
if errorlevel 3 goto STM32
if errorlevel 2 goto Arduino
if errorlevel 1 goto FT9XX

:FT9XX
echo.
@echo Be sure you finished reading main\FT900\ReadMe.txt
@echo 1: Finished
@echo 2: Not Finished
@echo 3: Compile Only
echo.

choice /C:123

if errorlevel 3 goto FT900Compile
if errorlevel 2 goto SkipRunme
if errorlevel 1 goto FT900CompileAndProg

:FT900Compile
cd sys_main\MCU_FT900
make
cd ..\..
pause
goto SkipRunme
:FT900CompileAndProg
cd sys_main\MCU_FT900
make
pause
cd img
python onewire.py loadflash xdemo.bin
cd ..\..\..
goto SkipRunme

:Arduino
echo.
@echo read the sys_main\MCU_Arduino\ReadMe.txt
echo.
pause
goto SkipRunme

:STM32
echo.
@echo Be sure you finished reading main\STM32F4\ReadMe.txt
@echo 1: Finished
@echo 2: Not Finished
@echo 3: Compile Only
echo.
choice /C:123
if errorlevel 3 goto Compile
if errorlevel 2 goto SkipRunme
if errorlevel 1 goto CompileAndProg
:Compile
@echo Start to compile STM32F4 code
@echo off
cd sys_main\MCU_STM32F4
make
cd ..\..
pause
goto SkipRunme
:CompileAndProg
@echo Start to compile STM32F4 code
@echo off
cd sys_main\MCU_STM32F4
make
cd ..\..
choice /M "Y: Jlink Connected N: Jlink Not Connected"
if errorlevel 2 goto SkipRunme
echo.
@echo connect the Jlink between PC and Discovery
@echo run following command to program the flash
@echo 1. device STM32F407VG
@echo 2. h
@echo 3. r
@echo 4. loadbin ./sys_main/MCU_STM32F4/img/xdemo.bin 0
@echo 5. r
@echo 6. g
echo.
jlink
goto SkipRunme

:VC2010
call sys_main\MCU_VC2010\vc2010.vcxproj
goto SkipRunme

:VC2012
call sys_main\MCU_VC2012\vc2012.vcxproj
goto SkipRunme

:SkipRunme