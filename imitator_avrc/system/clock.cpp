
#include "../main.h"
#ifdef CLOCK
#include "clock.h"

//i2c
#define SCLK_IO  DDRA_DDA7    // Направление линии тактирования
#define SDA_IO   DDRA_DDA6    // Направление линии данных
#define SCLK_WR  PORTA_PORTA7 // Выдать линию тактирования
#define SDA_WR   PORTA_PORTA6 // Выдать на линию данных
#define SCLK_RD  PINA_PINA7
#define SDA_RD   PINA_PINA6
#define SDA_SCLK_HI {SCLK_WR=1;SDA_WR=1;} // Выдать "1" на линию данных и тактирования I2C
#define clc_zad 4
namespace clockrt
{
  unsigned char clock_data[8]; // Массив значений часов-календаря
  unsigned char time[7];       // Массив даты времени
  unsigned char clock_datas[8]; // Массив значений часов-календаря
  unsigned char level_sda;
      unsigned int tik_count = 1;
      unsigned char tik_cn = 0;
      unsigned char tik = 0;
      unsigned char old_sec = 255;
  
  inline void Start(void)  // Передать условие старта обмена
  {
    SDA_SCLK_HI;
    _delay_us(clc_zad);   // немного подождать
    SDA_WR=0;       // Выдать "0" на линию данных I2C
    _delay_us(clc_zad);   // немного подождать
    SCLK_WR=0;      // Выдать "0" на линию тактирования I2C
  }
  inline void Stop(void)   // выдать команду СТОП обмена
  {
    SDA_WR=0;       // Выдать "0" на линию данных I2C
    _delay_us(clc_zad);   // немного подождать
    SCLK_WR=1;      // Выдать "1" на линию данных и тактирования I2C
    _delay_us(clc_zad);   // немного подождать
    SDA_WR=1;       // Выдать "1" на линию данных I2C
  }
  inline void Clock(void)  // Сформировать импульс тактирования
  {                 // уровень линии SDA сохранить в регистре level_sda
    SCLK_WR=1;      // Выдать "1" на линию данных и тактирования I2C
    level_sda = 1;  // По умолчанию нет ответа от eeprom level_sda=1
    _delay_us(clc_zad);   // немного подождать
    if(!SDA_RD) level_sda = 0; // Если SDA = 0, то получено подтверждение от eeprom
    SCLK_WR=0;      // Выдать "0" на линию тактирования I2C
    _delay_us(clc_zad);   // немного подождать
  }
  void Ack(void)    // Выдать команду ack - продолжить чтение страницы
  {
    SDA_WR=0;       // Выдать "0" на линию данных I2C
    Clock();        // Сформировать импульс тактирования
    SDA_WR=1;       // Выдать "1" на линию данных I2C
  }
  void Nack(void)   // Выдать команду nack - закончить чтение байта
  {
    SDA_WR=1;       // Выдать "1" на линию данных I2C
    Clock();        // Сформировать импульс тактирования
  }
  void RecAck(void) // Проверка на прием подтверждения от eeprom (результат в level_sda)
  {
    SDA_IO=0;       // Линия данных I2C на прием
    Clock();        // Сформировать импульс тактирования
    SDA_IO=1;       // Линия данных I2C на передачу
  }
  unsigned char InByte()
  {
    unsigned char bit_count;
    unsigned char data_i2c;
    data_i2c = 0;
    SDA_IO=0;         // Линия данных I2C на прием
    for(bit_count=0;bit_count<8;bit_count++) {
      SCLK_WR=1;      // Выдать "1" на линию данных и тактирования I2C
      _delay_us(clc_zad);   // немного подождать
      data_i2c <<= 1;
      if(SDA_RD==1)      // Если SDA = 0, то получено подтверждение от eeprom
        data_i2c += 1;
      SCLK_WR=0;      // Выдать "0" на линию тактирования I2C
      _delay_us(clc_zad);   // немного подождать
    }
    SDA_IO=1;         // Линия данных I2C на передачу
    return data_i2c;
  }
  void OutByte(unsigned char data_i2c) // Передать записываемые данные
  {
    unsigned char bit_count;
    for(bit_count=0;bit_count<8;bit_count++) {
      if(data_i2c & 0x80)
        SDA_WR=1;     // Выдать "1" на линию данных I2C
      else
        SDA_WR=0;     // Выдать "0" на линию данных I2C
      Clock();        // Сформировать импульс тактирования
      data_i2c <<= 1;
    }
  }
  void WrPoll(void)
  {
    /*
    unsigned char ack_count;
    ack_count=0xFF;
    do
    {
      ack_count--;
    */
    Start();          // Передать условие старта обмена
    OutByte(0xD0);    // Передать адрес устройства
    RecAck();         // Проверка на прием подтверждения от eeprom (результат в level_sda)
    /*
    if(!level_sda)
      return;
    }
    while(ack_count);
    */
    // Start();   // Передать условие старта обмена
    // Stop();    // выдать команду СТОП обмена
  }

  void ClockBuffWrite() // Запись регистров даты времени из буфера
  {
    unsigned char count_byte;
    WrPoll();
    OutByte(0);       // Начальный адрес
    RecAck();         // Проверка на прием подтверждения от eeprom (результат в level_sda)
    for(count_byte=0;count_byte<8;count_byte++) {
      OutByte(clock_datas[count_byte]);  // Переслать массив значений часов-календаря
      RecAck();                         // Проверка на прием подтверждения от eeprom (результат в level_sda)
    }
    Stop();         // выдать команду СТОП обмена
  }
  void ClockBuffRead() // Чтение регистров даты времени в буфер
  {
    unsigned char count_byte;
    WrPoll();
    OutByte(0x00);  // Начальный адрес
    RecAck();       // Проверка на прием подтверждения от eeprom (результат в level_sda)
    Start();        // Передать условие старта обмена
    OutByte(0xD1);  // Переключить на чтение
    RecAck();       // Проверка на прием подтверждения от eeprom (результат в level_sda)
    for(count_byte=0;count_byte<8;count_byte++) {
      clock_data[count_byte] = InByte();    // Заполнить массив значений часов-календаря
      if(count_byte!=7)
        Ack();      // Выдать команду ack - продолжить чтение страницы
      else
        Nack();     // Выдать команду nack - закончить чтение байта
    }
    Stop(); /* Выдать команду СТОП обмена */
  }
  void RefreshTime(void) // Обновить значение времени в массиве time[]
  {
      CritSec cs;
      ClockBuffRead(); /* Чтение регистров даты/времени в буфер */
      time[CT_SECOND]=(clock_data[CC_SECOND]&0x0F)+(clock_data[CC_SECOND]>>4)*10; /* Секунды */
      time[CT_MINUTE]=(clock_data[CC_MINUTE]&0x0F)+(clock_data[CC_MINUTE]>>4)*10; /* Минуты */
      time[CT_HOUR]=(clock_data[CC_HOUR]&0x0F)+((clock_data[CC_HOUR]>>4)&0x03)*10; /* Часы */
      time[CT_DAY]=(clock_data[CC_DAY] & 0x07); // День недели
      time[CT_DATE]=(clock_data[CC_DATE]&0x0F)+((clock_data[CC_DATE]>>4)&0x03)*10; /* Дата */
      time[CT_MONTH]=(clock_data[CC_MONTH]&0x0F)+((clock_data[CC_MONTH]>>4)&0x01)*10; /* Месяц */
      time[CT_YEAR]=(clock_data[CC_YEAR]&0x0F)+(clock_data[CC_YEAR]>>4)*10; /* Год */
  }
  void RefSav()
  {
      CritSec cs;
      for (unsigned char i=0;i<8;i++) clock_datas[i] = clock_data[i];
  }
  void SetYear(unsigned char year)
  {
      clock_datas[CC_YEAR] = ((year/10)<<4)+(year%10);
  }
  void SetMonth(unsigned char mount)
  {
      clock_datas[CC_MONTH] = ((mount/10)<<4)+(mount%10);
  }
  void SetDate(unsigned char date)
  {
      clock_datas[CC_DATE] = ((date/10)<<4)+(date%10);
  }
  void SetHour(unsigned char hour)
  {
      clock_datas[CC_HOUR] = ((hour/10)<<4)+(hour%10);
  }
  void SetMinute(unsigned char minute)
  {
      clock_datas[CC_MINUTE] = ((minute/10)<<4)+(minute%10);
  }
  void SetZeroSecond()
  {
      clock_datas[CC_SECOND] = 0;
  }
  void Update()
  {
      CritSec cs;
      ClockBuffWrite();
  }
  void init()
  {
    SCLK_IO=1;
    SDA_IO=1;
    SDA_SCLK_HI;
    RefreshTime();
    RefSav();
    if (time[CT_HOUR]>23)   SetHour(0);
    if (time[CT_MINUTE]>59) SetMinute(0);
    if (time[CT_SECOND]>59) SetZeroSecond();
    clock_datas[CC_MODE] = 0x13;
    Update();
  }
  void timer()
  {
      if (--tik_count>0) return;
      tik_count = 200;
      RefreshTime();
      if (++tik_cn>1) tik_cn = 0;
      if (old_sec==time[CT_SECOND]) return;
      old_sec=time[CT_SECOND];
      tik = 1;
  }
}
#endif
