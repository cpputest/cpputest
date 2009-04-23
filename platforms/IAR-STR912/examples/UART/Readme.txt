/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : Description of the UART Example1.
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
This example provides a description of how  to use the UART with hardware flow
control and communicate with the Hyperterminal.
First, the UART0 sends the TxBuffer to the hyperterminal and still waiting for
a string from the hyperterminal that you must enter which must end by '\r'
character (keypad ENTER button).
The string that you have entered is stored in the RxBuffer array. The receive
buffer have a RxBufferSize bytes as maximum.

The UART0 is configured as follow:
  - Word Length = 7 Bits
  - Two Stop Bit
  - No parity
  - BaudRate = 115200 baud
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  - Receive and transmit FIFOs are enabled
  - Transmit and Receive FIFOs levels have 8 bytes depth 


Directory contents
==================
91x_conf.h  Library Configuration file
91x_it.c    Interrupt handlers
main.c      Main program


Hardware environment
====================

 - Connect a null-modem male/female RS232 cable between the DB9 connector 
   and PC serial port.  

 - Hyperterminal configuration:
    - Word Length = 7 Bits
    - Two Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - flow control: None 
  
  
How to use it
=============
In order to make the program work, you must do the following :
- Put the Jumpers U0TX/L5 on the position 2-3(STR912-SK IAR Board)
- Run the example


******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****
