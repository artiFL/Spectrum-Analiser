#include "HSEClock.h"

void HS_Enable(void){

	//Конфигурация флеш, включение делителя тактирования
	FLASH -> ACR |= FLASH_ACR_PRFTEN;
	FLASH -> ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
	FLASH -> ACR |= (uint32_t)FLASH_ACR_LATENCY_7WS;// SW7

	//RCC->CR |= RCC_CR_CSSON;    // Clock security sistem

	//Чистим вилкой
	RCC -> CR = 0;
	RCC -> CFGR = 0;
	//Включаем bypass
	RCC -> CR |= RCC_CR_HSEBYP;
	//Enable HSE
	RCC -> CR |= RCC_CR_HSEON;
	//Wait till HSE is ready
	    while(!(RCC -> CR & RCC_CR_HSERDY));

	//PLL configuration   PLLM = /4     PLLN = *216    PLLP = /2 0b11011000000100
	RCC -> PLLCFGR = 0;
	RCC -> PLLCFGR =  0x3604;

	 //PLL surce
	RCC -> PLLCFGR |= RCC_PLLCFGR_PLLSRC;

	// HCLK = SYSCLK
	RCC -> CFGR |= RCC_CFGR_HPRE_DIV1;
	// PCLK1 = HCLK
	RCC -> CFGR |= RCC_CFGR_PPRE1_DIV4;
	// PCLK2 = HCLK
	RCC -> CFGR |= RCC_CFGR_PPRE2_DIV2;


	// Turn on the PLL
	RCC -> CR |= RCC_CR_PLLON;
	// Wait till PLL is ready
	    while((RCC -> CR & RCC_CR_PLLRDY) == 0);

	RCC -> CFGR &= ~RCC_CFGR_SW;
	RCC -> CFGR |= RCC_CFGR_SW_PLL;
	    while((RCC -> CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	ClockConf.PLLM = RCC -> PLLCFGR & RCC_PLLCFGR_PLLM;
	ClockConf.PLLN = (RCC -> PLLCFGR & RCC_PLLCFGR_PLLN) >> 6;
	ClockConf.PLLP = (((RCC -> PLLCFGR & RCC_PLLCFGR_PLLP) >> 16) + 1) * 2;
	((RCC -> CFGR >> 4) & 0xF) == 0 ? (ClockConf.PHCLK = 1) : (ClockConf.PHCLK = pow(2, (((RCC -> CFGR >> 4) & 0xF) % 8) + 1));
	(((RCC -> CFGR >> 10) & 0x7) % 4) == 0 ? (ClockConf.PPCLK1 = 1) : (ClockConf.PPCLK1 = pow(2, (((RCC -> CFGR >> 10) & 0xF) % 4) + 1));
	(((RCC -> CFGR >> 13) & 0x7) % 4) == 0 ? (ClockConf.PPCLK2 = 2) : (ClockConf.PPCLK2 = pow(2, (((RCC -> CFGR >> 13) & 0xF) % 4) + 1));

	ClockConf.AHB = (((clock / ClockConf.PLLM) * ClockConf.PLLN) / ClockConf.PLLP) / ClockConf.PHCLK;
	ClockConf.APB1Perip = ClockConf.AHB / ClockConf.PPCLK1;
	ClockConf.APB2Perip = ClockConf.AHB / ClockConf.PPCLK2;
	ClockConf.APB1Timer = ClockConf.APB1Perip * 2;
	ClockConf.APB2Timer = ClockConf.APB2Perip * 2;
	clock = ClockConf.AHB;
	ClockConf.Core = clock;

}

void DWT_init(void) {

    CoreDebug->DEMCR |= 0x01000000;
    DWT-> LAR = 0xC5ACCE55;
    DWT->CYCCNT = 0;
    DWT->CTRL = 0;

}

