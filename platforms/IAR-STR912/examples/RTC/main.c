/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD application team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : RTC example
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
#include <91x_lib.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 RTC_TIME time;
 RTC_DATE date;
 RTC_ALARM alarm;
 GPIO_InitTypeDef     GPIO_Struct;
 vu32 i;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




 int main (void)
 {
  #ifdef DEBUG
  debug();
  #endif
  
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);    /*Use OSC as the default clock source*/
  SCU_PCLKDivisorConfig(SCU_PCLK_Div1); /* ARM Peripheral bus clokdivisor = 1*/
  
  
  SCU_APBPeriphClockConfig(__RTC,ENABLE); /*enable RTC clock*/
  SCU_APBPeriphReset(__RTC,DISABLE);      /*RTC out of RESET state*/

 /*config GPIO pin P3.1 for software togling*/
  SCU_APBPeriphClockConfig(__GPIO3,ENABLE);
  SCU_APBPeriphReset(__GPIO3,DISABLE);
  
  GPIO_DeInit(GPIO3);
   
  GPIO_Struct.GPIO_Pin= GPIO_Pin_1;
  GPIO_Struct.GPIO_Direction=GPIO_PinOutput;
  GPIO_Struct.GPIO_Type=GPIO_Type_PushPull;
  GPIO_Struct.GPIO_Alternate=GPIO_OutputAlt1;
  GPIO_Init(GPIO3,&GPIO_Struct);


  /*configure date & time structures */
  date.day = 7 ;                          /* set day */
  date.weekday = 3;                       /* set weekday */
  date.month = 3;                         /* set month */
  date.year = 6;                          /* set year */
  date.century = 20;                      /* set century */
  time.hours = 19;                        /* set hours */
  time.minutes = 0;                       /* set minutes */
  time.seconds = 0;                       /* set seconds */
  time.milliseconds = 0;                  /* set milliseconds */

  /*set time & date*/
  RTC_SetDate(date);                      /* set date in RTC */
  RTC_SetTime(time);                      /* set time in RTC registers */

  alarm.day = 7;                          /* set alarm day */
  alarm.hours = 19;                       /* set alarm hours */
  alarm.minutes = 0;                      /* set alarm minutes */
  alarm.seconds = 10;                     /* set alarm seconds */

  /*set alarm*/
  RTC_SetAlarm(alarm);

  RTC_ClearFlag(RTC_FLAG_Alarm);          /* clear alarm flag*/
  RTC_AlarmCmd(ENABLE);                   /* enable alarm */
  while (!RTC_GetFlagStatus(RTC_FLAG_Alarm)); /* do polling or RTC alarm flag*/
  RTC_GetTime(BCD,&time);                 /*time structure must be equal to alarm time*/

  while(1)
  {
    for(i=0;i<100000;i++);
    GPIO_WriteBit(GPIO3, GPIO_Pin_1, Bit_SET);
    for(i=0;i<100000;i++);
    GPIO_WriteBit(GPIO3, GPIO_Pin_1, Bit_RESET);
  }

}



 /******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
