echo off

del /Q ..\arduino_xdemo*.*
del /Q ..\*.ino
del /Q ..\UI*.h
del /Q ..\EVE*.*

cd ..
rename *.cpp *.c
cd MCU_Arduino

pause

