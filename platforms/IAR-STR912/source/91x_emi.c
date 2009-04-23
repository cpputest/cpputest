/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : 91x_emi.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file provides all the EMI software functions.
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
#include "91x_emi.h"
#include "91x_scu.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* These constant variables are used as masks to handle the EMI registers.  */

#define   EMI_PageModeRead_TL_Mask	0xFFFFF3FF
#define   EMI_PageModeRead_Sel_Mask	0xFFFFFEFF
#define   EMI_MemWidth_Mask     	0xFFFFFFCF
#define   EMI_WriteProtect_Mask	    0xFFFFFEF7


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Registers reset value */
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************
* Function Name  : EMI_DeInit
* Description    : Deinitializes the EMI peripheral registers to their default
*                  reset values.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void EMI_DeInit(void)
{

  SCU_AHBPeriphReset(__EMI, ENABLE);          /* EMI peripheral under Reset */
  SCU_AHBPeriphReset(__EMI,DISABLE );         /* EMI not under Reset */

}

/*******************************************************************************
* Function Name  : EMI_StructInit
* Description    : Fills the EMI_InitTypeDef structure member with its reset
*                  value.
* Input          : EMI_InitStruct : pointer to a EMI_InitTypeDef structure
*                  which will be initialized.
* Output         : None
* Return         : None
*******************************************************************************/

void EMI_StructInit( EMI_InitTypeDef *EMI_InitStruct)
{

  /* Number of bus turnaround cycles added between read and write accesses.*/
  /*This member can be 0x01,0x02,0x03, ....0xF (Reset value:0xF "15 cycles"*/

  EMI_InitStruct->EMI_Bank_IDCY =0xF;


  /* Number of wait states for read accesses*/
  /*This member can be: 0x01,0x02,0x03, ....0x1F (Reset value:0x1F "31 cycles"*/

  EMI_InitStruct->EMI_Bank_WSTRD =0x1F;


  /* Number of wait states for write accesses*/
  /*This member can be: 0x01,0x02,0x03, ....0x1F (Reset value:0x1F "31 cycles"*/

  EMI_InitStruct->EMI_Bank_WSTWR =0x1F;

  /*Output enable assertion delay from chip select assertion*/
  /*This member can be: 0x01,0x02,0x03, ....0xF (Reset value:0x01 "1 cycle"*/

  EMI_InitStruct->EMI_Bank_WSTROEN =0x01;


  /*Write enable assertion delay from chip select assertion*/
  /*This member can be: 0x01,0x02,0x03, ....0xF (Reset value:0x00 "0 cycle"*/

  EMI_InitStruct->EMI_Bank_WSTWEN =0x00;


  /*This member Controls the memory width*/
  /*This member can be :"EMI_Width_Byte" = 8 bits width or "EMI_Width_HalfWord" = 16 bits width*/

  EMI_InitStruct->EMI_Bank_MemWidth = EMI_Width_Byte;


  /*Write protection feature */
  /*This member can be :"EMI_Bank_NonWriteProtect" = No write protection or "EMI_Bank_WriteProtect" = bank is write protected*/

  EMI_InitStruct-> EMI_Bank_WriteProtection= EMI_Bank_NonWriteProtect;


  /* page transfer length for page mode read */
  /*This member can be :"EMI_4Data" = 4 transfers burst or "EMI_8Data" = 8 transfers burst*/

  EMI_InitStruct->EMI_PageModeRead_TransferLength= EMI_4Data;

  /*Select or deselect the page mode read*/
  /*This member can be :"EMI_NormalMode" =Normal Mode or "EMI_PageModeRead" = Page Mode Read*/

  EMI_InitStruct->EMI_PageModeRead_Selection = EMI_NormalMode;


}

/*******************************************************************************
* Function Name  : EMI_Init
* Description    : Initializes EMI  peripheral according to the specified
*                  parameters in the EMI_InitStruct.

* Input          : EMI_Bankx:where x can be 0,1,2 or 3 to select the EMI Bank.
                   EMI_InitStruct: pointer to a EMI_InitTypeDef structure
				  ( Structure Config to be loaded in EMI Registers). .

* Output         : None
* Return         : None
*******************************************************************************/

void EMI_Init( EMI_Bank_TypeDef* EMI_Bankx, EMI_InitTypeDef* EMI_InitStruct)

{

  EMI_Bankx->ICR  = EMI_InitStruct-> EMI_Bank_IDCY ;

  EMI_Bankx->RCR  = EMI_InitStruct->EMI_Bank_WSTRD ;

  EMI_Bankx->WCR  = EMI_InitStruct->EMI_Bank_WSTWR ;

  EMI_Bankx->OECR = EMI_InitStruct->EMI_Bank_WSTROEN;

  EMI_Bankx->WECR = EMI_InitStruct->EMI_Bank_WSTWEN ;

  EMI_Bankx->BCR &= EMI_MemWidth_Mask;
  EMI_Bankx->BCR |= EMI_InitStruct->EMI_Bank_MemWidth;

  EMI_Bankx->BCR &= EMI_WriteProtect_Mask;
  EMI_Bankx->BCR |= EMI_InitStruct->EMI_Bank_WriteProtection;

  EMI_Bankx->BCR &= EMI_PageModeRead_TL_Mask;
  EMI_Bankx->BCR |= EMI_InitStruct->EMI_PageModeRead_TransferLength;

  EMI_Bankx->BCR &= EMI_PageModeRead_Sel_Mask;
  EMI_Bankx->BCR |=  EMI_InitStruct->EMI_PageModeRead_Selection;


}



/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
