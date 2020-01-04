#include "mgos_hd44780.h"

bool mgos_hd44780_init(void){
    const int NUMBER_OF_LINES = 1; 
    const int FONT = 0;

    mgos_msleep( 50 );
    mgos_gpio_set_mode( mgos_sys_config_get_HD44780_GPIO_RS() , MGOS_GPIO_MODE_OUTPUT );
    mgos_gpio_set_mode( mgos_sys_config_get_HD44780_GPIO_E() ,  MGOS_GPIO_MODE_OUTPUT );
    mgos_gpio_set_mode( mgos_sys_config_get_HD44780_GPIO_D4() , MGOS_GPIO_MODE_OUTPUT );
    mgos_gpio_set_mode( mgos_sys_config_get_HD44780_GPIO_D5() , MGOS_GPIO_MODE_OUTPUT );
    mgos_gpio_set_mode( mgos_sys_config_get_HD44780_GPIO_D6() , MGOS_GPIO_MODE_OUTPUT );
    mgos_gpio_set_mode( mgos_sys_config_get_HD44780_GPIO_D7() , MGOS_GPIO_MODE_OUTPUT );
    mgos_msleep( 50 );
    lcd_half_instruction(0,0,0,1,0); // <--- Now it's interface is 4bit
    lcd_execute_instruction(0,0b00100000 + (NUMBER_OF_LINES&1)*8 + (FONT&1)*4 );

    // Default configurations
    lcd_display_controls(LCD_DISPLAY_ON,LCD_CURSOR_ON,LCD_BLINK_ON);
    lcd_entry_mode_set(LCD_DIRECTION_INCREMENT,LCD_DISPLAY_SHIFT_OFF);
    lcd_cursor_home();
    lcd_clear_display();
    mgos_msleep(50);

    LOG( LL_INFO, ("mgos_hd44780_init done") );
    return true;
}

void lcd_half_instruction( int register_select, int data_bit3_msb, int data_bit2, int data_bit1, int data_bit0_lsb ){
  mgos_usleep(500);
  mgos_gpio_write( mgos_sys_config_get_HD44780_GPIO_E() , 1 );
  mgos_gpio_write( mgos_sys_config_get_HD44780_GPIO_RS() , register_select>=1 );
  mgos_gpio_write( mgos_sys_config_get_HD44780_GPIO_D7() , data_bit3_msb>=1 );
  mgos_gpio_write( mgos_sys_config_get_HD44780_GPIO_D6() , data_bit2>=1 );
  mgos_gpio_write( mgos_sys_config_get_HD44780_GPIO_D5() , data_bit1>=1 );
  mgos_gpio_write( mgos_sys_config_get_HD44780_GPIO_D4() , data_bit0_lsb>=1 );
  mgos_usleep(500);
  mgos_gpio_write( mgos_sys_config_get_HD44780_GPIO_E() , 0 );
}

void lcd_execute_instruction( int register_select, char c ){
    lcd_half_instruction( register_select, c&128, c&64, c&32, c&16 );
    lcd_half_instruction( register_select, c&8, c&4, c&2, c&1 );
}

void lcd_clear_display(){
    lcd_execute_instruction(0, 0b00000001 );
    mgos_msleep(5);
}
void lcd_cursor_home(){
    lcd_execute_instruction(0, 0b00000010 );
    mgos_msleep(5);
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


void lcd_move_cursor( int x , int y ){
    int line_address = 0;
    int physical_number_of_lines = mgos_sys_config_get_HD44780_PHYSICAL_NUMBER_OF_LINES();
    y = y % physical_number_of_lines;
    if( physical_number_of_lines == 2 ){
        if( y == 0 )    line_address = 0;
        else            line_address = 64;
    }
    else if( physical_number_of_lines == 4 ){
        if( y == 0 )    line_address = 0;
        else if (y==1)  line_address = 64;
        else if (y==2)  line_address = 20;
        else            line_address = 84;
    }
    int address = line_address + x;
    lcd_execute_instruction( 0 , 0b10000000 + address%128 );
}

void lcd_shift_cursor( LCD_SHIFT direction ){
    lcd_execute_instruction( 0 , 0b00010000 + direction*0b100 );
}

void lcd_shift_display( LCD_SHIFT direction ){
    lcd_execute_instruction( 0 , 0b00011000 + direction*0b100 );
}
