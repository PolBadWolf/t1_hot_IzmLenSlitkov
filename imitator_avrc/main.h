
#ifndef main__h
#define main__h
#include <ioavr.h>
#include <ina90.h>
#include <inavr.h>
#include "Configure.c"
// ==============================
#define _delay_us(us)     __delay_cycles((C_Fosc / 1000000) * (us));
#define _delay_ms(ms)     __delay_cycles((C_Fosc / 1000) * (ms));

class CritSec {
private:
    unsigned char sav;
public:
    CritSec() {
        sav = __save_interrupt();
        __disable_interrupt();
    }
    ~CritSec() {
        __restore_interrupt(sav);
    }
};

#include "system/ports.h"
//-------------------------------
// часы
#ifdef CLOCK
#include "system/clock.h"
#define DISPLAY_TIMER
#endif
//-------------------------------
#if defined(LCD) && defined(I7S)
#error LCD & I7S On
#endif
//-------------------------------
#ifdef LCD
#include "system/lcd.h"
extern tc_lcd *scr;
#define DISPLAY_TIMER
#endif
//-------------------------------
#ifdef I7S
//#include "lcd.h"
#define DISPLAY_TIMER
#endif
//-------------------------------
#ifdef KEY4
#include "system/key4.h"
#define DISPLAY_TIMER
#endif
//-------------------------------
#ifdef MENU
#define DISPLAY_TIMER
#endif
//-------------------------------
#ifdef USERTIMERDISPLAY
#define DISPLAY_TIMER
#endif
//-------------------------------
#ifdef RS485
#include "system/rs485.h"
#endif
//-------------------------------
#ifdef RS232
#include "system/rs232.h"
#endif
//-------------------------------
#ifdef CRC
#include "system/crc.h"
#endif
//-------------------------------
#ifdef USERTIMERUSER
#include "system/SystemTimerUser.h"
#endif
//-------------------------------
#ifdef DISPLAY_TIMER
#include "system/DisplayTimer.h"
#endif
#include "UserTimer.h"
#include "variable.h"
#include "object.h"
#include "menu.h"
//-------------------------------
#define PorogDatch 20
#endif
#include "dac_ad420.h"
#include "ReadDat.h"
#include "ImitWorkSlitok.h"

#define PortPC  ns_rs232
