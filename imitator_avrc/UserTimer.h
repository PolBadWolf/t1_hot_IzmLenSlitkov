
#ifndef UserTimer__h
#define UserTimer__h

#ifdef DISPLAY_TIMER
void UserTimerDisplay(void);
#endif

#ifdef USERTIMERUSER
void UserTimerUser(void);
extern unsigned int t_count;
#endif

#endif

