#include "ili9341.h"

static u8 rotation = 0;						//Положение развертки экрана
static u16 _width = ILI9341_TFTWIDTH;		//Аддаптивная ширина
static u16 _height = ILI9341_TFTHEIGHT;		//Аддаптивная высота

static void SPI_Send_Cmd (u8 data) {

	DC_L();
	CS_L();
	SPI4_write_byte(data);
	CS_H();

}

static void SPI_Send_Data (u8 data) {

	DC_H();
	CS_L();
	SPI4_write_byte(data);
	CS_H();

}

static void Set_Rotation(u8 m) {

    rotation = m % 4; // can't be higher than 3
    switch (rotation)
    {
        case 0:
            m = (MADCTL_MX | MADCTL_BGR);
            _width  = ILI9341_TFTWIDTH;
            _height = ILI9341_TFTHEIGHT;
            break;
        case 1:
            m = (MADCTL_MV | MADCTL_BGR);
            _width  = ILI9341_TFTHEIGHT;
            _height = ILI9341_TFTWIDTH;
            break;
        case 2:
            m = (MADCTL_MY | MADCTL_BGR);
            _width  = ILI9341_TFTWIDTH;
            _height = ILI9341_TFTHEIGHT;
            break;
        case 3:
            m = (MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
            _width  = ILI9341_TFTHEIGHT;
            _height = ILI9341_TFTWIDTH;
            break;
    }

    SPI_Send_Cmd(MEMORYACCESSCONTROL);
    SPI_Send_Data(m);
}

void ILI9341_init (void) {

	SPI_Send_Cmd(SOFTTRESET);//soft reset
	delay(100000);

	SPI_Send_Cmd(0xCB);
	SPI_Send_Data(0x39);
	SPI_Send_Data(0x2C);
	SPI_Send_Data(0x00);
	SPI_Send_Data(0x34);
	SPI_Send_Data(0x02);

	SPI_Send_Cmd(0xCF);
	SPI_Send_Data(0x00);
	SPI_Send_Data(0XC1);
	SPI_Send_Data(0X30);

	SPI_Send_Cmd(0xE8);
	SPI_Send_Data(0x85);
	SPI_Send_Data(0x00);
	SPI_Send_Data(0x78);

	SPI_Send_Cmd(0xEA);
	SPI_Send_Data(0x00);
	SPI_Send_Data(0x00);

	SPI_Send_Cmd(0xED);
	SPI_Send_Data(0x64);
	SPI_Send_Data(0x03);
	SPI_Send_Data(0X12);
	SPI_Send_Data(0X81);

	SPI_Send_Cmd(0xF7);
	SPI_Send_Data(0x20);

	SPI_Send_Cmd(POWERCPNTROL1);    	//Power control
	SPI_Send_Data(0x23);   	//VRH[5:0]

	SPI_Send_Cmd(POWERCPNTROL2);    	//Power control
	SPI_Send_Data(0x10);   	//SAP[2:0];BT[3:0]

	SPI_Send_Cmd(VCOMCONTROL1);    	//VCM control
	SPI_Send_Data(0x3e);   	//Contrast
	SPI_Send_Data(0x28);

	SPI_Send_Cmd(VCOMCONTROL2);    	//VCM control2
	SPI_Send_Data(0x86);  	 //--

	SPI_Send_Cmd(MEMORYACCESSCONTROL);    	// Memory Access Control
	SPI_Send_Data(0x48);  	//C8	   //48 68绔栧睆//28 E8 妯睆

	SPI_Send_Cmd(COLMODE);
	SPI_Send_Data(0x55);

	SPI_Send_Cmd(FRAMERATECONTROL);
	SPI_Send_Data(0x00);
	SPI_Send_Data(0x18);

	SPI_Send_Cmd(DISPLAYFUNCTIONCONTROL);    	// Display Function Control
	SPI_Send_Data(0x08);
	SPI_Send_Data(0x82);
	SPI_Send_Data(0x27);

	SPI_Send_Cmd(0xF2);    	// 3Gamma Function Disable
	SPI_Send_Data(0x00);

	SPI_Send_Cmd(GAMMASET);    	//Gamma curve selected
	SPI_Send_Data(0x01);

	SPI_Send_Cmd(POSITIVEGAMMACORRECTION);    	//Set Gamma
	SPI_Send_Data(0x0F);
	SPI_Send_Data(0x31);
	SPI_Send_Data(0x2B);
	SPI_Send_Data(0x0C);
	SPI_Send_Data(0x0E);
	SPI_Send_Data(0x08);
	SPI_Send_Data(0x4E);
	SPI_Send_Data(0xF1);
	SPI_Send_Data(0x37);
	SPI_Send_Data(0x07);
	SPI_Send_Data(0x10);
	SPI_Send_Data(0x03);
	SPI_Send_Data(0x0E);
	SPI_Send_Data(0x09);
	SPI_Send_Data(0x00);

	SPI_Send_Cmd(NEGATIVEGAMMACORRECTION);    	//Set Gamma
	SPI_Send_Data(0x00);
	SPI_Send_Data(0x0E);
	SPI_Send_Data(0x14);
	SPI_Send_Data(0x03);
	SPI_Send_Data(0x11);
	SPI_Send_Data(0x07);
	SPI_Send_Data(0x31);
	SPI_Send_Data(0xC1);
	SPI_Send_Data(0x48);
	SPI_Send_Data(0x08);
	SPI_Send_Data(0x0F);
	SPI_Send_Data(0x0C);
	SPI_Send_Data(0x31);
	SPI_Send_Data(0x36);
	SPI_Send_Data(0x0F);

	SPI_Send_Cmd(SLEEPOUT);    	//Exit Sleep
	delay(100000);

	SPI_Send_Cmd(DISPLAYON);    //Display on
	SPI_Send_Cmd(MEMORYWRITE);
	delay(100000);

	Set_Rotation(3);
}
//------------------------------------------------------------------------------
static void Set_Col (u16 StartCol, u16 EndCol) {//да да да, она отправляется по 8 бит

	SPI_Send_Cmd(COLUMNADDRSET);
	SPI_Send_Data(StartCol>>8);
	SPI_Send_Data(StartCol);
	SPI_Send_Data(EndCol>>8);
	SPI_Send_Data(EndCol);

}

static void Set_Page (u16 StartPage, u16 EndPage) {

	SPI_Send_Cmd(PAGEADDRSET);
	SPI_Send_Data(StartPage>>8);
	SPI_Send_Data(StartPage);
	SPI_Send_Data(EndPage>>8);
	SPI_Send_Data(EndPage);

}

void Set_XY (u16 x1, u16 y1, u16 x2, u16 y2) {

	Set_Col(x1, x2);
	Set_Page(y1, y2);
	SPI_Send_Cmd(MEMORYWRITE);

}
//------------------------------------------------------------------------------
#if (ILI9341_MODE == ILI9341_DIRECT_MODE)
void Set_Pixel (u16 poX, u16 poY, u16 color) {

	u16 ColorArray [] = {color}; //Уебанская обертка для передачи в дма
	Set_XY(poX, poY, poX, poY);

	SPI4 -> CR2 |= SPI_CR2_DS;					//SPI 16bit
	DMA2_Stream1 -> CR &= ~DMA_SxCR_MINC;		//Addr increment reset

	DC_H();
	CS_L();

	DMA_send16b(ColorArray, 1);
	delay(50);

	CS_H();

	SPI4 -> CR2 &= ~SPI_CR2_DS_3;				// SPI 8bit
	SPI4 -> CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2;

}

void Clear_Screen (void) {

	u16 black [] = {0x0000};
	Set_XY(0, 0, _width, _height);

	SPI4 -> CR2 |= SPI_CR2_DS;					//SPI 16bit
	DMA2_Stream1 -> CR &= ~DMA_SxCR_MINC;		//Addr increment reset
	FlagFuck = 1;

	DC_H();
	CS_L();

	DMA_send16b(black, 76800);

	CS_H();

	SPI4 -> CR2 &= ~SPI_CR2_DS_3;				// SPI 8bit
	SPI4 -> CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2;

}

void Push_Color (u16 color) {

	u16 ColorArray [] = {color};

	SPI4 -> CR2 |= SPI_CR2_DS;					//SPI 16bit
	DMA2_Stream1 -> CR &= ~DMA_SxCR_MINC;		//Addr increment reset

	DC_H();
	CS_L();

	DMA_send16b(ColorArray, 1);
	delay(10);

	CS_H();

	SPI4 -> CR2 &= ~SPI_CR2_DS_3;				// SPI 8bit
	SPI4 -> CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2;

}

void Fill_Screen (u16 color) {

	u16 coll [] = {color};
	Set_XY(0, 0, _width, _height);

	SPI4 -> CR2 |= SPI_CR2_DS;					//SPI 16bit
	DMA2_Stream1 -> CR &= ~DMA_SxCR_MINC;		//Addr increment reset
	FlagFuck = 1;

	DC_H();
	CS_L();

	DMA_send16b(coll, ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT);

	CS_H();

	SPI4 -> CR2 &= ~SPI_CR2_DS_3;				// SPI 8bit
	SPI4 -> CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2;

}

#endif

#if (ILI9341_MODE == ILI9341_BUFFER_MODE)
void Set_Pixel (u16 poX, u16 poY, u16 color){

	ScreenBuff[poY * _width + poX] = color;

}

void Clear_Screen (void){

	for (u32 i = 0; i < ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT; i ++) {

		ScreenBuff[i] = 0;

	}
}

void Fill_Screen (u16 color){

	for (u32 i = 0; i < ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT; i ++) {

		ScreenBuff[i] = color;

	}
}

#endif
//------------------------------------------------------------------------------
void Display_Image (u16 *array) {

	Set_XY(0, 0, _width, _height);

	SPI4 -> CR2 |= SPI_CR2_DS;
	DMA2_Stream1 -> CR |= DMA_SxCR_MINC;		//Addr increment
	FlagFuck = 0;

	DC_H();
	CS_L();

	DMA_send16b(array, ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT);

	CS_H();

	SPI4 -> CR2 &= ~SPI_CR2_DS_3;				// SPI 8bit
	SPI4 -> CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2;

}

void Draw_Vertical_Line(u16 poX, u16 poY, u16 length, u16 color) {

	for(int i = 0; i < length; i++)

		Set_Pixel(poX, poY + i, color);

}

void Draw_Horizontal_Line (u16 poX, u16 poY, u16 length, u16 color) {

	for(int i = 0; i < length; i++)

		Set_Pixel(poX + i, poY, color);

}

void Rectangle (u16 startX, u16 startY, u16 endX, u16 endY ,u16 color) {

	u16 lengthX,lengthY;

	lengthX = endX - startX;
	lengthY = endY - startY;

	Draw_Vertical_Line(startX, startY, lengthY, color);
	Draw_Horizontal_Line(startX, startY, lengthX, color);

	Draw_Vertical_Line(startX + lengthX, startY, lengthY, color);
	Draw_Horizontal_Line(startX, startY + lengthY, lengthX, color);

}

void Draw_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color){

	const u16 deltaX = fabs(x2 - x1);
	const u16 deltaY = fabs(y2 - y1);
	const u16 signX = x1 < x2 ? 1 : -1;
	const u16 signY = y1 < y2 ? 1 : -1;

	u16 error = deltaX - deltaY;

	Set_Pixel(x2, y2, color);

		while (x1 != x2 || y1 != y2){

			Set_Pixel(x1, y1, color);
			const int16_t error2 = error * 2;

	    if (error2 > -deltaY){

	      error -= deltaY;
	      x1 += signX;

	    }

	    if (error2 < deltaX){

	      error += deltaX;
	      y1 += signY;

	    }
	  }
}

void Draw_Circle (u16 poX, u16 poY, u16 radius, u16 color){

    int x = -radius, y = 0, err = 2 - 2 * radius, e2;

    do {

        Set_Pixel(poX - x, poY + y, color);
        Set_Pixel(poX + x, poY + y, color);
        Set_Pixel(poX + x, poY - y, color);
        Set_Pixel(poX - x, poY - y, color);

        e2 = err;

        if (e2 <= y) {

            err += ++y * 2 + 1;

            if (-x == y && e2 <= x) e2 = 0;
        }

        if (e2 > x) err += ++x * 2 + 1;

    }

    while (x <= 0);

}

void Draw_Triangle (u16 poX1, u16 poY1, u16 poX2, u16 poY2, u16 poX3, u16 poY3, u16 color){

	Draw_Line(poX1, poY1, poX2, poY2, color);
	Draw_Line(poX1, poY1, poX3, poY3, color);
	Draw_Line(poX2, poY2, poX3, poY3, color);

}

void Draw_Punct_Vertical(u16 x, u16 y, u16 light, u16 segment, u16 color){

	for (u16 i = y; i < light; i = i + segment + 10) {

		Draw_Vertical_Line(x, i, segment, color);

	}
}

void Draw_Punct_Horisontical(u16 x, u16 y, u16 light, u16 segment, u16 color){

	for (u16 i = x; i < light; i = i + segment + 10) {

		Draw_Horizontal_Line(i, y, segment, color);

	}
}
//--------------------------------------------------------------------------------
u8 Draw_Char (u16 x, u16 y, u8 FontID, u8 Char, u16 TextColor, u16 BgColor, u8 TransparentBg){

	u8 *pCharTable = font_GetFontStruct(FontID, Char);
	u8 CharWidth = font_GetCharWidth(pCharTable);//9
	u8 CharHeight = font_GetCharHeight(pCharTable);//16
	pCharTable += 2;

	if (FontID == FONTID_6X8M) {

			for (u8 row = 0; row < CharHeight; row ++) {

				for (u8 col = 0; col < CharWidth; col ++) {

					if (pCharTable[row] & (1 << (7 - col))) {

						Set_Pixel(x + col, y + row, TextColor);

					}

					else if (!TransparentBg) {

						Set_Pixel(x + col, y + row, BgColor);

					}
				}
			}
	}
	else {

		for (u8 row = 0; row < CharHeight; row ++) {

			for (u8 col = 0; col < CharWidth; col ++) {

				if (col < 8) {

					if (pCharTable[row * 2] & (1 << (7 - col))) {

						Set_Pixel(x + col, y + row, TextColor);

					}

					else if (!TransparentBg) {

						Set_Pixel(x + col, y + row, BgColor);

					}

				}

				else {

					if (pCharTable[(row * 2) + 1] & (1 << (15 - col))) {

						Set_Pixel(x + col, y + row, TextColor);

					}

					else if (!TransparentBg) {

						Set_Pixel(x + col, y + row, BgColor);

					}
				}
			}
		}
	}

	return CharWidth;
}

u16 Draw_String (u16 x, u16 y, u8 FontID, char *Str, u16 TextColor, u16 BgColor, u8 TransparentBg){

	u8 *pCharTable = font_GetFontStruct(FontID, Str[0]);
	u8 done = 0;
	u16 XStart = x;
	u8 StrHeigh = font_GetCharHeight(pCharTable);

	while (!done) {

		switch (*Str) {

		case '\0':
			done = 1;
			break;

		case '\n':
			y += StrHeigh = font_GetCharHeight(pCharTable);
			break;

		case '\r':
			x = XStart;
		break;

		default:
			if (TransparentBg) {

				x += Draw_Char(x, y, FontID, *Str, TextColor, 0, 1);

			}

			else {

				x += Draw_Char(x, y, FontID, *Str, TextColor, BgColor, 0);

			}

			StrHeigh = font_GetCharHeight(font_GetFontStruct(FontID, *Str));
			break;

		}

		Str++;

	}

	return x;

}

u16 Draw_Number (u16 x, u16 y, u8 FontID, u16 Number, u16 TextColor, u16 BgColor, u8 TransparentBg){

	//u8 *pCharTable = font_GetFontStruct(FontID, Number + 0x30);
	//u8 done = 0;
	//u16 XStart = x;
	//u8 StrHeigh = font_GetCharHeight(pCharTable);

	u8 n1,n2,n3,n4;

	if (Number > 9 && Number < 100) {

		n2 = (Number - (Number % 10)) / 10;
		n1 =  Number % 10;

		if (TransparentBg) {

			x += Draw_Char(x, y, FontID, n2 + 0x30, TextColor, 0, 1);
			x += Draw_Char(x, y, FontID, n1 + 0x30, TextColor, 0, 1);

		}

		else {

			x += Draw_Char(x, y, FontID, n2 + 0x30, TextColor, BgColor, 0);
			x += Draw_Char(x, y, FontID, n1 + 0x30, TextColor, BgColor, 0);

		}

		//StrHeigh = font_GetCharHeight(font_GetFontStruct(FontID, Number));

	}

	else if (Number > 99 && Number < 1000) {

	    n3 = (Number - (Number % 100)) / 100;
		n2 = ((Number - (Number % 10)) - (Number - (Number % 100))) / 10;
		n1 = Number % 10;

		if (TransparentBg) {

			x += Draw_Char(x, y, FontID, n3 + 0x30, TextColor, 0, 1);
			x += Draw_Char(x, y, FontID, n2 + 0x30, TextColor, 0, 1);
			x += Draw_Char(x, y, FontID, n1 + 0x30, TextColor, 0, 1);

		}

		else {

			x += Draw_Char(x, y, FontID, n3 + 0x30, TextColor, BgColor, 0);
			x += Draw_Char(x, y, FontID, n2 + 0x30, TextColor, BgColor, 0);
			x += Draw_Char(x, y, FontID, n1 + 0x30, TextColor, BgColor, 0);

		}

		//StrHeigh = font_GetCharHeight(font_GetFontStruct(FontID, Number));

	}

	else if (Number > 999 && Number < 10000) {

	    n4 = (Number - (Number % 1000)) / 1000;
	    n3 = ((Number - (Number % 100)) - (Number - (Number % 1000))) / 100;
	    n2 = ((Number - (Number % 10)) - (Number - (Number % 100))) / 10;
	    n1 = Number % 10;

		if (TransparentBg) {

			x += Draw_Char(x, y, FontID, n4 + 0x30, TextColor, 0, 1);
			x += Draw_Char(x, y, FontID, n3 + 0x30, TextColor, 0, 1);
			x += Draw_Char(x, y, FontID, n2 + 0x30, TextColor, 0, 1);
			x += Draw_Char(x, y, FontID, n1 + 0x30, TextColor, 0, 1);

		}

		else {

			x += Draw_Char(x, y, FontID, n4 + 0x30, TextColor, BgColor, 0);
			x += Draw_Char(x, y, FontID, n3 + 0x30, TextColor, BgColor, 0);
			x += Draw_Char(x, y, FontID, n2 + 0x30, TextColor, BgColor, 0);
			x += Draw_Char(x, y, FontID, n1 + 0x30, TextColor, BgColor, 0);

		}

		//StrHeigh = font_GetCharHeight(font_GetFontStruct(FontID, Number));

	}
	else if (Number > 9999) {

		Draw_String (x, y, FontID, "> 9999", TextColor, BgColor, 1);

	}

	else {

		if (TransparentBg) {

			x += Draw_Char(x, y, FontID, Number + 0x30, TextColor, 0, 1);

		}

		else {

			x += Draw_Char(x, y, FontID, Number + 0x30, TextColor, BgColor, 0);

		}
	}

	return (0);

}
























