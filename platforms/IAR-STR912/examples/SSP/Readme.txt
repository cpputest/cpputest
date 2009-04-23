/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : Description of the SSP Example.
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
This example provides a description of how to set a communication between the two
SSPs and performs a transfer from Master to Slave and from Slave to Master in 
Motorola frame format.
SSP0 is configured as Master in Motorola frame format and with an 8bits data size 
to communicate with the Slave SSP1 configured in motorola frame format and with the
same data size as SSP0. 
In the first part, the Master send the SSP0_Buffer_Tx to be received by the Slave.
Then data read by the SSP1 peripheral and stored in the SSP1_Buffer_Rx is compared
with the expected values of transmitted buffer. The result of this comparison is 
stored in the "TransferStatus1" variable.
After the first transfer, the SSP0 receive FIFO have to be cleared by reading its 
eight data.
In the second part, The Slave send the SSP1_Buffer_Tx to be received by the Master
Then data read by the SSP0 peripheral and stored in the SSP0_Buffer_Rx is compared 
with the expected values of transmitted buffer. The result of this comparison is 
stored in the "TransferStatus2" variable.   


Directory contents
==================
91x_conf.h  Library Configuration file
91x_it.c    Interrupt handlers
main.c      Main program

Hardware environment
====================
Connect both SSP0 and SSP1 pins as following:
 - Connect SSP0_SCLK(P5.4) to SSP1_SCLK(P3.4).
 - Connect SSP0_MISO(P5.6) to SSP1_MISO(P3.5).
 - Connect SSP0_MOSI(P5.5) to SSP1_MOSI(P3.6).
 - Connect SSP0_nSS (P5.7) to VCC via pull-up.
 - Connect SSP1_nSS (P3.7) to GND via pull-down.

How to use it
=============
In order to make the program work, you must do the following :

- Run the example


******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****
