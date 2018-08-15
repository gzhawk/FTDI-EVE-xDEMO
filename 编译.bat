@echo off
echo.
echo 请不要直接进入sys_main文件夹，再通过文件夹名字
echo 猜测某个平台应该有的编译环境，擅自尝试自行开启。
echo 安装了相应程序后，批处理会自动打开对应平台下的编译环境。
echo.
echo 1: FT9XX环境下，需要购买MM900EV1A+UMFTPD2A+UMFT81X配合使用.
echo.
echo 2: Arduino (Atmel MCU)环境下，在VM800P/VM801P上，配合一根任意USB线缆直接运行
echo （Arduino相关IDE可以在Arduino.cc网站免费下载使用）
echo 或者，直接购买Arduino Pro系列板子，加上CCModule/BasicModule运行
echo.
echo 3: ARM (STM32)环境下，这是基于STM32F4-Discovery开发板开发的
echo Discovery只有MCU平台，需要购买CCModule/BasicModule运行。
echo.
echo 4: PC环境下，通过VC2017编译环境，模拟MCU控制EVE，
echo 必须结合使用MPSSE线缆和CCModule/BasicModule进行调试。
echo.
echo 5: PC环境下，通过VC2017编译环境，模拟MCU控制EVE，
echo 必须结合使用板载FT4222的CCModule/BasicModule进行调试。
echo.
echo 6: PC环境下，通过VC2017编译环境，模拟MCU控制EVE，
echo 无需额外使用任何外围设备（线缆/模块等等）， 
echo 他可以直接在PC上模拟出LCD屏，并将运行结果显示在模拟出的LCD屏上
echo.
echo 7: 退出
echo.

choice /C:1234567

if errorlevel 7 goto SkipRunme
if errorlevel 6 goto VC_Emulator
if errorlevel 5 goto VC_FT4222
if errorlevel 4 goto VC_Mpsse
if errorlevel 3 goto STM32
if errorlevel 2 goto Arduino
if errorlevel 1 goto FT9XX

:FT9XX
echo.
@echo 请确保完成sys_main\MCU_FT900\ReadMe.txt里面所有工作后再继续
@echo 1: 已完成
@echo 2: 未完成
@echo 3: 仅编译
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
@echo 请阅读sys_main\MCU_Arduino\ReadMe.txt
echo.
pause
goto SkipRunme

:STM32
echo.
@echo 请确保完成sys_main\MCU_STM32F4\ReadMe.txt里面所有工作后再继续
@echo 1: 已完成
@echo 2: 未完成
@echo 3: 仅编译
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
choice /M "Y: Jlink已连接 N: Jlink未连接"
if errorlevel 2 goto SkipRunme
echo.
@echo 使用Jlink连接PC和Discovery板子
@echo 按顺序执行下面命令，进行烧录
@echo 1. device STM32F407VG
@echo 2. h
@echo 3. r
@echo 4. loadbin ./sys_main/MCU_STM32F4/img/xdemo.bin 0
@echo 5. r
@echo 6. g
echo.
jlink
goto SkipRunme

:VC_Mpsse
call sys_main\MCU_VC\vc_mpsse.vcxproj
goto SkipRunme

:VC_FT4222
call sys_main\MCU_VC\vc_ft4222.vcxproj
goto SkipRunme

:VC_Emulator
call sys_main\MCU_VC\vc_emulator.vcxproj
goto SkipRunme

:SkipRunme
