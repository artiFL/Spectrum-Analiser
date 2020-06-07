#include "ulib.h"
#include "math.h"
#include "font.h"
#include "stm32f7xx.h"

#define ILI9341_TFTWIDTH   			240      ///< ILI9341 max TFT width
#define ILI9341_TFTHEIGHT  			320      ///< ILI9341 max TFT height

#define ILI9341_DIRECT_MODE			0
#define ILI9341_BUFFER_MODE			1

#define ILI9341_MODE				ILI9341_BUFFER_MODE

#if (ILI9341_MODE == ILI9341_BUFFER_MODE)
uint16_t ScreenBuff[ILI9341_TFTHEIGHT * ILI9341_TFTWIDTH];
#endif

//color
#define BLACK       				0x0000
#define NAVY        				0x000F
#define DARKGREEN   				0x03E0
#define DARKCYAN   					0x03EF
#define MAROON      				0x7800
#define PURPLE      				0x780F
#define OLIVE       				0x7BE0
#define LIGHTGREY   				0xC618
#define DARKGREY    				0x7BEF
#define BLUE       					0x001F
#define GREEN       				0x07E0
#define CYAN        				0x07FF
#define RED         				0xF800
#define MAGENTA     				0xF81F
#define YELLOW      				0xFFE0
#define WHITE       				0xFFFF
#define ORANGE     					0xFD20
#define GREENYELLOW 				0xAFE5
#define PINK        				0xF81F

//command
#define MADCTL_MY  					0x80  ///< Bottom to top
#define MADCTL_MX  					0x40  ///< Right to left
#define MADCTL_MV  					0x20  ///< Reverse Mode
#define MADCTL_ML  					0x10  ///< LCD refresh Bottom to top
#define MADCTL_RGB 					0x00  ///< Red-Green-Blue pixel order
#define MADCTL_BGR 					0x08  ///< Blue-Green-Red pixel order
#define MADCTL_MH  					0x04  ///< LCD refresh right to left


#define NOP							0x00
#define SOFTTRESET  				0x01

#define READID						0x04
#define READSTATUS  				0x09
#define READPOWERMODE				0x0A
#define READMADCTL					0x0B
#define READPIXFORMAT				0x0C
#define READDISPIMAGEFORMAT 		0x0D
#define READDISPSIGMALMODE			0x0E
#define READSELFDIAGNOSTIC			0x0F

#define ENTERSLEEPMODE				0x10
#define SLEEPOUT					0x11

#define PARTIALMODEON				0x12
#define NORMALMODEON				0x13

#define INVERSIONOFF				0x20
#define INVERSIONON					0x21

#define GAMMASET					0x26

#define DISPLAYOFF					0x28
#define DISPLAYON					0x29

#define COLUMNADDRSET				0x2A
#define PAGEADDRSET					0x2B

#define MEMORYWRITE					0x2C

#define COLORSET					0x2D
#define MEMORYREAD					0x2E
#define PARTIALAREA					0x30
#define VERTICALSCROLLINGDEF		0x33
#define TEARINGLINEOFF				0x34
#define TEARINGLINEON				0x35
#define MEMORYACCESSCONTROL			0x36
#define VERTICALSCROLLSTARTADDR		0x37
#define IDLEOFF						0x38
#define IDLEON						0x39
#define COLMODE						0x3A
#define WRITEBRIGHTNESS				0x51
#define WRITECTRL					0x53
#define READCTRL					0x54
#define ADAPTIVEBRIGHTNESSCONTROL	0x55
#define	INVERSIONCONTROL			0xB4
#define FRAMERATECONTROL			0xB1
#define DISPLAYFUNCTIONCONTROL		0xB6
#define	POWERCPNTROL1				0xC0
#define	POWERCPNTROL2				0xC1
#define VCOMCONTROL1				0xC5
#define VCOMCONTROL2				0xC7
#define	POSITIVEGAMMACORRECTION		0xE0
#define NEGATIVEGAMMACORRECTION		0xE1
#define	DIGITALGAMMACONTROL1		0xE2
#define	DIGITALGAMMACONTROL2		0xE3
#define	INTERFACECONTROL			0xF6

void ILI9341_init (void);
//----------------------------------------------------------------------------------
void Set_XY (u16 x1, u16 y1, u16 x2, u16 y2);
void Set_Pixel (u16 poX, u16 poY, u16 color);
void Clear_Screen (void);
void Push_Color (u16 color);
void Fill_Screen (u16 color);
//----------------------------------------------------------------------------------
void Display_Image (u16 *array);
void Draw_Vertical_Line(u16 poX, u16 poY, u16 length, u16 color);
void Draw_Horizontal_Line (u16 poX, u16 poY, u16 length, u16 color);
void Rectangle (u16 startX, u16 startY, u16 endX, u16 endY ,u16 color);
void Draw_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void Draw_Circle (u16 poX, u16 poY, u16 radius, u16 color);
void Draw_Triangle (u16 poX1, u16 poY1, u16 poX2, u16 poY2, u16 poX3, u16 poY3, u16 color);
void Draw_Punct_Vertical(u16 x, u16 y, u16 light, u16 segment, u16 color);
void Draw_Punct_Horisontical(u16 x, u16 y, u16 light, u16 segment, u16 color);
//----------------------------------------------------------------------------------
u8 Draw_Char (u16 x, u16 y, u8 FontID, u8 Char, u16 TextColor, u16 BgColor, u8 TransparentBg);
u16 Draw_String (u16 x, u16 y, u8 FontID, char *Str, u16 TextColor, u16 BgColor, u8 TransparentBg);
u16 Draw_Number (u16 x, u16 y, u8 FontID, u16 Number, u16 TextColor, u16 BgColor, u8 TransparentBg);






























