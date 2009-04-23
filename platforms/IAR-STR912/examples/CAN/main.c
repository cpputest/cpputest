/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file contains the software implementation for the
*                      CAN software library verification.
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


/* Standard include ----------------------------------------------------------*/
#include "91x_lib.h"
/* Include of other module interface headers ---------------------------------*/
/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
canmsg RxCanMsg;
canmsg CanMsg;

GPIO_InitTypeDef    GPIO_InitStructure;
CAN_InitTypeDef     CAN_InitStructure;

  enum {
    LD1,
    LD2,
    LD3,
    LD4,
   };

typedef  enum {
    NA,
    Priority_1,
    Priority_2,
    Priority_3
  }VIC_Priority;

  /* buffer for receive messages */
  canmsg RxCanMsg;

  /* used message object numbers */
  enum {
	CAN_TX_MSGOBJ = 0,
	CAN_RX_MSGOBJ = 1
  };

  /* array of pre-defined transmit messages */
  canmsg TxCanMsg[3] = {
	{ CAN_STD_ID,      0x123, 4, { 0x01, 0x02, 0x04, 0x08 } },
	{ CAN_STD_ID,      0x321, 4, { 0xAA, 0x55, 0xAA, 0x55 } },
	{ CAN_EXT_ID, 0x12345678, 8, { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 } }
  };

/* Private functions ---------------------------------------------------------*/
void System_Setup(void)
{
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);
  SCU_PCLKDivisorConfig(SCU_PCLK_Div2);
  SCU_PLLFactorsConfig(128,25,4);
  SCU_PLLCmd(ENABLE);
  SCU_MCLKSourceConfig(SCU_MCLK_PLL);

  SCU_APBPeriphClockConfig(__CAN, ENABLE);
  SCU_APBPeriphClockConfig(__GPIO0, ENABLE);
  SCU_APBPeriphClockConfig(__GPIO1, ENABLE);
  SCU_APBPeriphClockConfig(__GPIO3, ENABLE);
  SCU_APBPeriphClockConfig(__GPIO5, ENABLE);
   SCU_AHBPeriphClockConfig(__VIC, ENABLE);

  SCU_APBPeriphReset(__CAN, DISABLE);
  SCU_APBPeriphReset(__GPIO0, DISABLE);
  SCU_APBPeriphReset(__GPIO1, DISABLE);
  SCU_APBPeriphReset(__GPIO3, DISABLE);
  SCU_APBPeriphReset(__GPIO5, DISABLE);
  SCU_AHBPeriphReset(__VIC, DISABLE);
}

void IO_Init(void)
{
/* P3.0, P3.1, P3.2 and P3.3 output*/
GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
GPIO_InitStructure.GPIO_Direction=GPIO_PinOutput;
GPIO_InitStructure.GPIO_Type=GPIO_Type_PushPull;
GPIO_InitStructure.GPIO_Alternate=GPIO_OutputAlt1;
GPIO_InitStructure.GPIO_IPConnected=GPIO_IPConnected_Disable;
GPIO_Init(GPIO3,&GPIO_InitStructure);

/* P5.0 alternate input 1, CAN_RX pin 12*/
GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
GPIO_InitStructure.GPIO_Direction=GPIO_PinInput;
GPIO_InitStructure.GPIO_Type=GPIO_Type_PushPull;
GPIO_InitStructure.GPIO_IPConnected=GPIO_IPConnected_Enable;
GPIO_InitStructure.GPIO_Alternate=GPIO_InputAlt1;
GPIO_Init(GPIO5,&GPIO_InitStructure);

/* P3.6 alternate output 2, CAN_TX pin 66*/
GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
GPIO_InitStructure.GPIO_Direction=GPIO_PinOutput;
GPIO_InitStructure.GPIO_Type=GPIO_Type_PushPull;
GPIO_InitStructure.GPIO_IPConnected=GPIO_IPConnected_Enable;
GPIO_InitStructure.GPIO_Alternate=GPIO_OutputAlt3;
GPIO_Init(GPIO3,&GPIO_InitStructure);

}

void LED_ON(u8 LED)
{
  switch (LED)
  {
    case LD1:   GPIO_WriteBit(GPIO3, GPIO_Pin_0, Bit_RESET);  break;
    case LD2:   GPIO_WriteBit(GPIO3, GPIO_Pin_1, Bit_RESET);  break;
    case LD3:   GPIO_WriteBit(GPIO3, GPIO_Pin_2, Bit_RESET);  break;
    case LD4:   GPIO_WriteBit(GPIO3, GPIO_Pin_3, Bit_RESET);  break;
    default:;
  }
}

void LED_OFF(u8 LED)
{
  switch (LED)
  {
    case LD1:   GPIO_WriteBit(GPIO3, GPIO_Pin_0, Bit_SET);  break;
    case LD2:   GPIO_WriteBit(GPIO3, GPIO_Pin_1, Bit_SET);  break;
    case LD3:   GPIO_WriteBit(GPIO3, GPIO_Pin_2, Bit_SET);  break;
    case LD4:   GPIO_WriteBit(GPIO3, GPIO_Pin_3, Bit_SET);  break;
    default:;
  }
}

void delay(u32 XTime)
{
  vu32 j;

  for(j=0;j<XTime;j++);
}

void CAN_Com_LoopBack(void)
{
  /* initialize the CAN at a standard bitrate, interrupts disabled */
  CAN_InitStructure.CAN_ConfigParameters=0x0;
  CAN_InitStructure.CAN_Bitrate=CAN_BITRATE_1M;
  CAN_Init(&CAN_InitStructure);

  /* switch into Loopback+Silent mode (self-test) */
  CAN_EnterTestMode(CAN_TESTR_LBACK | CAN_TESTR_SILENT);

  /* configure the message objects */
  CAN_InvalidateAllMsgObj();
  CAN_SetTxMsgObj(CAN_TX_MSGOBJ, CAN_STD_ID);
  CAN_SetRxMsgObj(CAN_RX_MSGOBJ, CAN_STD_ID, 0, CAN_LAST_STD_ID, TRUE);

  /* Send the pre-defined answer */
  CAN_SendMessage(CAN_TX_MSGOBJ, &TxCanMsg[1]);

  /* wait until end of transmission */
  CAN_WaitEndOfTx();

  /* wait for reception of a data frame */
  while (!CAN_ReceiveMessage(CAN_RX_MSGOBJ, FALSE, &RxCanMsg))
  {
    /*Add Timer*/
  }


  /* Test Received Msg */
  if((RxCanMsg.IdType == CAN_STD_ID)&&(RxCanMsg.Id == 0x321)&&(RxCanMsg.Dlc == 4)
    &&(RxCanMsg.Data[0]==0xAA)&&(RxCanMsg.Data[1]==0x55)&&(RxCanMsg.Data[2]==0xAA)&&(RxCanMsg.Data[3]==0x55)){
    /*Received Msg OK*/
    LED_ON(LD2);
  } else {
    /*Received Msg KO*/
    LED_ON(LD4);
  }


  /* release the message objects */
  CAN_ReleaseTxMessage(CAN_TX_MSGOBJ);
  CAN_ReleaseRxMessage(CAN_RX_MSGOBJ);

  /* switch back into Normal mode */
  CAN_LeaveTestMode();

}

void CAN_Com_LoopBack_IRQ(void)
{
  /* initialize the interrupt controller */
  VIC_Config(CAN_ITLine, VIC_IRQ, Priority_1);

  /* initialize the CAN at a standard bitrate, interrupts enabled */
  CAN_InitStructure.CAN_ConfigParameters=CAN_CR_IE;
  CAN_InitStructure.CAN_Bitrate=CAN_BITRATE_100K;
  CAN_Init(&CAN_InitStructure);

  /* switch into Loopback+Silent mode (self-test) */
  CAN_EnterTestMode(CAN_TESTR_LBACK | CAN_TESTR_SILENT);

  /* configure the message objects */
  CAN_InvalidateAllMsgObj();
  CAN_SetTxMsgObj(CAN_TX_MSGOBJ, CAN_EXT_ID);
  CAN_SetRxMsgObj(CAN_RX_MSGOBJ, CAN_EXT_ID, 0, CAN_LAST_EXT_ID, TRUE);

  /* enable global interrupt */
  VIC_ITCmd(CAN_ITLine, ENABLE);

  /* Send the pre-defined frame */
  CAN_SendMessage(CAN_TX_MSGOBJ, &TxCanMsg[2]);

  /* wait until end of transmission */
  CAN_WaitEndOfTx();

  /* reception and release are done in the interrupt handler */

  /* delay to add when the reception occurs */
  delay(0x7FF);

  /* Test Received Msg */
  if((RxCanMsg.IdType == CAN_EXT_ID)&&(RxCanMsg.Id == 0x12345678)&&(RxCanMsg.Dlc == 8)
    &&(RxCanMsg.Data[0]==0x10)&&(RxCanMsg.Data[1]==0x11)&&(RxCanMsg.Data[2]==0x12)&&(RxCanMsg.Data[3]==0x13)
    &&(RxCanMsg.Data[4]==0x14)&&(RxCanMsg.Data[5]==0x15)&&(RxCanMsg.Data[6]==0x16)&&(RxCanMsg.Data[7]==0x17)){
    /*Received Msg OK*/
    LED_ON(LD3);
  } else {
    /*Received Msg KO*/
    LED_ON(LD4);
  }


  /* switch back into Normal mode */
  CAN_LeaveTestMode();

  /* disable interrupts globally */
  VIC_ITCmd(CAN_ITLine, DISABLE);
}

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
  
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);         /*Use OSC as the default clock source*/
  SCU_PCLKDivisorConfig(SCU_PCLK_Div1);      /* ARM Peripheral bus clokdivisor = 1*/
    
    
  System_Setup();/*PCLK=8MHz*/

  IO_Init();

  LED_OFF(LD1);
  LED_OFF(LD2);
  LED_OFF(LD3);
  LED_OFF(LD4);

  /* CAN tests */
  CAN_Com_LoopBack();/* send and receive some frame */
  CAN_Com_LoopBack_IRQ();/* send and receive some frame, received in 91x_it.c */

  /* LD2, LD3 should be lit */
  /* If LD4 lit so something's wrong */

	return 0;
}


/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/







