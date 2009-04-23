/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file contains the software implementation for the
*                      MC software library verification.
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
#include "91x_lib.h"

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 MC_InitTypeDef MC_InitStructure;
 GPIO_InitTypeDef GPIO_InitStructure;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main()
{

#ifdef DEBUG
    debug();
#endif

  SCU_MCLKSourceConfig(SCU_MCLK_OSC);    /*Use OSC as the default clock source*/ 
  SCU_PCLKDivisorConfig(SCU_PCLK_Div1); /* ARM Peripheral bus clokdivisor = 1*/
  
/* MC Clock enable */
  SCU_APBPeriphClockConfig( __MC, ENABLE);



/* GPIO6 Clock enable */
  SCU_APBPeriphClockConfig(__GPIO6, ENABLE);

/* GPIO Configuration */
  GPIO_DeInit(GPIO6);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3 | 
  GPIO_Pin_4 | GPIO_Pin_5 ;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate=GPIO_OutputAlt3;
  GPIO_Init(GPIO6, &GPIO_InitStructure);


/* MC deinitialization */  
  MC_DeInit();

/* MC Configuration: The MC is running in Hardware complementary operating mode
   to provide three complementary output signals with respective duty cycle: 
   50%, 25% and 12.5%. */  
    
/* Channel U configuration */
  MC_InitStructure.MC_OperatingMode = MC_HardwareOperating_Mode;
  MC_InitStructure.MC_TachoMode = MC_TachoContinuous_Mode;
  MC_InitStructure.MC_TachoEvent_Mode = MC_TachoEvent_Hardware_Mode;
  MC_InitStructure.MC_TachoPrescaler = 0x00;
  MC_InitStructure.MC_TachoPeriod = 0xFF;
  MC_InitStructure.MC_TachoPolarity = MC_TachoEventEdge_Falling;
  MC_InitStructure.MC_Prescaler = 0x02;
  MC_InitStructure.MC_PWMMode = MC_PWMClassical_Mode;
  MC_InitStructure.MC_Complementary = MC_Complementary_Enable;
  MC_InitStructure.MC_Period = 0x3FF;
  MC_InitStructure.MC_Channel = MC_Channel_U;
  MC_InitStructure.MC_PulseU = 0x1FF;
  MC_InitStructure.MC_PolarityUL = MC_Polarity_NonInverted;
  MC_InitStructure.MC_PolarityUH = MC_Polarity_NonInverted;
  MC_InitStructure.MC_DeadTime = 0xF;
  MC_InitStructure.MC_RepetitionCounter = 0x0;

  MC_Init(&MC_InitStructure);


/* Channel V configuration */
  MC_InitStructure.MC_Channel = MC_Channel_V;
  MC_InitStructure.MC_PulseV = 0xFF;
  MC_InitStructure.MC_PolarityVL = MC_Polarity_NonInverted;
  MC_InitStructure.MC_PolarityVH = MC_Polarity_NonInverted;

  MC_Init(&MC_InitStructure);

/* Channel W configuration */
  MC_InitStructure.MC_Channel = MC_Channel_W;
  MC_InitStructure.MC_PulseW = 0x7F;
  MC_InitStructure.MC_PolarityWL = MC_Polarity_NonInverted;
  MC_InitStructure.MC_PolarityWH = MC_Polarity_NonInverted;

  MC_Init(&MC_InitStructure);
 

/* MC peripheral counters enable */ 
  MC_Cmd(ENABLE);

/* Enables the different outputs */
  MC_CtrlPWMOutputs(ENABLE);

  while(1);
}
 
/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
