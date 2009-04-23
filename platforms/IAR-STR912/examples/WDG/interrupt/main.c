/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : main program for wdg example1.
*
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


/* Standard include ----------------------------------------------------------*/
#include "91x_lib.h"

/* Include of other module interface headers ---------------------------------*/
/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

  WDG_InitTypeDef WDG_InitStructure;
  GPIO_InitTypeDef  GPIO3_InitStruct;

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

  SCU_MCLKSourceConfig(SCU_MCLK_OSC);      /*Use OSC as the default clock source*/
  SCU_PCLKDivisorConfig(SCU_PCLK_Div1);   /* ARM Peripheral bus clokdivisor = 1*/

  SCU_APBPeriphClockConfig(__GPIO3, ENABLE);/* Enable the clock for GPIO3*/
  GPIO_DeInit(GPIO3);/* GPIO3 default configuration : Reset configuration*/

/* Configure the GPIO3  */
  GPIO3_InitStruct.GPIO_Pin= GPIO_Pin_2;  /* Choose the pin  "p3.2 port"*/
  GPIO3_InitStruct.GPIO_Direction=GPIO_PinOutput; /* Choose the  p3.2 port Direction "output".*/
  GPIO3_InitStruct.GPIO_Type = GPIO_Type_PushPull ;
  GPIO3_InitStruct.GPIO_Alternate=GPIO_OutputAlt1;
  GPIO_Init(GPIO3,&GPIO3_InitStruct); /*GPIO3 initialization with the previous chosen parameters.*/
  

  SCU_APBPeriphClockConfig(__WDG, ENABLE); /* Enable the clock for the WDG peripheral*/
  WDG_DeInit();/* WDG default configuration : Reset configuration*/

  /* Configure the WDG to generate an EOC interrupt each 0.67s */
  WDG_InitStructure.WDG_Mode = WDG_Mode_Timer;
  WDG_InitStructure.WDG_Preload = 0xFFFF;
  WDG_InitStructure.WDG_Prescaler = 0xFF;
  WDG_InitStructure.WDG_ClockSource=WDG_ClockSource_Apb;

  WDG_Init(&WDG_InitStructure);  /* WDG initialization with the previous chosen parameters.*/


  /* Configure the VIC */
  SCU_AHBPeriphClockConfig(__VIC,ENABLE); /*give the clock to VIC.*/
  VIC_DeInit(); /* VIC default configuration : Reset configuration*/


  /* Configure the VIC  */
  VIC_Config(WDG_ITLine,VIC_IRQ,0);
  VIC_ITCmd(WDG_ITLine, ENABLE);  /* Enable the VIC */
  WDG_ITConfig(ENABLE); /* Enable the WDG interrupt */

  /* Enable the WDG */
  WDG_Cmd(ENABLE);


  while(1);

 

}


/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/







