/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : 91x_ahbapb.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file provides all the AHBAPB software functions.
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
#include "91x_ahbapb.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#define AHBAPB_SplitCounter_Mask 0xFFE0FFFF
#define AHBAPB_SetTimeOut_Mask   0xFFFFFFE0
#define AHBAPB_Address_Mask      0xFEFFFFFF
#define AHBAPB_FLAG_RW_Mask      0x01000000
/*******************************************************************************
* Function Name  : AHBAPB_DeInit
* Description    : Deinitializes the AHBAPBx peripheral registers to their default
*                  reset values.
* Input          : AHBAPBx: where x can be 0 or 1 to select the AHBAPB peripheral.
* Output         : None
* Return         : None
*******************************************************************************/
void AHBAPB_DeInit(AHBAPB_TypeDef* AHBAPBx)
{
  AHBAPBx->BCR = 0x00000000;
  AHBAPBx->BSR = 0x00000000;
}
/*******************************************************************************
* Function Name  : AHBAPB_Init
* Description    : Initializes the AHBAPBx peripheral according to the specified
*                  parameters in the AHBAPB_InitStruct .
* Input          :- AHBAPBx: where x can be 0 or 1 to select the AHBAPB peripheral.
*                 - AHBAPB_InitStruct: pointer to a AHBAPB_InitTypeDef structure that
*                   contains the configuration information for the specified AHBAPB
*                   peripheral.
* Output         : None
* Return         : None
*******************************************************************************/
void AHBAPB_Init(AHBAPB_TypeDef* AHBAPBx, AHBAPB_InitTypeDef* AHBAPB_InitStruct)
{

  if(AHBAPB_InitStruct->AHBAPB_Split == AHBAPB_Split_Enable)
  {
    /* Set SPLITEN bit;*/
    AHBAPBx->BCR |= AHBAPB_Split_Enable;
    /*Split_CNT bits[20:16]*/
    AHBAPBx->BCR &= AHBAPB_SplitCounter_Mask;
    AHBAPBx->BCR |=  (AHBAPB_InitStruct->AHBAPB_SplitCounter)<<16;
  }
  else
  {
    /*/ Clear SPLITEN bit;*/
    AHBAPBx->BCR &= AHBAPB_Split_Disable;
  }
  /*APB Time out*/
  if(AHBAPB_InitStruct->AHBAPB_Error == AHBAPB_Error_Enable)
  {
    /* Set ERREN bit*/
    AHBAPBx->BCR |= AHBAPB_Error_Enable;
    /*Time ouit counter*/
    AHBAPBx->BCR &= AHBAPB_SetTimeOut_Mask;
    AHBAPBx->BCR |= AHBAPB_InitStruct->AHBAPB_SetTimeOut;
  }
  else
  {
    /* Clear ERREN bit*/
    AHBAPBx->BCR &= AHBAPB_Error_Disable;
  }
}

/*******************************************************************************
* Function Name  : AHBAPB_StructInit
* Description    : Initialize the AHBAPB Init Structure parameters
* Input          : AHBAPB_InitStruct : pointer to a AHBAPB_InitTypeDef structure
*                  which will be initialized.
* Output         : None
* Return         : None
*******************************************************************************/
void AHBAPB_StructInit(AHBAPB_InitTypeDef* AHBAPB_InitStruct)
{
  /* Reset AHBAPB init structure parameters values */
  AHBAPB_InitStruct->AHBAPB_Split        = AHBAPB_Split_Enable;
  AHBAPB_InitStruct->AHBAPB_SplitCounter = 0xFF;
  AHBAPB_InitStruct->AHBAPB_Error        = AHBAPB_Error_Enable;
  AHBAPB_InitStruct->AHBAPB_SetTimeOut   = 0xFF;

}

/*******************************************************************************
* Function Name  : AHBAPB_GetFlagStatus
* Description    : Checks whether the specified AHBAPB flag is set or not.
* Input          : - AHBAPB: where x can be 0 or 1 to select the AHBAPB peripheral
*                  - AHBAPB_FLAG: specifies the flag to check.
*                    This parameter can be one of the following values:
*                      - AHBAPB_FLAG_ERROR: error flag
*                      - AHBAPB_FLAG_OUTM : Out of Memory flag
*                      - AHBAPB_FLAG_APBT : APB Time-out flag
*                      - AHBAPB_FLAG_RW   : Access type flag
* Output         : None
* Return         : The new state of AHBAPB_FLAG (SET or RESET).
*******************************************************************************/
FlagStatus AHBAPB_GetFlagStatus(AHBAPB_TypeDef* AHBAPBx, u8 AHBAPB_FLAG)
{
  if(AHBAPB_FLAG == AHBAPB_FLAG_RW)
  {
    if ((AHBAPBx->PAER & AHBAPB_FLAG_RW_Mask) == RESET)
    {
       return RESET;
    }
    else
    {
      return SET;
    }
  }
  else
  {
    if ((AHBAPBx->BSR & AHBAPB_FLAG) == RESET)
    {
      return RESET;
    }
    else
    {
     return SET;
    }
  }
}
/*******************************************************************************
* Function Name  : AHBAPB_ClearFlag
* Description    : Clears the AHBAPBx flags.
* Input          : - AHBAPB: where x can be 0 or 1 to select the AHBAPB peripheral
*                  - AHBAPB_FLAG: flags to clear. This parameter one of the
*                    following values:
*                      - AHBAPB_FLAG_ERROR: error flag
*                      - AHBAPB_FLAG_OUTM : Out of Memory flag
*                      - AHBAPB_FLAG_APBT : APB Time-out flag
* Output         : None
* Return         : None
*******************************************************************************/
void AHBAPB_ClearFlag(AHBAPB_TypeDef* AHBAPBx, u8 AHBAPB_FLAG)
{
  /* Clear the flag */
  AHBAPBx->BSR &= ~AHBAPB_FLAG;
}
/*******************************************************************************
* Function Name  : AHBAPB_GetPeriphAddrError
* Description    : Gets the AHBAPB error address peripherals.
* Input          : - AHBAPB: where x can be 0 or 1 to select the AHBAPB peripheral
* Output         : None
* Return         : The Peropheral address error
*******************************************************************************/
u32 AHBAPB_GetPeriphAddrError(AHBAPB_TypeDef* AHBAPBx)
{
  u32 AHBAPB_Address = 0x00000000;

  /*Return Oeripheral address without RW bit*/
  AHBAPB_Address = (AHBAPBx->PAER)& AHBAPB_Address_Mask;
  return (AHBAPB_Address);
}
/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
