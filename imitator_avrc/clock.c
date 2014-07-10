#include "iom128.h"
#include <ina90.h>
#include "i2c.c"


/* Код даты времени в массиве time[] */
#define HOUR            0
#define MINUTE          1
#define SECOND          2
#define DAY             3
#define DATE            4
#define MONTH           5
#define YEAR            6

/* Код даты времени в массиве clock_data[] */
#define CL_SECOND          0
#define CL_MINUTE          1
#define CL_HOUR            2
#define CL_DAY             3
#define CL_DATE            4
#define CL_MONTH           5
#define CL_YEAR            6
#define CL_MODE            7

void ClockBuffRead(void); /* Чтение регистров даты времени в буфер */
void ClockBuffWrite(void); /* Запись регистров даты времени из буфера */

unsigned char time[7];       /* Массив даты времени */
unsigned char clock_data[8]; /* Массив значений часов-календаря */
unsigned char param;      /* Номер текущего параметра */

void HourUp(void)
{
 if(++time[HOUR] == 24) time[HOUR] = 0; /* Часы */
 clock_data[CL_HOUR] = ((time[HOUR]/10)<<4)+(time[HOUR]%10);
// time[SECOND] = 0; /* Сбросить секунды */
// clock_data[CL_SECOND] = 0;
 clock_data[CL_MODE] = 0x13; /* Разрешить выход 32768 Гц */
 ClockBuffWrite(); /* Запись регистров даты времени из буфера */
}

void MinUp(void)
{
 if(++time[MINUTE] == 60) time[MINUTE] = 0; /* Минуты */
 clock_data[CL_MINUTE] = ((time[MINUTE]/10)<<4)+(time[MINUTE]%10);
// time[SECOND] = 0; /* Сбросить секунды */
// clock_data[CL_SECOND] = 0;
 clock_data[CL_MODE] = 0x13; /* Разрешить выход 32768 Гц */
 ClockBuffWrite(); /* Запись регистров даты времени из буфера */
}

void SecZero(void)
{
 time[SECOND] = 0; /* Сбросить секунды */
 clock_data[CL_SECOND] = 0;
// clock_data[CL_MODE] = 0x13; /* Разрешить выход 32768 Гц */
 ClockBuffWrite(); /* Запись регистров даты времени из буфера */
}

//void DayUp(void)
//{
// if(++time[DAY] == 8) time[DAY] = 1; /* День недели */
// clock_data[CL_DAY] = time[DATE];
// ClockBuffWrite(); /* Запись регистров даты времени из буфера */
//}

void DateUp(void)
{
 if(++time[DATE] == 32) time[DATE] = 1; /* Дата */
 clock_data[CL_DATE] = ((time[DATE]/10)<<4)+(time[DATE]%10);
 ClockBuffWrite(); /* Запись регистров даты времени из буфера */
}
void MonthUp(void)
{
 if(++time[MONTH] == 13) time[MONTH] = 1; /* Часы */
 clock_data[CL_MONTH] = ((time[MONTH]/10)<<4)+(time[MONTH]%10);
 ClockBuffWrite(); /* Запись регистров даты времени из буфера */
}
void YearUp(void)
{
 if(++time[YEAR] == 100) time[YEAR] = 8; /* Часы */
 clock_data[CL_YEAR] = ((time[YEAR]/10)<<4)+(time[YEAR]%10);
 ClockBuffWrite(); /* Запись регистров даты времени из буфера */
}

void TimeDown(void)
{
 if(!param)
 {
  if(time[HOUR]-- == 0) time[HOUR] = 23;  /* Часы */
  clock_data[CL_HOUR] = ((time[HOUR]/10)<<4)+(time[HOUR]%10);
 }
 else
 {
  if(time[MINUTE]-- == 0) time[MINUTE] = 59;  /* Минуты */
  clock_data[CL_MINUTE] = ((time[MINUTE]/10)<<4)+(time[MINUTE]%10);
 }
 time[SECOND] = 0; /* Сбросить секунды */
 clock_data[CL_SECOND] = 0;
 clock_data[CL_MODE] = 0x13; /* Разрешить выход 32768 Гц */
 ClockBuffWrite(); /* Запись регистров даты времени из буфера */
}

void TimeUp(void)
{
 if(!param)
 {
  if(++time[HOUR] == 24) time[HOUR] = 0; /* Часы */
  clock_data[CL_HOUR] = ((time[HOUR]/10)<<4)+(time[HOUR]%10);
 }
 else
 {
  if(++time[MINUTE] == 60) time[MINUTE] = 0; /* Минуты */
  clock_data[CL_MINUTE] = ((time[MINUTE]/10)<<4)+(time[MINUTE]%10);
 }

 time[SECOND] = 0; /* Сбросить секунды */
 clock_data[CL_SECOND] = 0;
 clock_data[CL_MODE] = 0x13; /* Разрешить выход 32768 Гц */
 ClockBuffWrite(); /* Запись регистров даты времени из буфера */
}

void RefreshTime(void) /* Обновить значение времени в массиве time[] */
{
 ClockBuffRead(); /* Чтение регистров даты/времени в буфер */
 time[SECOND]=(clock_data[CL_SECOND]&0x0F)+(clock_data[CL_SECOND]>>4)*10; /* Секунды */
 time[MINUTE]=(clock_data[CL_MINUTE]&0x0F)+(clock_data[CL_MINUTE]>>4)*10; /* Минуты */
 time[HOUR]=(clock_data[CL_HOUR]&0x0F)+((clock_data[CL_HOUR]>>4)&0x03)*10; /* Часы */
 time[DAY]=(clock_data[CL_DAY] & 0x07); // День недели
 time[DATE]=(clock_data[CL_DATE]&0x0F)+((clock_data[CL_DATE]>>4)&0x03)*10; /* Дата */
 time[MONTH]=(clock_data[CL_MONTH]&0x0F)+((clock_data[CL_MONTH]>>4)&0x01)*10; /* Месяц */
 time[YEAR]=(clock_data[CL_YEAR]&0x0F)+(clock_data[CL_YEAR]>>4)*10; /* Год */
}

