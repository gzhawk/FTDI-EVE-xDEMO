
1. After successfual install FT900 IDE, change the Makefile "TOOLDIR","LIB9XX","LIBINC" to the correct path you install IDE

2. If you don't want to use lib from FT900 IDE, you can change the Makefile by yourself and include the compile source files and link them to your code.

3. This example is using UMFTPD2A to program the MM900EVXX board:
	3.1 the UART port of UMFTPD2A is the 3th COM port: MM900EVXX-CN3-4(TX)/6(RX), UMFTPD1A-J2-4(TX)/5(RX)
	3.2 the one-wire programming pin: MM900EVXX-CN7-7(GND)/8(DBG), UMFTPD1A-CN2-DBG/GND


