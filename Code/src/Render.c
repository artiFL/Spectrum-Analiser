#include "Render.h"

static struct symbol sb = {"Ж","З", "И", "Й", "М", "Н", "О", "П"};

static void Floating_Number (u16 x, u16 y, u16 freq, u16 dbm, u16 color){

	Draw_String(x - 4, y -16, FONTID_16F, sb.MarkerDown, color, YELLOW,1);// ^

	Draw_Number(x + 8, y - 23, FONTID_6X8M, freq, color, YELLOW,1);//	freq
	Draw_String(x + 28, y - 23,FONTID_6X8M, "Mhz", color, YELLOW,1);// 	Mhz

	Draw_String(x, y - 15, FONTID_6X8M, "-", color, YELLOW,1);// 		-
	Draw_Number(x + 8, y - 15,FONTID_6X8M, dbm, color, YELLOW,1);//		100
	Draw_String(x + 28, y - 15, FONTID_6X8M, "dbm", color, YELLOW,1);// dbm

}

void Render (u8 YArrowPosition, u16 FreqLeft, u16 FreqRight, u16 YPointMaxFreqValue, u16 YPointMaxFreq){

	FreqLeft < 240 ? (FreqLeft = 240):(FreqLeft = FreqLeft);
	FreqRight > 960 ? (FreqRight = 960):(FreqRight = FreqRight);
	//------------------------------------------------------------------------------------
	Rectangle(8, 25, 315, 215, DARKGREY);													//основное окно ..306  76.5
	Draw_String(0, YArrowPosition - 7, FONTID_16F,">",DARKGREEN,YELLOW,1);					//указатель нул€ Y

	//--Vertical line calculated-----------------------------------------------------------

	u16 var = 307 / ((FreqRight - FreqLeft) / 100);											//расчет сдвига масштабировани€ вертикальной сетки
	u8 step = var;
	var = var + 8;

	for (u8 i = 0; i <= ((FreqRight - FreqLeft) / 100) - 1; i++) {

		Draw_Punct_Vertical(var, 25, 215, 5, DARKGREY);
		var = var + step;
		if (var  >= 310) {

			var = 0;
			break;

		}
	}
	//---Run/Stop---------------------------------------------------------------------------
	Draw_String(5,5,FONTID_16F,"Running", DARKGREEN,YELLOW,1);								//runing/stop
	//Draw_string(18,5,FONTID_16F,"Stop", RED,YELLOW,1);
	//---Mark point-------------------------------------------------------------------------
	Draw_String(80,5,FONTID_16F,"Mark 1", DARKCYAN,YELLOW,1);								// marker 1
	Draw_String(140,5, FONTID_16F,"Mark 2", 0x026f,YELLOW,1);								// marker 2
	//---Delta mark value-------------------------------------------------------------------
	u8 XDeltaFreqUpBar = 200;
	u8 YDeltaFreqUpBar = 3;
	Draw_String(XDeltaFreqUpBar, YDeltaFreqUpBar, FONTID_16F, sb.Delta, DARKGREY, YELLOW,1);// delta
	Draw_Number(XDeltaFreqUpBar + 20, YDeltaFreqUpBar + 2, FONTID_6X8M, 211, DARKGREY, YELLOW,1);
	Draw_String(XDeltaFreqUpBar + 39, YDeltaFreqUpBar + 2, FONTID_6X8M, "Mhz", DARKGREY, YELLOW,1);		// delata freq
	Draw_String(XDeltaFreqUpBar + 20, YDeltaFreqUpBar + 12, FONTID_6X8M, "-", DARKGREY, YELLOW,1);		// 	-
	Draw_Number(XDeltaFreqUpBar + 27, YDeltaFreqUpBar + 12, FONTID_6X8M, 80, DARKGREY, YELLOW,1);		//	100
	Draw_String(XDeltaFreqUpBar + 40, YDeltaFreqUpBar + 12, FONTID_6X8M, "dbm", DARKGREY, YELLOW,1);	// dbm
	//---Charge and battery status----------------------------------------------------------
	Draw_String(300,5, FONTID_16F, sb.ThirtyProcent,DARKGREY,YELLOW,1);							// отображение зар€да батареии
	Draw_String(285,5,FONTID_16F, sb.Lightning,YELLOW,DARKGREEN,1);								// статус зар€дки
	//---Left Freq--------------------------------------------------------------------------
	Draw_Number(7,220,FONTID_16F, FreqLeft, CYAN,YELLOW,1);									// left freq
	Draw_String(34,220,FONTID_16F, "Mhz", CYAN,YELLOW,1);									// freq
	//---Right Freq-------------------------------------------------------------------------
	Draw_Number(260,220,FONTID_16F, FreqRight, CYAN,YELLOW,1);								//right freq
	Draw_String(287,220,FONTID_16F, "Mhz", CYAN,YELLOW,1);									// freq
	//---Delta freq-------------------------------------------------------------------------
	Draw_Number(84,220,FONTID_16F, FreqRight - FreqLeft, CYAN,YELLOW,1);					// ѕолоса пропускани€
	Draw_String(111,220,FONTID_16F, "Mhz", CYAN,YELLOW,1);									// freq
	//---Floating number--------------------------------------------------------------------
	u32 XPointMaxFreq =  (((FreqRight - FreqLeft) * YPointMaxFreqValue) / 320);				// на частотах выше 1500ћгц точность отображени€ частоты падает до +-2%
	Floating_Number(YPointMaxFreqValue, YPointMaxFreq, XPointMaxFreq + FreqLeft, YPointMaxFreq, YELLOW);

}



