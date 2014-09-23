
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

#define ftUpTimer ftUserTimer
void UpTimeInc()
{
    vg::upTimeMS++;
    if (vg::upTimeMS<ftUpTimer) return;
    vg::upTimeMS = 0;
    vg::upTimeSN = 1;
    vg::upTimeS++;
    if (vg::upTimeS<60) return;
    vg::upTimeS = 0;
    vg::upTimeM++;
    if (vg::upTimeM<60) return;
    vg::upTimeM = 0;
    vg::upTimeH++;
    if (vg::upTimeH<24) return;
    vg::upTimeH = 0;
    vg::upTimeD++;
}
#undef ftUpTimer

void UserTimerUser(void)
{
  og::for_timer();
  UpTimeInc();
}
#endif


