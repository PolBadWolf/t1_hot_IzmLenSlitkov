
#ifndef izm_len__h
#define izm_len__h

namespace ns_izmlen
{
  void init();
  void for_timer();
  void ShowDat();
  void main();
  bool OperatingMode();
  // шаг/режим работы события срабативания датчиков
  extern unsigned char step;
  extern bool SensorNewDate;
  extern bool flNewLen;
  extern unsigned char nWorkDat;
  extern unsigned char ErrWorkDat;
  extern unsigned int  NewLen;
}


#endif


