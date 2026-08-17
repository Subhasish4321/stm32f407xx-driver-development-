#include <stdio.h>
#include <string.h>
#include "ds1307.h"
#include "lcd.h"
extern void initialise_monitor_handles();   

void delay(void)
{
    for(uint32_t i = 0 ; i < 500000 ; i ++);
}
void print_current_date_time(RTC_time_t *current_time,RTC_date_t *current_date);
#define GET_TODAY(args)  (args == 1)? "Sun":\
                         (args == 2)? "Mon":\
                         (args == 3)? "Tue":\
                         (args == 4)? "Wed":\
                         (args == 5)? "Thu":\
                         (args == 6)? "Fri":\
                         (args == 7)? "Sat":\
                         "Invalid day"

static void mdelay(uint32_t delay)
{
    for(uint32_t i=0;i<(delay*1000);i++);
}
int main(void)
{
    RTC_time_t current_time;
    RTC_date_t current_date;
   
    initialise_monitor_handles();
    printf("RTC test program\n");

    lcd_init();

    lcd_print_string("LCD testing..");
    mdelay(2000);

    lcd_display_clear();
    lcd_display_return_home();

    if(ds1307_init() == 1)
    {
        printf("RTC is has failed \n");
        while(1);
    }
    
    current_date.date = 16;
    current_date.month = 8;
    current_date.year = 26;
    current_date.day = MONDAY;

    current_time.hours = 2;
    current_time.minutes = 34;
    current_time.seconds = 0;
    current_time.time_format = TIME_FORMAT_12HRS_PM;

    ds1307_set_current_date(&current_date);
    ds1307_set_current_time(&current_time);
    
    delay();
    print_current_date_time(&current_time , &current_date);

    for(uint8_t i= 0 ;i < 10; i++)
    {
    	delay();
    }
    print_current_date_time(&current_time , &current_date);
    
    return 0;
}

void print_current_date_time(RTC_time_t *current_time,RTC_date_t *current_date)
{
	ds1307_get_current_date(current_date);
	ds1307_get_current_time(current_time);
    
    char time_str[12]; // HH:MM:SS AM/PM
    char date_str[13]; // DD/MM/YY Day
    /** @brief Convert time digits to ASCII chars by adding '0' (0x30) offset */
    time_str[0] = (current_time->hours / 10) + '0';
    time_str[1] = (current_time->hours % 10) + '0';
    time_str[2] = ':';
    time_str[3] = (current_time->minutes / 10) + '0';
    time_str[4] = (current_time->minutes % 10) + '0';
    time_str[5] = ':';
    time_str[6] = (current_time->seconds / 10) + '0';
    time_str[7] = (current_time->seconds % 10) + '0';
    
    char *am_pm;
    if(current_time->time_format != TIME_FORMAT_24HRS)
    {
        if(current_time->time_format == TIME_FORMAT_12HRS_AM)
        {
            am_pm = " AM";
        }
        else
        {
            am_pm = " PM";
        }
       
        // printf("Current time is : %02d:%02d:%02d %s \n", current_time->hours, current_time->minutes, current_time->seconds, am_pm);
        time_str[8] = '\0';
        strcat(time_str, am_pm);
        lcd_set_cursor(1,1);
        lcd_print_string(time_str);
        
    }
    else
    {
        time_str[8] = '\0';
        lcd_set_cursor(1,1);
        lcd_print_string(time_str);
    }
    
    date_str[0] = (current_date->date / 10) + '0';
    date_str[1] = (current_date->date % 10) + '0';
    date_str[2] = '/';
    date_str[3] = (current_date->month / 10) + '0';
    date_str[4] = (current_date->month % 10) + '0';
    date_str[5] = '/';
    date_str[6] = (current_date->year / 10) + '0';
    date_str[7] = (current_date->year % 10) + '0';
    date_str[8] = '\0';
    strcat(date_str, " ");
    strcat(date_str, GET_TODAY(current_date->day));
    lcd_set_cursor(2,1);
    lcd_print_string(date_str);
    // printf("Current day is : %s \n", GET_TODAY(current_date->day));
    // printf("Current date is : %02d/%02d/%02d \n", current_date->date, current_date->month, current_date->year);
}
