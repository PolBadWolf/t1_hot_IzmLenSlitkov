
#include "main.h"

//#include "clock.c"

unsigned int  acp0d;
unsigned int  acp0f = false;
unsigned int  acp0_mas[20] = {1023};
unsigned char acp0_ind = 0;
unsigned int  acp0_min = 1023;
unsigned long datt;
unsigned int zad =0;
//__eeprom unsigned char ee_FlagRes = 0x34;

void main(void)
{
  {
    void InitSystem(void);
    InitSystem();
  }
  __enable_interrupt();
  scr->LoadSymbol();
  ad420_Init();
  _delay_ms(1000);
  scr->Clear();
  //scr->dig_uz(0,10, vg::DD_save );
  scr->ShowString(c_stolbcov+0, "S R P");
  while(true)
  {
  ns_ReadDat::cycl();
  ImitWorkCycl();
    #ifdef MENU
    ns_menu::main();
    #endif
    //=============
  }
}
