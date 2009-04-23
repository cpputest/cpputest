/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : Description of the MC Example.
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

Example description
===================
This example show how to configure the MC peripheral to generate Three complementary
PWM signals, to insert a defined dead time value and to use the Tacho interface in 
hardware Mode.
The PCLK frequency is set to 25 MHz, the MC is running at 11.73 KHz, the 
PulseU, PulseV and PulseW duty cycles are fixed respectively to 50%, 25% and 12.5%.
The dead time is equal to 148 microsecond. 
The PWM waveform can be display in a scope. 


Directory contents
==================
91x_conf.h  Library Configuration file
main.c      Main program
91x_it.c    Interrupt handlers

Hardware environment
====================
Connect a scope to:
- Pin29 (P6.00/MC_UH).  
- Pin31 (P6.01/MC_UL).
- Pin19 (P6.02/MC_VH).
- Pin20 (P6.03/MC_VL).
- Pin83 (P6.04/MC_WH).
- Pin84 (P6.05/MC_WL).


How to use it
=============
In order to make the program work, you must do the following :

- Put the Jumpers from L9/MC_UH, L10/MC_UL, L11/MC_VH, L12/MC_VL, L13/MC_WH,
  L14/MC_WL jumpers on the position 2-3(STR912-SK IAR Board)
- Run the example


******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****
