#ifndef INC_LCD_SD_H_
#define INC_LCD_SD_H_

#include "main.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stm32f4xx_hal_spi.h>
#include "LCD_SCREEN.h"



/*
 * Pines SPI
 */
/**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */

#define SPI_SD_GPIO	GPIOA
#define SPI_SD_SCK	GPIO_PIN_5
#define SPI_SD_MISO	GPIO_PIN_6
#define SPI_SD_MOSI	GPIO_PIN_7

#define SPI_SD_GPIO_SS	GPIOB
#define SPI_SD_SS		GPIO_PIN_6



/*
 * Comandos para protocolo de comunicación con tarjeta SD a través de SPI
 */
#define SD_CMD0			(0x40+0)	//GO_IDLE_STATE
#define SD_CRC_CMD0		(0x95)		//Crc para cmd0
#define SD_CMD2			(0x40+2)
#define SD_CMD6			(0x40+6)	//SWITCH_FUNC
#define SD_CMD8			(0x40+8)	//??
#define SD_CRC_CMD8		(0x87)	//CRC_CMD8
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

/*Sectores Importantes */
#define SECTOR_0_MASTERBOOT		(0x00)




//Condiciciones de división de bytes en el BMP
#define RGB_BLUE		0
#define RGB_GREEN		1
#define RGB_RED			2

#define Comando					1
#define Data					2


typedef struct{

	uint8_t		ReceiveBytesSector[514];	//Almacen de los 512 bytes del sector + 2Bytes CRC
	uint32_t	PartitionTable;				//Sector donde se encuentra la Tabla de particion
	uint32_t	FileDirectory;				//Tabla de Localización de ficheros
	uint8_t		ClusterSize;				//Tamaño del cluster
	uint16_t	BytesPerSector;				//Bytes por sector
	uint32_t	FileOffsetPosition;			//Primer byte del archivo
	uint32_t	FileSize;					//Tamaño del archivo en bytes
	uint32_t	FileSector;					//Sector donde está el encabezado del archivo que queremos
	uint16_t	FileSectorsSize;			//Cantidad de Sectores que contiene el archivo. Tamaño del archivo/512
	uint8_t		FileFound;				//Flag para detectar que el archivo se ha encontrado

}SD_Handle_it;




typedef struct{

	SD_Handle_it SDhandle;
	SPI_HandleTypeDef hspi1;

}LCD_SD_Handle_it;


void LCD_SD_Init(LCD_SD_Handle_it *hLCD);
void LCD_SD_READSector(LCD_SD_Handle_it *hLCD,uint32_t SectorNumber);
uint8_t LCD_SD_Mount(LCD_SD_Handle_it *hLCD);
void LCD_SD_Begin(LCD_SD_Handle_it *hLCD);
uint8_t LCD_SD_FileOpen(LCD_SD_Handle_it *hLCD,char FileName[]);
void LCD_SD_SPItoScreen_IT(LCD_SD_Handle_it *hspi);


#endif
