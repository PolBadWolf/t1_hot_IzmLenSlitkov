
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
  extern   signed int rs_Dat[];
  extern unsigned int rs_slitok_len;
  extern unsigned int rs_slitok_speed;
}

#endif

