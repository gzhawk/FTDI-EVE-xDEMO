copy sys_main\MCU_Arduino\*.* to sys_main\
copy sys_main\UI\UI.h to sys_main\
copy sys_main\UI\EVE\*.* to sys_main\, and change all the .c to .cpp
rename sys_main\sys_main.c to sys_main\sys_main.cpp

(prepare.bat may help you do above, and clean.bat can clean them)

copy the specific ui file in sys_main\UI\ to sys_main\
execute sys_main\sys_main.ino to open Arduino IDE

