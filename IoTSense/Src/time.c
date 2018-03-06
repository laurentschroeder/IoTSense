/*
 * time.c
 *
 *  Created on: 05.03.2018
 *      Author: Laurent
 */

#include "stm32f0xx_hal.h"
#include "string.h"
#include "stdlib.h"

extern RTC_HandleTypeDef hrtc;

uint8_t time_received = 0;

void wait_for_time(void)
{
    while(time_received != 1)
    {

    }
}

void unlock_time(char *timestring)
{
    char delimiter[] = " :";
    char *ptr;
    char *parameters[9];
    uint8_t i = 0;

    // initialisieren und ersten Abschnitt erstellen
    ptr = strtok(timestring, delimiter);

    while(ptr != NULL)
    {
        parameters[i] = ptr;
        i++;
        // naechsten Abschnitt erstellen
        ptr = strtok(NULL, delimiter);
    }

    RTC_DateTypeDef date;
    RTC_TimeTypeDef time;
    if(strcmp(parameters[2], "Mon") == 0)
    {
        date.WeekDay = RTC_WEEKDAY_MONDAY;
    }
    else if(strcmp(parameters[2], "Tue") == 0)
    {
        date.WeekDay = RTC_WEEKDAY_TUESDAY;
    }
    else if(strcmp(parameters[2], "Wen") == 0)
    {
        date.WeekDay = RTC_WEEKDAY_WEDNESDAY;
    }
    else if(strcmp(parameters[2], "Thu") == 0)
    {
        date.WeekDay = RTC_WEEKDAY_THURSDAY;
    }
    else if(strcmp(parameters[2], "Fri") == 0)
    {
        date.WeekDay = RTC_WEEKDAY_FRIDAY;
    }
    else if(strcmp(parameters[2], "Sat") == 0)
    {
        date.WeekDay = RTC_WEEKDAY_SATURDAY;
    }
    else if(strcmp(parameters[2], "Sun") == 0)
    {
        date.WeekDay = RTC_WEEKDAY_SUNDAY;
    }

    if(strcmp(parameters[3], "Jan") == 0)
    {
        date.Month = RTC_MONTH_JANUARY;
    }
    else if(strcmp(parameters[3], "Feb") == 0)
    {
        date.Month = RTC_MONTH_FEBRUARY;
    }
    else if(strcmp(parameters[3], "Mar") == 0)
    {
        date.Month = RTC_MONTH_MARCH;
    }
    else if(strcmp(parameters[3], "Apr") == 0)
    {
        date.Month = RTC_MONTH_APRIL;
    }
    else if(strcmp(parameters[3], "May") == 0)
    {
        date.Month = RTC_MONTH_MAY;
    }
    else if(strcmp(parameters[3], "Jun") == 0)
    {
        date.Month = RTC_MONTH_JUNE;
    }
    else if(strcmp(parameters[3], "Jul") == 0)
    {
        date.Month = RTC_MONTH_JULY;
    }
    else if(strcmp(parameters[3], "Aug") == 0)
    {
        date.Month = RTC_MONTH_AUGUST;
    }
    else if(strcmp(parameters[3], "Sep") == 0)
    {
        date.Month = RTC_MONTH_SEPTEMBER;
    }
    else if(strcmp(parameters[3], "Oct") == 0)
    {
        date.Month = RTC_MONTH_OCTOBER;
    }
    else if(strcmp(parameters[3], "Nov") == 0)
    {
        date.Month = RTC_MONTH_NOVEMBER;
    }
    else if(strcmp(parameters[3], "Dec") == 0)
    {
        date.Month = RTC_MONTH_DECEMBER;
    }

    date.Date = (uint8_t)atoi(parameters[4]);
    time.Hours = (uint8_t)atoi(parameters[5]);
    time.Minutes = (uint8_t)atoi(parameters[6]);
    time.Seconds = (uint8_t)atoi(parameters[7]);

    char shortyear[2];
    shortyear[0] = parameters[8][2];
    shortyear[1] = parameters[8][3];
    date.Year = (uint8_t)atoi(shortyear);

    HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);

}
