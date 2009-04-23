/******************** (C) COPYRIGHT 2006 STMicroelectronics **********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : Description of the DMA example
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

This example configures the DMA peripheral to perform a memory to memory Data transfer.
The example consists in transferring a block of Data which occupies contiguous areas in memory
"Buffer0" and scatters the Data in three  memory areas"buffer1, Buffer2 & Buffer3".

The example uses a linked lists Item structure in the memory.
After running the example, you can verify with your tool chain’s that the content of 
Buffer0 is transferred to three buffers (Buffer1, Buffer2 & Buffer3).


Directory contents
==================
91x_conf.h Library Configuration file
main.c      Main program
91x_it.c    Interrupt handlers
Readme.tx   This file.

How to use it
=============

- Run the example


******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****
