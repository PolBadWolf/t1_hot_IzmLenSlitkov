
#include "../main.h"
#ifdef LCD

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


char __flash tc_lcd::Create_msg1[] = "GM code";
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
  F_String(0, Create_msg1);
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
  ScrFlash1 = 0;
  ScrFlash2 = 0;
  Flash = 0;
  FlashCount = 0;
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
  unsigned char FlTmp;
  FlashCount++;
  if (FlashCount>=FlashMaxCount)
    FlashCount = 0;
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
  if (cur_stroka==0)
    FlTmp = (ScrFlash1 & (1<<cur_stolbec) ) && (FlashCount>FlashFloorCount);
  else
    FlTmp = (ScrFlash2 & (1<<cur_stolbec) ) && (FlashCount>FlashFloorCount);
  // регенерация одного символа
  //u_RefResh =  &screen[0]+(cur_stolbec + cur_stroka*c_stolbcov);
  TempSimbol = *u_RefResh;
  if (TempSimbol>128) TempSimbol = TableDecode[TempSimbol-128];
  if (FlTmp)          TempSimbol = ' ';
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


//
  __flash unsigned char symbol_work[16] =
  { 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x0f, 0x0f, 0x06, 0x00, 0x00, 0x00, 0x00 };
void tc_lcd::LoadSymbol(void)
{
  unsigned char cs_ls;
  cs_ls = __save_interrupt();
  __disable_interrupt();
  f_SendCommand(0x40);
  _delay_ms(1);
  for (unsigned char i=0;i<16;i++)
  {
    f_SendData(symbol_work[i]);
    _delay_us(15);
  }
  f_SendCommand(0x80);
  _delay_ms(1);
  __restore_interrupt(cs_ls);
}
// ===========================================================================================================================
void tc_lcd::F_Char(unsigned char poz, char Simbol)
{
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  screen[poz] = (unsigned char)Simbol;
}
// ===========================================================================================================================
void tc_lcd::Char(unsigned char *poz, char Simbol)
{
#if (c_stolbcov>16)
  unsigned long *ScrFlashx;
#else
  unsigned int *ScrFlashx;
#endif
  unsigned char b;
  if (*poz>=(c_strok*c_stolbcov) ) *poz = 0;
  screen[*poz] = (unsigned char)Simbol;
  if (*poz<c_stolbcov)
  {
    ScrFlashx = &ScrFlash1;
    b = *poz;
  }
  else
  {
    ScrFlashx = &ScrFlash2;
    b = *poz - c_stolbcov;
  }
  if (Flash>0)
  {
    *ScrFlashx = *ScrFlashx | (1<<b);
    Flash--;
  }
  else
  {
    *ScrFlashx = *ScrFlashx & ~(1<<b);
  }
  poz++;
  if (*poz>=(c_strok*c_stolbcov) ) *poz = 0;
}
// ===========================================================================================================================
void tc_lcd::Char(unsigned char poz, char Simbol)
{
#if (c_stolbcov>16)
  unsigned long *ScrFlashx;
#else
  unsigned int *ScrFlashx;
#endif
  unsigned char b;
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  screen[poz] = (unsigned char)Simbol;
  if (poz<c_stolbcov)
  {
    ScrFlashx = &ScrFlash1;
    b = poz;
  }
  else
  {
    ScrFlashx = &ScrFlash2;
    b = poz - c_stolbcov;
  }
  if (Flash>0)
  {
    *ScrFlashx = *ScrFlashx | (1<<b);
    Flash--;
  }
  else
  {
    *ScrFlashx = *ScrFlashx & ~(1<<b);
  }
}
// ===========================================================================================================================
void tc_lcd::F_String(unsigned char  poz, char *Stroka)
{
  unsigned char s = (unsigned char)*Stroka;
  while (s)
  {
    if (poz>=(c_strok*c_stolbcov) ) poz = 0;
    screen[poz] = s;
    poz++;
    Stroka++;
    s = (unsigned char)*Stroka;
  }
}
// ===========================================================================================================================
void tc_lcd::F_String(unsigned char  poz, char __flash *Stroka)
{
  unsigned char s = (unsigned char)*Stroka;
  while (s)
  {
    if (poz>=(c_strok*c_stolbcov) ) poz = 0;
    screen[poz] = s;
    poz++;
    Stroka++;
    s = (unsigned char)*Stroka;
  }
}
// ===========================================================================================================================
void tc_lcd::String(unsigned char *poz, char *Stroka)
{
#if (c_stolbcov>16)
  unsigned long *ScrFlashx;
#else
  unsigned int *ScrFlashx;
#endif
  unsigned char b;
  unsigned char s = (unsigned char)*Stroka;
  while (s)
  {
    if (*poz>=(c_strok*c_stolbcov) ) *poz = 0;
    screen[*poz] = s;
    if (*poz<c_stolbcov)
    {
      ScrFlashx = &ScrFlash1;
      b = *poz;
    }
    else
    {
      ScrFlashx = &ScrFlash2;
      b = *poz - c_stolbcov;
    }
    if (Flash>0)
      *ScrFlashx = *ScrFlashx | (1<<b);
    else
      *ScrFlashx = *ScrFlashx & ~(1<<b);
    *poz++;
    Stroka++;
    s = (unsigned char)*Stroka;
  }
  if (*poz>=(c_strok*c_stolbcov) ) *poz = 0;
  if (Flash>0)
    Flash--;
}
// ===========================================================================================================================
void tc_lcd::String(unsigned char *poz, char __flash *Stroka)
{
#if (c_stolbcov>16)
  unsigned long *ScrFlashx;
#else
  unsigned int *ScrFlashx;
#endif
  unsigned char b;
  unsigned char s = (unsigned char)*Stroka;
  while (s)
  {
    if (*poz>=(c_strok*c_stolbcov) ) *poz = 0;
    screen[*poz] = s;
    if (*poz<c_stolbcov)
    {
      ScrFlashx = &ScrFlash1;
      b = *poz;
    }
    else
    {
      ScrFlashx = &ScrFlash2;
      b = *poz - c_stolbcov;
    }
    if (Flash>0)
      *ScrFlashx = *ScrFlashx | (1<<b);
    else
      *ScrFlashx = *ScrFlashx & ~(1<<b);
    *poz++;
    Stroka++;
    s = (unsigned char)*Stroka;
  }
  if (*poz>=(c_strok*c_stolbcov) ) *poz = 0;
  if (Flash>0)
    Flash--;
}
// ===========================================================================================================================
void tc_lcd::ClearFlash(unsigned char poz)
{
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  if (poz<c_stolbcov)
    ScrFlash1 = ScrFlash1 & ~(1<<poz);
  else
    ScrFlash2 = ScrFlash2 & ~(1<<(poz-c_stolbcov) );
}
// ===========================================================================================================================
void tc_lcd::ClearFlash(unsigned char poz, unsigned char len)
{
  for(unsigned char i=0; i<len; i++)
  {
    if (poz>=(c_strok*c_stolbcov) ) poz = 0;
    if (poz<c_stolbcov)
      ScrFlash1 = ScrFlash1 & ~(1<<poz);
    else
      ScrFlash2 = ScrFlash2 & ~(1<<(poz-c_stolbcov) );
    poz++;
  }
}
// ===========================================================================================================================
void tc_lcd::ClearFlashAll()
{
  ScrFlash1 = 0;
  ScrFlash2 = 0;
  Flash = 0;
}
// ===========================================================================================================================
void tc_lcd::SetFlash(unsigned char poz)
{
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  if (poz<c_stolbcov)
    ScrFlash1 = ScrFlash1 | (1<<poz);
  else
    ScrFlash2 = ScrFlash2 | (1<<(poz-c_stolbcov) );
}
// ===========================================================================================================================
void tc_lcd::SetFlash(unsigned char poz, unsigned char len)
{
  for(unsigned char i=0; i<len; i++)
  {
    if (poz>=(c_strok*c_stolbcov) ) poz = 0;
    if (poz<c_stolbcov)
      ScrFlash1 = ScrFlash1 | (1<<poz);
    else
      ScrFlash2 = ScrFlash2 | (1<<(poz-c_stolbcov) );
    poz++;
  }
}
// ===========================================================================================================================
void tc_lcd::Clear(void)
{
  for (unsigned char i=0;i<(c_strok*c_stolbcov);i++) screen[i] = ' ';
  ClearFlashAll();
}
// ===========================================================================================================================
void tc_lcd::F_Digit_uz(unsigned char poz, unsigned char len, unsigned char cif)
{
  signed char poz1 = poz+len-1;
  unsigned char cif_low;
  for(unsigned char i=0; i<len; i++)
  {
    cif_low = cif % 10;
    if (poz1<0) poz1 = (c_strok*c_stolbcov)-1;
    if (poz1>=(c_strok*c_stolbcov) ) poz1 = 0;
    screen[poz1--] = '0' + cif_low;
    cif = cif / 10;
  }
}
// ===========================================================================================================================
void tc_lcd::F_Digit_uz(unsigned char poz, unsigned char len, unsigned  int cif)
{
  signed char poz1 = poz+len-1;
  unsigned char cif_low;
  for(unsigned char i=0; i<len; i++)
  {
    cif_low = cif % 10;
    if (poz1<0) poz1 = (c_strok*c_stolbcov)-1;
    if (poz1>=(c_strok*c_stolbcov) ) poz1 = 0;
    screen[poz1--] = '0' + cif_low;
    cif = cif / 10;
  }
}
// ===========================================================================================================================
void tc_lcd::F_Digit_u (unsigned char poz, unsigned char len, unsigned char cif)
{
  signed char poz1 = poz+len-1;
  unsigned char cif_low;
  for(unsigned char i=0; i<len; i++)
  {
    cif_low = cif % 10;
    if (poz1<0) poz1 = (c_strok*c_stolbcov)-1;
    if (poz1>=(c_strok*c_stolbcov) ) poz1 = 0;
    if ( (i==0) || (cif_low>0) )
      screen[poz1--] = '0' + cif_low;
    else
      screen[poz1--] = ' ';
    cif = cif / 10;
  }
}
// ===========================================================================================================================
void tc_lcd::F_Digit_u (unsigned char poz, unsigned char len, unsigned  int cif)
{
  signed char poz1 = poz+len-1;
  unsigned char cif_low;
  for(unsigned char i=0; i<len; i++)
  {
    cif_low = cif % 10;
    if (poz1<0) poz1 = (c_strok*c_stolbcov)-1;
    if (poz1>=(c_strok*c_stolbcov) ) poz1 = 0;
    if ( (i==0) || (cif!=0) )
      screen[poz1--] = '0' + cif_low;
    else
      screen[poz1--] = ' ';
    cif = cif / 10;
  }
}
// ===========================================================================================================================
void tc_lcd::F_Digit_sz(unsigned char poz, unsigned char len,   signed char cif)
{
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  if (cif<0)
  {
    screen[poz++] = '-';
    if (poz>=(c_strok*c_stolbcov) ) poz = 0;
    F_Digit_uz(poz, (unsigned char)(len-1), (unsigned char)(-cif) );
  }
  else
    F_Digit_uz(poz, len, (unsigned char)cif );
}
// ===========================================================================================================================
void tc_lcd::F_Digit_sz(unsigned char poz, unsigned char len,   signed  int cif)
{
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  if (cif<0)
  {
    screen[poz++] = '-';
    if (poz>=(c_strok*c_stolbcov) ) poz = 0;
    F_Digit_uz(poz, (unsigned char)(len-1), (unsigned char)(-cif) );
  }
  else
    F_Digit_uz(poz, len, (unsigned char)cif );
}
// ===========================================================================================================================
void tc_lcd::F_Digit_s (unsigned char poz, unsigned char len,   signed char cif)
{
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  if (cif<0)
  {
    screen[poz++] = '-';
    if (poz>=(c_strok*c_stolbcov) ) poz = 0;
    F_Digit_uz(poz, (unsigned char)(len-1), (unsigned char)(-cif) );
  }
  else
  {
    F_Digit_u (poz, len, (unsigned char)cif );
  }
}
// ===========================================================================================================================
void tc_lcd::F_Digit_s (unsigned char poz, unsigned char len,   signed  int cif)
{
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  if (cif<0)
  {
    screen[poz++] = '-';
    if (poz>=(c_strok*c_stolbcov) ) poz = 0;
    F_Digit_uz(poz, (unsigned char)(len-1), (unsigned char)(-cif) );
  }
  else
  {
    F_Digit_u (poz, len, (unsigned char)cif );
  }
}
// ===========================================================================================================================
void tc_lcd::Digit_uz(unsigned char  poz, unsigned char len, unsigned long cif)
{
  signed char poz1;
  unsigned char cif_low;
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  if (Flash>0)
  {
    SetFlash(poz, len);
    Flash--;
  }
  else
    ClearFlash(poz, len);
  poz1 = poz+len-1;
  for(unsigned char i=0; i<len; i++)
  {
    if (poz1<0) poz1 = (c_strok*c_stolbcov)-1;
    if (poz1>=(c_strok*c_stolbcov) ) poz1 = 0;
    cif_low = cif % 10;
    screen[poz1--] = '0' + cif_low;
    cif = cif / 10;
  }
}
// ===========================================================================================================================
void tc_lcd::Digit_uz(unsigned char *poz, unsigned char len, unsigned long cif)
{
  unsigned char cif_low;
  signed char poz1;
  if (*poz>=(c_strok*c_stolbcov) ) *poz = 0;
  if (Flash>0)
  {
    SetFlash(*poz, len);
    Flash--;
  }
  else
    ClearFlash(*poz, len);
  *poz = *poz+len;
  if (*poz>=(c_strok*c_stolbcov) ) *poz = 0;
  poz1 = *poz-1;
  for(unsigned char i=0; i<len; i++)
  {
    if (poz1<0) poz1 = (c_strok*c_stolbcov)-1;
    if (poz1>=(c_strok*c_stolbcov) ) poz1 = 0;
    cif_low = cif % 10;
    screen[poz1--] = '0' + cif_low;
    cif = cif / 10;
  }
}
// ===========================================================================================================================
void tc_lcd::Digit_u (unsigned char  poz, unsigned char len, unsigned long cif)
{
  signed char poz1;
  unsigned char cif_low;
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  if (Flash>0)
  {
    SetFlash(poz, len);
    Flash--;
  }
  else
    ClearFlash(poz, len);
  poz = poz+len;
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  poz1 = poz-1;
  for(unsigned char i=0; i<len; i++)
  {
    if (poz1>=(c_strok*c_stolbcov) ) poz1 = 0;
    cif_low = cif % 10;
    if ( (i==0) || (cif>0) )
      screen[poz1] = '0' + cif_low;
    else
      screen[poz1] = ' ';
    //poz++;
    cif = cif / 10;
  }
}
// ===========================================================================================================================
void tc_lcd::Digit_u (unsigned char *poz, unsigned char len, unsigned long cif)
{
  unsigned char poz1, cif_low;
  if (*poz>=(c_strok*c_stolbcov) ) *poz = 0;
  if (Flash>0)
  {
    SetFlash(*poz, len);
    Flash--;
  }
  else
    ClearFlash(*poz, len);
  for(unsigned char i=0; i<len; i++)
  {
    poz1 = *poz+len-(i+1);
    if (poz1>=(c_strok*c_stolbcov) ) poz1 = 0;
    cif_low = cif % 10;
    if ( (i==0) || (cif>0) )
      screen[poz1] = '0' + cif_low;
    else
      screen[poz1] = ' ';
    //*poz++;
    cif = cif / 10;
  }
  if (*poz>=(c_strok*c_stolbcov) ) *poz = 0;
}
// ===========================================================================================================================
void tc_lcd::Digit_sz(unsigned char  poz, unsigned char len, signed long cif)
{
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  if (cif<0)
  {
    screen[poz] = '-';
    if (Flash>0)
      SetFlash(poz);
    else
      ClearFlash(poz);
    poz++;
    Digit_uz(poz, (unsigned char)(len-1), -cif );
  }
  else
    Digit_uz(poz, len, cif );
}
// ===========================================================================================================================
void tc_lcd::Digit_sz(unsigned char *poz, unsigned char len, signed long cif)
{
  if (*poz>=(c_strok*c_stolbcov) ) *poz = 0;
  if (cif<0)
  {
    screen[*poz] = '-';
    if (Flash>0)
      SetFlash(*poz);
    else
      ClearFlash(*poz);
    *poz++;
    Digit_uz(poz, (unsigned char)(len-1), -cif );
  }
  else
    Digit_uz(poz, len, cif );
}
// ===========================================================================================================================
void tc_lcd::Digit_s (unsigned char  poz, unsigned char len,   signed long cif)
{
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  if (cif<0)
  {
    screen[poz] = '-';
    if (Flash>0)
      SetFlash(poz);
    else
      ClearFlash(poz);
    poz++;
    Digit_uz(poz, (unsigned char)(len-1), -cif );
  }
  else
    Digit_u(poz, len, cif);
}
// ===========================================================================================================================
void tc_lcd::Digit_s (unsigned char *poz, unsigned char len,   signed long cif)
{
  if (*poz>=(c_strok*c_stolbcov) ) *poz = 0;
  if (cif<0)
  {
    screen[*poz] = '-';
    if (Flash>0)
      SetFlash(*poz);
    else
      ClearFlash(*poz);
    *poz++;
    Digit_uz(poz, (unsigned char)(len-1), -cif );
  }
  else
    Digit_u(poz, len, cif);
}
// ===========================================================================================================================
// decad
unsigned char tc_lcd::f_dec(unsigned char dec)
{
  dec &= 0x0f;
  if (dec>9) dec += 7;
  return dec + '0';
}
// ===========================================================================================================================
// hex for byte
void tc_lcd::Hex( unsigned char poz, unsigned char cif)
{
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  if (Flash>0)
  {
    SetFlash(poz++, 2);
    Flash--;
  }
  else
    ClearFlash(poz++, 2);
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  screen[poz++] = f_dec( cif>>4 );
  screen[poz++] = f_dec( cif    );
}
// ===========================================================================================================================
void tc_lcd::Hex2( unsigned char poz, unsigned int cif)
{
  unsigned char *uk = ((unsigned char *)&cif);
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  if (Flash>0)
  {
    SetFlash(poz++, 4);
    Flash--;
  }
  else
    ClearFlash(poz++, 4);
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  screen[poz++] = f_dec( uk[0]>>4 );
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  screen[poz++] = f_dec( uk[0] );
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  screen[poz++] = f_dec( uk[1]>>4 );
  if (poz>=(c_strok*c_stolbcov) ) poz = 0;
  screen[poz++] = f_dec( uk[1] );
}
// ===========================================================================================================================
// ===========================================================================================================================
// ===========================================================================================================================
// ===========================================================================================================================
// ===========================================================================================================================
// ===========================================================================================================================
// ===========================================================================================================================
// ===========================================================================================================================
// ===========================================================================================================================



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

