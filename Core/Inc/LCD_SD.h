#ifndef INC_LCD_SD_H_
#define INC_LCD_SD_H_

#include "main.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stm32f4xx_hal_spi.h>
#include "LCD_SCREEN.h"



/*------------------------------------------------------------------------------------------
 * 								DEFINES
 ------------------------------------------------------------------------------------------*/


/*
 * LCD SD CARD HARDWARE PINS DEFINITION.
 * -This section is for assign each pin of the SPI SD_READER (MISO,MOSI,SCK,SS) to Ucontroller pins.
 *
 */

#define SPI_SD_GPIO	GPIOA
#define SPI_SD_SCK	GPIO_PIN_5
#define SPI_SD_MISO	GPIO_PIN_6
#define SPI_SD_MOSI	GPIO_PIN_7

#define SPI_SD_GPIO_SS	GPIOB
#define SPI_SD_SS		GPIO_PIN_6



/*
 * Commands for the SD procotol. for more information, go to the reference manual of the SD card protocol.
 */
#define SD_CMD0			(0x40+0)	//GO_IDLE_STATE
#define SD_CRC_CMD0		(0x95)		//Crc para cmd0
#define SD_CMD2			(0x40+2)
#define SD_CMD6			(0x40+6)	//SWITCH_FUNC
#define SD_CMD8			(0x40+8)	//??
#define SD_CRC_CMD8		(0x87)		//CRC_CMD8
#define SD_CMD12		(0x40+12)	//Detener COmunicacion
#define SD_ACMD13		(0x40+13)
#define SD_CMD16	   	(0x40+16)
#define SD_CRC_CMD16	(0x15)
#define SD_CMD17		(0x40+17)
#define SD_CMD18		(0x40+18)
#define SD_ACMD41		(0x40+41)
#define SD_CMD55		(0x40+55)
#define SD_CMD58		(0x40+58)
#define SD_CMD59		(0x40+59)
#define SD_CRC_NO		(0x01)



//MASTER BOOT Sector address.
#define SECTOR_0_MASTERBOOT		(0x00)




//Conditions for colors convert(Temporary Disabled)
#define RGB_BLUE		0
#define RGB_GREEN		1
#define RGB_RED			2


//SD word type.
#define Comando			1
#define Data			2






/*------------------------------------------------------------------------------------------------------
 * 									STRUCTS DEFINITION
 ------------------------------------------------------------------------------------------------------*/


/*
 * struct to store the parameters of the SD card and File parameters.
 */
typedef struct{

	uint8_t		ReceiveBytesSector[514];	//Store of the 512 bytes of sector + 2Bytes CRC
	uint32_t	PartitionTable;				//Sector where is the partition table.
	uint32_t	FileDirectory;				//Table of Files location.
	uint8_t		ClusterSize;				//Cluster Size.
	uint16_t	BytesPerSector;				//Bytes per sector
	uint32_t	FileOffsetPosition;			//First byte File
	uint32_t	FileSize;					//Size of the file in Bytes.
	uint32_t	FileSector;					//Sector where is located the header of the file we want.
	uint16_t	FileSectorsSize;			//Amount of sector that contain the file(Size_file/512)
	uint8_t		FileFound;					//Flag to activate when the file is located.

}SD_Handle_it;



/*
 * Struct to handle the SD communication.
 */
typedef struct{

	SD_Handle_it SDhandle;
	SPI_HandleTypeDef hspi1;

}LCD_SD_Handle_it;





/*------------------------------------------------------------------------------------------------------
 * 									METHODS
 ------------------------------------------------------------------------------------------------------*/
void LCD_SD_Init(LCD_SD_Handle_it *hLCD);
void LCD_SD_READSector(LCD_SD_Handle_it *hLCD,uint32_t SectorNumber);
uint8_t LCD_SD_Mount(LCD_SD_Handle_it *hLCD);
void LCD_SD_Begin(LCD_SD_Handle_it *hLCD);
uint8_t LCD_SD_FileOpen(LCD_SD_Handle_it *hLCD,char FileName[]);
void LCD_SD_SPItoScreen_IT(LCD_SD_Handle_it *hspi);


#endif
