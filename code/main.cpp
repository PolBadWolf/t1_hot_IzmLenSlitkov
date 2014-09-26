
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
  ns_izmlen1::datTimeMassive[0][1] = 24305;
  ns_izmlen1::datTimeMassive[1][0] = 0;
  ns_izmlen1::datTimeMassive[1][1] = 0;
  ns_izmlen1::datTimeMassive[2][0] = 6255;
  ns_izmlen1::datTimeMassive[2][1] = 30561;
  ns_izmlen1::datTimeMassive[3][0] = 9383;
  ns_izmlen1::datTimeMassive[3][1] = 33687;
  ns_izmlen1::datTimeMassive[4][0] = 12509;
  ns_izmlen1::datTimeMassive[4][1] = 36814;
  ns_izmlen1::datTimeMassive[5][0] = 15479;
  ns_izmlen1::datTimeMassive[5][1] = 39785;
  ns_izmlen1::datTimeMassive[6][0] = 30961;
  ns_izmlen1::datTimeMassive[6][1] = 55267;
  ns_izmlen1::datTimeMassive[7][0] = 29238;
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
