;******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
;* File Name          : 91x_vect.s
;* Author             : MCD Application Team
;* Date First Issued  :  05/18/2006 : Version 1.0
;* Description        : This File used to initialize the exception and IRQ
;*                      vectors, and to enter/return to/from exceptions
;*                      handlers.
;*******************************************************************************
* History:
* 01/05/2008 : Version 1.3
* 05/22/2007 : Version 1.2
* 05/24/2006 : Version 1.1
* 05/18/2006 : Version 1.0
;*******************************************************************************
; THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
; CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
; A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
; OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
; OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
; CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;******************************************************************************/

    SECTION .intvec:CODE:ROOT(2)		
    CODE32


VectorAddress          EQU    0xFFFFF030  ; VIC Vector address register address.
VectorAddressDaisy     EQU    0xFC000030  ; Daisy VIC Vector address register
I_Bit                  EQU    0x80 ; when I bit is set, IRQ is disabled
F_Bit                  EQU    0x40 ; when F bit is set, FIQ is disabled


;*******************************************************************************
;              Import  the __iar_program_start address from 91x_init.s
;*******************************************************************************

       IMPORT  __iar_program_start

;*******************************************************************************
;                      Import exception handlers
;*******************************************************************************

        IMPORT  Undefined_Handler
        IMPORT  SWI_Handler
        IMPORT  Prefetch_Handler
        IMPORT  Abort_Handler
        IMPORT  FIQ_Handler
        IMPORT  WDG_IRQHandler
        IMPORT  SW_IRQHandler
        IMPORT  ARMRX_IRQHandler
        IMPORT  ARMTX_IRQHandler
        IMPORT  TIM0_IRQHandler
        IMPORT  TIM1_IRQHandler
        IMPORT  TIM2_IRQHandler
        IMPORT  TIM3_IRQHandler
        IMPORT  USBHP_IRQHandler
        IMPORT  USBLP_IRQHandler
        IMPORT  SCU_IRQHandler
        IMPORT  ENET_IRQHandler
        IMPORT  DMA_IRQHandler
        IMPORT  CAN_IRQHandler
        IMPORT  MC_IRQHandler
        IMPORT  ADC_IRQHandler
        IMPORT  UART0_IRQHandler
        IMPORT  UART1_IRQHandler
        IMPORT  UART2_IRQHandler
        IMPORT  I2C0_IRQHandler
        IMPORT  I2C1_IRQHandler
        IMPORT  SSP0_IRQHandler
        IMPORT  SSP1_IRQHandler
        IMPORT  LVD_IRQHandler
        IMPORT  RTC_IRQHandler
        IMPORT  WIU_IRQHandler
        IMPORT  EXTIT0_IRQHandler
        IMPORT  EXTIT1_IRQHandler
        IMPORT  EXTIT2_IRQHandler
        IMPORT  EXTIT3_IRQHandler
        IMPORT  USBWU_IRQHandler
        IMPORT  PFQBC_IRQHandler

        PUBLIC  WDG_IRQHandler_l
        PUBLIC  SW_IRQHandler_l
        PUBLIC  ARMRX_IRQHandler_l
        PUBLIC  ARMTX_IRQHandler_l
        PUBLIC  TIM0_IRQHandler_l
        PUBLIC  TIM1_IRQHandler_l
        PUBLIC  TIM2_IRQHandler_l
        PUBLIC  TIM3_IRQHandler_l
        PUBLIC  USBHP_IRQHandler_l
        PUBLIC  USBLP_IRQHandler_l
        PUBLIC  SCU_IRQHandler_l
        PUBLIC  ENET_IRQHandler_l
        PUBLIC  DMA_IRQHandler_l
        PUBLIC  CAN_IRQHandler_l
        PUBLIC  MC_IRQHandler_l
        PUBLIC  ADC_IRQHandler_l
        PUBLIC  UART0_IRQHandler_l
        PUBLIC  UART1_IRQHandler_l
        PUBLIC  UART2_IRQHandler_l
        PUBLIC  I2C0_IRQHandler_l
        PUBLIC  I2C1_IRQHandler_l
        PUBLIC  SSP0_IRQHandler_l
        PUBLIC  SSP1_IRQHandler_l
        PUBLIC  LVD_IRQHandler_l
        PUBLIC  RTC_IRQHandler_l
        PUBLIC  WIU_IRQHandler_l
        PUBLIC  EXTIT0_IRQHandler_l
        PUBLIC  EXTIT1_IRQHandler_l
        PUBLIC  EXTIT2_IRQHandler_l
        PUBLIC  EXTIT3_IRQHandler_l
        PUBLIC  USBWU_IRQHandler_l
        PUBLIC  PFQBC_IRQHandler_l


;*******************************************************************************
;            Export Peripherals IRQ handlers table address
;*******************************************************************************

;*******************************************************************************
;                        Exception vectors
;*******************************************************************************

        LDR     PC, Reset_Addr
        LDR     PC, Undefined_Addr
        LDR     PC, SWI_Addr
        LDR     PC, Prefetch_Addr
        LDR     PC, Abort_Addr
        NOP                             ; Reserved vector
        LDR     PC, [pc, #-0xff0]

;*******************************************************************************
;* Function Name  : FIQHandler
;* Description    : This function is called when FIQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
FIQHandler
       SUB    lr,lr,#4        ; Update the link register.
       STMFD  sp!,{r0-r7,lr}  ; Save The workspace plus the current return
                             ; address lr_fiq into the FIQ stack.
       ldr r0,=FIQ_Handler
       ldr lr,=FIQ_Handler_end
       bx r0                 ;Branch to FIQ_Handler.
FIQ_Handler_end:

      LDMFD   sp!,{r0-r7,pc}^; Return to the instruction following...
                              ; ...the exception interrupt.


;*******************************************************************************
;               Exception handlers address table
;*******************************************************************************

Reset_Addr      DCD     __iar_program_start
Undefined_Addr  DCD     UndefinedHandler
SWI_Addr        DCD     SWIHandler
Prefetch_Addr   DCD     PrefetchAbortHandler
Abort_Addr      DCD     DataAbortHandler
                DCD     0               ; Reserved vector

;*******************************************************************************
;                                  MACRO
;*******************************************************************************
;*******************************************************************************
;* Macro Name     : SaveContext
;* Description    : This macro is used to save the context before entering
;                   an exception handler.
;* Input          : The range of registers to store.
;* Output         : none
;*******************************************************************************

SaveContext MACRO reg1,reg2
        STMFD  sp!,{reg1-reg2,lr} ; Save The workspace plus the current return
                              ; address lr_ mode into the stack.
        MRS    r1,spsr        ; Save the spsr_mode into r1.
        STMFD  sp!,{r1}       ; Save spsr.
        ENDM

;*******************************************************************************
;* Macro Name     : RestoreContext
;* Description    : This macro is used to restore the context to return from
;                   an exception handler and continue the program execution.
;* Input          : The range of registers to restore.
;* Output         : none
;*******************************************************************************

RestoreContext MACRO reg1,reg2
        LDMFD   sp!,{r1}        ; Restore the saved spsr_mode into r1.
        MSR     spsr_cxsf,r1    ; Restore spsr_mode.
        LDMFD   sp!,{reg1-reg2,pc}^; Return to the instruction following...
                                ; ...the exception interrupt.
        ENDM

;*******************************************************************************
;                         Exception Handlers
;*******************************************************************************


;*******************************************************************************
;* Function Name  : UndefinedHandler
;* Description    : This function is called when undefined instruction
;                   exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************

UndefinedHandler
        SaveContext r0,r12    ; Save the workspace plus the current
                              ; return address lr_ und and spsr_und.
       ldr r0,=Undefined_Handler
       ldr lr,=Undefined_Handler_end
       bx r0                 ; Branch to Undefined_Handler.

Undefined_Handler_end:
        RestoreContext r0,r12 ; Return to the instruction following...
                              ; ...the undefined instruction.

;*******************************************************************************
;* Function Name  : SWIHandler
;* Description    : This function is called when SWI instruction executed.
;* Input          : none
;* Output         : none
;*******************************************************************************

SWIHandler
        SaveContext r0,r12    ; Save the workspace plus the current
                              ; return address lr_ svc and spsr_svc.
        ldr r0,=SWI_Handler
       ldr lr,=SWI_Handler_end
       bx r0                 ; Branch to SWI_Handler.

SWI_Handler_end:
        RestoreContext r0,r12 ; Return to the instruction following...
                              ; ...the SWI instruction.

;*******************************************************************************
;* Function Name  : PrefetchAbortHandler
;* Description    : This function is called when Prefetch Abort
;                   exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************

PrefetchAbortHandler
        SUB    lr,lr,#4       ; Update the link register.
        SaveContext r0,r12    ; Save the workspace plus the current
                              ; return address lr_abt and spsr_abt.

       ldr r0,=Prefetch_Handler
       ldr lr,=Prefetch_Handler_end
       bx r0                 ; Branch to Prefetch_Handler.

Prefetch_Handler_end:
        RestoreContext r0,r12 ; Return to the instruction following that...
                              ; ...has generated the prefetch abort exception.

;*******************************************************************************
;* Function Name  : DataAbortHandler
;* Description    : This function is called when Data Abort
;                   exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************

DataAbortHandler
        SUB    lr,lr,#8       ; Update the link register.
        SaveContext r0,r12    ; Save the workspace plus the current
                              ; return address lr_ abt and spsr_abt.
       ldr r0,=Abort_Handler
       ldr lr,=Abort_Handler_end
       bx r0                 ; Branch to Abort_Handler.

Abort_Handler_end:

        RestoreContext r0,r12 ; Return to the instruction following that...
                              ; ...has generated the data abort exception.

;*******************************************************************************
;* Function Name  : WDG_IRQHandler_l
;* Description    : This function is called when WDT IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
WDG_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=WDG_IRQHandler
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : SW_IRQHandler_l
;* Description    : This function is called when SW IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
SW_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=SW_IRQHandler
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : ARMRX_IRQHandler_l
;* Description    : This function is called when ARMRX IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
ARMRX_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=ARMRX_IRQHandler
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : ARMTX_IRQHandler_l
;* Description    : This function is called when ARMTX IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
ARMTX_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=ARMTX_IRQHandler
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : TIM0_IRQHandler_l
;* Description    : This function is called when TIM0 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
TIM0_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=TIM0_IRQHandler
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : TIM1_IRQHandler_l
;* Description    : This function is called when TIM1 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
TIM1_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=TIM1_IRQHandler
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : TIM2_IRQHandler_l
;* Description    : This function is called when TIM2 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
TIM2_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=TIM2_IRQHandler
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : TIM3_IRQHandler_l
;* Description    : This function is called when TIM3 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
TIM3_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=TIM3_IRQHandler
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : USBHP_IRQHandler_l
;* Description    : This function is called when USBHP IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
USBHP_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=USBHP_IRQHandler
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : USBLP_IRQHandler_l
;* Description    : This function is called when USBLP IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
USBLP_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=USBLP_IRQHandler
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : SCU_IRQHandler_l
;* Description    : This function is called when SCU IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
SCU_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=SCU_IRQHandler
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : ENET_IRQHandler_l
;* Description    : This function is called when ENET IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
ENET_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=ENET_IRQHandler
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : DMA_IRQHandler_l
;* Description    : This function is called when DMA IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
DMA_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=DMA_IRQHandler
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : CAN_IRQHandler_l
;* Description    : This function is called when CAN IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
CAN_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=CAN_IRQHandler
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : MC_IRQHandler_l
;* Description    : This function is called when MC IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
MC_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=MC_IRQHandler
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : ADC_IRQHandler_l
;* Description    : This function is called when ADC IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
ADC_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=ADC_IRQHandler
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : UART0_IRQHandler_l
;* Description    : This function is called when UART0 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
UART0_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       LDR    r1, = VectorAddressDaisy
       LDR    r1, [r1]
       ADD    pc,r1,#28
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=UART0_IRQHandler    ; Branch to the IRQ handler.
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy; Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : UART1_IRQHandler_l
;* Description    : This function is called when UART1 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
UART1_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       LDR    r1, = VectorAddressDaisy
       LDR    r1, [r1]
       ADD    pc,r1,#28
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=UART1_IRQHandler    ; Branch to the IRQ handler.
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r1,[r0]                 ; respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy; Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : UART2_IRQHandler_l
;* Description    : This function is called when UART2 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
UART2_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       LDR    r1, = VectorAddressDaisy
       LDR    r1, [r1]
       ADD    pc,r1,#28
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=UART2_IRQHandler    ; Branch to the IRQ handler.
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r0,[r0]                 ; respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy; Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : I2C0_IRQHandler_l
;* Description    : This function is called when I2C0 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
I2C0_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       LDR    r1, = VectorAddressDaisy
       LDR    r1, [r1]
       ADD    pc,r1,#28
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=I2C0_IRQHandler    ; Branch to the IRQ handler.
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r0,[r0]                 ; respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy; Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : I2C1_IRQHandler_l
;* Description    : This function is called when I2C1 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
I2C1_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       LDR    r1, = VectorAddressDaisy
       LDR    r1, [r1]
       ADD    pc,r1,#28
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=I2C1_IRQHandler    ; Branch to the IRQ handler.
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r0,[r0]                 ; respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy; Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : SSP0_IRQHandler_l
;* Description    : This function is called when SSP0 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
SSP0_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       LDR    r1, = VectorAddressDaisy
       LDR    r1, [r1]
       ADD    pc,r1,#28
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=SSP0_IRQHandler     ; Branch to the IRQ handler.
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r0,[r0]                 ; respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy; Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : SSP1_IRQHandler_l
;* Description    : This function is called when SSP1 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
SSP1_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       LDR    r1, = VectorAddressDaisy
       LDR    r1, [r1]
       ADD    pc,r1,#28
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=SSP1_IRQHandler     ; Branch to the IRQ handler.
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r0,[r0]                 ; respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy; Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : LVD_IRQHandler_l
;* Description    : This function is called when LVD IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
LVD_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       LDR    r1, = VectorAddressDaisy
       LDR    r1, [r1]
       ADD    pc,r1,#28
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=LVD_IRQHandler      ; Branch to the IRQ handler.
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r0,[r0]                 ; respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy; Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : RTC_IRQHandler_l
;* Description    : This function is called when RTC IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
RTC_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       LDR    r1, = VectorAddressDaisy
       LDR    r1, [r1]
       ADD    pc,r1,#28
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=RTC_IRQHandler      ; Branch to the IRQ handler.
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r0,[r0]                 ; respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy; Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : WIU_IRQHandler_l
;* Description    : This function is called when UART1 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
WIU_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       LDR    r1, = VectorAddressDaisy
       LDR    r1, [r1]
       ADD    pc,r1,#28
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=WIU_IRQHandler      ; Branch to the IRQ handler.
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r0,[r0]                 ; respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy; Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : EXTIT0_IRQHandler_l
;* Description    : This function is called when EXIT0 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
EXTIT0_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       LDR    r1, = VectorAddressDaisy
       LDR    r1, [r1]
       ADD    pc,r1,#28
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=EXTIT0_IRQHandler   ; Branch to the IRQ handler.
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r0,[r0]                 ; respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy; Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : EXTIT1_IRQHandler_l
;* Description    : This function is called when EXTIT1 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
EXTIT1_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       LDR    r1, = VectorAddressDaisy
       LDR    r1, [r1]
       ADD    pc,r1,#28
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=EXTIT1_IRQHandler   ; Branch to the IRQ handler.
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r0,[r0]                 ; respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy; Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : EXTIT2_IRQHandler_l
;* Description    : This function is called when EXTIT2 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
EXTIT2_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       LDR    r1, = VectorAddressDaisy
       LDR    r1, [r1]
       ADD    pc,r1,#28
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=EXTIT2_IRQHandler   ; Branch to the IRQ handler.
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r0,[r0]                 ; respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy; Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : EXTIT3_IRQHandler_l
;* Description    : This function is called when EXTIT3 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
EXTIT3_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       LDR    r1, = VectorAddressDaisy
       LDR    r1, [r1]
       ADD    pc,r1,#28
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=EXTIT3_IRQHandler   ; Branch to the IRQ handler.
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r0,[r0]                 ; respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy; Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : USBWU_IRQHandler_l
;* Description    : This function is called when UART1 IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
USBWU_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       LDR    r1, = VectorAddressDaisy
       LDR    r1, [r1]
       ADD    pc,r1,#28
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=USBWU_IRQHandler    ; Branch to the IRQ handler.
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r0,[r0]                 ; respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy; Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

;*******************************************************************************
;* Function Name  : PFQBC_IRQHandler_l
;* Description    : This function is called when PFQBC IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
PFQBC_IRQHandler_l:
       SUB    lr,lr,#4                ; Update the link register
       SaveContext r0,r12             ; Save the workspace plus the current
                                      ; return address lr_irq and spsr_irq
       LDR    r1, = VectorAddressDaisy
       LDR    r1, [r1]
       ADD    pc,r1,#28
       MSR    cpsr_c,#0x1F            ; Switch to SYS mode and enable IRQ
       STMFD  sp!,{lr}                ; Save the link register.
       LDR    r0,=PFQBC_IRQHandler    ; Branch to the IRQ handler.
       BLX    r0                      ; Branch to the IRQ handler.
       LDMFD  sp!,{lr}                ; Restore the link register.
       MSR    cpsr_c,#0xD2 | I_Bit    ; Switch to IRQ mode and disable IRQ
       LDR    r0, = VectorAddress     ; Write to the VectorAddress to clear the
       STR    r0,[r0]                 ; respective interrupt in the internal interrupt
       LDR    r1, = VectorAddressDaisy; Write to the VectorAddressDaisy to clear the
       STR    r1,[r1]                 ; respective interrupt in the internal interrupt
       RestoreContext r0,r12          ; Restore the context and return to the...

       LTORG

       END
;******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****
