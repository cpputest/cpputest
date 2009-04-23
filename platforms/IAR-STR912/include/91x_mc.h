/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : 91x_mc.h
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file contains all the functions prototypes for the 
*                      MC software library.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __91x_MC_H
#define __91x_MC_H

/* Includes ------------------------------------------------------------------*/
#include "91x_map.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  u16 MC_OperatingMode;
  u16 MC_TachoMode;
  u16 MC_TachoEvent_Mode;
  u8  MC_Prescaler;
  u8  MC_ForcedPWMState;
  u16 MC_TachoPrescaler;
  u16 MC_PWMMode;
  u16 MC_Complementary;
  u16 MC_Emergency;
  u16 MC_Period;
  u16 MC_TachoPeriod;
  u16 MC_Channel;
  u16 MC_PulseU;
  u16 MC_PulseV;
  u16 MC_PulseW;
  u16 MC_PolarityUL;
  u16 MC_PolarityUH;
  u16 MC_PolarityVL;
  u16 MC_PolarityVH;
  u16 MC_PolarityWL;
  u16 MC_PolarityWH;
  u16 MC_TachoPolarity;
  u16 MC_DeadTime;
  u8  MC_RepetitionCounter;
} MC_InitTypeDef;

typedef enum { DOWN = 0, UP  = !DOWN} CountingStatus;

/* Exported constants --------------------------------------------------------*/
#define MC_Complementary_Enable   0x0001
#define MC_Complementary_Disable  0x0002

#define MC_HardwareOperating_Mode 0x0001
#define MC_SoftwareOperating_Mode 0x0002

#define MC_PWMClassical_Mode    0x0001
#define MC_PWMZeroCentered_Mode 0x0002

#define MC_Emergency_Enable  0x0001
#define MC_Emergency_Disable 0x0002

#define MC_Polarity_Inverted     0x0001
#define MC_Polarity_NonInverted  0x0002

#define MC_TachoOneShot_Mode     0x0001
#define MC_TachoContinuous_Mode  0x0002

#define MC_TachoEventEdge_None           0x0000
#define MC_TachoEventEdge_Falling        0x0001
#define MC_TachoEventEdge_Rising         0x0002
#define MC_TachoEventEdge_RisingFalling  0x0003

#define MC_Channel_U           0x0001
#define MC_Channel_V           0x0002
#define MC_Channel_W           0x0003
#define MC_Channel_ALL         0x0004


#define MC_TachoEvent_Hardware_Mode  0x0001
#define MC_TachoEvent_Software_Mode  0x0002

#define MC_IT_CMPW  0x0001
#define MC_IT_CMPV  0x0002
#define MC_IT_CMPU  0x0004
#define MC_IT_ZPC   0x0008
#define MC_IT_ADT   0x0010
#define MC_IT_OTC   0x0020
#define MC_IT_CPT   0x0040
#define MC_IT_CM0   0x0080

#define MC_FLAG_CMPW  0x0001
#define MC_FLAG_CMPV  0x0002
#define MC_FLAG_CMPU  0x0004
#define MC_FLAG_ZPC   0x0008
#define MC_FLAG_ADT   0x0010
#define MC_FLAG_OTC   0x0020
#define MC_FLAG_CPT   0x0040
#define MC_FLAG_CM0   0x0080
#define MC_FLAG_EST   0x0100
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void MC_Init(MC_InitTypeDef* MC_InitStruct);
void MC_DeInit(void);
void MC_StructInit(MC_InitTypeDef* MC_InitStruct);
void MC_Cmd(FunctionalState NewState);
void MC_CtrlPWMOutputs(FunctionalState Newstate);
void MC_ITConfig(u16 MC_IT, FunctionalState NewState);
void MC_SetPrescaler(u8 Prescaler);
void MC_SetPeriod(u16 MC_Period);
void MC_SetPulseU(u16 MC_PulseU);
void MC_SetPulseV(u16 MC_PulseV);
void MC_SetPulseW(u16 MC_PulseW);
void MC_SetTachoCompare(u8 MC_Compare);
void MC_PWMModeConfig(u16 MC_PWMMode);
void MC_SetDeadTime(u16 MC_DeadTime);
void MC_EmergencyCmd(FunctionalState NewState);
void MC_EmergencyClear(void);
u16 MC_GetPeriod(void);
u16 MC_GetPulseU(void);
u16 MC_GetPulseV(void);
u16 MC_GetPulseW(void);
u16 MC_GetTachoCapture(void);
void MC_ClearOnTachoCapture(FunctionalState NewState);
void MC_ForceDataTransfer(u8 MC_ForcedData);
void MC_SoftwarePreloadConfig(void);
void MC_SoftwareTachoCapture(void);
CountingStatus MC_GetCountingStatus(void);
FlagStatus MC_GetFlagStatus(u16 MC_FLAG);
void MC_ClearFlag(u16 MC_FLAG);
ITStatus MC_GetITStatus(u16 MC_IT);
void MC_ClearITPendingBit(u16 MC_IT);
void MC_ClearPWMCounter(void);
void MC_ClearTachoCounter(void);

#endif /* __91x_MC_H */

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/

