/////////////////////////////////////////////////////////////
Aakanksha Budhiraja, ASU ID:- 1211210335
Kushal Anil Parmar, ASU ID:- 1207686255
/////////////////////////////////////////////////////////////
Assignment 3 - A SPI-based 1-wire Device Driver for LED Strip
==================================================================================================================

Part 1: A SPI device driver for LED strip

It containes 2 files:
kernemain.c
ws2812.c
------------------------

kernelmain.c 		-> Container the user program to send a pattern to the ws2812 driver.
						The user program gets an input from the user to light up "n" LEDs.
						It calls an IOCTL RESET to initialize the SPI device with its parameters. It sends 16 LED data encoded with 8bit GRB values each.
						The user program rotates the pattern and resends it in an infinite while loop by calling spi_led_write.

ws2812.c 			-> This is the driver to light up n LEDs. Write function receives a 24x16 bits long pattern.
						The driver encodes the pattern and initiates spi_async transfer to light up n LEDs.
						IOCTL function RESET to set the spi device parameters for correct mode of operation.
						Init function registers SPI device from the board info and match is produced when the driver is registered on the
						spi bus.


==================================================================================================
NOTE:
1) Reboot

2) Export your SDK's toolchain path to your current directory to avoid unrecognized command.
export PATH="/opt/iot-devkit/1.7.2/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux/:$PATH"

3) Connect IO pin MOSI (IO11) to "Data In" of the LED strip before starting the program.
=================================================================================================

Method:-

1) Change the following KDIR path to your source kernel path and the SROOT path to your sysroots path in the Makefile
KDIR:=/opt/iot-devkit/1.7.2/sysroots/i586-poky-linux/usr/src/kernel
SROOT=/opt/iot-devkit/1.7.2/sysroots/i586-poky-linux/

2) Run make command to compile both user app, the SPI led driver ws2812.
make

3) copy 2 files from current dir to board dir using the below command:
	i) 	ws2812.ko
	ii)	kernelmain

sudo scp <filename> root@<inet_address>:/home/root

4) Install the WS2812 driver 
insmod ws2812.ko

5) Run the user app
./kernelmain

6) $:/> Enter number of LEDs to light up
Enter a value from 1,2..16

7) Press Ctrl+C to stop user program

8) To test Part 2 remove the first driver
rmmod ws2812.ko

===============================================================================================================================================

Part 2: Accurate Delays in Linux

It containes 2 files:
kernelmain2.c
ws2812_bitbang.c
------------------------

kernelmain2.c 		-> Contains the user program to send a pattern to the ws2812 driver.
						The user program sends a pre-defined pattern to light up 16 LEDs data.
						The user program rotates the pattern and resends it in a while loop that calls the "spi_led_write" function.

ws2812_bitbang.c 	-> This is the driver to light up 16 LEDs. Write function receives a 16 byte long pattern.
						This driver accesses the port register of GPIO 12 pin using register access.
						The Driver performs bitbanging on gpio to meet the timing requirements to light up the LEDs.

===============================================================================================================================================
NOTE:
1) Reboot

2) Export your SDK's toolchain path to your current directory to avoid unrecognized command.
export PATH="/opt/iot-devkit/1.7.2/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux/:$PATH"

3) Connect IO pin 1 to "Data In" of the LED strip before starting the program.
================================================================================================================================================

Method:-

1) Change the following KDIR path to your source kernel path and the SROOT path to your sysroots path in the Makefile
KDIR:=/opt/iot-devkit/1.7.2/sysroots/i586-poky-linux/usr/src/kernel
SROOT=/opt/iot-devkit/1.7.2/sysroots/i586-poky-linux/

2) Run make command to compile both user app, the SPI led driver ws2812.
make

3) copy 2 files from current dir to board dir using the below command:
	i) 	ws2812_bitbang.ko
	ii)	kernelmain2

sudo scp <filename> root@<inet_address>:/home/root

4) Install the WS2812 driver 
insmod ws2812_bitbang.ko

5) Run the user app
./kernelmain2

6) The program will stop automatically after displaying 10 rotations.