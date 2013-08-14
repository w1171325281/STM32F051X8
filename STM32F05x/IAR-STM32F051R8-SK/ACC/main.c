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
 *    1. Date        : 1, March 2012
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-STM32F051R8-SK evaluation board. It shows
 * basic use of LCD and accelerometer drivers.
 *  The example show X and Y axeses on the LCD display
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
volatile Boolean Acc_flag = FALSE;
Int32U CriticalSecCntr;


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
  char buffer[9];
  Int16S READ_X, READ_Y, READ_Z;
  float X,Y;

  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

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

  Accl_Init();

  if(!Accl_Init())
  {
    HD44780_StrShow(1,1,"ACC fault");
    return 1;
  }

  while(1)
  {
    if(!Accl_Get(&READ_X,&READ_Y,&READ_Z))
    {
      HD44780_StrShow(1,1,"ACC fault");
      return 1;
    }
    X = ((Int16S)(READ_X ) * (2.0/2048));
    Y = ((Int16S)(READ_Y ) * (2.0/2048));

    sprintf(buffer,"%+.1f%+.1f",X,Y);
    HD44780_StrShow(1,1,(const HD44780_STRING_DEF *)buffer);
    Delay(50);
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
