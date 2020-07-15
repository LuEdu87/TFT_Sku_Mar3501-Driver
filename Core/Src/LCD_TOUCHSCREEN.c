
#include "LCD_TOUCHSCREEN.h"




/*------------------------------------------------------------------------------------------------------
 * 						STATIC METHODS DEFINITION
 ------------------------------------------------------------------------------------------------------*/
static uint8_t LCD_Touch_Painting(LCD_Touchscreen_it *hLCD);
static void LCD_Touch_ADCSwConfig(LCD_Touchscreen_it *hLCD);
static void LCD_Touch_PinsOff(LCD_Touchscreen_it *hLCD);
static void LCD_Touch_PinsConf(LCD_Touchscreen_it hLCD,uint8_t XoY);




/*
 * Function:
 *  Here you can use to paint the screen while it is being touched.
 *  This is for drawing the screen using the pencil or the fingers.
 */

static uint8_t LCD_Touch_Painting(LCD_Touchscreen_it *hLCD)
{
	uint32_t CoordX,CoordY;//,ScreenCoordX,ScreenCoordY;

	CoordX= LCD_Touch_ReadX(hLCD);
	CoordY= LCD_Touch_ReadY(hLCD);
	if((CoordX>=100)&&(CoordY>=100))
	{

		return ENABLE;
	}else
	{
		return DISABLE;
	}



}









/*
 * Function:
 *	ADC hardware configuration
 */
static void LCD_Touch_ADCSwConfig(LCD_Touchscreen_it *hLCD)
{
	hLCD->hadc1.Instance = ADC1;
	hLCD->hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
	hLCD->hadc1.Init.Resolution = ADC_RESOLUTION_10B;
	hLCD->hadc1.Init.ScanConvMode = DISABLE;
	hLCD->hadc1.Init.ContinuousConvMode = DISABLE;
	hLCD->hadc1.Init.DiscontinuousConvMode = DISABLE;
	hLCD->hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hLCD->hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hLCD->hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hLCD->hadc1.Init.NbrOfConversion = 1;
	hLCD->hadc1.Init.DMAContinuousRequests = DISABLE;
	hLCD->hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hLCD->hadc1) != HAL_OK)
	{
	Error_Handler();
	}
	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	*/
	hLCD->sConfig.Channel = ADC_CHANNEL_4;
	hLCD->sConfig.Rank = 1;
	hLCD->sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hLCD->hadc1, & hLCD->sConfig) != HAL_OK)
	{
	Error_Handler();
	}
}




/*
 * Function:
 *	This is to turn the LCD screen pins normal, because the SCREEN and TOUCHSCREEN share pins. So if you wan
 *	to use touchscreen and Screen, you must change pins configurations between them.
 */
static void LCD_Touch_PinsOff(LCD_Touchscreen_it *hLCD)
{


	hLCD->hLCD_GPIOx.Pull = GPIO_PULLDOWN;
	hLCD->hLCD_GPIOx.Mode = GPIO_MODE_OUTPUT_PP;
	hLCD->hLCD_GPIOx.Speed= GPIO_SPEED_FAST;

	hLCD->hLCD_GPIOx.Pin = RS_Pin_No;
	HAL_GPIO_Init(RS_GPIOx, &hLCD->hLCD_GPIOx);

	hLCD->hLCD_GPIOx.Pin = D0_Pin_No;
	HAL_GPIO_Init(D0_GPIOx, &hLCD->hLCD_GPIOx);

	hLCD->hLCD_GPIOx.Pin = CS_Pin_No;
	HAL_GPIO_Init(CS_GPIOx, &hLCD->hLCD_GPIOx);

	hLCD->hLCD_GPIOx.Pin = D1_Pin_No;
	HAL_GPIO_Init(D1_GPIOx, &hLCD->hLCD_GPIOx);

	HAL_GPIO_WritePin(CS_GPIOx, CS_Pin_No, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RS_GPIOx, RS_Pin_No, GPIO_PIN_SET);
}



/*
 * Function:
 *
 */
static void LCD_Touch_PinsConf(LCD_Touchscreen_it hLCD,uint8_t XoY)
{
	HAL_GPIO_WritePin(RST_GPIOx, RST_Pin_No, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RD_GPIOx, RD_Pin_No, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WR_GPIOx, RD_Pin_No, GPIO_PIN_SET);

	if(XoY==CoorX)
	{

		hLCD.hLCD_GPIOx.Pull = GPIO_PULLDOWN;
		hLCD.hLCD_GPIOx.Mode = GPIO_MODE_ANALOG;
		hLCD.hLCD_GPIOx.Speed= GPIO_SPEED_FAST;

		hLCD.hLCD_GPIOx.Pin = RS_Pin_No;
		HAL_GPIO_Init(RS_GPIOx, &hLCD.hLCD_GPIOx);


		hLCD.hLCD_GPIOx.Mode = GPIO_MODE_INPUT;
		hLCD.hLCD_GPIOx.Pin = D0_Pin_No;
		HAL_GPIO_Init(D0_GPIOx, &hLCD.hLCD_GPIOx);


		hLCD.hLCD_GPIOx.Pull = GPIO_PULLDOWN;
		hLCD.hLCD_GPIOx.Mode = GPIO_MODE_OUTPUT_PP;



		hLCD.hLCD_GPIOx.Pin = CS_Pin_No;
		HAL_GPIO_Init(CS_GPIOx, &hLCD.hLCD_GPIOx);

		hLCD.hLCD_GPIOx.Pin = D1_Pin_No;
		HAL_GPIO_Init(D1_GPIOx, &hLCD.hLCD_GPIOx);

		HAL_GPIO_WritePin(D1_GPIOx, D1_Pin_No, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CS_GPIOx, CS_Pin_No, GPIO_PIN_SET);


	}else if(XoY==CoorY)
	{
		hLCD.hLCD_GPIOx.Pull = GPIO_PULLDOWN;
		hLCD.hLCD_GPIOx.Mode = GPIO_MODE_ANALOG;
		hLCD.hLCD_GPIOx.Speed= GPIO_SPEED_FAST;

		hLCD.hLCD_GPIOx.Pin = CS_Pin_No;
		HAL_GPIO_Init(CS_GPIOx, &hLCD.hLCD_GPIOx);


		hLCD.hLCD_GPIOx.Mode = GPIO_MODE_INPUT;
		hLCD.hLCD_GPIOx.Pin = D1_Pin_No;
		HAL_GPIO_Init(D1_GPIOx, &hLCD.hLCD_GPIOx);




		hLCD.hLCD_GPIOx.Pull = GPIO_PULLDOWN;
		hLCD.hLCD_GPIOx.Mode = GPIO_MODE_OUTPUT_PP;


		hLCD.hLCD_GPIOx.Pin = D0_Pin_No;
		HAL_GPIO_Init(D0_GPIOx, &hLCD.hLCD_GPIOx);

		hLCD.hLCD_GPIOx.Pin = RS_Pin_No;
		HAL_GPIO_Init(RS_GPIOx, &hLCD.hLCD_GPIOx);

		HAL_GPIO_WritePin(RS_GPIOx, RS_Pin_No, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(D0_GPIOx, D0_Pin_No, GPIO_PIN_SET);

	}

}



/*
 * Function:
 *
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc1)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
		if(hadc1->Instance==ADC1)
		{
		/* USER CODE BEGIN ADC1_MspInit 0 */

		/* USER CODE END ADC1_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_ADC1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**ADC1 GPIO Configuration
		PA4     ------> ADC1_IN4
		PB0     ------> ADC1_IN8
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_4;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		}
}




/*
 * Function:
 *  This process is to calibrate the touchscreen. Here I put 3 points in different places and take the ADC value
 *  with this I create an Matrix of value, and using Krammer to resolve the equation system I take the ratios
 *  for the calibration.
 */
void LCD_Touch_Calibration(LCD_Touchscreen_it *hLCD)
{
	float K;
	LCD_Touch_PinsOff(hLCD);


	//1-Draw the first point and wait
	LCD_SCREEN_Coordenates(Calib_X0-5, Calib_Y0-5, Calib_X0+5, Calib_Y0+5);
	LCD_SCREEN_PaintColor();

	while(!LCD_Touch_Painting(hLCD));	//Esperamos que guarde que se pulse la pantalla.

	while(LCD_Touch_Painting(hLCD))
	{

		hLCD->ADC_X0 = LCD_Touch_ReadX(hLCD);
		hLCD->ADC_Y0 = LCD_Touch_ReadY(hLCD);
		HAL_Delay(100);

	}


	//2-Draw the second point and wait
	LCD_SCREEN_Coordenates(Calib_X1-5, Calib_Y1-5, Calib_X1+5, Calib_Y1+5);
	LCD_SCREEN_PaintColor();

	while(!LCD_Touch_Painting(hLCD));	//Esperamos que guarde que se pulse la pantalla.
	while(LCD_Touch_Painting(hLCD))
	{
		hLCD->ADC_X1 = LCD_Touch_ReadX(hLCD);
		hLCD->ADC_Y1 = LCD_Touch_ReadY(hLCD);
		HAL_Delay(500);

	}

	//3-Draw the third point and wait
	LCD_SCREEN_Coordenates(Calib_X2-5, Calib_Y2-5, Calib_X2+5, Calib_Y2+5);
	LCD_SCREEN_PaintColor();
	while(!LCD_Touch_Painting(hLCD));	//Esperamos que guarde que se pulse la pantalla.
	while(LCD_Touch_Painting(hLCD))
	{
		hLCD->ADC_X2 = LCD_Touch_ReadX(hLCD);//
		hLCD->ADC_Y2 = LCD_Touch_ReadY(hLCD);
		HAL_Delay(500);
	}
	HAL_Delay(500);



	//Solve the equation.
	K=((hLCD->ADC_X0-hLCD->ADC_X2)*(hLCD->ADC_Y1-hLCD->ADC_Y2))-((hLCD->ADC_X1-hLCD->ADC_X2)*(hLCD->ADC_Y0-hLCD->ADC_Y2));

	hLCD->A =((hLCD->ADC_Y0*(Calib_X2-Calib_X1))+(hLCD->ADC_Y1*(Calib_X0-Calib_X2))+(hLCD->ADC_Y2*(Calib_X1-Calib_X0)))/K;

	hLCD->B = ((hLCD->ADC_X0*(Calib_X1-Calib_X2))+(hLCD->ADC_X1*(Calib_X2-Calib_X0))+(hLCD->ADC_X2*(Calib_X0-Calib_X1)))/K;

	hLCD->C = ((hLCD->ADC_Y1*((hLCD->ADC_X0*Calib_X2)-(hLCD->ADC_X2*Calib_X0)))+(hLCD->ADC_Y2*((hLCD->ADC_X1*Calib_X0)-(hLCD->ADC_X0*Calib_X1)))+(hLCD->ADC_Y0*((hLCD->ADC_X2*Calib_X1)-(hLCD->ADC_X1*Calib_X2))))/K;

	hLCD->D = ((hLCD->ADC_Y0*(Calib_Y2-Calib_Y1))+(hLCD->ADC_Y1*(Calib_Y0-Calib_Y2))+(hLCD->ADC_Y2*(Calib_Y1-Calib_Y0)))/K;

	hLCD->E = ((hLCD->ADC_X0*(Calib_Y1-Calib_Y2))+(hLCD->ADC_X1*(Calib_Y2-Calib_Y0))+(hLCD->ADC_X2*(Calib_Y0-Calib_Y1)))/K;

	hLCD->F = ((hLCD->ADC_Y0*((Calib_Y1*hLCD->ADC_X2)-(hLCD->ADC_X1*Calib_Y2)))+(hLCD->ADC_Y1*((hLCD->ADC_X0*Calib_Y2)-(hLCD->ADC_X2*Calib_Y0)))+(hLCD->ADC_Y2*((hLCD->ADC_X1*Calib_Y0)-(Calib_Y1*hLCD->ADC_X0))))/K;


}





void LCD_Touch_Init(LCD_Touchscreen_it *hLCD)
{

}




/*
 * Function:
 *  Activation of ADC for the analogic read.
 */
void LCD_Touch_ADCInit(LCD_Touchscreen_it *hLCD)
{
	LCD_Touch_ADCSwConfig(hLCD);
	HAL_ADC_MspInit(&hLCD->hadc1);

}




/*
 * Function:
 *	Read the analogical resistance of the X coordinate
 */
uint32_t LCD_Touch_ReadX(LCD_Touchscreen_it *hLCD)
{
	uint32_t ReadValue=0;
	LCD_Touch_PinsConf(*hLCD,CoorX);
	hLCD->sConfig.Channel = ADC_CHANNEL_4;
	HAL_ADC_ConfigChannel(&hLCD->hadc1, & hLCD->sConfig);


	for(int i=0;i<20;i++)
	{
		HAL_ADC_Start(&hLCD->hadc1);
		HAL_ADC_PollForConversion(&hLCD->hadc1, 1000);
		ReadValue += HAL_ADC_GetValue(&hLCD->hadc1);
		HAL_ADC_Stop(&hLCD->hadc1);

	}

	LCD_Touch_PinsOff(hLCD);

	ReadValue /=20;

	return ReadValue;
}




/*
 * Function:
 *	Read the analogical resistance of the Y coordinate
 */
uint32_t LCD_Touch_ReadY(LCD_Touchscreen_it *hLCD)
{
	uint32_t ReadValue=0;
	LCD_Touch_PinsConf(*hLCD,CoorY);
	hLCD->sConfig.Channel = ADC_CHANNEL_8;
	HAL_ADC_ConfigChannel(&hLCD->hadc1, & hLCD->sConfig);


	for(int i=0;i<20;i++)
	{
		HAL_ADC_Start(&hLCD->hadc1);
		HAL_ADC_PollForConversion(&hLCD->hadc1, 1000);
		ReadValue += HAL_ADC_GetValue(&hLCD->hadc1);
		HAL_ADC_Stop(&hLCD->hadc1);
	}


	LCD_Touch_PinsOff(hLCD);

	ReadValue /=20;

	return ReadValue;
}




/*
 * Function:
 *	Return the Coordinate X of the screen.
 */
uint32_t LCD_Touch_CoorX(LCD_Touchscreen_it *hLCD)
{
	uint32_t CoordenadaX,LecturaX,LecturaY;
	LecturaX = LCD_Touch_ReadX(hLCD);
	LecturaY = LCD_Touch_ReadY(hLCD);


	CoordenadaX = (hLCD->A)*LecturaX+(hLCD->B)*LecturaY+hLCD->C;
	return CoordenadaX;
}




/*
 * Function:
 *	Return the coordinate Y of the screen.
 */
uint32_t LCD_Touch_CoorY(LCD_Touchscreen_it *hLCD)
{
	uint32_t CoordenadaY,LecturaX,LecturaY;
	LecturaX = LCD_Touch_ReadX(hLCD);
	LecturaY = LCD_Touch_ReadY(hLCD);


	CoordenadaY = (hLCD->D)*LecturaX+(hLCD->E)*LecturaY+hLCD->F;
	return CoordenadaY;
}






