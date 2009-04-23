/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : main program for DMA example.
*
*                     This example use the DMA Linked list items to perform
*                      a scattering of a "Buffer0"  in the ram to three other 
*					   buffers " Buffer1,Buffer2, Buffer3" in another location 
*					   in memory.
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

DMA_InitTypeDef  DMA_InitStruct;

/* The Link buffer contains the two next LLI, the First LLI is programmed on the DMA chaneel register before starting the transfer*/
/* The LLI is a structure of four registers : source address register, Destination address register, the Address of the next LLI structure register and the last register is for the Channel control register*/
/* The Channel control register ," last LLI element", contains all channel config :Flow controller, Src bus width,Dest bus width...*/
/* The value:0x0C48900C reflect the channel config: transfer size=12, src width= word, Dest width= word.., please refer to the DMA_CCx Control register on the STR9 UM for more details*/

volatile u32 Link[12] ={0x0,0x0,0x0,0x0C48900C,0x0,0x0,0x0,0x0C48900C};

/* The first Buffer to be transfered to the other three Buffers ( source Buffer)*/
volatile u32 Buffer0[36] ={0,1,2,3,4,5,6,7,8,9,10,11,0,1,2,3,4,5,6,7,8,9,10,11,0,1,2,3,4,5,6,7,8,9,10,11};



/* The Thrre Destination buffers*/

volatile u32 Buffer1[12] ={0,0,0,0,0,0,0,0,0,0,0,0};
volatile u32 Buffer2[12] ={0,0,0,0,0,0,0,0,0,0,0,0};
volatile u32 Buffer3[12] ={0,0,0,0,0,0,0,0,0,0,0,0};


/* Private functions ---------------------------------------------------------*/

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

   SCU_MCLKSourceConfig(SCU_MCLK_OSC);      /*Use OSC as the default clock source*/
   SCU_PCLKDivisorConfig(SCU_PCLK_Div1);    /* ARM Peripheral bus clokdivisor = 1*/
   
  /* source addresses and destination addresses for the Second LLI structure */
   Link[0]=(u32)(&Buffer0[12]);
   Link[1]=(u32)(&Buffer2[0]);

  /* source addresses and destination addresses for the Third LLI structure */
   Link[4]=(u32)(&Buffer0[24]);
   Link[5]=(u32)(&Buffer3[0]);

   /*Set the addresses of next linked list for the second LLI structure*/
   Link[2]=(u32)(&Link[4]);

   SCU_AHBPeriphClockConfig(__DMA,ENABLE); /* Enable the clock for DMA*/
   DMA_DeInit(); /* DMA default configuration : Reset configuration*/
   DMA_Cmd(ENABLE);/*Enable the DMA*/
   DMA_StructInit(&DMA_InitStruct);

   /* Write the first LLI*/
   DMA_InitStruct.DMA_Channel_LLstItm=(u32)(&Link[0]); /*Set the addresses of next linked list for the first LLI structure*/
   DMA_InitStruct.DMA_Channel_SrcAdd=(u32)(&Buffer0[0]); /* source address for the first LLI structure */
   DMA_InitStruct.DMA_Channel_DesAdd=(u32)(&Buffer1[0]); /*Destination address for the first LLI structure */
   DMA_InitStruct.DMA_Channel_SrcWidth= DMA_SrcWidth_Word;/* The source bus width is a word" 32 bits"*/
   DMA_InitStruct.DMA_Channel_DesWidth= DMA_DesWidth_Word; /* The Destination bus width is a word word*/
   DMA_InitStruct.DMA_Channel_FlowCntrl=DMA_FlowCntrlt0_DMA;/* DMA is The flow controller*/
   DMA_InitStruct.DMA_Channel_TrsfSize =12; /*transfer size*/

  /* Configure the DMA channel1 "the chosen channel to perform the transfer" */

  DMA_ChannelSRCIncConfig (DMA_Channel1, ENABLE);
  DMA_ChannelDESIncConfig (DMA_Channel1, ENABLE);


  DMA_Init(DMA_Channel1,&DMA_InitStruct);/* update the DMA channel1 registers with the cfirst LLI structure*/
  DMA_ChannelCmd (DMA_Channel1,ENABLE);/*Enable the DMA channel*/



  /*wait for the fifo to be empty*/
  while(DMA_GetChannelActiveStatus(DMA_Channel1));

 while(1);

}

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
