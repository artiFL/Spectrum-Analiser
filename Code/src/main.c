#include "main.h"

/* SI4313-B1-FM (Silicon Laboratories )
 *
 * Frequency range = 240–960 MHz
 * Sensitivity = –118 dBm
 * Data rate = 0.2 to 128 kbps
 * Programmable RX BW 2.6–620 kHz
 * RX 64 byte FIFO
 */

int main (void){

	DWT_init();

	STARTDWT;
	HS_Enable();
	RNG_Init();
	SPI4_Init();
	DMA_init();
	ILI9341_init();
	STOPDWT;

	volatile uint16_t *Flashsize = (uint16_t *)FLASHSIZE_BASE;
	volatile uint32_t *UniqueID = (uint32_t *)UID_BASE;

	Fill_Screen(BLUE);
	console_printf(MsgInfo,"<%d> InitializationTick" , DWT->CYCCNT);
	console_printf(MsgInfo,"STM32F767ZIT6 UID - %d%d%d", UniqueID[0], UniqueID[1], UniqueID[2]);
	console_printf(MsgWarning," __CORTEX_M%d __CM7_CMSIS_VERSION - %d",  __CORTEX_M, __CM7_CMSIS_VERSION);
	console_printf(MsgWarning,"Flash Size - %d Kbytes ", Flashsize[0]);
	console_printf(MsgInfo,"Version Firmware  - 2.12");
	console_printf(MsgInfo,"	Core - %dMhz\n\r	APB1Perip - %dMhz\n\r	APB1Timer - %dMhz\n\r	APB2Perip - %dMhz\n\r	APB2Timer - %dMhz", ClockConf.Core, ClockConf.APB1Perip, ClockConf.APB1Timer, ClockConf.APB2Perip, ClockConf.APB2Timer);
	Display_Image(ScreenBuff);

	delay(50000000);

	for(ever) {

		Running();

	}
}

void Running (void) {

	u16 YPosition = 190;
	u8 Step = 1;
	u16 Data;
	u16 Color = GREEN ;
	u16 DebugIterator, XPositionConteiner = 0;
	u16 YPositionConteiner = 200;

	u16 YPositionStart = YPosition;
	//u16 MarkerPosition = YPosition;
	u16 YPositionStep;
	u16 DisplayedData;

	for (u16 iterator = 9; iterator <= ILI9341_TFTHEIGHT - 5; iterator += Step) {

		DebugIterator++;

		if (iterator >= (ILI9341_TFTHEIGHT - 8)) {

			Render(YPositionStart, 300, 1200, XPositionConteiner, YPositionConteiner);
			iterator = 9;
			DebugIterator = 0;
			XPositionConteiner = 0;
			YPositionConteiner = 200;
			Display_Image(ScreenBuff);
			Clear_Screen();

		}

		Data = ((RNG -> DR) >> 24) / 8;
		YPositionStep = iterator + Step;
		DisplayedData = YPositionStart - Data;


			if (DebugIterator == (160 - 8) / Step) {

				DisplayedData = 60;

			}

			if (DebugIterator == (180 - 9) / Step) {

				DisplayedData = 90;

			}

			if (DebugIterator == (181 - 9) / Step) {

				DisplayedData = 70;

			}

			if (DisplayedData < YPositionConteiner) {

			XPositionConteiner = YPositionStep;
			YPositionConteiner = DisplayedData;

			}

		Draw_Line(iterator, YPosition, YPositionStep, DisplayedData, Color);
		YPosition = DisplayedData;

	}
}

void RNG_Init (void) {

	RCC -> AHB2ENR |= RCC_AHB2ENR_RNGEN;
	RNG -> CR |= RNG_CR_RNGEN;

}

