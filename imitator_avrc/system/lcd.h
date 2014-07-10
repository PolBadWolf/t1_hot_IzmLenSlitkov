
#ifndef lcd__h
#define lcd__h
// ==========================
#define c_strok 2
#define c_stolbcov 16
//unsigned char const lcd_dlina = c_strok*c_stolbcov;

class tc_lcd
{
private:
  // массив копия экрана
  unsigned char screen[c_strok*c_stolbcov];
  // указатель для регенерации экрана
  unsigned char *u_RefResh;
  // текущая строка и столбец
  unsigned char cur_stroka, cur_stolbec;
  // очередной символ для регенерации
  unsigned char TempSimbol;
  static __flash unsigned char TableDecode[];
  
  // настройка портов
  void f_InitPorts(void);
  // настройка переменных
  void f_InitVar(void);
  void f_InitLcd(void);
  // передача в lcd команды
  void f_SendCommand(unsigned char data);
  // передача в lcd данных
  void f_SendData(unsigned char data);
  // передача в lcd
  void f_Send(unsigned char data);
  //
  void _digit_minus( unsigned char poz, signed char lens );
  // с незначащим нулем
  void _digit_uz( unsigned char poz, signed char lens, unsigned char *data );
  void _digit_uz( unsigned char poz, signed char lens,   signed char *data );
  void _digit_uz( unsigned char poz, signed char lens, unsigned int  *data );
  void _digit_uz( unsigned char poz, signed char lens,   signed int  *data );
  void _digit_uz( unsigned char poz, signed char lens, unsigned long *data );
  void _digit_uz( unsigned char poz, signed char lens,   signed long *data );
  // без незначащего нуля
  void _digit_u( unsigned char poz, signed char lens, unsigned char *data );
  void _digit_u( unsigned char poz, signed char lens,   signed char *data );
  void _digit_u( unsigned char poz, signed char lens, unsigned int  *data );
  void _digit_u( unsigned char poz, signed char lens,   signed int  *data );
  void _digit_u( unsigned char poz, signed char lens, unsigned long *data );
  void _digit_u( unsigned char poz, signed char lens,   signed long *data );
  // decad
  unsigned char f_dec(unsigned char dec);
  // old send
  unsigned char v_old_send;
public:
  // инициализация
  tc_lcd();
  // для обновления
  void for_timer(void);
  // очистка экрана
  void Clear(void);
  // вывод символа
  void ShowChar(unsigned char position, unsigned char simbol);
  // вывод строки
  void ShowString(unsigned char position, char *stroka);
  // цифру на экран без знаковую, c незначащими нулями
  // char
  void dig_uz( unsigned char poz, signed char lens, unsigned char  data );
  void dig_uz( unsigned char poz, signed char lens, unsigned char *data );
  void dig_uz( unsigned char poz, signed char lens,   signed char  data );
  void dig_uz( unsigned char poz, signed char lens,   signed char *data );
  // int
  void dig_uz( unsigned char poz, signed char lens, unsigned int   data );
  void dig_uz( unsigned char poz, signed char lens, unsigned int  *data );
  void dig_uz( unsigned char poz, signed char lens,   signed int   data );
  void dig_uz( unsigned char poz, signed char lens,   signed int  *data );
  // long
  void dig_uz( unsigned char poz, signed char lens, unsigned long  data );
  void dig_uz( unsigned char poz, signed char lens, unsigned long *data );
  void dig_uz( unsigned char poz, signed char lens,   signed long  data );
  void dig_uz( unsigned char poz, signed char lens,   signed long *data );
  // цифру на экран без знаковую, без незначащих нулей
  // char
  void dig_u( unsigned char poz, signed char lens, unsigned char  data );
  void dig_u( unsigned char poz, signed char lens, unsigned char *data );
  void dig_u( unsigned char poz, signed char lens,   signed char  data );
  void dig_u( unsigned char poz, signed char lens,   signed char *data );
  // int
  void dig_u( unsigned char poz, signed char lens, unsigned int   data );
  void dig_u( unsigned char poz, signed char lens, unsigned int  *data );
  void dig_u( unsigned char poz, signed char lens,   signed int   data );
  void dig_u( unsigned char poz, signed char lens,   signed int  *data );
  // long
  void dig_u( unsigned char poz, signed char lens, unsigned long  data );
  void dig_u( unsigned char poz, signed char lens, unsigned long *data );
  void dig_u( unsigned char poz, signed char lens,   signed long  data );
  void dig_u( unsigned char poz, signed char lens,   signed long *data );
  // цифру на экран знаковую, c незначащими нулями
  // char
  void dig_sz( unsigned char poz, signed char lens, unsigned char  data );
  void dig_sz( unsigned char poz, signed char lens, unsigned char *data );
  void dig_sz( unsigned char poz, signed char lens,   signed char  data );
  void dig_sz( unsigned char poz, signed char lens,   signed char *data );
  // int
  void dig_sz( unsigned char poz, signed char lens, unsigned int   data );
  void dig_sz( unsigned char poz, signed char lens, unsigned int  *data );
  void dig_sz( unsigned char poz, signed char lens,   signed int   data );
  void dig_sz( unsigned char poz, signed char lens,   signed int  *data );
  // long
  void dig_sz( unsigned char poz, signed char lens, unsigned long  data );
  void dig_sz( unsigned char poz, signed char lens, unsigned long *data );
  void dig_sz( unsigned char poz, signed char lens,   signed long  data );
  void dig_sz( unsigned char poz, signed char lens,   signed long *data );
  // цыфру на экран с запятой без знаковую c незначащами нулями
  // char
  void digit_uz( unsigned char poz, signed char lens, signed char tchk, unsigned char  data );
  void digit_uz( unsigned char poz, signed char lens, signed char tchk, unsigned char *data );
  void digit_uz( unsigned char poz, signed char lens, signed char tchk,   signed char  data );
  void digit_uz( unsigned char poz, signed char lens, signed char tchk,   signed char *data );
  // int
  void digit_uz( unsigned char poz, signed char lens, signed char tchk, unsigned int   data );
  void digit_uz( unsigned char poz, signed char lens, signed char tchk, unsigned int  *data );
  void digit_uz( unsigned char poz, signed char lens, signed char tchk,   signed int   data );
  void digit_uz( unsigned char poz, signed char lens, signed char tchk,   signed int  *data );
  // long
  void digit_uz( unsigned char poz, signed char lens, signed char tchk, unsigned long  data );
  void digit_uz( unsigned char poz, signed char lens, signed char tchk, unsigned long *data );
  void digit_uz( unsigned char poz, signed char lens, signed char tchk,   signed long  data );
  void digit_uz( unsigned char poz, signed char lens, signed char tchk,   signed long *data );
  // цыфру на экран с запятой без знаковую без незначащих нулей
  // char
  void digit_u( unsigned char poz, signed char lens, signed char tchk, unsigned char  data );
  void digit_u( unsigned char poz, signed char lens, signed char tchk, unsigned char *data );
  void digit_u( unsigned char poz, signed char lens, signed char tchk,   signed char  data );
  void digit_u( unsigned char poz, signed char lens, signed char tchk,   signed char *data );
  // int
  void digit_u( unsigned char poz, signed char lens, signed char tchk, unsigned int   data );
  void digit_u( unsigned char poz, signed char lens, signed char tchk, unsigned int  *data );
  void digit_u( unsigned char poz, signed char lens, signed char tchk,   signed int   data );
  void digit_u( unsigned char poz, signed char lens, signed char tchk,   signed int  *data );
  // long
  void digit_u( unsigned char poz, signed char lens, signed char tchk, unsigned long  data );
  void digit_u( unsigned char poz, signed char lens, signed char tchk, unsigned long *data );
  void digit_u( unsigned char poz, signed char lens, signed char tchk,   signed long  data );
  void digit_u( unsigned char poz, signed char lens, signed char tchk,   signed long *data );
  // цифру на экран с запятой, знаковую, c незначащими нулями
  // char
  void digit_sz( unsigned char poz, signed char lens, signed char tchk, unsigned char  data );
  void digit_sz( unsigned char poz, signed char lens, signed char tchk, unsigned char *data );
  void digit_sz( unsigned char poz, signed char lens, signed char tchk,   signed char  data );
  void digit_sz( unsigned char poz, signed char lens, signed char tchk,   signed char *data );
  // int
  void digit_sz( unsigned char poz, signed char lens, signed char tchk, unsigned int   data );
  void digit_sz( unsigned char poz, signed char lens, signed char tchk, unsigned int  *data );
  void digit_sz( unsigned char poz, signed char lens, signed char tchk,   signed int   data );
  void digit_sz( unsigned char poz, signed char lens, signed char tchk,   signed int  *data );
  // long
  void digit_sz( unsigned char poz, signed char lens, signed char tchk, unsigned long  data );
  void digit_sz( unsigned char poz, signed char lens, signed char tchk, unsigned long *data );
  void digit_sz( unsigned char poz, signed char lens, signed char tchk,   signed long  data );
  void digit_sz( unsigned char poz, signed char lens, signed char tchk,   signed long *data );
  // hex for byte
  void Hex( unsigned char poz, unsigned char  data);
  void Hex( unsigned char poz, unsigned char *data);
  //
  void LoadSymbol(void);
};
#endif

