/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : 91x_i2c.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file provides all the I2C software functions.
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
#include "91x_i2c.h"
#include "91x_scu.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* I2C IT enable */
#define  I2C_IT_Enable     0x01
#define  I2C_IT_Disable    0xFE

/* I2C Peripheral Enable/Disable */
#define  I2C_PE_Set        0x20
#define  I2C_PE_Reset      0xDF

/* Address direction bit */
#define I2C_ADD0_Set       0x01
#define I2C_ADD0_Reset     0xFE

/* I2C START Enable/Disable */
#define  I2C_Start_Enable       0x08
#define  I2C_Start_Disable      0xF7

/* I2C STOP Enable/Disable */
#define  I2C_Stop_Enable        0x02
#define  I2C_Stop_Disable       0xFD

/* I2C Masks */
#define  I2C_Frequency_Mask     0x1F
#define  I2C_AddressHigh_Mask   0xF9
#define  I2C_OwnAddress_Mask    0x0300
#define  I2C_StandardMode_Mask  0x7f
#define  I2C_FastMode_Mask      0x80
#define  I2C_Event_Mask         0x3FFF
#define  I2C_HeaderSet_Mask     0xF1
#define  I2C_HeaderReset_Mask   0xFE

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : I2C_DeInit
* Description    : Deinitializes the I2C peripheral registers to their default
*                  reset values.
* Input          :- I2Cx: I2C peripheral can be:
*                    - I2C0
*					 - I2C1  
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_DeInit(I2C_TypeDef* I2Cx)
{
  if (I2Cx == I2C0)
  {
  /* Reset the I2C0 registers values */
   SCU_APBPeriphReset(__I2C0, ENABLE);
   SCU_APBPeriphReset(__I2C0, DISABLE);
  }
   if (I2Cx == I2C1)
  {
  /* Reset the I2C1 registers values */
  SCU_APBPeriphReset(__I2C1, ENABLE);
  SCU_APBPeriphReset(__I2C1, DISABLE);
  }
}

/*******************************************************************************
* Function Name  : I2C_Init
* Description    : Initializes the I2C  peripheral according to the specified
*                  parameters in the I2C_InitTypeDef structure.
* Input          :- I2Cx: I2C peripheral can be:
*                    - I2C0
*					 - I2C1  
*
*                  - I2C_InitStruct: pointer to an I2C_InitTypeDef structure that
*                  contains the configuration information for the specified I2C
*                  peripheral.
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct)
{
  u16 wResult = 0x0F;
  u32 dPCLK = 25000000;

  /* Get PCLK frequency value */
 dPCLK = SCU_GetPCLKFreqValue()*1000;
  /* Disable I2C peripheral to set FR[2:0] bits */
  I2C_Cmd (I2Cx, DISABLE);
  /* Clear frequency FR[2:0] bits */
  I2Cx->OAR2 &= I2C_Frequency_Mask;
  /* Set frequency bits depending on PCLK value */
  if ((dPCLK <1667000) & (dPCLK > 10000000))
    I2Cx->OAR2 |= 0x20;
  else if (dPCLK < 26670000)
    I2Cx->OAR2 |= 0x40;
  else if (dPCLK < 40000000)
    I2Cx->OAR2 |= 0x60;
  else if (dPCLK < 53330000)
    I2Cx->OAR2 |= 0x80;
  else if (dPCLK < 66000000)
    I2Cx->OAR2 |= 0xA0;
  else if (dPCLK < 80000000)
    I2Cx->OAR2 |= 0xC0;
  else if (dPCLK < 100000000)
    I2Cx->OAR2 |= 0xE0;
  I2C_Cmd (I2Cx, ENABLE);

  /* Configure general call */
  if (I2C_InitStruct->I2C_GeneralCall == I2C_GeneralCall_Enable)
  {
    /* Enable general call */
    I2Cx->CR |= I2C_GeneralCall_Enable;
  }
  else
  {
    /* Disable general call */
    I2Cx->CR &= I2C_GeneralCall_Disable;
  }
  /* Configure acknowledgement */
  if (I2C_InitStruct->I2C_Ack == I2C_Ack_Enable)
  {
    /* Enable acknowledgement */
    I2Cx->CR |= I2C_Ack_Enable;
  }
  else
  {
    /* Disable acknowledgement */
    I2Cx->CR &= I2C_Ack_Disable;
  }

  /* Configure LSB own address */
  I2Cx->OAR1 = I2C_InitStruct->I2C_OwnAddress;
  /* Clear MSB own address ADD[9:8] bits */
  I2Cx->OAR2 &= I2C_AddressHigh_Mask;
  /* Set MSB own address value */
  I2Cx->OAR2 |= (I2C_InitStruct->I2C_OwnAddress & I2C_OwnAddress_Mask)>>7;

  /* Configure speed in standard mode */
  if (I2C_InitStruct->I2C_CLKSpeed <= 100000)
  {
    /* Standard mode speed calculate */
    wResult = ((dPCLK/I2C_InitStruct->I2C_CLKSpeed)-7)/2;
    /* Set speed value and clear FM/SM bit for standard mode in LSB clock divider */
    I2Cx->CCR = wResult & I2C_StandardMode_Mask;
  }
  /* Configure speed in fast mode */
  else if (I2C_InitStruct->I2C_CLKSpeed <= 400000)
  {
    /* Fast mode speed calculate */
    wResult = ((dPCLK/I2C_InitStruct->I2C_CLKSpeed)-9)/3;
    /* Set speed value and set FM/SM bit for fast mode in LSB clock divider */
    I2Cx->CCR = wResult | I2C_FastMode_Mask;
  }
  /* Set speed in MSB clock divider */
  I2Cx->ECCR = wResult >>7;
}

/*******************************************************************************
* Function Name  : I2C_StructInit		         		
* Description    : Initialize the I2C Init Structure parameters
* Input          : - I2C_InitStruct: pointer to an I2C_InitTypeDef structure
                     which will be initialized.
* Output         : None	
* Return         : None.						
*******************************************************************************/
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct)
{
  /* Initialize the I2C_CLKSpeed member */
  I2C_InitStruct->I2C_CLKSpeed = 5000;

  /* Initialize the I2C_OwnAddress member */
  I2C_InitStruct->I2C_OwnAddress = 0x0;

  /* Initialize the I2C_GeneralCall member */
  I2C_InitStruct->I2C_GeneralCall = I2C_GeneralCall_Disable;

  /* Initialize the I2C_Ack member */
  I2C_InitStruct->I2C_Ack = I2C_Ack_Disable;
}

/*******************************************************************************
* Function Name  : I2C_Cmd
* Description    : Enables or disables the specified I2C peripheral.
* Input          :- I2Cx: I2C peripheral can be:
*                    - I2C0
*					 - I2C1  
*                 - NewState: new state of the I2C peripheral. This parameter
*                    can be: ENABLE or DISABLE.
* Output         : None
* Return         : None.
*******************************************************************************/
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  if (NewState == ENABLE)
  {
    /* Enable the I2C peripheral by setting twice the PE bit on the CR register */
    I2Cx->CR |= I2C_PE_Set;
	  I2Cx->CR |= I2C_PE_Set;
  }
  else
  {
    /* Disable the I2C peripheral */
    I2Cx->CR &= I2C_PE_Reset;
  }
}

/*******************************************************************************
* Function Name  : I2C_GenerateSTART
* Description    : Generates I2C communication START condition.
* Input          :- I2Cx: I2C peripheral can be:
*                    - I2C0
*					 - I2C1  
*
*                 - NewState: new state of the Start condition. This parameter
*                    can be: ENABLE or DISABLE.
* Output         : None
* Return         : None.
*******************************************************************************/
void I2C_GenerateStart(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  if (NewState == ENABLE)
  {
    /* Generate a START condition */
    I2Cx->CR |= I2C_Start_Enable;
  }
  else
  {
    /* Disable the START condition generation */
    I2Cx->CR &= I2C_Start_Disable;
  }
}

/*******************************************************************************
* Function Name  : I2C_GenerateSTOP
* Description    : Generates I2C communication STOP condition.
* Input          :- I2Cx: I2C peripheral can be:
*                    - I2C0
*					 - I2C1  
*
*                  - NewState: new state of the Stop condition. This parameter
*                    can be: ENABLE or DISABLE.
* Output         : None
* Return         : None.
*******************************************************************************/
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  if (NewState == ENABLE)
  {
    /* Generate a SIOP condition */
    I2Cx->CR |= I2C_Stop_Enable;
  }
  else
  {
    /* Disable the STOP condition generation */
    I2Cx->CR &= I2C_Stop_Disable;
  }
}

/*******************************************************************************
* Function Name  : I2C_AcknowledgeConfig
* Description    : Enables or disables I2C acknowledge feature.
* Input          :- I2Cx: I2C peripheral can be:
*                    - I2C0
*					 - I2C1  
*                 - NewState: new state of the Acknowledgement. This parameter
*                    can be: ENABLE or DISABLE.
* Output         : None
* Return         : None.
*******************************************************************************/
void I2C_AcknowledgeConfig(I2C_TypeDef *I2Cx, FunctionalState NewState)
{
  if (NewState == ENABLE)
  {
    /* Enable the acknowledgement */
    I2Cx->CR |= I2C_Ack_Enable;
  }
  else
  {
    /* Disable the acknowledgement */
    I2Cx->CR &= I2C_Ack_Disable;
  }
}

/*******************************************************************************
* Function Name  : I2C_ITConfig
* Description    : Enables or disables I2C interrupt feature.
* Input          :- I2Cx: I2C peripheral can be:
*                    - I2C0
*					 - I2C1  
*                  - NewState: new state of the specified I2C interrupt.
*                    This parameter can be: ENABLE or DISABLE.
* Output         : None
* Return         : None.
*******************************************************************************/
void I2C_ITConfig(I2C_TypeDef *I2Cx, FunctionalState NewState)
{
  if (NewState == ENABLE)
  {
    /* Enable the I2C interrupt */
    I2Cx->CR |= I2C_IT_Enable;
  }
  else
  {
    /* Disable the I2C interrupt */
    I2Cx->CR &= I2C_IT_Disable;
  }
}

/*******************************************************************************
* Function Name  : I2C_ReadRegister
* Description    : Reads any I2C register and returns its value.
* Input          :- I2Cx: I2C peripheral can be:
*                    - I2C0
*					 - I2C1     
*                 - I2C_Register: the I2C register to be read. This parameter
*                    can be one of the following values:
*                         - I2C_CR:   CR register.
*                         - I2C_SR1:  SR1 register.
*                         - I2C_SR2:  SR2 register.
*                         - I2C_CCR:  CCR register.
*                         - I2C_OAR1: OAR1 register.
*                         - I2C_OAR2: OAR2 register.
*                         - I2C_DR:   DR register.
*                         - I2C_ECCR: ECCR register.
* Output         : None
* Return         : The value of the register passed as parameter
*******************************************************************************/
u8 I2C_ReadRegister(I2C_TypeDef* I2Cx, u8 I2C_Register)
{
  /* Return the selected register value */
  if (I2Cx == I2C0)
  {
  return (*(u8 *)(I2C0_BASE + I2C_Register));
  }
  if (I2Cx == I2C1)
  {
  return (*(u8 *)(I2C1_BASE + I2C_Register));
  }
 return 0;
}

/*******************************************************************************
* Function Name  : I2C_GetFlagStatus
* Description    : Checks whether the specified I2C flag is set or not.
* Input          :- I2Cx: I2C peripheral can be:
*                    - I2C0
*					 - I2C1  
*                 - I2C_FLAG: flag to check. This parameter can be one of the
*                    following values:
*                         - I2C_FLAG_SB: Start bit flag
*                         - I2C_FLAG_M_SL: Master/Slave flag
*                         - I2C_FLAG_ADSL: Adress matched flag
*                         - I2C_FLAG_BTF: Byte transfer finished flag
*                         - I2C_FLAG_BUSY: Bus busy flag
*                         - I2C_FLAG_TRA: Transmitter/Receiver flag
*                         - I2C_FLAG_ADD10: 10-bit addressing in Master mode flag
*                         - I2C_FLAG_EVF: Event flag
*                         - I2C_FLAG_GCAL: General call flag
*                         - I2C_FLAG_BERR: Bus error flag
*                         - I2C_FLAG_ARLO: Arbitration lost flag
*                         - I2C_FLAG_STOPF: Stop detection flag
*                         - I2C_FLAG_AF: Acknowledge failure flag
*                         - I2C_FLAG_ENDAD: End of address transmission flag
*                         - I2C_FLAG_ACK: Acknowledge enable flag
* Output         : None	
* Return         : The NewState of the I2C_Flag (SET or RESET).
*******************************************************************************/
FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, u16 I2C_FLAG)
{
  u16 wFlag1=0, wFlag2=0, wTmp=0;

  wFlag1 = I2Cx->SR2;
  wFlag1 = wFlag1<<8;
  wFlag2 = I2Cx->CR & 0x04;

  /* Get all the I2C flags in a unique register*/
  wTmp = (((I2Cx->SR1 | (wFlag1)) & I2C_Event_Mask) | (wFlag2<<12));

  /* Check the status of the specified I2C flag */
  if((wTmp & I2C_FLAG) != RESET)
  {
    /* Return SET if I2C_FLAG is set */
    return SET;
  }
  else
  {
    /* Return RESET if I2C_FLAG is reset */
    return RESET;
  }
}

/*******************************************************************************
* Function Name  : I2C_ClearFlag
* Description    : Clears the I2C Flag passed as a parameter 
* Input          :- I2Cx: I2C peripheral can be:
*                    - I2C0
*					 - I2C1
*                 - I2C_FLAG: flag to check. This parameter can be one of the
*                    following values:
*                         - I2C_FLAG_SB: Start bit flag
*                         - I2C_FLAG_M_SL: Master/Slave flag
*                         - I2C_FLAG_ADSL: Adress matched flag
*                         - I2C_FLAG_BTF: Byte transfer finished flag
*                         - I2C_FLAG_BUSY: Bus busy flag
*                         - I2C_FLAG_TRA: Transmitter/Receiver flag
*                         - I2C_FLAG_ADD10: 10-bit addressing in Master mode flag
*                         - I2C_FLAG_EVF: Event flag
*                         - I2C_FLAG_GCAL: General call flag
*                         - I2C_FLAG_BERR: Bus error flag
*                         - I2C_FLAG_ARLO: Arbitration lost flag
*                         - I2C_FLAG_STOPF: Stop detection flag
*                         - I2C_FLAG_AF: Acknowledge failure flag
*                         - I2C_FLAG_ENDAD: End of address transmission flag
*                         - I2C_FLAG_ACK: Acknowledge enable flag
*                  - parameter needed in the case that the flag to be cleared
*                    need a write in one register
* Output         : None	
* Return         : None.
*******************************************************************************/
void I2C_ClearFlag(I2C_TypeDef* I2Cx, u16 I2C_FLAG, ...)
{
  u8 bTmp = (u8)*((u32 *) & I2C_FLAG + sizeof(I2C_FLAG));

  /* flags that need a read of the SR2 register to be cleared */
  if ((I2C_FLAG==I2C_FLAG_ADD10) || (I2C_FLAG==I2C_FLAG_EVF) || (I2C_FLAG==I2C_FLAG_BERR) || (I2C_FLAG==I2C_FLAG_ARLO) |
      (I2C_FLAG==I2C_FLAG_STOPF) ||(I2C_FLAG==I2C_FLAG_AF)  || (I2C_FLAG==I2C_FLAG_ENDAD))
  {
    /* Read the SR2 register */
    I2Cx->SR2;

    /* Two flags need a second step to be cleared */
    switch (I2C_FLAG)
    {
       case  I2C_FLAG_ADD10:
         /* Send the MSB 10bit address passed as second parameter */
         I2Cx->DR = bTmp;
         break;
       case  I2C_FLAG_ENDAD:
         /* Write to the I2C_CR register by setting PE bit */
         I2Cx->CR |= I2C_PE_Set;
         break;
    }
  }

  /* flags that need a read of the SR1 register to be cleared */
  else if (I2C_FLAG==I2C_FLAG_SB || I2C_FLAG==I2C_FLAG_ADSL || I2C_FLAG==I2C_FLAG_BTF || I2C_FLAG==I2C_FLAG_TRA)
  {
    /* Read the SR1 register */
    (void)I2Cx->SR1;

    /* three flags need a second step to be cleared */
    if (I2C_FLAG == I2C_FLAG_SB)
    {
      /* Send the address byte passed as second parameter */
      I2Cx->DR = bTmp;
    }
    else if (I2C_FLAG==I2C_FLAG_BTF || I2C_FLAG==I2C_FLAG_TRA)
    {
      /* return the received byte in the variable passed as second parameter  */
      bTmp=I2Cx->DR;
    }
  }

  /* flags that need to disable the I2C interface */
  else if ( I2C_FLAG==I2C_FLAG_M_SL || I2C_FLAG==I2C_FLAG_GCAL)
  {
    I2C_Cmd(I2Cx, DISABLE);
    I2C_Cmd(I2Cx, ENABLE);
  }
}

/*******************************************************************************
* Function Name  : I2C_Send7bitAddress                                             
* Description    : Transmits the address byte to select the slave device.      
* Input          :- I2Cx: I2C peripheral can be:
*                    - I2C0
*					 - I2C1
*                 - Address: specifies the slave address which will be transmitted    
*                 - Direction: specifies whether the I2C device will be a 
*                    Transmitter or a Receiver. This parameter can be one of the 
*                    following values
*                         - I2C_MODE_TRANSMITTER: Transmitter mode
*                         - I2C_MODE_RECEIVER: Receiver mode  
* Output         : None	
* Return         : None.                                                       
*******************************************************************************/
void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, u8 Address, u8 Direction)
{
  /* Test on the direction to define the read/write bit */
  if (Direction == I2C_MODE_RECEIVER)
  {
    /* Set the address bit0 for read */
    Address |= I2C_ADD0_Set;
  }
  else
  {
    /* Reset the address bit0 for write */
    Address &= I2C_ADD0_Reset;
  }
  /* Send the address */
  I2Cx->DR = Address;
}

/*******************************************************************************
* Function Name  : I2C_SendData
* Description    : Send a data byte.
* Input          :- I2Cx: I2C peripheral can be:
*                    - I2C0
*					 - I2C1  
*                   - bData : the byte to be sent
* Output         : None	
* Return         : None.
*******************************************************************************/
void I2C_SendData(I2C_TypeDef* I2Cx, u8 bData)
{
  /* Write in the DR register the byte to be sent */
  I2Cx->DR = bData;
}

/*******************************************************************************
* Function Name  : I2C_ReceiveData
* Description    : Read the received byte.
* Input          : - I2Cx: I2C peripheral can be:
*                    - I2C0
*					 - I2C1
* Output         : None	
* Return         : The received byte
*******************************************************************************/
u8 I2C_ReceiveData(I2C_TypeDef* I2Cx)
{
  /* Return from the DR register the received byte */
  return I2Cx->DR;
}

/*******************************************************************************
* Function Name  : I2C_GetLastEvent
* Description    : Get the Last happened I2C Event.
* Input          :- I2Cx: I2C peripheral can be:
*                    - I2C0
*					 - I2C1  
* Output         : None	
* Return         : The Last happened Event.
*******************************************************************************/
u16 I2C_GetLastEvent(I2C_TypeDef* I2Cx)
{
  u16 wFlag1=0, wFlag2 =0, wLastEvent=0;
  wFlag2 = I2Cx->SR1;
  wFlag1 = I2Cx->SR2;
  wFlag1 = wFlag1<<8;
  /* Get the last event value from I2C status register */
  wLastEvent = ((( wFlag2 | (wFlag1)) & I2C_Event_Mask));
  /* Return the last event */
  return wLastEvent;
}

/*******************************************************************************
* Function Name  : I2C_CheckEvent
* Description    : Checks whether the Last I2C Event is equal to the one passed
*                  as parameter.
* Input          :- I2Cx: I2C peripheral can be:
*                    - I2C0
*					 - I2C1   
*                  - I2C_EVENT: the event to check. This parameter can be one of
*                    the following values:
*                         - I2C_EVENT_SLAVE_ADDRESS_MATCHED
*                         - I2C_EVENT_SLAVE_BYTE_RECEIVED
*                         - I2C_EVENT_SLAVE_BYTE_TRANSMITTED
*                         - I2C_EVENT_MASTER_MODE_SELECT
*                         - I2C_EVENT_MASTER_MODE_SELECTED
*                         - I2C_EVENT_MASTER_BYTE_RECEIVED
*                         - I2C_EVENT_MASTER_BYTE_TRANSMITTED
*                         - I2C_EVENT_MASTER_MODE_ADDRESS10
*                         - I2C_EVENT_SLAVE_STOP_DETECTED
*                         - I2C_EVENT_SLAVE_ACK_FAILURE
* Output         : None	
* Return         : An ErrorStatus enumuration value:
*                         - SUCCESS: Last event is equal to the I2C_Event
*                         - ERROR: Last event is different from the I2C_Event
*******************************************************************************/
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx,u16 I2C_EVENT)
{
  u16  wLastEvent = I2C_GetLastEvent(I2Cx);

  /* Check whther the last event is equal to I2C_EVENT */
  if (wLastEvent == I2C_EVENT)
  {
    /* Return SUCCESS when last event is equal to I2C_EVENT */
    return SUCCESS;
  }
  else
  {
    /* Return ERROR when last event is different from I2C_EVENT */
    return ERROR;
  }
}

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
