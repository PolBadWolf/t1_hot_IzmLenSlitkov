
#ifndef menu__h
#define menu__h
#ifdef MENU
namespace ns_menu
{
  void timer(void);
  void main(void);
  void zero();
  void init(void);
  struct T_psw {
    unsigned char pin[5];
    unsigned char mask;
  };
  extern __eeprom T_psw ee_psw[];
}
#endif
#endif

