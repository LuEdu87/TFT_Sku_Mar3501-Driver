#include "LCD_SCREEN.h"





/*
 * This array is for INIT parameters of the screen. Each line is a command and follow of parameters.
 * for more information go to Reference manual of ILI9486 SKU_MAR3501   .
 */
const short int InitCmd[86] = {
				  //CMD		//Parameters
				    0xF1,   0x36,0x04,0x00,0x3C,0X0F,0x8F
		   	   	,-1,0xF2,   0x18,0xA3,0x12,0x02,0x02,0x12,0xFF,0x10,0x00
				,-1,0xF8,   0x21,0x04
				,-1,0xF9,   0x00,0x08
				,-1,0xB4,   0x00
				,-1,0xC1,   0x41
				,-1,0xC5,   0x00,0x91,0x80,0x00
				,-1,0xE0,   0x0F,0x1F,0x1C,0x0C,0x0F,0x08,0x48,0x98,0x37,0x0A,0x13,0x04,0x11,0x0D,0x00
				,-1,0xE1,   0x0F,0x32,0x2E,0x0B,0x0D,0x05,0x47,0x75,0x37,0x06,0x10,0x03,0x24,0x20,0x00
				,-1,0x3A,   0x55
				,-1,0x11
				,-1,0xB1,   0xA0,0x1F
				,-1,0x36,   0x28
				,-1,0x29
};


//Characters matrix
extern const uint8_t characters[];


/*------------------------------------------------------------------------------------------------------
 * 						STATIC METHODS DEFINITION
 ------------------------------------------------------------------------------------------------------*/
static uint8_t LCD_SCREEN_MSBFound(uint32_t Value);
static char	LCD_SCREEN_IntToChar(uint8_t Number);
static uint8_t* LCD_SCREEN_FindCharacter(char Letter);
static void LCD_SCREEN_Wr(void);
static void LCD_SCREEN_ClkPinsEnable(void);
static void LCD_SCREEN_ConvertDatatoPins(uint8_t *datos);



/*
 * 	Function:
 * 		This function is for see where is the first bit in the position of the value(32 bits).
 */


static uint8_t LCD_SCREEN_MSBFound(uint32_t Value)
{
	uint8_t MSBPosition=0,DivisorMSB;


	for(int i=31;i>=0;i--)
	{

		if((Value>>i)&1)
		{
			MSBPosition=i;
			break;
		}
	}


	//Each position of the MSB for 10x multiplies, are at the same time multiplies of 3
	//I calculate the ratios betweeen the position of MSB and 3.


	if(Value>9)//This if for show the numbers between 10 and 15.
	{
		DivisorMSB=(MSBPosition)/3;

	}else
	{

		DivisorMSB=1;
	}

	return DivisorMSB;
}






/*
 * Funtion:
 * 	This convert a number in ASCII code.
 */
static char	LCD_SCREEN_IntToChar(uint8_t Number)
{
	char temp;

	temp=Number+'0';

	return temp;
}






/*
 * Function:
 * 	This is to connect the characters pixels matrix to the Letters you want to write in screen.
 *
 */
static uint8_t* LCD_SCREEN_FindCharacter(char Letter)
{
	switch(Letter)
	{
		case ' ':

			return (uint8_t *)(characters+15*0);
		break;

		case 'a':
			return (uint8_t*)(characters+15*1);
		break;

		case 'b':
			return (uint8_t*)(characters+15*2);
		break;

		case 'c':
			return (uint8_t*)(characters+15*3);
		break;

		case 'd':
			return (uint8_t*)(characters+15*4);
		break;

		case 'e':
			return (uint8_t*)(characters+15*5);
		break;

		case 'f':
			return (uint8_t*)(characters+15*6);
		break;

		case 'g':
			return (uint8_t*)(characters+15*7);
		break;

		case 'h':
			return (uint8_t*)(characters+15*8);
		break;

		case 'i':
			return (uint8_t*)(characters+15*9);
		break;

		case 'j':
			return (uint8_t*)(characters+15*10);
		break;

		case 'k':
			return (uint8_t*)(characters+15*11);
		break;

		case 'l':
			return (uint8_t*)(characters+15*12);
		break;

		case 'm':
			return (uint8_t*)(characters+15*13);
		break;

		case 'n':
			return (uint8_t*)(characters+15*14);
		break;

		case 'o':
			return (uint8_t*)(characters+15*15);
		break;

		case 'p':
			return (uint8_t*)(characters+15*16);
		break;

		case 'q':
			return (uint8_t*)(characters+15*17);
		break;

		case 'r':
			return (uint8_t*)(characters+15*18);
		break;

		case 's':
			return (uint8_t*)(characters+15*19);
		break;

		case 't':
			return (uint8_t*)(characters+15*20);
		break;

		case 'u':
			return (uint8_t*)(characters+15*21);
		break;

		case 'v':
			return (uint8_t*)(characters+15*22);
		break;

		case 'w':
			return (uint8_t*)(characters+15*23);
		break;

		case 'x':
			return (uint8_t*)(characters+15*24);
		break;

		case 'y':
			return (uint8_t*)(characters+15*25);
		break;

		case 'z':
			return (uint8_t*)(characters+15*26);
		break;


		/*-----------------------------------------
		 * CAPS
		 ------------------------------------------*/
		case 'A':
			return (uint8_t*)(characters+15*27);
		break;

		case 'B':
			return (uint8_t*)(characters+15*28);
		break;

		case 'C':
			return (uint8_t*)(characters+15*29);
		break;

		case 'D':
			return (uint8_t*)(characters+15*30);
		break;

		case 'E':
			return (uint8_t*)(characters+15*31);
		break;

		case 'F':
			return (uint8_t*)(characters+15*32);
		break;

		case 'G':
			return (uint8_t*)(characters+15*33);
		break;

		case 'H':
			return (uint8_t*)(characters+15*34);
		break;

		case 'I':
			return (uint8_t*)(characters+15*35);
		break;

		case 'J':
			return (uint8_t*)(characters+15*36);
		break;

		case 'K':
			return (uint8_t*)(characters+15*37);
		break;

		case 'L':
			return (uint8_t*)(characters+15*38);
		break;

		case 'M':
			return (uint8_t*)(characters+15*39);
		break;

		case 'N':
			return (uint8_t*)(characters+15*40);
		break;

		case 'O':
			return (uint8_t*)(characters+15*41);
		break;

		case 'P':
			return (uint8_t*)(characters+15*42);
		break;

		case 'Q':
			return (uint8_t*)(characters+15*43);
		break;

		case 'R':
			return (uint8_t*)(characters+15*44);
		break;

		case 'S':
			return (uint8_t*)(characters+15*45);
		break;

		case 'T':
			return (uint8_t*)(characters+15*46);
		break;

		case 'U':
			return (uint8_t*)(characters+15*47);
		break;

		case 'V':
			return (uint8_t*)(characters+15*48);
		break;

		case 'W':
			return (uint8_t*)(characters+15*49);
		break;

		case 'X':
			return (uint8_t*)(characters+15*50);
		break;

		case 'Y':
			return (uint8_t*)(characters+15*51);
		break;

		case 'Z':
			return (uint8_t*)(characters+15*52);
		break;
		/*
		 * NUMBERS 0-9
		 */
		 case '0':
			return (uint8_t*)(characters+15*53);
		break;

		case '1':
			return (uint8_t*)(characters+15*54);
		break;

		case '2':
			return (uint8_t*)(characters+15*55);
		break;

		case '3':
			return (uint8_t*)(characters+15*56);
		break;

		case '4':
			return (uint8_t*)(characters+15*57);
		break;

		case '5':
			return (uint8_t*)(characters+15*58);
		break;

		case '6':
			return (uint8_t*)(characters+15*59);
		break;

		case '7':
			return (uint8_t*)(characters+15*60);
		break;

		case '8':
			return (uint8_t*)(characters+15*61);
		break;

		case '9':
			return (uint8_t*)(characters+15*62);
		break;

		case ':':
			return (uint8_t*)(characters+15*63);
		break;

		case '.':
			return (uint8_t*)(characters+15*64);
		break;

		case ',':
			return (uint8_t*)(characters+15*65);
		break;

		case '-':
			return (uint8_t*)(characters+15*66);
		break;

		case '/':
			return (uint8_t*)(characters+15*67);
		break;

		case '+':
			return (uint8_t*)(characters+15*68);
		break;

		case '#':
			return (uint8_t*)(characters+15*69);
		break;

		default:
			return NULL;
		break;
	}
}






/*
 * Function:
 * 	Here we can make the toggling of the wr pin.
 */
static void  LCD_SCREEN_Wr(void)
{

	HAL_GPIO_TogglePin(WR_GPIOx, WR_Pin_No);

}










/*
 * Function:
 *  Activate the clocks of GPIO on the BUS.
 */

static void LCD_SCREEN_ClkPinsEnable(void)
{

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
}






/*
 * Function:
 *  This function is to put each bit of the param or CMD in the correct pin of the LCD.
 * @datos: value that you want to put in the pins (D0..D7)
 *
 *
 */
static void LCD_SCREEN_ConvertDatatoPins(uint8_t *datos)
{
	HAL_GPIO_WritePin(D0_GPIOx, D0_Pin_No, ((*datos>>0) & 1));
	HAL_GPIO_WritePin(D1_GPIOx, D1_Pin_No, ((*datos>>1) & 1));
	HAL_GPIO_WritePin(D2_GPIOx, D2_Pin_No, ((*datos>>2) & 1));
	HAL_GPIO_WritePin(D3_GPIOx, D3_Pin_No, ((*datos>>3) & 1));
	HAL_GPIO_WritePin(D4_GPIOx, D4_Pin_No, ((*datos>>4) & 1));
	HAL_GPIO_WritePin(D5_GPIOx, D5_Pin_No, ((*datos>>5) & 1));
	HAL_GPIO_WritePin(D6_GPIOx, D6_Pin_No, ((*datos>>6) & 1));
	HAL_GPIO_WritePin(D7_GPIOx, D7_Pin_No, ((*datos>>7) & 1));

}






/*
 * Function:
 * 	Hardware Pins Configuration for the SCREEN.
 *  *
 */
void LCD_SCREEN_HwInit(LCD_SCREEN_Handle_t hLCD)
{

	LCD_SCREEN_ClkPinsEnable();
	hLCD.hLCD_GPIOx.Pull = GPIO_PULLUP;
	hLCD.hLCD_GPIOx.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	hLCD.hLCD_GPIOx.Mode = GPIO_MODE_OUTPUT_PP;


	//RST
	hLCD.hLCD_GPIOx.Pin = RST_Pin_No;
	HAL_GPIO_Init(RST_GPIOx, &hLCD.hLCD_GPIOx);
	//CS
	hLCD.hLCD_GPIOx.Pin = CS_Pin_No;
	HAL_GPIO_Init(CS_GPIOx, &hLCD.hLCD_GPIOx);
	//RS
	hLCD.hLCD_GPIOx.Pin = RS_Pin_No;
	HAL_GPIO_Init(RS_GPIOx, &hLCD.hLCD_GPIOx);
	//WR
	hLCD.hLCD_GPIOx.Pin = WR_Pin_No;
	HAL_GPIO_Init(WR_GPIOx, &hLCD.hLCD_GPIOx);
	//RD
	hLCD.hLCD_GPIOx.Pin = RD_Pin_No;
	HAL_GPIO_Init(RD_GPIOx, &hLCD.hLCD_GPIOx);



	hLCD.hLCD_GPIOx.Pin = D0_Pin_No;
	HAL_GPIO_Init(D0_GPIOx, &hLCD.hLCD_GPIOx);

	hLCD.hLCD_GPIOx.Pin = D1_Pin_No;
	HAL_GPIO_Init(D1_GPIOx, &hLCD.hLCD_GPIOx);

	hLCD.hLCD_GPIOx.Pin = D2_Pin_No;
	HAL_GPIO_Init(D2_GPIOx, &hLCD.hLCD_GPIOx);

	hLCD.hLCD_GPIOx.Pin = D3_Pin_No;
	HAL_GPIO_Init(D3_GPIOx, &hLCD.hLCD_GPIOx);

	hLCD.hLCD_GPIOx.Pin = D4_Pin_No;
	HAL_GPIO_Init(D4_GPIOx, &hLCD.hLCD_GPIOx);

	hLCD.hLCD_GPIOx.Pin = D5_Pin_No;
	HAL_GPIO_Init(D5_GPIOx, &hLCD.hLCD_GPIOx);

	hLCD.hLCD_GPIOx.Pin = D6_Pin_No;
	HAL_GPIO_Init(D6_GPIOx, &hLCD.hLCD_GPIOx);

	hLCD.hLCD_GPIOx.Pin = D7_Pin_No;
	HAL_GPIO_Init(D7_GPIOx, &hLCD.hLCD_GPIOx);

	//SS

	hLCD.hLCD_GPIOx.Mode = GPIO_MODE_OUTPUT_PP;
	hLCD.hLCD_GPIOx.Pin = D10_Pin_No;
	HAL_GPIO_Init(D10_GPIOx, &hLCD.hLCD_GPIOx);
}






/*
 * Function:
 *   Setting the CS pin of the screen.
 */
void  LCD_SCREEN_Enable(void)
{

	HAL_GPIO_WritePin(CS_GPIOx, CS_Pin_No, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(CS_GPIOx, CS_Pin_No, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RS_GPIOx, RS_Pin_No, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WR_GPIOx, WR_Pin_No, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RD_GPIOx, RD_Pin_No, GPIO_PIN_SET);

}






/*
 * Function:
 * 	Initialization of the hardware and Initial parameters of the screen.
 *
 */
void LCD_SCREEN_Init(LCD_SCREEN_Handle_t hLCD)
{
	LCD_SCREEN_HwInit(hLCD);

	LCD_SCREEN_Reset();	//Hardware Reset
	LCD_SCREEN_Enable();//Activation of CS(LOW level)


	//SCREEN Initial Parameters Writing.
	HAL_Delay(100);
	LCD_SCREEN_WriteData((short int*)InitCmd,sizeof(InitCmd));
	HAL_Delay(100);
}





/*
 * Function:
 * 	Deactivate of the screen(CS High)
 */
void  LCD_SCREEN_Disable(void)
{

	HAL_GPIO_WritePin(CS_GPIOx, CS_Pin_No, GPIO_PIN_RESET );

}






/*
 * Function:
 * 	Hardware reset of the screen.
 */
void  LCD_SCREEN_Reset(void)
{

	HAL_GPIO_WritePin(RST_GPIOx, RST_Pin_No, GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(RST_GPIOx, RST_Pin_No, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS_GPIOx, CS_Pin_No, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RS_GPIOx, RS_Pin_No, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WR_GPIOx, WR_Pin_No, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RD_GPIOx, RD_Pin_No, GPIO_PIN_SET);
}






/*
 * Function:
 *  In this function the goal is to send an array of CMD and parameters. When a parameters end and start a
 *  a new CMD, it's necessary to put a -1 between them. With this, the program knows that the next
 *  number is a CMD and followed for parameters.
 *
 */
void  LCD_SCREEN_WriteData(short int *data,uint32_t lenght)
{
	uint32_t Counter;
	Counter=lenght;

	while(Counter >0)
	{
		HAL_GPIO_WritePin(RS_GPIOx, RS_Pin_No, GPIO_PIN_RESET);
		LCD_SCREEN_Wr();
		LCD_SCREEN_ConvertDatatoPins((uint8_t*)data);
		LCD_SCREEN_Wr();
		HAL_GPIO_WritePin(RS_GPIOx, RS_Pin_No, GPIO_PIN_SET);
		Counter--;
		data++;
		while(Counter > 0)
		{
			if(*data==-1)
			{
				data++;
				Counter--;
				break;
			}
			LCD_SCREEN_Wr();
			LCD_SCREEN_ConvertDatatoPins((uint8_t*)data);
			LCD_SCREEN_Wr();
			data++;
			Counter--;

		}
	}
}






/*
 * Function:
 *   This is a function for send only one CMD or data.
 */
void  LCD_SCREEN_WriteDataSingle(uint8_t data,uint8_t CmdorData)
{
	if(CmdorData==Comando)
	{
		HAL_GPIO_WritePin(RS_GPIOx, RS_Pin_No, GPIO_PIN_RESET);
		LCD_SCREEN_Wr();
		LCD_SCREEN_ConvertDatatoPins(&data);
		LCD_SCREEN_Wr();
		HAL_GPIO_WritePin(RS_GPIOx, RS_Pin_No, GPIO_PIN_SET);

	}else
	{
		LCD_SCREEN_Wr();
		LCD_SCREEN_ConvertDatatoPins(&data);
		LCD_SCREEN_Wr();
	}
}






/*
 * Function:
 *   Here we can put the coordinates for the crop and the size(HighxWidth) of where we want to paint,
 *   and the size of it.
 *
 */
void LCD_SCREEN_Coordenates(uint16_t Xo,uint16_t Yo,uint16_t Xe,uint16_t Ye)
{
	LCD_SCREEN_WriteDataSingle(0x2B, Comando);
	LCD_SCREEN_WriteDataSingle((uint8_t)(Yo>>8),Data);
	LCD_SCREEN_WriteDataSingle((uint8_t)Yo,Data);
	LCD_SCREEN_WriteDataSingle((uint8_t)(Ye>>8),Data);
	LCD_SCREEN_WriteDataSingle((uint8_t)Ye,Data);

	LCD_SCREEN_WriteDataSingle(0x2A, Comando);
	LCD_SCREEN_WriteDataSingle((uint8_t)(Xo>>8),Data);
	LCD_SCREEN_WriteDataSingle((uint8_t)Xo,Data);
	LCD_SCREEN_WriteDataSingle((uint8_t)(Xe>>8),Data);
	LCD_SCREEN_WriteDataSingle((uint8_t)Xe,Data);
}






/*
 * Function:
 *   This function is for paint some color in the coordinates that it was defined previously(LCD_SCREEN_Coordenates)
 *   @NOTE:It's not finished yet, the number 100 of the loop, is temporary.
 */
void LCD_SCREEN_PaintColor(void)
{
	LCD_SCREEN_WriteDataSingle(0x2C, Comando);
	for(int i =0;i<100;i++)
	{
		LCD_SCREEN_WriteDataSingle((uint8_t)(Violeta>>8), Data);
		LCD_SCREEN_WriteDataSingle((uint8_t)Violeta, Data);
	}
}










/*
 * Function:
 *   In this function, the goal is to paint in a specific area. It's a combination of the 2 previous
 *   function. Here you put the initial coordinates, the final coordinates and the color .
 */
void LCD_SCREEN_PaintPixels(uint16_t Xo, uint16_t Yo, uint16_t Xf, uint16_t Yf, uint16_t Colours)
{
	LCD_SCREEN_Coordenates(Xo, Yo, Xf, Yf);
	uint16_t size= (Xf-Xo+1)*(Yf-Yo+1);

	LCD_SCREEN_WriteDataSingle(MemoryWriteCMD, Comando);
	for(uint16_t k=0; k<size;k++)
	{
		LCD_SCREEN_WriteDataSingle((uint8_t)(Colours>>8), Data);
		LCD_SCREEN_WriteDataSingle((uint8_t)Colours, Data);
	}
}






/*
 * Function:
 *   With this you can put numbers/letters on the screen. Beside the coordinates and color.
 */
void LCD_SCREEN_PaintCharacters(uint16_t CoordX,uint16_t CoordY,char Letter[],uint16_t Colours, uint8_t size)
{
	uint16_t CounterCharac;
	uint8_t*  LetterPosition;

	CounterCharac = strlen(Letter);
	uint8_t	counter=0,i,j;


	while(counter<CounterCharac)
	{
		LetterPosition = LCD_SCREEN_FindCharacter(Letter[counter]);


		for(i=0;i<15;i++)
		{
			for(j=0;j<8;j++)
			{
				if((*(LetterPosition+i)>>j)&1)
				{
					LCD_SCREEN_PaintPixels(CoordX+(j*size), CoordY+(i*size), CoordX+(j*size)+(size-1), CoordY+(i*size)+(size-1), Colours);
				}else
				{
					LCD_SCREEN_PaintPixels(CoordX+(j*size), CoordY+(i*size), CoordX+(j*size)+(size-1), CoordY+(i*size)+(size-1), Blanco);
				}
			}
		}

		counter++;
		CoordX+=(j*size);
		if(CoordX>MaxWidth)
		{
			CoordX=0;
			CoordY+=(i*size);
		}
	}
}






/*
 * Function:
 *   The same as PaintCharacters, but using a variable as input.
 *   @NOTE: if the variable is a float, it's necessary to put COMA and if it's not, NO_COMA
 *   @this is important because the program need to know if the "Measure" value is one or other for the
 *   @correct representation.
 *
 */
void LCD_SCREEN_PaintVariable(uint16_t CoordX,uint16_t CoordY,float Measure,uint16_t Colours, uint8_t Size,uint8_t ComaOrNoComa)
{
	uint8_t DivisorMSB;
	uint32_t Millions,Decimals = 0;
	char Valor[10]={0,0,0,0,0,0,0,0,0,0};
	unsigned int Value=0,i;


	if(ComaOrNoComa==NO_COMA)
	{
		Value = Measure;

	}else if(ComaOrNoComa==COMA)
	{

		Value = Measure;
		Decimals = (Measure-Value)*100;	//We take the 2 bigger decimals
	}

	DivisorMSB=LCD_SCREEN_MSBFound(Value);
	Millions=pow(10,DivisorMSB);

	//2.Take the ratios and search it individually.
	//According the position of the MSB, you can know if the number is 10000,1000,100,etc
	for(i=0;i<(DivisorMSB+1);i++)	//
	{
		Valor[i]=LCD_SCREEN_IntToChar(Value/Millions);
		Value %=Millions;
		Millions/=10;
	}




	if(ComaOrNoComa==COMA)
	{
		DivisorMSB=LCD_SCREEN_MSBFound(Decimals);
		Valor[i]='.';
		Valor[i+1]=LCD_SCREEN_IntToChar(Decimals/10);
		Decimals %=10;
		Valor[i+2]=LCD_SCREEN_IntToChar(Decimals);
	}

	LCD_SCREEN_PaintCharacters(CoordX, CoordY, Valor, Violeta, Size);
}



