/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : 91x_mc.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file provides all the MC software functions.
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

/* Includes ------------------------------------------------------------------*/
#include "91x_mc.h"
#include "91x_scu.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#define MC_ODCS_Set    0x0001
#define MC_ODCS_Reset  0x00FE

#define MC_CMS_Set    0x0004
#define MC_CMS_Reset  0x00FB

#define MC_CPC_Set  0x0008
#define MC_CTC_Set  0x0010

#define MC_PCE_Set   0x0020
#define MC_PCE_Reset 0xFFDF

#define MC_TCE_Set   0x0040
#define MC_TCE_Reset 0x00BF

#define MC_DTE_Set   0x0080
#define MC_DTE_Reset 0x007F

#define MC_TCB_Set    0x0004
#define MC_TCB_Reset  0x00FB

#define MC_STC_Set  0x0008

#define MC_TES_Set    0x0010
#define MC_TES_Reset  0x00EF

#define MC_CCPT_Set    0x0020
#define MC_CCPT_Reset  0x005F

#define MC_DISEST_Set    0x0040
#define MC_DISEST_Reset  0x003F

#define MC_DTS_Set    0x0001
#define MC_DTS_Reset  0x00FE

#define MC_SDT_Set    0x0002

#define MC_C0SE_Set    0x0004
#define MC_C0SE_Reset  0x00FB

#define MC_CUSE_Set    0x0008
#define MC_CUSE_Reset  0x00F7 

#define MC_CVSE_Set    0x0010
#define MC_CVSE_Reset  0x00EF

#define MC_CWSE_Set   0x0020
#define MC_CWSE_Reset 0x00D0

#define MC_RSE_Set    0x0040
#define MC_RSE_Reset  0x00BF

#define MC_GPI_Set    0x0080
#define MC_GPI_Reset  0x007F

#define MC_PUH_Set    0x0020
#define MC_PUH_Reset  0x005F
#define MC_PUL_Set    0x0010
#define MC_PUL_Reset  0x006F

#define MC_PVH_Set    0x0008
#define MC_PVH_Reset  0x0077
#define MC_PVL_Set    0x0004
#define MC_PVL_Reset  0x007B

#define MC_PWH_Set    0x0002
#define MC_PWH_Reset  0x007D
#define MC_PWL_Set    0x0001
#define MC_PWL_Reset  0x007E

#define MC_ODS_Set    0x0040
#define MC_ODS_Reset  0xFF3F

#define MC_ESC_Clear  0x4321

#define MC_PCR1_TIN_MASK 0xFFFC
#define MC_OPR_Mask      0x0040 
#define MC_UDCS_Mask     0x0002
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/******************************************************************************
* Function Name  : MC_DeInit
* Description    : Deinitializes MC peripheral registers to their default reset
*                  values.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MC_DeInit(void)
{      
  /* MC peripheral is under Reset */
  SCU_APBPeriphReset(__MC, ENABLE);

  /* MC peripheral Reset off */
  SCU_APBPeriphReset(__MC, DISABLE);           
}

/*******************************************************************************
* Function Name  : MC_Init
* Description    : Initializes the MC peripheral according to the specified
*                  parameters in the MC_InitStruct .
* Input          : IMC_InitStruct: pointer to a MC_InitTypeDef structure that
*                  contains the configuration information for the MC peripheral.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_Init(MC_InitTypeDef* MC_InitStruct)
{
  /* Select the operating Mode */
  if(MC_InitStruct->MC_OperatingMode == MC_SoftwareOperating_Mode)
   {
     /* Select the Data transfer Mode */
     MC->PCR2 |= MC_DTS_Set;

     /* Enable the registers Software Data Transfer */
     MC->PCR2 |= MC_C0SE_Set | MC_CUSE_Set | MC_CVSE_Set | MC_CWSE_Set 
                 |MC_RSE_Set;
     
   }
   else
   {
     /* Select the Data transfer Mode */
     MC->PCR2 &= MC_DTS_Reset;   

     /* Disable the registers Software Data Transfer */
     MC->PCR2 &= MC_C0SE_Reset | MC_CUSE_Reset | MC_CVSE_Reset | MC_CWSE_Reset 
                 |MC_RSE_Reset;
    }

   /* Select the MC PWM counter Mode */
   if(MC_InitStruct->MC_PWMMode == MC_PWMZeroCentered_Mode)
   {
     MC->PCR0 |= MC_CMS_Set;
   }
   else
   {
     MC->PCR0 &= MC_CMS_Reset;
   }

   /* Set the MC PWM counter Prescaler */
   MC->CPRS = MC_InitStruct->MC_Prescaler;

   /* Set the MC PWM Period */
   MC->CMP0 = MC_InitStruct->MC_Period;

   /* Set the MC PWM Repetition counter */
   MC->REP = MC_InitStruct->MC_RepetitionCounter;

   /* Set the Tacho Compare value */
   MC->TCMP = MC_InitStruct->MC_TachoPeriod;

   /* Set the Tacho Prescaler value */
   MC->TPRS = MC_InitStruct->MC_TachoPrescaler;

   /* Set the MC Tacho Input Polarity */
   MC->PCR1 = (MC->PCR1 & MC_PCR1_TIN_MASK) | MC_InitStruct->MC_TachoPolarity;

   /* Set the MC PWM Forced State */
   MC->OPR |= MC_ODS_Set;
   MC->OPR = (MC->OPR & MC_OPR_Mask) | MC_InitStruct->MC_ForcedPWMState; 
   
   /* Select the Tacho Mode */
   if(MC_InitStruct->MC_TachoMode == MC_TachoOneShot_Mode)
   {
     MC->PCR1 |= MC_TCB_Set;
   }
   else
   {
     MC->PCR1 &= MC_TCB_Reset;
   }

   /* Select the Tacho Event Mode */
   if(MC_InitStruct->MC_TachoEvent_Mode == MC_TachoEvent_Software_Mode)
   {
     MC->PCR1 |= MC_TES_Set;
   }
   else
   {
     MC->PCR1 &= MC_TES_Reset;
   }

   /* Enable or disable the emergency input */
   if(MC_InitStruct->MC_Emergency == MC_Emergency_Enable)
   {
     MC->PCR1 &= MC_DISEST_Reset;
   }
   else
   {
     MC->PCR1 |= MC_DISEST_Set;
   }

/* Select the complementary Mode */
   if(MC_InitStruct->MC_Complementary == MC_Complementary_Enable)
   {
     MC->DTG = MC_InitStruct->MC_DeadTime;
     MC->PCR0 |= MC_ODCS_Set;
   }
   else
   {
     MC->PCR0 &= MC_ODCS_Reset;
   }

   /* Tacho Mode selection */
   if(MC_InitStruct->MC_TachoMode == MC_TachoOneShot_Mode)
   {
     MC->PCR1 |= MC_TCB_Set;
   }
   else
   {
     MC->PCR1 &= MC_TCB_Reset;
   }

   switch(MC_InitStruct->MC_Channel)
   {
     /* Channel U configuration */
     case MC_Channel_U:
     {   
       MC->CMPU = MC_InitStruct->MC_PulseU;

       if(MC_InitStruct->MC_PolarityUL == MC_Polarity_Inverted)
       {
       	 MC->PSR |= MC_PUL_Set;
       }
       else
       {
         MC->PSR &= MC_PUL_Reset;
       }
       if(MC_InitStruct->MC_PolarityUH == MC_Polarity_Inverted)
       {
       	 MC->PSR |= MC_PUH_Set;
       }
       else
       {
         MC->PSR &= MC_PUH_Reset;
       }
       break;
     }

     /* Channel V configuration */
     case MC_Channel_V:
     {
       MC->CMPV = MC_InitStruct->MC_PulseV;

       if(MC_InitStruct->MC_PolarityVL == MC_Polarity_Inverted)
       {
       	 MC->PSR |= MC_PVL_Set;
       }
       else
       {
         MC->PSR &= MC_PVL_Reset;
       }
       if(MC_InitStruct->MC_PolarityVH == MC_Polarity_Inverted)
       {
       	 MC->PSR |= MC_PVH_Set;
       }
       else
       {
         MC->PSR &= MC_PVH_Reset;
       }
       break;
     }

     /* Channel W configuration */
     case MC_Channel_W:
     {
       MC->CMPW = MC_InitStruct->MC_PulseW;
       
       if(MC_InitStruct->MC_PolarityWL == MC_Polarity_Inverted)
       {
       	 MC->PSR |= MC_PWL_Set;
       }
       else
       {
         MC->PSR &= MC_PWL_Reset;
       }
       if(MC_InitStruct->MC_PolarityWH == MC_Polarity_Inverted)
       {
       	 MC->PSR |= MC_PWH_Set;
       }
       else
       {
         MC->PSR &= MC_PWH_Reset;
       }
       break;
     }
     /* All Channel Configuration */
     case MC_Channel_ALL:
     {
       MC->CMPU = MC_InitStruct->MC_PulseU;
       MC->CMPV = MC_InitStruct->MC_PulseV;
       MC->CMPW = MC_InitStruct->MC_PulseW;
       
       if(MC_InitStruct->MC_PolarityUL == MC_Polarity_Inverted)
       {
       	 MC->PSR |= MC_PUL_Set;
       }
       else
       {
         MC->PSR &= MC_PUL_Reset;
       }
       if(MC_InitStruct->MC_PolarityUH == MC_Polarity_Inverted)
       {
       	 MC->PSR |= MC_PUH_Set;
       }
       else
       {
         MC->PSR &= MC_PUH_Reset;
       }

       if(MC_InitStruct->MC_PolarityVL == MC_Polarity_Inverted)
       {
       	 MC->PSR |= MC_PVL_Set;
       }
       else
       {
         MC->PSR &= MC_PVL_Reset;
       }
       if(MC_InitStruct->MC_PolarityVH == MC_Polarity_Inverted)
       {
       	 MC->PSR |= MC_PVH_Set;
       }
       else
       {
         MC->PSR &= MC_PVH_Reset;
       }

       if(MC_InitStruct->MC_PolarityWL == MC_Polarity_Inverted)
       {
       	 MC->PSR |= MC_PWL_Set;
       }
       else
       {
         MC->PSR &= MC_PWL_Reset;
       }
       if(MC_InitStruct->MC_PolarityWH == MC_Polarity_Inverted)
       {
       	 MC->PSR |= MC_PWH_Set;
       }
       else
       {
         MC->PSR &= MC_PWH_Reset;
       }
     }
     default:
     break;
   }
}

/*******************************************************************************
* Function Name  : MC_StructInit
* Description    : Fills each MC_InitStruct member with its default value.
* Input          : MC_InitStruct : pointer to a MC_InitTypeDef structure which
*                  will be initialized.
* Output         : None                        
* Return         : None.
*******************************************************************************/
void MC_StructInit(MC_InitTypeDef* MC_InitStruct)
{
  MC_InitStruct->MC_OperatingMode = MC_HardwareOperating_Mode;
  MC_InitStruct->MC_TachoMode = MC_TachoContinuous_Mode;
  MC_InitStruct->MC_TachoEvent_Mode = MC_TachoEvent_Hardware_Mode;
  MC_InitStruct->MC_Prescaler = 0x00;
  MC_InitStruct->MC_TachoPrescaler = 0x0000;
  MC_InitStruct->MC_PWMMode = MC_PWMClassical_Mode;
  MC_InitStruct->MC_Complementary = MC_Complementary_Enable;
  MC_InitStruct->MC_Emergency = MC_Emergency_Disable;
  MC_InitStruct->MC_ForcedPWMState = 0x003F;
  MC_InitStruct->MC_Period = 0x0000;
  MC_InitStruct->MC_TachoPeriod = 0x00FF;
  MC_InitStruct->MC_Channel = MC_Channel_ALL;
  MC_InitStruct->MC_PulseU = 0x0000;
  MC_InitStruct->MC_PulseV = 0x0000;
  MC_InitStruct->MC_PulseW = 0x0000;
  MC_InitStruct->MC_PolarityUL = MC_Polarity_NonInverted;
  MC_InitStruct->MC_PolarityUH = MC_Polarity_NonInverted;
  MC_InitStruct->MC_PolarityVL = MC_Polarity_NonInverted;
  MC_InitStruct->MC_PolarityVH = MC_Polarity_NonInverted;
  MC_InitStruct->MC_PolarityWL = MC_Polarity_NonInverted;
  MC_InitStruct->MC_PolarityWH = MC_Polarity_NonInverted;
  MC_InitStruct->MC_TachoPolarity = MC_TachoEventEdge_RisingFalling;
  MC_InitStruct->MC_DeadTime = 0x003F;
  MC_InitStruct->MC_RepetitionCounter = 0x0000;
}

/*******************************************************************************
* Function Name  : MC_Cmd
* Description    : Enables or disables the MC peripheral.
* Input          : Newstate: new state of the MC peripheral.
*                  This parameter can be: ENABLE or DISABLE.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_Cmd(FunctionalState NewState)
{
  if(NewState == ENABLE)
  {
  /* Enable the PWM counter */
    MC->PCR0 |= MC_PCE_Set;
  
  /* Enable the Tacho counter */
    MC->PCR0 |= MC_TCE_Set;
  
  /* Enable the Dead Time counter */
    MC->PCR0 |= MC_DTE_Set;
  }
  else
  {
  /* Disable the PWM counter */
    MC->PCR0 &= MC_PCE_Reset;
    
  /* Disable the Tacho counter */
    MC->PCR0 &= MC_TCE_Reset;
    
  /* Disable the Dead counter */
    MC->PCR0 &= MC_DTE_Reset;
  }
}

/*******************************************************************************
* Function Name  : MC_ClearPWMCounter
* Description    : Clears the MC PWM counter.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MC_ClearPWMCounter(void)
{
/* Clear the PWM counter */
  MC->PCR0 |= MC_CPC_Set;
}

/*******************************************************************************
* Function Name  : MC_ClearTachoCounter
* Description    : Clears the MC Tacho counter.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MC_ClearTachoCounter(void)
{
/* Clear the Tacho counter */
  MC->PCR0 |= MC_CTC_Set;
}

/*******************************************************************************
* Function Name  : MC_CtrlPWMOutputs
* Description    : Enables or disables MC peripheral Main Outputs.
* Input          : Newstate: new state of the MC peripheral Main Outputs.
*                  This parameter can be: ENABLE or DISABLE.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_CtrlPWMOutputs(FunctionalState Newstate)
{
  if(Newstate == ENABLE)
  {
  /* Enable the dead time generator data */
    MC->OPR &= MC_ODS_Reset;
  }
  else
  {
  /* Enable the default state data */
    MC->OPR |= MC_ODS_Set;
  }
}

/*******************************************************************************
* Function Name  : MC_ITConfig
* Description    : Enables or disables the MC interrupts.
* Input          : - MC_IT: specifies the MC interrupts sources to be enabled
*                    or disabled.
*                    This parameter can be any combination of the following values:
*                         - MC_IT_CMPW: Compare W Interrupt.
*                         - MC_IT_CMPV: Compare V Interrupt.
*                         - MC_IT_CMPU: Compare U Interrupt.
*                         - MC_IT_ZPC: Zero of PWM counter Interrupt.
*                         - MC_IT_ADT: Automatic Data Transfer Interrupt.
*                         - MC_IT_OTC: Overflow of Tacho counter Interrupt.
*                         - MC_IT_CPT: Capture of Tacho counter Interrupt.
*                         - MC_IT_CM0: Compare 0 Interrupt.
*                  - Newstate: new state of IMC interrupts.
*                    This parameter can be: ENABLE or DISABLE.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_ITConfig(u16 MC_IT, FunctionalState NewState)
{
  if(NewState == ENABLE)
  {
    /* Enable the specific interrupt source */
    MC->IMR |= MC_IT;

    /* Enable the global peripheral interrupt sources */
    MC->PCR2 |= MC_GPI_Set;
  }
  else
  {   
    /* Disable the specific interrupt source */
    MC->IMR &= ~MC_IT;

    /* Disable the global peripheral interrupt sources */
    MC->PCR2 &= MC_GPI_Reset;
  }
}

/*******************************************************************************
* Function Name  : MC_SetPrescaler
* Description    : Sets the MC prescaler value.
* Input          : MC_Prescaler: MC prescaler new value.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_SetPrescaler(u8 MC_Prescaler)
{
/* Set the Prescaler Register value */
  MC->CPRS = MC_Prescaler;
}

/*******************************************************************************
* Function Name  : MC_SetPeriod
* Description    : Sets the MC period value.
* Input          : MC_Period: MC period new value.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_SetPeriod(u16 MC_Period)
{
/* Set the Period Register value */
  MC->CMP0 = MC_Period;
}

/*******************************************************************************
* Function Name  : MC_SetPulseU
* Description    : Sets the MC pulse U value.
* Input          : MC_PulseU: MC pulse U new value.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_SetPulseU(u16 MC_PulseU)
{
/* Set the Pulse U Register value */
  MC->CMPU = MC_PulseU;
}

/*******************************************************************************
* Function Name  : MC_SetPulseV
* Description    : Sets the MC pulse V value.
* Input          : MC_PulseV: MC pulse V new value.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_SetPulseV(u16 MC_PulseV)
{
/* Set the Pulse V Register value */
   MC->CMPV = MC_PulseV;
}

/*******************************************************************************
* Function Name  : MC_SetPulseW
* Description    : Sets the MC pulse W value.
* Input          : MC_PulseW: MC pulse W new value.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_SetPulseW(u16 MC_PulseW)
{
/* Set the Pulse W Register value */
   MC->CMPW = MC_PulseW;
}

/*******************************************************************************
* Function Name  : MC_PWMModeConfig
* Description    : Selects the MC PWM counter Mode.
* Input          : MC_PWMMode: MC PWM counter Mode.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_PWMModeConfig(u16 MC_PWMMode)
{
   /* Select the MC PWM counter Mode */
   if(MC_PWMMode == MC_PWMZeroCentered_Mode)
   {
     MC->PCR0 |= MC_CMS_Set;
   }
   else
   {
     MC->PCR0 &= MC_CMS_Reset;
   }
}

/*******************************************************************************
* Function Name  : MC_SetDeadTime
* Description    : Sets the MC dead time value.
* Input          : MC_DeadTime: MC dead time new value.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_SetDeadTime(u16 MC_DeadTime)
{
/* Set the dead time Register value */
  MC->DTG = MC_DeadTime;
}

/*******************************************************************************
* Function Name  : MC_SetTachoCompare
* Description    : Sets the MC Tacho Compare Register value.
* Input          : MC_Compare: MC Tacho compare new value.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_SetTachoCompare(u8 MC_Compare)
{
 /* Sets the Tacho Compare Register value */
  MC->TCMP = MC_Compare;
}
/*******************************************************************************
* Function Name  : MC_EmergencyCmd
* Description    : Enables or disables the MC emergency feauture.
* Input          : Newstate: new state of the MC peripheral Emergency.
*                  This parameter can be: ENABLE or DISABLE.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_EmergencyCmd(FunctionalState NewState)
{
  if(NewState == ENABLE)
  {
   /* Reset the DISEST Bit in the PCR1 Register to enable the emergency stop input */
    MC->PCR1 &= MC_DISEST_Reset;
  }
  else
  {
  /* Set the DISEST Bit in the PCR1 Register to disable the emergency stop input */
    MC->PCR1 |= MC_DISEST_Reset;
  }
}

/*******************************************************************************
* Function Name  : MC_EmergencyClear
* Description    : Clears the MC Emergency Register.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MC_EmergencyClear(void)
{
/* Force PWM outputs in safe state (All OFF) */
   MC->OPR |= MC_ODS_Set;

/* Clear EST bit */
   MC->ESC = MC_ESC_Clear;
   
/* Re-enable protection */
   MC->ESC = 0;
   
/* Enables the Outputs */
   MC->OPR &= MC_ODS_Reset;
}

/*******************************************************************************
* Function Name  : MC_GetPeriod
* Description    : Gets the MC period value.
* Input          : None
* Output         : None
* Return         : MC period value.
*******************************************************************************/
u16 MC_GetPeriod(void)
{
/* Return the PWM signal period value */
  return MC->CMP0;
}

/*******************************************************************************
* Function Name  : MC_GetPulseU
* Description    : Gets the MC pulse U value.
* Input          : None
* Output         : None
* Return         : MC pulse U value.
*******************************************************************************/
u16 MC_GetPulseU(void)
{
/* Return the PWM pulse U Register value */
  return MC->CMPU;
}

/*******************************************************************************
* Function Name  : MC_GetPulseV
* Description    : Gets the MC pulse V value.
* Input          : None
* Output         : None
* Return         : MC pulse V value.
*******************************************************************************/
u16 MC_GetPulseV(void)
{
/* Return the PWM pulse V Register value */
  return MC->CMPV;
}

/*******************************************************************************
* Function Name  : MC_GetPulseW
* Description    : Gets the MC pulse W value.
* Input          : None
* Output         : None
* Return         : MC pulse W value.
*******************************************************************************/
u16 MC_GetPulseW(void)
{
/* Return the PWM pulse W Register value */
  return MC->CMPW;
}

/*******************************************************************************
* Function Name  : MC_GetTachoCapture
* Description    : Gets the MC Tacho period value.
* Input          : None
* Output         : None
* Return         : MC Tacho capture value.
*******************************************************************************/
u16 MC_GetTachoCapture(void)
{
/* Return the Tacho Capture Register value */
  return MC->TCPT;
}

/*******************************************************************************
* Function Name  : MC_ClearOnTachoCapture
* Description    : Enables or disables the the Clear on capture of tacho counter.
* Input          : Newstate: new state of the CCPT bit.
*                  This parameter can be: ENABLE or DISABLE.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_ClearOnTachoCapture(FunctionalState NewState)
{
  if(NewState == ENABLE)
  {
    /* Enable the Clear on capture of tacho counter */
    MC->PCR1 |= MC_CCPT_Set;
  }
  else
  {   
    /* Disable the Clear on capture of tacho counter */
    MC->PCR1 &= MC_CCPT_Reset;
  }
}
/*******************************************************************************
* Function Name  : MC_ForceDataTransfer
* Description    : Sets the MC Outputs default states.
* Input          : MC_ForcedData: MC outputs new states.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_ForceDataTransfer(u8 MC_ForcedData)
{
   /* Set the MC PWM Forced State */
   MC->OPR |= MC_ODS_Set;
   MC->OPR = (MC->OPR & MC_OPR_Mask) | MC_ForcedData;
}

/*******************************************************************************
* Function Name  : MC_PreloadConfig
* Description    : Enables the Software Data Transfer.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MC_SoftwarePreloadConfig(void)
{
  /* Set the SDT: Software Data Transfer bit */
  MC->PCR2 |= MC_SDT_Set;
}

/*******************************************************************************
* Function Name  : MC_SoftwareTachoCapture
* Description    : Enables the Software Tacho Capture.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MC_SoftwareTachoCapture(void)
{
  /* Set the STC: Software Tacho Capture bit */
  MC->PCR1 |= MC_STC_Set;
}

/*******************************************************************************
* Function Name  : MC_GetCountingStatus
* Description    : Checks whether the PWM Counter is counting Up or Down.
* Input          : None
* Output         : None
* Return         : The new state of the PWM Counter(DOWN or UP).
*******************************************************************************/
CountingStatus MC_GetCountingStatus(void)
{
  if((MC->PCR0 & MC_UDCS_Mask) != DOWN)
  {
    return UP;
  }
  else
  {
    return DOWN;
  }
}

/*******************************************************************************
* Function Name  : MC_GetFlagStatus
* Description    : Checks whether the specified MC flag is set or not.
* Input          : MC_FLAG: specifies the flag to check.
*                  This parameter can be one of the following values:
*                         - MC_FLAG_CMPW: Compare W Flag.
*                         - MC_FLAG_CMPV: Compare V Flag.
*                         - MC_FLAG_CMPU: Compare U Flag.
*                         - MC_FLAG_ZPC: Zero of PWM counter Flag.
*                         - MC_FLAG_ADT: Automatic Data Transfer Flag.
*                         - MC_FLAG_OTC: Overflow of Tacho counter Flag.
*                         - MC_FLAG_CPT: Capture of Tacho counter Flag.
*                         - MC_FLAG_CM0: Compare 0 Flag.
*                         - MC_FLAG_EST: Emergency Stop Flag.
* Output         : None
* Return         : The new state of the MC_FLAG(SET or RESET).
*******************************************************************************/
FlagStatus MC_GetFlagStatus(u16 MC_FLAG)
{
  if((MC->IPR & MC_FLAG) != RESET)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/*******************************************************************************
* Function Name  : MC_ClearFlag
* Description    : Clears the MC’s pending flags.
* Input          : MC_FLAG: specifies the flag to clear.
*                  This parameter can be any combination of the following values:
*                         - MC_FLAG_CMPW: Compare W Flag.
*                         - MC_FLAG_CMPV: Compare V Flag.
*                         - MC_FLAG_CMPU: Compare U Flag.
*                         - MC_FLAG_ZPC: Zero of PWM counter Flag.
*                         - MC_FLAG_ADT: Automatic Data Transfer Flag.
*                         - MC_FLAG_OTC: Overflow of Tacho counter Flag.
*                         - MC_FLAG_CPT: Capture of Tacho counter Flag.
*                         - MC_FLAG_CM0: Compare 0 Flag.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_ClearFlag(u16 MC_FLAG)
{
/* Clear the corresponding Flag */
  MC->IPR &= ~MC_FLAG;
}

/*******************************************************************************
* Function Name  : MC_GetITStatus
* Description    : Checks whether the MC interrupt has occurred or not.
* Input          : MC_IT: specifies the MC interrupt source to check.
*                  This parameter can be one of the following values:
*                         - MC_IT_CMPW: Compare W Interrupt.
*                         - MC_IT_CMPV: Compare V Interrupt.
*                         - MC_IT_CMPU: Compare U Interrupt.
*                         - MC_IT_ZPC: Zero of PWM counter Interrupt.
*                         - MC_IT_ADT: Automatic Data Transfer Interrupt.
*                         - MC_IT_OTC: Overflow of Tacho counter Interrupt.
*                         - MC_IT_CPT: Capture of Tacho counter Interrupt.
*                         - MC_IT_CM0: Compare 0 Interrupt.
* Output         : None
* Return         : The new state of the MC_IT(SET or RESET).
*******************************************************************************/
ITStatus MC_GetITStatus(u16 MC_IT)
{
  if((MC->IPR & MC_IT) && (MC->IMR & MC_IT))
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/*******************************************************************************
* Function Name  : MC_ClearITPendingBit
* Description    : Clears the IMC's interrupt pending bits.
* Input          : MC_IT: specifies the pending bit to clear.
*                  This parameter can be any combination of the following values:
*                         - MC_IT_CMPW: Compare W Interrupt.
*                         - MC_IT_CMPV: Compare V Interrupt.
*                         - MC_IT_CMPU: Compare U Interrupt.
*                         - MC_IT_ZPC: Zero of PWM counter Interrupt.
*                         - MC_IT_ADT: Automatic Data Transfer Interrupt.
*                         - MC_IT_OTC: Overflow of Tacho counter Interrupt.
*                         - MC_IT_CPT: Capture of Tacho counter Interrupt.
*                         - MC_IT_CM0: Compare 0 Interrupt.
* Output         : None
* Return         : None
*******************************************************************************/
void MC_ClearITPendingBit(u16 MC_IT)
{
/* Clear the corresponding interrupt pending bit */ 
  MC->IPR &= ~MC_IT;
}

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
