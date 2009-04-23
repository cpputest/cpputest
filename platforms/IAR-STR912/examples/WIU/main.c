/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : Main program body
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
  WIU_InitTypeDef WIU_InitStructure;

/* Private function prototypes -----------------------------------------------*/
  void SCU_Configuration(void);
  void GPIO_Configuration(void);
  void VIC_Configuration(void);

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
  
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);     /*Use OSC as the default clock source*/
  SCU_PCLKDivisorConfig(SCU_PCLK_Div1);  /* ARM Peripheral bus clokdivisor = 1*/
  
  /* Configure the system clocks */
  SCU_Configuration();

  /* Configure the GPIO ports */
  GPIO_Configuration();

  /* Enable the WIU & Clear the WIU line 29 pending bit */
  WIU_Cmd(ENABLE );
  WIU_ClearITPendingBit(WIU_Line29);


  WIU_InitStructure.WIU_Line = WIU_Line29 ;
  WIU_InitStructure.WIU_TriggerEdge = WIU_FallingEdge ;
  WIU_Init(&WIU_InitStructure);

  /* Select WIU line 29 as VIC1.13 interrupt source */
  SCU_WakeUpLineConfig(29);

  /* Configure and enable the interrupt controller */
  VIC_Configuration();
  
    
  

    while(1);
}

/*******************************************************************************
* Function Name  : SCU_Configuration
* Description    : This function configures the system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SCU_Configuration(void)
{
  /*-------------------- System Clock Config --------------------*/
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);
  SCU_PLLFactorsConfig(0xC0, 0x19, 0x3);
  SCU_PLLCmd(ENABLE);
  SCU_MCLKSourceConfig(SCU_MCLK_PLL);

  /* Enable VIC clock */
  SCU_AHBPeriphClockConfig(__VIC,ENABLE);
  VIC_DeInit();

  /* Enable WIU clock */
  SCU_APBPeriphClockConfig(__WIU, ENABLE);
  WIU_DeInit();

  /* GPIO 7 clock source enable */
  SCU_APBPeriphClockConfig(__GPIO7, ENABLE);
  GPIO_DeInit(GPIO7);

  /* GPIO 3 clock source enable */
  SCU_APBPeriphClockConfig(__GPIO3, ENABLE);
  GPIO_DeInit(GPIO3);

}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : This function configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIO7  pin configuration */
  GPIO_DeInit(GPIO7);
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull ;
  GPIO_Init (GPIO7, &GPIO_InitStructure);

  /* GPIO3 pin configuration */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_Alternate=GPIO_OutputAlt1;
  GPIO_Init(GPIO3, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : VIC_Configuration
* Description    : This function configures and enables the interrupt controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void VIC_Configuration(void)
{
  VIC_DeInit();
  /* Configure the External interrupt group 3 priority */
  VIC_Config(EXTIT3_ITLine, VIC_IRQ, 0);
  /* Enable the External interrupt group 3 */
  VIC_ITCmd(EXTIT3_ITLine, ENABLE);
}

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
