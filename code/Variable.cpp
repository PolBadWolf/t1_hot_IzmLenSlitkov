
#include "main.h"
#include "variable.h"

namespace vg
{
  // =====================================
//  __eeprom TDateSmena ee_Archiv[ee_ArchivLen] = {0};
  __eeprom unsigned int rs_Dat[8] = {1000, 2000, 3000, 4000, 5000, 5950, 10350, 10850};
  __eeprom unsigned int prcPorog  = 4000; // level 10.0%
  __eeprom unsigned char crutchesTail = 3;
  unsigned int rs_slitok_len = 2500;
  unsigned int rs_slitok_speed = 100;
  __eeprom unsigned char kf_usr = 20;
  char teleMsg[10] = "FN+sS ";
  unsigned int upTimeMS;
  unsigned char upTimeS, upTimeM, upTimeH, upTimeD, upTimeSN;
  // =====================================
  void init()
  {
      for (unsigned char i=5; i<10; i++)
          teleMsg[i] = 0;
      {
          CritSec cs_upt;
          upTimeMS = 0;
          upTimeS  = 0;
          upTimeM  = 0;
          upTimeH  = 0;
          upTimeD  = 0;
          upTimeSN = 1;
      }
  }
}

/*
template <typename T>
inline void CritVarWrite(T *variable, T *newDat)
{
    CritSec cs;
    *variable = *newDat;
}
*/
/*template <class T>
T CritVarRead(T *variable)
{
    CritSec cs;
    return *variable;
}
*/
//template <class T>
/*
unsigned char CritVarRead(unsigned char *variable)
{
    CritSec cs;
    return *variable;
}*/
/*
unsigned char CritVarRead(unsigned char *variable)
{
    CritSec cs;
    return *variable;
}
*/

