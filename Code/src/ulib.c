#include "ulib.h"

void SPI4_Init (void) {

/*
  * голубой     SDA     PE6
  * оранжевый	SCL		PE2
  * зеленый		RS		PC8
  * фиолетовый	RESET	PC11
  * черный		GND  *  2
  * коричневый	CS		PC9
  *
  * 1			LEDK
  * 2			LEDA
  * 3			VDD
  *
*/
//--------------------------------------------------------------
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOEEN;

	//MOSI
	GPIOE -> MODER |= GPIO_MODER_MODER6_1;
	GPIOE -> OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR6;
	GPIOE -> AFR[0]	|= (0x05 << (6 * 4));
	//SCL
	GPIOE -> MODER |= GPIO_MODER_MODER2_1;
	GPIOE -> OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR2;
	GPIOE -> AFR[0]	|= (0x05 << (2 * 4));

	//RS (COMMAND SELECT)
	GPIOC -> MODER |= GPIO_MODER_MODER8_0;
	GPIOC -> OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR8;
	GPIOC -> BSRR |= GPIO_BSRR_BS_8;
	//RESET
	GPIOC -> MODER |= GPIO_MODER_MODER11_0;
	GPIOC -> OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR11;
	GPIOC -> BSRR |= GPIO_BSRR_BS_11;
	//CS
	GPIOC -> MODER |= GPIO_MODER_MODER9_0;
	GPIOC -> OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR9;
	GPIOC -> BSRR |= GPIO_BSRR_BR_9;
//--------------------------------------------------------------
	RCC -> APB2ENR |= RCC_APB2ENR_SPI4EN;

	SPI4->CR1     = 0x0000;
	SPI4->CR2     = 0x0000;
	SPI4 -> CR1 |= SPI_CR1_MSTR;
	//SPI4 -> CR1 |= SPI_CR1_BR_0; // div 2
	SPI4 -> CR1 |= SPI_CR1_BIDIMODE;
	SPI4 -> CR1 |= SPI_CR1_BIDIOE;
	SPI4 -> CR2 &= ~SPI_CR2_DS_3;
	SPI4 -> CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2; // datasise 8bit
	SPI4 -> CR1 |= SPI_CR1_SSM;
	SPI4 -> CR1 |= SPI_CR1_SSI;
	SPI4 -> CR2 |= SPI_CR2_TXDMAEN;  //DMA enable transmitt
	SPI4 -> CR1 |= SPI_CR1_SPE;

	delay(500);
}

void DMA_init (void) {

	RCC -> AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	DMA2_Stream1 -> CR |= DMA_SxCR_CHSEL_2;		//Chanel 4

	DMAStop;		//DMA Disable

	DMA2_Stream1 -> CR &= ~DMA_SxCR_MINC;		//Addr increment reset
	DMA2_Stream1 -> CR &= ~DMA_SxCR_PINC;		//Peripheral incremental mode disable
	DMA2_Stream1 -> CR |= DMA_SxCR_MSIZE_0;		//Memory size		16b
	DMA2_Stream1 -> CR |= DMA_SxCR_PSIZE_0;		//peripheral size	16b
	DMA2_Stream1 -> CR &= ~DMA_SxCR_CIRC;		//Circle off
	DMA2_Stream1 -> CR |= DMA_SxCR_DIR_0;		//Memory to peripheral
	DMA2_Stream1 -> CR |= DMA_SxCR_PL;			//Very High priority
	DMA2_Stream1 -> CR |= DMA_SxCR_TCIE;		//Transfer complete interrupt enable

	DMA2_Stream1 -> PAR = (u32)&SPI4 -> DR;//(u32)&(*(__IO uint8_t*)&(SPI4->DR));//

}

void DMA_send16b (u16 *Data, u32 size) {

	if (FlagFuck != 0) {

		FlagFuck = (u32)Data;

	}

	else {

		FlagFuck = (u32)&Data[65535];

	}

	u32 BuffSize;

	size > 65535 ? (BuffSize = 65535) : (BuffSize = size);

	DMAStop;
	DMA2_Stream1 -> PAR = (u32)&SPI4 -> DR;
	DMA2_Stream1 -> M0AR = (u32)Data;
	DMA2_Stream1 -> NDTR = BuffSize;
	DMAStart;

	while (!(DMA2 -> LISR & DMA_LISR_TCIF1));
	DMA2->LIFCR |= DMA_LIFCR_CTCIF1;

	if (size > 65534) {

		DMAStop;
		DMA2_Stream1 -> NDTR = size - BuffSize;
		DMA2_Stream1 -> M0AR = FlagFuck;
		DMAStart;

		while (!(DMA2 -> LISR & DMA_LISR_TCIF1));
		DMA2->LIFCR |= DMA_LIFCR_CTCIF1;

	}
}

void DMA_send8b (u16 *Data, u32 size) {

	if (FlagFuck != 0) {

		FlagFuck = (u32)Data;

	}

	else {

		FlagFuck = (u32)&Data[65535];

	}

	u32 BuffSize;

	size > 65535 ? (BuffSize = 65535) : (BuffSize = size);

	DMAStop;
	DMA2_Stream1 -> PAR = (u32)&SPI4_DR_8bit;
	DMA2_Stream1 -> M0AR = (u32)Data;
	DMA2_Stream1 -> NDTR = BuffSize;
	DMAStart;

	while (!(DMA2 -> LISR & DMA_LISR_TCIF1));
	DMA2->LIFCR |= DMA_LIFCR_CTCIF1;

	if (size > 65535) {

		DMAStop;
		DMA2_Stream1 -> NDTR = size - BuffSize;
		DMA2_Stream1 -> M0AR = FlagFuck;
		DMAStart;

		while (!(DMA2 -> LISR & DMA_LISR_TCIF1));
		DMA2->LIFCR |= DMA_LIFCR_CTCIF1;

	}

}

void SPI4_write_byte (u16 data)
{
	SPI4_DR_8bit = data;

	while(!(SPI4 -> SR & SPI_SR_TXE));
	delay(2);//BAG

}

void delay (u32 delay) {

	while(delay) {

		delay--;
	}
}





