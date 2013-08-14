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
 * basic use of a LCD driver and an UART.
 *  The example return received char back to terminal program (local echo) and
 * show it on the LCD display
 *
 *  Terminal configuration:
 *    Word Length = 8 Bits
 *    One Stop Bit
 *    No parity
 *    BaudRate = 115200
 *    flow control: None
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
USART_InitTypeDef USART_InitStructure;
char c, buffer[9];
int index = 0;

  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */

  /* Initialize LEDs, Key Button, LCD and COM port(USART) available on
     STM320518-EVAL board *****************************************************/
  LEDInit(LED1);
  LEDInit(LED2);
  LEDInit(LED3);
  LEDInit(LED4);

  /* USARTx configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  COMInit(COM1, &USART_InitStructure);

  if(HD44780_OK != HD44780_PowerUpInit())
  {
    return 1;
  }

  memset(buffer,0,sizeof(buffer));
  while(1)
  {
    c = getchar();
    putchar(c);
    buffer[index] = c;
    HD44780_StrShow(1,1,(const HD44780_STRING_DEF *)buffer);

    if(++index >= sizeof(buffer)-1)
    {
      memmove(buffer, buffer+1, sizeof(buffer)-1);
      --index;
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
