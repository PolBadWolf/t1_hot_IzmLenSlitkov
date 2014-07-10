
#include "main.h"

void main(void)
{
  {
    void InitSystem(void);
    InitSystem();
  }
  scr->F_Digit_uz( (unsigned char)0, (unsigned char)4, (unsigned int)1234);
  __enable_interrupt();
  _delay_ms(1000);
  scr->LoadSymbol();
  ad420_Init();
#ifdef MENU
  ns_menu::init();
#endif
  //=============
  // scr->Clear();
//  ns_izmlen::ShowDat();
  //scr->dig_uz(0,10, vg::DD_save );
  //scr->F_String(c_stolbcov+0, "S R P");
  while(true)
  {
  //ns_ReadDat::cycl();
  //ImitWorkCycl();
    ns_izmlen::main();
    #ifdef MENU
    ns_menu::main();
    #endif
    //=============
  }
}
