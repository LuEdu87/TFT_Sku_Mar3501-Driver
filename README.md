# TFT_Sku_Mar3501-Driver
This project is a driver design for the Sku Mar 3501, a LCD screen for Arduino that contain a touchscreen and SD card reader  to use with Stm32F446RE nucleo board. I've designed a basic driver for each feature.

Here you have the wikipage of the LCD with libraries and programms to work with it.
[LCD Sku Mar3501](http://www.lcdwiki.com/3.5inch_Arduino_Display-UNO)

##Screen Features.
This LCD in the driver is programmed as RGB565, so for each pixel you need two bytes.

##SD card Reader.
The SD reader, I've used a 2Giga microSD to put some pictures and read it from the screen and show through the SPI protocol.
Actually I've only designed the read from SD card, in the future I'll add the write process, to store data.

NOTE: The SD card reader is limited to 2 Mhz. That is because this hardware is design to work with Arduino, and the arduino clock is over 16Mhz. So be careful,
if you want to use more speed in the SPI protocol, you will have headache with it.
