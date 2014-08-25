
#ifndef izm_len__h
#define izm_len__h

#define izmLen_TimeOut 80000
#define izmLen_DatPorog 60

#if   (izmLen_DatPorog<256)
#define izmLen_DatType unsigned char
#elif (izmLen_DatPorog<65536)
#define izmLen_DatType unsigned int
#else
#define izmLen_DatType unsigned long
#endif

namespace ns_izmlen
{
  void init();
  void for_timer();
  void ShowDat();
  void main();
  // ======================
  // шаг/режим работы события срабативания датчиков
  extern unsigned char step;
  unsigned char ReadStep();
  extern bool SensorNewDate;
  extern bool flNewLen;
  extern unsigned char nWorkDat;
  extern unsigned char ErrWorkDat;
  extern unsigned int  NewLen;
  //-----------
    // massive save count tiks from use sensors
#if   (izmLen_TimeOut<256)
    extern unsigned int  timer_mass[2][6];
#elif (izmLen_TimeOut<65536)
    extern unsigned int  timer_mass[2][6];
#else
    extern unsigned long timer_mass[2][6];
#endif
}


#endif


