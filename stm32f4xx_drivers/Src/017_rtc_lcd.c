#include <stdio.h>
#include "ds1307.h"
extern void initialise_monitor_handles();   

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
    current_date.year = 2026;
    current_date.day = MONDAY;

    current_time.hours = 10;
    current_time.minutes = 30;
    current_time.seconds = 0;
    current_time.time_format = TIME_FORMAT_24HRS;

    ds1307_set_current_date(&current_date);
    ds1307_set_current_time(&current_time);

    ds1307_get_current_date(&current_date);
    ds1307_get_current_time(&current_time);
    
    char *am_pm;
    if(current_time.time_format != TIME_FORMAT_24HRS)
    {
        if(current_time.time_format == TIME_FORMAT_12HRS_AM)
        {
            am_pm = "AM";
        }
        else
        {
            am_pm = "PM";
        }
        printf("Current time is : %02d:%02d:%02d %s \n", current_time.hours, current_time.minutes, current_time.seconds, am_pm);
    }
    else
    {
        printf("Current time is : %02d:%02d:%02d \n", current_time.hours, current_time.minutes, current_time.seconds);
    }
    
    printf("Current date is : %02d/%02d/%04d \n", current_date.date, current_date.month, current_date.year);

    return 0;
}

