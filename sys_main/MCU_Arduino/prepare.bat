echo off

copy /Y arduino_xdemo.cpp ..\
copy /Y arduino_xdemo.h ..\
copy /Y sys_main.ino ..\
copy /Y ..\UI\UI.h ..\
copy /Y ..\UI\EVE\*.* ..\

cd ..
rename *.c *.cpp

echo.
echo.
echo not finished! check blow!
echo.
echo.
echo copy the specific UI_xxx file you want in sys_main\UI\ to sys_main\
echo.
echo execute sys_main\sys_main.ino to open Arduino IDE
echo.
echo.

cd MCU_Arduino

pause

