
#ifndef rs485__h
#define rs485__h
#include "Configure.h"
#ifdef RS485
namespace ns_rs485
{
  void init(void);
  unsigned char WriteByte(unsigned char data);
  unsigned char ReadByte(unsigned char *Byte);
  extern unsigned char RxLen;       // заполнение буффера
  extern unsigned char RxOverFlow;  // переполнение при приеме
  extern unsigned char TxLen;       // заполнение буффера
  extern unsigned char TxOverFlow;  // переполнение при передаче
}
#endif
#endif

