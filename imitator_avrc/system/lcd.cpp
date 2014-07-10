
#include "../main.h"
#if defined(LCD)

#include "lcd.h"

#define lcd_d_ddr        DDRA
#define lcd_d_port       PORTA
#define lcd_d_pin        PINA
// ei
#define lcd_ei_ddr        DDRA_DDA4
#define lcd_ei_port       PORTA_PORTA4
#define lcd_ei_pin        PINA_PINA4
// res
#define lcd_res_ddr        DDRA_DDA5
#define lcd_res_port       PORTA_PORTA5
#define lcd_res_pin        PINA_PINA5


// конструктор класса
tc_lcd::tc_lcd()
{
  // настройка портов
  f_InitPorts();
  // настройка переменных
  f_InitVar();
  // настройка контролера lcd
  f_InitLcd();
  Clear();
  ShowString(0, "GM code");
}
void tc_lcd::f_InitPorts(void)
{
  // настройка портов
  lcd_d_ddr = 15;
  lcd_d_port = 15;
  lcd_ei_ddr = 1;
  lcd_ei_port = 1;
  lcd_res_ddr = 1;
  lcd_res_port = 1;
  _delay_ms(500);
}
void tc_lcd::f_InitVar(void)
{
  cur_stroka = cur_stolbec = 0;
  u_RefResh = &screen[0];
  v_old_send = 0;
}
void tc_lcd::f_InitLcd(void)
{
  _delay_ms(40);
  f_SendCommand(0x28); //4-ех разрядная шина, 2 строки
  _delay_ms(2);
  f_SendCommand(0x28); //4-ех разрядная шина, 2 строки
  _delay_ms(2);
  f_SendCommand (0xc);  //0b00001111 - дисплей, курсор, мерцание включены
  f_SendCommand (0x1);  //0b00000001 - очистка дисплея
  _delay_ms(2);
  f_SendCommand (0x6);  //0b00000110 - курсор движется вправо, сдвига нет
  f_SendCommand (0x2);  //0b00000001 - очистка дисплея
  _delay_ms(2);
  f_SendCommand(0x80);
}
void tc_lcd::for_timer(void)
{
  // установка текущей позитции
  u_RefResh++;
  cur_stolbec++;
  if (cur_stolbec>=c_stolbcov)
  {
    cur_stolbec = 0;
    cur_stroka++;
    if (cur_stroka==c_strok)
    {
      cur_stroka = 0;
      u_RefResh = &screen[0];
      f_SendCommand(0x80);
    }
    else
    {
      u_RefResh = &screen[c_stolbcov];
      f_SendCommand(0xC0);
    }
    _delay_us(50);
  }
  // регенерация одного символа
  //u_RefResh =  &screen[0]+(cur_stolbec + cur_stroka*c_stolbcov);
  TempSimbol = *u_RefResh;
  if (TempSimbol>128) TempSimbol = TableDecode[TempSimbol-128];
  f_SendData(TempSimbol);
}
// передача в lcd команды
void tc_lcd::f_SendCommand(unsigned char data)
{
  static unsigned char cs_cm;
  cs_cm = __save_interrupt();
  __disable_interrupt();
  _delay_us(50);
  // передача команды
  if (lcd_res_port!=0) {
    lcd_res_port = 0;
    _delay_us(50);
  }
  f_Send(data);
  __restore_interrupt(cs_cm);
}
// передача в lcd данных
void tc_lcd::f_SendData(unsigned char data)
{
  static unsigned char cs_dt;
  cs_dt = __save_interrupt();
  __disable_interrupt();
    _delay_us(50);
  // передача данных
  if (lcd_res_port!=1) {
    lcd_res_port = 1;
    _delay_us(50);
  }
  f_Send(data);
  __restore_interrupt(cs_dt);
}
void tc_lcd::f_Send(unsigned char data)
{
  lcd_d_port = (lcd_d_port & 0xf0) | ((data>>4) & 0x0f);
  lcd_ei_port = 1;
  __no_operation();
  __no_operation();
  __no_operation();
  lcd_ei_port = 0;
  __no_operation();
  __no_operation();
  __no_operation();
  //
  lcd_d_port = (lcd_d_port & 0xf0) | (data & 0x0f);
  lcd_ei_port = 1;
  __no_operation();
  __no_operation();
  __no_operation();
  lcd_ei_port = 0;
}
__flash unsigned char tc_lcd::TableDecode[] = {
0xD9,0xDA,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0xA2,0xB5,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xA4,0xA5,0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8,
0x50,0x43,0x54,0xA9,0xAA,0x58,0xE1,0xAB,0xAC,0xE2,0xAD,0xAE,0x62,0xAF,0xB0,0xB1,
0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,
0x70,0x63,0xBF,0x79,0xE4,0x78,0xE5,0xC0,0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7};

void tc_lcd::Clear(void)
{
  for (unsigned char i=0;i<(c_strok*c_stolbcov);i++) screen[i] = ' ';
}
void tc_lcd::ShowChar(unsigned char position, unsigned char simbol)
{
  screen[position] = simbol;
}
void tc_lcd::ShowString(unsigned char position, char *stroka)
{
  unsigned char *u_scr;
  unsigned char *u_str;
  unsigned char simbol;
  u_scr = &screen[position];
  u_str = (unsigned char *)stroka;
  simbol = *u_str;
  while(simbol)
  {
    *u_scr = simbol;
    u_scr++;
    u_str++;
    simbol = *u_str;
  }
}

void tc_lcd::_digit_minus( unsigned char poz, signed char lens )
{
  for (unsigned char i=0;i<lens;i++)
  {
    screen[poz] = '-';
    poz++;
  }
}
// char
void tc_lcd::_digit_uz( unsigned char poz, signed char lens, unsigned char *data )
{
  unsigned char *s = &screen[0] + poz + lens;
  while( lens>0 )
  {
    s--;
    *s = '0' + ((*data) % 10);
    (*data) /= 10;
    lens--;
  }
}
void tc_lcd::_digit_uz( unsigned char poz, signed char lens,   signed char *data )
{
  if (*data<0) _digit_minus(poz, lens);
  else         _digit_uz(poz, lens, (unsigned char *)data);
}
// int
void tc_lcd::_digit_uz( unsigned char poz, signed char lens, unsigned int  *data )
{
  unsigned char *s = &screen[0] + poz + lens;
  while( lens>0 )
  {
    s--;
    *s = '0' + ((*data) % 10);
    (*data) /= 10;
    lens--;
  }
}
void tc_lcd::_digit_uz( unsigned char poz, signed char lens,   signed int  *data )
{
  if (*data<0) _digit_minus(poz, lens);
  else         _digit_uz(poz, lens, (unsigned int *)data);
}
// long
void tc_lcd::_digit_uz( unsigned char poz, signed char lens, unsigned long *data )
{
  unsigned char *s = &screen[0] + poz + lens;
  while( lens>0 )
  {
    s--;
    *s = '0' + ((*data) % 10);
    (*data) /= 10;
    lens--;
  }
}
void tc_lcd::_digit_uz( unsigned char poz, signed char lens,   signed long *data )
{
  if (*data<0) _digit_minus(poz, lens);
  else         _digit_uz(poz, lens, (unsigned long *)data);
}
// char no zero
void tc_lcd::_digit_u( unsigned char poz, signed char lens, unsigned char *data )
{
  unsigned char *s = &screen[0] + poz + lens;
  s--;
  *s = '0' + ((*data) % 10);
  (*data) /= 10;
  lens--;
  while( lens>0 )
  {
    s--;
    if (*data)
    {
      *s = '0' + ((*data) % 10);
      (*data) /= 10;
    }
    else *s = ' ';
    lens--;
  }
}
void tc_lcd::_digit_u( unsigned char poz, signed char lens,   signed char *data )
{
  if (*data<0) _digit_minus(poz, lens);
  else        _digit_u(poz, lens, (unsigned char *)data);
}
// int no zero
void tc_lcd::_digit_u( unsigned char poz, signed char lens, unsigned int *data )
{
  unsigned char *s = &screen[0] + poz + lens;
  s--;
  *s = '0' + ((*data) % 10);
  (*data) /= 10;
  lens--;
  while( lens>0 )
  {
    s--;
    if (*data)
    {
      *s = '0' + ((*data) % 10);
      (*data) /= 10;
    }
    else *s = ' ';
    lens--;
  }
}
void tc_lcd::_digit_u( unsigned char poz, signed char lens,   signed int *data )
{
  if (*data<0) _digit_minus(poz, lens);
  else        _digit_u(poz, lens, (unsigned int *)data);
}
// long no zero
void tc_lcd::_digit_u( unsigned char poz, signed char lens, unsigned long *data )
{
  unsigned char *s = &screen[0] + poz + lens;
  s--;
  *s = '0' + ((*data) % 10);
  (*data) /= 10;
  lens--;
  while( lens>0 )
  {
    s--;
    if (*data)
    {
      *s = '0' + ((*data) % 10);
      (*data) /= 10;
    }
    else *s = ' ';
    lens--;
  }
}
void tc_lcd::_digit_u( unsigned char poz, signed char lens,   signed long *data )
{
  if (*data<0) _digit_minus(poz, lens);
  else        _digit_u(poz, lens, (unsigned long *)data);
}
//---------------------
// цифру на экран без знаковую, c незначащими нулями
// char
void tc_lcd::dig_uz( unsigned char poz, signed char lens, unsigned char  data )
{
  _digit_uz(poz, lens, &data);
}
void tc_lcd::dig_uz( unsigned char poz, signed char lens, unsigned char *data )
{
  unsigned char dat;
  dat = *data;
  _digit_uz(poz, lens, &dat);
}
void tc_lcd::dig_uz( unsigned char poz, signed char lens,   signed char  data )
{
  _digit_uz(poz, lens, &data);
}
void tc_lcd::dig_uz( unsigned char poz, signed char lens,   signed char *data )
{
  signed char dat;
  dat = *data;
  _digit_uz(poz, lens, &dat);
}
// int
void tc_lcd::dig_uz( unsigned char poz, signed char lens, unsigned int   data )
{
  _digit_uz(poz, lens, &data);
}
void tc_lcd::dig_uz( unsigned char poz, signed char lens, unsigned int  *data )
{
  unsigned int dat;
  dat = *data;
  _digit_uz(poz, lens, &dat);
}
void tc_lcd::dig_uz( unsigned char poz, signed char lens,   signed int   data )
{
  _digit_uz(poz, lens, &data);
}
void tc_lcd::dig_uz( unsigned char poz, signed char lens,   signed int  *data )
{
  signed int dat;
  dat = *data;
  _digit_uz(poz, lens, &dat);
}
// long
void tc_lcd::dig_uz( unsigned char poz, signed char lens, unsigned long  data )
{
  _digit_uz(poz, lens, &data);
}
void tc_lcd::dig_uz( unsigned char poz, signed char lens, unsigned long *data )
{
  unsigned long dat;
  dat = *data;
  _digit_uz(poz, lens, &dat);
}
void tc_lcd::dig_uz( unsigned char poz, signed char lens,   signed long  data )
{
  _digit_uz(poz, lens, &data);
}
void tc_lcd::dig_uz( unsigned char poz, signed char lens,   signed long *data )
{
  signed long dat;
  dat = *data;
  _digit_uz(poz, lens, &dat);
}
//----------------------------------
// цифру на экран без знаковую, без незначащих нулей
// char
void tc_lcd::dig_u( unsigned char poz, signed char lens, unsigned char  data )
{
  _digit_u(poz, lens, &data);
}
void tc_lcd::dig_u( unsigned char poz, signed char lens, unsigned char *data )
{
  unsigned char dat;
  dat = *data;
  _digit_u(poz, lens, &dat);
}
void tc_lcd::dig_u( unsigned char poz, signed char lens,   signed char  data )
{
  _digit_u(poz, lens, &data);
}
void tc_lcd::dig_u( unsigned char poz, signed char lens,   signed char *data )
{
  signed char dat;
  dat = *data;
  _digit_u(poz, lens, &dat);
}
// int
void tc_lcd::dig_u( unsigned char poz, signed char lens, unsigned int   data )
{
  _digit_u(poz, lens, &data);
}
void tc_lcd::dig_u( unsigned char poz, signed char lens, unsigned int  *data )
{
  unsigned int dat;
  dat = *data;
  _digit_u(poz, lens, &dat);
}
void tc_lcd::dig_u( unsigned char poz, signed char lens,   signed int   data )
{
  _digit_u(poz, lens, &data);
}
void tc_lcd::dig_u( unsigned char poz, signed char lens,   signed int  *data )
{
  signed int dat;
  dat = *data;
  _digit_u(poz, lens, &dat);
}
// long
void tc_lcd::dig_u( unsigned char poz, signed char lens, unsigned long  data )
{
  _digit_u(poz, lens, &data);
}
void tc_lcd::dig_u( unsigned char poz, signed char lens, unsigned long *data )
{
  unsigned long dat;
  dat = *data;
  _digit_u(poz, lens, &dat);
}
void tc_lcd::dig_u( unsigned char poz, signed char lens,   signed long  data )
{
  _digit_u(poz, lens, &data);
}
void tc_lcd::dig_u( unsigned char poz, signed char lens,   signed long *data )
{
  signed long dat;
  dat = *data;
  _digit_u(poz, lens, &dat);
}
//---------------------------------------------
// цифру на экран знаковую, c незначащими нулями
// char
void tc_lcd::dig_sz( unsigned char poz, signed char lens, unsigned char  data )
{
  _digit_uz(poz, lens, &data);
}
void tc_lcd::dig_sz( unsigned char poz, signed char lens, unsigned char *data )
{
  unsigned char dat;
  dat = *data;
  _digit_uz(poz, lens, &dat);
}
void tc_lcd::dig_sz( unsigned char poz, signed char lens,   signed char  data )
{
  unsigned char dat;
  if (data<0)
  {
    screen[poz] = '-';
    poz++;
    lens--;
    dat = -data;
  }
  else dat = data;
  _digit_uz(poz, lens, &dat);
}
void tc_lcd::dig_sz( unsigned char poz, signed char lens,   signed char *data )
{
  unsigned char dat;
  if (*data<0)
  {
    screen[poz] = '-';
    poz++;
    lens--;
    dat = -(*data);
  }
  else dat = *data;
  _digit_uz(poz, lens, &dat);
}
// int
void tc_lcd::dig_sz( unsigned char poz, signed char lens, unsigned int   data )
{
  _digit_uz(poz, lens, &data);
}
void tc_lcd::dig_sz( unsigned char poz, signed char lens, unsigned int  *data )
{
  unsigned int dat;
  dat = *data;
  _digit_uz(poz, lens, &dat);
}
void tc_lcd::dig_sz( unsigned char poz, signed char lens,   signed int   data )
{
  unsigned int dat;
  if (data<0)
  {
    screen[poz] = '-';
    poz++;
    lens--;
    dat = -data;
  }
  else dat = data;
  _digit_uz(poz, lens, &dat);
}
void tc_lcd::dig_sz( unsigned char poz, signed char lens,   signed int  *data )
{
  signed int dat;
  if (*data<0)
  {
    screen[poz] = '-';
    poz++;
    lens--;
    dat = -(*data);
  }
  else dat = *data;
  _digit_uz(poz, lens, &dat);
}
// long
void tc_lcd::dig_sz( unsigned char poz, signed char lens, unsigned long  data )
{
  _digit_uz(poz, lens, &data);
}
void tc_lcd::dig_sz( unsigned char poz, signed char lens, unsigned long *data )
{
  unsigned long dat;
  dat = *data;
  _digit_uz(poz, lens, &dat);
}
void tc_lcd::dig_sz( unsigned char poz, signed char lens,   signed long  data )
{
  unsigned long dat;
  if (data<0)
  {
    screen[poz] = '-';
    poz++;
    lens--;
    dat = -data;
  }
  else dat = data;
  _digit_uz(poz, lens, &dat);
}
void tc_lcd::dig_sz( unsigned char poz, signed char lens,   signed long *data )
{
  unsigned long dat;
  if (*data<0)
  {
    screen[poz] = '-';
    poz++;
    lens--;
    dat = -(*data);
  }
  else dat = *data;
  _digit_uz(poz, lens, &dat);
}

// цыфру на экран с запятой без знаковую c незначащами нулями
// char
void tc_lcd::digit_uz( unsigned char poz, signed char lens, signed char tchk, unsigned char  data )
{
  signed char zap;
  unsigned char dat;
  dat = data;
  zap = lens-tchk;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
void tc_lcd::digit_uz( unsigned char poz, signed char lens, signed char tchk, unsigned char *data )
{
  signed char zap;
  unsigned char dat;
  dat = *data;
  zap = lens-tchk;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
void tc_lcd::digit_uz( unsigned char poz, signed char lens, signed char tchk,   signed char  data )
{
  signed char zap;
  unsigned char dat;
  zap = lens-tchk;
  if (data<0)
  {
    _digit_minus(poz+zap, tchk);
    ShowChar(poz+zap-1, '.');
    _digit_minus(poz, zap-1);
  }
  else
  {
    dat = data;
    _digit_uz(poz+zap, tchk, &dat);
    ShowChar(poz+zap-1, '.');
    _digit_uz(poz, zap-1, &dat);
  }
}
void tc_lcd::digit_uz( unsigned char poz, signed char lens, signed char tchk,   signed char *data )
{
  signed char zap;
  unsigned char dat;
  zap = lens-tchk;
  if (*data<0)
  {
    _digit_minus(poz+zap, tchk);
    ShowChar(poz+zap-1, '.');
    _digit_minus(poz, zap-1);
  }
  else
  {
    dat = *data;
    _digit_uz(poz+zap, tchk, &dat);
    ShowChar(poz+zap-1, '.');
    _digit_uz(poz, zap-1, &dat);
  }
}
// int
void tc_lcd::digit_uz( unsigned char poz, signed char lens, signed char tchk, unsigned int  data )
{
  signed char zap;
  unsigned int dat;
  dat = data;
  zap = lens-tchk;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
void tc_lcd::digit_uz( unsigned char poz, signed char lens, signed char tchk, unsigned int *data )
{
  signed char zap;
  unsigned int dat;
  dat = *data;
  zap = lens-tchk;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
void tc_lcd::digit_uz( unsigned char poz, signed char lens, signed char tchk,   signed int  data )
{
  signed char zap;
  unsigned int dat;
  zap = lens-tchk;
  if (data<0)
  {
    _digit_minus(poz+zap, tchk);
    ShowChar(poz+zap-1, '.');
    _digit_minus(poz, zap-1);
  }
  else
  {
    dat = data;
    _digit_uz(poz+zap, tchk, &dat);
    ShowChar(poz+zap-1, '.');
    _digit_uz(poz, zap-1, &dat);
  }
}
void tc_lcd::digit_uz( unsigned char poz, signed char lens, signed char tchk,   signed int *data )
{
  signed char zap;
  unsigned int dat;
  zap = lens-tchk;
  if (*data<0)
  {
    _digit_minus(poz+zap, tchk);
    ShowChar(poz+zap-1, '.');
    _digit_minus(poz, zap-1);
  }
  else
  {
    dat = *data;
    _digit_uz(poz+zap, tchk, &dat);
    ShowChar(poz+zap-1, '.');
    _digit_uz(poz, zap-1, &dat);
  }
}
// long
void tc_lcd::digit_uz( unsigned char poz, signed char lens, signed char tchk, unsigned long  data )
{
  signed char zap;
  unsigned long dat;
  zap = lens-tchk;
  dat = data;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
void tc_lcd::digit_uz( unsigned char poz, signed char lens, signed char tchk, unsigned long *data )
{
  signed char zap;
  unsigned long dat;
  zap = lens-tchk;
  dat = *data;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
void tc_lcd::digit_uz( unsigned char poz, signed char lens, signed char tchk,   signed long  data )
{
  signed char zap;
  unsigned long dat;
  zap = lens-tchk;
  if (data<0)
  {
    _digit_minus(poz+zap, tchk);
    ShowChar(poz+zap-1, '.');
    _digit_minus(poz, zap-1);
  }
  else
  {
    dat = data;
    _digit_uz(poz+zap, tchk, &dat);
    ShowChar(poz+zap-1, '.');
    _digit_uz(poz, zap-1, &dat);
  }
}
void tc_lcd::digit_uz( unsigned char poz, signed char lens, signed char tchk,   signed long *data )
{
  signed char zap;
  unsigned long dat;
  zap = lens-tchk;
  if (*data<0)
  {
    _digit_minus(poz+zap, tchk);
    ShowChar(poz+zap-1, '.');
    _digit_minus(poz, zap-1);
  }
  else
  {
    dat = *data;
    _digit_uz(poz+zap, tchk, &dat);
    ShowChar(poz+zap-1, '.');
    _digit_uz(poz, zap-1, &dat);
  }
}
// цифру на экран с запятой, знаковую, c незначащими нулями
// char
void tc_lcd::digit_sz( unsigned char poz, signed char lens, signed char tchk, unsigned char  data )
{
  signed char zap;
  unsigned char dat;
  dat = data;
  zap = lens-tchk;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
void tc_lcd::digit_sz( unsigned char poz, signed char lens, signed char tchk, unsigned char *data )
{
  signed char zap;
  unsigned char dat;
  dat = *data;
  zap = lens-tchk;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
void tc_lcd::digit_sz( unsigned char poz, signed char lens, signed char tchk,   signed char  data )
{
  signed char zap;
  unsigned char dat;
  if (data<0)
  {
    screen[poz] = '-';
    poz++;
    lens--;
    dat = -data;
  }
  else dat = data;
  zap = lens-tchk;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
void tc_lcd::digit_sz( unsigned char poz, signed char lens, signed char tchk,   signed char *data )
{
  signed char zap;
  unsigned char dat;
  if (*data<0)
  {
    screen[poz] = '-';
    poz++;
    lens--;
    dat = -(*data);
  }
  else dat = *data;
  zap = lens-tchk;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
// int
void tc_lcd::digit_sz( unsigned char poz, signed char lens, signed char tchk, unsigned int   data )
{
  signed char zap;
  unsigned int dat;
  dat = data;
  zap = lens-tchk;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
void tc_lcd::digit_sz( unsigned char poz, signed char lens, signed char tchk, unsigned int  *data )
{
  signed char zap;
  unsigned int dat;
  dat = *data;
  zap = lens-tchk;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
void tc_lcd::digit_sz( unsigned char poz, signed char lens, signed char tchk,   signed int   data )
{
  signed char zap;
  unsigned int dat;
  if (data<0)
  {
    screen[poz] = '-';
    poz++;
    lens--;
    dat = -data;
  }
  else dat = data;
  zap = lens-tchk;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
void tc_lcd::digit_sz( unsigned char poz, signed char lens, signed char tchk,   signed int  *data )
{
  signed char zap;
  unsigned int dat;
  if (*data<0)
  {
    screen[poz] = '-';
    poz++;
    lens--;
    dat = -(*data);
  }
  else dat = *data;
  zap = lens-tchk;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
// long
void tc_lcd::digit_sz( unsigned char poz, signed char lens, signed char tchk, unsigned long  data )
{
  signed char zap;
  unsigned long dat;
  dat = data;
  zap = lens-tchk;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
void tc_lcd::digit_sz( unsigned char poz, signed char lens, signed char tchk, unsigned long *data )
{
  signed char zap;
  unsigned int dat;
  dat = *data;
  zap = lens-tchk;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
void tc_lcd::digit_sz( unsigned char poz, signed char lens, signed char tchk,   signed long  data )
{
  signed char zap;
  unsigned long dat;
  if (data<0)
  {
    screen[poz] = '-';
    poz++;
    lens--;
    dat = -data;
  }
  else dat = data;
  zap = lens-tchk;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}
void tc_lcd::digit_sz( unsigned char poz, signed char lens, signed char tchk,   signed long *data )
{
  signed char zap;
  unsigned long dat;
  if (*data<0)
  {
    screen[poz] = '-';
    poz++;
    lens--;
    dat = -(*data);
  }
  else dat = *data;
  zap = lens-tchk;
  _digit_uz(poz+zap, tchk, &dat);
  ShowChar(poz+zap-1, '.');
  _digit_uz(poz, zap-1, &dat);
}


// hex for byte
void tc_lcd::Hex( unsigned char poz, unsigned char  data)
{
  screen[poz+0] = f_dec( data>>4 );
  screen[poz+1] = f_dec( data    );
}
void tc_lcd::Hex( unsigned char poz, unsigned char *data)
{
  screen[poz+0] = f_dec( (*data)>>4 );
  screen[poz+1] = f_dec(  *data     );
}
// decad
unsigned char tc_lcd::f_dec(unsigned char dec)
{
  dec &= 0x0f;
  if (dec>9) dec += 7;
  return dec + '0';
}
//
  __flash unsigned char symbol_work[8] =
  { 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00 };
void tc_lcd::LoadSymbol(void)
{
  unsigned char cs_ls;
  cs_ls = __save_interrupt();
  __disable_interrupt();
  f_SendCommand(0x40);
  _delay_ms(1);
  for (unsigned char i=0;i<8;i++)
  {
    f_SendData(symbol_work[i]);
    _delay_us(15);
  }
  f_SendCommand(0x80);
  _delay_ms(1);
  __restore_interrupt(cs_ls);
}


#undef lcd_d_ddr
#undef lcd_d_port
#undef lcd_d_pin
#undef lcd_ei_ddr
#undef lcd_ei_port
#undef lcd_ei_pin
#undef lcd_res_ddr
#undef lcd_res_port
#undef lcd_res_pin


#endif

