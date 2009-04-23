/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : main program for wdg example2.
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
  
  WDG_InitTypeDef WDG_InitStructure;
  GPIO_InitTypeDef  GPIO3_InitStruct;

  SCU_APBPeriphClockConfig(__GPIO3, ENABLE);/* Enable the clock for GPIO3*/
  GPIO_DeInit(GPIO3);/* GPIO3 default configuration : Reset configuration*/

   /* Configure the GPIO3  */
  GPIO3_InitStruct.GPIO_Pin= GPIO_Pin_2;  /* Choose the pin  "P3.2 port"*/
  GPIO3_InitStruct.GPIO_Direction=GPIO_PinOutput; /* Choose the  P3.2 port Direction "output".*/
  GPIO3_InitStruct.GPIO_Type = GPIO_Type_PushPull ;
  GPIO3_InitStruct.GPIO_Alternate=GPIO_OutputAlt1;
  GPIO_Init(GPIO3,&GPIO3_InitStruct); /*GPIO3 initialization with the previous chosen parameters.*/

  GPIO_WriteBit(GPIO3, GPIO_Pin_2, Bit_RESET);  /*  Reset the P3.2 port " on STR912-SK-IAR Board, LED3 is on "*/

  SCU_APBPeriphClockConfig(__WDG, ENABLE); /* Enable the clock for the WDG peripheral*/
  WDG_DeInit();/* WDG default configuration : Reset configuration*/

  /* Configure the WDG to generate a system reset signal each 1s */
  WDG_InitStructure.WDG_Mode = WDG_Mode_Wdg;
  WDG_InitStructure.WDG_Preload = 0x7d;
  WDG_InitStructure.WDG_Prescaler = 0xFF;
  WDG_InitStructure.WDG_ClockSource=WDG_ClockSource_Rtc; /* using the 32khz RTC clock as clock source*/

  WDG_Init(&WDG_InitStructure);  /* WDG initialization with the previous chosen parameters.*/



 while(1)

 {
   GPIO_WriteBit(GPIO3, GPIO_Pin_2, Bit_SET);
 }


}


/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/







