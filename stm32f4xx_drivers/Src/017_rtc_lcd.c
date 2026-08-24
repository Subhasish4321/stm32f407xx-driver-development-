#include <stdio.h>
#include "ds1307.h"
#define SYSTICK_TIM_CLK 16000000
void delay(void)
{
    for(uint32_t i = 0 ; i < 500000 ; i ++);
}
void print_current_date_time(RTC_time_t *current_time,RTC_date_t *current_date);

extern void initialise_monitor_handles();
#define GET_TODAY(args)  (args == 1)? "Sunday":\
                         (args == 2)? "Monday":\
                         (args == 3)? "Tuesday":\
                         (args == 4)? "Wednesday":\
                         (args == 5)? "Thursday":\
                         (args == 6)? "Friday":\
                         (args == 7)? "Saturday":\
                         "Invalid day"

volatile uint8_t one_second_elapsed = 0;
void init_systick_timer(uint32_t tick_hz)
{
	uint32_t *pSRVR = (uint32_t*)0xE000E014;
	uint32_t *pSCSR = (uint32_t*)0xE000E010;

    /* calculation of reload value */
    uint32_t count_value = (SYSTICK_TIM_CLK/tick_hz)-1;

    //Clear the value of SVR
    *pSRVR &= ~(0x00FFFFFFFF);

    //load the value in to SVR
    *pSRVR |= count_value;

    //do some settings
    *pSCSR |= ( 1 << 1); //Enables SysTick exception request:
    *pSCSR |= ( 1 << 2);  //Indicates the clock source, processor clock source

    //enable the systick
    *pSCSR |= ( 1 << 0); //enables the counter

}

int main(void)
{
    RTC_time_t current_time;
    RTC_date_t current_date;
    initialise_monitor_handles();
    printf("RTC test program\n");
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

    init_systick_timer(1);
    while(1)
    {
       if(one_second_elapsed)
       {
    	   one_second_elapsed = 0;

    	   ds1307_get_current_date(&current_date);
    	   ds1307_get_current_time(&current_time);

    	   print_current_date_time(&current_time, &current_date);
       }

    }
    return 0;
}

void SysTick_Handler(void)
{
	one_second_elapsed = 1;
}

void print_current_date_time(RTC_time_t *current_time,RTC_date_t *current_date)
{
    char *am_pm;
    if(current_time->time_format != TIME_FORMAT_24HRS)
    {
        if(current_time->time_format == TIME_FORMAT_12HRS_AM)
        {
            am_pm = "AM";
        }
        else
        {
            am_pm = "PM";
        }
        printf("Current time is : %02d:%02d:%02d %s \n", current_time->hours, current_time->minutes, current_time->seconds, am_pm);

    }
    else
    {
        printf("Current time is : %02d:%02d:%02d \n", current_time->hours, current_time->minutes, current_time->seconds);
    }
    printf("Current date is : %02d/%02d/%02d ,%s \n", current_date->date, current_date->month, current_date->year,GET_TODAY(current_date->day));
}
