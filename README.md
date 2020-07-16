# TFT_Sku_Mar3501-Driver
This project is a driver design for the Sku Mar 3501, a LCD screen for Arduino that contain a touchscreen and SD card reader  to use with Stm32F446RE nucleo board. I've designed a basic driver for each feature.

Here you have the wikipage of the LCD with libraries and programs to work with it.
[LCD Sku Mar3501](http://www.lcdwiki.com/3.5inch_Arduino_Display-UNO)

Here the Nucleo Stm32F446RE
[STM32F446RE](https://www.st.com/en/microcontrollers-microprocessors/stm32f446re.html)

##Screen Features.
This LCD in the driver is programmed as RGB565, so for each pixel you need two bytes. This panel uses a parallel protocol of 8 bits (D0..D7).
For more information about the functionality, you can go to the wiki and download the reference manual of ili9488. That is the microcontroller inside of the screen.

NOTE: In this version I've only used the write process to paint the screen, as future improvement, I'll put a read process, to return configuration's features from the screen.

##SD card Reader.
The SD reader, I've used a 2Giga MicroSD to put some pictures and read it from the screen and show through the SPI protocol.
Actually, I've only designed the read from SD card, in the future, I'll add the write process, to store data.

NOTE1: I didn't use any library for the communication with the SD. So I'll have some problems compared with it. This is something that I will improve by the time.

NOTE: The SD card reader is limited to 2 Mhz. That is because this hardware is designed to work with Arduino, and the Arduino clock is over 16Mhz. So be careful, if you want to use more speed in the SPI protocol, you will have headache with it.

##Touchscreen.
The touchpad in this shield is really easy. You'll change some pin configuration ( in the code are described) and change it between Screen pins and analogical pins, to read the resistance of the panel.

NOTE: The problem with this panel ,is the screen and touchn share pins, so you can't put an interruption to break the Screen process. For example, if the screen is painting, the touch panel is disabled until the screen finish.
Probably in the future I would put an RTOS system to alternate between paint and touch.

