
#ifndef rs232__h
#define rs232__h
namespace ns_rs232
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

