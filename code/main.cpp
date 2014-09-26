
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
  ns_izmlen1::datTimeMassive[0][1] = 8871;
  ns_izmlen1::datTimeMassive[1][0] = 3753;
  ns_izmlen1::datTimeMassive[1][1] = 12621;
  ns_izmlen1::datTimeMassive[2][0] = 7506;
  ns_izmlen1::datTimeMassive[2][1] = 16374;
  ns_izmlen1::datTimeMassive[3][0] = 11259;
  ns_izmlen1::datTimeMassive[3][1] = 20127;
  ns_izmlen1::datTimeMassive[4][0] = 15012;
  ns_izmlen1::datTimeMassive[4][1] = 23880;
  ns_izmlen1::datTimeMassive[5][0] = 18765;
  ns_izmlen1::datTimeMassive[5][1] = 27633;
  ns_izmlen1::datTimeMassive[6][0] = 22518;
  ns_izmlen1::datTimeMassive[6][1] = 31386;
  ns_izmlen1::datTimeMassive[7][0] = 23768;
  ns_izmlen1::datTimeMassive[7][1] = 32636;
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
