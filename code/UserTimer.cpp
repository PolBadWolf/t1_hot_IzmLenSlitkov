
#include "main.h"
#ifdef DISPLAY_TIMER
void UserTimerDisplay(void)
{
}
#endif
#ifdef USERTIMERUSER
#include "UserTimer.h"
//bool fl = true;
//int sch;
unsigned int t_count = 0;
unsigned int de_dac = 0;
signed char fl_dac = 1;

void UserTimerUser(void)
{
  og::for_timer();
}
#endif


