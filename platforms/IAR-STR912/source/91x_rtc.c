/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : 91x_rtc.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file provides the RTC library software functions
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

/* Includes ------------------------------------------------------------------*/
#include "91x_rtc.h"
#include "91x_scu.h"

/* Include of other module interface headers ---------------------------------*/
/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
u8 BYTEToBCD2(u8 value);
u16 WORDToBCD3(u16 value);
u8 BCD2ToBYTE(u8 value);
u16 BCD3ToBYTE(u16 value);
/* Interface functions -------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : BYTEToBCD2
* Description    : Converts a 2 digit decimal to BCD format
* Input          : None
* Output         : None
* Return         : Converted byte
*******************************************************************************/
u8 BYTEToBCD2(u8 value)
{
  u8 bcdhigh = 0;
  while (value >= 10)
  {
    bcdhigh++;
    value -= 10;
  }
  return  (bcdhigh << 4) | value;
}
/*******************************************************************************
* Function Name  : WORDToBCD3
* Description    : Converts a 3 digit decimal to BCD format
* Input          : None
* Output         : None
* Return         : Converted word
*******************************************************************************/
u16 WORDToBCD3(u16 value)
{
	u16 bcdhigh = 0;
	while (value >= 100)
	{
		bcdhigh++;
		value -= 100;
	}
	bcdhigh <<= 4;
	while (value >= 10)
	{
		bcdhigh++;
		value -= 10;
	}
	return  (bcdhigh << 4) | value;
}

/*******************************************************************************
* Function Name  : BCD3ToWORD
* Description    : convert from 3 digit BCD to Binary
* Input          : None
* Output         : None
* Return         : Converted word
*******************************************************************************/
u16 BCD3ToWORD(u16 value)
{
  return (u16)((((value&0xF00)>>8)*100) + (((value&0x0F0)>>4)*10) + (value&0x0F));
}

/*******************************************************************************
* Function Name  : BCD2ToBYTE
* Description    : convert from 2 digit BCD to Binary
* Input          : None
* Output         : None
* Return         : Converted word
*******************************************************************************/
u8 BCD2ToBYTE(u8 value)
{
  u32 tmp;
  tmp= ((value&0xF0)>>4)*10;
  return (u8)(tmp+ (value&0x0F));	
}

/*******************************************************************************
* Function Name  : RTC_DeInit
* Description    : Resets the RTC peripheral registers
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_DeInit(void)
{
	SCU_APBPeriphReset(__RTC,ENABLE);
	SCU_APBPeriphReset(__RTC,DISABLE);
}

/*******************************************************************************
* Function Name  : RTC_SetDate
* Description    : Sets the Date register
* Input          : struct of type RTC_DATE
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_SetDate(RTC_DATE Date)
{
	u32 tmp = 0;
	
	RTC->CR |=0x80;  /*Enable write operation in DTR register*/
	RTC->DTR = 0;
	tmp = BYTEToBCD2(Date.century);
	RTC->DTR|=tmp<<24;
	tmp = BYTEToBCD2(Date.year);
	RTC->DTR|=tmp<<16;
	tmp = BYTEToBCD2(Date.month);
	RTC->DTR|=tmp<<8;
	tmp = BYTEToBCD2(Date.weekday);
	RTC->DTR|=tmp;
        RTC->TR &=0xFFFFFF;
        tmp = BYTEToBCD2(Date.day);
	RTC->TR|=tmp<<24;
	RTC->CR &=~0x80; /*Disable write operation in DTR register*/
}
/*******************************************************************************
* Function Name  : RTC_SetTime
* Description    : Sets the Time register
* Input          : struct of type RTC_TIME
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_SetTime(RTC_TIME Time)
{
        u32 tmp = 0;
	
	RTC->CR |=0x80;  /*Enable write operation in TR register*/
	RTC->TR &= 0xFF000000;
	tmp = BYTEToBCD2(Time.hours);
	RTC->TR|=tmp<<16;
	tmp = BYTEToBCD2(Time.minutes);
	RTC->TR|=tmp<<8;
	tmp = BYTEToBCD2(Time.seconds);
	RTC->TR|=tmp;
        RTC->MILR = 0;
	RTC->MILR |= WORDToBCD3(Time.milliseconds);
	RTC->CR &=~0x80; /*Disable write operation in TR register*/
}
/*******************************************************************************
* Function Name  : RTC_SetAlarm
* Description    : Sets the Alarm register
* Input          : Struct of type RTC_ALARM
* Output         : Date
* Return         : None
*******************************************************************************/
void RTC_SetAlarm(RTC_ALARM Alarm)
{
	u32 tmp = 0;

        RTC->CR |=0x80;  /*Enable write operation in ATR register*/
        RTC->ATR = 0;
	tmp = BYTEToBCD2(Alarm.day);
	RTC->ATR|=tmp<<24;
	tmp = BYTEToBCD2(Alarm.hours);
	RTC->ATR|=tmp<<16;
	tmp = BYTEToBCD2(Alarm.minutes);
	RTC->ATR|=tmp<<8;
	tmp = BYTEToBCD2(Alarm.seconds);
	RTC->ATR|=tmp;
	RTC->CR &=~0x80; /*Disable write operation in ATR register*/
}

/*******************************************************************************
* Function Name  : RTC_GetDate
* Description    : Gets RTC date in BCD coded or BINARY code
* Input          : -Format: BCD or BINARY
*                  -Date: pointer to structure of type RTC_DATE to be filled by function
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_GetDate(u8 Format, RTC_DATE * Date)
{
	Date->century = (u8)((RTC->DTR&0xFF000000)>>24);
	Date->year = (u8)((RTC->DTR&0x00FF0000)>>16);
	Date->month = (u8)((RTC->DTR&0x00001F00)>>8);
        Date->day = (u8)((RTC->TR&0x3F000000)>>24);
	Date->weekday = (u8)(RTC->DTR&0xF);
	if (Format == BINARY)
	{
		Date->century = BCD2ToBYTE(Date->century);
		Date->year = BCD2ToBYTE(Date->year);
		Date->month = BCD2ToBYTE(Date->month);
                Date->day = BCD2ToBYTE(Date->day);
		Date->weekday = BCD2ToBYTE(Date->weekday);
	}
}

/*******************************************************************************
* Function Name  : RTC_GetTime
* Description    : Gets TIME in BCD coded or BINARY code
* Input          : -Format: BCD or BINARY
*                  -Time : pointer to structure of type RTC_TIME to be filled by function
* Output         : Time
* Return         : None
*******************************************************************************/
void RTC_GetTime(u8 Format, RTC_TIME * Time)
{
	
	Time->hours = (u8)((RTC->TR&0x003F0000)>>16);
	Time->minutes = (u8)((RTC->TR&0x00007F00)>>8);
	Time->seconds = (u8)(RTC->TR&0x7F);
        Time->milliseconds =(u16)(RTC->MILR&0xFFF);
	if (Format == BINARY)
	{
		Time->hours = BCD2ToBYTE(Time->hours);
		Time->minutes = BCD2ToBYTE(Time->minutes);
                Time->seconds = BCD2ToBYTE(Time->seconds);
                Time->milliseconds = BCD3ToWORD(Time->milliseconds);
	}
}


/*******************************************************************************
* Function Name  : RTC_GetAlarm
* Description    : Gets the RTC Alarm in BCD or BINARY code
* Input          : -Format: BCD or BINARY
*                  -Alarm : pointer to structure of type RTC_ALARM to be filled by function
* Output         : Alarm
* Return         : None
*******************************************************************************/
void RTC_GetAlarm(u8 Format,RTC_ALARM * Alarm)
{
        Alarm->day = (u8)((RTC->ATR&0x3F000000)>>24);
	Alarm->hours = (u8)((RTC->ATR&0x003F0000)>>16);
	Alarm->minutes = (u8)((RTC->ATR&0x00007F00)>>8);
	Alarm->seconds = (u8)((RTC->ATR)&0x7F);
	if (Format == BINARY)
	{
		Alarm->day = BCD2ToBYTE(Alarm->day);
		Alarm->hours = BCD2ToBYTE(Alarm->hours);
		Alarm->minutes = BCD2ToBYTE(Alarm->minutes);
		Alarm->seconds = BCD2ToBYTE(Alarm->seconds);
	}
}

/*******************************************************************************
* Function Name  : RTC_TamperConfig
* Description    : configures the Tamper mode and tamper polarity
* Input          : -TamperMode: RTC_TamperMode_Edge or RTC_TamperMode_Level
*                  -TamperPol : RTC_TamperPol_Low or RTC_TamperMode_High
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_TamperConfig(u32 TamperMode, u32 TamperPol)
{
	RTC->CR&=RTC_TamperMode_Edge;
	if (TamperMode!=RTC_TamperMode_Edge)
	RTC->CR|=RTC_TamperMode_Level;
	
	RTC->CR&=RTC_TamperPol_Low;
	if (TamperPol!=RTC_TamperPol_Low)
	RTC->CR|=RTC_TamperPol_High;
}

/*******************************************************************************
* Function Name  : RTC_TamperCmd
* Description    : Enable or Disable Tamper
* Input          : NewState: ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_TamperCmd(FunctionalState NewState)
{
	RTC->CR&=0xFFFFFFFE;
	if (NewState==ENABLE)
	RTC->CR|=0x1;
}

/*******************************************************************************
* Function Name  : RTC_AlarmCmd
* Description    : Enable or Disable Alarm
* Input          : NewState: ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_AlarmCmd(FunctionalState NewState)
{
	RTC->CR&=~0x100000;
	if (NewState==ENABLE)
	RTC->CR|=0x100000;
}

/*******************************************************************************
* Function Name  : RTC_CalibClockCmd
* Description    : Enable or Disable RTC Calibration Clock Output
* Input          : NewState: ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_CalibClockCmd(FunctionalState NewState)
{
	RTC->CR&=~0x40;
	if (NewState ==ENABLE)
	RTC->CR|=0x40;
}

/*******************************************************************************
* Function Name  : SRAMBattPowerCmd
* Description    : Enable or Disable SRAM backup Power by VBATT
* Input          : NewState : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_SRAMBattPowerCmd(FunctionalState NewState)
{
	RTC->CR&=~0x8;
	if (NewState ==ENABLE)
	RTC->CR|=0x8;
}

/*******************************************************************************
* Function Name  : RTC_PeridicIntConfig
* Description    : Select a Periodic CLock
* Input          : PeriodicClock
* Output         : None
* Return         : None
* Note           : When PeriodicClock = RTC_Per_DISABLE the Periodic clock generation
*                  will be disabled.
*******************************************************************************/
void RTC_PeriodicIntConfig(u32 PeriodicClock)
{
	RTC->CR &=~0xF0000;
	RTC->CR|=PeriodicClock;
}

/*******************************************************************************
* Function Name  : RTC_ITConfig
* Description    : Enable or Disable an interrupt
* Input          : -RTC_IT : RTC interrupt
*                  -Newstate: Enable or Disable
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_ITConfig(u32 RTC_IT, FunctionalState NewState)
{
	RTC->CR&=~RTC_IT;
	if (NewState==ENABLE)
	RTC->CR|=RTC_IT;
}

/*******************************************************************************
* Function Name  : RTC_GetFlagStatus
* Description    : Gets a RTC flag status 
* Input          : RTC_FLAG
* Output         : None
* Return         : FlagStatus :SET or RESET
*******************************************************************************/
FlagStatus RTC_GetFlagStatus(u32 RTC_FLAG)
{
	if (RTC->SR&RTC_FLAG) return SET;
	else return RESET;
}

/*******************************************************************************
* Function Name  : RTC_ClearFlag
* Description    : Clears a RTC flag
* Input          : RTC_FLAG
* Output         : None
* Return         : None
* Note           : Before clearing the RTC Periodic Flag you need to disable the 
*                  Periodic interrupt generation, to do this use function 
*                  RTC_PeriodicIntConfig(RTC_Per_DISABLE)
*******************************************************************************/
void RTC_ClearFlag(u32 RTC_FLAG)
{
  vu32 tmp=0;
  if (RTC_FLAG == RTC_FLAG_Per)  tmp=RTC->SR; 
  else if (RTC_FLAG == RTC_FLAG_Alarm) RTC->CR&=~0x100000;
  else if (RTC_FLAG == RTC_FLAG_Tamper) RTC->CR&=~0x1;
}


/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
