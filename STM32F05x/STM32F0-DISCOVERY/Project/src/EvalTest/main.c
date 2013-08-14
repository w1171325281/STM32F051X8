/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Tools development Team
* Date First Issued  : January 2012
* Description        : This code is used for MB1034 board test
********************************************************************************
* History:
**
*
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_conf.h"


/* Private typedef -----------------------------------------------------------*/

#define TEST_LSE_no

/* Private define ------------------------------------------------------------*/
#define  MS_DELAY_HSI (1000)
#define  SECOND_DELAY (1000*MS_DELAY_HSI)

// LEDs definition: LD3~LD4
#define  GPIO_LED_LD3   GPIO_Pin_9
#define  GPIO_LED_LD4   GPIO_Pin_8

#define GPIO_HIGH(a,b) 		a->BSRR = b
#define GPIO_LOW(a,b)		a->BRR = b
#define GPIO_TOGGLE(a,b) 	a->ODR ^= b 

#define  TURNONLD3   GPIO_HIGH(GPIOC, GPIO_LED_LD3)
#define  TURNOFFLD3  GPIO_LOW(GPIOC, GPIO_LED_LD3)
#define  TURNONLD4   GPIO_HIGH(GPIOC, GPIO_LED_LD4)
#define  TURNOFFLD4  GPIO_LOW(GPIOC, GPIO_LED_LD4)

#define  LSE_FAIL_FLAG  0x80
#define  LSE_PASS_FLAG  0x100

/* Private consts ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/



uint32_t TickValue=0;
/* Private variables ---------------------------------------------------------*/

/*
uint32_t LSE_Delay;
uint32_t BlinkSpeed, count, i;
uint32_t KeyState;
*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

//------------------------------------------------------------------------------
// Function Name  : LEDs_Init and User button
// Description    : LEDs initialization
//------------------------------------------------------------------------------
void Init_GPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  // Enable GPIOA, GPIOC clocks
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
  GPIO_Init(GPIOC, &GPIO_InitStructure);
   

  // Init User/Wakeup button B1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void TimingDelay_Decrement(void)
{
    TickValue--;  
}


//------------------------------------------------------------------------------
// Function Name : delay
// Description   : delay for some time in counts(rough)
// Input         : counts is how many counts of time to delay
//------------------------------------------------------------------------------
void delay(uint32_t counts)
{
  while(counts-- != 0)
    ;
}

//------------------------------------------------------------------------------
// Function Name : delay_ms
// Description   : delay for some time in ms unit(accurate)
// Input         : n_ms is how many ms of time to delay
//------------------------------------------------------------------------------
void delay_ms(uint32_t n_ms)
{
  // SysTick interrupt each 1000 Hz with HCLK equal to 32MHz
  // - 30 to compensate the overhead of this sub routine
  SysTick_Config(8000*PLL_MUL_X - 30);
  // Enable the SysTick Counter

  TickValue = n_ms;
  while(TickValue == n_ms)
    ;

  // SysTick interrupt each 1000 Hz with HCLK equal to 32MHz
  SysTick_Config(8000*PLL_MUL_X);
  while(TickValue != 0)
    ;


}

//------------------------------------------------------------------------------
// Function Name : delay_10us
// Description   : delay for some time in 10us unit(accurate)
// Input         : n_10us is how many 10us of time to delay
//------------------------------------------------------------------------------
void delay_10us(uint32_t n_10us)
{
  // Enable the SysTick Counter
  TickValue = n_10us;
  // SysTick interrupt each 10us with HCLK equal to 24MHz
  // - 30 to compensate the overhead of this sub routine
  SysTick_Config(80*PLL_MUL_X - 30);
  while(TickValue == n_10us)
    ;

  // SysTick interrupt each 10us with HCLK equal to 24MHz
  SysTick_Config(80 * PLL_MUL_X);
  while(TickValue != 0)
    ;

}

 
/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
  uint8_t i;
  uint32_t count ;
  uint32_t LSE_Delay;
  uint32_t BlinkSpeed;
  uint8_t KeyState; 

  #ifdef DEBUG
    debug();
  #endif

  
  // Initialize variables
  LSE_Delay = 0;
  BlinkSpeed = 6;
  KeyState = 0; // released

  
  // Init LEDs LD3&LD4, and user button
  Init_GPIO();


  // Wait till HSE is ready - HSE_RDY @ bit 17
  while((RCC->CR & (1L<<17)) == 0)
  {
    if((RCC->CR & (1<<17)) == 0)
      delay(1000);
    if((RCC->CR & (1<<17)) == 0)
      delay(SECOND_DELAY);
    if((RCC->CR & (1<<17)) == 0)
      // turn on blue LED LD4
      TURNONLD4;
    if((RCC->CR & (1<<17)) == 0)
      delay(SECOND_DELAY);
    if((RCC->CR & (1<<17)) == 0)
      // turn off blue LED LD4
      TURNOFFLD4;
  }
  
  TURNOFFLD4;
    // HSE passes
  
  // Flash no wait state
  *(uint32_t *)0x40022000 = 0x00;
  
  // for SILICON_CHIP configuration
  // PLLCLK = 8MHz * 3 = 24 MHz
  RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_2);
  
  // Enable PLL
  RCC_PLLCmd(ENABLE);
  
  
  // Wait till PLL is ready - PLL_RDY @ bit 25
  while((RCC->CR & (1L<<25)) == 0)
  {
    if((RCC->CR & (1<<25)) == 0)
      delay(1000);
    if(((RCC->CR) & (1L<<25)) == 0)
      delay(SECOND_DELAY);
    if((RCC->CR & (1<<25)) == 0)
      // turn on blue LED LD4
      TURNONLD4;
    if((RCC->CR & (1<<25)) == 0)
      delay(SECOND_DELAY);
    if((RCC->CR & (1<<25)) == 0)
      // turn off blue LED LD4
      TURNOFFLD4;
  }

  // Select PLL as system clock source
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  // Wait till PLL is used as system clock source
  while(RCC_GetSYSCLKSource() != 0x08)
    ;

  // Enable PWR clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  // Enable access to the backup register, so LSE can be enabled
  PWR_BackupAccessCmd(ENABLE);
  




#ifdef TEST_LSE
  // Enable LSE (Low Speed External Oscillation)
  RCC_LSEConfig(RCC_LSE_ON);

  // first loop, wait for press of B1 button
  while(1)
  {
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
    {
      delay_ms(50);
      if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
      {
        KeyState = 1; // pressed
        // Turn on Blue LED during 1s each time B1 is pressed
        TURNONLD4;
        delay_ms(1000);
        TURNOFFLD4;

        break;
      }
    }
    
    TURNONLD3;
    
    if(LSE_Delay & LSE_FAIL_FLAG)
      TURNONLD4;
    // delay_ms(1000);
    // delay 1s in below loop
    for(i=0; i<100; i++)
    {
      delay_ms(10);
      if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
        if(KeyState == 0)
          break;
    }
    
    if(i<100)
      continue;
    
    if(LSE_Delay < LSE_FAIL_FLAG)
      LSE_Delay += 10;
    TURNOFFLD3;
    TURNOFFLD4;
//    delay_ms(1000);
    // delay 1s in below loop
    
    for(i=0; i<100; i++)
    {
      delay_ms(10);
      if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
        if(KeyState == 0)
          break;
    }
    
    if(i<100)
      continue;
    
    if(LSE_Delay < LSE_FAIL_FLAG)
      LSE_Delay += 10;
    
    // LSE_FAIL_FLAG = 0x80
    if(LSE_Delay >= LSE_FAIL_FLAG)
      ;
    else if(LSE_Delay >= 60)
    {
      // check whether LSE is ready, with 6 seconds timeout
      if(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        // set flag: LSE FAIL
        LSE_Delay |= LSE_FAIL_FLAG;
      else
        // set flag: LSE PASS
        LSE_Delay |= LSE_PASS_FLAG;
      // Disable LSE
      RCC_LSEConfig(RCC_LSE_OFF);
    }
  }
#else
  LSE_Delay |= LSE_PASS_FLAG;  
#endif
  
  BlinkSpeed = 1;
  count = 0;
  
  // main loop
  while(1)
  {
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
    {
      if(KeyState == 0)
      {
        delay_ms(50);
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
        {
          KeyState = 1; // pressed
          // BlinkSpeed: 1 -> 2 -> 0, then re-cycle
          // Turn on LD4 Blue LED during 1s each time B1 is pressed
          TURNONLD4;
          delay_ms(1000);
          TURNOFFLD4;

          BlinkSpeed++;
          if(BlinkSpeed == 3)
            BlinkSpeed = 0;
        }
      }
    }
    else
    {
      if(KeyState == 1)
      {
        delay_ms(50);
        if(0 == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
        {
          KeyState = 0; // released
        }
      }
    }
    for(i=0; i<10; i++)
    {
      delay_ms(10);
      if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
        if(KeyState == 0)
          break;
    }
    if(i<10)
      continue;
   
    count++;
    
    if(LSE_Delay < 60)
      LSE_Delay += 1;
    // LSE_FAIL_FLAG = 0x80
    else if(LSE_Delay >= LSE_FAIL_FLAG)
      ;
    else if(LSE_Delay >= 60)
    {
      // check whether LSE is ready, with 6 seconds timeout
      if(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        // set flag: LSE FAIL
        LSE_Delay |= LSE_FAIL_FLAG;
      else
        // set flag: LSE PASS
        LSE_Delay |= LSE_PASS_FLAG;
      // Disable LSE
      RCC_LSEConfig(RCC_LSE_OFF);
    }
    if(BlinkSpeed == 2)
    {
      if(0 == (count % 2))
        TURNONLD3;
      else
        TURNOFFLD3;
    }
    else if(BlinkSpeed == 1)
    {
      if(2 == (count % 4))
        TURNONLD3;
      if(0 == (count % 4))
        TURNOFFLD3;
    }
    else
      TURNOFFLD3;
    if(LSE_Delay & LSE_FAIL_FLAG)
    {
      if(10 == (count % 20))
        TURNONLD4;
      if(0 == (count % 20))
        TURNOFFLD4;
    }
  }

}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
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
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
