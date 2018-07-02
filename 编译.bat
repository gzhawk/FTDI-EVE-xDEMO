@echo off
echo.
echo �벻Ҫֱ�ӽ���sys_main�ļ��У���ͨ���ļ�������
echo �²�ĳ��ƽ̨Ӧ���еı��뻷�������Գ������п�����
echo ��װ����Ӧ�������������Զ��򿪶�Ӧƽ̨�µı��뻷����
echo.
echo 1: FT9XX�����£���Ҫ����MM900EV1A+UMFTPD2A+UMFT81X���ʹ��.
echo.
echo 2: Arduino (Atmel MCU)�����£���VM800P/VM801P�ϣ����һ������USB����ֱ������
echo ��Arduino���IDE������Arduino.cc��վ�������ʹ�ã�
echo ���ߣ�ֱ�ӹ���Arduino Proϵ�а��ӣ�����CCModule/BasicModule����
echo.
echo 3: ARM (STM32)�����£����ǻ���STM32F4-Discovery�����忪����
echo Discoveryֻ��MCUƽ̨����Ҫ����CCModule/BasicModule���С�
echo.
echo 4: PC�����£�ͨ��VC2010���뻷����ģ��MCU����EVE��
echo ������ʹ��MPSSE���º�CCModule/BasicModule���е��ԡ�
echo.
echo 5: PC�����£�ͨ��VC2012��ע�⣬��2012������2010�����뻷����ģ��MCU����EVE��
echo �������ʹ���κ���Χ�豸������/ģ��ȵȣ��� 
echo ������ֱ����PC��ģ���LCD�����������н����ʾ��ģ�����LCD����
echo.
echo 6: PC�����£�ͨ��VC2017��ע�⣬��2017������2012/2010�����뻷����ģ��MCU����EVE��
echo �������ʹ���κ���Χ�豸������/ģ��ȵȣ��� 
echo ������ֱ����PC��ģ���LCD�����������н����ʾ��ģ�����LCD����
echo.
echo 7: �˳�
echo.

choice /C:1234567

if errorlevel 7 goto SkipRunme
if errorlevel 6 goto VC2017
if errorlevel 5 goto VC2012
if errorlevel 4 goto VC2010
if errorlevel 3 goto STM32
if errorlevel 2 goto Arduino
if errorlevel 1 goto FT9XX

:FT9XX
echo.
@echo ��ȷ�����sys_main\MCU_FT900\ReadMe.txt�������й������ټ���
@echo 1: �����
@echo 2: δ���
@echo 3: ������
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
@echo ���Ķ�sys_main\MCU_Arduino\ReadMe.txt
echo.
pause
goto SkipRunme

:STM32
echo.
@echo ��ȷ�����sys_main\MCU_STM32F4\ReadMe.txt�������й������ټ���
@echo 1: �����
@echo 2: δ���
@echo 3: ������
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
choice /M "Y: Jlink������ N: Jlinkδ����"
if errorlevel 2 goto SkipRunme
echo.
@echo ʹ��Jlink����PC��Discovery����
@echo ��˳��ִ���������������¼
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

:VC2017
call sys_main\MCU_VC2017\vc2017.vcxproj
goto SkipRunme

:SkipRunme
