
Some information you may like from ST:
	ST: AN3969 (stsw-stm32066)
	ST: AN3983 (stsw-stm32068)

1. Install ARM GCC: https://launchpad.net/gcc-arm-embedded

2. After successfual install, run below command to test if it works (you may add the related 'bin' path into the PATH of your enviornment valuable to make it executable at any path from MSDOS enviornment if you can not successful run arm-none-eabi-gcc):
    Start -> Run -> "cmd": arm-none-eabi-gcc -v

3. Install MAKE for windows: 
    http://gnuwin32.sourceforge.net/packages/make.htm
    download "Complete package, except sources", you would have both 'make' and 'dep'
	After successful install, you may have to add the 'make' path into the PATH in your enviornment valuable.

4. This example is using J-LINK EDU programmer and J-LINK commander application to program the Discovery board, so you need to have the cable and install J-LINK related software, then add the jlink.exe path into the PATH in your enviornment valuable.

5. Read this to know how to connect the programming cable (we use J-LINK):
	http://www.segger.com/admin/uploads/evalBoardDocs/AN00014_ConnectingJLinkToSTM32F407Discovery.pdf

6. the connection between STM32F4Discovery and FT800 CC Card as below:
    SCK : PA5
	MISO: PA6
	MOSI: PA7
	NSS : PD15
	PD  : PD13
	GND : select a proper GND on both STM32F4Discovery and CC card
    5v/3.3v: select the correct power 5v or 3.3v which fits your CC card (CC card can be powered either 5v or 3.3v)

7. you need to power on the STM32F4Discovery by connect CN1 with a USB cable between your laptop/PC, during the flash programming and demo running purpose

