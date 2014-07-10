#include "iom128.h"
#include <ina90.h>
#include "i2c.c"


/* ��� ���� ������� � ������� time[] */
#define HOUR            0
#define MINUTE          1
#define SECOND          2
#define DAY             3
#define DATE            4
#define MONTH           5
#define YEAR            6

/* ��� ���� ������� � ������� clock_data[] */
#define CL_SECOND          0
#define CL_MINUTE          1
#define CL_HOUR            2
#define CL_DAY             3
#define CL_DATE            4
#define CL_MONTH           5
#define CL_YEAR            6
#define CL_MODE            7

void ClockBuffRead(void); /* ������ ��������� ���� ������� � ����� */
void ClockBuffWrite(void); /* ������ ��������� ���� ������� �� ������ */

unsigned char time[7];       /* ������ ���� ������� */
unsigned char clock_data[8]; /* ������ �������� �����-��������� */
unsigned char param;      /* ����� �������� ��������� */

void HourUp(void)
{
 if(++time[HOUR] == 24) time[HOUR] = 0; /* ���� */
 clock_data[CL_HOUR] = ((time[HOUR]/10)<<4)+(time[HOUR]%10);
// time[SECOND] = 0; /* �������� ������� */
// clock_data[CL_SECOND] = 0;
 clock_data[CL_MODE] = 0x13; /* ��������� ����� 32768 �� */
 ClockBuffWrite(); /* ������ ��������� ���� ������� �� ������ */
}

void MinUp(void)
{
 if(++time[MINUTE] == 60) time[MINUTE] = 0; /* ������ */
 clock_data[CL_MINUTE] = ((time[MINUTE]/10)<<4)+(time[MINUTE]%10);
// time[SECOND] = 0; /* �������� ������� */
// clock_data[CL_SECOND] = 0;
 clock_data[CL_MODE] = 0x13; /* ��������� ����� 32768 �� */
 ClockBuffWrite(); /* ������ ��������� ���� ������� �� ������ */
}

void SecZero(void)
{
 time[SECOND] = 0; /* �������� ������� */
 clock_data[CL_SECOND] = 0;
// clock_data[CL_MODE] = 0x13; /* ��������� ����� 32768 �� */
 ClockBuffWrite(); /* ������ ��������� ���� ������� �� ������ */
}

//void DayUp(void)
//{
// if(++time[DAY] == 8) time[DAY] = 1; /* ���� ������ */
// clock_data[CL_DAY] = time[DATE];
// ClockBuffWrite(); /* ������ ��������� ���� ������� �� ������ */
//}

void DateUp(void)
{
 if(++time[DATE] == 32) time[DATE] = 1; /* ���� */
 clock_data[CL_DATE] = ((time[DATE]/10)<<4)+(time[DATE]%10);
 ClockBuffWrite(); /* ������ ��������� ���� ������� �� ������ */
}
void MonthUp(void)
{
 if(++time[MONTH] == 13) time[MONTH] = 1; /* ���� */
 clock_data[CL_MONTH] = ((time[MONTH]/10)<<4)+(time[MONTH]%10);
 ClockBuffWrite(); /* ������ ��������� ���� ������� �� ������ */
}
void YearUp(void)
{
 if(++time[YEAR] == 100) time[YEAR] = 8; /* ���� */
 clock_data[CL_YEAR] = ((time[YEAR]/10)<<4)+(time[YEAR]%10);
 ClockBuffWrite(); /* ������ ��������� ���� ������� �� ������ */
}

void TimeDown(void)
{
 if(!param)
 {
  if(time[HOUR]-- == 0) time[HOUR] = 23;  /* ���� */
  clock_data[CL_HOUR] = ((time[HOUR]/10)<<4)+(time[HOUR]%10);
 }
 else
 {
  if(time[MINUTE]-- == 0) time[MINUTE] = 59;  /* ������ */
  clock_data[CL_MINUTE] = ((time[MINUTE]/10)<<4)+(time[MINUTE]%10);
 }
 time[SECOND] = 0; /* �������� ������� */
 clock_data[CL_SECOND] = 0;
 clock_data[CL_MODE] = 0x13; /* ��������� ����� 32768 �� */
 ClockBuffWrite(); /* ������ ��������� ���� ������� �� ������ */
}

void TimeUp(void)
{
 if(!param)
 {
  if(++time[HOUR] == 24) time[HOUR] = 0; /* ���� */
  clock_data[CL_HOUR] = ((time[HOUR]/10)<<4)+(time[HOUR]%10);
 }
 else
 {
  if(++time[MINUTE] == 60) time[MINUTE] = 0; /* ������ */
  clock_data[CL_MINUTE] = ((time[MINUTE]/10)<<4)+(time[MINUTE]%10);
 }

 time[SECOND] = 0; /* �������� ������� */
 clock_data[CL_SECOND] = 0;
 clock_data[CL_MODE] = 0x13; /* ��������� ����� 32768 �� */
 ClockBuffWrite(); /* ������ ��������� ���� ������� �� ������ */
}

void RefreshTime(void) /* �������� �������� ������� � ������� time[] */
{
 ClockBuffRead(); /* ������ ��������� ����/������� � ����� */
 time[SECOND]=(clock_data[CL_SECOND]&0x0F)+(clock_data[CL_SECOND]>>4)*10; /* ������� */
 time[MINUTE]=(clock_data[CL_MINUTE]&0x0F)+(clock_data[CL_MINUTE]>>4)*10; /* ������ */
 time[HOUR]=(clock_data[CL_HOUR]&0x0F)+((clock_data[CL_HOUR]>>4)&0x03)*10; /* ���� */
 time[DAY]=(clock_data[CL_DAY] & 0x07); // ���� ������
 time[DATE]=(clock_data[CL_DATE]&0x0F)+((clock_data[CL_DATE]>>4)&0x03)*10; /* ���� */
 time[MONTH]=(clock_data[CL_MONTH]&0x0F)+((clock_data[CL_MONTH]>>4)&0x01)*10; /* ����� */
 time[YEAR]=(clock_data[CL_YEAR]&0x0F)+(clock_data[CL_YEAR]>>4)*10; /* ��� */
}

