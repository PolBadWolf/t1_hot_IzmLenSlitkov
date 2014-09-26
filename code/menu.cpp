
#include "system/Configure.h"
#ifdef MENU
#include "main.h"
#include "menu.h"

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//            показ старого пароля при установки нового пароля
// #define ShowPass
//              закрывать звездочками новый вводимый пароль
// #define ShowStar
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

namespace ns_menu
{
  // текущий шаг пункта меню
  unsigned char step;
  unsigned char step_back;
  // код нажатой кнопки
  unsigned char key;
  // тайм аут в меню
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
  unsigned char __eeprom PaswordE[InputSetup_CurPswLen] = { 0, 0, 0, 0, 0 };
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
  void SetupMenu_time();
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
  void DebugScrDac_zm();
  void DebugScrDac_zp();
  void DebugScrDac();
  void DebugScrDac_s();
  void DebugScrDac_zero();
  void DebugScrDac_to();
  unsigned int znDac = 0;
  // ================================================================================================================
  void LevelSpeed();
  void LevelSpeed_v();
  void LevelSpeed_zm();
  void LevelSpeed_zp();
  void LevelSpeedYn();
  void LevelSpeedYn_zm();
  void LevelSpeedYn_zp();
  // ================================================================================================================
    void DebugScrSpeed();
    void DebugScrSpeed_v();
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
    { SetupMenu_time    , SetupMenu_m   , SetupMenu_zm    , SetupMenu_zp    , SetupMenu_i     , ExitSetup       , SetupMenu       },
#define UkSetPassword     6
    { FnVoid            , SetPassword_m , SetPassword_zm  , SetPassword_zp  , SetPassword_i   , ExitSetup       , SetPassword     },
#define UkSetPassYn       7
    { FnVoid            , SetPassYn_m   , SetPassYn_zm    , SetPassYn_zp    , FnVoid          , ExitSetup       , SetPassYn       },
#define UkSetSensor       8
    { FnVoid            , SetSensor_m   , SetSensor_zm    , SetSensor_zp    , SetSensor_i     , ExitSetup       , SetSensor       },
#define UkSetSensYn       9
    { FnVoid            , SetSensYn_m   , SetSensYn_zm    , SetSensYn_zp    , FnVoid          , ExitSetup       , SetSensYn       },
#define UkDebugScrSpeed   10
    { DebugScrSpeed_v   , Default       , FnVoid          , FnVoid          , FnVoid          , FnVoid          , DebugScrSpeed   },
#define UkDebugScrDac     11
    { FnVoid            , Default       , DebugScrDac_zm  , DebugScrDac_zp  , DebugScrDac_zero, DebugScrDac_to  , DebugScrDac     },
#define UkLevelSpeed      12
    { FnVoid            , SetupMenu     , LevelSpeed_zm   , LevelSpeed_zp   , LevelSpeedYn    , Default         , LevelSpeed      },
#define UkLevelSpeedYn    13
    { FnVoid            , FnVoid        , LevelSpeedYn_zm , LevelSpeedYn_zp , FnVoid          , Default         , LevelSpeedYn    },
    { FnVoid            , FnVoid        , FnVoid,           FnVoid,           FnVoid          , FnVoid          , FnVoid          }
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
    //DebugScrDac();
  }
  // ================================================================================================================
  void WaitWorkMode()
  {
    for (unsigned char i=0; i<8; i++)
    {
      if ( ns_izmlen::dat[i]->level() )
        scr->Char(16+ 8+i, ' ');
      else
      {
        scr->Flash = 1;
        scr->Char(16+ 8+i, '0'+i+1);
      }
      scr->F_Char( 0+ 8+i, !(bool)ns_izmlen::dat[i]->level() ); 
    }
    if ( ns_izmlen::ReadStep()==0 ) return;
    if ( ns_izmlen::ReadStep()==1 ) return;
    if ( ns_izmlen::ReadStep()==2 ) return;
    if ( ns_izmlen::ReadStep()==3 ) return;
    Default();
  }
  // ================================================================================================================
  //char __flash Default_msg1[] = "Work    ";
  char __flash Default_msg2[] = "L=      ";
  char __flash Default_msg3[] = "Error   ";
  char __flash Default_msg4[] = "Sensors:";
  char __flash Default_msg5[] = "L=?             ";
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
//    scr->F_String(0 , Default_msg1);
    scr->F_String(0 , vg::teleMsg);
    DefaultFshow = 1;
    Default_v();
    /*
    for (unsigned char i=0; i<8; i++)
    {
      scr->F_Char( 0+ 8+i, !(bool)ns_izmlen::dat[i]->level() ); 
    }
    */
  }
  void Default_v()
  {
    unsigned char flLocLen = 0, flLocSens = 0;
    if (DefaultFshow)
    {
        flLocLen = 1;
        flLocSens = 1;
    }
    if ( ns_izmlen::flNewLen )
    {
        ns_izmlen::flNewLen = false;
        flLocLen = 1;
        flLocSens = 1;
    }
    if ( ns_izmlen::SensorNewDate() )
    {
        ns_izmlen::SensorNewDateReset();
        flLocSens = 1;
    }
    // ============
    if ( flLocLen )
    {
//        /*
      if (ns_izmlen::NewLen==0 && !DefaultFshow )
      {
        scr->F_String(0 , Default_msg3);
        scr->F_String(16, Default_msg4);
      }
      else
//          */
      {
        // scr->F_String(0 , Default_msg1);
        scr->F_String(0 , vg::teleMsg);
        scr->F_String(16, Default_msg2);
        scr->F_Digit_u (16+2, 2, ns_izmlen::NewLen/1000 );
        scr->F_Char(16+4, '.' ); 
        scr->F_Digit_uz(16+5, 3, ns_izmlen::NewLen%1000 );
      }
        //scr->F_Char(16+5, '0'+ns_izmlen::nWorkDat );
        for (unsigned char i=0; i<8; i++)
        {
          if ( ns_izmlen::ErrWorkDat&(1<<i) )
          {
            scr->Flash = 1;
            scr->Char(16+ 8+i, '0'+i+1 );
          }
          else
            scr->Char(16+ 8+i, ' ' );
        }
    }
    if ( flLocSens )
    {
      for (unsigned char i=0; i<8; i++)
      {
        scr->F_Char( 0+ 8+i, !(bool)ns_izmlen::dat[i]->level() ); 
      }
    }
    if (ns_izmlen::ReadStep()==5)
    {
        //scr->F_String(0 , Default_msg1);
        scr->F_String(0 , vg::teleMsg);
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
    { // пароль совпал
      SetupMenu();
    }
    else
    { // ошибка
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
#define SetupLen 12
  char __flash SetupMenu_msg1[] = "Menu:";
  char __flash  SetupMenuList[SetupLen][17] = {
     { "Distance to S1  " }
    ,{ "Distance to S2  " }
    ,{ "Distance to S3  " }
    ,{ "Distance to S4  " }
    ,{ "Distance to S5  " }
    ,{ "Distance to S6  " }
    ,{ "Distance to S7  " }
    ,{ "Distance to S8  " }
    ,{ "Debug scr speed " }
    ,{ "Debug out DAC   " }
    ,{ "Set %level speed" }
    ,{ "Set new password" }
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
  void SetupMenu_time()
  {
      unsigned char tmp;
      {
          CritSec cs_tik;
          tmp = vg::upTimeSN;
      }
      if (tmp)
      {
          {
              CritSec cs_tak;
              vg::upTimeSN = 0;
          }
          scr->F_Digit_u ( 6, 3, vg::upTimeD );
          scr->F_Char    ( 9, 'd' );
          scr->F_Digit_uz(10, 2, vg::upTimeH );
          scr->F_Char    (12, 'h' );
          scr->F_Digit_uz(13, 2, vg::upTimeM );
          scr->F_Char    (15, 'm' );
      }
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
    if (SetupMenuInd==11)
    {
      SetPassword();
      return;
    }
    if (SetupMenuInd<8)
    {
      SetSensor();
      return;
    }
    if (SetupMenuInd==8)
    {
      DebugScrSpeed();
      return;
    }
    if (SetupMenuInd==9)
    {
      DebugScrDac();
      return;
    }
    if (SetupMenuInd==10)
    {
      LevelSpeed();
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
    SetSensorInd = 4;
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
    for (unsigned char i=0; i<5; i++)
    {
      n = 4-i;
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
    char __flash DebugScrDac_msg1[] = "Debug Out DAC";
    char __flash DebugScrDac_msg2[] = "16000mm-20.000mA";
    void DebugScrDac()
    {
        step = UkDebugScrDac;
        znDac = 1000;
        scr->Clear();
        scr->F_String(0 , DebugScrDac_msg1 );
        DebugScrDac_s();
        {
            CritSec cs;
            timeout_max = 120000;
            timeout = timeout_max;
        }
    }
    void DebugScrDac_s()
    {
        scr->F_String (16, DebugScrDac_msg2 );
        scr->F_Digit_u(16, 5, znDac );
        unsigned int tmp = znDac+4000; // 0-16000 to 4-20
        scr->F_Digit_u (16+8, 2, tmp/1000 );
        scr->F_Digit_uz(16+11, 3, tmp%1000 );
        ad420_OutDac(znDac);
    }
    void DebugScrDac_to()
    {
        DebugScrDac_zero();
        Default();
    }
    void DebugScrDac_zero()
    {
        ad420_OutDac(0);
    }
    void DebugScrDac_zm()
    {
        unsigned int tmp = znDac/1000;
        if ( tmp>0 )
            tmp--;
        znDac = tmp*1000;
        DebugScrDac_s();
    }
    void DebugScrDac_zp()
    {
        unsigned int tmp = znDac/1000;
        if ( tmp<16 )
            tmp++;
        znDac = tmp*1000;
        DebugScrDac_s();
    }
  // ===============================================================================================================
  // ================================================================================================================
    void DebugScrSpeed()
    {
        step = UkDebugScrSpeed;
        {
          CritSec cs;
          timeout_max = 0;
          timeout = timeout_max;
        }
        scr->Clear();
        DefaultFshow = 1;
        DebugScrSpeed_v();
    }
    void DebugScrSpeed_v()
    {
        unsigned char flLocLen = 0, flLocSen = 0;
        if (DefaultFshow)
        {
            flLocLen = 1;
            flLocSen = 1;
        }
        if ( ns_izmlen::flNewLen )
        {
            ns_izmlen::flNewLen = false;
            flLocLen = 1;
            flLocSen = 1;
        }
        if ( ns_izmlen::SensorNewDate() )
        {
            flLocSen = 1;
            ns_izmlen::SensorNewDateReset();
        }
        // ============
        if ( flLocLen )
        {
            scr->F_Digit_u ( 0, 1, ns_izmlen::NewLen/1000 );
            scr->F_Char(1, '.' ); 
            scr->F_Digit_uz(2, 2, (ns_izmlen::NewLen%1000)/10 );
        }
        // ============
        if ( flLocSen )
        {
            signed long dTime;
            unsigned long len, speed;
            unsigned char nach = 1;
            for (unsigned char i=nach; i<6;i++)
            {
                dTime = ns_izmlen::datTimeMassive[i][1]-ns_izmlen::datTimeMassive[i-1][1];
                unsigned char p = 16 + (i-nach)*3;
                if (dTime<=0)
                {
                    scr->F_Char(p+0, ' ' );
                    scr->F_Char(p+1, '-' );
                    scr->F_Char(p+2, '-' );
                }
                else
                {
                    len = vg::rs_Dat[i]-vg::rs_Dat[i-1];
//                    speed = len*10/dTime;
                    speed = (len*ftUserTimer)/(dTime*10);
                    speed = (speed+5)/10;
                    scr->F_Char(p+0, ' ' );
                    scr->F_Char(p+1, '0'+(speed/10)%10 );
                    scr->F_Char(p+2, '0'+(speed%10) );
                }
            }
            nach = 5;
            for (unsigned char i=nach; i<8;i++)
            {
                dTime = ns_izmlen::datTimeMassive[i][0]-ns_izmlen::datTimeMassive[i-1][0];
                unsigned char p = 6 + (i-nach)*3;
                if (dTime<=0)
                {
                    scr->F_Char(p+0, ' ' );
                    scr->F_Char(p+1, '-' );
                    scr->F_Char(p+2, '-' );
                }
                else
                {
                    len = vg::rs_Dat[i]-vg::rs_Dat[i-1];
//                    speed = len*10/dTime;
                    speed = (len*ftUserTimer)/(dTime*10);
                    speed = (speed+5)/10;
                    scr->F_Digit_u ( p, 3, (unsigned char)speed );
                    /*
                    scr->F_Char(p+0, ' ' );
                    scr->F_Char(p+1, '0'+(speed/10)%10 );
                    scr->F_Char(p+2, '0'+(speed%10) );*/
                }
            }
        }
    }
  // ================================================================================================================
    unsigned int levelSpeedTmp;
    char __flash levelSpeed_msg1[] = "Sub Speed";
    char __flash levelSpeed_msg2[] = "level=200.0%";
    void LevelSpeed()
    {
        step = UkLevelSpeed;
        {
          CritSec cs;
          timeout_max = 60000;
          timeout = timeout_max;
        }
        scr->Clear();
        scr->F_String ( 0, levelSpeed_msg1);
        scr->F_String (16, levelSpeed_msg2);
        levelSpeedTmp = vg::prcPorog;
        LevelSpeed_v();
    }
    void LevelSpeed_v()
    {
        scr->F_Digit_u (22, 3, (unsigned int)levelSpeedTmp/10 );
        scr->F_Char    (26, '0'+(levelSpeedTmp%10) );
    }
    void LevelSpeed_zm()
    {
        if (levelSpeedTmp>25)  // 2.5%
        {
            levelSpeedTmp--;
            LevelSpeed_v();
        }
    }
    void LevelSpeed_zp()
    {
        if (levelSpeedTmp<4000) // 400.0%
        {
            levelSpeedTmp++;
            LevelSpeed_v();
        }
    }
    char __flash levelSpeedYn_msg1[] = "Save? (-/+)";
    void LevelSpeedYn()
    {
        step = UkLevelSpeedYn;
        scr->Clear();
        scr->F_String(16 , levelSpeedYn_msg1 );
        {
          CritSec cs;
          timeout_max = 30000;
          timeout = timeout_max;
        }
    }
    void LevelSpeedYn_zp()
    {
        vg::prcPorog = levelSpeedTmp;
        ExitSetup();
    }
    void LevelSpeedYn_zm()
    {
        SetupMenu();
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


