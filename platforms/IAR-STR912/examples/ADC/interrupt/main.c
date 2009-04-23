/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : Short Example, Program using the ADC peripheral.
*                      The purpose of this program is to convert an analog input
*                      using the ADC and use the result to modify the pulse of
*                      a PWM signal.
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
ADC_InitTypeDef   ADC_InitStructure;
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

  
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);         /*Use OSC as the default clock source*/
  SCU_PCLKDivisorConfig(SCU_PCLK_Div1);      /* ARM Peripheral bus clokdivisor = 1*/ 
    
  SCU_AHBPeriphClockConfig(__VIC, ENABLE);    /* Enable the clock for the VIC */
  VIC_DeInit();                               /* VIC Deinitialization */

  SCU_APBPeriphClockConfig(__ADC, ENABLE);    /* Enable the clock for the ADC */
  ADC_DeInit();                               /* ADC Deinitialization */

  SCU_APBPeriphClockConfig(__TIM01, ENABLE);  /* Enable the clock for TIM0 and TIM1 */
  TIM_DeInit(TIM0);                           /* TIM0 Deinitialization */

  SCU_APBPeriphClockConfig(__GPIO4, ENABLE);  /* Enable the clock for the GPIO4 */
  GPIO_DeInit(GPIO4);                         /* GPIO4 Deinitialization */

  SCU_APBPeriphClockConfig(__GPIO3, ENABLE);  /* Enable the clock for the GPIO3 */
  GPIO_DeInit(GPIO3);                         /* GPIO3 Deinitialization */

  /* Configure the GPIO4 pin 5 as analog input */
  GPIO_ANAPinConfig(GPIO_ANAChannel5, ENABLE);

  /* GPIO6 configuration (PWM on P3.0, pin 55) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Enable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt3;
  GPIO_Init(GPIO3,&GPIO_InitStructure);

  /* TIM0 Structure Initialization */
  TIM_StructInit(&TIM_InitStructure);

  /* TIM0 Configuration in PWM Mode */
  TIM_InitStructure.TIM_Mode = TIM_PWM;
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB;
  TIM_InitStructure.TIM_Prescaler = 0x0;
  TIM_InitStructure.TIM_Pulse_Level_1 = TIM_HIGH;
  TIM_InitStructure.TIM_Period_Level = TIM_LOW;
  TIM_InitStructure.TIM_Pulse_Length_1 = 0x200;
  TIM_InitStructure.TIM_Full_Period = 0x404;
  TIM_Init (TIM0, &TIM_InitStructure);

  /* Start the counter of TIM0 */
  TIM_CounterCmd(TIM0, TIM_START);

  /* ADC Structure Initialization */
  ADC_StructInit(&ADC_InitStructure);

  /* Configure the ADC in continuous mode conversion */
  ADC_InitStructure.ADC_Channel_5_Mode = ADC_NoThreshold_Conversion;
  ADC_InitStructure.ADC_Select_Channel = ADC_Channel_5;
  ADC_InitStructure.ADC_Scan_Mode = DISABLE;
  ADC_InitStructure.ADC_Conversion_Mode = ADC_Continuous_Mode;

  /* Enable the ADC */
  ADC_Cmd(ENABLE);

  /* Prescaler config */
  ADC_PrescalerConfig(0x0);

  /* Configure the ADC */
  ADC_Init(&ADC_InitStructure);

  /* VIC configuration */
  VIC_Config(ADC_ITLine, VIC_IRQ, 0);
  VIC_ITCmd(ADC_ITLine, ENABLE);

  /* ADC interrupt config */
  ADC_ITConfig(ADC_IT_ECV, ENABLE);

  /* Start the conversion */
  ADC_ConversionCmd(ADC_Conversion_Start);

  while(1);
}
/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
