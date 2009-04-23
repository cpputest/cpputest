/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : 91x_i2c.h
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file contains all the functions prototypes for the
*                      I2C software library.
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

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __91x_I2C_H
#define __91x_I2C_H

/* Includes ----------------------------------------------------------------- */
#include "91x_map.h"

/* Exported types ----------------------------------------------------------- */
/* I2C Init structure definition */
typedef struct
{
   u32 I2C_CLKSpeed;
   u16 I2C_OwnAddress;
   u8  I2C_GeneralCall;
   u8  I2C_Ack;
}I2C_InitTypeDef;

/* Exported constants --------------------------------------------------------*/
#define  I2C_GeneralCall_Enable      0x10
#define  I2C_GeneralCall_Disable     0xEF
/* Acknowledgement */
#define  I2C_Ack_Enable      0x04
#define  I2C_Ack_Disable     0xFB

/* I2C Flags */
#define  I2C_FLAG_SB      0x0001
#define  I2C_FLAG_M_SL    0x0002
#define  I2C_FLAG_ADSL    0x0004
#define  I2C_FLAG_BTF     0x0008
#define  I2C_FLAG_BUSY    0x0010
#define  I2C_FLAG_TRA     0x0020
#define  I2C_FLAG_ADD10   0x0040
#define  I2C_FLAG_EVF     0x0080
#define  I2C_FLAG_GCAL    0x0100
#define  I2C_FLAG_BERR    0x0200
#define  I2C_FLAG_ARLO    0x0400
#define  I2C_FLAG_STOPF   0x0800
#define  I2C_FLAG_AF      0x1000
#define  I2C_FLAG_ENDAD   0x2000
#define  I2C_FLAG_ACK     0x4000

/* I2C Events */
#define  I2C_EVENT_SLAVE_ADDRESS_MATCHED   ( I2C_FLAG_EVF | I2C_FLAG_BUSY | I2C_FLAG_ADSL)
#define  I2C_EVENT_SLAVE_BYTE_RECEIVED     ( I2C_FLAG_EVF | I2C_FLAG_BUSY | I2C_FLAG_BTF )
#define  I2C_EVENT_SLAVE_BYTE_TRANSMITTED  ( I2C_FLAG_EVF | I2C_FLAG_BUSY | I2C_FLAG_BTF  | I2C_FLAG_TRA )
#define  I2C_EVENT_MASTER_MODE_SELECT      ( I2C_FLAG_EVF | I2C_FLAG_BUSY | I2C_FLAG_M_SL | I2C_FLAG_SB )
#define  I2C_EVENT_MASTER_MODE_SELECTED    ( I2C_FLAG_EVF | I2C_FLAG_BUSY | I2C_FLAG_M_SL | I2C_FLAG_ENDAD )
#define  I2C_EVENT_MASTER_BYTE_RECEIVED    ( I2C_FLAG_EVF | I2C_FLAG_BUSY | I2C_FLAG_M_SL | I2C_FLAG_BTF )
#define  I2C_EVENT_MASTER_BYTE_TRANSMITTED ( I2C_FLAG_EVF | I2C_FLAG_BUSY | I2C_FLAG_M_SL | I2C_FLAG_BTF | I2C_FLAG_TRA )
#define  I2C_EVENT_MASTER_MODE_ADDRESS10   ( I2C_FLAG_EVF | I2C_FLAG_BUSY | I2C_FLAG_M_SL |I2C_FLAG_ADD10 )
#define  I2C_EVENT_SLAVE_STOP_DETECTED     ( I2C_FLAG_EVF | I2C_FLAG_STOPF )
#define  I2C_EV31                          ( I2C_FLAG_EVF | I2C_FLAG_BUSY | I2C_FLAG_BTF  | I2C_FLAG_TRA | I2C_FLAG_AF)
#define  I2C_EVENT_SLAVE_ACK_FAILURE       (  I2C_FLAG_EVF | I2C_FLAG_BUSY |I2C_FLAG_AF)
#define  I2C_EVENT_ALL                     (  I2C_FLAG_EVF | I2C_FLAG_BUSY |I2C_FLAG_AF |I2C_FLAG_BTF)
#define  I2C_BUS_ERROR_DETECTED       I2C_FLAG_BERR
#define  I2C_ARBITRATION_LOST         I2C_FLAG_ARLO
#define  I2C_SLAVE_GENERAL_CALL       (I2C_FLAG_BUSY | I2C_FLAG_GCAL)

/* Master/Receiver Mode */
#define  I2C_MODE_TRANSMITTER          0x00
#define  I2C_MODE_RECEIVER             0x01

/* I2C Registers offset */
#define  I2C_CR     0x00
#define  I2C_SR1    0x04
#define  I2C_SR2    0x08
#define  I2C_CCR    0x0C
#define  I2C_OAR1   0x10
#define  I2C_OAR2   0x14
#define  I2C_DR     0x18
#define  I2C_ECCR   0x1C

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void I2C_DeInit(I2C_TypeDef* I2Cx);
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct);
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct);
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_GenerateStart(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_AcknowledgeConfig(I2C_TypeDef *I2Cx, FunctionalState NewState);
void I2C_ITConfig(I2C_TypeDef *I2Cx, FunctionalState NewState);
void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, u8 Address, u8 Direction);
u8 I2C_ReadRegister(I2C_TypeDef* I2Cx, u8 I2C_Register);
FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, u16 I2C_FLAG);
void I2C_ClearFlag(I2C_TypeDef* I2Cx, u16 I2C_FLAG, ...);
void I2C_SendData(I2C_TypeDef* I2Cx, u8 bData);
u8 I2C_ReceiveData(I2C_TypeDef* I2Cx);
u16 I2C_GetLastEvent(I2C_TypeDef* I2Cx);
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx,u16 I2C_Event);

#endif /* __91x_I2C_H */

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
