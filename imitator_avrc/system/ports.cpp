
#include "../main.h"
#include "ports.h"

tc_ports::tc_ports(unsigned char p, unsigned char InBit) // чтение, порог 100, выходной бит 0
{
  init(p, InBit, 0, 100, 0);
}
tc_ports::tc_ports(unsigned char p, unsigned char InBit, unsigned char OutOn) // порог 100, выходной бит 0
{
  init(p, InBit, OutOn, 100, 0);
}
tc_ports::tc_ports(unsigned char p, unsigned char InBit, unsigned char OutOn, unsigned char porog) // выходной бит 0
{
  init(p, InBit, OutOn, porog, 0);
}
tc_ports::tc_ports(unsigned char p, unsigned char InBit, unsigned char OutOn, unsigned char porog, unsigned char OutBit)
{
  init(p, InBit, OutOn, porog, OutBit);
}

void tc_ports::init(unsigned char p, unsigned char InBit, unsigned char OutOn, unsigned char porog, unsigned char OutBit)
{
  switch (p)
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
    default : p_in = &PINC; p_ddr = &DDRC; p_out = &PORTC; InBit = 3;
  }
  if (InBit<8) maska_in = 1<<InBit;
  else maska_in = 1;
  if (OutOn) *p_ddr |= maska_in;
  else *p_ddr &= ~maska_in;
  *p_out |= maska_in;
  tr = fl = 0;
  count = 0;
  v_porog = porog;
  u_porog = &v_porog;
  if (OutBit<8) maska_out = 1<<OutBit;
  else maska_out = 1;
}

unsigned char tc_ports::ddr(void)
{
  return (*p_ddr & maska_in)?maska_out:0;
}
void tc_ports::ddr(unsigned char d)
{
  if (d) *p_ddr |= maska_in;
  else *p_ddr &= ~maska_in;
}
unsigned char tc_ports::pin(void)
{
  return (*p_in & maska_in)?maska_out:0;
}
unsigned char tc_ports::pout(void)
{
  return (*p_out & maska_in)?maska_out:0;
}
void tc_ports::pout(unsigned char d)
{
  if (d) *p_out |= maska_in;
  else *p_out &= ~maska_in;
}
void tc_ports::for_timer(void)
{
  if (*p_in & maska_in)
  {
    if (count<(*u_porog)) count++;
    if ( (tr==0) && (count>=(*u_porog)) )
    {
      tr = 1;
      fl = maska_out;
      count = *u_porog;
    }
  }
  else
  {
    if (count>0) count--;
    if ( (tr==1) && (count==0) )
    {
      tr = 0;
      fl = 0;
    }
  }
}
