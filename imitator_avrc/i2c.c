#include "iom128.h"
#include <ina90.h>

//i2c
#define FLSC        0x80 /* PORT_OUT, ������������ ��� I2C */
#define FLSD        0x40 /* PORT_IN_OUT, ������ I2C */
#define SCLK_HI     PORTA |=  FLSC /* ������ "1" �� ����� ������������ */
#define SCLK_LO     PORTA &= ~FLSC /* ������ "0" �� ����� ������������ */
#define SDA_HI      PORTA |=  FLSD /* ������ "1" �� ����� ������ */
#define SDA_LO      PORTA &= ~FLSD /* ������ "0" �� ����� ������ */
#define SDA_SCLK_HI PORTA |= FLSD | FLSC /* ������ "1" �� ����� ������ � ������������ I2C */
#define SDA_INPUT   DDRA  &= ~FLSD /* ����� ������ �� ����� */
#define SDA_OUTPUT  DDRA  |=  FLSD /* ����� ������ �� �������� */
#define SDA         (PINA & FLSD)


extern unsigned char clock_data[8]; /* ������ �������� �����-��������� */
unsigned char level_sda;

void Start(void)    // �������� ������� ������ ������
{
  clockrt::Start();
 /*
  SDA_SCLK_HI;       // ������ "1" �� ����� ������ � ������������ I2C
 //Delay4mks();     // ������� ���������
 __delay_cycles(64);
 SDA_LO;            // ������ "0" �� ����� ������ I2C
 //Delay4mks();     // ������� ���������
 __delay_cycles(64);
 SCLK_LO;           // ������ "0" �� ����� ������������ I2C
 */
}

void Stop(void) /* ������ ������� ���� ������ */
{
 SDA_LO;      /* ������ "0" �� ����� ������ I2C */
 //Delay4mks(); /* ������� ��������� */
 __delay_cycles(64);
 SCLK_HI;     /* ������ "1" �� ����� ������������ I2C */
 __delay_cycles(64);
 //Delay4mks(); /* ������� ��������� */
 SDA_HI;      /* ������ "1" �� ����� ������ I2C */
}

void Clock(void) /* ������������ ������� ������������ */
{                /* ������� ����� SDA ��������� � �������� level_sda */
 SCLK_HI;     /* ������ "1" �� ����� ������������ I2C */
 //Delay4mks(); /* ������� ��������� */
 __delay_cycles(64);
 level_sda = 1; /* �� ��������� ��� ������ �� eeprom level_sda=1 */
 if(!SDA) level_sda = 0; /* ���� SDA = 0, �� �������� ������������� �� eeprom */
 SCLK_LO;     /* ������ "0" �� ����� ������������ I2C */
 //Delay4mks(); /* ������� ��������� */
 __delay_cycles(64);
}

void Ack(void) /* ������ ������� ack - ���������� ������ �������� */
{
 SDA_LO;      /* ������ "0" �� ����� ������ I2C */
 Clock();     /* ������������ ������� ������������ */
 SDA_HI;      /* ������ "1" �� ����� ������ I2C */
}

void Nack(void) /* ������ ������� nack - ��������� ������ ����� */
{
 SDA_HI;      /* ������ "1" �� ����� ������ I2C */
 Clock();     /* ������������ ������� ������������ */
}

void RecAck(void) /* �������� �� ����� ������������� �� eeprom (��������� � level_sda) */
{
 SDA_INPUT;   /* ����� ������ I2C �� ����� */
// SDA_LO;      /* ��������� � ������ ��������� */
 Clock();     /* ������������ ������� ������������ */
 SDA_OUTPUT;  /* ����� ������ I2C �� �������� */
}

unsigned char InByte(void)
{
 unsigned char bit_count;
 unsigned char data_i2c;
 data_i2c = 0;
 SDA_INPUT;   /* ����� ������ I2C �� ����� */
// SDA_LO;      /* ��������� � ������ ��������� */
 for(bit_count=0;bit_count<8;bit_count++)
 {
 SCLK_HI;     /* ������ "1" �� ����� ������������ I2C */
 __delay_cycles(64);
 //Delay4mks(); /* ������� ��������� */
 data_i2c <<= 1;
 if(SDA)  /* ���� SDA = 0, �� �������� ������������� �� eeprom */
  data_i2c += 0x01;
 SCLK_LO;     /* ������ "0" �� ����� ������������ I2C */
 __delay_cycles(64);
 //Delay4mks(); /* ������� ��������� */
 }
 //SDA_HI;
 SDA_OUTPUT;  /* ����� ������ I2C �� �������� */
 return data_i2c;
}

void OutByte(unsigned char data_i2c) /* �������� ������������ ������ */
{
 unsigned char bit_count;
 for(bit_count=0;bit_count<8;bit_count++)
 {
  if(data_i2c & 0x80)
   SDA_HI;    /* ������ "1" �� ����� ������ I2C */
  else
   SDA_LO;    /* ������ "0" �� ����� ������ I2C */
  Clock();    /* ������������ ������� ������������ */
  data_i2c <<= 1;
 }
}

void WrPoll(void)
{
/* unsigned char ack_count;
 ack_count=0xFF;
 do
 {
  ack_count--;  */
  Start(); /* �������� ������� ������ ������ */
  OutByte(0xD0); /* �������� ����� ���������� */
  RecAck(); /* �������� �� ����� ������������� �� eeprom (��������� � level_sda) */
/*  if(!level_sda)
   return;
 }
 while(ack_count); */
// Start(); /* �������� ������� ������ ������ */
// Stop(); /* ������ ������� ���� ������ */ 
}

void ClockBuffWrite(void) /* ������ ��������� ���� ������� �� ������ */
{
 unsigned char count_byte;
 WrPoll();
 OutByte(0); /* ��������� ����� */
 RecAck(); /* �������� �� ����� ������������� �� eeprom (��������� � level_sda) */
 for(count_byte=0;count_byte<8;count_byte++)
 {
  OutByte(clock_data[count_byte]); /* ��������� ������ �������� �����-��������� */
  RecAck(); /* �������� �� ����� ������������� �� eeprom (��������� � level_sda) */
 }
 Stop(); /* ������ ������� ���� ������ */
}

void ClockBuffRead(void) /* ������ ��������� ���� ������� � ����� */
{
 unsigned char count_byte;
 WrPoll();
 OutByte(0x00); /* ��������� ����� */
 RecAck(); /* �������� �� ����� ������������� �� eeprom (��������� � level_sda) */
 Start(); /* �������� ������� ������ ������ */
 OutByte(0xD1); /* ����������� �� ������ */
 RecAck(); /* �������� �� ����� ������������� �� eeprom (��������� � level_sda) */
 for(count_byte=0;count_byte<8;count_byte++)
 {
  clock_data[count_byte] = InByte(); /* ��������� ������ �������� �����-��������� */
  if(count_byte!=7)
   Ack(); /* ������ ������� ack - ���������� ������ �������� */
  else
   Nack(); /* ������ ������� nack - ��������� ������ ����� */
 }
 Stop(); /* ������ ������� ���� ������ */
}


