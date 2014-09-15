#include "lcd.h"
#include "Arduino.h"
#include <string.h>
#define LCD_FONT_SMALL 0x00
void LCD::init ( uint8_t rs, uint8_t en, uint8_t rw, uint8_t d0,
				 uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4,
				 uint8_t d5, uint8_t d6, uint8_t d7 )
{
	_data_pins[0] = d0;
	_data_pins[1] = d1;
	_data_pins[2] = d2;
	_data_pins[3] = d3;
	_data_pins[4] = d4;
	_data_pins[5] = d5;
	_data_pins[6] = d6;
	_data_pins[7] = d7;

	_rs_pin = rs;
	_rw_pin = rw;
	_en_pin = en;

	// set pin modes to OUTPUT
	for (int i = 0; i < sizeof(_data_pins) / sizeof(uint8_t); ++i) {
		pinMode(_data_pins[i], OUTPUT);
	}
	pinMode(_rs_pin, OUTPUT);
	pinMode(_rw_pin, OUTPUT);
	pinMode(_en_pin, OUTPUT);

	// apply default settings
	_print_direction = LCD_PRINT_L2R;
	_shift_on_write = LCD_NO_SHIFT;
	_display = LCD_DISPLAY_OFF;
	_cursor = LCD_CURSOR_OFF;
	_cursor_blink = LCD_CURSOR_BLINK_OFF;
	_rows = LCD_ROWS_TWO;
	_font = LCD_FONT_SMALL;

	_entry_mode();
	_display_control();
	_fn_set();
}

void LCD::begin ( lcd_param_t rows, lcd_param_t font )
{
	// set row and font size
	_rows = rows == LCD_ROWS_TWO ? LCD_ROWS_TWO : LCD_ROWS_ONE;
	_font = font == LCD_FONT_LARGE ? LCD_FONT_LARGE : LCD_FONT_SMALL;
	_fn_set();

	// erase screen
	erase();
}

void LCD::erase ( )
{
	command(LCD_CMD_ERASE);
	home();
}

void LCD::home ( )
{
	command(LCD_CMD_RETURN);
}

void LCD::shift ( int8_t times )
{
	_shift_setting = LCD_SHIFT_DISPLAY_RIGHT;

	if (times < 0) {
		times = -times;
		_shift_setting = LCD_SHIFT_DISPLAY_LEFT;
	}

	while (times > 0) {
		_shift();
		--times;
	}
}

void LCD::shiftRight ( )
{
	_shift_setting = LCD_SHIFT_DISPLAY_RIGHT;
	_shift();
}

void LCD::shiftLeft ( )
{
	_shift_setting = LCD_SHIFT_DISPLAY_LEFT;
}

void LCD::next ( )
{
	_shift_setting = _print_direction == LCD_PRINT_L2R ?
		LCD_SHIFT_CURSOR_RIGHT :
		LCD_SHIFT_CURSOR_LEFT;
	_shift();
}

void LCD::prev ( )
{
	_shift_setting = _print_direction == LCD_PRINT_L2R ?
		LCD_SHIFT_CURSOR_LEFT :
		LCD_SHIFT_CURSOR_RIGHT;
	_shift();
}

void LCD::right ( )
{
	_shift_setting = LCD_SHIFT_CURSOR_RIGHT;
	_shift();
}

void LCD::left ( )
{
	_shift_setting = LCD_SHIFT_CURSOR_LEFT;
	_shift();
}

void LCD::setOneLine ( )
{
	_rows = LCD_ROWS_ONE;
	_fn_set();
}

void LCD::setTwoLines ( )
{
	_rows = LCD_ROWS_TWO;
	_fn_set();
}

void LCD::setSmallFont ( )
{
	_font = LCD_FONT_SMALL;
	_fn_set();
}

void LCD::setLargeFont ( )
{
	_font = LCD_FONT_LARGE;
	_fn_set();
}

void LCD::setShift ( )
{
	_shift_on_write = LCD_SHIFT;
	_entry_mode();
}

void LCD::unsetShift ( )
{
	_shift_on_write = LCD_NO_SHIFT;
	_entry_mode();
}

void LCD::setForward ( )
{
	_print_direction = LCD_PRINT_L2R;
	_entry_mode();
}

void LCD::setReverse ( )
{
	_print_direction = LCD_PRINT_R2L;
	_entry_mode();
}

void LCD::_fn_set ( )
{
	command(LCD_CMD_FN_SET | _rows | _font);
}

void LCD::_entry_mode ( )
{
	command(LCD_CMD_ENTRY_MODE | _print_direction | _shift_on_write);
}

void LCD::_display_control ( )
{
	command(LCD_CMD_DISPLAY | _display | _cursor | _cursor_blink);
}

void LCD::_shift ( )
{
	command(LCD_CMD_SHIFT | _shift_setting);
}

void LCD::print ( char *str )
{
	unsigned int len = strlen(str);

	// write to RAM
	pinMode(_rs_pin, OUTPUT);
	pinMode(_rw_pin, OUTPUT);
	digitalWrite(_rs_pin, HIGH);
	digitalWrite(_rw_pin, HIGH);

	for (unsigned int i = 0; i < len; ++i) {
		send(str[i]);
	}
}

void LCD::command ( uint8_t cmd )
{
	// write to command register
	pinMode(_rs_pin, OUTPUT);
	pinMode(_rw_pin, OUTPUT);
	digitalWrite(_rs_pin, LOW);
	digitalWrite(_rw_pin, HIGH);

	send(cmd);
}

void LCD::send ( uint8_t value )
{
	// set data pins
	for (int i = 0; i < sizeof(_data_pins) / sizeof(uint8_t); ++i) {
		pinMode(_data_pins[i], OUTPUT);
		digitalWrite(_data_pins[i], (value >> i) & 0x01);
	}

	// pulse enable
	digitalWrite(_en_pin, LOW);
	delay(1);
	digitalWrite(_en_pin, HIGH);
	delay(1);
	digitalWrite(_en_pin, LOW);
	delay(100);
}
