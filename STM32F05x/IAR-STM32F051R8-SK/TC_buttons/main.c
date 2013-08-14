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
 *    1. Date        : 20, June 2012
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-STM32F051R8-SK evaluation board. It shows
 * basic use of capacitive touch buttons, timer, LCD and interrupt controllers.
 *  Displays ID numbers of the touch buttons.
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
char str[9] = "B:     ";
Boolean pressed, hold;
Int32U buttons_state;
RCC_ClocksTypeDef RCC_Clocks;

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

  if(CB_Init())
  {
    return 1;
  }

  if(HD44780_OK != HD44780_PowerUpInit())
  {
    return 1;
  }

  HD44780_StrShow(1,1,(const HD44780_STRING_DEF *)str);

  hold = 0;
  while(1)
  {
    buttons_state = CB_GetState();
    pressed = !!buttons_state;
    if(!!(buttons_state & 1))
    {
      str[2] = '1';
    }
    else
    {
      str[2] = ' ';
    }
    if(!!(buttons_state & 2))
    {
      str[3] = '2';
    }
    else
    {
      str[3] = ' ';
    }
    if(!!(buttons_state & 4))
    {
      str[4] = '3';
    }
    else
    {
      str[4] = ' ';
    }
    if(!!(buttons_state & 8))
    {
      str[5] = '4';
    }
    else
    {
      str[5] = ' ';
    }
    if(!!(buttons_state & 16))
    {
      str[6] = '5';
    }
    else
    {
      str[6] = ' ';
    }
    if(!!(buttons_state & 32))
    {
      str[7] = '6';
    }
    else
    {
      str[7] = ' ';
    }
    if(pressed)
    {
      LEDOn(LED1);
    }
    else
    {
      LEDOff(LED1);
    }
    if(hold != buttons_state)
    {
      hold = buttons_state;
      HD44780_StrShow(1,1,(const HD44780_STRING_DEF *)str);
    }
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
