#include "stm32f7xx.h"
#include "macro.h"


#define SPI4_DR_8bit         *(__IO uint8_t*)&(SPI4->DR)

#define DMAStart 	DMA2_Stream1 -> CR |= DMA_SxCR_EN
#define DMAStop 	DMA2_Stream1 -> CR &= ~DMA_SxCR_EN

#define CS_L() GPIOC -> BSRR |= GPIO_BSRR_BR_9
#define CS_H() GPIOC -> BSRR |= GPIO_BSRR_BS_9

// A0 pin macros
#define DC_L() GPIOC -> BSRR |= GPIO_BSRR_BR_8
#define DC_H() GPIOC -> BSRR |= GPIO_BSRR_BS_8

// RESET pin macros
#define RST_L() GPIOC -> BSRR |= GPIO_BSRR_BR_11
#define RST_H() GPIOC -> BSRR |= GPIO_BSRR_BS_11

u16 flagDMA;
u32 FlagFuck;

void SPI4_Init (void);
void DMA_init (void);
void DMA_send16b (u16 *Data, u32 size);
void DMA_send8b (u16 *Data, u32 size);
void SPI4_write_byte (u16 data);
void delay (u32 delay);
