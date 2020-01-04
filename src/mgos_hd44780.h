#ifndef HD44780_H
#define HD44780_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgos.h"
#include "mgos_gpio.h"
#include "mgos_config.h"

typedef enum {
    LCD_DIRECTION_DECREMENT = 0,
    LCD_DIRECTION_INCREMENT = 1
} LCD_DIRECTION;
typedef enum {
    LCD_DISPLAY_SHIFT_OFF = 0,
    LCD_DISPLAY_SHIFT_ON = 1
} LCD_DISPLAY_SHIFT;
typedef enum {
    LCD_DISPLAY_OFF = 0,
    LCD_DISPLAY_ON = 1
} LCD_DISPLAY;
typedef enum {
    LCD_CURSOR_OFF = 0,
    LCD_CURSOR_ON = 1
} LCD_CURSOR;
typedef enum {
    LCD_BLINK_OFF = 0,
    LCD_BLINK_ON = 1
} LCD_BLINK;
typedef enum {
    LCD_SHIFT_LEFT = 0,
    LCD_SHIFT_RIGHT = 1
} LCD_SHIFT;


void lcd_half_instruction( int register_select, int bit3_msb , int bit2 , int bit1 , int bit0_lsb );
void lcd_execute_instruction( int register_select, char c );

void lcd_clear_display();
void lcd_cursor_home();
void lcd_display_controls( LCD_DISPLAY display , LCD_CURSOR cursor , LCD_BLINK blink );
void lcd_entry_mode_set( LCD_DIRECTION dir , LCD_DISPLAY_SHIFT displ );
void lcd_write( const char* text );
void lcd_move_cursor( int x , int y );
void lcd_shift_cursor( LCD_SHIFT direction );
void lcd_shift_display( LCD_SHIFT direction );

#ifdef __cplusplus
}
#endif

#endif