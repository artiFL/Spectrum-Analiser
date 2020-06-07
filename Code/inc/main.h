#include "stm32f7xx.h"
#include "macro.h"
#include "ili9341.h"
#include "HSEClock.h"
#include "consol.h"
#include "Render.h"

u32 clock = 8000000; // Standart clock input

void Running (void);
void RNG_Init (void);
