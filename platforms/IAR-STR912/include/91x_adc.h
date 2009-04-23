/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : 91x_adc.h
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file contains all the functions prototypes for the
*                      ADC software library.
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


/* Define to prevent recursive inclusion ------------------------------------ */

#ifndef __91x_ADC_H
#define __91x_ADC_H

/* Includes ------------------------------------------------------------------*/

#include "91x_map.h"

/* Exported types ------------------------------------------------------------*/

/* ADC Init structure define */
typedef struct
{
  u16 ADC_WDG_High_Threshold;
  u16 ADC_WDG_Low_Threshold;
  u16 ADC_Channel_0_Mode;
  u16 ADC_Channel_1_Mode;
  u16 ADC_Channel_2_Mode;
  u16 ADC_Channel_3_Mode;
  u16 ADC_Channel_4_Mode;
  u16 ADC_Channel_5_Mode;
  u16 ADC_Channel_6_Mode;
  u16 ADC_Channel_7_Mode;
  u16 ADC_Select_Channel;
  FunctionalState ADC_Scan_Mode;
  u16 ADC_Conversion_Mode;
}ADC_InitTypeDef;

/* ADC watchdog thresholds */
typedef enum
{
  ADC_HighThreshold,
  ADC_LowThreshold
}ADC_ThresholdType;

/* Exported constants --------------------------------------------------------*/

/* ADC channels */
#define   ADC_Channel_0     0x00      /* Channel 0 */
#define   ADC_Channel_1     0x01      /* Channel 1 */
#define   ADC_Channel_2     0x02      /* Channel 2 */
#define   ADC_Channel_3     0x03      /* Channel 3 */
#define   ADC_Channel_4     0x04      /* Channel 4 */
#define   ADC_Channel_5     0x05      /* Channel 5 */
#define   ADC_Channel_6     0x06      /* Channel 6 */
#define   ADC_Channel_7     0x07      /* Channel 7 */

/* ADC modes */
#define   ADC_Continuous_Mode    0x0010 /* Continuous mode conversion */
#define   ADC_Single_Mode        0xFFEF /* Single mode conversion     */

/* ADC Flags */
#define   ADC_FLAG_ECV         0x002F      /* End of Conversion Flag      */
#define   ADC_FLAG_AWD         0x002E      /* Analog WatchDog Flag        */

#define   ADC_FLAG_OV_CH_0     0x00CF      /* Overflow Flag for channel 0 */
#define   ADC_FLAG_OV_CH_1     0x00EF      /* Overflow Flag for channel 1 */
#define   ADC_FLAG_OV_CH_2     0x010F      /* Overflow Flag for channel 2 */
#define   ADC_FLAG_OV_CH_3     0x012F      /* Overflow Flag for channel 3 */
#define   ADC_FLAG_OV_CH_4     0x014F      /* Overflow Flag for channel 4 */
#define   ADC_FLAG_OV_CH_5     0x016F      /* Overflow Flag for channel 5 */
#define   ADC_FLAG_OV_CH_6     0x018F      /* Overflow Flag for channel 6 */
#define   ADC_FLAG_OV_CH_7     0x01AF      /* Overflow Flag for channel 7 */

/* ADC conversion modes */
#define ADC_NoThreshold_Conversion      0x0003   /* Conversion w/o thresholds */
#define ADC_LowThreshold_Conversion     0x0002   /* Low threshold conversion  */
#define ADC_HighThreshold_Conversion    0x0001   /* High threshold conversion */
#define ADC_No_Conversion               0x0000   /* No ADC conversion         */

/* ADC interrupts */
#define   ADC_IT_ECV     0x0400     /* End of Conversion Interrupt */
#define   ADC_IT_AWD     0x0200     /* Analog Watchdog Interrupt   */

/* ADC Start/Stop conversion */
#define     ADC_Conversion_Start     0x0001     /* ADC start conversion */
#define     ADC_Conversion_Stop      0xFFFE     /* ADC stop conversion  */

/* Module private variables --------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void ADC_DeInit(void);
void ADC_Init(ADC_InitTypeDef* ADC_InitStruct);
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct);
void ADC_PrescalerConfig(u8 ADC_Prescaler);
u8 ADC_GetPrescalerValue(void);
FlagStatus ADC_GetFlagStatus(u16 ADC_Flag);
void ADC_ClearFlag(u16 ADC_Flag);
u16 ADC_GetConversionValue(u16 ADC_Channel);
FlagStatus ADC_GetAnalogWatchdogResult(u16 ADC_Channel);
void ADC_ClearAnalogWatchdogResult(u16 ADC_Channel);
u16 ADC_GetWatchdogThreshold(ADC_ThresholdType ADC_Threshold);
void ADC_ITConfig(u16 ADC_IT, FunctionalState ADC_NewState);
void ADC_StandbyModeCmd(FunctionalState ADC_NewState);
void ADC_Cmd(FunctionalState ADC_NewState);
void ADC_ConversionCmd(u16 ADC_Conversion);

#endif /* __91x_ADC_H */

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/

