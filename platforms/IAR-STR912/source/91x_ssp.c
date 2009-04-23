/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : 91x_ssp.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file provides all the SSP software functions.
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
#include "91x_ssp.h"
#include "91x_scu.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* SSP peripheral Enable */
#define SSP_Enable   0x0002
#define SSP_Disable  0xFFFD

/* SSP Loop Back Mode Enable */
#define SSP_LoopBackMode_Enable   0x0001
#define SSP_LoopBackMode_Disable  0xFFFE

/* SSP Flag Mask */
#define SSP_Flag_Mask  0x001F

/* SSP DMA transmit/ receive enable/disable Masks */
#define SSP_DMA_TransmitEnable   0x0002
#define SSP_DMA_TransmitDisable  0xFFFD
#define SSP_DMA_ReceiveEnable    0x0001
#define SSP_DMA_ReceiveDisable   0xFFFE

/* SSP Masks */
#define SSP_FrameFormat_Mask     0xFFCF
#define SSP_DataSize_Mask        0xFFF0
#define SSP_ClockRate_Mask       0x00FF
#define SSP_ClockPrescaler_Mask  0xFF00

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : SSP_DeInit
* Description    : Deinitializes the SSPx peripheral registers to their default
*                  reset values.
* Input          : SSPx: where x can be 0 or 1 to select the SSP peripheral.
* Output         : None
* Return         : None
*******************************************************************************/
void SSP_DeInit(SSP_TypeDef* SSPx)
{
  if(SSPx == SSP0)
  {
    /* Reset the SSP0 registers values*/
    SCU_APBPeriphReset(__SSP0,ENABLE);
    SCU_APBPeriphReset(__SSP0,DISABLE);
  }
  else if (SSPx == SSP1)
  {
    /* Reset the SSP1 registers values*/
    SCU_APBPeriphReset(__SSP1,ENABLE);
    SCU_APBPeriphReset(__SSP1,DISABLE);
  }
}

/*******************************************************************************
* Function Name  : SSP_Init
* Description    : Initializes the SSPx  peripheral according to the specified
*                  parameters in the SSP_InitTypeDef structure.
* Input          : - SSPx: where x can be 0 or 1 to select the SSP peripheral.
*                  - SSP_InitStruct: pointer to a SSP_InitTypeDef structure that
*                    contains the configuration information for the specified SSP
*                    peripheral.
* Output         : None
* Return         : None
*******************************************************************************/
void SSP_Init(SSP_TypeDef* SSPx, SSP_InitTypeDef* SSP_InitStruct)
{
  if(SSP_InitStruct->SSP_FrameFormat == SSP_FrameFormat_Motorola)
  {
    /* Set the Motorola frame format */
    SSPx->CR0 &= SSP_FrameFormat_Motorola;
    /* Configure the Clock polarity */
    if(SSP_InitStruct->SSP_CPOL == SSP_CPOL_High)
    {
      /* SCK is held high when no data is being transfered */
      SSPx->CR0 |= SSP_CPOL_High;
    }
    else
    {
      /* SCK is held low when no data is being transfered */
      SSPx->CR0 &= SSP_CPOL_Low;
    }
    /* Configure the Clock Phase */
    if(SSP_InitStruct->SSP_CPHA == SSP_CPHA_2Edge)
    {
      /* Data captured on second clock edge */
      SSPx->CR0 |= SSP_CPHA_2Edge;
    }
    else
    {
      /* Data captured on first clock edge */
      SSPx->CR0 &= SSP_CPHA_1Edge;
    }
  }
   /* Configure the Frame format */
  else
  {
    /* Clear the FRF[1:0] bits */
    SSPx->CR0 &= SSP_FrameFormat_Mask;
    /* Set the TI frame format */
    SSPx->CR0 |= SSP_InitStruct->SSP_FrameFormat;
  }
  /* Configure the Mode */
  if(SSP_InitStruct->SSP_Mode == SSP_Mode_Slave)
  {
    /* Set the slave mode */
    SSPx->CR1 |= SSP_Mode_Slave;
    /* Configure the Slave output */
    if(SSP_InitStruct->SSP_SlaveOutput == SSP_SlaveOutput_Disable)
    {
      /* Slave output disabled */
      SSPx->CR1 |= SSP_SlaveOutput_Disable;
    }
    else
    {
      /* Slave output enabled */
      SSPx->CR1 &= SSP_SlaveOutput_Enable;
    }
  }
  else
  {
    /* Set the master mode */
    SSPx->CR1 &= SSP_Mode_Master;
    /* Clear clock rate SCR[7:0] bits */
    SSPx->CR0 &= SSP_ClockRate_Mask;
    /* Set the serial clock rate */
    SSPx->CR0 |= (SSP_InitStruct->SSP_ClockRate<<8);
    /* Clear clock prescaler CPSDVSR[7:0] bits */
    SSPx->PR &= SSP_ClockPrescaler_Mask;
    /* Set the serial clock prescaler */
    SSPx->PR |= SSP_InitStruct->SSP_ClockPrescaler;
  }

  /* Clear data size DSS[3:0] bits */
  SSPx->CR0 &= SSP_DataSize_Mask;
  /* Set the data size */
  SSPx->CR0 |= SSP_InitStruct->SSP_DataSize;
}
/*******************************************************************************
* Function Name  : SSP_StructInit
* Description    : Fills in a SSP_InitTypeDef structure with the reset value of
*                  each parameter.
* Input          : SSP_InitStruct : pointer to a SSP_InitTypeDef structure
                   which will be initialized.
* Output         : None
* Return         : None
*******************************************************************************/
void SSP_StructInit(SSP_InitTypeDef* SSP_InitStruct)
{
  /* Initialize the SSP_FrameFormat member */
  SSP_InitStruct->SSP_FrameFormat = SSP_FrameFormat_Motorola;

  /* Initialize the SSP_Mode member */
  SSP_InitStruct->SSP_Mode = SSP_Mode_Master;

  /* Initialize the SSP_CPOL member */
  SSP_InitStruct->SSP_CPOL = SSP_CPOL_Low;

  /* Initialize the SSP_CPHA member */
  SSP_InitStruct->SSP_CPHA = SSP_CPHA_1Edge;

  /* Initialize the SSP_DataSize member */
  SSP_InitStruct->SSP_DataSize = SSP_DataSize_8b;

  /* Initialize the SSP_SlaveOutput member */
  SSP_InitStruct->SSP_SlaveOutput = SSP_SlaveOutput_Enable;

  /* Initialize the SSP_ClockRate member */
  SSP_InitStruct->SSP_ClockRate = 0;

  /* Initialize the SSP_ClockPrescaler member */
  SSP_InitStruct->SSP_ClockPrescaler = 0;
}

/*******************************************************************************
* Function Name  : SSP_Cmd
* Description    : Enables or disables the specified SSP peripheral.
* Input          : - SSPx: where x can be 0 or 1 to select the SSP peripheral.
*                  - NewState: new state of the  SSPx peripheral. This parameter
*                    can be: ENABLE or DISABLE.
* Output         : None
* Return         : None
*******************************************************************************/
void SSP_Cmd(SSP_TypeDef* SSPx, FunctionalState NewState)
{
  if(NewState == ENABLE)
  {
    /* Enable the SSP peripheral */
    SSPx->CR1 |= SSP_Enable;
  }
  else
  {
    /* Disable the SSP peripheral */
    SSPx->CR1 &= SSP_Disable;
  }
}

/*******************************************************************************
* Function Name  : SSP_ITConfig
* Description    : Enables or disables the specified SSP interrupts.
* Input          : - SSPx: where x can be 0 or 1 to select the SSP peripheral.
*                  - SSP_IT: specifies the SSP interrupts sources to be enabled
*                    or disabled. This parameter can be any combination of the
*                    following values:
*                         - SSP_IT_TxFifo: Transmit FIFO half empty or less interrupt
*                         - SSP_IT_RxFifo: Receive FIFO half full or less interrupt
*                         - SSP_IT_RxTimeOut: Receive timeout interrupt
*                         - SSP_IT_RxOverrun: Receive overrun interrupt
*                  - NewState: new state of the specified SSP interrupts.
*                    This parameter can be: ENABLE or DISABLE.
* Output         : None
* Return         : None
*******************************************************************************/
void SSP_ITConfig(SSP_TypeDef* SSPx, u16 SSP_IT, FunctionalState NewState)
{
  if(NewState == ENABLE)
  {
    /* Enable the selected SSP interrupts */
    SSPx->IMSCR |= SSP_IT;
  }
  else
  {
    /* Disable the selected SSP interrupts */
    SSPx->IMSCR &= ~SSP_IT;
  }
}

/*******************************************************************************
* Function Name  : SSP_DMACmd
* Description    : Configures the SSP0 DMA interface.
* Input          :  - SSPx: where x can be 0 or 1 to select the SSP peripheral.
*                   - SSP_DMATransfert : specifies the DMA transfert to be
*                    enabled or disabled. This parameter can be one of the
*                    following values:
*                         - SSP_DMA_Transmit: transmit Fifo DMA transfert
*                         - SSP_DMA_Receive : receive Fifo DMA transfert
*                  - NewState: new state of the DMA transfert.
*                    This parameter can be: ENABLE or DISABLE.
* Output         : None
* Return         : None
*******************************************************************************/
void SSP_DMACmd(SSP_TypeDef* SSPx, u16 SSP_DMATransfert, FunctionalState NewState)
{
  if(NewState == ENABLE)
  {
    if(SSP_DMATransfert == SSP_DMA_Transmit)
    {
      /* Enable DMA for the transmit FIFO */
      SSPx->DMACR |= SSP_DMA_TransmitEnable;
    }
    else
    {
      /* Enable DMA for the receive FIFO */
      SSPx->DMACR |= SSP_DMA_ReceiveEnable;
    }
  }
  else
  {
    if(SSP_DMATransfert == SSP_DMA_Transmit)
    {
      /* Disable DMA for the transmit FIFO */
      SSPx->DMACR &= SSP_DMA_TransmitDisable;
    }
    else
    {
      /* Disable DMA for the receive FIFO */
      SSPx->DMACR &= SSP_DMA_ReceiveDisable;
    }
  }
}

/*******************************************************************************
* Function Name  : SSP_SendData.
* Description    : Transmits a Data through the SSP peripheral.
* Input          : - SSPx: where x can be 0 or 1 to select the SSP peripheral.
*                  - Data : Data to be transmitted.
* Output         : None
* Return         : None
*******************************************************************************/
void SSP_SendData(SSP_TypeDef* SSPx, u16 Data)
{
  /* Write in the DR register the data to be sent */
  SSPx->DR = Data;
}

/*******************************************************************************
* Function Name  : SSP_ReceiveData.
* Description    : Returns the most recent received data by the SSP peripheral.
* Input          : SSPx: where x can be 0 or 1 to select the SSP peripheral.
* Output         : None
* Return         : The value of the received data.
*******************************************************************************/
u16 SSP_ReceiveData(SSP_TypeDef* SSPx)
{
  /* Return the data in the DR register */	
  return SSPx->DR;
}

/*******************************************************************************
* Function Name  : SSP_LoopBackConfig
* Description    : Enable or disable the Loop back mode for the selected SSPx peripheral.
* Input          : - SSPx: where x can be 0 or 1 to select the SSP peripheral.
*                  - NewState: new state of the Loop Back mode.
*                    This parameter can be: ENABLE or DISABLE.
* Output         : None
* Return         : None.
*******************************************************************************/
void SSP_LoopBackConfig(SSP_TypeDef* SSPx, FunctionalState NewState)
{
  if(NewState == ENABLE)
  {
    /* Enable loop back mode */
    SSPx->CR1 |= SSP_LoopBackMode_Enable;
  }
  else
  {
    /* Disable loop back mode */
    SSPx->CR1 &= SSP_LoopBackMode_Disable;
  }
}



/*******************************************************************************
* Function Name  : SSP_GetFlagStatus
* Description    : Checks whether the specified SSP flag is set or not.
* Input          : - SSPx: where x can be 0 or 1 to select the SSP peripheral.
*                  - SSP_FLAG: flag to check. This parameter can be one of the
*                    following values:
*                         - SSP_FLAG_Busy: busy flag
*                         - SSP_FLAG_RxFifoFull: Receive FIFO full flag
*                         - SSP_FLAG_RxFifoNotEmpty: Receive FIFO not empty flag
*                         - SSP_FLAG_TxFifoNotFull: Transmit FIFO not full flag
*                         - SSP_FLAG_TxFifoEmpty: Transmit FIFO empty flag
*                         - SSP_FLAG_TxFifo: Transmit FIFO half empty or less flag
*                         - SSP_FLAG_RxFifo: Receive FIFO half full or less flag
*                         - SSP_FLAG_RxTimeOut: Receive timeout flag
*                         - SSP_FLAG_RxOverrun: Receive overrun flag
* Output         : None
* Return         : The new state of SSP_Flag (SET or RESET).
*******************************************************************************/
FlagStatus SSP_GetFlagStatus(SSP_TypeDef* SSPx, u16 SSP_FLAG)
{
    u32 SSPReg = 0, FlagPos = 0;
    u32 StatusReg = 0;

  /* Get the SSP register index */
  SSPReg = SSP_FLAG >> 5;

  /* Get the flag position */
  FlagPos = SSP_FLAG & SSP_Flag_Mask;

  /* Find the register of the flag to check */
  if(SSPReg == 1)
  {
    /* The flag to check is in SR register */
    StatusReg = SSPx->SR;  	
  }
  else if (SSPReg == 2)
  {
    /* The flag to check is in RISR register */
    StatusReg = SSPx->RISR;
  }

  /* Check the status of the specified SSP flag */
  if((StatusReg & (1 << FlagPos)) != RESET)
  {
    /* Return SET if the SSP flag is set */
    return SET;
  }
  else
  {
    /* Return RESET if the SSP flag is reset */
    return RESET;
  }
}

/*******************************************************************************
* Function Name  : SSP_ClearFlag
* Description    : Clears the SSPx flags.
* Input          : - SSPx: where x can be 0 or 1 to select the SSP peripheral.
*                  - SSP_FLAG: flags to clear. This parameter one of the
*                    following values:
*                         - SSP_FLAG_RxTimeOut: Receive timeout flag
*                         - SSP_FLAG_RxOverrun: Receive overrun flag
* Output         : None
* Return         : None
*******************************************************************************/
void SSP_ClearFlag(SSP_TypeDef* SSPx, u16 SSP_FLAG)
{
    u8 FlagPos = 0;

  /* Get the flag position */
  FlagPos = SSP_FLAG & SSP_Flag_Mask;

  /* Clear the selected SSP flag */
  SSPx->ICR = (1 << FlagPos);
}

/*******************************************************************************
* Function Name  : SSP_GetITStatus
* Description    : Checks whether the specified SSP interrupt flag is set or not.
* Input          : - SSPx: where x can be 0 or 1 to select the SSP peripheral.
*                  - SSP_IT: interrupt flag to check. This parameter can be one
*                    of the following values:
*                         - SSP_IT_TxFifo: Transmit FIFO half empty or less interrupt
*                         - SSP_IT_RxFifo: Receive FIFO half full or less interrupt
*                         - SSP_IT_RxTimeOut: Receive timeout interrupt
*                         - SSP_IT_RxOverrun: Receive overrun interrupt
* Output         : None
* Return         : The new state of SSP_IT flag (SET or RESET).
*******************************************************************************/
ITStatus SSP_GetITStatus(SSP_TypeDef* SSPx, u16 SSP_IT)
{
  /* Check the status of the specified interrupt flag */
  if((SSPx->MISR & SSP_IT) != RESET)
  {
    /* Return SET if the SSP interrupt flag is set */
    return SET;
  }
  else
  {
    /* Return RESET if SSP interrupt flag is reset */
    return RESET;
  }
}

/*******************************************************************************
* Function Name  : SSP_ClearITPendingBit
* Description    : Clears the pending interrupt flags.
* Input          : - SSPx: where x can be 0 or 1 to select the SSP peripheral.
*                  - SSP_IT: interrupts pending bits to clear. This parameter
*                    can be any combination of the following values:
*                         - SSP_IT_RxTimeOut: Receive timeout interrupt
*                         - SSP_IT_RxOverrun: Receive overrun interrupt
* Output         : None
* Return         : None
*******************************************************************************/
void SSP_ClearITPendingBit(SSP_TypeDef* SSPx, u16 SSP_IT)
{
  /* Clear the selected SSP interrupts pending bits */
  SSPx->ICR = SSP_IT;
}

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/

