/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2006; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*
*                                         BOARD SUPPORT PACKAGE
*
*                                               ST STR912
*                                                on the
*                                         IAR STR912-SK Board
*
* Filename      : bsp.h
*********************************************************************************************************
*/

#ifndef  __BSP_H__
#define  __BSP_H__

#ifdef   BSP_GLOBALS
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif

/*
*********************************************************************************************************
*                                                 CONSTANTS
*********************************************************************************************************
*/

#define  BSP_INT_TYPE_IRQ               0                       /* Interrupt type IRQ.                                  */
#define  BSP_INT_TYPE_FIQ               1                       /* Interrupt type FIQ.                                  */


/*
*********************************************************************************************************
*                                                DATA TYPES
*********************************************************************************************************
*/

typedef  void (*BSP_PFNCT)(void);

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void            BSP_Init                (void);
void            BSP_IntDisAll           (void);

/*
*********************************************************************************************************
*                                              INTERRUPTS
*********************************************************************************************************
*/

void            BSP_IntVectSet          (CPU_INT08U vec_nbr, CPU_INT08U prio, BSP_PFNCT  pfnct);
void            BSP_IntEn               (CPU_INT08U vec_nbr, CPU_INT08U prio, CPU_INT08U type);
void            BSP_IntDisAll           (void);
void            BSP_IntDis              (CPU_INT08U vec_nbr);
/*
*********************************************************************************************************
*                                             LED SERVICES
*********************************************************************************************************
*/

void            BSP_LED_On              (CPU_INT08U led);
void            BSP_LED_Off             (CPU_INT08U led);
void            BSP_LED_Toggle          (CPU_INT08U led);

/*
*********************************************************************************************************
*                                         PUSH BUTTON SERVICES
*********************************************************************************************************
*/

CPU_BOOLEAN     BSP_PB_GetStatus        (CPU_INT08U pb);

/*
*********************************************************************************************************
*                                             ADC SERVICES
*********************************************************************************************************
*/

CPU_INT16U      BSP_ADC_GetStatus       (CPU_INT08U  adc);

/*
*********************************************************************************************************
*                                            SPEAKER SERVICES
*********************************************************************************************************
*/

void            BSP_Speaker_On          (void);
void            BSP_Speaker_Off         (void);

/*
*********************************************************************************************************
*                                          LCD LIGHT SERVICES
*********************************************************************************************************
*/

#ifdef DISP_MODULE_PRESENT
void            BSP_LCD_LightOn         (void);
void            BSP_LCD_LightOff        (void);
void            BSP_LCD_LightToggle     (void);
#endif


/*
*********************************************************************************************************
*                                             RS-232 SERVICES
*********************************************************************************************************
*/

void            BSP_Ser_Init            (CPU_INT32U  baud_rate);
void            BSP_Ser_WrByte          (CPU_CHAR    tx_byte);
void            BSP_Ser_WrStr           (CPU_CHAR   *tx_str);
CPU_INT08U      BSP_Ser_RdByte          (void);
void            BSP_Ser_RdStr           (CPU_CHAR   *rx_str, CPU_INT32U len);
void            BSP_Ser_Printf          (CPU_CHAR   *format, ...);

/*
*********************************************************************************************************
*                                             SPI SERVICES
*********************************************************************************************************
*/

void            BSP_SPI_En              (void);
CPU_INT16U      BSP_SPI_Init            (CPU_INT16U spi_freq);


/*
*********************************************************************************************************
*                                          BACKWARDS COMPATIBILITY
*********************************************************************************************************
*/

#define  LED_Init           BSP_LED_Init
#define  LED_On             BSP_LED_On
#define  LED_Off            BSP_LED_Off
#define  LED_Toggle         BSP_LED_Toggle
#define  PB_GetStatus       BSP_PB_Getstatus
#define  Speaker_Init       BSP_Speaker_Init
#define  Speaker_On         BSP_Speaker_On
#define  Speaker_Off        BSP_Speaker_Off
#define  ADC_GetStatus      BSP_ADC_GetStatus
#define  LCD_LightOn        BSP_LCD_LightOn
#define  LCD_LightOff       BSP_LCD_LightOff
#define  LCD_LightToggle    BSP_LCD_LightToggle
#define  Ser_Init           BSP_Ser_Init
#define  Ser_WrByte         BSP_Ser_WrByte
#define  Ser_WrStr          BSP_Ser_WrStr
#define  Ser_RdByte         BSP_Ser_RdByte
#define  Ser_RdStr          BSP_Ser_RdStr
#define  Ser_Printf         BSP_Ser_Printf
#define  SPI_En             BSP_SPI_En
#define  SPI_Init           BSP_SPI_Init

/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif
