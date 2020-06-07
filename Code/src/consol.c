#include "consol.h"

#define LineHeight	10
static uint16_t ConsoleLine = 0;

void console_printf(eConsoleMsgType msgType, const char *args, ...) {

	char StrBuff[256];
	uint16_t TextColor = WHITE;

	switch (msgType)
	{
	case MsgInfo:
		TextColor = GREEN;
		break;
	case MsgWarning:
		TextColor = ORANGE;
		break;
	case MsgError:
		TextColor = RED;
		break;
	case MsgErrorCons:
		ConsoleLine = 0;
		TextColor = RED;
		break;
	}



	va_list ap;
	va_start(ap, args);
	vsnprintf(StrBuff, sizeof(StrBuff), args, ap);
	va_end(ap);

	Draw_String(0, ConsoleLine * LineHeight, FONTID_6X8M, (u8*)StrBuff, TextColor, WHITE, 1);
    printf("%s", StrBuff);

    if (msgType != MsgInfo){

    	delay(50000);

    }


	if (++ConsoleLine > 240 / LineHeight)
    {
		ConsoleLine = 0;
		delay(50000);
		Clear_Screen();
    }
}

void FatalError(eTypeError TypeError) {

	Fill_Screen(BLACK);
	console_printf(MsgErrorCons, "Fatal ERROR %d", TypeError);
	Display_Image(ScreenBuff);

	delay(140000000);
	SCB->AIRCR = 0x05FA0004; //reboot soft "not working correctly"

}







