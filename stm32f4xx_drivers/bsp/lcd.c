#include "lcd.h"

static void lcd_enable(void);
static void write_4_bits(uint8_t data);
static void udelay(uint32_t delay);
static void mdelay(uint32_t delay);

/**
 * For sending commands to LCD ,the below steps to be followed:
 * 1. Create the command code.
 * 2. Set the RS pin low for command and RW pin low for write.
 * 3 First send higher nibble of the command code to data lines D4-D7.
 * 4.Make LCD EN pinhigh to low to latch the data.
 * 5. Then send lower nibble of the command code to data lines D4-D7.
 * 6. Make LCD EN pin high to low to latch the data.(When the LCD detects a high to low transition on the EN pin it reads the data from the data lines.)
 * 7.Wait for the instruction execution time before sending the next command or confirm the LCD is Not busy by reading the Busy flag status on D7 pin.
 * - D7 pin high means LCD is busy and D7 pin low means LCD is not busy.
 *
 */
void lcd_send_command(uint8_t command)
{
    //RS =0  for command , RW =0 for write
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
    
    //Send higher nibble of the command code to data lines D4-D7.
    write_4_bits(command >> 4);
    write_4_bits(command & 0x0F);

   //The waiting part is already taken care in the write_4_bits function by calling the lcd_enable function which has a delay of 100us after sending the data to the data lines.

}

void lcd_send_char(uint8_t data)
{
    //RS =1  for data , RW =0 for write
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_SET);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
    
    //Send higher nibble of the command code to data lines D4-D7.
    write_4_bits(data >> 4);
    write_4_bits(data & 0x0F);

   //The waiting part is already taken care in the write_4_bits function by calling the lcd_enable function which has a delay of 100us after sending the data to the data lines.

}

void lcd_print_string(char *str)
{
    while(*str != '\0')
    {
        lcd_send_char(*str++);
    }
}

void lcd_display_return_home(void)
{
    lcd_send_command(LCD_CMD_DISP_RETURN_HOME);
    mdelay(2);
}
void lcd_set_cursor(uint8_t row, uint8_t column)
{
    column--; // Decrement column to make it zero-based
    switch(row)
    {
        case 1:
            //set cursor to first row address that is 0x80
            lcd_send_command((column |= 0x80));
        break;
        case 2:
            //set cursor to second row address that is 0xC0
            lcd_send_command((column |= 0xC0));
        break;
        default:
        break;

    }

}
void lcd_init(void)
{
    //1. Initialize GPIO pins for LCD 
    GPIO_Handle_t lcd_signal;

    lcd_signal.pGPIOx = LCD_GPIO_PORT;
    lcd_signal.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_RS;
    lcd_signal.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    lcd_signal.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    lcd_signal.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_Init(&lcd_signal);
    
    lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_RW;
    GPIO_Init(&lcd_signal);

    lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_EN;
    GPIO_Init(&lcd_signal);

    lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D4;
    GPIO_Init(&lcd_signal);

    lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D5;
    GPIO_Init(&lcd_signal);

    lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D6;
    GPIO_Init(&lcd_signal);

    lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D7;
    GPIO_Init(&lcd_signal);
 
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D4, GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D5, GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D6, GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D7, GPIO_PIN_RESET);

    //2. DO the LCD initiaization. For understanding the initialization ,read the 4bit interface in the LCD datasheet.
    mdelay(40);

    //RS =0  for command , RW =0 for write
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

    //Write the command 0011 or 0x3 to the data pins D4-D7
    write_4_bits(0x3);
    //wait for 5ms
    mdelay(5);
    //Write the command 0011 or 0x3 to the data pins D4-D7
    write_4_bits(0x3);

    udelay(150);

    write_4_bits(0x3);
    write_4_bits(0x2);

    //function set command 0x28 for 4bit interface, 2 lines and 5x8 font.
    lcd_send_command(LCD_CMD_4DL_2N_5X8F );

    //lcd display on/off control command 0x0E for display on and cursor on.
    lcd_send_command(LCD_CMD_DON_CURON);

    //Display clear
    lcd_display_clear();
    
    //entry mode set command 0x06 for increment cursor and no display shift.
    lcd_send_command(LCD_CMD_INCADD);
}
//lsb goes to db4 and msb goes to db7
static void write_4_bits(uint8_t data)
{
    //Write the data to the data pins D4-D7
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D4, (data & 0x01));
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D5, (data & 0x02));
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D6, (data & 0x04));
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D7, (data & 0x08));

    //lcd enable ,is done after every data write to the data pins to latch. This is done by setting the EN pin high for 1ms and then setting it low.
    lcd_enable();
}

void lcd_display_clear(void)
{
    lcd_send_command(LCD_CMD_DISP_CLEAR);
    mdelay(2);
}
static void lcd_enable(void)
{
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_SET);
    udelay(10);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
    udelay(100);
}

static void mdelay(uint32_t delay)
{
    for(uint32_t i=0;i<(delay*1000);i++);
}

static void udelay(uint32_t delay)
{
    for(uint32_t i=0;i<(delay*1);i++);
}