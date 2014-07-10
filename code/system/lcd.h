
#ifndef lcd__h
#define lcd__h
#ifdef LCD
// ==========================
#define c_strok 2
#define c_stolbcov 16
//unsigned char const lcd_dlina = c_strok*c_stolbcov;
#define FlashFloorCount  512
#define FlashMaxCount  1024
#if (FlashFloorCount>=FlashMaxCount)
#error Flash Parametr Error
#endif

class tc_lcd
{
private:
  // массив копия экрана
  unsigned char screen[c_strok*c_stolbcov];
#if (c_stolbcov>16)
  unsigned long ScrFlash1;
  unsigned long ScrFlash2;
#else
  unsigned int ScrFlash1;
  unsigned int ScrFlash2;
  unsigned int *ScrFlashx;
#endif
#if (FlashMaxCount<256)
  unsigned char FlashCount;
#else
  unsigned int FlashCount;
#endif
  // указатель для регенерации экрана
  unsigned char *u_RefResh;
  // текущая строка и столбец
  unsigned char cur_stroka, cur_stolbec;
  // очередной символ для регенерации
  unsigned char TempSimbol;
  static __flash unsigned char TableDecode[];

  static char __flash Create_msg1[];
  
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
  //
  void LoadSymbol(void);
  // выдать n раз символ
  void CloneSymbol(unsigned char poz, unsigned char lens, char symbol);
  // счетчик n раз выдавать с миганием
  unsigned char Flash;
  // выдача символа
  void F_Char(unsigned char  poz, char Simbol);
  void   Char(unsigned char  poz, char Simbol);
  void   Char(unsigned char *poz, char Simbol);
  // выдача строки
  void F_String(unsigned char  poz, char *Stroka);
  void F_String(unsigned char  poz, char __flash *Stroka);
  void   String(unsigned char *poz, char *Stroka);
  void   String(unsigned char *poz, char __flash *Stroka);
  // сброс мигания
  void ClearFlash(unsigned char poz);
  void ClearFlash(unsigned char poz, unsigned char len);
  void ClearFlashAll();
  // установка мигания
  void SetFlash(unsigned char poz);
  void SetFlash(unsigned char poz, unsigned char len);
  // быстрая выдача цифр
  void F_Digit_uz(unsigned char poz, unsigned char len, unsigned char cif);
  void F_Digit_uz(unsigned char poz, unsigned char len, unsigned  int cif);
  void F_Digit_u (unsigned char poz, unsigned char len, unsigned char cif);
  void F_Digit_u (unsigned char poz, unsigned char len, unsigned  int cif);
  void F_Digit_sz(unsigned char poz, unsigned char len,   signed char cif);
  void F_Digit_sz(unsigned char poz, unsigned char len,   signed  int cif);
  void F_Digit_s (unsigned char poz, unsigned char len,   signed char cif);
  void F_Digit_s (unsigned char poz, unsigned char len,   signed  int cif);
  // выдача цифр
  void Digit_uz(unsigned char  poz, unsigned char len, unsigned long cif);
  void Digit_uz(unsigned char *poz, unsigned char len, unsigned long cif);
  void Digit_u (unsigned char  poz, unsigned char len, unsigned long cif);
  void Digit_u (unsigned char *poz, unsigned char len, unsigned long cif);
  void Digit_sz(unsigned char  poz, unsigned char len,   signed long cif);
  void Digit_sz(unsigned char *poz, unsigned char len,   signed long cif);
  void Digit_s (unsigned char  poz, unsigned char len,   signed long cif);
  void Digit_s (unsigned char *poz, unsigned char len,   signed long cif);
  // hex for byte
  void Hex(unsigned char  poz, unsigned char cif);
  void Hex2(unsigned char  poz, unsigned int cif);
};
#endif
#endif

