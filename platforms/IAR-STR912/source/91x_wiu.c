/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : 91x_wiu.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file provides all the WIU software functions.
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

/* Standard include ----------------------------------------------------------*/
#include "91x_wiu.h"
#include "91x_scu.h"

/* Include of other module interface headers ---------------------------------*/
/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* WIU Masks "used" only in this module */
#define WIU_Enable        0x02


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Interface functions -------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/******************************************************************************
* Function Name  : WIU_Init
* Description    : Initializes the WIU unit according to the specified parameters
*                  in the WIU_InitTypeDef structure.
* Input          : WIU_InitStruct: pointer to a WIU_InitTypeDef structure that
*                  contains the configuration information for the WIU peripheral.
* Output         : None
* Return         : None
******************************************************************************/
void WIU_Init(WIU_InitTypeDef* WIU_InitStruct)
{
  /* select the Wake-up line to be used */
  WIU->MR |= WIU_InitStruct->WIU_Line;

  /* configure the triggering edge */
  if(WIU_InitStruct->WIU_TriggerEdge == WIU_RisingEdge)
  {
    /* trigger on rising edge */
    WIU->TR |= WIU_InitStruct->WIU_Line;
  }
  else
  {
    /* trigger on falling edge */
    WIU->TR &= ~WIU_InitStruct->WIU_Line;
  }

}

/******************************************************************************
* Function Name  : WIU_DeInit
* Description    : Deinitializes the WIU registers to their default reset values.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void WIU_DeInit(void)
{
  /* initialize the WIU registers to their reset value */
  SCU_APBPeriphReset(__WIU, ENABLE);
  SCU_APBPeriphReset(__WIU, DISABLE);
}

/******************************************************************************
* Function Name  : WIU_StructInit
* Description    : Fills in a WIU_InitTypeDef structure with the reset value of
*                  each parameter.
* Input          : WIU_InitStruct : pointer to a WIU_InitTypeDef structure
*                  which will be initialized.
* Output         : None
* Return         : None
******************************************************************************/
void WIU_StructInit(WIU_InitTypeDef* WIU_InitStruct)
{
  /* initialize the WIU_InitStruct fields to their reset values */
  WIU_InitStruct->WIU_Line = 0x0 ;
  WIU_InitStruct->WIU_TriggerEdge = WIU_FallingEdge ;
}

/*******************************************************************************
* Function Name  : WIU_Cmd
* Description    : Enables or disables the WIU peripheral.
* Input          : NewState: new state of the WIU peripheral (Newstate can be
*                  ENABLE or DISABLE)
* Output         : None
* Return         : None
*******************************************************************************/
void WIU_Cmd(FunctionalState NewState )
{
     if(NewState == ENABLE)
  {
      /* Enable the Wake-up Unit (for interrupts and wake-up from low power modes) */
      WIU->CTRL |= WIU_Enable ;
  }
    else
  {
        /* Disable the Wake-up Unit (for interrupts and wake-up from low power modes) */

     WIU->CTRL &= WIU_Enable ;
  }
}

/*******************************************************************************
* Function Name  : WIU_GenerateSWInterrupt
* Description    : Generates a Software interrupt.
* Input          : - WIU_Line: specifies the WIU lines to be enabled or
*                    disabled. This parameter can be:
*                     - WIU_Linex: External interrupt line x where x(0..31)
* Output         : None
* Return         : None
*******************************************************************************/
void WIU_GenerateSWInterrupt(u32 WIU_Line)
{
  WIU->INTR |= WIU_Line;
}

/*******************************************************************************
* Function Name  : WIU_GetFlagStatus
* Description    : Checks whether the specified WIU line flag is set or not.
* Input          : - WIU_Line: specifies the WIU lines flag to check.
*                    This parameter can be:
*                     - WIU_Linex: External interrupt line x where x(0..31)
* Output         : None
* Return         : The new state of WIU_Line (SET or RESET).
*******************************************************************************/
FlagStatus WIU_GetFlagStatus(u32 WIU_Line)
{
  if((WIU->PR & WIU_Line) != RESET)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/*******************************************************************************
* Function Name  : WIU_ClearFlag
* Description    : Clears the WIU’s line pending flags.
* Input          : - WIU_Line: specifies the WIU lines flags to clear.
*                    This parameter can be:
*                     - WIU_Linex: External interrupt line x where x(0..31)
* Output         : None
* Return         : None
*******************************************************************************/
void WIU_ClearFlag(u32 WIU_Line)
{
  WIU->PR = WIU_Line;
}

/*******************************************************************************
* Function Name  : WIU_GetITStatus
* Description    : Checks whether the specified WIU line is asserted or not.
* Input          : - WIU_Line: specifies the WIU lines to check.
*                    This parameter can be:
*                     - WIU_Linex: External interrupt line x where x(0..31)
* Output         : None
* Return         : The new state of WIU_Line (SET or RESET).
*******************************************************************************/
ITStatus WIU_GetITStatus(u32 WIU_Line)
{
  if(((WIU->PR & WIU_Line) != RESET)&& ((WIU->MR & WIU_Line) != RESET))
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/*******************************************************************************
* Function Name  : WIU_ClearITPendingBit
* Description    : Clears the WIU’s line pending bits.
* Input          : - WIU_Line: specifies the WIU lines to clear.
*                    This parameter can be:
*                     - WIU_Linex: External interrupt line x where x(0..31)
* Output         : None
* Return         : None
*******************************************************************************/
void WIU_ClearITPendingBit(u32 WIU_Line)
{
  WIU->PR = WIU_Line;
}


/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
