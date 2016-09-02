echo off

del /Q ..\arduino_xdemo.cpp
del /Q ..\arduino_xdemo.h
del /Q ..\sys_main.ino
del /Q ..\UI*.h
del /Q ..\EVE*.*

cd ..
rename *.cpp *.c
cd MCU_Arduino

pause

