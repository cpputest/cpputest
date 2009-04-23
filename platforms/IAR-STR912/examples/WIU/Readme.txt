/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : Description of the WIU Example.
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
This example provides a description of how to configure an wakeup interrupt 
line. In this example, the wakeup line 29 (External group 3)is configured to 
generate an interrupt on each falling edge. In the interrupt routine a led 
connected to P3.2 will be toggled.
This led will be toggled due to the interrupt generated on wakeup Line 29 
at each falling edge.

Directory contents
==================

91x_conf.h  Library Configuration file
main.c      Main program
91x_it.c    Interrupt handlers

Hardware environment
====================
- Connect a push-button to P7.5 pin (B1 on the STR912-SK board) 
to generate edges on WIU line 29. 
- Connect a LED to P3.2 pin (LED 3 on STR912-SK-IAR board)

How to use it
=============
In order to make the program work, you must do the following :

- Run the example


******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE******
