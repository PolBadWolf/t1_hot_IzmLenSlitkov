
#include "../main.h"

void InitSystem(void)
{
  #ifdef DISPLAY_TIMER
  ns_DisplayTimer::TimerInit();
  #endif
  #ifdef USERTIMERUSER
  ns_TimerUser::Init();
  #endif
  //---------------------
  #ifdef RS485
  ns_rs485::init();
  #endif
  //---------------------
  #ifdef RS232
  ns_rs232::init();
  #endif
  //---------------------
  vg::init();
  //---------------------
  og::init();
  //---------------------
}

#ifdef LCD
tc_lcd *scr = new tc_lcd;
#endif

