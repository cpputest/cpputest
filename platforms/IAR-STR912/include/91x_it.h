/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : 91x_it.h
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file contains the headers of the interrupt
*                      handlers'routines
********************************************************************************
* History:
* 01/05/2008 : Version 1.3
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

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef _91x_IT_H
#define _91x_IT_H

/* Includes ------------------------------------------------------------------*/
#include "91x_lib.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Module private variables --------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Undefined_Handler  (void);
void SWI_Handler        (void);
void Prefetch_Handler   (void);
void Abort_Handler      (void);
void FIQ_Handler        (void);
void WDG_IRQHandler_l   (void);
void SW_IRQHandler_l    (void);
void ARMRX_IRQHandler_l (void);
void ARMTX_IRQHandler_l (void);
void TIM0_IRQHandler_l  (void);
void TIM1_IRQHandler_l  (void);
void TIM2_IRQHandler_l  (void);
void TIM3_IRQHandler_l  (void);
void USBHP_IRQHandler_l (void);
void USBLP_IRQHandler_l (void);
void SCU_IRQHandler_l   (void);
void ENET_IRQHandler_l  (void);
void DMA_IRQHandler_l   (void);
void CAN_IRQHandler_l   (void);
void MC_IRQHandler_l    (void);
void ADC_IRQHandler_l   (void);
void UART0_IRQHandler_l (void);
void UART1_IRQHandler_l (void);
void UART2_IRQHandler_l (void);
void I2C0_IRQHandler_l  (void);
void I2C1_IRQHandler_l  (void);
void SSP0_IRQHandler_l  (void);
void SSP1_IRQHandler_l  (void);
void LVD_IRQHandler_l   (void);
void RTC_IRQHandler_l   (void);
void WIU_IRQHandler_l   (void);
void EXTIT0_IRQHandler_l(void);
void EXTIT1_IRQHandler_l(void);
void EXTIT2_IRQHandler_l(void);
void EXTIT3_IRQHandler_l(void);
void EXTIT4_IRQHandler_l(void);
void USBWU_IRQHandler_l (void);
void PFQBC_IRQHandler_l (void);

#endif /* _91x_IT_H */

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
