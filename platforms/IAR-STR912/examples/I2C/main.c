/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : Main program body
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
#include "91x_lib.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Init Structure definition */
 I2C_InitTypeDef   I2C_Struct;
 GPIO_InitTypeDef  GPIO_Struct;
 
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE 4
#define I2C0OwnAddr   0xA0
#define I2C1OwnAddr   0xA0
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Tx_Idx=0, Rx_Idx=0, Direction;
u8 I2C0_Buffer_Tx[BUFFER_SIZE], I2C0_Buffer_Rx[BUFFER_SIZE]={0, 0, 0, 0};
u8 I2C1_Buffer_Tx[BUFFER_SIZE], I2C1_Buffer_Rx[BUFFER_SIZE]={0, 0, 0, 0};
TestStatus TransferStatus1, TransferStatus2;

/* Private function prototypes -----------------------------------------------*/
void GPIO_Configuration(void);
void SCU_Configuration(void);
void VIC_Configuration(void);
void Delay(u32 Xtime);
void Fill_Buffer(u8 *Buffer, u8 Data);
TestStatus Buffercmp(u8* pBuffer, u8* pBuffer1, u16 BufferLength);

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
  
#ifdef DEBUG
    debug();
#endif

  SCU_MCLKSourceConfig(SCU_MCLK_OSC);    /*Use OSC as the default clock source*/
  SCU_PCLKDivisorConfig(SCU_PCLK_Div1);  /* ARM Peripheral bus clokdivisor = 1*/
  
  /* SCU configuration */
  SCU_Configuration();
  /* GPIO pins configuration */
  GPIO_Configuration();
  /* VIC configuration */
  VIC_Configuration();
  
  /*I2C0 & I2C1 reset */
  I2C_DeInit(I2C0);
  I2C_DeInit(I2C1);
   
  /* Enable I2C0, I2C1 */
  I2C_Cmd(I2C0, ENABLE);
  I2C_Cmd(I2C1, ENABLE);
  /* Configure GPIO2, I2C0 and I2C1 */
  /* I2C0 Configuration */
  I2C_Struct.I2C_GeneralCall = I2C_GeneralCall_Disable;
  I2C_Struct.I2C_Ack = I2C_Ack_Enable;
  I2C_Struct.I2C_CLKSpeed = 400000;
  I2C_Struct.I2C_OwnAddress = I2C0OwnAddr;
  I2C_Init(I2C0, &I2C_Struct);

  /* I2C1 Configuration */
  /* we keep the same config as I2C0 for the other I2C_Struct members */ 
  /* We change just the address*/ 

  I2C_Struct.I2C_OwnAddress = I2C1OwnAddr;
  I2C_Init(I2C1, &I2C_Struct);
  Direction = I2C_MODE_TRANSMITTER;
  Fill_Buffer(I2C1_Buffer_Tx, 0x1);
  I2C_ITConfig(I2C1, ENABLE);
  I2C_ITConfig(I2C0, ENABLE);
  I2C_GenerateStart(I2C1, ENABLE);
  while (Tx_Idx < BUFFER_SIZE+1);
  /* Check if the transmitted data is read correctly */
  TransferStatus1 = Buffercmp(I2C0_Buffer_Rx, I2C1_Buffer_Tx, BUFFER_SIZE);
  /* TransferStatus = PASSED, if the transmitted from I2C1 and received data 
     by the I2C0 are the same */
  /* TransferStatus = FAILED, if the transmitted from I2C1 and received data 
     by the I2C0 are different */
  /*--------------------------------------------------*/
  /* Delay between transmission and reception --------*/
  /*--------------------------------------------------*/
  Delay(100000);

  /*--------------------------------------------------*/
  /* Reception Phase----------------------------------*/
  /*--------------------------------------------------*/
  /*reset counters*/
  Tx_Idx = Rx_Idx = 0;
  Direction = I2C_MODE_RECEIVER;
  Fill_Buffer(I2C0_Buffer_Tx, 0x5);
  I2C_GenerateStart(I2C1, ENABLE);
  while (Tx_Idx < BUFFER_SIZE+1);
  /* Check if the transmitted data is read correctly */
  TransferStatus2 = Buffercmp(I2C1_Buffer_Rx, I2C0_Buffer_Tx, BUFFER_SIZE);
  /* TransferStatus = PASSED, if the transmitted from I2C0 and received data 
     by the I2C1 are the same */
  /* TransferStatus = FAILED, if the transmitted from I2C0 and received data 
     by the I2C1 are different */

  while(1);
}
/*******************************************************************************
Function name : void Delay(void)
Description   : Delay
Input param   : None
Output param  : None
*******************************************************************************/
void Delay(u32 Xtime)
{
  u32 j;

  for(j=Xtime; j>0; j--);
}

/*******************************************************************************
* Function Name  : Fill_Buffer
* Description    : Fill buffer with corresponding Pattern
* Input          : - Buffer: buffers to be filled.
*                  - Data : Data Pattern
* Output         : None
* Return         :
*******************************************************************************/
void Fill_Buffer(u8  *Buffer, u8 Data)
{
  u8 IndexTmp;

  /* Put in global buffer same values */
  for( IndexTmp = 0; IndexTmp < BUFFER_SIZE; IndexTmp++ )
  {
    Buffer[IndexTmp] =Data++;
  }
}

/*******************************************************************************
* Function Name  : Buffercmp
* Description    : Compares two buffers.
* Input          : - pBuffer, pBuffer1: buffers to be compared.
*                : - BufferLength: buffer's length
* Output         : None
* Return         : PASSED: pBuffer identical to pBuffer1
*                  FAILED: pBuffer differs from pBuffer1
*******************************************************************************/
TestStatus Buffercmp(u8* pBuffer, u8* pBuffer1, u16 BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer != *pBuffer1)
    {
      return FAILED;
    }
    
    pBuffer++;
    pBuffer1++;
  }

  return PASSED;  
}


/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configure the used I/O ports pins
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_Struct;

 /* GPIO2 Config */
  GPIO_DeInit(GPIO2);
  GPIO_Struct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3  ;
  GPIO_Struct.GPIO_Type = GPIO_Type_OpenCollector;
  GPIO_Struct.GPIO_IPConnected = GPIO_IPConnected_Enable;
  GPIO_Struct.GPIO_Alternate=GPIO_OutputAlt2;
  GPIO_Init(GPIO2, &GPIO_Struct);
}

/*******************************************************************************
* Function Name  : SCU_Configuration
* Description    : Configure the different system clocks
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SCU_Configuration(void)
{
  /*clock enable for used peripherals*/

  SCU_APBPeriphClockConfig(__I2C0,ENABLE);
  SCU_APBPeriphClockConfig(__I2C1,ENABLE);
  SCU_APBPeriphClockConfig(__GPIO2, ENABLE);
  SCU_AHBPeriphClockConfig(__VIC, ENABLE);
}

/*******************************************************************************
* Function Name  : VIC_Configuration
* Description    : Configure the used I/O ports pins
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void VIC_Configuration(void)
{
  /* VIC Configuration */

  VIC_DeInit();
  VIC_Config(I2C0_ITLine,VIC_IRQ , 0);
  VIC_Config(I2C1_ITLine,VIC_IRQ , 1);
  VIC_ITCmd(I2C0_ITLine, ENABLE);
  VIC_ITCmd(I2C1_ITLine, ENABLE);
}
/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
