/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : Short Example, Program using the TIM peripheral.
*                      The purpose of this program is to configure the TIM in 
*                      OPM mode.
********************************************************************************
* History:
* 05/22/2007 : Version 1.2
* 05/24/2006 : Version 1.1
* 05/18/2006 : Version 1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "91x_lib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

TIM_InitTypeDef   TIM_InitStructure;
GPIO_InitTypeDef  GPIO_InitStructure;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{

  #ifdef DEBUG
    debug();
  #endif
  
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);        /*Use OSC as the default clock source*/
  SCU_PCLKDivisorConfig(SCU_PCLK_Div1);      /* ARM Peripheral bus clokdivisor = 1*/
  
/* TIM0 Deinitialization */
  SCU_APBPeriphClockConfig(__TIM01, ENABLE); /* Enable the clock for TIM0 and TIM1 */
  TIM_DeInit(TIM0);

  /* TIM2 Deinitialization */
  SCU_APBPeriphClockConfig(__TIM23, ENABLE); /* Enable the clock for TIM2 and TIM3 */
  TIM_DeInit(TIM2);                          /* TIM2 Deinitialization */


  SCU_APBPeriphClockConfig(__GPIO4, ENABLE); /* Enable the clock for GPIO4 */
  GPIO_DeInit(GPIO4);                        /* GPIO4 Deinitialization */

  SCU_APBPeriphClockConfig(__GPIO7, ENABLE); /* Enable the clock for GPIO7 */
  GPIO_DeInit(GPIO7);                        /* GPIO7 Deinitialization */

  /* GPIO Structure Initialization */
  GPIO_StructInit(&GPIO_InitStructure);

  /*GPIO4 configuration */
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 |GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt2;
  GPIO_Init(GPIO4,&GPIO_InitStructure);

  /* GPIO7 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Enable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1;
  GPIO_Init(GPIO7,&GPIO_InitStructure);


  /* TIM0 Structure Initialization */
  TIM_StructInit(&TIM_InitStructure);

  /* TIM0 Configuration in PWM Mode */
  TIM_InitStructure.TIM_Mode = TIM_PWM;
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB;
  TIM_InitStructure.TIM_Prescaler = 0xFF;
  TIM_InitStructure.TIM_Pulse_Level_1 = TIM_HIGH;
  TIM_InitStructure.TIM_Period_Level = TIM_LOW;
  TIM_InitStructure.TIM_Pulse_Length_1 = 0xBFF;
  TIM_InitStructure.TIM_Full_Period = 0xFFF;
  TIM_Init (TIM0, &TIM_InitStructure);

  /* Start the counter of TIM0 */
  TIM_CounterCmd(TIM0, TIM_START);

  /* TIM1 Structure Initialization */
  TIM_StructInit(&TIM_InitStructure);

  /* TIM1 Configuration in OPM Mode */
  TIM_InitStructure.TIM_Mode = TIM_OPM;
  TIM_InitStructure.TIM_OPM_INPUT_Edge = TIM_OPM_EDGE_RISING;
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB;
  TIM_InitStructure.TIM_Prescaler = 0xFF;
  TIM_InitStructure.TIM_Pulse_Level_1 = TIM_HIGH;
  TIM_InitStructure.TIM_Period_Level = TIM_LOW;
  TIM_InitStructure.TIM_Pulse_Length_1 = 0x3FF;
  TIM_Init (TIM2, &TIM_InitStructure);

  /* Start the counter of TIM1 */
  TIM_CounterCmd(TIM2, TIM_START);


  while(1);
}

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
