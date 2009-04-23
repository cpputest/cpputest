/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : Short Example, Program using the FMI.
*                      The purpose of this program is to erase one sector 
*                      then write some data to this sector.
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

u8 FMI_Timeout_Status;

/* Private function prototypes -----------------------------------------------*/
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
  
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);    /*Use OSC as the default clock source*/
  SCU_PCLKDivisorConfig(SCU_PCLK_Div1); /* ARM Peripheral bus clokdivisor = 1*/
  
  SCU_AHBPeriphClockConfig(__FMI, ENABLE);
  SCU_AHBPeriphReset(__FMI, DISABLE);

/* -------------------------------------------------------------------------- */
/*                Configure bank 0 as 512KB and bank 1 as 32KB                */
/*        Configure bank 0 at address 0x0 and bank 1 at address 0x80000       */
/* -------------------------------------------------------------------------- */

  FMI_BankRemapConfig(0x4, 0x2, 0x0, 0x80000);

/* -------------------------------------------------------------------------- */
/*             Disable the Write protection for sector 0 of bank 1            */
/* -------------------------------------------------------------------------- */ 

  FMI_WriteProtectionCmd(FMI_B1S0, DISABLE);
  
/* -------------------------------------------------------------------------- */
/*                        Erase the sector 0 of bank 1                        */
/* -------------------------------------------------------------------------- */ 

  FMI_EraseSector(FMI_B1S0);
  FMI_Timeout_Status = FMI_WaitForLastOperation(FMI_BANK_1);

/* -------------------------------------------------------------------------- */
/*                                 Write Data                                 */
/* -------------------------------------------------------------------------- */ 
  FMI_WriteHalfWord(0x80000,0x1111);
  FMI_Timeout_Status = FMI_WaitForLastOperation(FMI_BANK_1);

  FMI_WriteHalfWord(0x80002,0x1111);
  FMI_Timeout_Status = FMI_WaitForLastOperation(FMI_BANK_1);

  FMI_WriteHalfWord(0x80004,0x2222);
  FMI_Timeout_Status = FMI_WaitForLastOperation(FMI_BANK_1);

  FMI_WriteHalfWord(0x80006,0x2222);
  FMI_Timeout_Status = FMI_WaitForLastOperation(FMI_BANK_1);

  FMI_WriteHalfWord(0x80008,0x3333);
  FMI_Timeout_Status = FMI_WaitForLastOperation(FMI_BANK_1);

  FMI_WriteHalfWord(0x8000A,0x3333);
  FMI_Timeout_Status = FMI_WaitForLastOperation(FMI_BANK_1);

  while(1);
}

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
