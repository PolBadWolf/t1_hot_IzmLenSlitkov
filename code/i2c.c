#include "iom128.h"
#include <ina90.h>

//i2c
#define FLSC        0x80 /* PORT_OUT, тактирование для I2C */
#define FLSD        0x40 /* PORT_IN_OUT, данные I2C */
#define SCLK_HI     PORTA |=  FLSC /* Выдать "1" на линию тактирования */
#define SCLK_LO     PORTA &= ~FLSC /* Выдать "0" на линию тактирования */
#define SDA_HI      PORTA |=  FLSD /* Выдать "1" на линию данных */
#define SDA_LO      PORTA &= ~FLSD /* Выдать "0" на линию данных */
#define SDA_SCLK_HI PORTA |= FLSD | FLSC /* Выдать "1" на линию данных и тактирования I2C */
#define SDA_INPUT   DDRA  &= ~FLSD /* Линия данных на прием */
#define SDA_OUTPUT  DDRA  |=  FLSD /* Линия данных на передачу */
#define SDA         (PINA & FLSD)


extern unsigned char clock_data[8]; /* Массив значений часов-календаря */
unsigned char level_sda;

void Start(void)    // Передать условие старта обмена
{
  clockrt::Start();
 /*
  SDA_SCLK_HI;       // Выдать "1" на линию данных и тактирования I2C
 //Delay4mks();     // немного подождать
 __delay_cycles(64);
 SDA_LO;            // Выдать "0" на линию данных I2C
 //Delay4mks();     // немного подождать
 __delay_cycles(64);
 SCLK_LO;           // Выдать "0" на линию тактирования I2C
 */
}

void Stop(void) /* выдать команду СТОП обмена */
{
 SDA_LO;      /* Выдать "0" на линию данных I2C */
 //Delay4mks(); /* немного подождать */
 __delay_cycles(64);
 SCLK_HI;     /* Выдать "1" на линию тактирования I2C */
 __delay_cycles(64);
 //Delay4mks(); /* немного подождать */
 SDA_HI;      /* Выдать "1" на линию данных I2C */
}

void Clock(void) /* Сформировать импульс тактирования */
{                /* уровень линии SDA сохранить в регистре level_sda */
 SCLK_HI;     /* Выдать "1" на линию тактирования I2C */
 //Delay4mks(); /* Немного подождать */
 __delay_cycles(64);
 level_sda = 1; /* По умолчанию нет ответа от eeprom level_sda=1 */
 if(!SDA) level_sda = 0; /* Если SDA = 0, то получено подтверждение от eeprom */
 SCLK_LO;     /* Выдать "0" на линию тактирования I2C */
 //Delay4mks(); /* Немного подождать */
 __delay_cycles(64);
}

void Ack(void) /* Выдать команду ack - продолжить чтение страницы */
{
 SDA_LO;      /* Выдать "0" на линию данных I2C */
 Clock();     /* Сформировать импульс тактирования */
 SDA_HI;      /* Выдать "1" на линию данных I2C */
}

void Nack(void) /* Выдать команду nack - закончить чтение байта */
{
 SDA_HI;      /* Выдать "1" на линию данных I2C */
 Clock();     /* Сформировать импульс тактирования */
}

void RecAck(void) /* Проверка на прием подтверждения от eeprom (результат в level_sda) */
{
 SDA_INPUT;   /* Линия данных I2C на прием */
// SDA_LO;      /* Перевести в третье состояние */
 Clock();     /* Сформировать импульс тактирования */
 SDA_OUTPUT;  /* Линия данных I2C на передачу */
}

unsigned char InByte(void)
{
 unsigned char bit_count;
 unsigned char data_i2c;
 data_i2c = 0;
 SDA_INPUT;   /* Линия данных I2C на прием */
// SDA_LO;      /* Перевести в третье состояние */
 for(bit_count=0;bit_count<8;bit_count++)
 {
 SCLK_HI;     /* Выдать "1" на линию тактирования I2C */
 __delay_cycles(64);
 //Delay4mks(); /* Немного подождать */
 data_i2c <<= 1;
 if(SDA)  /* Если SDA = 0, то получено подтверждение от eeprom */
  data_i2c += 0x01;
 SCLK_LO;     /* Выдать "0" на линию тактирования I2C */
 __delay_cycles(64);
 //Delay4mks(); /* Немного подождать */
 }
 //SDA_HI;
 SDA_OUTPUT;  /* Линия данных I2C на передачу */
 return data_i2c;
}

void OutByte(unsigned char data_i2c) /* Передать записываемые данные */
{
 unsigned char bit_count;
 for(bit_count=0;bit_count<8;bit_count++)
 {
  if(data_i2c & 0x80)
   SDA_HI;    /* Выдать "1" на линию данных I2C */
  else
   SDA_LO;    /* Выдать "0" на линию данных I2C */
  Clock();    /* Сформировать импульс тактирования */
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
  Start(); /* Передать условие старта обмена */
  OutByte(0xD0); /* Передать адрес устройства */
  RecAck(); /* Проверка на прием подтверждения от eeprom (результат в level_sda) */
/*  if(!level_sda)
   return;
 }
 while(ack_count); */
// Start(); /* Передать условие старта обмена */
// Stop(); /* выдать команду СТОП обмена */ 
}

void ClockBuffWrite(void) /* Запись регистров даты времени из буфера */
{
 unsigned char count_byte;
 WrPoll();
 OutByte(0); /* Начальный адрес */
 RecAck(); /* Проверка на прием подтверждения от eeprom (результат в level_sda) */
 for(count_byte=0;count_byte<8;count_byte++)
 {
  OutByte(clock_data[count_byte]); /* Переслать массив значений часов-календаря */
  RecAck(); /* Проверка на прием подтверждения от eeprom (результат в level_sda) */
 }
 Stop(); /* выдать команду СТОП обмена */
}

void ClockBuffRead(void) /* Чтение регистров даты времени в буфер */
{
 unsigned char count_byte;
 WrPoll();
 OutByte(0x00); /* Начальный адрес */
 RecAck(); /* Проверка на прием подтверждения от eeprom (результат в level_sda) */
 Start(); /* Передать условие старта обмена */
 OutByte(0xD1); /* Переключить на чтение */
 RecAck(); /* Проверка на прием подтверждения от eeprom (результат в level_sda) */
 for(count_byte=0;count_byte<8;count_byte++)
 {
  clock_data[count_byte] = InByte(); /* Заполнить массив значений часов-календаря */
  if(count_byte!=7)
   Ack(); /* Выдать команду ack - продолжить чтение страницы */
  else
   Nack(); /* Выдать команду nack - закончить чтение байта */
 }
 Stop(); /* Выдать команду СТОП обмена */
}


