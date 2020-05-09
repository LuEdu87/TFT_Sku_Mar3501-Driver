

#ifndef INC_LCD_TOUCHSCREEN_H_
#define INC_LCD_TOUCHSCREEN_H_

#include "main.h"
#include "stm32f4xx_hal.h"
#include "LCD_SCREEN.h"
#include <stdio.h>


#define RST_GPIOx		GPIOC
#define RST_Pin_No		GPIO_PIN_1

#define CS_GPIOx		GPIOB
#define CS_Pin_No		GPIO_PIN_0

#define RS_GPIOx		GPIOA
#define RS_Pin_No		GPIO_PIN_4

#define WR_GPIOx		GPIOA
#define WR_Pin_No		GPIO_PIN_1

#define RD_GPIOx		GPIOA
#define RD_Pin_No		GPIO_PIN_0



#define D0_GPIOx		GPIOA
#define D0_Pin_No		GPIO_PIN_9

#define D1_GPIOx		GPIOC
#define D1_Pin_No		GPIO_PIN_7



/*
 * Determinada que pines activar como analogicos
 * para leer la coordenada X o Y
 */

#define CoorX					1
#define CoorY					2



/*
 * Coordenadas (X0,Y0) para el 20% 50% y 80% que usaré para la calibracion
 * de la pantalla.
 */

#define Calib_X0		50
#define Calib_X1		240
#define Calib_X2		384

#define Calib_Y0		64
#define Calib_Y1		313
#define Calib_Y2		127





typedef struct{

	GPIO_InitTypeDef hLCD_GPIOx;
	ADC_HandleTypeDef hadc1;
	ADC_ChannelConfTypeDef sConfig;

	float 		A,B,C,D,E,F;
	uint16_t 	ADC_X0,ADC_X1,ADC_X2,ADC_Y0,ADC_Y1,ADC_Y2;


}LCD_Touchscreen_it;


void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void LCD_Touch_Calibration(LCD_Touchscreen_it *hLCD);
void LCD_Touch_Init(LCD_Touchscreen_it *hLCD);
void LCD_Touch_ADCInit(LCD_Touchscreen_it *hLCD);
uint32_t LCD_Touch_ReadX(LCD_Touchscreen_it *hLCD);
uint32_t LCD_Touch_ReadY(LCD_Touchscreen_it *hLCD);
uint32_t LCD_Touch_CoorX(LCD_Touchscreen_it *hLCD);
uint32_t LCD_Touch_CoorY(LCD_Touchscreen_it *hLCD);




#endif /* INC_LCD_TOUCHSCREEN_H_ */
