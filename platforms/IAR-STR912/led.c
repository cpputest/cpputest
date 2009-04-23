

#include "91x_gpio.h"
#define RED_LEDS GPIO3
#define WHITE_LEDS GPIO6


void Led_TurnOn(int led)
{
      GPIO_WriteBit(WHITE_LEDS, 1 << (led-1), Bit_RESET); 
}

void Led_TurnOff(int led)
{
      GPIO_WriteBit(WHITE_LEDS, 1 << (led-1), Bit_SET); 
}

#include "91x_lib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
  GPIO_InitTypeDef GPIO_InitStructure;
/* Private function prototypes -----------------------------------------------*/
  void LED_Configuration(void);
  static void Delay(u32 nCount);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void initLeds()
{
  #ifdef DEBUG
    debug();
  #endif
    
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);    /*Use OSC as the default clock source*/
  SCU_PCLKDivisorConfig(SCU_PCLK_Div1); /* ARM Peripheral bus clokdivisor = 1*/
  
  /* Configure the system clocks */
  LED_Configuration();

/* GPIO Configuration --------------------------------------------------------*/
  GPIO_DeInit(WHITE_LEDS);
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //0xf3
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull ;
  GPIO_InitStructure.GPIO_Alternate=GPIO_OutputAlt1;
  GPIO_Init (WHITE_LEDS, &GPIO_InitStructure);

}

void flashLeds(int repeat)
{
  for (int i = 0; i < repeat; i++)
  {
    /* Turn OFF leds */
    GPIO_Write(WHITE_LEDS,0x00);   
    /* Insert delay */
    Delay(0x7FFFF);

    /* Turn ON leds */
    GPIO_Write(WHITE_LEDS,0xFF);  
    /* Insert delay */
    Delay(0x7FFFF);
  }
}

/*******************************************************************************
* Function Name  : LED_Configuration
* Description    : Configures the system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LED_Configuration(void)
{
  /* Enable the __GPIO3 */
  SCU_APBPeriphClockConfig(__GPIO3 ,ENABLE);

}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
static void Delay(u32 nCount)
{
  u32 j = 0;

  for(j = nCount; j != 0; j--);
}
/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/

