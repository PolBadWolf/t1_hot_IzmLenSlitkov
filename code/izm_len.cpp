
#include "main.h"
#include "izm_len.h"

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
    CRITVARREAD
    // massive sensors
    tc_ports1<izmLen_DatType> *dat[6];
    // flag block this modul
    unsigned char fi_block = true;
    // flag on work count tiks
    unsigned char fl_timer = 0;
    // count tiks
#if   (izmLen_TimeOut<256)
    unsigned char timer = 1;
#elif (izmLen_TimeOut<65536)
    unsigned int  timer = 1;
#else
    unsigned long timer = 1;
#endif


    // ���/����� ������ ������� ������������ ��������
  unsigned char step = 0;
  bool fl_time_out = false;
  unsigned int  timer2 = 0;
  unsigned char timer2_step = 0;
    // massive save count tiks from use sensors
#if   (izmLen_TimeOut<256)
    unsigned char timer_mass[2][6];
#elif (izmLen_TimeOut<65536)
    unsigned int  timer_mass[2][6];
#else
    unsigned long timer_mass[2][6];
#endif
  unsigned long dochet1;
  unsigned long dochet2;
  unsigned long Base, BaseOb, BaseOb1, BaseOb2, TimeOb, TimeOb1, TimeOb2, Len, otr1, otr2;
  // ������ ���������� ��������
  unsigned char WorkDat[6];
  unsigned char WorkDatInd;
  // ���� ��������� ������
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
    WaitReadInitI//,
//    Empty
  };
  unsigned char EventMassLen;
    inline void SetStep(unsigned char newStep)
    {
        CritSec ns_cs;
        step = newStep;
    }
    inline unsigned char ReadStep()
    {
        unsigned char o;
        {
            CritSec rd_cs;
            o = step;
        }
        return o;
    }
  // ==============================================================================================================
    void init()
    {
        // ��������� � ������ ���� �� ������ ���������
        ad420_Init();
        ad420_SwapWord(0);
        // ��������� ������
        PortReadyDef = 1;
        PortNobDef = 1;
        SetPort(PortReady, 0);
        SetPort(PortNob, 0);
        // create object sensors
    //  dat[0] = new tc_ports1<DatTypeData>(port, bit, Porog, Event, tag);
        dat[0] = new tc_ports1<izmLen_DatType>('f', 0, izmLen_DatPorog, Event, 0);
        dat[1] = new tc_ports1<izmLen_DatType>('f', 1, izmLen_DatPorog, Event, 1);
        dat[2] = new tc_ports1<izmLen_DatType>('f', 2, izmLen_DatPorog, Event, 2);
        dat[3] = new tc_ports1<izmLen_DatType>('f', 3, izmLen_DatPorog, Event, 3);
        dat[4] = new tc_ports1<izmLen_DatType>('f', 4, izmLen_DatPorog, Event, 4);
        dat[5] = new tc_ports1<izmLen_DatType>('f', 5, izmLen_DatPorog, Event, 5);
        // len massive events
        EventMassLen = sizeof(EventMass) / sizeof(EventMass[0]);
        // de block work timer : read sensors & go event
        fi_block = false;
        SetStep(0);
  }
  // ==============================================================================================================
  void Event(unsigned char level, unsigned char tag)
  { // ������� �������� �������
    void (*EventT)(unsigned char level, unsigned char tag);
    unsigned char tStep;
#ifdef DebugSensorError
    scr->F_Char(tag, '0'+level);
#endif
    // �������� �� ����������� ����
    tStep = CritVarRead(&step);
    // tStep = ReadStep();
    if (tStep<EventMassLen)
    {
      SensorNewDate = true;
      //EventMass[step](level, tag);
      {
        CritSec cs;
        EventT = EventMass[step];
      }
      EventT(level, tag);
    }
    else
    {
      // ������ � ���� ��� ������ ���������
      SetStep(0);
    }
#ifdef DebugSensorError
    scr->Hex(8, step);
#endif
  }
  // -----------------------------------------------------------------------
    void for_timer()
    {
        // block work irq count, sensors, wait timer call
        if (fi_block) return;
        // count tiks
        if (fl_timer)
        {
            if (timer<izmLen_TimeOut)
                timer++;
            else
            {
                fl_timer = 0;
                fl_time_out = true;
                SetStep(UkRender);
            }
        }
        // read sensors
        dat[0]->for_timer();
        dat[1]->for_timer();
        dat[2]->for_timer();
        dat[3]->for_timer();
        dat[4]->for_timer();
        dat[5]->for_timer();
        // wait timer call
        if (timer2==1)
        {
            timer2 = 0;
            SetStep(timer2_step);
        }
        if (timer2>1)
            timer2--;
    }
  // ==============================================================================================================
  void Empty(unsigned char level, unsigned char tag)
  {
  }
    // ==============================================================================================================
    void FistInit(unsigned char level, unsigned char tag)
    {   // �������� ������������ ��������
        // this block work from sensors
        if ( (level!=255) || (level!=255) )
            return;
        // disable count tiks
        fl_timer = 0;
        SetStep(UkFistWaitOff);
    }
  // ==============================================================================================================
  void FistWaitOff(unsigned char level, unsigned char tag)
  {
    // �������� ���� ��������
    unsigned char x = 0;
    for (unsigned char i=0;i<6;i++)
    {
      x |= !dat[i]->level();
    }
    if (!x)
    { 
      // �� ������ ���������
      SetStep(UkWaitReadInit);
    }
  }
  // ==============================================================================================================
  void WaitReadInit(unsigned char level, unsigned char tag)
  {
    // ����� ������
    SetPort(PortReady, 0);
    SetPort(PortNob, 0);
    // ����� ����� ������ 
    fl_err = 0;
    // ����� ������� �����
    {
        CritSec ms_cs;
        for (unsigned char ud=0; ud<2; ud++)
        {
          for (unsigned char nd=0; nd<6; nd++)
          {
              timer_mass[ud][nd] = 0;
          }
        }
        fl_timer = 0;
    }
    // �� �������� ������������ ��������
    SetStep(UkWaitReadBegin);
    WaitReadBegin(level, tag);
  }
  // ==============================================================================================================
  void WaitReadBegin(unsigned char level, unsigned char tag)
  { // ��������� �������� ��� ������
    if ( level==0 )
    {
      // �������� ���� �����
      timer = 1;
      fl_timer = 1;
      fl_time_out = false;
      // ��������� ���
      timer_mass[level][tag] = timer;
      // ���������� ���������� ������
      step = UkWaitReadEnd;
      if ( tag==0 )
      {
#ifdef DebugSensorError
        scr->F_String(c_stolbcov+0, "             ");
#endif
        return;
      }
      else
      { // �� �������� ������ ������
        fl_err |= 1<<fl_err_d0;
      }
    }
    else
    { // ��������� ����� � ������� - ������������� ����������
      // **************************
    }
#ifdef DebugSensorError
    DebugSensorError
#endif
    // ������
  }
  // ==============================================================================================================
  void WaitReadEnd(unsigned char level, unsigned char tag)
  {
    { // ������� ������������ ?
      CritSec cs;
      timer2 = 0;
    }
      // �������� �� ��������� ������������
      if ( timer_mass[level][tag]>0 )
      {
        // ��������� ������������
        fl_err |= 1<<tag;
#ifdef DebugSensorError
    DebugSensorError
#endif
      }
      else
      {
        // ��������� ���
        timer_mass[level][tag] = timer;
      }
      // �������� �� ���������
    unsigned char x = 1;
    for (unsigned char i=0;i<6;i++)
    {
      x &= dat[i]->level();
    }
    if (x)
    { // ������� ������������ ?
      CritSec cs;
      // �������� �������� 5 ������ ����� ��������� �������
      timer2_step = UkRender;
      timer2 = 5000;
    }
  }
  // ==============================================================================================================
    void main()
    {
        // curen step
        unsigned char temp;
        temp = ReadStep();
       // temp = CritVarRead(&step);
    // this init work
    if ( temp==UkFistInit )
    {
        EventMass[UkFistInit](255,255);
        return;
    }
    // step wait for off sensors
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
    // ��������� ���� �����
    fl_timer = 0;
    if ( fl_time_out )
    { // ���� ��� ��� ���������
      // ����� ������������� � �������� ��������
      for (unsigned char n=0; n<6; n++)
      {
        for (unsigned char l=0; l<2; l++)
        {
          /* // �����, ��� ��� �������� �������� �� ����� ���������� �� ������� ��������
          if ( timer_mass[l][n]==0 )
          {
             fl_err |= 1<<n;
          }
          */
        }
        if ( !dat[n]->level() )
        { // �������� ������
          fl_err |= 1<<n;
        }
      }
      ErrWorkDat = fl_err;
      NewLen = 0;
      // ����� ������
      ad420_SwapWord(0);
      SetPort(PortNob, 1);
      _delay_ms(500);
      SetPort(PortReady, 1);
      flNewLen = true;
      SetStep(UkBlock);
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
      // ����� ������� ���������� ��������
      for (unsigned char i=0; i<6; i++)
        WorkDat[i] = 255;
      WorkDatInd = 0;
      // �������� � ���������� ������������� ��������
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
        // ���� ������, ��
        //scr->F_String(c_stolbcov+0, "���� ��������");
        ErrWorkDat = fl_err;
        NewLen = 0;
        // ����� ������
        ad420_SwapWord(0);
        SetPort(PortNob, 1);
        _delay_ms(500);
        SetPort(PortReady, 1);
        flNewLen = true;
        SetStep(UkBlock);
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
        // ��� ��� ���� ����� ������� � ����������
        if ( timer_mass[0][WorkDat[WorkDatInd-1]]<timer_mass[1][WorkDat[0]] )
        { // ��������� ������ ������� ��������
          dochet1 = timer_mass[1][WorkDat[0]] - timer_mass[0][WorkDat[WorkDatInd-1]];
          Base = vg::rs_Dat[WorkDat[WorkDatInd-1]] - vg::rs_Dat[WorkDat[0]];
//          BaseOb = vg::rs_Dat[WorkDat[WorkDatInd-1]] - vg::rs_Dat[WorkDat[WorkDatInd-2]];
//          TimeOb = timer_mass[0][WorkDat[WorkDatInd-1]] - timer_mass[0][WorkDat[WorkDatInd-2]];
          // � ������
          BaseOb1 = vg::rs_Dat[WorkDat[WorkDatInd-1]] - vg::rs_Dat[WorkDat[WorkDatInd-2]];
          TimeOb1 = timer_mass[0][WorkDat[WorkDatInd-1]] - timer_mass[0][WorkDat[WorkDatInd-2]];
          // � �����
          BaseOb2 = vg::rs_Dat[WorkDat[1]] - vg::rs_Dat[WorkDat[0]];
          TimeOb2 = timer_mass[1][WorkDat[1]] - timer_mass[1][WorkDat[0]];
          // all
          BaseOb = BaseOb1 + BaseOb2;
          TimeOb = TimeOb1 + TimeOb2;
          otr1 = (dochet1*BaseOb1*10/TimeOb1+5)/10;
          otr2 = (dochet1*BaseOb2*10/TimeOb2+5)/10;
          Len = Base + ((otr1+otr2)*10)/vg::kf_usr;
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
      //send to
      SendOutDebug(NewLen, ErrWorkDat, ns_izmlen::timer_mass );
    }
  }
  // ==============================================================================================================
  // ==============================================================================================================
  void FistWaitOffI(unsigned char level, unsigned char tag)
  {
    FistWaitOff(level, tag);
  }
  void FistWaitOffI1()
  {
    SetPort(PortReady, 0);
    SetStep(UkFistWaitOff);
  }
  // ==============================================================================================================
  void WaitReadInitI(unsigned char level, unsigned char tag)
  {
    WaitReadInit(level, tag);
  }
  void WaitReadInitI1()
  {
    SetPort(PortReady, 0);
    SetStep(UkWaitReadInit);
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


