#ifndef LCD_H_
#define LCD_H_

#include "stm32f407xx.h"

/* Bsp exposed apis*/
void lcd_init(void);
void lcd_send_command(uint8_t command);
void lcd_send_char(uint8_t data);
void lcd_display_clear(void);
void lcd_display_return_home(void);
void lcd_set_cursor(uint8_t row, uint8_t column);
void lcd_print_string(char *str);

/*Application configuration items */
#define LCD_GPIO_PORT GPIOD
#define LCD_GPIO_RS GPIO_PIN_NO_0
#define LCD_GPIO_RW GPIO_PIN_NO_1
#define LCD_GPIO_EN GPIO_PIN_NO_2
#define LCD_GPIO_D4 GPIO_PIN_NO_3
#define LCD_GPIO_D5 GPIO_PIN_NO_4
#define LCD_GPIO_D6 GPIO_PIN_NO_5
#define LCD_GPIO_D7 GPIO_PIN_NO_6

/*LCD commands, Read page 23 and following pages from data sheet to know more.*/
#define LCD_CMD_4DL_2N_5X8F 0x28 /* 4-bit mode, 2 lines, 5x8 font */
#define LCD_CMD_DON_CURON 0x0E /* Display on, cursor on */
#define LCD_CMD_DON_CUROFF 0x0C /* Display on, cursor off */
#define LCD_CMD_DOFF_CURON 0x0A /* Display off, cursor on */
#define LCD_CMD_DOFF_CUROFF 0x08 /* Display off, cursor off */
#define LCD_CMD_INCADD 0x06 /* Increment cursor (shift cursor to right) */
#define LCD_CMD_DISP_CLEAR 0x01 /* Clear display */
#define LCD_CMD_DISP_RETURN_HOME 0x02 /* Return cursor to home position */

#endif /* LCD_H_ */