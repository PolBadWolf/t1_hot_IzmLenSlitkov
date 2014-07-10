
#include "main.h"

#include "ReadDat.h"

namespace ns_ReadDat
{
#define BuffForSelectLen 8
  unsigned char BuffForSelect[BuffForSelectLen];
  unsigned char byte;
  unsigned int temp;
  
  void cycl()
  {
    if ( !PortPC::ReadByte(&byte) )
      return;
    for (unsigned char i=0;i<BuffForSelectLen-1;i++)
      BuffForSelect[i] = BuffForSelect[i+1];
    BuffForSelect[BuffForSelectLen-1] = byte;
    /*
    for (int x=0;x<8;x++)
    {
      scr->Hex(16+(x*2), BuffForSelect[x]);
    }
    */
    if ( BuffForSelect[0]!=0xe6 ) return;
    if ( BuffForSelect[1]!=0x19 ) return;
    if ( BuffForSelect[2]!=0x00 ) return;
    if ( BuffForSelect[3]>=10   ) return;
    if ( BuffForSelect[6]!=crc8_buf(BuffForSelect,6) ) return;
    temp = 0;
    ((unsigned char *)& temp)[0] = BuffForSelect[4];
    ((unsigned char *)& temp)[1] = BuffForSelect[5];
    if ( BuffForSelect[3]==8 )
    {
      scr->dig_uz(12,4, temp );
    }
    if ( BuffForSelect[3]<8 )
    {
      vg::rs_Dat[BuffForSelect[3]] = temp;
      return;
    }
    if ( BuffForSelect[3]==8 )
    {
      vg::rs_slitok_len = temp;
      return;
    }
    if ( BuffForSelect[3]==9 )
    {
      vg::rs_slitok_speed = temp;
      return;
    }
  }
}

