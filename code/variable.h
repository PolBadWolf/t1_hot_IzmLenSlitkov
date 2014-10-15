
#ifndef variable__h
#define variable__h
//#define ee_ArchivLen 93
namespace vg
{
  void init();
  /*
  struct TDateSmena {
    unsigned char Year;
    unsigned char Mounth;
    unsigned char Date;
    unsigned char Smena;
    unsigned long Impuls;
  };
  */
  //extern __eeprom TDateSmena ee_Archiv[];
  __eeprom extern unsigned int rs_Dat[8];
  __eeprom extern unsigned int prcPorog;
  __eeprom extern unsigned char crutchesTail;
  extern unsigned int rs_slitok_len;
  extern unsigned int rs_slitok_speed;
  extern __eeprom unsigned char kf_usr;
  extern char teleMsg[10];
  extern unsigned int upTimeMS;
  extern unsigned char upTimeS, upTimeM, upTimeH, upTimeD, upTimeSN;
}

//inline void CritVarWrite(T *variable, T *newDat);

//unsigned char CritVarRead(unsigned char *variable);
/*
template <class T>
T CritVarRead(T *variable);
unsigned char CritVarRead(unsigned char *variable);
*/

#endif

