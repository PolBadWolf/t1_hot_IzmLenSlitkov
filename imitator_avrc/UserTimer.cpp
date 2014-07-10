
#include "main.h"
#include "UserTimer.h"

#ifdef DISPLAY_TIMER
void UserTimerDisplay(void)
{
}
#endif

#ifdef USERTIMERUSER
//bool fl = true;
//int sch;
unsigned int t_count = 0;
unsigned int de_dac = 0;
signed char fl_dac = 1;

void UserTimerUser(void)
{
  //PowerDown::timer();
  og::timer();
  t_count++;
  if (t_count>=1000) t_count = 0;
  ad420_SwapWord(de_dac);
  de_dac += fl_dac;
  if ( (fl_dac<0) && (de_dac==0) )
  {
    fl_dac = 1;
  }
  if ( (fl_dac>0) && (de_dac==65535) )
  {
    fl_dac = -1;
  }
  ImitWorkTimer();
}
#endif


