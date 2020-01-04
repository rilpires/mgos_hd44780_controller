# HD44780 library for Mongoose OS
A library used to control a HD44780 LCD display in a MongooseOS project, without an I2C module that is commonly attached to the LCD.
It should use 6 GPIO pins in total, that should be mapped in config_schema like this example:

```yaml
  - ["HD44780.GPIO.D7" , 14 ]
  - ["HD44780.GPIO.D6" , 12 ]
  - ["HD44780.GPIO.D5" , 13 ]
  - ["HD44780.GPIO.D4" , 15 ]
  - ["HD44780.GPIO.E"  , 4  ] 
  - ["HD44780.GPIO.RS" , 2  ]
  - ["HD44780.PHYSICAL_NUMBER_OF_LINES" , 4 ]
```
Also, <code>HD44780.PHYSICAL_NUMBER_OF_LINES</code> is expected to be 2(default) or 4. I don't think there is models with a different number of lines.

### About the other LCD pins:
- pins D0,D1,D2 and D3 are not necessary since this library uses the 4-bit interface mode.
- pin R/W should be low (GND) all the time, since this library doesn't support reading commands from display

## Configuration

```yaml
config_schema:
  - ["HD44780" , "o" , {"title": "HD44780 configuration schema"}]
  - ["HD44780.PHYSICAL_NUMBER_OF_LINES" , "i" , 2 , {"title": "HD44780 model available number of lines. Expected 2 or 4."}]
  - ["HD44780.GPIO" , "o" , {"title": "ESP8266 GPIO pins used in HD44780's pins"}]
  - ["HD44780.GPIO.D7" , "i" , 14 , {"title": "ESP8266's GPIO pin used in HD44780's D7 pin"}]
  - ["HD44780.GPIO.D6" , "i" , 12 , {"title": "ESP8266's GPIO pin used in HD44780's D6 pin"}]
  - ["HD44780.GPIO.D5" , "i" , 13 , {"title": "ESP8266's GPIO pin used in HD44780's D5 pin"}]
  - ["HD44780.GPIO.D4" , "i" , 15 , {"title": "ESP8266's GPIO pin used in HD44780's D4 pin"}]
  - ["HD44780.GPIO.E" , "i" , 4  , {"title": "ESP8266's GPIO pin used in HD44780's E pin"}]
  - ["HD44780.GPIO.RS" , "i" , 2 , {"title": "ESP8266's GPIO pin used in HD44780's RS pin"}]
```

## Example
This is a simple C program example using a 4-line model.
Don't forget to set <code>HD44780.PHYSICAL_NUMBER_OF_LINES"</code> config equal to 4 in this case.

![LCD Example](https://i.ibb.co/StwptSJ/photo-2020-01-04-17-17-16.jpg)

### main.c
```c
#include "mgos.h"
#include "mgos_hd44780.h"

enum mgos_app_init_result mgos_app_init(void) {
    
    lcd_write("First line!");

    lcd_move_cursor(19,1); // Moves to the final of the second line
    lcd_entry_mode_set( LCD_DIRECTION_DECREMENT , LCD_DISPLAY_SHIFT_OFF );
    lcd_write("Second line?"); // Writes "Second line?" backwards because entry mode decrementing cursor 

    lcd_move_cursor(0,2); // Moves to the beggining of the third line
    lcd_entry_mode_set( LCD_DIRECTION_INCREMENT , LCD_DISPLAY_SHIFT_OFF );
    
    // Writes 1 2 3, spaced because cursor shifting
    lcd_write("1");
    lcd_shift_cursor(LCD_SHIFT_RIGHT);
    lcd_write("2");
    lcd_shift_cursor(LCD_SHIFT_RIGHT);
    lcd_write("3");

    lcd_move_cursor(9,3); // Moves to the middle of the fourth line
    lcd_write("Bye...");

    lcd_move_cursor(0,3);

    return MGOS_APP_INIT_SUCCESS;
}
```

### mos.yml
```yaml

config_schema: 
  - ["HD44780.PHYSICAL_NUMBER_OF_LINES" , 4 ]

libs:
  - origin: https://github.com/rilpires/mgos_hd44780_controller
    name: hd44780
    
manifest_version: 2017-05-18
    
```

## Notes
Don't forget to add:
``` yaml
  name: hd44780
```
in libs section.

There is some functionalities yet to implement:
- Set the number of lines used to half of the available lines. I don't think this is useful at all but it is necessary in order to use the 5x10 font (it is basically the same font with an underline below it)
- Implement reading commands from the display. It would be necessary an additional GPIO pin.
- Implement creating custom characters.
