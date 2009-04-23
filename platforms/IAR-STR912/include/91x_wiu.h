/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : 91x_wiu.h
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file contains all the functions prototypes for the
*                      WIU software library.
**********************************************************************************
* History:
* 05/22/2007 : Version 1.2
* 05/24/2006 : Version 1.1
* 05/18/2006 : Version 1.0
**********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*********************************************************************************/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __91x_WIU_H
#define __91x_WIU_H

/* Includes ------------------------------------------------------------------*/
#include "91x_map.h"

/* Exported types ------------------------------------------------------------*/
/* WIU Init structure type define */
typedef struct
{
  u8   WIU_TriggerEdge;
  u32  WIU_Line ;
}WIU_InitTypeDef ;

/* Exported constants --------------------------------------------------------*/
/* Wake-up line triggering edge */
#define WIU_FallingEdge  0x00
#define WIU_RisingEdge   0x01

/* Wake-up lines*/
#define WIU_Line0  0x0001
#define WIU_Line1  (WIU_Line0<<1)
#define WIU_Line2  (WIU_Line1<<1)
#define WIU_Line3  (WIU_Line2<<1)
#define WIU_Line4  (WIU_Line3<<1)
#define WIU_Line5  (WIU_Line4<<1)
#define WIU_Line6  (WIU_Line5<<1)
#define WIU_Line7  (WIU_Line6<<1)
#define WIU_Line8  (WIU_Line7<<1)
#define WIU_Line9  (WIU_Line8<<1)
#define WIU_Line10 (WIU_Line9<<1)
#define WIU_Line11 (WIU_Line10<<1)
#define WIU_Line12 (WIU_Line11<<1)
#define WIU_Line13 (WIU_Line12<<1)
#define WIU_Line14 (WIU_Line13<<1)
#define WIU_Line15 (WIU_Line14<<1)
#define WIU_Line16 (WIU_Line15<<1)
#define WIU_Line17 (WIU_Line16<<1)
#define WIU_Line18 (WIU_Line17<<1)
#define WIU_Line19 (WIU_Line18<<1)
#define WIU_Line20 (WIU_Line19<<1)
#define WIU_Line21 (WIU_Line20<<1)
#define WIU_Line22 (WIU_Line21<<1)
#define WIU_Line23 (WIU_Line22<<1)
#define WIU_Line24 (WIU_Line23<<1)
#define WIU_Line25 (WIU_Line24<<1)
#define WIU_Line26 (WIU_Line25<<1)
#define WIU_Line27 (WIU_Line26<<1)
#define WIU_Line28 (WIU_Line27<<1)
#define WIU_Line29 (WIU_Line28<<1)
#define WIU_Line30 (WIU_Line29<<1)
#define WIU_Line31 (WIU_Line30<<1)

/* Exported constants --------------------------------------------------------*/
/* Module private variables --------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void WIU_DeInit(void);
void WIU_StructInit(WIU_InitTypeDef* WIU_InitStruct);
void WIU_Init(WIU_InitTypeDef* WIU_InitStruct);
void WIU_Cmd(FunctionalState NewState );
void WIU_GenerateSWInterrupt(u32 WIU_Line);
FlagStatus WIU_GetFlagStatus(u32 WIU_Line);
void WIU_ClearFlag(u32 WIU_Line);
ITStatus WIU_GetITStatus(u32 WIU_Line);
void WIU_ClearITPendingBit(u32 WIU_Line);

#endif /* __91x_WIU_H */

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/

