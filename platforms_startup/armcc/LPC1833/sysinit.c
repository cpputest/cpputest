#include "cmsis_18xx.h"

typedef struct {						/*!< CREG Structure         */
	__I  uint32_t  RESERVED0;
	__IO uint32_t  CREG0;				/*!< Chip configuration register 32 kHz oscillator output and BOD control register. */
	__I  uint32_t  RESERVED1[62];
	__IO uint32_t  MXMEMMAP;			/*!< ARM Cortex-M3/M4 memory mapping */
	__I  uint32_t  RESERVED2[5];
	__IO uint32_t  CREG5;				/*!< Chip configuration register 5. Controls JTAG access. */
	__IO uint32_t  DMAMUX;				/*!< DMA muxing control     */
	__IO uint32_t  FLASHCFGA;			/*!< Flash accelerator configuration register for flash bank A */
	__IO uint32_t  FLASHCFGB;			/*!< Flash accelerator configuration register for flash bank B */
	__IO uint32_t  ETBCFG;				/*!< ETB RAM configuration  */
	__IO uint32_t  CREG6;				/*!< Chip configuration register 6. */
	__I  uint32_t  RESERVED4[52];
	__I  uint32_t  CHIPID;				/*!< Part ID                */
	__I  uint32_t  RESERVED5[191];
	__IO uint32_t  USB0FLADJ;			/*!< USB0 frame length adjust register */
	__I  uint32_t  RESERVED9[63];
	__IO uint32_t  USB1FLADJ;			/*!< USB1 frame length adjust register */
} LPC_CREG_T;

#define LPC_CREG_BASE             0x40043000
#define LPC_CREG                  ((LPC_CREG_T             *) LPC_CREG_BASE)

static inline void Chip_CREG_SetFlashAcceleration(uint32_t Hz)
{
	uint32_t FAValue = Hz / 21510000;

	LPC_CREG->FLASHCFGA = (LPC_CREG->FLASHCFGA & (~(0xF << 12))) | (FAValue << 12);
	LPC_CREG->FLASHCFGB = (LPC_CREG->FLASHCFGB & (~(0xF << 12))) | (FAValue << 12);
}


/* Set up and initialize hardware prior to call to main */
void SystemInit(void)
{
    unsigned int *pSCB_VTOR = (unsigned int *) 0xE000ED08;

    extern void *__Vectors;
    *pSCB_VTOR = (unsigned int) &__Vectors;

    Chip_CREG_SetFlashAcceleration(CGU_IRC_FREQ);
}

