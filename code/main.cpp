
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
  ns_izmlen1::datTimeMassive[0][1] = 6401;
  ns_izmlen1::datTimeMassive[1][0] = 5006;
  ns_izmlen1::datTimeMassive[1][1] = 11404;
  ns_izmlen1::datTimeMassive[2][0] = 10009;
  ns_izmlen1::datTimeMassive[2][1] = 16407;
  ns_izmlen1::datTimeMassive[3][0] = 15012;
  ns_izmlen1::datTimeMassive[3][1] = 21413;
  ns_izmlen1::datTimeMassive[4][0] = 20015;
  ns_izmlen1::datTimeMassive[4][1] = 26416;
  ns_izmlen1::datTimeMassive[5][0] = 24786;
  ns_izmlen1::datTimeMassive[5][1] = 31169;
  ns_izmlen1::datTimeMassive[6][0] = 46786;
  ns_izmlen1::datTimeMassive[6][1] = 53185;
  ns_izmlen1::datTimeMassive[7][0] = 49337;
  ns_izmlen1::datTimeMassive[7][1] = 55738;
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
