/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : 91x_dma.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : This file provides all the DMA software functions
*                      needed to access all DMA registers.
********************************************************************************
* History:v 1.0
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

# include"91x_dma.h"
# include"91x_scu.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* DMA Masks "used" only in this module */

#define       DMA_Width_DES_MASK	            0xFF1FFFFF
#define       DMA_Width_SRC_MASK	            0xFFE3FFFF
#define       DMA_Bst_DES_MASK           	    0xFFFC7FFF
#define       DMA_Bst_SRC_MASK	                0xFFFF8FFF
#define       DMA_FlowCntrl_Mask	            0xFFFFC7FF
#define       DMA_TrsfSisze_Mask	            0xFFFFF000
#define       SRC_Mask	                        0xFFFFFFE1
#define       DES_Mask	                        0xFFFFFC3F
#define       DMA_TCIE	                        0x80000000
#define       DMA_ChannelDESInc	                0x08000000
#define       DMA_ChannelSRCInc	                0x04000000
#define       DMA_BufferChannel	                0x20000000
#define       DMA_HaltChannel	                0x00040000
#define       DMA_LockChannel	                0x00010000
#define       DMA_CacheChannel	                0x40000000
#define       DMA_ChannelActive	                0x00020000
#define       DMA_Enable                        0x00000001
#define       DMA_ChannelEnable                 0x00000001




/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : DMA_ITMaskConfig
* Description    : Enables or disables the specified DMA_Channelx Mask interrupt.
* Input          :
*                  -DMA_Channelx: where x can be 0,1,2,3,4,5,6,or 7 to select the DMA Channel.
*                  -DMA_ITMask: specifies the DMA interrupt mask source to be enabled or disabled.
*                                This parameter can be:
*                                    - DMA_ITMask_IE  (Interrupt error mask).
*                                    - DMA_ITMask_ITC (Terminal count interrupt mask).
*                                    - DMA_ITMask_ALL ( All interrupts mask)
*
*                  -NewState: new state of the specified DMA_Channelx mask interrupt.
*                             This parameter can be: ENABLE or DISABLE.
* Output         : None.
* Return         : None.
*******************************************************************************/


void DMA_ITMaskConfig(DMA_Channel_TypeDef * DMA_Channelx, u16 DMA_ITMask , FunctionalState NewState)

{
  if (NewState==ENABLE) /* Mask the Interrupt */
  {
    DMA_Channelx-> CCNF |= DMA_ITMask ;
  }

  else /* Disable the Interrupt Mask*/
  {
    DMA_Channelx-> CCNF &= ~ DMA_ITMask ;
  }
}





/*******************************************************************************
* Function Name  : DMA_ITConfig
* Description    : Enables or disables the DMA_Channelx Terminal Count interrupt.
* Input          :
*                 -DMA_Channelx: where x can be 0,1,2,3,4,5,6,or 7 to select the DMA Channel.
*                 -NewState: new state of the specified DMA_Channelx mask interrupt.
*                             This parameter can be: ENABLE or DISABLE.
* Output         : None.
* Return         : None.
*******************************************************************************/


void DMA_ITConfig(DMA_Channel_TypeDef * DMA_Channelx, FunctionalState NewState)

{
  if (NewState==ENABLE) /* Enable the Terminal Count Interrupt */
  {
    DMA_Channelx->CC |= DMA_TCIE ;
  }

  else /* Disable the Terminal Count Interrupt  */
  {
    DMA_Channelx-> CC &= ~ DMA_TCIE ;
  }
}


/********************************************************************************
* Function Name  : DMA_SyncConfig
* Description    : Enables or disables synchronization logic for the corresponding DMA Request Signal.
* Input          :
*                 -SRCReq:specifies the DMA Request Source.
*                  This parameter can be:
*                     -DMA_USB_RX_Mask	           
*                     -DMA_USB_TX_Mask	           
*                     -DMA_TIM0_Mask	               
*                     -DMA_TIM1_Mask	              
*                     -DMA_UART0_RX_Mask             
*                     -DMA_UART0_TX_Mask             
*                     -DMA_UART1_RX_Mask           
*                     -DMA_UART1_TX_Mask            
*                     -DMA_External_Req0_Mask        
*                     -DMA_External_Req1_Mask	    
*                     -DMA_I2C0_Mask	                
*                     -DMA_I2C1_Mask	               
*                     -DMA_SSP0_RX_Mask	            
*                     -DMA_SSP0_TX_Mask	            
*                     -DMA_SSP1_RX_Mask	            
*                     -DMA_SSP1_TX_Mask	            
*
*                  -NewState: new state of the specified DMA_Channelx mask interrupt.
*                             This parameter can be: ENABLE or DISABLE.
* Output         : None.
* Return         : None.
*******************************************************************************/


void DMA_SyncConfig(u16 SRCReq, FunctionalState NewState)

{
  if (NewState==ENABLE) /* Enable the synchronization logic for the corresponding DMA Request Signal */
  {

    DMA->SYNR &= ~ SRCReq ;

  }

  else /* Disable the synchronization logic for the corresponding DMA Request Signal.  */
  {
    DMA->SYNR |= SRCReq ;

  }
}


/********************************************************************************
* Function Name  : DMA_SetSReq
* Description    : Set the DMA to generate a Single transfer request for the corresponding DMA Request Source.
* Input          :
*                 -SRCReq:specifies the DMA Request Source.
*                  This parameter can be:
*                     -DMA_USB_RX_Mask	           
*                     -DMA_USB_TX_Mask	           
*                     -DMA_TIM0_Mask	               
*                     -DMA_TIM1_Mask	              
*                     -DMA_UART0_RX_Mask             
*                     -DMA_UART0_TX_Mask             
*                     -DMA_UART1_RX_Mask           
*                     -DMA_UART1_TX_Mask            
*                     -DMA_External_Req0_Mask        
*                     -DMA_External_Req1_Mask	    
*                     -DMA_I2C0_Mask	                
*                     -DMA_I2C1_Mask	               
*                     -DMA_SSP0_RX_Mask	            
*                     -DMA_SSP0_TX_Mask	            
*                     -DMA_SSP1_RX_Mask	            
*                     -DMA_SSP1_TX_Mask	            
*
* Output         : None.
* Return         : None.
*******************************************************************************/


void DMA_SetSReq(u16 SRCReq)

{ /* Set the DMA to generate a Single transfer request for the corresponding DMA Request Source */
  DMA->SSRR |= SRCReq ;
}




/********************************************************************************
* Function Name  : DMA_SetLSReq
* Description    : Set the DMA to generate a Last Single transfer request for the corresponding DMA Request Source.
* Input          :
*                 -SRCReq:specifies the DMA Request Source.
*                  This parameter can be:
*                     -DMA_USB_RX_Mask	           
*                     -DMA_USB_TX_Mask	           
*                     -DMA_TIM0_Mask	               
*                     -DMA_TIM1_Mask	              
*                     -DMA_UART0_RX_Mask             
*                     -DMA_UART0_TX_Mask             
*                     -DMA_UART1_RX_Mask           
*                     -DMA_UART1_TX_Mask            
*                     -DMA_External_Req0_Mask        
*                     -DMA_External_Req1_Mask	    
*                     -DMA_I2C0_Mask	                
*                     -DMA_I2C1_Mask	               
*                     -DMA_SSP0_RX_Mask	            
*                     -DMA_SSP0_TX_Mask	            
*                     -DMA_SSP1_RX_Mask	            
*                     -DMA_SSP1_TX_Mask	            
* Output         : None.
* Return         : None.
*******************************************************************************/

void DMA_SetLSReq(u16 SRCReq )
{ /* Set the DMA to generate a Last Single transfer request for the corresponding DMA Request Source */
  DMA->SLSRR |= SRCReq ;
}


/********************************************************************************
* Function Name  : DMA_SetBReq
* Description    : Set the DMA to generate a Burst transfer request for the corresponding DMA Request Source.
* Input          :
*                 -SRCReq:specifies the DMA Request Source.
*                  This parameter can be:
*                     -DMA_USB_RX_Mask	           
*                     -DMA_USB_TX_Mask	           
*                     -DMA_TIM0_Mask	               
*                     -DMA_TIM1_Mask	              
*                     -DMA_UART0_RX_Mask             
*                     -DMA_UART0_TX_Mask             
*                     -DMA_UART1_RX_Mask           
*                     -DMA_UART1_TX_Mask            
*                     -DMA_External_Req0_Mask        
*                     -DMA_External_Req1_Mask	    
*                     -DMA_I2C0_Mask	                
*                     -DMA_I2C1_Mask	               
*                     -DMA_SSP0_RX_Mask	            
*                     -DMA_SSP0_TX_Mask	            
*                     -DMA_SSP1_RX_Mask	            
*                     -DMA_SSP1_TX_Mask	            
* Output         : None.
* Return         : None.
*******************************************************************************/


void DMA_SetBReq(u16 SRCReq)

{ /* Set the DMA to generate a Burst transfer request for the corresponding DMA Request Source */
  DMA->SBRR |= SRCReq ;
}



/********************************************************************************
* Function Name  : DMA_SetLBReq
* Description    : Set the DMA to generate a Last Burst transfer request for the corresponding DMA Request Source.
* Input          :
*                 -SRCReq:specifies the DMA Request Source.
*                  This parameter can be:
*                     -DMA_USB_RX_Mask	           
*                     -DMA_USB_TX_Mask	           
*                     -DMA_TIM0_Mask	               
*                     -DMA_TIM1_Mask	              
*                     -DMA_UART0_RX_Mask             
*                     -DMA_UART0_TX_Mask             
*                     -DMA_UART1_RX_Mask           
*                     -DMA_UART1_TX_Mask            
*                     -DMA_External_Req0_Mask        
*                     -DMA_External_Req1_Mask	    
*                     -DMA_I2C0_Mask	                
*                     -DMA_I2C1_Mask	               
*                     -DMA_SSP0_RX_Mask	            
*                     -DMA_SSP0_TX_Mask	            
*                     -DMA_SSP1_RX_Mask	            
*                     -DMA_SSP1_TX_Mask	            
* Output         : None.
* Return         : None.
*******************************************************************************/


void DMA_SetLBReq(u16 SRCReq)

{ /* Set the DMA to generate a Last Burst transfer request for the corresponding DMA Request Source */
  DMA->SLBRR |= SRCReq ;
}


/********************************************************************************
* Function Name  : DMA_GetSReq
* Description    : Check for a specific source if it request a Single transfer .
* Input          :
* Input          :
*                 -SRCReq:specifies the DMA Request Source.
*                  This parameter can be:
*                     -DMA_USB_RX_Mask	           
*                     -DMA_USB_TX_Mask	           
*                     -DMA_TIM0_Mask	               
*                     -DMA_TIM1_Mask	              
*                     -DMA_UART0_RX_Mask             
*                     -DMA_UART0_TX_Mask             
*                     -DMA_UART1_RX_Mask           
*                     -DMA_UART1_TX_Mask            
*                     -DMA_External_Req0_Mask        
*                     -DMA_External_Req1_Mask	    
*                     -DMA_I2C0_Mask	                
*                     -DMA_I2C1_Mask	               
*                     -DMA_SSP0_RX_Mask	            
*                     -DMA_SSP0_TX_Mask	            
*                     -DMA_SSP1_RX_Mask	            
*                     -DMA_SSP1_TX_Mask	            
* Output         : None.
* Return         : SET or RESET.
*******************************************************************************/


FlagStatus DMA_GetSReq(u16 SRCReq)

{ /* Check for a specific source if it request a Single transfer . */
 if ( (DMA->SSRR & SRCReq )!= RESET )
 {
   return SET;
 }

 else
 {
  return RESET;
 }
}


/********************************************************************************
* Function Name  : DMA_GetLSReq
* Description    : Check for a specific source if it request a Last Single transfer .
* Input          :
*                 -SRCReq:specifies the DMA Request Source.
*                  This parameter can be:
*                     -DMA_USB_RX_Mask	           
*                     -DMA_USB_TX_Mask	           
*                     -DMA_TIM0_Mask	               
*                     -DMA_TIM1_Mask	              
*                     -DMA_UART0_RX_Mask             
*                     -DMA_UART0_TX_Mask             
*                     -DMA_UART1_RX_Mask           
*                     -DMA_UART1_TX_Mask            
*                     -DMA_External_Req0_Mask        
*                     -DMA_External_Req1_Mask	    
*                     -DMA_I2C0_Mask	                
*                     -DMA_I2C1_Mask	               
*                     -DMA_SSP0_RX_Mask	            
*                     -DMA_SSP0_TX_Mask	            
*                     -DMA_SSP1_RX_Mask	            
*                     -DMA_SSP1_TX_Mask	            
* Output         : None.
* Return         : SET or RESET.
*******************************************************************************/


FlagStatus DMA_GetLSReq(u16 SRCReq)

{ /* Check for a specific source if it request a Last Single transfer . */
 if ( (DMA->SLSRR & SRCReq)!= RESET  )
 {
   return SET;
 }

 else
 {
  return RESET;
 }
}

/********************************************************************************
* Function Name  : DMA_GetBReq
* Description    : Check for a specific source if it request a Burst transfer .
* Input          :
*                 -SRCReq:specifies the DMA Request Source.
*                  This parameter can be:
*                     -DMA_USB_RX_Mask	           
*                     -DMA_USB_TX_Mask	           
*                     -DMA_TIM0_Mask	               
*                     -DMA_TIM1_Mask	              
*                     -DMA_UART0_RX_Mask             
*                     -DMA_UART0_TX_Mask             
*                     -DMA_UART1_RX_Mask           
*                     -DMA_UART1_TX_Mask            
*                     -DMA_External_Req0_Mask        
*                     -DMA_External_Req1_Mask	    
*                     -DMA_I2C0_Mask	                
*                     -DMA_I2C1_Mask	               
*                     -DMA_SSP0_RX_Mask	            
*                     -DMA_SSP0_TX_Mask	            
*                     -DMA_SSP1_RX_Mask	            
*                     -DMA_SSP1_TX_Mask	            
* Output         : None.
* Return         : SET or RESET.
*******************************************************************************/


FlagStatus DMA_GetBReq(u16 SRCReq)

{ /* Check for a specific source if it request a Burst transfer . */
 if (( DMA->SBRR & SRCReq ) != RESET )
 {
   return SET;
 }

 else
 {
  return RESET;
 }
}

/********************************************************************************
* Function Name  : DMA_GetLSReq
* Description    : Check for a specific source if it request a Last Burst transfer .
* Input          :
* Input          :
*                 - SRCReq:specifies the DMA Request Source.
*                  This parameter can be:
*                     -DMA_USB_RX_Mask	           
*                     -DMA_USB_TX_Mask	           
*                     -DMA_TIM0_Mask	               
*                     -DMA_TIM1_Mask	              
*                     -DMA_UART0_RX_Mask             
*                     -DMA_UART0_TX_Mask             
*                     -DMA_UART1_RX_Mask           
*                     -DMA_UART1_TX_Mask            
*                     -DMA_External_Req0_Mask        
*                     -DMA_External_Req1_Mask	    
*                     -DMA_I2C0_Mask	                
*                     -DMA_I2C1_Mask	               
*                     -DMA_SSP0_RX_Mask	            
*                     -DMA_SSP0_TX_Mask	            
*                     -DMA_SSP1_RX_Mask	            
*                     -DMA_SSP1_TX_Mask	            
* Output         : None.
* Return         : SET or RESET.
*******************************************************************************/


FlagStatus DMA_GetLBReq(u16 SRCReq)

{ /* Check for a specific source if it request a Last Burst transfer . */
 if ( ( DMA->SLBRR & SRCReq ) != RESET )
 {
   return SET;
 }

 else
 {
  return RESET;
 }
}



/*******************************************************************************
* Function Name  : DMA_ChannelHalt
* Description    : Enables DMA requests or ignore extra source DMA requests for
                   the specified channel.
* Input          :
                  -DMA_Channelx: where x can be 0,1,2,3,4,5,6,or 7 to select the DMA Channel.
                  -NewState: new state of the specified DMA_Channelx mask interrupt.
                             This parameter can be: ENABLE or DISABLE.
* Output         : None.
* Return         : None.
*******************************************************************************/


void DMA_ChannelHalt(DMA_Channel_TypeDef * DMA_Channelx, FunctionalState NewState)

{
  if (NewState==ENABLE) /* Enables DMA requests */

  {
    DMA_Channelx->CCNF |= DMA_HaltChannel ;
  }

  else /* Ignore extra source DMA request */
  {
    DMA_Channelx->CCNF &= ~ DMA_HaltChannel ;
  }
}


/*******************************************************************************
* Function Name  : DMA_ChannelLockTrsf
* Description    : Enables or disables the Locked Transfers Feature for the specified DMA_Channelx
* Input          :
*                  -DMA_Channelx: where x can be 0,1,2,3,4,5,6,or 7 to select the DMA Channel.
*                  -NewState: new state of the specified DMA_Channelx mask interrupt.
*                             This parameter can be: ENABLE or DISABLE.
* Output         : None.
* Return         : None.
*******************************************************************************/


void DMA_ChannelLockTrsf(DMA_Channel_TypeDef * DMA_Channelx, FunctionalState NewState)

{
  if (NewState==ENABLE) /* Locked transfers enabled on channel x */

  {
    DMA_Channelx->CCNF |= DMA_LockChannel ;
  }

  else /* Locked transfers disabled on channel xt */
  {
    DMA_Channelx->CCNF &= ~ DMA_LockChannel;
  }
}


/*******************************************************************************
* Function Name  : DMA_ChannelCache
* Description    : Enables or disables the cacheability Feature for the specified DMA_Channelx
* Input          :
*                  -DMA_Channelx: where x can be 0,1,2,3,4,5,6,or 7 to select the DMA Channel.
*                  -NewState: new state of the specified DMA_Channelx mask interrupt.
*                             This parameter can be: ENABLE or DISABLE.
* Output         : None.
* Return         : None.
*******************************************************************************/


void DMA_ChannelCache (DMA_Channel_TypeDef * DMA_Channelx, FunctionalState NewState)

{
  if (NewState==ENABLE) /* Cacheability Feature enabled on channelx */

  {
    DMA_Channelx->CC |= DMA_CacheChannel ;
  }

  else /* Cacheability Feature disabled on channelx */
  {
    DMA_Channelx->CC &= ~ DMA_CacheChannel ;
  }
}


/*******************************************************************************
* Function Name  : DMA_ChannelBuffering
* Description    : Enables or disables the Buffering Feature for the specified DMA_Channelx
* Input          :
*                  -DMA_Channelx: where x can be 0,1,2,3,4,5,6,or 7 to select the DMA Channel.
*                  -NewState: new state of the specified DMA_Channelx mask interrupt.
*                             This parameter can be: ENABLE or DISABLE.
* Output         : None.
* Return         : None.
*******************************************************************************/


void DMA_ChannelBuffering (DMA_Channel_TypeDef * DMA_Channelx, FunctionalState NewState)

{
  if (NewState==ENABLE) /* Cacheability Feature enabled on channel x */

  {
    DMA_Channelx->CC |= DMA_BufferChannel ;
  }

  else /* Cacheability Feature disabled on channel xt */
  {
    DMA_Channelx->CC &= ~ DMA_BufferChannel ;
  }
}

/*******************************************************************************
* Function Name  : MA_ChannelProt0Mod
* Description    : Sets The User or Privileged mode for the specified DMA_Channelx
* Input          :
*                  -DMA_Channelx: where x can be 0,1,2,3,4,5,6,or 7 to select the DMA Channel.
*                  -Prot0Mode: Specifies the Privileged mode Or the User mode.
*                                This parameter can be:
*                      - DMA_PrevilegedMode
*                      - DMA_UserMode
*
*
* Output         : None.
* Return         : None.
*******************************************************************************/


void DMA_ChannelProt0Mode (DMA_Channel_TypeDef * DMA_Channelx, u32 Prot0Mode)

{
  if (Prot0Mode==DMA_PrevilegedMode) /* Privileged mode */
  {
    DMA_Channelx->CC |= DMA_PrevilegedMode ;
  }

  else /* User mode */
  {
    DMA_Channelx->CC &= DMA_UserMode ;
  }
}





/*******************************************************************************
* Function Name  : DMA_ChannelSRCIncConfig
* Description    : Enables or disables the Source address incrementation after each transfer for
*                   the specified DMA_Channelx
* Input          :
*                  -DMA_Channelx: where x can be 0,1,2,3,4,5,6,or 7 to select the DMA Channel.
*                  -NewState: new state of the specified DMA_Channelx mask interrupt.
*                             This parameter can be: ENABLE or DISABLE.
* Output         : None.
* Return         : None.
*******************************************************************************/


void DMA_ChannelSRCIncConfig (DMA_Channel_TypeDef * DMA_Channelx, FunctionalState NewState)

{
  if (NewState==ENABLE) /* The Source address is incremented after each transfer */

  {
    DMA_Channelx->CC |= DMA_ChannelSRCInc ;
  }

  else /* The Source address is not incremented after each Transfer */
  {
    DMA_Channelx->CC &= ~ DMA_ChannelSRCInc  ;
  }
}


/*******************************************************************************
* Function Name  : DMA_ChannelDESIncConfig
* Description    : Enables or disables the Destination address incrementation after each transfer for
*                   the specified DMA_Channelx
* Input          :
*                  -DMA_Channelx: where x can be 0,1,2,3,4,5,6,or 7 to select the DMA Channel.
*                  -NewState: new state of the specified DMA_Channelx mask interrupt.
*                             This parameter can be: ENABLE or DISABLE.
* Output         : None.
* Return         : None.
*******************************************************************************/


void DMA_ChannelDESIncConfig (DMA_Channel_TypeDef * DMA_Channelx, FunctionalState NewState)

{
  if (NewState==ENABLE) /* The Destination address is incremented after each transfer */

  {
    DMA_Channelx->CC |= DMA_ChannelDESInc ;
  }

  else /* The Destination address is not incremented after each Transfer */
  {
    DMA_Channelx->CC &= ~ DMA_ChannelDESInc  ;
  }
}



/********************************************************************************
* Function Name  : DMA_GetChannelStatus
* Description    : Checks the status of DMA channelx ( Enabled or Disabled).
*                  - ChannelIndx:specifies the DMA Channel to be checked.
*                   This parameter can be:
*				      - Channel0
*					  - Channel1
*					  - Channel2
*					  - Channel3
*					  - Channel4
*					  - Channel5
*				      - Channel6
*					  - Channel7
* Output         : None.
*
* Return         : SET or RESET.
*******************************************************************************/


FlagStatus  DMA_GetChannelStatus(u8 ChannelIndx )

{

 if ( ( DMA->ENCSR & (1 <<  ChannelIndx )) != RESET )
 {
   return SET; /*  Channelx Enabled */
 }

 else

 {

  return RESET; /*  Channelx Disabled */

 }

}



/********************************************************************************
* Function Name  : DMA_GetITStatus
* Description    : Checks the status of Terminal Count and  Error interrupts request after and before Masking.
* Input          :
*                  - ChannelIndx:specifies the DMA Channel to be checked.
*                   This parameter can be:
*				      - Channel0
*					  - Channel1
*					  - Channel2
*					  - Channel3
*					  - Channel4
*					  - Channel5
*				      - Channel6
*					  - Channel7
*
*.                 - DMA_ITReq: specifies the DMA interrupt request status to be checked.
*                   This parameter can be:
*
*					  - DMA_IS 
*					  - DMA_TCS 
*					  - DMA_ES
*					  - DMA_TCRS
*					  - DMA_ERS.
*
* Output         : None.
*
* Return         : SET or RESET.
*******************************************************************************/


ITStatus DMA_GetITStatus(u8 ChannelIndx,u8 DMA_ITReq)

{
  u32 DMAReg = 0;

  switch(DMA_ITReq)

 {

   case (DMA_IS):  /*The status of the interrupts after masking : logical or of all Interrupts after Masking*/
    DMAReg = DMA->ISR;	
    break;

  			

   case (DMA_TCS):     /* The status of the Terminal count request after masking */
    DMAReg = DMA->TCISR;
    break;


   case (DMA_ES):   /* The status of the error request after masking */
    DMAReg = DMA->EISR;
    break;


   case (DMA_TCRS): /* Indicates if the DMA channel is requesting a transfer complete (terminal count Interrupt) prior to masking or Not. */
    DMAReg = DMA->TCRISR;
    break;

   case (DMA_ERS):  /* Indicates if the DMA channel is requesting an Error Interrupt prior to masking or Not. */
     DMAReg = DMA->ERISR;
     break;


  }

  if((DMAReg &(1 <<  ChannelIndx )) != RESET )

  {
   return SET;
  }

  else

  {

  return RESET;
				

  }

}


/********************************************************************************
* Function Name  : DMA_ClearIT
* Description    : Clears The Interrupt pending bits for termnal count or Error interrupts for a specified DMA Channel.
*                  - ChannelIndx:specifies the DMA Channel to be checked.
*                   This parameter can be:
*				      - Channel0
*					  - Channel1
*					  - Channel2
*					  - Channel3
*					  - Channel4
*					  - Channel5
*				      - Channel6
*					  - Channel7
*				  - DMA_ITClr : Specifies the DMA interrupt pending to be cleared.
*.                    This parameter can be:
*                     - DMA_TCC
*					  - DMA_EC.
*
* Output         : None.

* Return         : SET or RESET.
*******************************************************************************/


void DMA_ClearIT(u8 ChannelIndx,u8 DMA_ITClr)

{


 switch(DMA_ITClr)

 {

  case (DMA_TCC): /* Clear The status of the Terminal count interrupt on the corresponding channel.*/
    DMA->TCICR |=(1 <<  ChannelIndx );
    break;         	



  case (DMA_EC): /* Clear The status of the error interrupt on the corresponding channel.*/
    DMA->EICR |=(1 <<  ChannelIndx );
    break;		

				

 }



}


/*******************************************************************************
* Function Name  : DMA_Cmd(FunctionalState NewState)
* Description    : Enables or disables the DMA peripheral.
*
* Input          :
*                  -NewState: new state of the DMA.
*                             This parameter can be: ENABLE or DISABLE.
* Output         : None.
* Return         : None.
*******************************************************************************/


void DMA_Cmd(FunctionalState NewState)

{
  if (NewState==ENABLE) /* ENABLE the DMA peripheral  */

  {
    DMA-> CNFR |= DMA_Enable ;
  }

  else /* DISABLE the DMA peripheral */
  {
    DMA-> CNFR &= ~ DMA_Enable ;
  }
}



/*******************************************************************************
* Function Name  : DMA_ChannelCmd
* Description    : Enables or disables the specified DMA_Channelx
*
* Input          :
*                  -DMA_Channelx: where x can be 0,1,2,3,4,5,6,or 7 to select the DMA Channel.
*                  -NewState: new state of the specified DMA_Channelx mask interrupt.
*                             This parameter can be: ENABLE or DISABLE.
* Output         : None.
* Return         : None.
*******************************************************************************/


void DMA_ChannelCmd (DMA_Channel_TypeDef *DMA_Channelx,FunctionalState NewState)

{
  if (NewState==ENABLE) /*  Enable The Channelx */

  {
    DMA_Channelx->CCNF |= DMA_ChannelEnable ;
  }

  else /* Disable The Channelx */
  {
    DMA_Channelx-> CCNF &= ~ DMA_ChannelEnable ;
  }
}



/********************************************************************************
* Function Name  : DMA_GetChannelActiveStatus
* Description    : Checks The DMA_Channelx FIFO if it has data or not.
* Input          :
*                  -DMA_Channelx: where x can be 0,1,2,3,4,5,6,or 7 to select the DMA Channel.
*
*
* Output         : None.
*
* Return         : SET or RESET.
*******************************************************************************/


FlagStatus  DMA_GetChannelActiveStatus( DMA_Channel_TypeDef * DMA_Channelx )

{

 if ( ( DMA_Channelx->CCNF & DMA_ChannelActive) != RESET )
 {
   return SET; /* The DMA_Channelx FIFO has data */
 }

 else

 {

  return RESET; /* No data in the DMA_Channelx FIFO */


 }

}


/********************************************************************************
* Function Name   : DMA_DeInit
* Description     : Initializes the DMA peripheral registers to their default reset values.
*
*
* Input           : None
*
* Output          : None.
*
* Called Functions:
*
*                   - SCU_AHBPeriphReset: Function defined in the System clock Unit "scu.c".
*
*
* Return          : None
*******************************************************************************/


void DMA_DeInit(void)

{
 
  SCU_AHBPeriphReset(__DMA, ENABLE);         /*DMA peripheral is under Reset " Reset on"*/
  SCU_AHBPeriphReset(__DMA, DISABLE);        /*DMA peripheral Reset off*/
}



/********************************************************************************
* Function Name  : DMA_StructInit
* Description    : Fills each DMA_InitStruct member with its reset value.
* Input          :
*                  -DMA_InitStruct: pointer to a DMA_InitTypeDef structure which will be initialized.
*
* Output         : None.
*
* Return         : None
*******************************************************************************/


void DMA_StructInit(DMA_InitTypeDef * DMA_InitStruct)

{

	 /* Initialize The current source address */
     DMA_InitStruct-> DMA_Channel_SrcAdd =0x0000000;


     /* Initialize The current Destination address */
     DMA_InitStruct->DMA_Channel_DesAdd=0x00000000;



	 /* Initialize The Linked List Items */
     DMA_InitStruct->DMA_Channel_LLstItm=0x00000000 ;



     /* Initialize The Destination width */
	 DMA_InitStruct->DMA_Channel_DesWidth= DMA_DesWidth_Byte;



     /* Initialize The source width */
     DMA_InitStruct->DMA_Channel_SrcWidth= DMA_SrcWidth_Byte;


     /* Initialize The Burst Size for the Destination */
     DMA_InitStruct->DMA_Channel_DesBstSize= DMA_DesBst_1Data; /* 1 Data "one Data can be byte, halfword or word depending on the Destination width  */



     /* Initialize The Burst Size for the Source*/
     DMA_InitStruct->DMA_Channel_SrcBstSize= DMA_SrcBst_1Data; /* 1 Data "one Data can be byte, halfword or word depending on the source width  */

    /* Initialize The Flow control and transfer type for the DMA transfer */
     DMA_InitStruct->DMA_Channel_FlowCntrl=DMA_FlowCntrlt0_DMA; /* memory to memory transfer with DMA as flow controller */


     /* Initialize The Transfer Size */
     DMA_InitStruct->DMA_Channel_TrsfSize =0x00;

	

     /* Initialize the DMA source request peripheral :"This field is ignored if the source of the transfer is from memory" */
     DMA_InitStruct->DMA_Channel_Src =0x00;



     /* Initialize the DMA Destination request peripheral :"This field is ignored if the destination of the transfer is to memory.*/

     DMA_InitStruct->DMA_Channel_Des=0x00;


}




/********************************************************************************
* Function Name  : DMA_Init
* Description    : Initializes the DMA_Channelx according to the specified parameters
*                    in the DMA_InitStruct .
*
* Input          :
*                  -DMA_Channelx: where x can be 0,1,2,3,4,5,6,or 7 to select the DMA Channel.
*				  -DMA_InitStruct: pointer to a DMA_InitTypeDef structure
*				  ( Structure Config to be load in DMA Registers). .
*
* Output         : None.
*
* Return         : None
*******************************************************************************/


void DMA_Init(DMA_Channel_TypeDef * DMA_Channelx, DMA_InitTypeDef * DMA_InitStruct)

{



   /* Select the DMA source peripheral request */
    DMA_Channelx->CCNF &= SRC_Mask;
    DMA_Channelx->CCNF |= DMA_InitStruct->DMA_Channel_Src;


 /* Select the flow controller and the transfer type */
    DMA_Channelx->CCNF &= DMA_FlowCntrl_Mask;
    DMA_Channelx->CCNF |=DMA_InitStruct->DMA_Channel_FlowCntrl;


    /* Select the DMA Destination peripheral request*/
    DMA_Channelx->CCNF &= DES_Mask;
    DMA_Channelx->CCNF |= DMA_InitStruct->DMA_Channel_Des;

    /* Set the source address */
    DMA_Channelx->SRC = DMA_InitStruct-> DMA_Channel_SrcAdd ;


	 /* Set the destination address */

    DMA_Channelx->DES = DMA_InitStruct->DMA_Channel_DesAdd ;



    /* Set the linked list Items address */
    DMA_Channelx->LLI = DMA_InitStruct->DMA_Channel_LLstItm ;


    /* Set The Destination width */
    DMA_Channelx->CC &= DMA_Width_DES_MASK;
    DMA_Channelx->CC |= DMA_InitStruct->DMA_Channel_DesWidth;


    /* Set The Source width  */
    DMA_Channelx->CC &= DMA_Width_SRC_MASK;
    DMA_Channelx->CC |= DMA_InitStruct->DMA_Channel_SrcWidth;

    /* Set The Burst Size for the Destination */
    DMA_Channelx->CC &= DMA_Bst_DES_MASK;
    DMA_Channelx->CC |= DMA_InitStruct->DMA_Channel_DesBstSize;

   /* Set The Burst Size for the Source */
   DMA_Channelx->CC &= DMA_Bst_SRC_MASK;
   DMA_Channelx->CC |=DMA_InitStruct->DMA_Channel_SrcBstSize;


    /* Initialize The Transfer Size for the Source */
    DMA_Channelx->CC &= DMA_TrsfSisze_Mask;
    DMA_Channelx->CC |= DMA_InitStruct->DMA_Channel_TrsfSize;


}


/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/
