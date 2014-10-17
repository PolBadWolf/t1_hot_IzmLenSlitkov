
#include "main.h"

namespace ns_izmlen1
{
    void SendDebug();
    void IzmRenderMain();
}

void main(void)
{
    {
    void InitSystem(void);
    InitSystem();
  }
  
//    ns_izmlen1::SendDebug();
  ns_izmlen1::datTimeMassive[0][0] = 1;
  ns_izmlen1::datTimeMassive[0][1] = 0x4b5d;
  ns_izmlen1::datTimeMassive[1][0] = 0x0b33;
  ns_izmlen1::datTimeMassive[1][1] = 0x5768;
  ns_izmlen1::datTimeMassive[2][0] = 0x167b;
  ns_izmlen1::datTimeMassive[2][1] = 0x62de;
  ns_izmlen1::datTimeMassive[3][0] = 0x223b;
  ns_izmlen1::datTimeMassive[3][1] = 0x6fff;
  ns_izmlen1::datTimeMassive[4][0] = 0x2ceb;
  ns_izmlen1::datTimeMassive[4][1] = 0x8000;
  ns_izmlen1::datTimeMassive[5][0] = 0x370e;
  ns_izmlen1::datTimeMassive[5][1] = 0x92a1;
  ns_izmlen1::datTimeMassive[6][0] = 0x69f4;
  ns_izmlen1::datTimeMassive[6][1] = 0xe6a3;
  ns_izmlen1::datTimeMassive[7][0] = 0x6fa7; //29268;
  ns_izmlen1::datTimeMassive[7][1] = 0;
    ns_izmlen1::IzmRenderMain();
  
  _delay_ms(1000);
//  scr->F_Digit_uz( (unsigned char)0, (unsigned char)4, (unsigned int)1234);
  __enable_interrupt();
  _delay_ms(1000);
  scr->LoadSymbol();
  ad420_Init();
#ifdef MENU
  ns_menu::init();
#endif
    ns_wdogtimer::enabled();
    //=============
    while(true)
    {
        ns_wdogtimer::reset();
        ns_izmlen::main();
#ifdef MENU
        ns_menu::main();
#endif
    //=============
    }
}
