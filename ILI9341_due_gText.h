/*
ILI9341_due_gText.cpp - Used for rendering GLCD fonts on Arduino Due

Copyright (c) 2014  Marek Buriak

The library is based on GLCD library by Bill Perry and Michael Margolis: 
https://code.google.com/p/glcd-arduino

This file is part of the Arduino ILI9341_due library. 
Sources for this library can be found at https://github.com/marekburiak/ILI9341_Due.

Fonts .h files can be created with GLCDFontCreator2 (not the one MikroElektronika):
https://code.google.com/p/glcd-arduino/downloads/detail?name=GLCDFontCreator2.zip&can=2&q=


ILI9341_due is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2.1 of the License, or
(at your option) any later version.

ILI9341_due is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with ILI9341_due.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef	GTEXT_H
#define GTEXT_H

#include "WString.h"
//#include "../Streaming/Streaming.h" 
#include "ILI9341_due.h"

#define INCLUDE_NOT_WORKING_YET 0

#define GTEXT_VERSION 1 // software version of this code

// Font Indices
#define FONT_LENGTH			0
#define FONT_FIXED_WIDTH	2
#define FONT_HEIGHT			3
#define FONT_FIRST_CHAR		4
#define FONT_CHAR_COUNT		5
#define FONT_WIDTH_TABLE	6

#define DRAW_DIRECTION_RIGHT 0
#define DRAW_DIRECTION_DOWN 1
#define DRAW_DIRECTION_LEFT 2
#define DRAW_DIRECTION_UP 3

#define FONT_MODE_SOLID 0
#define FONT_MODE_TRANSPARENT 1

// the following returns true if the given font is fixed width
// zero length is flag indicating fixed width font (array does not contain width data entries)
#define isFixedWidthFont(font)  (pgm_read_byte(font+FONT_LENGTH) == 0 && pgm_read_byte(font+FONT_LENGTH+1) == 0))

/*
* Coodinates for predefined areas are compressed into a single 32 bit token.
*
* This works as the coordinates are cuurenly only 8 bit values.
*
* This allows the creatation of an unlmited number of predefined areas with zero code or
* data space overhead.
* 
* A macro is used to create the tokens from a set of x1,y1 x2,y2 coordinates.
*
* A union is used to extract the coordinates from the 32 bit token.
*
* WARNING:
*	This is non portable code in that it will only work on little endian processors.
*	If you use big endian you have to switch the byte ordering in the union.
*/

#define MK_TareaToken(x1, y1, x2, y2) \
	(uint32_t) (((uint32_t) (x1) << 24) | ((uint32_t)(y1) << 16) | ((uint32_t)(x2) << 8) | (uint32_t)(y2))

/// @cond hide_from_doxygen
typedef union
{       
	struct
	{      
		uint8_t y2;
		uint8_t x2;
		uint8_t y1;
		uint8_t x1;
	}coord;

	uint32_t token; // swap byte order above for big endian

}TareaToken;
/// @endcond

typedef uint8_t textMode;  // type holding mode for scrolling and future attributes like padding etc.
// the only textMode supported in the current release is scrolling

const textMode SCROLL_UP = 0;
const textMode SCROLL_DOWN = 1; // this was changed from -1 so it can used in a bitmask 
const textMode DEFAULT_SCROLLDIR = SCROLL_UP;

/**
* @defgroup glcd_enum GLCD enumerations
*/

/**
* @ingroup glcd_enum
* @hideinitializer
* @brief Pre Defined Text areas
*
* These enumerations are used to easily define text areas 
* using predefined display areas.\n
* They are used with the
* \ref gText::DefineArea(predefinedArea selection, textMode mode) "DefineArea()" function call.
*/
typedef enum  {

	//textAreaFULL         = MK_TareaToken( 0,               0,                DISPLAY_WIDTH -1,   DISPLAY_HEIGHT -1   ),
	///**<Entire GLCD display */

	//textAreaTOP          = MK_TareaToken( 0,               0,                DISPLAY_WIDTH -1,   DISPLAY_HEIGHT/2 -1 ),
	///**<Top half of GLCD display */

	//textAreaBOTTOM       = MK_TareaToken( 0,               DISPLAY_HEIGHT/2, DISPLAY_WIDTH -1,   DISPLAY_HEIGHT -1   ),
	///**<Bottom half of GLCD display */

	//textAreaLEFT         = MK_TareaToken( 0,               0,                DISPLAY_WIDTH/2 -1, DISPLAY_HEIGHT -1   ),
	///**<Left side of GLCD display */

	//textAreaRIGHT        = MK_TareaToken( DISPLAY_WIDTH/2, 0,                DISPLAY_WIDTH -1,   DISPLAY_HEIGHT -1   ),
	///**<Right side of GLCD display */

	//textAreaTOPLEFT      = MK_TareaToken( 0,               0,                DISPLAY_WIDTH/2 -1, DISPLAY_HEIGHT/2 -1 ),
	///**<Upper left quarter of GLCD display */

	//textAreaTOPRIGHT     = MK_TareaToken( DISPLAY_WIDTH/2, 0,                DISPLAY_WIDTH -1,   DISPLAY_HEIGHT/2 -1 ),
	///**<Upper right quarter of GLCD display */

	//textAreaBOTTOMLEFT   = MK_TareaToken( 0,               DISPLAY_HEIGHT/2, DISPLAY_WIDTH/2 -1, DISPLAY_HEIGHT -1   ),
	///**<Buttom left quarter of GLCD display */

	//textAreaBOTTOMRIGHT  = MK_TareaToken( DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, DISPLAY_WIDTH -1,   DISPLAY_HEIGHT -1   )
	///**<Bottom right quarter of GLCD display */

} predefinedArea;



/*
* enums for ansi style erase function
* These values match the ANSI EraseInLine terminal primitive: CSI n K
*/  
/**
* @ingroup glcd_enum
* @hideinitializer
* @brief Erase line options
* @details
* These enumerations are used with the
* \ref gText::EraseTextLine( eraseLine_t type) "EraseTextLine()" function call.\n
* They provide similar functionality to the
* ANSI EraseInLine terminal primitive: CSI n K
*
*/
enum eraseLine_t {
	eraseTO_EOL, 	/**< Erase From cursor to end of Line */
	eraseFROM_BOL,	/**< Erase From Begining of Line to Cursor*/
	eraseFULL_LINE	/**< Erase Entire line */
};

typedef const uint8_t* Font_t;  	
//typedef uint8_t (*FontCallback)(Font_t);


//static glcd_Device    *device;              // static pointer to the device instance

/// @cond hide_from_doxygen
typedef struct _area
{
	uint16_t x1;
	uint16_t y1;
	uint16_t x2;
	uint16_t y2;
	int8_t  mode;
} area;
/// @endcond

/**
* @class gText
* @brief Functions for Text Areas
* @details
* A text area acts like a terminal monitor and text output is displayed 
* within the confines of a rectangle given in the DefineArea command.
* All of the following text area functions operate on a user defined text area.
*/

// graphical device text routines
class ILI9341_due_gText : public Print
{
private:
	ILI9341_due *_ili;
	//FontCallback	ucg_pgm_read;     // now static, move back here if each instance needs its own callback
	uint16_t _fontColor;
	uint16_t _fontBgColor;
	Font_t _font;
	area _area;
	int16_t	_x;
	int16_t	_y;
	uint8_t _scale;
	uint8_t _letterSpacing;
	uint8_t _fontMode;
#ifndef GLCD_NODEFER_SCROLL
	uint8_t			_needScroll; // set when text scroll has been defered
#endif
	void specialChar(uint8_t c);
	void drawSolidChar(char c, uint16_t index, uint16_t charWidth, uint16_t charHeight);
	void drawTransparentChar(char c, uint16_t index, uint16_t charWidth, uint16_t charHeight);
	// Scroll routines are private for now
	//void ScrollUp(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t pixels, uint8_t color);
	//void ScrollDown(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t pixels, uint8_t color);

public:
	ILI9341_due_gText(ILI9341_due *ili); // default - uses the entire display
	ILI9341_due_gText(ILI9341_due *ili, int16_t x1, int16_t y1, int16_t x2, int16_t y2); //, textMode mode=DEFAULT_SCROLLDIR);
	
	ILI9341_due_gText(ILI9341_due *ili, int16_t x1, int16_t y1, int16_t columns, int16_t rows, Font_t font); //, textMode mode=DEFAULT_SCROLLDIR);
	
	
	bool defineArea(int16_t x1, int16_t y1, int16_t x2, int16_t y2); //, textMode mode=DEFAULT_SCROLLDIR);
	bool defineArea(int16_t x1, int16_t y1, int16_t columns, int16_t rows, Font_t font); //, textMode mode=DEFAULT_SCROLLDIR);
	void clearArea(uint16_t color);

	// Font Functions
	void selectFont(Font_t font);
	void selectFont(Font_t font, uint16_t color);
	void setFontColor(uint16_t color);
	void setFontColor(uint8_t R, uint8_t G, uint8_t B);
	void setFontColor(uint16_t color, uint16_t backgroundColor);
	void setFontColor(uint8_t R, uint8_t G, uint8_t B, uint8_t bgR, uint8_t bgG, uint8_t bgB);
	void setFontLetterSpacing(uint8_t letterSpacing);
	void setFontMode(uint8_t fontMode);
	
	int putChar(uint8_t c);
	void puts(char *str);
	void puts(const String &str); // for Arduino String Class
	void puts_P(PGM_P str);
	void drawString(char *str, int16_t x, int16_t y);
	void drawString(String &str, int16_t x, int16_t y); // for Arduino String class
	void drawString_P(PGM_P str, int16_t x, int16_t y);

	size_t write(uint8_t c);  // character output for print base class

	void cursorTo( uint8_t column, uint8_t row); // 0 based coordinates for character columns and rows
	void cursorTo( int8_t column); // move cursor on the current row
	void cursorToXY( int16_t x, int16_t y); // coordinates relative to active text area

	uint16_t charWidth(uint8_t c);
	uint16_t stringWidth(const char* str);
	uint16_t stringWidth_P(PGM_P str);
	uint16_t stringWidth_P(String &str);

	void eraseTextLine(uint16_t color, eraseLine_t type=eraseTO_EOL); //ansi like line erase function 
	void eraseTextLine(uint16_t color, uint8_t row); // erase the entire text line in the given row and move cursor to left position

	void printNumber(long n);

#if INCLUDE_NOT_WORKING_YET
	ILI9341_due_gText(ILI9341_due *ili, predefinedArea selection); //, textMode mode=DEFAULT_SCROLLDIR);
	bool defineArea(predefinedArea selection); //, textMode mode=DEFAULT_SCROLLDIR);
	void setTextMode(textMode mode); // change to the given text mode
#endif

	//#ifndef USE_ARDUINO_FLASHSTR	
	//	// when the following function is supported in arduino it will be removed from this library
	//	void printFlash(FLASHSTRING str); //this overrides the Arduino print function to implicilty store the string in flash (progmem)
	//    void printFlashln(FLASHSTRING str);
	//#endif

	/*@}*/

};

#endif