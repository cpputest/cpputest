//Based on....
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
/* Private define ------------------------------------------------------------*/
#define TxBufferSize   (countof(TxBuffer) - 1)
#define RxBufferSize   0xFF

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
  UART_InitTypeDef UART_InitStructure;
  u8 TxBuffer[] = "\n\rUART Example1: UART - Hyperterminal communication using hardware flow control\n\r";
  u8 RxBuffer[RxBufferSize];
  u8 NbrOfDataToTransfer = TxBufferSize;
  u8 TxCounter = 0;
  u8 RxCounter = 0;

/* Private function prototypes -----------------------------------------------*/
  void SCU_Configuration(void);
  void GPIO_Configuration(void);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//was main() - JWG
int uartInit()
{
 SCU_MCLKSourceConfig(SCU_MCLK_OSC);     /*Use OSC as the default clock source*/
 SCU_PCLKDivisorConfig(SCU_PCLK_Div1);  /* ARM Peripheral bus clokdivisor = 1*/
  
/* Configure the system clocks */
  SCU_Configuration();

  /* Configure the GPIO ports */
  GPIO_Configuration();

  /* UART0 configuration -------------------------------------------------------*/
  /* UART0 configured as follow:
        - Word Length = 7 Bits
        - Two Stop Bit
        - No parity
        - BaudRate = 115200 baud
        - Hardware flow control disabled
        - Receive and transmit enabled
        - Receive and transmit FIFOs are enabled
        - Transmit and Receive FIFOs levels have 8 bytes depth
  */
  UART_InitStructure.UART_WordLength = UART_WordLength_7D;
  UART_InitStructure.UART_StopBits = UART_StopBits_2;
  UART_InitStructure.UART_Parity = UART_Parity_No ;
  UART_InitStructure.UART_BaudRate = 115200;
  UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
  UART_InitStructure.UART_Mode = UART_Mode_Tx_Rx;
  UART_InitStructure.UART_FIFO = UART_FIFO_Enable;
  UART_InitStructure.UART_TxFIFOLevel = UART_FIFOLevel_1_2; /* FIFO size 16 bytes, FIFO level 8 bytes */
  UART_InitStructure.UART_RxFIFOLevel = UART_FIFOLevel_1_2; /* FIFO size 16 bytes, FIFO level 8 bytes */

  UART_DeInit(UART0);
  UART_Init(UART0, &UART_InitStructure);

  /* Enable the UART0 */
  UART_Cmd(UART0, ENABLE);


  /* Communication  hyperterminal-UART0 using the hardware flow control */
  /* Send a buffer from UART to hyperterminal */
  return 0;
}

int putchar(int c)
{
    while(UART_GetFlagStatus(UART0, UART_FLAG_TxFIFOFull) != RESET);
    UART_SendData(UART0, c);
    return c;
}

void uartPut(char* msg)
{
  while(*msg != 0)
  {
    putchar(*msg++);
  }
}

int uartEcho()
{
  uartPut("Echo keypress mode:");

  do
  {
    if((UART_GetFlagStatus(UART0, UART_FLAG_RxFIFOEmpty) != SET)&&(RxCounter < RxBufferSize))
    {
       RxBuffer[RxCounter] = UART0->DR;
       UART_SendData(UART0, RxBuffer[RxCounter++]);
    }

  }while((RxBuffer[RxCounter - 1] != '@')&&(RxBuffer[RxCounter - 1] != '\r')&&(RxCounter != RxBufferSize));

  return 1;
}



/*******************************************************************************
* Function Name  : SCU_Configuration
* Description    : Configures the system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SCU_Configuration(void)
{

  /* Enable the UART0 Clock */
  SCU_APBPeriphClockConfig(__UART0, ENABLE);

  /* Enable the GPIO3 Clock */
  SCU_APBPeriphClockConfig(__GPIO3, ENABLE);

  /* Enable the GPIO5 Clock */
  SCU_APBPeriphClockConfig(__GPIO5, ENABLE);

}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
   
  GPIO_DeInit(GPIO3);
  /*Gonfigure UART0_Tx pin GPIO3.4*/
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull ;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt3  ;
  GPIO_Init (GPIO3, &GPIO_InitStructure);
  
  GPIO_DeInit(GPIO5);
  /*Gonfigure UART0_Rx pin GPIO5.1*/
  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull ;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1  ;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Enable;
  GPIO_Init (GPIO5, &GPIO_InitStructure);
}

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/

