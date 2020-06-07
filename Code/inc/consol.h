#ifndef CONSOL_H_
#define CONSOL_H_

#include "ili9341.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

typedef enum
{
	MsgInfo = 0,
	MsgWarning = 1,
	MsgError = 2,
	MsgErrorCons = 3
}
eConsoleMsgType;

typedef enum
{
	UnknownError = 404,
	LowVoltageBat = 1,
	HoghVoltageBat = 2,
	WaitingError = 3

}
eTypeError;



void console_printf(eConsoleMsgType msgType, const char *args, ...);
void FatalError(eTypeError TypeError);

#endif
