/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2008; Micrium, Inc.; Weston, FL
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
* Filename      : bsp.c
* Version       : V1.10
* Programmer(s) : FT
*********************************************************************************************************
*/

#define  BSP_GLOBALS

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <includes.h>

/*
*********************************************************************************************************
*                                           #DEFINE CONSTANTS
*********************************************************************************************************
*/

#define  BSP_WSR_AHB_MASK                    0xFFFFFFFB         /* AHB Wait state enable mask                              */
#define  BSP_WSR_AHB_VAL                              0         /* AHB Wait state enable value                             */
#define  BSP_WSR_DTCM_MASK                   0xFFFFFFFD         /* DTCM Wait state enable mask                             */
#define  BSP_WSR_DTCM_VAL                             0         /* DTCM Wait state enable value                            */
                                                                /* ------------ SRAM CONFIGURATION ------------------------ */
#define  BSP_SRAM_SIZE_MASK                  0xFFFFFFE7         /* SRAM Size Mask                                           */
#define  BSP_SRAM_SIZE                             0x02         /* SRAM Size = 10b =  96 KB                                 */
#define  BSP_SRAM_LK_EN_MASK                 0xFFFFFFDF         /* SRAM Arbiter lock transfer enable mask                   */
#define  BSP_SRAM_LK_EN_VAL                    DEF_TRUE         /* SRAM Arbiter lock transfer enable                        */
#define  BSP_SRAM_ADDR                       0x40000000         /* SRAM start address AHB-BUFFERED                          */
                                                                /* -------------- FLASH CONFIGURATION --------------------- */
#define  BSP_BOOT_BANK_SIZE                        0x04         /* Boot Bank Size     (2^4) * 32 = 512KB                    */
#define  BSP_BOOT_BANK_ADDR                  0x00000000         /* Boot Bank Start Address       = 0x0000000                */
#define  BSP_NON_BOOT_BANK_SIZE                    0x02         /* Non Boot Bank Size (2^2) * 8  = 32KB                     */
#define  BSP_NON_BOOT_BANK_ADDR              0x00800000         /* Non Boot Bank Start Address   = 0x0080000                */

#define  BSP_FLASH_ADDR                      0x00000000         /* Flash start address Bank 0                               */
#define  BSP_FLASH_MAX_NBR_RETRIES                  100         /* Maximun Number of retries for flash programing           */

#define  BSP_EXCEPT_VECT_TBL_SIZE                    64         /* Size in bytes of the Exception Vector + Constants table  */

#define  BSP_PLL_M                                   25         /* PLL dividers and multipliers                             */
#define  BSP_PLL_N                                  192
#define  BSP_PLL_P                                    2

#define  BSP_TMR0_PRESCALER                      0x0030
#define  BSP_VIC_CHANNELS                            16
#define  BSP_NBR_INT                                 32
#define  BSP_TMR0_INT_PRIO                           15         /* Tick interrupt priority                                  */


/*
*********************************************************************************************************
*                                                MACROS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           LOCAL VARIABLES
*********************************************************************************************************
*/

        CPU_INT16U          BSP_Tmr0_Cmp_Value;

static  GPIO_InitTypeDef    BSP_GPIOInit;
static  UART_InitTypeDef    BSP_UARTInit;
static  ADC_InitTypeDef     BSP_ADCInit;
static  TIM_InitTypeDef     BSP_TIMInit;
static  SSP_InitTypeDef     BSP_SSPInit;

/*
*********************************************************************************************************
*                                       LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void    BSP_LED_Init       (void);
static  void    BSP_IO_Init        (void);
static  void    BSP_ADC_Init       (void);
static  void    BSP_Speaker_Init   (void);

static  void    BSP_Tmr_TickInit   (void);
static  void    Tmr_TickISR_Handler(void);
static  void    BSP_VIC_Init       (void);

#ifdef DISP_MODULE_PRESENT
static  void    DispE_High         (void);
static  void    DispE_Low          (void);
static  void    DispRW_Low         (void);
#endif

/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                            GLOBAL FUNCTIONS
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initializes the board support package.
*
* Arguments   : none
*
* Returns     : none
*
* Note(s)     : This function should be called by your application code before you make use of any of the
*               functions found in this module.
*********************************************************************************************************
*/

void  BSP_Init (void)
{
    CPU_INT32U  scr0_val;


    SCU_MCLKSourceConfig(SCU_MCLK_OSC);
                                                                /* Set the PLL's multipliers and dividers                   */
    SCU_PLLFactorsConfig(BSP_PLL_N, BSP_PLL_M, BSP_PLL_P);
    SCU_PLLCmd(ENABLE);                                         /* Enable the PLL                                           */
    SCU_RCLKDivisorConfig(SCU_RCLK_Div1);                       /* Set RCLK, the CPU clock's main divider                   */
    SCU_PCLKDivisorConfig(SCU_PCLK_Div1);                       /* Set APBDIV, the PCLK divider                             */
    SCU_FMICLKDivisorConfig(SCU_FMICLK_Div2);

    SCU_MCLKSourceConfig(SCU_MCLK_PLL);                         /* Select the PLL output as CPU clock                       */

    scr0_val  = SCU->SCR0;                                      /* Set the size of the SRAM                                 */
    scr0_val &= (BSP_SRAM_SIZE_MASK & BSP_WSR_AHB_MASK & BSP_WSR_DTCM_MASK & BSP_SRAM_LK_EN_MASK);
    scr0_val |= ((BSP_SRAM_LK_EN_VAL << 5) | (BSP_SRAM_SIZE << 3) | (BSP_WSR_AHB_VAL << 2) | (BSP_WSR_DTCM_VAL << 1));
    SCU->SCR0 = scr0_val;

    SCU_PFQBCCmd(ENABLE);                                       /* Enable the PFQBC                                         */

    BSP_IO_Init();                                              /* Initialize the board's I/Os                              */

    BSP_VIC_Init();                                             /* Initialize the Vectored Interrupt Controller             */

    BSP_LED_Init();                                             /* Initialize the I/Os for the LED controls                 */
    BSP_ADC_Init();                                             /* Initialize the ADC control                               */
    BSP_Speaker_Init();                                         /* Initialize the speaker                                   */
    BSP_Tmr_TickInit();                                         /* Initialize the uC/OS-II tick interrupt                   */
}

/*
*********************************************************************************************************
*                                          OS_CPU_ExceptHndlr ()
*
* Description : Exception handler. This function should be used to handle any exceptions.  It is called by
*               OS_CPU_ARM_EXCEPT_HANDLER(), which is declared in os_cpu_a.s
*
* Arguments   : ID, an identifier used to indicate what type of ARM exception has been triggered
*               Possible ID values are shown below.
*                  OS_CPU_ARM_EXCEPT_RESET             0x00
*                  OS_CPU_ARM_EXCEPT_UNDEF_INSTR       0x01
*                  OS_CPU_ARM_EXCEPT_SWI               0x02
*                  OS_CPU_ARM_EXCEPT_PREFETCH_ABORT    0x03
*                  OS_CPU_ARM_EXCEPT_DATA_ABORT        0x04
*                  OS_CPU_ARM_EXCEPT_ADDR_ABORT        0x05
*                  OS_CPU_ARM_EXCEPT_IRQ               0x06
*                  OS_CPU_ARM_EXCEPT_FIQ               0x07
*********************************************************************************************************
*/

void  OS_CPU_ExceptHndlr (CPU_INT32U except_id)
{
    BSP_PFNCT  pfnct;
#if (BSP_CFG_TRACE_LEVEL == TRACE_LEVEL_DBG)
    CPU_INT32U     *sp;
#endif

    if (except_id == OS_CPU_ARM_EXCEPT_IRQ) {
        while (((VIC0->ISR) != 0) || ((VIC1->ISR) != 0)) {
                                                                /* Read the interrupt vector                                */
            pfnct = (BSP_PFNCT)(VIC_GetCurrentISRAdd(VIC0));
            if (pfnct != (BSP_PFNCT)0) {                        /* Make sure we don't have a NULL pointer                   */
                (*pfnct)();                                     /* Execute the ISR for the interrupting device              */
            } else {
                VIC0->VAR  = 0x00000000;                        /* Signal the end of the interrupt                          */
                VIC1->VAR  = 0x00000000;
            }
        }
    } else {
#if (BSP_CFG_TRACE_LEVEL == TRACE_LEVEL_DBG)
        if (OSIntNesting == 1) {
            sp = (CPU_INT32U *)OSTCBCur->OSTCBStkPtr;
        } else {
            sp = (CPU_INT32U *)OS_CPU_ExceptStkPtr;
        }

        BSP_TRACE_DBG(("\nCPU_ARM_EXCEPTION #%d trapped.\n", except_id));
        BSP_TRACE_DBG(("R0  : 0x%08x\n", *(sp + 0x01)));
        BSP_TRACE_DBG(("R1  : 0x%08x\n", *(sp + 0x02)));
        BSP_TRACE_DBG(("R2  : 0x%08x\n", *(sp + 0x03)));
        BSP_TRACE_DBG(("R3  : 0x%08x\n", *(sp + 0x04)));
        BSP_TRACE_DBG(("R4  : 0x%08x\n", *(sp + 0x05)));
        BSP_TRACE_DBG(("R5  : 0x%08x\n", *(sp + 0x06)));
        BSP_TRACE_DBG(("R6  : 0x%08x\n", *(sp + 0x07)));
        BSP_TRACE_DBG(("R7  : 0x%08x\n", *(sp + 0x08)));
        BSP_TRACE_DBG(("R8  : 0x%08x\n", *(sp + 0x09)));
        BSP_TRACE_DBG(("R9  : 0x%08x\n", *(sp + 0x0A)));
        BSP_TRACE_DBG(("R10 : 0x%08x\n", *(sp + 0x0B)));
        BSP_TRACE_DBG(("R11 : 0x%08x\n", *(sp + 0x0C)));
        BSP_TRACE_DBG(("R12 : 0x%08x\n", *(sp + 0x0D)));
        BSP_TRACE_DBG(("SP  : 0x%08x\n",   sp));
        BSP_TRACE_DBG(("LR  : 0x%08x\n", *(sp + 0x0E)));
        BSP_TRACE_DBG(("PC  : 0x%08x\n", *(sp + 0x0F)));
        BSP_TRACE_DBG(("CPSR: 0x%08x\n", *(sp + 0x00)));
#endif
        while (DEF_TRUE) {
            ;
        }
    }
}

/*
*********************************************************************************************************
*                                      BSP_IntDisAll()
*
* Description : This function disables all interrupts from the interrupt controller.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

void  BSP_IntDisAll (void)
{
    CPU_INT08U  i;


    for (i = 0; i < BSP_NBR_INT; i++) {
        VIC_ITCmd(i, DISABLE);
    }
}


/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                   PERIPHERAL FUNCTIONS (PB, LED, and ADC)
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           BSP_LED_Init()
*
* Description : This function initializes the board's LEDs.
*
* Arguments   : none
*
* Returns    : None
*********************************************************************************************************
*/

static  void  BSP_LED_Init (void)
{
    BSP_LED_Off(0);
}

/*
*********************************************************************************************************
*                                                 BSP_LED_On()
*
* Description : This function is used to control any or all the LEDs on the board.
*
* Arguments   : led    is the number of the LED to control
*                      0    indicates that you want to turn on all of the board's LEDs
*                      1    turn on LD2
*                      2    turn on LD3
*                      .
*                      4    turn on LD5
*
* Returns    : None
*********************************************************************************************************
*/

void  BSP_LED_On (CPU_INT08U  led)
{
    switch (led) {
        case 0:
             GPIO_WriteBit(GPIO3, 0xFF, Bit_RESET);
             GPIO_WriteBit(GPIO6, 0xFF, Bit_RESET);
             break;

        case 1:
             GPIO_WriteBit(GPIO3, 0x01, Bit_RESET);
             break;

        case 2:
             GPIO_WriteBit(GPIO3, 0x02, Bit_RESET);
             break;

        case 3:
             GPIO_WriteBit(GPIO3, 0x04, Bit_RESET);
             break;

        case 4:
             GPIO_WriteBit(GPIO3, 0x08, Bit_RESET);
             break;

        case 5:
             GPIO_WriteBit(GPIO3, 0x10, Bit_RESET);
             break;

        case 6:
             GPIO_WriteBit(GPIO3, 0x20, Bit_RESET);
             break;

        case 7:
             GPIO_WriteBit(GPIO3, 0x40, Bit_RESET);
             break;

        case 8:
             GPIO_WriteBit(GPIO3, 0x80, Bit_RESET);
             break;

        case 9:
             GPIO_WriteBit(GPIO6, 0x01, Bit_RESET);
             break;

        case 10:
             GPIO_WriteBit(GPIO6, 0x02, Bit_RESET);
             break;

        case 11:
             GPIO_WriteBit(GPIO6, 0x04, Bit_RESET);
             break;

        case 12:
             GPIO_WriteBit(GPIO6, 0x08, Bit_RESET);
             break;

        case 13:
             GPIO_WriteBit(GPIO6, 0x10, Bit_RESET);
             break;

        case 14:
             GPIO_WriteBit(GPIO6, 0x20, Bit_RESET);
             break;

        case 15:
             GPIO_WriteBit(GPIO6, 0x40, Bit_RESET);
             break;

        case 16:
             GPIO_WriteBit(GPIO6, 0x80, Bit_RESET);
             break;

        default:
             break;
    }
}

/*
*********************************************************************************************************
*                                            BSP_LED_Off()
*
* Description : This function is used to control any or all the LEDs on the board.
*
* Arguments   : led    is the number of the LED to turn OFF
*                      0    indicates that you want to turn off all of the board's LEDs
*                      1    turn off LD2
*                      2    turn off LD3
*                      .
*                      4    turn off LD5
*
* Returns    : None
*********************************************************************************************************
*/

void  BSP_LED_Off (CPU_INT08U  led)
{
    switch (led) {
        case 0:
             GPIO_WriteBit(GPIO3, 0xFF, Bit_SET);
             GPIO_WriteBit(GPIO6, 0xFF, Bit_SET);
             break;

        case 1:
             GPIO_WriteBit(GPIO3, 0x01, Bit_SET);
             break;

        case 2:
             GPIO_WriteBit(GPIO3, 0x02, Bit_SET);
             break;

        case 3:
             GPIO_WriteBit(GPIO3, 0x04, Bit_SET);
             break;

        case 4:
             GPIO_WriteBit(GPIO3, 0x08, Bit_SET);
             break;

        case 5:
             GPIO_WriteBit(GPIO3, 0x10, Bit_SET);
             break;

        case 6:
             GPIO_WriteBit(GPIO3, 0x20, Bit_SET);
             break;

        case 7:
             GPIO_WriteBit(GPIO3, 0x40, Bit_SET);
             break;

        case 8:
             GPIO_WriteBit(GPIO3, 0x80, Bit_SET);
             break;

        case 9:
             GPIO_WriteBit(GPIO6, 0x01, Bit_SET);
             break;

        case 10:
             GPIO_WriteBit(GPIO6, 0x02, Bit_SET);
             break;

        case 11:
             GPIO_WriteBit(GPIO6, 0x04, Bit_SET);
             break;

        case 12:
             GPIO_WriteBit(GPIO6, 0x08, Bit_SET);
             break;

        case 13:
             GPIO_WriteBit(GPIO6, 0x10, Bit_SET);
             break;

        case 14:
             GPIO_WriteBit(GPIO6, 0x20, Bit_SET);
             break;

        case 15:
             GPIO_WriteBit(GPIO6, 0x40, Bit_SET);
             break;

        case 16:
             GPIO_WriteBit(GPIO6, 0x80, Bit_SET);
             break;

        default:
             break;
    }
}

/*
*********************************************************************************************************
*                                             BSP_LED_Toggle()
*
* Description : This function is used to alternate the state of an LED
*
* Arguments   : led    is the number of the LED to control
*                      0    indicates that you want ALL the LEDs to toggle
*                      1    toggle LD2
*                      2    toggle LD3
*                      .
*                      4    toggle LD5
*
* Returns    : None
*********************************************************************************************************
*/

void  BSP_LED_Toggle (CPU_INT08U  led)
{
    CPU_INT08U  led_status_upper;
    CPU_INT08U  led_status_lower;


    led_status_lower = GPIO_Read(GPIO3);             /* Read the current state of the LEDs             */
    led_status_upper = GPIO_Read(GPIO6);

    switch (led) {
        case 0:
             led_status_lower ^= 0xFF;
             led_status_upper ^= 0xFF;
             GPIO_Write(GPIO3, led_status_lower);
             GPIO_Write(GPIO6, led_status_upper);
             break;

        case 1:
             led_status_lower ^= 0x01;
             GPIO_Write(GPIO3, led_status_lower);
             break;

        case 2:
             led_status_lower ^= 0x02;
             GPIO_Write(GPIO3, led_status_lower);
             break;

        case 3:
             led_status_lower ^= 0x04;
             GPIO_Write(GPIO3, led_status_lower);
             break;

        case 4:
             led_status_lower ^= 0x08;
             GPIO_Write(GPIO3, led_status_lower);
             break;

        case 5:
             led_status_lower ^= 0x10;
             GPIO_Write(GPIO3, led_status_lower);
             break;

        case 6:
             led_status_lower ^= 0x20;
             GPIO_Write(GPIO3, led_status_lower);
             break;

        case 7:
             led_status_lower ^= 0x40;
             GPIO_Write(GPIO3, led_status_lower);
             break;

        case 8:
             led_status_lower ^= 0x80;
             GPIO_Write(GPIO3, led_status_lower);
             break;

        case 9:
             led_status_upper ^= 0x01;
             GPIO_Write(GPIO6, led_status_upper);
             break;

        case 10:
             led_status_upper ^= 0x02;
             GPIO_Write(GPIO6, led_status_upper);
             break;

        case 11:
             led_status_upper ^= 0x04;
             GPIO_Write(GPIO6, led_status_upper);
             break;

        case 12:
             led_status_upper ^= 0x08;
             GPIO_Write(GPIO6, led_status_upper);
             break;

        case 13:
             led_status_upper ^= 0x10;
             GPIO_Write(GPIO6, led_status_upper);
             break;

        case 14:
             led_status_upper ^= 0x20;
             GPIO_Write(GPIO6, led_status_upper);
             break;

        case 15:
             led_status_upper ^= 0x40;
             GPIO_Write(GPIO6, led_status_upper);
             break;

        case 16:
             led_status_upper ^= 0x80;
             GPIO_Write(GPIO6, led_status_upper);
             break;

        default:
             break;
    }
}

/*
*********************************************************************************************************
*                                         BSP_PB_GetStatus()
*
* Description : This function is used to get the status of any push button on the board.
*
* Arguments   : push_button    is the number of the push button to probe
*                              1    probe the push button B1
*                              2    probe the push button B2
*
* Returns     : DEF_FALSE   if the push button is pressed
*               DEF_TRUE    if the push button is not pressed
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_PB_GetStatus (CPU_INT08U push_button_id)
{
    CPU_BOOLEAN  status;


    status = DEF_FALSE;

    switch (push_button_id) {
        case 1:
             if ((GPIO_ReadBit(GPIO7, GPIO_Pin_5)) == Bit_RESET) {
                 status = DEF_TRUE;
             }
             break;

        case 2:
             if ((GPIO_ReadBit(GPIO7, GPIO_Pin_6)) == Bit_RESET) {
                 status = DEF_TRUE;
             }
             break;

        default:
             break;
    }

    return (status);
}

/*
*********************************************************************************************************
*                                     BSP_Speaker_Init
*
* Description : This function initializes TIM3, which drives the PWM for the signal itself.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

static  void  BSP_Speaker_Init (void)
{
    SCU_APBPeriphClockConfig(__TIM23, ENABLE);                  /* Enable the timer's clock signal                          */
    SCU_APBPeriphReset(__TIM23, DISABLE);                       /* Remove the timer from reset state                        */

                                                                /* TIM3 will be used for PWM output driving the speaker ... */
    TIM_DeInit(TIM3);                                           /*  ... Return the timer's registers to default values      */
    TIM_StructInit(&BSP_TIMInit);
    BSP_TIMInit.TIM_Mode            = TIM_PWM;
    BSP_TIMInit.TIM_Clock_Source    = TIM_CLK_APB;
    BSP_TIMInit.TIM_Prescaler       = 0;
    BSP_TIMInit.TIM_Pulse_Level_1   = TIM_HIGH;
    BSP_TIMInit.TIM_Period_Level    = TIM_LOW;
    BSP_TIMInit.TIM_Pulse_Length_1  = 0x0000;
    BSP_TIMInit.TIM_Full_Period     = (0x03FF >> 2);
    TIM_Init(TIM3, &BSP_TIMInit);

    TIM_CounterCmd(TIM3, TIM_START);
}

/*
*********************************************************************************************************
*                                       BSP_Speaker_On()
*
* Description : This function turns on the speaker.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

void  BSP_Speaker_On (void)
{
    TIM_SetPulse(TIM3, TIM_PWM_OC1_Channel, 0x30);
}

/*
*********************************************************************************************************
*                                       BSP_Speaker_Off()
*
* Description : This function turns on the speaker.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

void  BSP_Speaker_Off (void)
{
    TIM_SetPulse(TIM3, TIM_PWM_OC1_Channel, 0x80);
}

/*
*********************************************************************************************************
*                                           BSP_ADC_Init()
*
* Description : This function initializes the board's ADC.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

static  void  BSP_ADC_Init (void)
{
    SCU_APBPeriphClockConfig(__ADC,   ENABLE);                  /* Enable the ADC's clock signal                            */
    ADC_DeInit();


    ADC_StructInit(&BSP_ADCInit);

    BSP_ADCInit.ADC_Channel_5_Mode  = ADC_NoThreshold_Conversion;
    BSP_ADCInit.ADC_Channel_7_Mode  = ADC_NoThreshold_Conversion;
    BSP_ADCInit.ADC_Scan_Mode       = ENABLE;
    BSP_ADCInit.ADC_Conversion_Mode = ADC_Continuous_Mode;

    ADC_Cmd(ENABLE);
    ADC_PrescalerConfig(0x04);
    ADC_Init(&BSP_ADCInit);

    ADC_ConversionCmd(ADC_Conversion_Start);
}

/*
*********************************************************************************************************
*                                           BSP_ADC_GetStatus()
*
* Description : This function get the status of the board's ADC.
*
* Arguments   : adc     is the ID of the ADC.
*                       1  is the ADC labelled "AN_TR"
*                       2  is the microphone input
*
* Returns     : none
*********************************************************************************************************
*/

CPU_INT16U  BSP_ADC_GetStatus (CPU_INT08U  adc)
{
    CPU_INT16U  result;


    switch (adc) {
        case 1:
             result = ADC_GetConversionValue(ADC_Channel_5);
             break;

        case 2:
             result = ADC_GetConversionValue(ADC_Channel_7);
             break;

        default:
             result = 0;
             break;
    }

    return (result);
}

/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                               LCD FUNCTIONS
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           BSP_LCD_LightOn()
*
* Description : This function is used to turn on the LCD's light.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/
#ifdef DISP_MODULE_PRESENT
void  BSP_LCD_LightOn (void)
{
    GPIO_WriteBit(GPIO9, 0x20, Bit_SET);
}
#endif

/*
*********************************************************************************************************
*                                           BSP_LCD_LightOff()
*
* Description : This function is used to turn off the LCD's light.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/
#ifdef DISP_MODULE_PRESENT
void  BSP_LCD_LightOff (void)
{
    GPIO_WriteBit(GPIO9, 0x20, Bit_RESET);
}
#endif

/*
*********************************************************************************************************
*                                        BSP_LCD_LightToggle()
*
* Description : This function is used to toggle the state of the LCD's light.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/
#ifdef DISP_MODULE_PRESENT
void  BSP_LCD_LightToggle (void)
{
    CPU_INT08U  lcd_light_state;

    lcd_light_state  = GPIO_Read(GPIO9);
    lcd_light_state ^= 0x20;
    GPIO_Write(GPIO9, lcd_light_state);
}
#endif

/*
*********************************************************************************************************
*                                             DispDataWr()
*
* Description : This function sends a single BYTE to the display device.
*
* Arguments   : 'data'  is the BYTE to send to the display device
*
* Returns     : none
*********************************************************************************************************
*/

#ifdef DISP_MODULE_PRESENT
void  DispDataWr (INT8U data)
{
    DispRW_Low();                                    /* Set R/W LOW to write to the LCD module         */

    DispE_High();

    GPIO_Write(GPIO8, data);

    DispDly_uS(100);

    DispE_Low();
}
#endif

/*
*********************************************************************************************************
*                                               DispDly_uS()
*
* Description : This function is called to delay for the specified number of microseconds.
*
* Arguments   : us      Number of microseconds
*
* Returns     : none
*********************************************************************************************************
*/

#ifdef DISP_MODULE_PRESENT
void  DispDly_uS (CPU_INT32U us)
{
    CPU_INT32U  tim1_freq;
    CPU_INT16U  dly_period;
    CPU_INT16U  tmr_start_val;
    CPU_INT16U  tmr_cur_val;
    CPU_INT16U  ms_dly;


    if (us < 1000) {
        tmr_start_val = TIM_GetCounterValue(TIM1);

        tim1_freq     = SCU_GetPCLKFreqValue();
        tim1_freq    /= 2;                           /* The default TIM1 divider of 2 is being used    */
        dly_period    = (CPU_INT16U)((tim1_freq / 1000) * us);

        tmr_cur_val   = TIM_GetCounterValue(TIM1);

        while (((CPU_INT16U)(tmr_cur_val - tmr_start_val)) < dly_period) {
            tmr_cur_val = TIM_GetCounterValue(TIM1);
        }
    } else {
        ms_dly = us / 1000;
        OSTimeDlyHMSM(0,0,0,ms_dly);
    }
}
#endif

/*
*********************************************************************************************************
*                                          DispInitPort()
*
* Description : This routine normally initializes the I/O ports used by the display driver.  However,
*               because these ports should have already been initialized in BSP_IO_Init(), this version
*               of DispInitPort() is used to initialize TIM1, which is needed by DispDly_uS().
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

#ifdef DISP_MODULE_PRESENT
void  DispInitPort (void)
{
    TIM_DeInit(TIM1);                                /* Ensure that TIM1 registers are at reset values */
    TIM_CounterCmd(TIM1, TIM_START);                 /* Start the timer                                */

}
#endif

/*
*********************************************************************************************************
*                                             DispSel()
*
* Description : This changes the Register Select control line to the LCD controller.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

#ifdef DISP_MODULE_PRESENT
void  DispSel (CPU_INT08U sel)
{
    if (sel == DISP_SEL_CMD_REG) {
        GPIO_WriteBit(GPIO9, 0x80, Bit_RESET);        /* Select the command register (RS low)           */
    } else {
        GPIO_WriteBit(GPIO9, 0x80, Bit_SET);          /* Select the data    register (RS high)          */
    }
}
#endif

/*
*********************************************************************************************************
*                                      DISPLAY CONTROL LINE FUNCTIONS
*********************************************************************************************************
*/

#ifdef DISP_MODULE_PRESENT
static  void  DispE_High (void)
{
    GPIO_WriteBit(GPIO9, 0x10, Bit_SET);
}

static  void  DispE_Low (void)
{
    GPIO_WriteBit(GPIO9, 0x10, Bit_RESET);
}

static  void  DispRW_Low (void)
{
    GPIO_WriteBit(GPIO9, 0x40, Bit_RESET);
}
#endif

/*
*********************************************************************************************************
*********************************************************************************************************
*                             uC/Probe PLUG-IN FOR uC/OS-II FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       OSProbe_TmrInit()
*
* Description : Select & initialize a timer for use with the uC/Probe Plug-In for uC/OS-II.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

#if (APP_CFG_PROBE_OS_PLUGIN_EN == DEF_ENABLED) && (OS_PROBE_HOOKS_EN == 1)
void  OSProbe_TmrInit (void)
{
    TIM_DeInit(TIM1);                                /* Ensure that TIM1 registers are at reset values */
    TIM_CounterCmd(TIM1, TIM_START);                 /* Start the timer                                */
}
#endif


/*
*********************************************************************************************************
*                                        OSProbe_TmrRd()
*
* Description : Read the current counts of a 16-bit free running timer.
*
* Argument(s) : none.
*
* Return(s)   : The 16 bit counts (in a 32 bit variable) of the timer.
*********************************************************************************************************
*/

#if (APP_CFG_PROBE_OS_PLUGIN_EN == DEF_ENABLED) && (OS_PROBE_HOOKS_EN == 1)
CPU_INT32U  OSProbe_TmrRd (void)
{
    CPU_INT32U  cnts;


    cnts = (CPU_INT32U)(TIM_GetCounterValue(TIM1));
    return (cnts);
}
#endif

/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                         uC/OS-II TIMER FUNCTIONS
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         BSP_Tmr_TickInit()
*
* Description : This function is called to initialize uC/OS-II's tick source (typically a timer
*               generating interrupts every 1 to 100 mS).
*
*               We decided to use Timer #0 as the tick interrupt source.
*
* Arguments   : none
*
* Returns    : None
*********************************************************************************************************
*/

static  void  BSP_Tmr_TickInit (void)
{
    CPU_INT32U  pclk_freq;


    SCU_APBPeriphClockConfig(__TIM01, ENABLE);                  /* Enable the timer's clock signal                          */
    SCU_APBPeriphReset(__TIM01, DISABLE);                       /* Remove the timer from reset state                        */

    TIM_DeInit(TIM0);                                           /* Return the timer's registers to default values           */
                                                                /* Set up the timer's interrupt                             */

    BSP_IntVectSet(TIM0_ITLine, BSP_TMR0_INT_PRIO, Tmr_TickISR_Handler);
    BSP_IntEn(TIM0_ITLine, BSP_TMR0_INT_PRIO, BSP_INT_TYPE_IRQ);

    pclk_freq          = SCU_GetPCLKFreqValue();

    pclk_freq         *= 1000;
                                                                /* Calculate the required period                            */
    BSP_Tmr0_Cmp_Value = (pclk_freq / BSP_TMR0_PRESCALER) / OS_TICKS_PER_SEC;

    TIM0->OC1R         = BSP_Tmr0_Cmp_Value;                    /* Set the output compare register                          */
                                                                /* Set the timer's prescaler                                */
    TIM_PrescalerConfig(TIM0, (BSP_TMR0_PRESCALER - 1));
    TIM_ITConfig(TIM0, TIM_IT_OC1, ENABLE);                     /* Enable the interrupt                                     */
    TIM_CounterCmd(TIM0, TIM_START);                            /* Start the timer                                          */
}

/*
*********************************************************************************************************
*                                         TIMER #0 IRQ HANDLER
*
* Description : This function handles the timer interrupt that is used to generate TICKs for uC/OS-II.
*
* Arguments   : none
*********************************************************************************************************
*/

static  void  Tmr_TickISR_Handler (void)
{
    CPU_INT16U  cmp_value;


    TIM_ClearFlag(TIM0, TIM_IT_OC1);                 /* Clear the status register                      */

    OSTimeTick();                                    /* Call uC/OS-II's OSTimeTick() to signal a tick  */

    cmp_value  = TIM0->OC1R;
    cmp_value += BSP_Tmr0_Cmp_Value;                 /* Calculate a new ouput compare value            */

    TIM0->OC1R = cmp_value;                          /* Set the output compare register                */

    VIC0->VAR  = 0x00000000;                         /* Signal the end of the interrupt                */
}

/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                       SERIAL PORT COMMUNICATIONS
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                   BSP_Ser_Init()
*
* Description :   This function prepares UART0 for use with uC/TTCP.
*
* Arguments   :   None.
*
* Returns     :   None.
*********************************************************************************************************
*/

void  BSP_Ser_Init (CPU_INT32U baud_rate)
{

    SCU_APBPeriphClockConfig(__GPIO3, ENABLE);                  /* Enable the appropriate devices' clock signals            */
    SCU_APBPeriphClockConfig(__GPIO5, ENABLE);
    SCU_APBPeriphClockConfig(__UART0, ENABLE);

    SCU_APBPeriphReset(__GPIO3, DISABLE);                       /* Remove the devices from reset                            */
    SCU_APBPeriphReset(__GPIO5, DISABLE);
    SCU_APBPeriphReset(__UART0, DISABLE);

    BSP_GPIOInit.GPIO_Pin               = 0x10;
    BSP_GPIOInit.GPIO_Direction         = GPIO_PinOutput;
    BSP_GPIOInit.GPIO_Type              = GPIO_Type_PushPull;
    BSP_GPIOInit.GPIO_IPConnected       = GPIO_IPConnected_Disable;
    BSP_GPIOInit.GPIO_Alternate         = GPIO_OutputAlt3;
                                                                /* Initialize Port 3 pins used by the UART                  */
    GPIO_Init(GPIO3, &BSP_GPIOInit);

    BSP_GPIOInit.GPIO_Pin               = 0x02;
    BSP_GPIOInit.GPIO_Direction         = GPIO_PinInput;
    BSP_GPIOInit.GPIO_IPConnected       = GPIO_IPConnected_Enable;
    BSP_GPIOInit.GPIO_Alternate         = GPIO_InputAlt1;
                                                                /* Initialize Port 5 pins used by the UART                  */
    GPIO_Init(GPIO5, &BSP_GPIOInit);

    BSP_UARTInit.UART_WordLength           = UART_WordLength_8D;
    BSP_UARTInit.UART_StopBits             = UART_StopBits_1;
    BSP_UARTInit.UART_Parity               = UART_Parity_No;
    BSP_UARTInit.UART_BaudRate             = baud_rate;
    BSP_UARTInit.UART_HardwareFlowControl  = UART_HardwareFlowControl_None;
    BSP_UARTInit.UART_Mode                 = UART_Mode_Tx_Rx;
    BSP_UARTInit.UART_FIFO                 = UART_FIFO_Enable;
    BSP_UARTInit.UART_TxFIFOLevel          = UART_FIFOLevel_1_8;
    BSP_UARTInit.UART_RxFIFOLevel          = UART_FIFOLevel_1_8;

    UART_Init(UART0, &BSP_UARTInit);                            /* Use 8 data bits, 1 stop bit, and no parity               */
    UART_Cmd(UART0, ENABLE);                                    /* Enable UART0                                             */
}

/*
*********************************************************************************************************
*                                                BSP_Ser_WrByte()
*
* Description :   Transmit a single byte using UART0
*
* Arguments   :   The byte that should be transmitted.
*
* Returns     :   None.
*********************************************************************************************************
*/

void  BSP_Ser_WrByte (CPU_INT08U tx_byte)
{
    CPU_INT16U  tx_flag;


    tx_flag = UART0->FR;
    while ((tx_flag & 0x0020) != 0) {                           /* Wait for an empty entry in the Tx FIFO                   */
        tx_flag = UART0->FR;
    }
    UART_SendData(UART0, tx_byte);
}

/*
*********************************************************************************************************
*                                                BSP_Ser_WrStr()
*
* Description :   Transmits a string using UART0
*
* Arguments   :   The string that will be transmitted.
*
* Returns     :   None.
*********************************************************************************************************
*/

void  BSP_Ser_WrStr (CPU_CHAR *tx_str)
{
    while ((*tx_str) != 0) {
        BSP_Ser_WrByte(*tx_str++);
        if (*tx_str == '\n') {
            BSP_Ser_WrByte('\r');        
        }
    }
}

/*
*********************************************************************************************************
*                                                BSP_Ser_RdByte()
*
* Description :   Receive a single byte using UART0
*
* Arguments   :   None.
*
* Returns     :   The received byte
*********************************************************************************************************
*/

CPU_INT08U  BSP_Ser_RdByte (void)
{
    CPU_INT16U  rx_flag;
    CPU_INT08U  rx_byte;


    rx_flag = UART0->FR;
    while ((rx_flag & 0x0010) != 0) {                           /* Wait for a byte to appear in the FIFO                    */
        OSTimeDly(5);
        rx_flag = UART0->FR;
    }
    rx_byte = UART_ReceiveData(UART0);
    return (rx_byte);
}

/*
*********************************************************************************************************
*                                                BSP_Ser_RdStr()
*
* Description :   This function reads a string using Channel 0 of the UART.
*
* Arguments   :   s      A pointer to a buffer at which the string can be stored
*                 len    The size of the string that will be read
*
* Returns     :   None
*********************************************************************************************************
*/

void  BSP_Ser_RdStr (CPU_CHAR    *rx_str,
                     CPU_INT32U   len)
{
    CPU_CHAR  input;
    CPU_CHAR  input_ix;


    input_ix  = 0;
    rx_str[0] = 0;

    while (1)
    {
        input = BSP_Ser_RdByte();

        if ((input == '\r') ||
            (input == '\n')) {
            BSP_Ser_Printf("\n");
            rx_str[input_ix] = 0;
            break;
        }

        if (input == '\b') {
            if (input_ix > 0) {
                BSP_Ser_Printf("\b \b");
                input_ix--;
                rx_str[input_ix] = 0;
            }
        }

        if (ASCII_IsPrint(input)) {
            BSP_Ser_Printf("%c", input);
            rx_str[input_ix] = input;
            input_ix++;
            if (input_ix >= len) {
               input_ix = len;
            }
        }
    }
}

/*
*********************************************************************************************************
*                                                BSP_Ser_Printf()
*
* Description :   Formatted output to the serial port.
*
* Arguments   :   Format string follwing the C format convention.
*
* Returns     :   None.
*********************************************************************************************************
*/

void  BSP_Ser_Printf (CPU_CHAR *format, ...)
{
   static  CPU_CHAR  buffer[80 + 1];
           va_list   vArgs;


   va_start(vArgs, format);
   vsprintf((char *)buffer, (char const *)format, vArgs);
   va_end(vArgs);

   BSP_Ser_WrStr((CPU_CHAR*) buffer);
}

/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                         SPI FUNCTIONS
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            BSP_SPI_En()
*
* Description: This function enables the SSP by removing from the reset state
*
* Arguments  : None
*
* Returns    : None
*********************************************************************************************************
*/

void  BSP_SPI_En (void)
{
    BSP_GPIOInit.GPIO_Pin         = 0x30;            /* GPIO5[4] CLK, GPIO5[5] MOSI.                   */
    BSP_GPIOInit.GPIO_Direction   = GPIO_PinOutput;
    BSP_GPIOInit.GPIO_Type        = GPIO_Type_PushPull;
    BSP_GPIOInit.GPIO_IPConnected = GPIO_IPConnected_Enable;
    BSP_GPIOInit.GPIO_Alternate   = GPIO_OutputAlt2;

    GPIO_Init(GPIO5, &BSP_GPIOInit);                 /* Initialize pins used by the SSP                */

    BSP_GPIOInit.GPIO_Pin         = 0x40;            /* GIO5[6] MISO.                                  */
    BSP_GPIOInit.GPIO_Direction   = GPIO_PinInput;
    BSP_GPIOInit.GPIO_Type        = GPIO_Type_PushPull;
    BSP_GPIOInit.GPIO_IPConnected = GPIO_IPConnected_Enable;
    BSP_GPIOInit.GPIO_Alternate   = GPIO_InputAlt1;

    GPIO_Init(GPIO5, &BSP_GPIOInit);

    BSP_GPIOInit.GPIO_Pin         = 0x80;            /* GPIO5[7] NSS.                                  */
    BSP_GPIOInit.GPIO_Direction   = GPIO_PinOutput;
    BSP_GPIOInit.GPIO_Type        = GPIO_Type_PushPull;
    BSP_GPIOInit.GPIO_IPConnected = GPIO_IPConnected_Enable;
    BSP_GPIOInit.GPIO_Alternate   = GPIO_OutputAlt2;

    GPIO_Init(GPIO5, &BSP_GPIOInit);

    SCU_APBPeriphClockConfig(__SSP0,   ENABLE);      /* Enable the SSP's clock signal                  */
    SCU_APBPeriphReset(__SSP0,   DISABLE);           /* Remove the SSP from reset state                */

}

/*
*********************************************************************************************************
*                                        BSP_SPI_Init()
*
* Description: This function initializes the SSP for use with an SD card reader
*
* Arguments  : spi_freq    The frequency at which the SSP should run
*
* Returns    : The actual frequency of the SSP
*********************************************************************************************************
*/

CPU_INT16U  BSP_SPI_Init (CPU_INT16U spi_freq)
{
    CPU_INT32U  pclk_val;
    CPU_INT16U  actual_freq;
    CPU_INT16U  div_val;
    CPU_INT08U  scr_val;
    CPU_INT08U  cpsdvr_val;


    SSP_Cmd(SSP0, DISABLE);                          /* Disable the SSP                                */

    pclk_val  = SCU_GetPCLKFreqValue();
                                                     /* Calculate the divider needed to produce the... */
                                                     /* ...serial clock. Round up if req'd.            */
    div_val   = ((((pclk_val * 100) / spi_freq) + 50) / 100) / 2;

                                                     /* Translate the divider into values that will... */
                                                     /* ...be written into the SSP's registers.        */
    if (div_val < 32256) {
        if (div_val < 257) {
            if (div_val == 0) {
                scr_val    = 0;
                cpsdvr_val = 2;
            } else {
                scr_val    = (CPU_INT08U)(div_val - 1);
                cpsdvr_val = 2;
            }
        } else {
            cpsdvr_val = (CPU_INT08U)(div_val / 128);
            if ((cpsdvr_val & 0x01) != 0) {
                cpsdvr_val++;
                scr_val = (CPU_INT08U)((div_val / cpsdvr_val) - 1);
            } else {
                scr_val = 255;
            }
        }
    } else {
        scr_val    = 255;
        cpsdvr_val = 254;
    }
                                                     /* Calculate the actual frequency                 */
    actual_freq = (CPU_INT16U)(pclk_val / ((cpsdvr_val) * (1 + scr_val)));

    SSP_StructInit(&BSP_SSPInit);                    /* Set up the SSP's initialization structure      */

    BSP_SSPInit.SSP_FrameFormat    = SSP_FrameFormat_Motorola;
    BSP_SSPInit.SSP_Mode           = SSP_Mode_Master;
    BSP_SSPInit.SSP_CPOL           = SSP_CPOL_High;
    BSP_SSPInit.SSP_CPHA           = SSP_CPHA_2Edge;
    BSP_SSPInit.SSP_DataSize       = SSP_DataSize_8b;
    BSP_SSPInit.SSP_SlaveOutput    = SSP_SlaveOutput_Disable;
    BSP_SSPInit.SSP_ClockRate      = scr_val;
    BSP_SSPInit.SSP_ClockPrescaler = cpsdvr_val;

    SSP_Init(SSP0, &BSP_SSPInit);                    /* Initialize the SSP with the structure's values */

    SSP_Cmd(SSP0, ENABLE);                           /* Enable the SSP                                 */

    return (actual_freq);                            /* Return the serial clock's actual frequency     */
}

/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                 STATIC BOARD SUPPORT INITIALIZATION FUNCTIONS
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                    INITIALIZE INTERRUPT CONTROLLER
*
* Description : This function initializes the Vectored Interrupt Controller (VIC).
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

static  void  BSP_VIC_Init (void)
{
    CPU_INT08U  i;
    CPU_INT32U  int_src;


    int_src = 0;

    SCU_AHBPeriphClockConfig(__VIC,   ENABLE);                  /* Enable the VIC's clock signal                            */
    SCU_AHBPeriphReset(__VIC,   DISABLE);                       /* Remove the VIC from reset state                          */

    for (i = 0; i < BSP_VIC_CHANNELS; i++) {
        VIC_ITCmd(int_src, DISABLE);
        int_src++;
    }

    for (i = 0; i < BSP_VIC_CHANNELS; i++) {
        VIC_ITCmd(int_src, DISABLE);
        int_src++;
    }
}

/*
*********************************************************************************************************
*                                          BSP_IntVectSet()
*
* Description : This function sets the specified interrupt source's ISR handler call back function.
*
* Arguments   : vec_nbr     Interrupt source number who's ISR handler address is to be configured. 0-31.
*
*               prio        Interrupt priority level. 0-15.
*
*               pfnct       Pointer to ISR handler function in the form of 'void ISR_Handler (void)'.
*
* Returns    : None
*********************************************************************************************************
*/

void  BSP_IntVectSet (CPU_INT08U vec_nbr, CPU_INT08U prio, BSP_PFNCT pfnct)
{
    if (vec_nbr > 31) {
        return;
    }

    if (pfnct == (void *)0) {
        return;
    }

    if (vec_nbr < 16) {
        VIC0->VAiR[prio] = (CPU_INT32U)pfnct;                   /* Specified vector is attached to VIC0                 */
    } else {
        VIC1->VAiR[prio] = (CPU_INT32U)pfnct;                   /* Specified vector is attached to VIC1                 */
    }
}

/*
*********************************************************************************************************
*                                          BSP_IntEn()
*
* Description : This function sets the specified interrupt source's ISR handler call back function.
*
* Arguments   : vec_nbr     Interrupt source number who's ISR handler address is to be configured. 0-31.
*
*               prio        Interrupt priority level. 0-15.
*
*               type        Interrupt type.  BSP_INT_TYPE_IRQ  or  BSP_INT_TYPE_FIQ.
*
* Returns    : None
*********************************************************************************************************
*/

void BSP_IntEn (CPU_INT08U vec_nbr, CPU_INT08U prio, CPU_INT08U type)
{
    if (vec_nbr > 31) {
        return;
    }

    if ((type != BSP_INT_TYPE_IRQ) &&
        (type != BSP_INT_TYPE_FIQ)) {
        return;
    }

    if (prio > 15) {
        return;
    }

    VIC_ITModeConfig(vec_nbr, (VIC_ITLineMode)type);
    VIC_VectEnableConfig(vec_nbr, prio);
    VIC_ITSourceConfig(vec_nbr, prio);
    VIC_ITCmd(vec_nbr,  ENABLE);
}


/*
*********************************************************************************************************
*                                          BSP_IntDis()
*
* Description : This function disbale the specified interrupt source.
*
* Arguments   : vec_nbr     Interrupt source number to be disabled. 0-31.
*
* Return(s)   : none.
*
* Caller(s)   : Applciation
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntDis (CPU_INT08U vec_nbr)
{
    if (vec_nbr > 31) {
        return;
    }

    VIC_ITCmd(vec_nbr,  DISABLE);
}

/*
*********************************************************************************************************
*                                          BSP_IO_Init()
*
* Description : This function initializes the GPIO pins used by the application.
*
* Arguments   : None
*
* Returns    : None
*********************************************************************************************************
*/

static  void  BSP_IO_Init (void)
{
    SCU_APBPeriphClockConfig(__GPIO3, ENABLE);
    SCU_APBPeriphClockConfig(__GPIO4, ENABLE);
    SCU_APBPeriphClockConfig(__GPIO5, ENABLE);
    SCU_APBPeriphClockConfig(__GPIO6, ENABLE);
    SCU_APBPeriphClockConfig(__GPIO7, ENABLE);
#ifdef DISP_MODULE_PRESENT
    SCU_APBPeriphClockConfig(__GPIO8, ENABLE);
    SCU_APBPeriphClockConfig(__GPIO9, ENABLE);
#endif

    SCU_APBPeriphReset(__GPIO3, DISABLE);                       /* Remove the ports from reset state                        */
    SCU_APBPeriphReset(__GPIO4, DISABLE);
    SCU_APBPeriphReset(__GPIO5, DISABLE);
    SCU_APBPeriphReset(__GPIO6, DISABLE);
    SCU_APBPeriphReset(__GPIO7, DISABLE);
#ifdef DISP_MODULE_PRESENT
    SCU_APBPeriphReset(__GPIO8, DISABLE);
    SCU_APBPeriphReset(__GPIO9, DISABLE);
#endif

    BSP_GPIOInit.GPIO_Pin         = 0xF3;
    BSP_GPIOInit.GPIO_Direction   = GPIO_PinOutput;
    BSP_GPIOInit.GPIO_Type        = GPIO_Type_PushPull;
    BSP_GPIOInit.GPIO_IPConnected = GPIO_IPConnected_Disable;
    BSP_GPIOInit.GPIO_Alternate   = GPIO_OutputAlt1;

    GPIO_Init(GPIO3, &BSP_GPIOInit);

    BSP_GPIOInit.GPIO_Pin         = 0xFF;

    GPIO_Init(GPIO6, &BSP_GPIOInit);

    BSP_GPIOInit.GPIO_Pin         = 0x60;
    BSP_GPIOInit.GPIO_Direction   = GPIO_PinInput;
    BSP_GPIOInit.GPIO_Type        = GPIO_Type_PushPull;
    BSP_GPIOInit.GPIO_IPConnected = GPIO_IPConnected_Disable;
    BSP_GPIOInit.GPIO_Alternate   = GPIO_InputAlt1;

    GPIO_Init(GPIO7, &BSP_GPIOInit);                            /* Initialize GPIO pins corresponding to buttons            */

#ifdef DISP_MODULE_PRESENT
    GPIO8->DDR = 0xFF;
    GPIO9->DDR = 0xFF;                                          /* Set the direction of the LCD pins                        */
#endif

    GPIO_ANAPinConfig(GPIO_ANAChannel5, ENABLE);                /* Configure GPIO4 pin 5 as analog input (for the ADC)      */
    GPIO_ANAPinConfig(GPIO_ANAChannel7, ENABLE);                /* Configure GPIO4 pin 7 as analog input (for the ADC)      */

                                                                /* Configure GPIO4 pin 6 for the PWM (for the speaker)      */
    GPIO_StructInit(&BSP_GPIOInit);
    BSP_GPIOInit.GPIO_Pin           = GPIO_Pin_6;
    BSP_GPIOInit.GPIO_Direction     = GPIO_PinOutput;
    BSP_GPIOInit.GPIO_Type          = GPIO_Type_PushPull;
    BSP_GPIOInit.GPIO_IPConnected   = GPIO_IPConnected_Disable;
    BSP_GPIOInit.GPIO_Alternate     = GPIO_OutputAlt2;
    GPIO_Init(GPIO4,&BSP_GPIOInit);
}
