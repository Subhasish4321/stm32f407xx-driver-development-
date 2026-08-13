#ifndef DS1307_H_
#define DS1307_H_
#include <stdint.h>

/* Application configurable items */
#

/**
 * Register addresses for the DS1307 RTC chip.
 */
#define DS1307_ADDR_SEC 0x00
#define DS1307_ADDR_MIN 0x01
#define DS1307_ADDR_HOUR 0x02
#define DS1307_ADDR_DAY 0x03
#define DS1307_ADDR_DATE 0x04
#define DS1307_ADDR_MONTH 0x05
#define DS1307_ADDR_YEAR 0x06

#define TIME_FORMAT_12HRS_AM 0
#define TIME_FORMAT_12HRS_PM 1
#define TIME_FORMAT_24HRS 2

#define DS1307_I2C_ADDRESS 0x68  /* 0x68 = 1101000b  as mentioned in data sheet */

#define SUNDAY 1
#define MONDAY 2
#define TUESDAY 3
#define WEDNESDAY 4
#define THURSDAY 5
#define FRIDAY 6
#define SATURDAY 7

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t time_format;  
}RTC_time_t;

typedef struct 
{
    uint8_t date;
    uint8_t month;
    uint8_t year;  
    uint8_t day;
}RTC_date_t;

/* Function Prototypes */
void ds1307_init(void);

void ds1307_set_current_time(RTC_time_t *rtc_time);
void ds1307_get_current_time(RTC_time_t *rtc_time);

void ds1307_set_current_date(RTC_date_t *rtc_date);
void ds1307_get_current_date(RTC_date_t *rtc_date);

#endif /* DS1307_H_ */