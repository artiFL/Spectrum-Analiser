#include "stm32f7xx.h"
#include "macro.h"

extern u32 clock;

#define STARTDWT 	DWT->CTRL |= 0x00000001
#define STOPDWT 	DWT->CTRL &= 0xFFFFFFFE

typedef struct {

	u8 PLLM;
	u8 PLLP;
	u16 PLLN;
	u8 PHCLK;
	u8 PPCLK2;
	u8 PPCLK1;
	u32 AHB;
	u32 APB1Perip;
	u32 APB1Timer;
	u32 APB2Perip;
	u32 APB2Timer;
	u32 Core;

} ClockConfiguration;

ClockConfiguration ClockConf;

void HS_Enable(void);
void DWT_init(void);
