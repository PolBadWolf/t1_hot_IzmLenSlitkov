
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
  /*
//    ns_izmlen1::SendDebug();
  ns_izmlen1::datTimeMassive[0][0] = 1;
  ns_izmlen1::datTimeMassive[0][1] = 1746;
  ns_izmlen1::datTimeMassive[1][0] = 1001;
  ns_izmlen1::datTimeMassive[1][1] = 2747;
  ns_izmlen1::datTimeMassive[2][0] = 2002;
  ns_izmlen1::datTimeMassive[2][1] = 3748;
  ns_izmlen1::datTimeMassive[3][0] = 3002;
  ns_izmlen1::datTimeMassive[3][1] = 4749;
  ns_izmlen1::datTimeMassive[4][0] = 0;
  ns_izmlen1::datTimeMassive[4][1] = 0;
  ns_izmlen1::datTimeMassive[5][0] = 0;
  ns_izmlen1::datTimeMassive[5][1] = 0;
  ns_izmlen1::datTimeMassive[6][0] = 9357;
  ns_izmlen1::datTimeMassive[6][1] = 1110;
  ns_izmlen1::datTimeMassive[7][0] = 9867;
  ns_izmlen1::datTimeMassive[7][1] = 11613;
    ns_izmlen1::IzmRenderMain();
   */
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
