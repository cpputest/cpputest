/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : Description of the I2C Example.
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
I2Cs and performs a transfer from Master to Slave and from Slave to Master in 
interrupt mode.
The I2C1 is configured in Master transmitter during transmission phase and in Master 
receiver during receive phase. However, I2C0 is configured as Slave receiver during 
transmission phase and as slave transmitter during receive phase.
The Own I2C0 address is defined to 0xA0. The speed is set to the maximum frequency
of 400kHz for both I2Cs.
In the first part, the Master send the I2C1_Buffer_Tx to be received by the Slave.
Then data read by the I2C0 peripheral and stored in the I2C0_Buffer_Rx is compared
with the expected values of transmitted buffer. The result of this comparison is 
stored in the "TransferStatus1" variable.
In the second part, The Slave send the I2C0_Buffer_Tx to be received by the Master
Then data read by the I2C1 peripheral and stored in the I2C1_Buffer_Rx is compared 
with the expected values of transmitted buffer. The result of this comparison is 
stored in the "TransferStatus2" variable.   

Directory contents
==================
91x_conf.h  Library Configuration file
91x_it.c    Interrupt handlers
main.c      Main program


Hardware environment
====================
Connect both I2C0 and I2C1 pins as following:
 - Connect I2C0_CLKOUT(P2.0) to I2C1_CLKOUT(P2.2).
 - Connect I2C0_DOUT(P2.1) to I2C1_DOUT(P2.3).
 - Connect a pull-up resistor to both CLKOUT and DOUT lines

How to use it
=============
In order to make the program work, you must do the following :

- Run the example


******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****
