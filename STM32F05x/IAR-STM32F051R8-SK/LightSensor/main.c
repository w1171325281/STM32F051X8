/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 29, February 2012
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-STM32F051R8-SK evaluation board. It shows
 * basic use of an ADC and intreface to Light sensor (APDS-9007).
 *  The example dimming the LEDs depending of intensity of ambient light.
 *
 *    $Revision: 60080 $
 **************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "arm_comm.h"
#include "main.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
static __IO uint32_t TimingDelay;
uint32_t LightInt;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

/* Private function prototypes -----------------------------------------------*/
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)

void DelayResolution100us(Int32U Delay)
{
  for(volatile int i = 500; i;i--);
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
 RCC_ClocksTypeDef RCC_Clocks;
 int temp;

  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */
  /* SysTick end of count event each 5ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

  /* Initialize LEDs, Key Button, LCD and COM port(USART) available on
     STM320518-EVAL board *****************************************************/
  LEDInit(LED1);
  LEDInit(LED2);
  LEDInit(LED3);
  LEDInit(LED4);

  if(HD44780_OK != HD44780_PowerUpInit())
  {
    return 1;
  }

  Light_Init();
  Light_Ctrl(PWR_ON);

  while(1)
  {
    /* ADC1 regular Software Start Conv */
    ADC_StartOfConversion(LIGHT_ADC);
    while(!ADC_GetFlagStatus(LIGHT_ADC, ADC_FLAG_EOC));
    temp = ADC_GetConversionValue(LIGHT_ADC);
    ADC_ClearFlag(LIGHT_ADC, ADC_FLAG_EOC);
    LightInt = (temp*40/(4096-1));
    Delay(20);
  }
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(RS232_COM1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(RS232_COM1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}


GETCHAR_PROTOTYPE
{
  int ch;
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(RS232_COM1, USART_FLAG_RXNE) == RESET)
  {}

  ch = USART_ReceiveData(RS232_COM1);

  return ch;
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void LightInt_Handler(void)
{
  static uint32_t dim;
  static uint32_t count;
  Led_TypeDef dim_indx;

  LEDOff(LED1);
  LEDOff(LED2);
  LEDOff(LED3);
  LEDOff(LED4);

  if(LightInt < 10)
  {
    dim = LightInt;
    dim_indx = LED1;
  }
  else if(LightInt < 20)
  {
    LEDOn(LED1);
    dim = LightInt - 10;
    dim_indx = LED2;
  }
  else if(LightInt < 30)
  {
    LEDOn(LED1);
    LEDOn(LED2);
    dim = LightInt - 20;
    dim_indx = LED3;
  }
  else
  {
    LEDOn(LED1);
    LEDOn(LED2);
    LEDOn(LED3);
    dim = LightInt - 30;
    dim_indx = LED4;
  }

  if(count < dim)
  {
    LEDOn(dim_indx);
  }

  if(++count >= 10)
  {
    count = 0;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
