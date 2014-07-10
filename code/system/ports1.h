


#ifndef ports1__h
#define ports1__h

void dd_init();

template<typename TIn>
class tc_ports1
{
public:
  tc_ports1(unsigned char PortName, unsigned char PortBit, TIn Nintegr);
  tc_ports1(unsigned char PortName, unsigned char PortBit, TIn Nintegr, void (*pEvent)(unsigned char level, unsigned char tag), unsigned char tag);
  void for_timer();
  unsigned char level();
  bool inv;
  unsigned char Tag;
private:
  unsigned char Nintg;
  void (*Event)(unsigned char level, unsigned char tag);
  TIn Porog;
  unsigned char volatile __tiny *p_ddr;
  unsigned char volatile __tiny *p_in;
  unsigned char volatile __tiny *p_out;
  unsigned char p_mask;
  unsigned char tr;
  TIn Count;
};


#endif

