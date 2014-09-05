
#include "../main.h"
// ==============================
// Watchdog Timer Prescale :
//   16 = 16ms
//   32 = 28.1ms
//   64 = 56.2ms
//  128 = 0.11s
//  256 = 0.22s
//  512 = 0.45s
// 1024 = 0.9s
// 2048 = 1.8s
#define WDP_SCA 2048
#if (WDP_SCA==16)
#define WDP_0 0
#define WDP_1 0
#define WDP_2 0
#elif (WDP_SCA==32)
#define WDP_0 1
#define WDP_1 0
#define WDP_2 0
#elif (WDP_SCA==64)
#define WDP_0 0
#define WDP_1 1
#define WDP_2 0
#elif (WDP_SCA==128)
#define WDP_0 1
#define WDP_1 1
#define WDP_2 0
#elif (WDP_SCA==256)
#define WDP_0 0
#define WDP_1 0
#define WDP_2 1
#elif (WDP_SCA==512)
#define WDP_0 1
#define WDP_1 0
#define WDP_2 1
#elif (WDP_SCA==1024)
#define WDP_0 0
#define WDP_1 1
#define WDP_2 1
#elif (WDP_SCA==2048)
#define WDP_0 1
#define WDP_1 1
#define WDP_2 1
#else
#error Error sca Watchdog
#endif

namespace ns_wdogtimer
{
    __monitor void enabled()
    {
        unsigned char ini_pre = (1<<WDCE) | (1<<WDE) | (    0<<WDP0) | (    0<<WDP1) | (    0<<WDP2);
        unsigned char ini_pos = (0<<WDCE) | (1<<WDE) | (WDP_0<<WDP0) | (WDP_1<<WDP1) | (WDP_2<<WDP2);
        WDTCR = ini_pre;
        WDTCR = ini_pos;
    }
    __monitor void disabled()
    {
        unsigned char ini_pre = (1<<WDCE) | (1<<WDE) | (    0<<WDP0) | (    0<<WDP1) | (    0<<WDP2);
        unsigned char ini_pos = (0<<WDCE) | (0<<WDE) | (WDP_0<<WDP0) | (WDP_1<<WDP1) | (WDP_2<<WDP2);
        WDTCR = ini_pre;
        WDTCR = ini_pos;
    }
    __monitor void reset()
    {
        __clear_watchdog_timer();
    }
    WatchDogOff::WatchDogOff()
    {
        stat = WDTCR & WDE;
        disabled();
    }
    WatchDogOff::~WatchDogOff()
    {
        if (stat)
        {
            enabled();
            reset();
        }
    }
}
