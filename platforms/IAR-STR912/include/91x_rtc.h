/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : 91x_rtc.h
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file provides the RTC library software functions
*                      prototypes & definitions
********************************************************************************
* History:
* 05/22/2007 : Version 1.2
* 05/24/2006 : Version 1.1
* 05/18/2006 : Version 1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
* A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __91x_RTC_H
#define __91x_RTC_H

/* Includes ------------------------------------------------------------------*/
#include "91x_map.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  u8 century;
  u8 year;
  u8 month;
  u8 day;
  u8 weekday;
}RTC_DATE;

typedef struct
{
  u8 hours;
  u8 minutes;
  u8 seconds;
  u16 milliseconds;
}RTC_TIME;
	
typedef struct
{
  u8 day;
  u8 hours;
  u8 minutes;
  u8 seconds;
}RTC_ALARM;

/* Exported constants --------------------------------------------------------*/

#define BINARY 0
#define BCD 1

/*TamperMode*/
#define RTC_TamperMode_Edge  0xFFFFFFEF
#define RTC_TamperMode_Level 0x10

/*TamperPol*/
#define RTC_TamperPol_High  0x4
#define RTC_TamperPol_Low   0xFFFFFFFB

/*PeriodicClock*/
#define RTC_Per_2Hz      0x10000
#define RTC_Per_16Hz     0x20000
#define RTC_Per_128Hz    0x40000
#define RTC_Per_1024Hz   0x80000
#define RTC_Per_DISABLE  0x0

/*RTC_IT*/
#define RTC_IT_Per      0x200000
#define RTC_IT_Alarm    0x800000
#define RTC_IT_Tamper   0x400000

/*RTC_FLAG*/
#define RTC_FLAG_Per     0x80000000
#define RTC_FLAG_Alarm   0x40000000
#define RTC_FLAG_Tamper  0x10000000


/* Module private variables --------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void RTC_DeInit(void);
void RTC_SetDate(RTC_DATE Date);
void RTC_SetTime(RTC_TIME Time);
void RTC_SetAlarm(RTC_ALARM Alarm);
void RTC_GetDate(u8 Format, RTC_DATE * Date);
void RTC_GetTime(u8 Format, RTC_TIME * Time);
void RTC_GetAlarm(u8 Format, RTC_ALARM * Alarm);
void RTC_TamperConfig(u32 TamperMode, u32 TamperPol);
void RTC_TamperCmd(FunctionalState NewState);
void RTC_AlarmCmd(FunctionalState NewState);
void RTC_CalibClockCmd(FunctionalState NewState);
void RTC_SRAMBattPowerCmd(FunctionalState NewState);
void RTC_PeriodicIntConfig(u32 PeriodicClock);
void RTC_ITConfig(u32 RTC_IT, FunctionalState NewState);
FlagStatus RTC_GetFlagStatus(u32 RTC_FLAG);
void RTC_ClearFlag(u32 RTC_FLAG);
#endif /*__91x_RTC_H*/

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/



