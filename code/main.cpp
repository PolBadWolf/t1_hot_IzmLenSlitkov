
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
  ns_izmlen1::datTimeMassive[0][1] = 21975;
  ns_izmlen1::datTimeMassive[1][0] = 1500;//2849;
  ns_izmlen1::datTimeMassive[1][1] = 24854;
  ns_izmlen1::datTimeMassive[2][0] = 1600; //5974;
  ns_izmlen1::datTimeMassive[2][1] = 27829;
  ns_izmlen1::datTimeMassive[3][0] = 9165;
  ns_izmlen1::datTimeMassive[3][1] = 30922;
  ns_izmlen1::datTimeMassive[4][0] = 12050;
  ns_izmlen1::datTimeMassive[4][1] = 34509;
  ns_izmlen1::datTimeMassive[5][0] = 14951;
  ns_izmlen1::datTimeMassive[5][1] = 39808;
  ns_izmlen1::datTimeMassive[6][0] = 27861;
  ns_izmlen1::datTimeMassive[6][1] = 56570;
  ns_izmlen1::datTimeMassive[7][0] = 28000; //29268;
  ns_izmlen1::datTimeMassive[7][1] = 0;
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
