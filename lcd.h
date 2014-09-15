#ifndef LiquidCrystal_h
#define LiquidCrystal_h

#include <inttypes.h>

#define LCD_CMD_ERASE 0x01
#define LCD_CMD_RETURN 0x02
#define LCD_CMD_ENTRY_MODE 0x04
#define LCD_CMD_DISPLAY 0x08
#define LCD_CMD_SHIFT 0x10
#define LCD_CMD_FN_SET 0x30
#define LCD_CMD_DDRAM_ADDR 0x80

// ENTRY_MODE params
#define LCD_PRINT_L2R 0x00
#define LCD_PRINT_R2L 0x02
#define LCD_NO_SHIFT 0x00
#define LCD_SHIFT 0x01

// FN_SET params
#define LCD_FONT_SMALL 0x00
#define LCD_FONT_LARGE 0x04
#define LCD_ROWS_ONE 0x00
#define LCD_ROWS_TWO 0x08

// DISPLAY params
#define LCD_DISPLAY_OFF 0x00
#define LCD_DISPLAY_ON 0x04
#define LCD_CURSOR_OFF 0x00
#define LCD_CURSOR_ON 0x02
#define LCD_CURSOR_BLINK_OFF 0x00
#define LCD_CURSOR_BLINK_ON 0x01

// SHIFT params
#define LCD_SHIFT_CURSOR_LEFT 0x00
#define LCD_SHIFT_CURSOR_RIGHT 0x01
#define LCD_SHIFT_DISPLAY_LEFT 0x02
#define LCD_SHIFT_DISPLAY_RIGHT 0x03

typedef uint8_t lcd_param_t;

class LCD {
public:
	// initialization and control
	void init ( uint8_t rs, uint8_t en, uint8_t rw, uint8_t d0,
			  uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4,
			  uint8_t d5, uint8_t d6, uint8_t d7 );
	void begin ( lcd_param_t rows, lcd_param_t font );
	void on ( );
	void off ( );

	// LCD output
	void print ( char *str );
	void erase ( );
	void shift ( int8_t );
	void shiftLeft ( );
	void shiftRight ( );

	// cursor movement
	void home ( );
	void next ( );
	void prev ( );
	void right ( );
	void left ( );
	void moveTo ( uint8_t row, uint8_t col );

	// configuration
	void setForward ( );
	void setReverse ( );
	void setOneLine ( );
	void setTwoLines ( );
	void setSmallFont ( );
	void setLargeFont ( );
	void setCursorBlink ( );
	void unsetCursorBlink ( );
	void setCursor ( );
	void unsetCursor ( );
	void setInPlace ( );
	void unsetInPlace ( );
	void setShift ( );
	void unsetShift ( );

	// direct manipulation
	void send ( uint8_t value );
	void command ( lcd_param_t cmd );


private:
	// pins
	uint8_t _data_pins[8];
	uint8_t _rs_pin;
	uint8_t _rw_pin;
	uint8_t _en_pin;

	// configuration parameters
	lcd_param_t _print_direction;
	lcd_param_t _rows;
	lcd_param_t _font;
	lcd_param_t _shift_on_write;
	lcd_param_t _display;
	lcd_param_t _cursor;
	lcd_param_t _cursor_blink;
	lcd_param_t _shift_setting;

	// configuration helpers
	void _entry_mode ( );
	void _display_control ( );
	void _fn_set ( );
	void _shift ( );
};

#endif
