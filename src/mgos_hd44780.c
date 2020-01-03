#include "mgos_hd44780.h"

enum HD44780_PIN {
  HD44780_PIN_RS = 2,
  HD44780_PIN_E  = 4,
  HD44780_PIN_D4 = 15,
  HD44780_PIN_D5 = 13,
  HD44780_PIN_D6 = 12,
  HD44780_PIN_D7 = 14,
};


bool mgos_hd44780_init(void){
    return true;
}


void lcd_half_instruction( int register_select, int data_bit3_msb, int data_bit2, int data_bit1, int data_bit0_lsb ){
  mgos_usleep(5000);
  register_select = (register_select>=1);
  data_bit3_msb = (data_bit3_msb>=1);
  data_bit2 = (data_bit2>=1);
  data_bit1 = (data_bit1>=1);
  data_bit0_lsb = (data_bit0_lsb>=1);
  mgos_gpio_write( HD44780_PIN_E , 1 );
  mgos_gpio_write( HD44780_PIN_RS , register_select );
  mgos_gpio_write( HD44780_PIN_D7 , data_bit3_msb );
  mgos_gpio_write( HD44780_PIN_D6 , data_bit2 );
  mgos_gpio_write( HD44780_PIN_D5 , data_bit1 );
  mgos_gpio_write( HD44780_PIN_D4 , data_bit0_lsb );
  mgos_usleep(5000);
  mgos_gpio_write( HD44780_PIN_E , 0 );
}

void lcd_init(){
    mgos_msleep( 50 );
    mgos_gpio_set_mode( HD44780_PIN_RS , MGOS_GPIO_MODE_OUTPUT );
    mgos_gpio_set_mode( HD44780_PIN_E ,  MGOS_GPIO_MODE_OUTPUT );
    mgos_gpio_set_mode( HD44780_PIN_D4 , MGOS_GPIO_MODE_OUTPUT );
    mgos_gpio_set_mode( HD44780_PIN_D5 , MGOS_GPIO_MODE_OUTPUT );
    mgos_gpio_set_mode( HD44780_PIN_D6 , MGOS_GPIO_MODE_OUTPUT );
    mgos_gpio_set_mode( HD44780_PIN_D7 , MGOS_GPIO_MODE_OUTPUT );

    mgos_msleep( 50 );

    int NUMBER_OF_LINES = 1;
    int FONT = 0;

    // 1st function set
    lcd_half_instruction(0,0,0,1,0);

    // 2nd function set
    lcd_execute_instruction(0,0b00100000 + (NUMBER_OF_LINES&1)*8 + (FONT&1)*4 );

    // Default configurations
    lcd_display_controls(LCD_DISPLAY_ON,LCD_CURSOR_ON,LCD_BLINK_ON);
    lcd_entry_mode_set(LCD_DIRECTION_INCREMENT,LCD_DISPLAY_SHIFT_OFF);
    lcd_cursor_home();
    lcd_clear_display();
    mgos_msleep(50);

    LOG( LL_INFO, ("lcd_init done") );
}

void lcd_execute_instruction( int register_select, char c ){
    lcd_half_instruction( register_select, c&128, c&64, c&32, c&16 );
    lcd_half_instruction( register_select, c&8, c&4, c&2, c&1 );
}

void lcd_clear_display(){
    lcd_execute_instruction(0, 0b00000001 );
    mgos_msleep(50);
}
void lcd_cursor_home(){
    lcd_execute_instruction(0, 0b00000010 );
    mgos_msleep(50);
}

void lcd_display_controls( LCD_DISPLAY display , LCD_CURSOR cursor , LCD_BLINK blink ){
    lcd_execute_instruction( 0 , 0b00001000 + (display&1)*4 + (cursor&1)*2 + (blink&1) );
}
void lcd_entry_mode_set( LCD_DIRECTION dir , LCD_DISPLAY_SHIFT displ ){
    lcd_execute_instruction( 0 , 0b00000100 + (dir&1)*2 + (displ&1) );
}
void lcd_write( const char* text ){
    for( size_t i = 0 ; i < strlen(text) ; i++ ){
        lcd_execute_instruction(1,text[i]);
    }
}


int DISPLAY_HEIGHT = 4;
int get_line_address( int line_number ){
    if( DISPLAY_HEIGHT == 2 ){
        switch(line_number){
            case 0: return 0;
            case 1: return 40;
        }
    }
    else if( DISPLAY_HEIGHT == 4 ){
        switch(line_number){
            case 0: return 0;
            case 1: return 128;
            case 2: return 40;
            default: return 168;
        }
    }
    return 0;
}

void lcd_move_cursor( int x , int y ){
    int address = get_line_address(y) + x;
    lcd_execute_instruction( 0 , 128 + address&127 );
}