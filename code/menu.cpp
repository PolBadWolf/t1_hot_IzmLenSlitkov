
#include "system/Configure.h"
#ifdef MENU
#include "main.h"
#include "menu.h"

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//            ����� ������� ������ ��� ��������� ������ ������
// #define ShowPass
//              ��������� ����������� ����� �������� ������
// #define ShowStar
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// ������ ��������
namespace ns_izmlen
{
  extern tc_ports1<unsigned char> *dat[6];
}

namespace ns_menu
{
  // ������� ��� ������ ����
  unsigned char step;
  unsigned char step_back;
  // ��� ������� ������
  unsigned char key;
  // ���� ��� � ����
  unsigned long timeout = 0;
  unsigned long timeout_temp;
  unsigned long timeout_max=18000;
  // ================================================================================================================
  void FnVoid() {}
  // ================================================================================================================
  void init();
  // ================================================================================================================
  void WaitWorkMode();
  // ================================================================================================================
  void Default();
  void Default_v();
  // ================================================================================================================
  void InputSetup();
  void InputSetup_s();
  void InputSetup_zm();
  void InputSetup_zp();
  void InputSetup_i();
  void InputSetup_m();
  void ExitSetup();
  #define InputSetup_CurPswLen 5
  unsigned char InputSetup_CurPsw[InputSetup_CurPswLen];
  unsigned char InputSetup_CurPswInd;
  unsigned char __eeprom PaswordE[InputSetup_CurPswLen] = { 1, 2, 3, 4, 5 };
  // ================================================================================================================
  void SetupError();
  void SetupError_v();
  unsigned int SetupError_timer = 0;
  // ================================================================================================================
  unsigned char SetupMenuInd;
  void SetupMenu();
  void SetupMenu_s();
  void SetupMenu_m();
  void SetupMenu_zm();
  void SetupMenu_zp();
  void SetupMenu_i();
  // ================================================================================================================
  void SetPassword();
  void SetPassword_s();
  void SetPassword_m();
  void SetPassword_zm();
  void SetPassword_zp();
  void SetPassword_i();
  // ================================================================================================================
  void SetPassYn();
  void SetPassYn_m();
  void SetPassYn_zm();
  void SetPassYn_zp();
  // ================================================================================================================
  void SetSensor();
  void SetSensor_s();
  void SetSensor_m();
  void SetSensor_zm();
  void SetSensor_zp();
  void SetSensor_i();
  // ================================================================================================================
  void SetSensYn();
  void SetSensYn_m();
  void SetSensYn_zm();
  void SetSensYn_zp();
  // ================================================================================================================
  void SetKoff();  
  void SetKoff_v();
  void SetKoff_m();
  void SetKoff_zm();
  void SetKoff_zp();
  void SetKoff_e();
  // ================================================================================================================
    void SetKoffYn();
    void SetKoffYn_zm();
    void SetKoffYn_zp();
  // ================================================================================================================
  void FnVoid();
  // ================================================================================================================
  void (* const __flash MassMenu[][7])(void) = {
//    0                 1               2                 3                 4                 5               e
#define UkInit            0
    { init              , init          , init            , init            , init            , init            , init            },
#define UkWaitWorkMode    1
    { WaitWorkMode      , FnVoid        , FnVoid          , FnVoid          , FnVoid          , FnVoid          , init            },
#define UkDefault         2
    { Default_v         , InputSetup    , FnVoid          , FnVoid          , FnVoid          , FnVoid          , Default         },
#define UkInputSetup      3
    { FnVoid            , InputSetup_m  , InputSetup_zm   , InputSetup_zp   , InputSetup_i    , ExitSetup       , InputSetup      },
#define UkSetupError      4
    { SetupError_v      , FnVoid        , FnVoid          , FnVoid          , FnVoid          , FnVoid          , SetupError      },
#define UkSetupMenu       5
    { FnVoid            , SetupMenu_m   , SetupMenu_zm    , SetupMenu_zp    , SetupMenu_i     , ExitSetup       , SetupMenu       },
#define UkSetPassword     6
    { FnVoid            , SetPassword_m , SetPassword_zm  , SetPassword_zp  , SetPassword_i   , ExitSetup       , SetPassword     },
#define UkSetPassYn       7
    { FnVoid            , SetPassYn_m   , SetPassYn_zm    , SetPassYn_zp    , FnVoid          , ExitSetup       , SetPassYn       },
#define UkSetSensor       8
    { FnVoid            , SetSensor_m   , SetSensor_zm    , SetSensor_zp    , SetSensor_i     , ExitSetup       , SetSensor       },
#define UkSetSensYn       9
    { FnVoid            , SetSensYn_m   , SetSensYn_zm    , SetSensYn_zp    , FnVoid          , ExitSetup       , SetSensYn       },
#define UkSetKoff         10
    { SetKoff_v         , SetKoff_m     , SetKoff_zm      , SetKoff_zp      , SetKoff_e       , ExitSetup       , SetKoff         },
#define UkSetKoffYn       11
    { FnVoid            , FnVoid        , SetKoffYn_zm    , SetKoffYn_zp    , FnVoid          , ExitSetup       , SetKoffYn       },
    { FnVoid            , FnVoid        , FnVoid          , FnVoid          , FnVoid          , FnVoid          , FnVoid          },
    { FnVoid            , FnVoid        , FnVoid          , FnVoid          , FnVoid          , FnVoid          , FnVoid          },
    { FnVoid            , FnVoid        , FnVoid          , FnVoid          , FnVoid          , FnVoid          , FnVoid          }
  };
  unsigned char __flash  PaswordF[InputSetup_CurPswLen] = { 3, 2, 4, 6, 1 };
  // ================================================================================================================
  char __flash init_msg1[] = "Stuck     ";
  char __flash init_msg2[] = "Sensors : ";
  void init()
  {
    step = UkWaitWorkMode;
    {
      CritSec cs;
      timeout_max = 0;
      timeout = timeout_max;
    }
    scr->Clear();
    scr->F_String(0 , init_msg1);
    scr->F_String(16, init_msg2);
  }
  // ================================================================================================================
  void WaitWorkMode()
  {
    for (unsigned char i=0; i<6; i++)
    {
      if ( ns_izmlen::dat[i]->level() )
        scr->Char(26 + i, ' ');
      else
      {
        scr->Flash = 1;
        scr->Char(26 + i, '0'+i+1);
      }
      scr->F_Char(10+i, !(bool)ns_izmlen::dat[i]->level() ); 
    }
    if ( ns_izmlen::ReadStep()==0 ) return;
    if ( ns_izmlen::ReadStep()==1 ) return;
    Default();
  }
  // ================================================================================================================
  char __flash Default_msg1[] = "Work mode ";
  char __flash Default_msg2[] = "Len=      ";
  char __flash Default_msg3[] = "Error     ";
  char __flash Default_msg4[] = "Sensors : ";
  char __flash Default_msg5[] = "Len=?           ";
  unsigned char DefaultFshow = 1;
  void Default()
  {
    {
      CritSec cs;
      timeout_max = 0;
      timeout = timeout_max;
    }
    scr->Clear();
    step = UkDefault;
    scr->F_String(0 , Default_msg1);
    DefaultFshow = 1;
    Default_v();
    for (unsigned char i=0; i<6; i++)
    {
      scr->F_Char(10+i, !(bool)ns_izmlen::dat[i]->level() ); 
    }
  }
  void Default_v()
  {
    if ( ns_izmlen::SensorNewDate || DefaultFshow )
    {
      ns_izmlen::SensorNewDate = false;
      for (unsigned char i=0; i<6; i++)
      {
        scr->F_Char(10+i, !(bool)ns_izmlen::dat[i]->level() ); 
      }
    }
    if ( ns_izmlen::flNewLen || DefaultFshow )
    {
      ns_izmlen::flNewLen = false;
      if (ns_izmlen::NewLen==0 && !DefaultFshow )
      {
        scr->F_String(0 , Default_msg3);
        scr->F_String(16, Default_msg4);
      }
      else
      {
        scr->F_String(0 , Default_msg1);
        scr->F_String(16, Default_msg2);
        scr->F_Char(16+4, '0'+(ns_izmlen::NewLen/1000) ); 
        scr->F_Char(16+5, '.' ); 
        scr->F_Digit_uz(16+6, 3, ns_izmlen::NewLen%1000 );
      }
        //scr->F_Char(16+5, '0'+ns_izmlen::nWorkDat );
        for (unsigned char i=0; i<6; i++)
        {
          if ( ns_izmlen::ErrWorkDat&(1<<i) )
          {
            scr->Flash = 1;
            scr->Char(26+i, '0'+i+1 );
          }
          else
            scr->Char(26+i, ' ' );
        }
    }
    if (ns_izmlen::ReadStep()==4)
    {
        scr->F_String(16, Default_msg5);
    }
    DefaultFshow = 0;
  }
  // ================================================================================================================
  char __flash InputSetup_msg1[] = "Input password:";
  void InputSetup()
  {
    {
      CritSec cs;
      timeout_max = 30000;
      timeout = timeout_max;
    }
    step_back = step;
    step = UkInputSetup;
    InputSetup_CurPswInd = 0;
    for (unsigned char i=0; i<InputSetup_CurPswLen; i++)
      InputSetup_CurPsw[i] = 0;
    scr->Clear();
    scr->F_String(0 , InputSetup_msg1);
    InputSetup_s();
  }
  void InputSetup_s()
  {
    unsigned char c = 0;
    for (c=0; c<InputSetup_CurPswInd; c++)
    {
      scr->Flash = 0;
//      scr->Char(16+c, '0'+InputSetup_CurPsw[c]);
      scr->Char(16+c, '*');
    }
    if (InputSetup_CurPswInd<InputSetup_CurPswLen)
    {
      scr->Flash = 1;
      scr->Char(16+InputSetup_CurPswInd, '0'+InputSetup_CurPsw[InputSetup_CurPswInd]);
    }
  }
  void InputSetup_zm()
  {
    if (InputSetup_CurPsw[InputSetup_CurPswInd]==0)
      InputSetup_CurPsw[InputSetup_CurPswInd] = 9;
    else
      InputSetup_CurPsw[InputSetup_CurPswInd]--;
    InputSetup_s();
  }
  void InputSetup_zp()
  {
    if (InputSetup_CurPsw[InputSetup_CurPswInd]>=9)
      InputSetup_CurPsw[InputSetup_CurPswInd] = 0;
    else
      InputSetup_CurPsw[InputSetup_CurPswInd]++;
    InputSetup_s();
  }
  void InputSetup_i()
  {
    InputSetup_CurPswInd++;
    if (InputSetup_CurPswInd<InputSetup_CurPswLen)
    {
      InputSetup_CurPsw[InputSetup_CurPswInd] = InputSetup_CurPsw[InputSetup_CurPswInd-1];
      InputSetup_s();
      return;
    }
    bool fl1 = true;
    bool fl2 = true;
    for (unsigned char i=0; i<InputSetup_CurPswLen; i++)
    {
      if ( PaswordE[i]!=InputSetup_CurPsw[i] ) fl1 = false;
      if ( PaswordF[i]!=InputSetup_CurPsw[i] ) fl2 = false;
    }
    if ( fl1 || fl2 )
    { // ������ ������
      SetupMenu();
    }
    else
    { // ������
      SetupError();
    }
  }
  void InputSetup_m()
  {
    if (InputSetup_CurPswInd>0)
      ExitSetup();
  }
  void ExitSetup()
  {
    MassMenu[step_back][6]();
  }
  // ================================================================================================================
  char __flash SetupError_msg1[] = "Password error!";
  void SetupError()
  {
    step = UkSetupError;
    scr->Clear();
    scr->F_String(0 , SetupError_msg1);
    {
      CritSec cs;
      timeout_max = 0;
      timeout = timeout_max;
      SetupError_timer = 30000;
    }
  }
  void SetupError_v()
  {
    unsigned int temp;
    {
      CritSec cs;
      temp = SetupError_timer;
    }
    if ( temp>1 )
    {
      scr->F_Digit_u(16, 2, temp/1000 );
    }
    else
    {
      ExitSetup();
    }
  }
  // ================================================================================================================
#define SetupLen 8
  char __flash SetupMenu_msg1[] = "Menu:";
  char __flash  SetupMenuList[SetupLen][17] = {
    { "Distance to S1  " },
    { "Distance to S2  " },
    { "Distance to S3  " },
    { "Distance to S4  " },
    { "Distance to S5  " },
    { "Distance to S6  " },
    { "Set new password" },
    { "Set K averaging " }
  };
  void SetupMenu()
  {
    step = UkSetupMenu;
    scr->Clear();
    {
      CritSec cs;
      timeout_max = 180000;
      timeout = timeout_max;
    }
    scr->F_String(0 , SetupMenu_msg1);
    SetupMenuInd = 0;
    SetupMenu_s();
  }
  void SetupMenu_s()
  {
    scr->F_String(16 , SetupMenuList[SetupMenuInd] );
  }
  void SetupMenu_m()
  {
    ExitSetup();
  }
  void SetupMenu_zm()
  {
    if (SetupMenuInd>0)
    {
      SetupMenuInd--;
    }
    SetupMenu_s();
  }
  void SetupMenu_zp()
  {
    if (SetupMenuInd<(SetupLen-1) )
    {
      SetupMenuInd++;
    }
    SetupMenu_s();
  }
  void SetupMenu_i()
  {
    if (SetupMenuInd==6)
    {
      SetPassword();
      return;
    }
    if (SetupMenuInd<6)
    {
      SetSensor();
      return;
    }
    if (SetupMenuInd==7)
    {
      SetKoff();
      return;
    }
  }
  // ================================================================================================================
  char __flash SetPassword_msg1[] = "Set password:";
  void SetPassword()
  {
    step = UkSetPassword;
    {
      CritSec cs;
      timeout_max = 60000;
      timeout = timeout_max;
    }
    scr->Clear();
    scr->F_String(0 , SetPassword_msg1 );
    for (unsigned char i=0; i<InputSetup_CurPswLen; i++)
    {
      InputSetup_CurPsw[i] = 0;
    }
    InputSetup_CurPswInd = 0;
    SetPassword_s();
  }
  void SetPassword_m()
  {
    SetupMenu();
  }
  void SetPassword_s()
  {
    for (unsigned char c=0; c<InputSetup_CurPswInd; c++)
    {
      scr->Flash = 0;
      scr->Char(16+c, '0'+InputSetup_CurPsw[c]);
//      scr->Char(16+c, '*');
    }
    if (InputSetup_CurPswInd<InputSetup_CurPswLen)
    {
      scr->Flash = 1;
      scr->Char(16+InputSetup_CurPswInd, '0'+InputSetup_CurPsw[InputSetup_CurPswInd]);
    }
  }
  void SetPassword_zm()
  {
    if ( InputSetup_CurPsw[InputSetup_CurPswInd]==0 )
      InputSetup_CurPsw[InputSetup_CurPswInd] = 9;
    else
      InputSetup_CurPsw[InputSetup_CurPswInd]--;
    SetPassword_s();
  }
  void SetPassword_zp()
  {
    if ( InputSetup_CurPsw[InputSetup_CurPswInd]>=9 )
      InputSetup_CurPsw[InputSetup_CurPswInd] = 0;
    else
      InputSetup_CurPsw[InputSetup_CurPswInd]++;
    SetPassword_s();
  }
  void SetPassword_i()
  {
    if (InputSetup_CurPswInd<(InputSetup_CurPswLen-1) )
    {
      InputSetup_CurPswInd++;
      InputSetup_CurPsw[InputSetup_CurPswInd] = InputSetup_CurPsw[InputSetup_CurPswInd-1];
      SetPassword_s();
      return;
    }
    SetPassYn();
  }
  // ================================================================================================================
  char __flash SetPassYn_msg1[] = "Save? (-/+)";
  void SetPassYn()
  {
    step = UkSetPassYn;
    {
      CritSec cs;
      timeout_max = 60000;
      timeout = timeout_max;
    }
    scr->Clear();
    scr->F_String(16 , SetPassYn_msg1 );
  }
  void SetPassYn_m()
  {
    SetupMenu();
  }
  void SetPassYn_zm()
  {
    SetupMenu();
  }
  void SetPassYn_zp()
  {
    for (unsigned char i=0; i<InputSetup_CurPswLen; i++)
      PaswordE[i] = InputSetup_CurPsw[i];
    ExitSetup();
  }
  // ================================================================================================================
  unsigned int  SetSensorDl;
  unsigned char SetSensorInd;
  char __flash SetSensor_msg1[] = "Distance to S";
  void SetSensor()
  {
    step = UkSetSensor;
    SetSensorDl = vg::rs_Dat[SetupMenuInd];
    SetSensorInd = 3;
    scr->Clear();
    scr->F_String(0 , SetSensor_msg1 );
    scr->Char(13, '1'+SetupMenuInd );
    SetSensor_s();
    {
      CritSec cs;
      timeout_max = 120000;
      timeout = timeout_max;
    }
  }
  void SetSensor_s()
  {
    unsigned int  temp1 = SetSensorDl;
    unsigned char temp2, n;
    for (unsigned char i=0; i<4; i++)
    {
      n = 3-i;
      temp2 = temp1 %10;
      if (SetSensorInd==i)
      {
        scr->Flash = 1;
      }
      else
      {
        scr->Flash = 0;
      }
      scr->Char(16+n, '0'+temp2);
      temp1 = temp1 /10;
    }
  }
  void SetSensor_m()
  {
    SetupMenu();
  }
  void SetSensor_zm()
  {
    unsigned int k = 1;
    unsigned char s;
    for(unsigned char i=0; i<SetSensorInd; i++)
      k = k*10;
    s = (SetSensorDl/k)%10;
    //scr->Char(10, '0'+s);
    if (s>0)
      SetSensorDl = SetSensorDl-k;
    SetSensor_s();
  }
  void SetSensor_zp()
  {
    unsigned int k = 1;
    unsigned char s;
    for(unsigned char i=0; i<(SetSensorInd+0); i++)
      k = k*10;
    s = (SetSensorDl/k)%10;
    //scr->Char(14, '0'+s);
    if (s<9)
      SetSensorDl = SetSensorDl+k;
    SetSensor_s();
  }
  void SetSensor_i()
  {
    if (SetSensorInd>0)
    {
      SetSensorInd--;
      SetSensor_s();
      return;
    }
    else
    {
      SetSensYn();
      return;
    }
  }
  // ================================================================================================================
  char __flash SetSensYn_msg1[] = "Save? (-/+)";
  void SetSensYn()
  {
    step = UkSetSensYn;
    scr->Clear();
    scr->F_String(16 , SetSensYn_msg1 );
    {
      CritSec cs;
      timeout_max = 30000;
      timeout = timeout_max;
    }
  }
  void SetSensYn_m()
  {
    SetupMenu();
  }
  void SetSensYn_zm()
  {
    SetupMenu();
  }
  void SetSensYn_zp()
  {
    vg::rs_Dat[SetupMenuInd] = SetSensorDl;
    SetupMenu();
  }
// ================================================================================================================
    unsigned char koffTmp = 0;
    char __flash SetKoff_msg1[] = "(V1+V2)/K  K=";
    char __flash SetKoff_msg2[] = "can.  -  + enter";
    char __flash SetKoff_msg3[] = "Save? (-/+)";
    void SetKoff()
    {
        koffTmp = vg::kf_usr;
        step = UkSetKoff;
        scr->Clear();
        scr->F_String(0            , SetKoff_msg1 );
        scr->F_String(c_stolbcov+0 , SetKoff_msg2 );
        SetKoff_v();
        {
          CritSec cs;
          timeout_max = 120000;
          timeout = timeout_max;
        }
  }
  void SetKoff_v()
  {
      scr->Char(13, '0'+(koffTmp/10)%10 );
      scr->Char(14, '.' );
      scr->Char(15, '0'+(koffTmp%10) );
  }
  void SetKoff_m()
  {
      SetupMenu();
  }
  void SetKoff_zm()
  {
      if (koffTmp<=15)
          koffTmp = 15;
      else
          koffTmp--;
      SetKoff_v();
  }
  void SetKoff_zp()
  {
      if (koffTmp>=25)
          koffTmp = 25;
      else
          koffTmp++;
      SetKoff_v();
  }
  void SetKoff_e()
  {
      SetKoffYn();
  }
  // ===============================================================================================================
    void SetKoffYn()
    {
        step = UkSetKoffYn;
        scr->Clear();
        scr->F_String(0            , SetKoff_msg1 );
        scr->F_String(c_stolbcov+0 , SetKoff_msg3 );
        {
          CritSec cs;
          timeout_max = 30000;
          timeout = timeout_max;
        }
    }
    void SetKoffYn_zm()
    {
        ExitSetup();
    }
    void SetKoffYn_zp()
    {
        vg::kf_usr = koffTmp;
        ExitSetup();
    }
  // ================================================================================================================
  // ================================================================================================================
  // ================================================================================================================
  // ================================================================================================================
  // ================================================================================================================
  // ================================================================================================================
  // ================================================================================================================
  // ================================================================================================================
  // ================================================================================================================
  // ================================================================================================================
  // ================================================================================================================
  // ================================================================================================================
  void timer()
  {
    if (timeout>1)
    {
      timeout--;
    }
    if (SetupError_timer>1)
    {
      SetupError_timer--;
    }
  }
  // ================================================================================================================
  void main()
  {
    //scr->dig_uz(26, 5, timeout);
    if ( key_status()>0 )
    {
      key_read(&key);
      {
        CritSec cs;
        if (timeout>1)
          timeout = timeout_max;
      }
      MassMenu[step][key]();
    }
    else
    {
      {
        CritSec cs;
        timeout_temp = timeout;
      }
      if ( timeout_temp==1 )
      {
        {
          CritSec cs;
          timeout = 0;
        }
        MassMenu[step][5]();
      }
      else
      {
        MassMenu[step][0]();
      }
    }
  }
}

#endif


