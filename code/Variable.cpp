
#include "main.h"
#include "variable.h"

namespace vg
{
  void init()
  {
  }
  // =====================================
//  __eeprom TDateSmena ee_Archiv[ee_ArchivLen] = {0};
  __eeprom unsigned int rs_Dat[6] = {0, 1000, 2000, 3000, 4000, 5000};
  unsigned int rs_slitok_len = 2500;
  unsigned int rs_slitok_speed = 100;
  __eeprom unsigned char kf_usr = 20;
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

