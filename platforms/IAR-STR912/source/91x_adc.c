/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : 91x_adc.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file provides all the ADC software functions.
********************************************************************************
* History:
* 05/22/2007 : Version 1.2
* 05/24/2006 : Version 1.1
* 05/18/2006 : Version 1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
* A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


/* Standard include ----------------------------------------------------------*/
#include "91x_adc.h"
#include "91x_scu.h"
/* Include of other module interface headers ---------------------------------*/
/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* ADC mask */
#define   ADC_FLAG_MASK           0x001F     /* ADC Flag Mask           */
#define   ADC_RESULT_MASK         0x03FF     /* ADC Result Mask         */
#define   ADC_SCAN_MODE_MASK      0x0020     /* ADC Sacn Mode Mask      */
#define   ADC_STANDBY_MODE_MASK   0x0008     /* ADC Standby Mode Mask   */
#define   ADC_CMD_MASK            0x0002     /* ADC Command Mask        */
#define   ADC_CHANNEL_MASK        0xFE3F     /* ADC Channel Select Mask */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Interface functions -------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : ADC_DeInit
* Description    : Deinitialize the ADC module registers to their default reset
*                  values
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_DeInit(void)
{
  /* Reset the ADC registers values */
  SCU_APBPeriphReset(__ADC,ENABLE);
  SCU_APBPeriphReset(__ADC,DISABLE);
}

/*******************************************************************************
* Function Name  : ADC_Init
* Description    : Initializes ADC  peripheral according to the specified
*                  parameters in the ADC_InitTypeDef structure.
* Input          : ADC_InitStruct: pointer to a ADC_InitTypeDef structure that
*                  contains the configuration information for the specified
*                  ADC peripheral.
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_Init(ADC_InitTypeDef* ADC_InitStruct)
{
  /* Set the low threshold of the watchdog */
  ADC->LTR = ADC_InitStruct->ADC_WDG_Low_Threshold;

  /* Set the high threshold of the watchdog */
  ADC->HTR = ADC_InitStruct->ADC_WDG_High_Threshold;

    
  /* Channel 0 conversion mode */
  ADC->CCR &= 0xFFFC; 
  ADC->CCR |= ADC_InitStruct->ADC_Channel_0_Mode;
  
  /* Channel 1 conversion mode */
  ADC->CCR &= 0xFFF3; 
  ADC->CCR |= ADC_InitStruct->ADC_Channel_1_Mode << 0x2;

  /* Channel 2 conversion mode */
  ADC->CCR &= 0xFFCF;
  ADC->CCR |= ADC_InitStruct->ADC_Channel_2_Mode << 0x4;
 
  /* Channel 3 conversion mode */
  ADC->CCR &= 0xFF3F; 
  ADC->CCR |= ADC_InitStruct->ADC_Channel_3_Mode << 0x6;

  /* Channel 4 conversion mode */
  ADC->CCR &= 0xFCFF; 
  ADC->CCR |= ADC_InitStruct->ADC_Channel_4_Mode << 0x8;

  /* Channel 5 conversion mode */
  ADC->CCR &= 0xF3FF; 
  ADC->CCR |= ADC_InitStruct->ADC_Channel_5_Mode << 0xA;

  /* Channel 6 conversion mode */
  ADC->CCR &= 0xCFFF; 
  ADC->CCR |= ADC_InitStruct->ADC_Channel_6_Mode << 0xC;

  /* Channel 7 conversion mode */
  ADC->CCR &= 0x3FFF; 
  ADC->CCR |= ADC_InitStruct->ADC_Channel_7_Mode << 0xE;

  /* Select the channel to be converted */
  ADC->CR &= ADC_CHANNEL_MASK;
  ADC->CR |= ADC_InitStruct->ADC_Select_Channel << 0x6;

  /* Enable/disable the scan mode */
  if (ADC_InitStruct->ADC_Scan_Mode == ENABLE)
  {
    /* Enable the scan mode */
    ADC->CR |= ADC_SCAN_MODE_MASK;
  }
  else
  {
    /* Disable the scan mode */
    ADC->CR &= ~ADC_SCAN_MODE_MASK;
  }

  /* Configure the conversion mode */
  if (ADC_InitStruct->ADC_Conversion_Mode == ADC_Continuous_Mode)
  {
    /* ADC continuous mode */
    ADC->CR |= ADC_Continuous_Mode;
  }
  else
  {
    /* ADC single mode */
    ADC->CR &= ADC_Single_Mode;
  }
}

/*******************************************************************************
* Function Name  : ADC_StructInit
* Description    : Fills each ADC_InitStruct member with its reset value.
* Input          : ADC_InitStruct : pointer to a ADC_InitTypeDef structure
*                   which will be initialized.
* Output         : None
* Return         : None.
*******************************************************************************/
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
{
  ADC_InitStruct->ADC_WDG_High_Threshold = 0x0000;
  ADC_InitStruct->ADC_WDG_Low_Threshold  = 0x0000;
  ADC_InitStruct->ADC_Channel_0_Mode     = ADC_No_Conversion;
  ADC_InitStruct->ADC_Channel_1_Mode     = ADC_No_Conversion;
  ADC_InitStruct->ADC_Channel_2_Mode     = ADC_No_Conversion;
  ADC_InitStruct->ADC_Channel_3_Mode     = ADC_No_Conversion;
  ADC_InitStruct->ADC_Channel_4_Mode     = ADC_No_Conversion;
  ADC_InitStruct->ADC_Channel_5_Mode     = ADC_No_Conversion;
  ADC_InitStruct->ADC_Channel_6_Mode     = ADC_No_Conversion;
  ADC_InitStruct->ADC_Channel_7_Mode     = ADC_No_Conversion;
  ADC_InitStruct->ADC_Select_Channel     = ADC_Channel_0;
  ADC_InitStruct->ADC_Scan_Mode          = DISABLE;
  ADC_InitStruct->ADC_Conversion_Mode    = ADC_Single_Mode;
}

/*******************************************************************************
* Function Name  : ADC_PrescalerConfig
* Description    : This routine is used to configure the ADC prescaler value.
* Input          : ADC_Prescaler: specifies the prescaler value. This parameter
*                  can be a value from 0x0 to 0xFF.
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_PrescalerConfig(u8 ADC_Prescaler)
{
  ADC->PRS &= 0xFF00;
  ADC->PRS |= ADC_Prescaler;

}
/*******************************************************************************
* Function Name  : ADC_GetPrescalerValue
* Description    : This routine is used to get the ADC prescaler value.
* Input          : None
* Output         : None
* Return         : The prescaler value.
*******************************************************************************/
u8 ADC_GetPrescalerValue(void)
{
  return ADC->PRS & 0x00FF;
}
/*******************************************************************************
* Function Name  : ADC_GetFlagStatus
* Description    : Checks whether the specified ADC flag is set or not.
* Input          : ADC_Flag: flag to check.
*                  This parameter can be one of the following values:
*                     - ADC_FLAG_OV_CH_0: Conversion overflow status for 
*                                         channel 0.
*                     - ADC_FLAG_OV_CH_1: Conversion overflow status for
*                                         channel 1.
*                     - ADC_FLAG_OV_CH_2: Conversion overflow status for
*                                         channel 2.
*                     - ADC_FLAG_OV_CH_3: Conversion overflow status for
*                                         channel 3.
*                     - ADC_FLAG_OV_CH_4: Conversion overflow status for
*                                         channel 4.
*                     - ADC_FLAG_OV_CH_5: Conversion overflow status for
*                                         channel 5.
*                     - ADC_FLAG_OV_CH_6: Conversion overflow status for
*                                         channel 6.
*                     - ADC_FLAG_OV_CH_7: Conversion overflow status for
*                                         channel 7.
*                     - ADC_FLAG_ECV:     End of conversion status.
*                     - ADC_FLAG_AWD:     Analog watchdog status.
* Output         : None
* Return         : The NewState of the ADC_Flag (SET or RESET).
*******************************************************************************/
FlagStatus ADC_GetFlagStatus(u16 ADC_Flag)
{
  u8 AdcReg = 0, FlagPos = 0;
  
  /* Get the ADC register index */
  AdcReg = ADC_Flag >> 5;

  /* Get the flag position */
  FlagPos = ADC_Flag & ADC_FLAG_MASK;

  if(AdcReg == 1) /* The flag to check is in CR register */
  {
    if((ADC->CR & (1<<FlagPos))!= RESET)
    {
      return SET;
    }
    else
    {
      return RESET;
    }
  }
  else if(AdcReg == 6) /* The flag to check is in DR0 register */
  {
    if((ADC->DR0 & (1<<FlagPos))!= RESET)
    {
      return SET;
    }
    else
    {
      return RESET;
    }
  }
  else if(AdcReg == 7) /* The flag to check is in DR1 register */
  {
    if((ADC->DR1 & (1<<FlagPos))!= RESET)
    {
      return SET;
    }
    else
    {
      return RESET;
    }
  }
  else if(AdcReg == 8) /* The flag to check is in DR2 register */
  {
    if((ADC->DR2 & (1<<FlagPos))!= RESET)
    {
      return SET;
    }
    else
    {
      return RESET;
    }
  }
  else if(AdcReg == 9) /* The flag to check is in DR3 register */
  {
    if((ADC->DR3 & (1<<FlagPos))!= RESET)
    {
      return SET;
    }
    else
    {
      return RESET;
    }
  }

  else if(AdcReg == 0xA) /* The flag to check is in DR4 register */
  {
    if((ADC->DR4 & (1<<FlagPos))!= RESET)
    {
      return SET;
    }
    else
    {
      return RESET;
    }
  }
  else if(AdcReg == 0xB) /* The flag to check is in DR5 register */
  {
    if((ADC->DR5 & (1<<FlagPos))!= RESET)
    {
      return SET;
    }
    else
    {
      return RESET;
    }
  }
  else if(AdcReg == 0xC) /* The flag to check is in DR6 register */
  {
    if((ADC->DR6 & (1<<FlagPos))!= RESET)
    {
      return SET;
    }
    else
    {
      return RESET;
    }
  }
  else /* (AdcReg == 0xD), The flag to check is in DR7 register */
  {
    if((ADC->DR7 & (1<<FlagPos))!= RESET)
    {
      return SET;
    }
    else
    {
      return RESET;
    }
  }
}

/*******************************************************************************
* Function Name  : ADC_ClearFlag
* Description    : Clears the ADC Flag passed as a parameter.
* Input          : ADC_Flag: flag to clear.
*                  This parameter can be one of the following values:
*                     - ADC_FLAG_ECV: End of conversion status.
*                     - ADC_FLAG_AWD: Analog watchdog status.
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_ClearFlag(u16 ADC_Flag)
{  
  /* Clear the correspondent flag */
  ADC->CR |= (1<<(ADC_Flag & ADC_FLAG_MASK));
}

/*******************************************************************************
* Function Name  : ADC_GetConversionValue
* Description    : Read the result of conversion from the appropriate data 
*                  register.
* Input          : ADC_Channel: the correspondent channel of the ADC peripheral.
*                  This parameter can be one of the following values:
*                     - ADC_Channel_0: ADC channel 0.
*                     - ADC_Channel_1: ADC channel 1.
*                     - ADC_Channel_2: ADC channel 2.
*                     - ADC_Channel_3: ADC channel 3.
*                     - ADC_Channel_4: ADC channel 4.
*                     - ADC_Channel_5: ADC channel 5.
*                     - ADC_Channel_6: ADC channel 6.
*                     - ADC_Channel_7: ADC channel 7.
* Output         : None
* Return         : The result of the conversion for the specific channel.
*******************************************************************************/
u16 ADC_GetConversionValue(u16 ADC_Channel)
{
  u16 ADC_Conversion_Value = 0;

  switch (ADC_Channel)
  {
    case (ADC_Channel_0):
      /* Get the conversion value of the channel 0 */
      ADC_Conversion_Value = ADC->DR0 & ADC_RESULT_MASK;
      break;

    case (ADC_Channel_1):
      /* Get the conversion value of the channel 1 */
      ADC_Conversion_Value = ADC->DR1 & ADC_RESULT_MASK;
      break;

    case (ADC_Channel_2):
      /* Get the conversion value of the channel 2 */
      ADC_Conversion_Value = ADC->DR2 & ADC_RESULT_MASK;
      break;

    case (ADC_Channel_3):
      /* Get the conversion value of the channel 3 */
      ADC_Conversion_Value = ADC->DR3 & ADC_RESULT_MASK;
      break;

    case (ADC_Channel_4):
      /* Get the conversion value of the channel 4 */
      ADC_Conversion_Value = ADC->DR4 & ADC_RESULT_MASK;
      break;

    case (ADC_Channel_5):
      /* Get the conversion value of the channel 5 */
      ADC_Conversion_Value = ADC->DR5 & ADC_RESULT_MASK;
      break;
 
    case (ADC_Channel_6):
      /* Get the conversion value of the channel 6 */
      ADC_Conversion_Value = ADC->DR6 & ADC_RESULT_MASK;
      break;

    case (ADC_Channel_7):
      /* Get the conversion value of the channel 7 */
      ADC_Conversion_Value = ADC->DR7 & ADC_RESULT_MASK;
      break;

    default:
      break;
  }

  return(ADC_Conversion_Value);
}

/*******************************************************************************
* Function Name  : ADC_GetAnalogWatchdogResult
* Description    : Return the result of the comparaison on the selected Analog 
*                  Watchdog.
* Input          : ADC_Channel: the correspondent channel of the ADC peripheral.
*                  This parameter can be one of the following values:
*                     - ADC_Channel_0: ADC channel 0.
*                     - ADC_Channel_1: ADC channel 1.
*                     - ADC_Channel_2: ADC channel 2.
*                     - ADC_Channel_3: ADC channel 3.
*                     - ADC_Channel_4: ADC channel 4.
*                     - ADC_Channel_5: ADC channel 5.
*                     - ADC_Channel_6: ADC channel 6.
*                     - ADC_Channel_7: ADC channel 7.
* Output         : None
* Return         : The state of the comparision (SET or RESET).
*******************************************************************************/
FlagStatus ADC_GetAnalogWatchdogResult(u16 ADC_Channel)
{
  if (ADC->CRR & (1<<ADC_Channel) != RESET)
  {
      return SET;
  }
  else
  {
    return RESET;
  }
}

/*******************************************************************************
* Function Name  : ADC_ClearAnalogWatchdogResult
* Description    : Clear the result of the comparaison on the selected Analog 
*                  Watchdog.
* Input          : ADC_Channel: the correspondent channel of the ADC peripheral.
*                  This parameter can be one of the following values:
*                     - ADC_Channel_0: ADC channel 0.
*                     - ADC_Channel_1: ADC channel 1.
*                     - ADC_Channel_2: ADC channel 2.
*                     - ADC_Channel_3: ADC channel 3.
*                     - ADC_Channel_4: ADC channel 4.
*                     - ADC_Channel_5: ADC channel 5.
*                     - ADC_Channel_6: ADC channel 6.
*                     - ADC_Channel_7: ADC channel 7.
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_ClearAnalogWatchdogResult(u16 ADC_Channel)
{
  /* Clear the correspondent watchdog result */
  ADC->CRR = 1<<ADC_Channel;
}

/*******************************************************************************
* Function Name  : ADC_GetWatchdogThreshold
* Description    : Get the higher/lower thresholds values of the watchdog.
* Input          : ADC_Threshold: the lower or the higher threshold.
*                  This parameter can be one of the following values:
*                     - ADC_HigherThreshold: The higher threshold.
*                     - ADC_LowerThreshold: The lower threshold.
* Output         : None
* Return         : The selected threshold value.
*******************************************************************************/
u16 ADC_GetWatchdogThreshold(ADC_ThresholdType ADC_Threshold)
{
  u16 ADC_Threshold_Value = 0;

  switch (ADC_Threshold)
  {
    case ADC_LowThreshold:
      /* Get the low threshold of the watchdog */
      ADC_Threshold_Value = ADC->LTR;
      break;

    case ADC_HighThreshold:
      /* Get the high threshol of the watchdog */
      ADC_Threshold_Value = ADC->HTR;
      break;

    default:
      break;
  }

  return(ADC_Threshold_Value);
}

/*******************************************************************************
* Function Name  : ADC_ITConfig
* Description    : Enables or disables the specified ADC interrupts.
* Input          : - ADC_IT: specifies the ADC interrupts sources to be enabled 
*                    or disabled.
*                    This parameter can be one of the following values:
*                       - ADC_IT_EndOfConversion: End of conversion interrupt.
*                       - ADC_IT_AnalogWDG: Analog watchdog interrupt.
*                  - ADC_NewState: new state of the specified ADC interrupts.
*                    (ADC_Newstate can be ENABLE or DISABLE).
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_ITConfig(u16 ADC_IT, FunctionalState ADC_NewState)
{
  if (ADC_NewState == ENABLE)
  {
    /* Enable the interrupt */
    ADC->CR |= ADC_IT;
  }
  else
  {
    /* Disable the interrupt */
    ADC->CR &= ~ADC_IT;
  }
}

/*******************************************************************************
* Function Name  : ADC_StandbyModeCmd
* Description    : Enable or disable the standby mode.
* Input          : ADC_NewState: new state of the ADC standby mode.
*                  (ADC_Newstate can be ENABLE or DISABLE).
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_StandbyModeCmd(FunctionalState ADC_NewState)
{
  if (ADC_NewState == ENABLE)
  {
    /* Enable the standby mode */
    ADC->CR |= ADC_STANDBY_MODE_MASK;
  }
  else
  {
    /* Disable the standby mode */
    ADC->CR &= ~ADC_STANDBY_MODE_MASK;
  }
}

/*******************************************************************************
* Function Name  : ADC_Cmd
* Description    : Power on or put in reset mode the ADC peripheral.
* Input          : ADC_NewState: new state of the ADC peripheral.
*                  (ADC_Newstate can be ENABLE or DISABLE).
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_Cmd(FunctionalState ADC_NewState)
{
  if (ADC_NewState == ENABLE)
  {
    /* Enable the ADC */
    ADC->CR |= ADC_CMD_MASK;
  }
  else
  {
    /* Disable the ADC */
    ADC->CR &= ~ADC_CMD_MASK;
  }
}

/*******************************************************************************
* Function Name  : ADC_ConversionCmd
* Description    : Start or stop the ADC conversion in the selected mode.
* Input          : ADC_Conversion: the conversion command.
*                  This parameter can be one of the following values:
*                     - ADC_Conversion_Start: Start the conversion.
*                     - ADC_Conversion_Stop: Stop the Conversion.
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_ConversionCmd(u16 ADC_Conversion)
{
  if (ADC_Conversion == ADC_Conversion_Start)
  {
    /* Start the ADC conversion */
    ADC->CR |= ADC_Conversion_Start;
  }
  else
  {
    /* Stop the ADC conversion */
    ADC->CR &= ADC_Conversion_Stop;
  }
}

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
