
#ifndef izm_len__h
#define izm_len__h

#define izmLenTimeOut 25000
namespace ns_izmlen
{
  void init();
  void for_timer();
  void ShowDat();
  void main();
  bool OperatingMode();
  // ���/����� ������ ������� ������������ ��������
  extern unsigned char step;
  extern bool SensorNewDate;
  extern bool flNewLen;
  extern unsigned char nWorkDat;
  extern unsigned char ErrWorkDat;
  extern unsigned int  NewLen;
  //-----------
    // ������ ����������� �����
#if (izmLenTimeOut<65536)
    extern unsigned int  timer_mass[2][6];
#else
    extern unsigned long timer_mass[2][6];
#endif
}


#endif


