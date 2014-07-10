
#include "main.h"
#include "object.h"

namespace og
{
  // фотодатчики
//  tc_ports *PhotoDat[5];
  // tc_ports *Fort;
  // tc_ports *Otop;
  //tc_ports *D_Tahometr;
  //tc_ports *D_Tok;
  // tc_ports *Otop;
  // "датчики" имитации прохождения загатовки
  tc_ports *ImitDat[8];

  void init(void)
  {
    //D_Tahometr = new tc_ports('c', 0, 0, PorogDatch);
    //D_Tok      = new tc_ports('c', 1, 0, PorogDatch);
    //D_Tahometr->pout(1);
    //D_Tok->pout(1);
    ImitDat[0] = new tc_ports('f', 0, 1, PorogDatch, 0);
    ImitDat[1] = new tc_ports('f', 1, 1, PorogDatch, 1);
    ImitDat[2] = new tc_ports('f', 2, 1, PorogDatch, 2);
    ImitDat[3] = new tc_ports('f', 3, 1, PorogDatch, 3);
    ImitDat[4] = new tc_ports('f', 4, 1, PorogDatch, 4);
    ImitDat[5] = new tc_ports('f', 5, 1, PorogDatch, 5);
    ImitDat[6] = new tc_ports('f', 6, 1, PorogDatch, 6);
    ImitDat[7] = new tc_ports('f', 7, 1, PorogDatch, 7);
  }
  void timer()
  {
    //D_Tahometr->for_timer();
    //D_Tok->for_timer();
  }
}
