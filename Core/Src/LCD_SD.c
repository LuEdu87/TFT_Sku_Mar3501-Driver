#include "LCD_SD.h"


/*------------------------------------------------------------------------------------------------------
 * 						STATIC METHODS DEFINITION
 ------------------------------------------------------------------------------------------------------*/
static HAL_StatusTypeDef LCD_SD_SPIReceive_IT(SPI_HandleTypeDef *hspi,void (*WrPins)(uint8_t,uint8_t),uint8_t *pTxData, uint8_t *pRxData, uint32_t Size, uint32_t Timeout);
//static void LCD_SD_BMPtoRGB565(uint8_t BMPbyte);
static void LCD_SD_ReadFileData(LCD_SD_Handle_it *hLCD,uint32_t FileStartAddress,uint32_t AmountofBytes);
//static void	LCD_SD_ReadHeaderFile(LCD_SD_Handle_it *hLCD);
static uint16_t LCD_SD_FindFile(LCD_SD_Handle_it *hLCD,char FileName[],uint32_t SizeName);
static uint8_t LCD_SD_Mounting(LCD_SD_Handle_it *hLCD);
static void LCD_SD_SPISoftInit(LCD_SD_Handle_it *hLCD);
static void LCD_SD_SPIHwInit(SPI_HandleTypeDef *hLCD);
static void LCD_SD_SPIHwDeInit(SPI_HandleTypeDef* hspi);
static void LCD_SD_FindPartitionTable(LCD_SD_Handle_it *hLCD);
static void LCD_SD_FindFilesDirectory(LCD_SD_Handle_it *hLCD);
static void LCD_SD_SPISend(LCD_SD_Handle_it *hLCD, uint8_t *PtCmd, uint8_t *PtCmdAns,uint32_t Size);
static void LCD_SD_SPIReceive(LCD_SD_Handle_it *hLCD, uint8_t *PtData,uint32_t Size);
static void LCD_SD_SPIEmptyCycles(LCD_SD_Handle_it *hLCD);





/*
 * Function:
 * 	SPI protocol configuration
 */

static void LCD_SD_SPISoftInit(LCD_SD_Handle_it *hLCD)
{

	hLCD->hspi1.Instance = SPI1;
	hLCD->hspi1.Init.Mode = SPI_MODE_MASTER;
	hLCD->hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hLCD->hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hLCD->hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hLCD->hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hLCD->hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
	hLCD->hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128 ;
	hLCD->hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hLCD->hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hLCD->hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hLCD->hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hLCD->hspi1) != HAL_OK)
	{
	Error_Handler();
	}


}






/*
 * Function:
 * 	Hardware and clock configuration for SPI communication.
 */
static void LCD_SD_SPIHwInit(SPI_HandleTypeDef *hspi)
{

	HAL_SPI_MspInit(hspi);
}






/*
 * Function:
 * 	Disabling the SPI clock and pins.
 */
static void LCD_SD_SPIHwDeInit(SPI_HandleTypeDef* hspi)
{
  if(hspi->Instance==SPI1)
  {

    __HAL_RCC_SPI1_CLK_DISABLE();


    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

  }

}





/*
 * Function:
 * 	Function to read the data store inside of SD card..
 */

static void LCD_SD_ReadFileData(LCD_SD_Handle_it *hLCD,uint32_t FileStartAddress1,uint32_t AmountofBytes)
{
	uint32_t FileStartAddress= FileStartAddress1*hLCD->SDhandle.BytesPerSector;
	uint8_t tempTx=0;
	uint8_t CMD18[]	=	{SD_CMD18,(uint8_t)(FileStartAddress>>24),(uint8_t)(FileStartAddress>>16),(uint8_t)(FileStartAddress>>8),(uint8_t)FileStartAddress,SD_CRC_NO,0x00,0x00};//Buscamos la dirección 0 del SD
	uint8_t CMD12[]	=	{SD_CMD12,0x00,0x00,0x00,0x00,SD_CRC_NO,0x00,0x00};
	uint8_t ReceiveAns[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	memset(hLCD->SDhandle.ReceiveBytesSector,0,sizeof(hLCD->SDhandle.ReceiveBytesSector));

	HAL_Delay(100);
	LCD_SD_SPIEmptyCycles(hLCD);
	LCD_SD_SPISend(hLCD,(uint8_t*)CMD18,(uint8_t*)ReceiveAns, sizeof(CMD18));

	while(ReceiveAns[0] != 0xFE)
	{
		LCD_SD_SPIReceive(hLCD, ReceiveAns, 1);
	}

	ReceiveAns[0]=0x00;
	uint16_t CounterSize=0;
	LCD_SCREEN_WriteDataSingle(MemoryWriteCMD, Comando);
	do
	{
		LCD_SD_SPIReceive_IT(&hLCD->hspi1,LCD_SCREEN_WriteDataSingle,&tempTx, (uint8_t*)(hLCD->SDhandle.ReceiveBytesSector),hLCD->SDhandle.BytesPerSector+2,0xFF);
		while(ReceiveAns[0] != 0xFE)
		{
			LCD_SD_SPIReceive(hLCD, ReceiveAns, 1);
		}
		ReceiveAns[0]=0x00;
		CounterSize++;
	}while(CounterSize<=(hLCD->SDhandle.FileSectorsSize));


	ReceiveAns[0]=0x00;
	while(ReceiveAns[0] == 0x00)
	{

		LCD_SD_SPISend(hLCD,(uint8_t*)CMD12,(uint8_t*)ReceiveAns, sizeof(CMD12));
	}



}





/*
 * Function:
 * 	This function is for search the file that we want to read. This part will read the table of files and try
 *  to find it.
 */
static uint16_t LCD_SD_FindFile(LCD_SD_Handle_it *hLCD,char FileNameOut[],uint32_t SizeName)
{

	uint8_t	 EqualCounter=0,LoopCounter=0;
	hLCD->SDhandle.FileFound=0;
	char FileName[SizeName];
	strcpy(FileName,FileNameOut);

	for(int i=0;i<SizeName;i++)
	{
		if((FileName[i]>0x60)&&(FileName[i]<0x7B))	//if the value is between this range, is a low letter
		{
			FileName[i]=FileName[i]-32;		//This convert the low letter in CAPS using ASCII table.
		}
	}


#warning TODO: It needs a part where if the file doesnt  exist return a value.
	while(hLCD->SDhandle.FileFound!=1)
	{
		LCD_SD_READSector(hLCD, hLCD->SDhandle.FileDirectory+LoopCounter);
			//Here search the format of the file.
		for(int i=8;i<sizeof(hLCD->SDhandle.ReceiveBytesSector);i+=32)
		{
			if((hLCD->SDhandle.ReceiveBytesSector[i]==FileName[SizeName-3])&&(hLCD->SDhandle.ReceiveBytesSector[i+1]==FileName[SizeName-2])&&(hLCD->SDhandle.ReceiveBytesSector[i+2]==FileName[SizeName-1]))
			{

				for(int j=0;j<6;j++)
				{
					if((hLCD->SDhandle.ReceiveBytesSector[(i-8)]==FileName[0]))	//The first character has to be the same.
					{
						if((hLCD->SDhandle.ReceiveBytesSector[j+(i-8)]==FileName[j])||(hLCD->SDhandle.ReceiveBytesSector[j+(i-8)]==' '))
						{
							EqualCounter++;
						}
						else
						{
							EqualCounter=0;
							break;
						}
					}
					else
					{
						EqualCounter=0;
						break;
					}

				}
				if(EqualCounter==6)
				{
					if((hLCD->SDhandle.ReceiveBytesSector[i-1]==(uint8_t)FileName[SizeName-6])||((hLCD->SDhandle.ReceiveBytesSector[i-1]=='1'))||((hLCD->SDhandle.ReceiveBytesSector[i-1]==' '))) //el ultimo caracter de nombre puede ser un 1, otro caracter igual al nombre o un espacio si el nombre es corto
					{
						hLCD->SDhandle.FileSector = hLCD->SDhandle.FileDirectory+((((hLCD->SDhandle.ReceiveBytesSector[i+19]<<8)|(hLCD->SDhandle.ReceiveBytesSector[i+18]))-2)*(hLCD->SDhandle.ClusterSize));//SectorFileDirectory+(OffsetsDeClustersdelArchivo-2)*Cluster;
						hLCD->SDhandle.FileSize = (hLCD->SDhandle.ReceiveBytesSector[i+23]<<24)|(hLCD->SDhandle.ReceiveBytesSector[i+22]<<16)|(hLCD->SDhandle.ReceiveBytesSector[i+21]<<8)|(hLCD->SDhandle.ReceiveBytesSector[i+20]);
						hLCD->SDhandle.FileSectorsSize =hLCD->SDhandle.FileSize/hLCD->SDhandle.BytesPerSector;
						hLCD->SDhandle.FileFound=1;
						return 1;
					}
				}
			}
		}
		LoopCounter++;

	}

	return 0;
}





/*
 * Function:
 * 	Process to mount the SD card, using the steps in the SD card procotol for prepare it.
 * 	@for more information, you should read the SD card IDLE state in the reference manual.
 */
static uint8_t LCD_SD_Mounting(LCD_SD_Handle_it *hLCD)
{

	uint8_t CMD0[] 		= 	{SD_CMD0,0x00,0x00,0x00,0x00,SD_CRC_CMD0,0x00,0x00};
	uint8_t CMD8[] 		= 	{SD_CMD8,0x00,0x00,0x01,0xAA,SD_CRC_CMD8,0x00,0x00};
	uint8_t CMD16[] 	= 	{SD_CMD16,0x00,0x00,0x02,0x00,SD_CRC_NO,0x00,0x00};
	uint8_t CMD55[] 	= 	{SD_CMD55,0x00,0x00,0x00,0x00,SD_CRC_NO,0x00,0x00};
	uint8_t ACMD41[] 	= 	{SD_ACMD41,0x40,0x00,0x00,0x00,SD_CRC_NO,0x00,0x00};
	uint8_t CMD58[]		= 	{SD_CMD58,0x00,0x00,0x00,0x00,SD_CRC_NO,0x00,0x00};
	uint8_t CMD59[]		= 	{SD_CMD59,0x00,0x00,0x00,0x00,SD_CRC_NO,0x00,0x00};

	uint8_t ReceiveAns[8] = {0x0,0x0,0x0,0x0,0x0,0x0,0x00,0x00};



	while(ReceiveAns[7]!=0x01)
	{
		LCD_SD_SPIEmptyCycles(hLCD);
		LCD_SD_SPISend(hLCD,(uint8_t*)CMD0,(uint8_t*)ReceiveAns, sizeof(CMD0));
	}



	LCD_SD_SPIEmptyCycles(hLCD);
	LCD_SD_SPISend(hLCD,(uint8_t*)CMD8,(uint8_t*)ReceiveAns, sizeof(CMD8));
	LCD_SD_SPIReceive(hLCD, (uint8_t*)ReceiveAns, sizeof(ReceiveAns));


	LCD_SD_SPIEmptyCycles(hLCD);
	LCD_SD_SPISend(hLCD,(uint8_t*)CMD58,(uint8_t*)ReceiveAns, sizeof(CMD58));

	LCD_SD_SPIEmptyCycles(hLCD);
	LCD_SD_SPISend(hLCD,(uint8_t*)CMD59,(uint8_t*)ReceiveAns, sizeof(CMD59));

	while(ReceiveAns[7]!=0x00)
	{
		LCD_SD_SPIEmptyCycles(hLCD);
		LCD_SD_SPISend(hLCD,(uint8_t*)CMD55,(uint8_t*)ReceiveAns, sizeof(CMD55));
		LCD_SD_SPIEmptyCycles(hLCD);
		LCD_SD_SPISend(hLCD,(uint8_t*)ACMD41,(uint8_t*)ReceiveAns, sizeof(ACMD41));
	}


	hLCD->hspi1.Instance->CR1 &= ~(SPI_CR1_SPE);
	hLCD->hspi1.Instance->CR1 &= ~(0x07<<SPI_CR1_BR_Pos);
	hLCD->hspi1.Instance->CR1 |= (3<<SPI_CR1_BR_Pos);
	hLCD->hspi1.Instance->CR1 |= (SPI_CR1_SPE);
	LCD_SD_SPIEmptyCycles(hLCD);
	LCD_SD_SPISend(hLCD,(uint8_t*)CMD16,(uint8_t*)ReceiveAns, sizeof(CMD16));

	return ReceiveAns[1];
}





/*
 * Function:
 * 		As before, this function is to find the directory file. Where are all the files location.
 */
static void LCD_SD_FindFilesDirectory(LCD_SD_Handle_it *hLCD)
{

	uint16_t ReservedSector	=	0;
	uint32_t HiddenSector	=	0;
	uint32_t SectorPerFlat	=	0;

	uint32_t SectorAddress=hLCD->SDhandle.PartitionTable*512;	//Convertir Sector en dirección, ya que cada sector tiene 512 bytes.
	uint8_t CMD17[]={SD_CMD17,(uint8_t)(SectorAddress>>24),(uint8_t)(SectorAddress>>16),(uint8_t)(SectorAddress>>8),(uint8_t)SectorAddress,SD_CRC_NO,0x00,0x00};

	uint8_t ReceiveAns[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


	memset(hLCD->SDhandle.ReceiveBytesSector,0,sizeof(hLCD->SDhandle.ReceiveBytesSector));



	LCD_SD_SPIEmptyCycles(hLCD);
	LCD_SD_SPISend(hLCD,(uint8_t*)CMD17,(uint8_t*)ReceiveAns, sizeof(CMD17));

	while(ReceiveAns[0] != 0xFE)
	{
		LCD_SD_SPIReceive(hLCD, ReceiveAns, 1);
	}
	LCD_SD_SPIReceive(hLCD, (uint8_t*)hLCD->SDhandle.ReceiveBytesSector, sizeof(hLCD->SDhandle.ReceiveBytesSector));

	if((hLCD->SDhandle.ReceiveBytesSector[01]==0x58)&&(hLCD->SDhandle.ReceiveBytesSector[00]==0xEB))
	{
		HiddenSector 	= (hLCD->SDhandle.ReceiveBytesSector[0x1F]<<24) | (hLCD->SDhandle.ReceiveBytesSector[0x1E]<<16) | (hLCD->SDhandle.ReceiveBytesSector[0x1D]<<8) | (hLCD->SDhandle.ReceiveBytesSector[0x1C]);
		SectorPerFlat 	= (hLCD->SDhandle.ReceiveBytesSector[0x27]<<24) | (hLCD->SDhandle.ReceiveBytesSector[0x26]<<16) | (hLCD->SDhandle.ReceiveBytesSector[0x25]<<8) | (hLCD->SDhandle.ReceiveBytesSector[0x24]);
		ReservedSector 	= (hLCD->SDhandle.ReceiveBytesSector[0x0F]<<8)  | (hLCD->SDhandle.ReceiveBytesSector[0x0E]);
		hLCD->SDhandle.FileDirectory	=	HiddenSector + ReservedSector + (2*SectorPerFlat);
		hLCD->SDhandle.ClusterSize		= hLCD->SDhandle.ReceiveBytesSector[0x0D];
		hLCD->SDhandle.BytesPerSector	= (hLCD->SDhandle.ReceiveBytesSector[0x0C]<<8)|(hLCD->SDhandle.ReceiveBytesSector[0x0B]);
	}else
		hLCD->SDhandle.FileDirectory = 0;

}




/*
 * Function:
 * 	In this case the function will try to find the partition table, where are all the data of the sd card.
 */
static void LCD_SD_FindPartitionTable(LCD_SD_Handle_it *hLCD)
{
	uint8_t CMD17[]={SD_CMD17,0x00,0x00,0x00,0x00,SD_CRC_NO,0x00,0x00};
	uint8_t ReceiveAns[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	memset(hLCD->SDhandle.ReceiveBytesSector,0,sizeof(hLCD->SDhandle.ReceiveBytesSector));



	LCD_SD_SPIEmptyCycles(hLCD);
	LCD_SD_SPISend(hLCD,(uint8_t*)CMD17,(uint8_t*)ReceiveAns, sizeof(CMD17));

	while(ReceiveAns[0] != 0xFE)
	{
		LCD_SD_SPIReceive(hLCD, ReceiveAns, 1);
	}
	LCD_SD_SPIReceive(hLCD, (uint8_t*)hLCD->SDhandle.ReceiveBytesSector, sizeof(hLCD->SDhandle.ReceiveBytesSector));

	hLCD->SDhandle.PartitionTable = (hLCD->SDhandle.ReceiveBytesSector[0x1C7]<<8) | (hLCD->SDhandle.ReceiveBytesSector[0x1C6]);
}





/*
 * Function:
 * 	An easy way to send data through the SPI protocol to SD Reader.
 */

static void LCD_SD_SPISend(LCD_SD_Handle_it *hLCD, uint8_t *PtCmd, uint8_t *PtCmdAns,uint32_t Size)
{
	HAL_SPI_TransmitReceive(&hLCD->hspi1, PtCmd, PtCmdAns, Size, 0xFF);
}






/*
 * Function:
 * 	An easy way to receive data through the SPI protocol from SD Reader.
 */
static void LCD_SD_SPIReceive(LCD_SD_Handle_it *hLCD, uint8_t *PtData,uint32_t Size)
{
	HAL_SPI_Receive(&hLCD->hspi1, (uint8_t*)PtData, Size, 0xFF);
}



/*
 * Function:
 *   This is a simple empty cycles to make a delay between messages. Because it needs a delay between SPI messages.
 */
static void LCD_SD_SPIEmptyCycles(LCD_SD_Handle_it *hLCD)
{


	uint8_t Garbage[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	uint8_t AnsGarbage[8];

	for(int i =0;i<2;i++)
	{

		HAL_SPI_TransmitReceive(&hLCD->hspi1,(uint8_t*)Garbage,(uint8_t*)AnsGarbage, sizeof(Garbage), 0xFF);

	}


}



/*
 * Function:
 *	In this case I recycled a HAL process of SPI protocol, and modify it, to write the pins in the screen.
 */
static HAL_StatusTypeDef LCD_SD_SPIReceive_IT(SPI_HandleTypeDef *hspi,void (*WrPins)(uint8_t,uint8_t),uint8_t *pTxData, uint8_t *pRxData, uint32_t Size, uint32_t Timeout)
{
  uint16_t             initial_TxXferCount;
  uint32_t             tmp_mode;
  HAL_SPI_StateTypeDef tmp_state;
  uint32_t             tickstart;

  //static uint8_t		StartFrstSector=0;

  /* Variable used to alternate Rx and Tx during transfer */
  uint32_t             txallowed = 1U;
  HAL_StatusTypeDef    errorcode = HAL_OK;

  /* Check Direction parameter */
  assert_param(IS_SPI_DIRECTION_2LINES(hspi->Init.Direction));

  /* Process Locked */
  __HAL_LOCK(hspi);

  /* Init tickstart for timeout management*/
  tickstart = HAL_GetTick();

  /* Init temporary variables */
  tmp_state           = hspi->State;
  tmp_mode            = hspi->Init.Mode;
  initial_TxXferCount = Size;

  if (!((tmp_state == HAL_SPI_STATE_READY) || \
		((tmp_mode == SPI_MODE_MASTER) && (hspi->Init.Direction == SPI_DIRECTION_2LINES) && (tmp_state == HAL_SPI_STATE_BUSY_RX))))
  {
	errorcode = HAL_BUSY;
	goto error;
  }

  if ((pTxData == NULL) || (pRxData == NULL) || (Size == 0U))
  {
	errorcode = HAL_ERROR;
	goto error;
  }

  /* Don't overwrite in case of HAL_SPI_STATE_BUSY_RX */
  if (hspi->State != HAL_SPI_STATE_BUSY_RX)
  {
	hspi->State = HAL_SPI_STATE_BUSY_TX_RX;
  }

  /* Set the transaction information */
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
  hspi->pRxBuffPtr  = (uint8_t *)pRxData;
  hspi->RxXferCount = Size;
  hspi->RxXferSize  = Size;
  hspi->pTxBuffPtr  = NULL;//(uint8_t *)pTxData;
  hspi->TxXferCount = Size;
  hspi->TxXferSize  = Size;

  /*Init field not used in handle to zero */
  hspi->RxISR       = NULL;
  hspi->TxISR       = NULL;

  /* Check if the SPI is already enabled */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
	/* Enable SPI peripheral */
	__HAL_SPI_ENABLE(hspi);
  }

  /* Transmit and Receive data in 16 Bit mode */
  if (hspi->Init.DataSize == SPI_DATASIZE_16BIT)
  {
	if ((hspi->Init.Mode == SPI_MODE_SLAVE) || (initial_TxXferCount == 0x01U))
	{
	  hspi->Instance->DR = *((uint16_t *)hspi->pTxBuffPtr);
	  hspi->pTxBuffPtr += sizeof(uint16_t);
	  hspi->TxXferCount--;
	}
	while ((hspi->TxXferCount > 0U) || (hspi->RxXferCount > 0U))
	{
	  /* Check TXE flag */
	  if ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE)) && (hspi->TxXferCount > 0U) && (txallowed == 1U))
	  {
		hspi->Instance->DR = *((uint16_t *)hspi->pTxBuffPtr);
		hspi->pTxBuffPtr += sizeof(uint16_t);
		hspi->TxXferCount--;
		/* Next Data is a reception (Rx). Tx not allowed */
		txallowed = 0U;

	  }

	  /* Check RXNE flag */
	  if ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)) && (hspi->RxXferCount > 0U))
	  {
		*((uint16_t *)hspi->pRxBuffPtr) = (uint16_t)hspi->Instance->DR;
		hspi->pRxBuffPtr += sizeof(uint16_t);
		hspi->RxXferCount--;
		/* Next Data is a Transmission (Tx). Tx is allowed */
		txallowed = 1U;
	  }
	  if (((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY))
	  {
		errorcode = HAL_TIMEOUT;
		goto error;
	  }
	}
  }
  /* Transmit and Receive data in 8 Bit mode */
  else
  {
	if ((hspi->Init.Mode == SPI_MODE_SLAVE) || (initial_TxXferCount == 0x01U))
	{
	  *((__IO uint8_t *)&hspi->Instance->DR) = (*hspi->pTxBuffPtr);
	  hspi->pTxBuffPtr += 0;
	  hspi->TxXferCount--;
	}
	while ((hspi->TxXferCount > 0U) || (hspi->RxXferCount > 0U))
	{
	  /* Check TXE flag */
	  if ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE)) && (hspi->TxXferCount > 0U) && (txallowed == 1U))
	  {
		*(__IO uint8_t *)&hspi->Instance->DR = (*hspi->pTxBuffPtr);
		//hspi->pTxBuffPtr++;
		hspi->TxXferCount--;
		/* Next Data is a reception (Rx). Tx not allowed */
		txallowed = 0U;

	  }

	  /* Wait until RXNE flag is reset */
	  if ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)) && (hspi->RxXferCount > 0U))
	  {
		(*(uint8_t *)hspi->pRxBuffPtr) = hspi->Instance->DR;

		if(hspi->RxXferCount>2)
		{
			WrPins(((uint8_t)hspi->Instance->DR),Data);
		}


		hspi->RxXferCount--;

		/* Next Data is a Transmission (Tx). Tx is allowed */
		txallowed = 1U;
	  }
	}
  }


  /* Clear overrun flag in 2 Lines communication mode because received is not read */
  if (hspi->Init.Direction == SPI_DIRECTION_2LINES)
  {
	__HAL_SPI_CLEAR_OVRFLAG(hspi);
  }

error :
  hspi->State = HAL_SPI_STATE_READY;
  __HAL_UNLOCK(hspi);
  return errorcode;
}






void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	  if(hspi->Instance==SPI1)
	  {

	    __HAL_RCC_SPI1_CLK_ENABLE();

	    __HAL_RCC_GPIOA_CLK_ENABLE();
	    __HAL_RCC_GPIOB_CLK_ENABLE();

	    GPIO_InitStruct.Pin = SPI_SD_SCK|SPI_SD_MISO|SPI_SD_MOSI;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
	    HAL_GPIO_Init(SPI_SD_GPIO, &GPIO_InitStruct);


	    GPIO_InitStruct.Pin = SPI_SD_SS;
	    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	    HAL_GPIO_Init(SPI_SD_GPIO_SS, &GPIO_InitStruct);


	  }
}





/*
 * Function:
 * 	Combination of previous function to Init the SD card
 */
void LCD_SD_Init(LCD_SD_Handle_it *hLCD)
{
	LCD_SD_SPISoftInit(hLCD);
	LCD_SD_SPIHwInit(&hLCD->hspi1);
	HAL_GPIO_WritePin(SPI_SD_GPIO_SS, SPI_SD_SS, GPIO_PIN_RESET);
}




/*
 * Function:
 * 	Combination of previous function to mount the SD card on the ucontroller.
 */
uint8_t LCD_SD_Mount(LCD_SD_Handle_it *hLCD)
{
	LCD_SD_Init(hLCD);
	LCD_SD_Mounting(hLCD);
	LCD_SD_FindPartitionTable(hLCD);
	LCD_SD_FindFilesDirectory(hLCD);



	return 0;
}


/*
 * Function:
 * 	UNUSED.
 */
void LCD_SD_Begin(LCD_SD_Handle_it *hLCD)
{

}




/*
 * Function:
 * 	Here we can search for the file in the SD card.
 */
uint8_t LCD_SD_FileOpen(LCD_SD_Handle_it *hLCD,char FileName[])
{
	HAL_Delay(50);
	if(LCD_SD_FindFile(hLCD,FileName,strlen(FileName))&1)
	{

		//LCD_SD_ReadHeaderFile(hLCD);
		LCD_SD_ReadFileData(hLCD,hLCD->SDhandle.FileSector,hLCD->SDhandle.FileSize);

		return 1;
	}else
	{
		return 0;
	}

}


/*
 * Function:
 * 	This function search the sector 0 and after that try to find the partition table. When it find it, look
 * 	in the file's directory.
 */

void LCD_SD_READSector(LCD_SD_Handle_it *hLCD,uint32_t SectorNumber)
{
		uint32_t SectorAddress= SectorNumber*hLCD->SDhandle.BytesPerSector;
		uint8_t CMD17[]={SD_CMD17,(uint8_t)(SectorAddress>>24),(uint8_t)(SectorAddress>>16),(uint8_t)(SectorAddress>>8),(uint8_t)SectorAddress,SD_CRC_NO,0x00,0x00};//Buscamos la dirección 0 del SD
		uint8_t ReceiveAns[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
		memset(hLCD->SDhandle.ReceiveBytesSector,0,sizeof(hLCD->SDhandle.ReceiveBytesSector));


		LCD_SD_SPIEmptyCycles(hLCD);

		LCD_SD_SPISend(hLCD,(uint8_t*)CMD17,(uint8_t*)ReceiveAns, sizeof(CMD17));

		while(ReceiveAns[0] != 0xFE)
		{
			LCD_SD_SPIReceive(hLCD,(uint8_t*)ReceiveAns, 1);

		}

		LCD_SD_SPIReceive(hLCD, (uint8_t*)hLCD->SDhandle.ReceiveBytesSector, sizeof(hLCD->SDhandle.ReceiveBytesSector));



}
