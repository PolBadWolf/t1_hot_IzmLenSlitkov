
#include "../main.h"
#include "ports1.h"

// =====================================================================================================================================================================================
tc_ports1<unsigned char>::tc_ports1(unsigned char PortName, unsigned char PortBit, unsigned char Nintegr)
{
  Tag = 0;
  Event = 0;
  Porog = Nintegr;
  inv = false;
  switch (PortName)
  {
    case 'a' : p_in = &PINA; p_ddr = &DDRA; p_out = &PORTA; break;
    case 'A' : p_in = &PINA; p_ddr = &DDRA; p_out = &PORTA; break;
    case 'b' : p_in = &PINB; p_ddr = &DDRB; p_out = &PORTB; break;
    case 'B' : p_in = &PINB; p_ddr = &DDRB; p_out = &PORTB; break;
    case 'c' : p_in = &PINC; p_ddr = &DDRC; p_out = &PORTC; break;
    case 'C' : p_in = &PINC; p_ddr = &DDRC; p_out = &PORTC; break;
    case 'd' : p_in = &PIND; p_ddr = &DDRD; p_out = &PORTD; break;
    case 'D' : p_in = &PIND; p_ddr = &DDRD; p_out = &PORTD; break;
    case 'e' : p_in = &PINE; p_ddr = &DDRE; p_out = &PORTE; break;
    case 'E' : p_in = &PINE; p_ddr = &DDRE; p_out = &PORTE; break;
    case 'f' : p_in = &PINF; p_ddr = &DDRF; p_out = &PORTF; break;
    case 'F' : p_in = &PINF; p_ddr = &DDRF; p_out = &PORTF; break;
    case 'g' : p_in = &PING; p_ddr = &DDRG; p_out = &PORTG; break;
    case 'G' : p_in = &PING; p_ddr = &DDRG; p_out = &PORTG; break;
    default  : p_in = &PING; p_ddr = &DDRG; p_out = &PORTG; PortBit = 4;
  }
  if (PortBit>=8) PortBit = 0;
  p_mask = 1<<PortBit;
  *p_ddr = *p_ddr & (~p_mask);
  *p_out = p_mask;
  tr = (bool)(*p_in & p_mask) ^ inv;
  if (tr) Count = Porog;
  else    Count = 0;
  // init status
  for_timer();
}
tc_ports1<unsigned int >::tc_ports1(unsigned char PortName, unsigned char PortBit, unsigned int  Nintegr)
{
  Tag = 0;
  Event = 0;
  Porog = Nintegr;
  inv = false;
  switch (PortName)
  {
    case 'a' : p_in = &PINA; p_ddr = &DDRA; p_out = &PORTA; break;
    case 'A' : p_in = &PINA; p_ddr = &DDRA; p_out = &PORTA; break;
    case 'b' : p_in = &PINB; p_ddr = &DDRB; p_out = &PORTB; break;
    case 'B' : p_in = &PINB; p_ddr = &DDRB; p_out = &PORTB; break;
    case 'c' : p_in = &PINC; p_ddr = &DDRC; p_out = &PORTC; break;
    case 'C' : p_in = &PINC; p_ddr = &DDRC; p_out = &PORTC; break;
    case 'd' : p_in = &PIND; p_ddr = &DDRD; p_out = &PORTD; break;
    case 'D' : p_in = &PIND; p_ddr = &DDRD; p_out = &PORTD; break;
    case 'e' : p_in = &PINE; p_ddr = &DDRE; p_out = &PORTE; break;
    case 'E' : p_in = &PINE; p_ddr = &DDRE; p_out = &PORTE; break;
    case 'f' : p_in = &PINF; p_ddr = &DDRF; p_out = &PORTF; break;
    case 'F' : p_in = &PINF; p_ddr = &DDRF; p_out = &PORTF; break;
    case 'g' : p_in = &PING; p_ddr = &DDRG; p_out = &PORTG; break;
    case 'G' : p_in = &PING; p_ddr = &DDRG; p_out = &PORTG; break;
    default  : p_in = &PING; p_ddr = &DDRG; p_out = &PORTG; PortBit = 4;
  }
  if (PortBit>=8) PortBit = 0;
  p_mask = 1<<PortBit;
  *p_ddr = *p_ddr & (~p_mask);
  *p_out = p_mask;
  tr = (bool)(*p_in & p_mask) ^ inv;
  if (tr) Count = Porog;
  else    Count = 0;
  // init status
  for_timer();
}
// =====================================================================================================================================================================================
tc_ports1<unsigned char>::tc_ports1(unsigned char PortName, unsigned char PortBit, unsigned char Nintegr, void (*pEvent)(unsigned char level, unsigned char tag), unsigned char tag)
{
  Tag = tag;
  Event = *pEvent;
  Porog = Nintegr;
  inv = false;
  switch (PortName)
  {
    case 'a' : p_in = &PINA; p_ddr = &DDRA; p_out = &PORTA; break;
    case 'A' : p_in = &PINA; p_ddr = &DDRA; p_out = &PORTA; break;
    case 'b' : p_in = &PINB; p_ddr = &DDRB; p_out = &PORTB; break;
    case 'B' : p_in = &PINB; p_ddr = &DDRB; p_out = &PORTB; break;
    case 'c' : p_in = &PINC; p_ddr = &DDRC; p_out = &PORTC; break;
    case 'C' : p_in = &PINC; p_ddr = &DDRC; p_out = &PORTC; break;
    case 'd' : p_in = &PIND; p_ddr = &DDRD; p_out = &PORTD; break;
    case 'D' : p_in = &PIND; p_ddr = &DDRD; p_out = &PORTD; break;
    case 'e' : p_in = &PINE; p_ddr = &DDRE; p_out = &PORTE; break;
    case 'E' : p_in = &PINE; p_ddr = &DDRE; p_out = &PORTE; break;
    case 'f' : p_in = &PINF; p_ddr = &DDRF; p_out = &PORTF; break;
    case 'F' : p_in = &PINF; p_ddr = &DDRF; p_out = &PORTF; break;
    case 'g' : p_in = &PING; p_ddr = &DDRG; p_out = &PORTG; break;
    case 'G' : p_in = &PING; p_ddr = &DDRG; p_out = &PORTG; break;
    default  : p_in = &PING; p_ddr = &DDRG; p_out = &PORTG; PortBit = 4;
  }
  if (PortBit>=8) PortBit = 0;
  p_mask = 1<<PortBit;
  *p_ddr = *p_ddr & (~p_mask);
  *p_out = p_mask;
  tr = (bool)(*p_in & p_mask) ^ inv;
  if (tr) Count = Porog;
  else    Count = 0;
  // init status
  for_timer();
}
tc_ports1<unsigned int >::tc_ports1(unsigned char PortName, unsigned char PortBit, unsigned int  Nintegr, void (*pEvent)(unsigned char level, unsigned char tag), unsigned char tag)
{
  Tag = tag;
  Event = *pEvent;
  Porog = Nintegr;
  inv = false;
  switch (PortName)
  {
    case 'a' : p_in = &PINA; p_ddr = &DDRA; p_out = &PORTA; break;
    case 'A' : p_in = &PINA; p_ddr = &DDRA; p_out = &PORTA; break;
    case 'b' : p_in = &PINB; p_ddr = &DDRB; p_out = &PORTB; break;
    case 'B' : p_in = &PINB; p_ddr = &DDRB; p_out = &PORTB; break;
    case 'c' : p_in = &PINC; p_ddr = &DDRC; p_out = &PORTC; break;
    case 'C' : p_in = &PINC; p_ddr = &DDRC; p_out = &PORTC; break;
    case 'd' : p_in = &PIND; p_ddr = &DDRD; p_out = &PORTD; break;
    case 'D' : p_in = &PIND; p_ddr = &DDRD; p_out = &PORTD; break;
    case 'e' : p_in = &PINE; p_ddr = &DDRE; p_out = &PORTE; break;
    case 'E' : p_in = &PINE; p_ddr = &DDRE; p_out = &PORTE; break;
    case 'f' : p_in = &PINF; p_ddr = &DDRF; p_out = &PORTF; break;
    case 'F' : p_in = &PINF; p_ddr = &DDRF; p_out = &PORTF; break;
    case 'g' : p_in = &PING; p_ddr = &DDRG; p_out = &PORTG; break;
    case 'G' : p_in = &PING; p_ddr = &DDRG; p_out = &PORTG; break;
    default  : p_in = &PING; p_ddr = &DDRG; p_out = &PORTG; PortBit = 4;
  }
  if (PortBit>=8) PortBit = 0;
  p_mask = 1<<PortBit;
  *p_ddr = *p_ddr & (~p_mask);
  *p_out = p_mask;
  tr = (bool)(*p_in & p_mask) ^ inv;
  if (tr) Count = Porog;
  else    Count = 0;
  // init status
  for_timer();
}
// =====================================================================================================================================================================================
void tc_ports1<unsigned char>::for_timer()
{
  if ( (*p_in & p_mask) ^ inv )
  {
    if ( Count<Porog ) Count++;
    if ( (Count>=Porog) && (!tr) )
    {
      tr = 1;
      // event
      if (Event!=0)
      {
         Event(tr, Tag);
      }
    }
  }
  else
  {
    if ( Count>0 ) Count--;
    if ( (Count==0) && (tr) )
    {
      tr = 0;
      // event
      if (Event!=0)
      {
         Event(tr, Tag);
      }
    }
  }
}
void tc_ports1<unsigned int >::for_timer()
{
  if ( (*p_in & p_mask) ^ inv )
  {
    if ( Count<Porog ) Count++;
    if ( (Count>=Porog) && (!tr) )
    {
      tr = 1;
      // event
      if (Event!=0)
      {
         Event(tr, Tag);
      }
    }
  }
  else
  {
    if ( Count>0 ) Count--;
    if ( (Count==0) && (tr) )
    {
      tr = 0;
      // event
      if (Event!=0)
      {
         Event(tr, Tag);
      }
    }
  }
}
// =====================================================================================================================================================================================
unsigned char tc_ports1<unsigned char>::level()
{
  return tr;
}
unsigned char tc_ports1<unsigned int >::level()
{
  return tr;
}
// =====================================================================================================================================================================================
