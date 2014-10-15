
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
  ns_izmlen1::datTimeMassive[0][1] = 20216;
  ns_izmlen1::datTimeMassive[1][0] = 3278;
  ns_izmlen1::datTimeMassive[1][1] = 23358;
  ns_izmlen1::datTimeMassive[2][0] = 6362;
  ns_izmlen1::datTimeMassive[2][1] = 26379;
  ns_izmlen1::datTimeMassive[3][0] = 9427;
  ns_izmlen1::datTimeMassive[3][1] = 29459;
  ns_izmlen1::datTimeMassive[4][0] = 12334;
  ns_izmlen1::datTimeMassive[4][1] = 33122;
  ns_izmlen1::datTimeMassive[5][0] = 14942;
  ns_izmlen1::datTimeMassive[5][1] = 38139;
  ns_izmlen1::datTimeMassive[6][0] = 28236;
  ns_izmlen1::datTimeMassive[6][1] = 61812;
  ns_izmlen1::datTimeMassive[7][0] = 29720; //29268;
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
