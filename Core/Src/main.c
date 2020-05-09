/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD_SD.h"
#include "LCD_SCREEN.h"
#include "LCD_TOUCHSCREEN.h"
//extern const unsigned char Kurumi[307209];
//extern const unsigned char Kurumi_Conv_Izq_a_Der[307209];
//extern const unsigned char Kurumi_Toalla[307209];
//extern const unsigned char Kurumi_Escor[307209];
//extern const unsigned char Kurumi_cortinas[307209];
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define NoCompilar	0
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */





/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
//ADC_HandleTypeDef hadc1;

//SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
LCD_SCREEN_Handle_t hlcd;
LCD_SD_Handle_it hsd;
LCD_Touchscreen_it htch;


char *FileSD[]={"Cortinas_Kurumi.bin",
				"Toalla_Kurumi.bin",
				"Cuerdas_Kurumi.bin",
				"Escor_Kurumi.bin",
				"Sofa_Kurumi.bin",
				"School_Kurumi.bin",
				"Clock_Kurumi.bin",
				"Shh_Kurumi.bin",
				"Ero_Kurumi.bin",
				"Nurse_kurumi.bin"
				};







/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
//static void MX_ADC1_Init(void);
//static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
//void PintarKurumi(uint8_t *Picture,uint32_t Largo);
//void DibujarTest(uint16_t Xo,uint16_t  Yo,char Letters[],uint8_t size);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint16_t Valor=0;
	int8_t i=0;
	uint16_t Almacen=0;



  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
//  MX_ADC1_Init();
  //MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  LCD_SCREEN_Init(hlcd);


  LCD_Touch_ADCInit(&htch);
  LCD_Touch_Calibration(&htch);


  LCD_SCREEN_Coordenates(0, 0, MaxWidth, MaxHigh);
  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  /* USER CODE END 2 */






	LCD_SD_Mount(&hsd);
	LCD_SD_Begin(&hsd);
	Valor=sizeof(FileSD)/sizeof(FileSD[0]);


	while(1)
	{

		Almacen = 0;
		LCD_SD_FileOpen(&hsd, FileSD[i]);
		while(Almacen<6)
		{
			Almacen=LCD_Touch_CoorX(&htch);
		}

		if(Almacen>250)
		{
			i++;
			if(i>=Valor)i=0;
		}else
		{
			i--;

			if(i<0)
			{
				i=Valor-1;
			}
		}



		//i++;
		//HAL_Delay(100);
	}







/*
	LCD_SCREEN_PaintCharacters(0, 0, "Tiempo en Segundos ", Violeta, 2);

	while(1)
	{
		while(TestNumeros<60)
		{
			LCD_SCREEN_PaintVariable(0, 30, TestNumeros, Violeta, 3,COMA);
			TestNumeros+=0.01;
			TestNumeros++;
			HAL_Delay(0.1);
		}

		while(TestNumeros>1)
		{
			LCD_SCREEN_PaintVariable(0, 30, TestNumeros, Violeta, 3,COMA);
			TestNumeros-=0.01;
			TestNumeros--;
			HAL_Delay(0.1);
		}
	}

*/



	//LCD_Coordenates(0, 0, 479,319);
	//PintarKurumi((uint8_t*)Kurumi_cortinas,sizeof(Kurumi_cortinas));

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */


	//LCD_SD_Init(&hlcd);

	//HAL_GPIO_WritePin(D10_GPIOx, D10_Pin_No, GPIO_PIN_RESET);


	//LCD_SD_SearchFileDirectory(&hlcd);
	//LCD_SD_READSector(&hlcd,(uint32_t)hlcd.SDhandle.PartitionTable);	//NO OLVIDAR CORREGIR ESTE FALLO DE DIRECCION
//	while(1)
//	{
//		LCD_SD_Mount(&hlcd);
//		if(i>(Valor-1))
//		{
//			i=0;
//		}
//		LCD_SD_FileOpen(&hlcd, FileSD[i]);
//		i++;
//	}

	//LCD_SD_FileOpen(&hlcd, FileSD1);






#ifndef NoCompilar
#define 0
	LCD_Coordenates(0, 0, 479,319);
	PintarKurumi((uint8_t*)Kurumi_cortinas,sizeof(Kurumi_cortinas));
	LCD_InitTouch(&hlcd);
	while(1){

		if(LCD_Painting(&hlcd)==ENABLE)
		{
			if(FlagCalib==0)
			{
				HAL_Delay(1000);
				LCD_TouchCalibration(&hlcd);
				FlagCalib=1;
			}
			LCD_Coordenates(LCD_TouchCoorX(&hlcd), LCD_TouchCoorY(&hlcd), LCD_TouchCoorX(&hlcd)+5, LCD_TouchCoorY(&hlcd)+5);
			DibujarTest();
		}

	}
#endif



	//LCD_SD_Send(&hlcd, SEND_OP_COND);




	while (1) ;
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 120;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
//static void MX_ADC1_Init(void)
//{
//
//  /* USER CODE BEGIN ADC1_Init 0 */
//
//  /* USER CODE END ADC1_Init 0 */
//
//  ADC_ChannelConfTypeDef sConfig = {0};
//
//  /* USER CODE BEGIN ADC1_Init 1 */
//
//  /* USER CODE END ADC1_Init 1 */
//  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
//  */
//  hadc1.Instance = ADC1;
//  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
//  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
//  hadc1.Init.ScanConvMode = DISABLE;
//  hadc1.Init.ContinuousConvMode = DISABLE;
//  hadc1.Init.DiscontinuousConvMode = DISABLE;
//  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
//  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//  hadc1.Init.NbrOfConversion = 1;
//  hadc1.Init.DMAContinuousRequests = DISABLE;
//  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
//  if (HAL_ADC_Init(&hadc1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
//  */
//  sConfig.Channel = ADC_CHANNEL_4;
//  sConfig.Rank = 1;
//  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
//  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN ADC1_Init 2 */
//
//  /* USER CODE END ADC1_Init 2 */
//
//}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
//static void MX_SPI1_Init(void)
//{
//
//  /* USER CODE BEGIN SPI1_Init 0 */
//////
//  /* USER CODE END SPI1_Init 0 */
//
//  /* USER CODE BEGIN SPI1_Init 1 */
//////
//  /* USER CODE END SPI1_Init 1 */
//  /* SPI1 parameter configuration*/
//  hspi1.Instance = SPI1;
//  hspi1.Init.Mode = SPI_MODE_MASTER;
//  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
//  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
//  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
//  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
//  hspi1.Init.NSS = SPI_NSS_SOFT;
//  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
//  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
//  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
//  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//  hspi1.Init.CRCPolynomial = 10;
//  if (HAL_SPI_Init(&hspi1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN SPI1_Init 2 */
//////
//  /* USER CODE END SPI1_Init 2 */
//
//}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
void DibujarTest(uint16_t Xo,uint16_t  Yo,char Letters[],uint8_t size)
{
	//uint8_t CntChrt;
	uint8_t Letra_a[]={0x00,0x41,0x41,0x63,0x63,0x63,0x63,0x55,0x55,0x55,0x55,0x49,0x49,0x49,0x00};/*"m",0*/
	HAL_Delay(100);

	for(int i=0;i<15;i++)
	{
		for(int j=0;j<8;j++)
		{
			if((Letra_a[i]>>j)&1)
			{

				LCD_SCREEN_Coordenates(Xo+(j*size), Yo+(i*size), Xo+(j*size)+(size-1), Yo+(i*size)+(size-1));
				LCD_SCREEN_WriteDataSingle(MemoryWriteCMD, Comando);
				for(uint8_t k=0; k<(size*size);k++)
				{
					LCD_SCREEN_WriteDataSingle((uint8_t)(Violeta>>8), Data);
					LCD_SCREEN_WriteDataSingle((uint8_t)Violeta, Data);
				}
			}else
			{
				LCD_SCREEN_Coordenates(Xo+(j*size), Yo+(i*size), Xo+(j*size)+(size-1), Yo+(i*size)+(size-1));
				LCD_SCREEN_WriteDataSingle(MemoryWriteCMD, Comando);
				for(uint8_t k=0; k<(size*size);k++)
				{
					LCD_SCREEN_WriteDataSingle((uint8_t)0xFF, Data);
					LCD_SCREEN_WriteDataSingle((uint8_t)0xFF, Data);
				}
			}
		}
	}


}




//void PintarKurumi(uint8_t *Picture,uint32_t Largo)
//{
//	//int SizeColor;
//
//	//SizeColor =sizeof(*Picture)/sizeof(*Picture[0]);
//	HAL_Delay(1000);
//	//LCD_WriteData((int*)Picture, Largo);
//
//
//	LCD_WriteDataSingle(0x2C, Comando);
//
//	for(int i=0;i<Largo;i++)
//	{
//		LCD_WriteDataSingle(Picture[i],Data);
//
//	}
//	LCD_WriteDataSingle(0x00, Comando);
//
//}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
