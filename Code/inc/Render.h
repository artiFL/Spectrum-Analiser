//#include "macro.h"
#include "ili9341.h"
#include "consol.h"


struct symbol
{
  char *ThirtyProcent;
  char *SixtyProcent;
  char *NinetyProcent;
  char *HundredProcent;
  char *Lightning;
  char *ArrowUp;
  char *Delta;
  char *MarkerDown;

};

void Render (u8 YArrowPosition, u16 FreqLeft, u16 FreqRight, u16 YPointMaxFreqValue, u16 YPointMaxFreq);


