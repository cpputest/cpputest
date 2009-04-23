/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : 91x_emi.h
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file contains all the functions prototypes for the
*                      EMI software library.
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
#ifndef __91x_EMI_H
#define __91x_EMI_H

/* Includes ------------------------------------------------------------------*/
#include "91x_map.h"

/* Exported types ------------------------------------------------------------*/
 typedef struct
 {
  u32 EMI_Bank_IDCY;

  u32 EMI_Bank_WSTRD;

  u32 EMI_Bank_WSTWR;

  u32 EMI_Bank_WSTROEN;

  u32 EMI_Bank_WSTWEN;

  u32 EMI_Bank_MemWidth ;

  u32  EMI_Bank_WriteProtection;

  u32 EMI_PageModeRead_TransferLength;

  u32 EMI_PageModeRead_Selection;

 } EMI_InitTypeDef;

/* Exported constants --------------------------------------------------------*/


/* page transfer length for page mode read */
#define EMI_4Data	                 0x00000000	/*4 transfers burst*/
#define EMI_8Data	                 0x00000400	/*8 transfers burst*/


/*Select or deselect the page mode read*/
#define EMI_NormalMode	               0x00000000	/*Normal Mode*/
#define EMI_PageModeRead               0x00000100	/*Page Mode Read*/


/*memory width*/
#define EMI_Width_Byte	                 0x00000000	/*8 bits width*/
#define EMI_Width_HalfWord	         0x00000010	/*16 bits width*/

/*Write protection feature */
#define EMI_Bank_NonWriteProtect	 0x00000000	/*No write protection*/
#define EMI_Bank_WriteProtect	         0x00000008	/*bank is write protected*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void EMI_DeInit(void);
void EMI_Init( EMI_Bank_TypeDef* EMI_Bankx, EMI_InitTypeDef* EMI_InitStruct);
void EMI_StructInit(EMI_InitTypeDef* EMI_InitStruct);

#endif /* __EMI_H */

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
