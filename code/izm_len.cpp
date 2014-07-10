
#include "main.h"
#include "izm_len.h"

#define DatPorog 50
#define DatTypeData unsigned char
#define TimeOut 25000

#define SetPort(Port, Level)  Port = !Level
#define PortReadyDef  DDRE_DDE5
#define PortReady     PORTE_PORTE5
#define PortNobDef    DDRE_DDE4
#define PortNob       PORTE_PORTE4

//#define DebugSensorError \
  scr->F_String(c_stolbcov+0, "Error sensor"); \
  scr->ShowChar(c_stolbcov+12, '0'+tag);

namespace ns_izmlen
{
  // флаг блокировки работы модуля
  unsigned char fi_block = true;
  // рабочий режим
  bool VarOperatingMode = false;
  // массив датчиков
  tc_ports1<DatTypeData> *dat[6];
  // шаг/режим работы события срабативания датчиков
  unsigned char step = 0;
  // флаг разрешения работы счета тиков
  unsigned char fl_timer = 0;
  // счетчик тиков
#if (TimeOut<65536)
  unsigned int  timer = 1;
#else
  unsigned long timer = 1;
#endif
  bool fl_time_out = false;
  unsigned int  timer2 = 0;
  unsigned char timer2_step = 0;
  // массив регистрации тиков
#if (TimeOut<65536)
  unsigned int  timer_mass[2][6];
#else
  unsigned long timer_mass[2][6];
#endif
  unsigned long dochet1;
  unsigned long dochet2;
  unsigned long Base, BaseOb, TimeOb, Len;
  // массив работающих датчиков
  unsigned char WorkDat[6];
  unsigned char WorkDatInd;
  // флаг указатель ошибки
  unsigned int fl_err = 0;
#define fl_err_d0 0
  // ==============================================================================================================
  bool SensorNewDate = false;
  bool flNewLen = false;
  unsigned char nWorkDat = 0;
  unsigned char ErrWorkDat = 0;
  unsigned int  NewLen = 0;
  // ==============================================================================================================
  void Event(unsigned char level, unsigned char tag);
  void Empty(unsigned char level, unsigned char tag);
  void FistInit(unsigned char level, unsigned char tag);
  void FistWaitOff(unsigned char level, unsigned char tag);
  void WaitReadInit(unsigned char level, unsigned char tag);
  void WaitReadBegin(unsigned char level, unsigned char tag);
  void WaitReadEnd(unsigned char level, unsigned char tag);
  void FistWaitOffI(unsigned char level, unsigned char tag);
  void FistWaitOffI1();
  void WaitReadInitI(unsigned char level, unsigned char tag);
  void WaitReadInitI1();
  // ==============================================================================================================
  void (* const __flash EventMass[])(unsigned char level, unsigned char tag) = 
  {
#define UkFistInit 0
    FistInit,
#define UkFistWaitOff     1
    FistWaitOff,
#define UkWaitReadInit    2
    WaitReadInit,
#define UkWaitReadBegin   3
    WaitReadBegin,
#define UkWaitReadEnd     4
    WaitReadEnd,
#define UkRender          5
    Empty,
#define UkBlock           6
    Empty,
#define UkFistWaitOffI    7
    FistWaitOffI,
#define UkWaitReadInitI   8
    WaitReadInitI,
    Empty
  };
  unsigned char EventMassLen;
  // ==============================================================================================================
  void init()
  {
    ad420_Init();
    ad420_SwapWord(0);
    PortReadyDef = 1;
    PortNobDef = 1;
    SetPort(PortReady, 0);
    SetPort(PortNob, 0);
//  dat[0] = new tc_ports1<DatTypeData>(port, bit, Porog, Event, tag);
    dat[0] = new tc_ports1<DatTypeData>('f', 0, DatPorog, Event, 0);
    dat[1] = new tc_ports1<DatTypeData>('f', 1, DatPorog, Event, 1);
    dat[2] = new tc_ports1<DatTypeData>('f', 2, DatPorog, Event, 2);
    dat[3] = new tc_ports1<DatTypeData>('f', 3, DatPorog, Event, 3);
    dat[4] = new tc_ports1<DatTypeData>('f', 4, DatPorog, Event, 4);
    dat[5] = new tc_ports1<DatTypeData>('f', 5, DatPorog, Event, 5);
    EventMassLen = sizeof(EventMass) / sizeof(EventMass[0]);
    fi_block = false;
    for (unsigned char d=0; d<6; d++)
    {
      Event( dat[d]->level(), d);
    }
  }
  // ==============================================================================================================
  void Event(unsigned char level, unsigned char tag)
  { // событие сработки датчика
#ifdef DebugSensorError
    scr->F_Char(tag, '0'+level);
#endif
    // проверка на коррекность кода
    if (step<EventMassLen)
    {
      SensorNewDate = true;
      EventMass[step](level, tag);
    }
    else
    {
      // ошибка в коде или логике программы
      step = 0;
    }
#ifdef DebugSensorError
    scr->Hex(8, step);
#endif
  }
  // -----------------------------------------------------------------------
  void for_timer()
  {
    if (fi_block) return;
    if (fl_timer)
    {
      if (timer<TimeOut)
        timer++;
      else
      {
        fl_timer = 0;
        fl_time_out = true;
        step = UkRender;
      }
    }
    dat[0]->for_timer();
    dat[1]->for_timer();
    dat[2]->for_timer();
    dat[3]->for_timer();
    dat[4]->for_timer();
    dat[5]->for_timer();
    if (timer2>1)
      timer2--;
    if (timer2==1)
    {
      timer2 = 0;
      step = timer2_step;
    }
  }
  // ==============================================================================================================
  void Empty(unsigned char level, unsigned char tag)
  {
  }
  // ==============================================================================================================
  void FistInit(unsigned char level, unsigned char tag)
  { // ожидание освобождения датчиков
    // выключить счет тиков
    fl_timer = 0;
    VarOperatingMode = false;
    step = UkFistWaitOff;
  }
  // ==============================================================================================================
  void FistWaitOff(unsigned char level, unsigned char tag)
  {
    // проверка всех датчиков
    unsigned char x = 0;
    for (unsigned char i=0;i<6;i++)
    {
      x |= !dat[i]->level();
    }
    if (!x)
    { 
      VarOperatingMode = true;
      // на начало измерений
      step = UkWaitReadInit;
    }
  }
  // ==============================================================================================================
  void WaitReadInit(unsigned char level, unsigned char tag)
  {
    // сброс выхода
    SetPort(PortReady, 0);
    SetPort(PortNob, 0);
    // сброс флага ошибки 
    fl_err = 0;
    // сброс массива тиков
    for (unsigned char ud=0; ud<2; ud++)
    {
      for (unsigned char nd=0; nd<6; nd++)
      {
          timer_mass[ud][nd] = 0;
      }
    }
    fl_timer = 0;
    // на ожидание срабатывания датчиков
    step = UkWaitReadBegin;
    WaitReadBegin(level, tag);
  }
  // ==============================================================================================================
  void WaitReadBegin(unsigned char level, unsigned char tag)
  { // первичная сработка при замере
    if ( level==0 )
    {
      // включить счет тиков
      timer = 1;
      fl_timer = 1;
      fl_time_out = false;
      // сохранить тик
      timer_mass[level][tag] = timer;
      // продолжаем сохранение данных
      step = UkWaitReadEnd;
      if ( tag==0 )
      {
#ifdef DebugSensorError
        scr->F_String(c_stolbcov+0, "             ");
#endif
        return;
      }
      else
      { // не работает превый датчик
        fl_err |= 1<<fl_err_d0;
      }
    }
    else
    { // произошел съезд с датчика - принципиально невозможно
      // **************************
    }
#ifdef DebugSensorError
    DebugSensorError
#endif
    // ошибка
  }
  // ==============================================================================================================
  void WaitReadEnd(unsigned char level, unsigned char tag)
  {
    { // датчики освободились ?
      CritSec cs;
      timer2 = 0;
    }
      // проверка на повторное срабатывание
      if ( timer_mass[level][tag]>0 )
      {
        // повторное срабатывание
        fl_err |= 1<<tag;
#ifdef DebugSensorError
    DebugSensorError
#endif
      }
      else
      {
        // сохранить тик
        timer_mass[level][tag] = timer;
      }
      // проверка на окончание
    unsigned char x = 1;
    for (unsigned char i=0;i<6;i++)
    {
      x &= dat[i]->level();
    }
    if (x)
    { // датчики освободились ?
      CritSec cs;
      // защитный интервал 5 секунд перед принятием решения
      timer2_step = UkRender;
      timer2 = 5000;
    }
  }
  // ==============================================================================================================
  void main()
  {
    unsigned char temp;
    {
      CritSec cs;
      temp = step;
    }
    if ( temp==UkFistWaitOffI )
    {
      FistWaitOffI1();
      return;
    }
    if ( temp==UkWaitReadInitI )
    {
      WaitReadInitI1();
      return;
    }
    if ( temp!=UkRender )
      return;
    // выключить счет тиков
    fl_timer = 0;
    if ( fl_time_out )
    { // тайм аут при измерении
      // поиск несработавших и залипших датчиков
      for (unsigned char n=0; n<6; n++)
      {
        for (unsigned char l=0; l<2; l++)
        {
          /* // решил, что при залипших датчиках не нужно определять по которым проехали
          if ( timer_mass[l][n]==0 )
          {
             fl_err |= 1<<n;
          }
          */
        }
        if ( !dat[n]->level() )
        { // залипший датчик
          fl_err |= 1<<n;
        }
      }
      ErrWorkDat = fl_err;
      NewLen = 0;
      // сброс выхода
      ad420_SwapWord(0);
      SetPort(PortNob, 1);
      _delay_ms(500);
      SetPort(PortReady, 1);
      flNewLen = true;
      step = UkBlock;
      {
        CritSec cs;
        timer2_step = UkFistWaitOffI;
        timer2 = 5000;
      };
      fl_time_out = false;
      return;
    }
    else
    {
      // сброс массива работающих датчиков
      for (unsigned char i=0; i<6; i++)
        WorkDat[i] = 255;
      WorkDatInd = 0;
      // проверка и маркировка несработавших датчиков
      for (unsigned char n=0; n<6; n++)
      {
        for (unsigned char l=0; l<2; l++)
        {
          if ( timer_mass[l][n]==0 )
          {
            fl_err |= 1<<n;
          }
        }
        if ( (fl_err & (1<<n))==0 )
        {
          WorkDat[WorkDatInd] = n;
          WorkDatInd++;
        }
      }
      nWorkDat = WorkDatInd;
      ErrWorkDat = fl_err;
      unsigned long Len1 = 0;
      unsigned char OpMin = 0;
      if ( WorkDatInd<3 )
      {
        // если меньше, то
        //scr->F_String(c_stolbcov+0, "мало датчиков");
        ErrWorkDat = fl_err;
        NewLen = 0;
        // сброс выхода
        ad420_SwapWord(0);
        SetPort(PortNob, 1);
        _delay_ms(500);
        SetPort(PortReady, 1);
        flNewLen = true;
        step = UkBlock;
        {
          CritSec cs;
          timer2_step = UkWaitReadInitI;
          // timer2_step = UkFistWaitOff;
          timer2 = 5000;
        };
        fl_time_out = false;
        return;
      }
      else
      {
        // вот тут надо будет парсить с алгоритмом
        if ( timer_mass[0][WorkDat[WorkDatInd-1]]<timer_mass[1][WorkDat[0]] )
        { // заготовка длинее массива датчиков
          dochet1 = timer_mass[1][WorkDat[0]] - timer_mass[0][WorkDat[WorkDatInd-1]];
          Base = vg::rs_Dat[WorkDat[WorkDatInd-1]] - vg::rs_Dat[WorkDat[0]];
          BaseOb = vg::rs_Dat[WorkDat[WorkDatInd-1]] - vg::rs_Dat[WorkDat[WorkDatInd-2]];
          TimeOb = timer_mass[0][WorkDat[WorkDatInd-1]] - timer_mass[0][WorkDat[WorkDatInd-2]];
          Len = Base + (dochet1*BaseOb*10/TimeOb+5)/10;
          Len1 = Len1 + Len;
          OpMin++;
        }
        else
        {
          for (unsigned char i=0; i<WorkDatInd; i++)
          {
            if ( timer_mass[0][WorkDat[i]]<timer_mass[1][WorkDat[OpMin]] )
              continue;
            dochet1 = timer_mass[1][WorkDat[OpMin]] - timer_mass[0][WorkDat[i-1]];    // 1
            dochet2 = timer_mass[0][WorkDat[i]]     - timer_mass[1][WorkDat[OpMin]];  // 2
            if ( dochet1<dochet2 )
            {
              Base = vg::rs_Dat[WorkDat[i-1]] - vg::rs_Dat[WorkDat[OpMin]];                       // 1
              BaseOb = vg::rs_Dat[WorkDat[i]] - vg::rs_Dat[WorkDat[i-1]];
              TimeOb = timer_mass[0][WorkDat[i]] - timer_mass[0][WorkDat[i-1]];
              Len = (Base*10 + (dochet1*BaseOb*1005)/(TimeOb*100) +5 )/10;                                   // 1
              Len1 = Len1 + Len;
            }
            else
            {
              Base = vg::rs_Dat[WorkDat[i-0]] - vg::rs_Dat[WorkDat[OpMin]];                        // 2
              BaseOb = vg::rs_Dat[WorkDat[i]] - vg::rs_Dat[WorkDat[i-1]];
              TimeOb = timer_mass[0][WorkDat[i]] - timer_mass[0][WorkDat[i-1]];
              Len = (Base*10 - (dochet2*BaseOb*998)/(TimeOb*100) +18 )/10;                                    // 2
              Len1 = Len1 + Len;
            }
          OpMin++;
          }
        }
      }
      NewLen = (Len1*10/OpMin+5)/10;
      ad420_SwapWord(NewLen*4);
      if (WorkDatInd<6)
        SetPort(PortNob, 1 );
      else
        SetPort(PortNob, 0 );
      _delay_ms(500);
      SetPort(PortReady, 1);
      flNewLen = true;
      step = UkBlock;
      {
        CritSec cs;
        timer2_step = UkWaitReadInitI;
        timer2 = 5000;
      };
    }
  }
  // ==============================================================================================================
  bool OperatingMode()
  {
    return VarOperatingMode;
  }
  // ==============================================================================================================
  void FistWaitOffI(unsigned char level, unsigned char tag)
  {
    FistWaitOff(level, tag);
  }
  void FistWaitOffI1()
  {
    SetPort(PortReady, 0);
    step = UkFistWaitOff;
  }
  // ==============================================================================================================
  void WaitReadInitI(unsigned char level, unsigned char tag)
  {
    WaitReadInit(level, tag);
  }
  void WaitReadInitI1()
  {
    SetPort(PortReady, 0);
    step = UkWaitReadInit;
      ad420_SwapWord(0);
  }
  // ==============================================================================================================
  // ==============================================================================================================
  // ==============================================================================================================
  // ==============================================================================================================
  // ==============================================================================================================
  // ==============================================================================================================
  // ==============================================================================================================
  // ==============================================================================================================
  // ==============================================================================================================
  // ==============================================================================================================
}


