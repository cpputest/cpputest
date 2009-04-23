/******************** (C) COPYRIGHT 2006 STMicroelectronics **********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : Description of the CAN example
**********************************************************************************
* History:
* 05/24/2006 : Version 1.1
* 05/18/2006 : Version 1.0
*********************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**********************************************************************************

Example description
===================
This example provides a short description of how to use the CAN peripheral and
gives a configuration sequence. In this example, standard ID and extended ID frames 
are sent and received in loopback + silent modes. The standard ID frame is sent at a
1 Megabyte standard baud rate and received by polling. The extended ID frame is sent
at a 100 kilobyte standard baud rate and received with the CAN IRQ handler.


Directory contents
==================
91x_conf.h  Library Configuration file
91x_it.c    Interrupt handlers
main.c      Main program
Readme.txt   This file
How to use it
=============
In order to make the program work, you must do the following :

- Put the Jumper CANTX on the position 1-3(only for STR912-SK IAR Board)
- Run the example


******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****
