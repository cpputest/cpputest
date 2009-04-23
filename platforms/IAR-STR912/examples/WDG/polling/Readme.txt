/******************** (C) COPYRIGHT 2006 STMicroelectronics **********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : Description of the WDG polling
**********************************************************************************
* History:
* 05/22/2007 : Version 1.2
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
This example configures the watchdog pripheral on watchdog mode in order to generate 
a system Reset each 1s.
After a Reset the system changes the state of GPIO3 port: P3.2. 
(changes the state of a led "LED3" mounted on STR912-SK board).

The wdg use the RTC 32Khz clock as clock source.

Directory contents
==================
91x_conf.h Library Configuration file
main.c      Main program
91x_it.c    Interrupt handlers

How to use it
=============
In order to make the program work, you must do the following :
- Load program into the Flash.
- Run the example


******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****
