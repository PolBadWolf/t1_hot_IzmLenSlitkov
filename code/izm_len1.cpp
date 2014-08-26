
#include "main.h"

#define PortWarningDDR      DDRE_DDE5
#define PortWarningPORT     PORTE_PORTE5

#define nDat 8
#define nDatMask ((1<<nDat)-1)

namespace ns_izmlen1
{
    // tmp null function
    unsigned char ReadStep(){return 3;}
    bool SensorNewDate = true;
    bool flNewLen = true;
    unsigned int NewLen = true;
    unsigned char ErrWorkDat = 0;
    // massive sensors / 0 - sensor show hot tube, 1 - sensor not view /
    tc_ports1<izmLen_DatType> *dat[nDat];
#if (nDat<9)
    // bits flag events from sensors
    unsigned char datFlagEvent;
    // tmp summ err dat 
    unsigned char datErrTmp;
#elif (nDat<17)
    // bits flag events from sensors
    unsigned int  datFlagEvent;
    // tmp summ err dat 
    unsigned int  datErrTmp;
#else
    // bits flag events from sensors
    unsigned long datFlagEvent;
    // tmp summ err dat 
    unsigned long datErrTmp;
#endif
    // time massive
    unsigned long datTimeMassive[nDat][2];
    unsigned long datTime;
    // =========================================================================
    void Event1(unsigned char level, unsigned char tag);
    void Event2(unsigned char level, unsigned char tag);
    // =========================================================================
    // init wait free sensors
    void InitWaitFreeSensorsMain();
    // =========================================================================
    // wait free sensors
#define waitFreeSensorsTimeOut 180000
#if (waitFreeSensorsTimeOut<256)
    unsigned char waitFreeSensorsTime;
#elif (waitFreeSensorsTimeOut<65536)
    unsigned int  waitFreeSensorsTime;
#else
    unsigned long waitFreeSensorsTime;
#endif
    void WaitFreeSensorsMain();
    void WaitFreeSensorsTimer();
    // =========================================================================
#define izmSensorsTimeOut 180000
    void IzmBeginTimer();
    void WaitEndIzmTimer();
    // =========================================================================
    void Empty() {}
    void WarningEnabled();
    void WarningDisabled();
    // =========================================================================
    // =========================================================================
    // =========================================================================
    unsigned char eventMassStep = 0;
#define EvMain      0
#define EvTimer     1
    void (* const flash EventMass[][2])() = 
    {
    // init out dac
         {Empty                     ,Empty}
    // wait end out dac
        ,{Empty                     ,Empty}
#define InitWaitFreeSensors         2
        // init wait free sensors
        ,{InitWaitFreeSensorsMain   ,Empty}
#define WaitFreeSensors             3
        // wait free sensors
        ,{WaitFreeSensorsMain       ,WaitFreeSensorsTimer}
#define IzmBegin                    4
        // izm begin
        ,{Empty                     ,IzmBeginTimer}
#define WaitEndIzm                  5
        // wait end izm
        ,{Empty                     ,WaitEndIzmTimer}
        ,{Empty                     ,Empty}
        ,{Empty                     ,Empty}
        ,{Empty                     ,Empty}
        ,{Empty                     ,Empty}
        ,{Empty                     ,Empty}
        ,{Empty                     ,Empty}
    };
    // =========================================================================
    inline void ScanSensors()
    {
        datFlagEvent = 0;
        for (unsigned char n=0; n<nDat; n++)
        {
            dat[n]->for_timer();
        }
    }
    // =========================================================================
    void for_timer()
    {
        // call procedure out len to DAC
        // ========================
        // call read sensors
        ScanSensors();
        // ========================
        EventMass[eventMassStep][EvTimer]();
    }
    // =========================================================================
    void main()
    {
        {
            CritSec cs_event;
            EventMass[eventMassStep][EvMain]();
        }
    }
    // =========================================================================
    void Event1(unsigned char level, unsigned char tag)
    {
        if (datTimeMassive[tag][level]==0)
        {   // free cell
            datTimeMassive[tag][level] = datTime;
        }
        datFlagEvent |= 1<<tag;
    }
    void Event2(unsigned char level, unsigned char tag)
    {
    }
    // =========================================================================
    // init wait free sensors
    void InitWaitFreeSensorsMain()
    {
        // set max timeout
        waitFreeSensorsTime = waitFreeSensorsTimeOut;
        // wait free sensors
        eventMassStep = WaitFreeSensors;
    }
    // =========================================================================
    void WaitFreeSensorsMain()
    {
        unsigned char stat = 0;
        for (unsigned char n=0; n<nDat; n++)
        {   // summ status sensors
            stat |= (!dat[n]->level())<<n;
        }
        if (stat==0)
        {   // sensors free
            WarningDisabled();
            // reset time massive
            for (unsigned char n=0; n<nDat; n++)
            {   // n - sensor
                for (unsigned char l=0; l<2; l++)
                {   // l - logic level sensor : 0 - View hot; 1 - view ice
                    datTimeMassive[n][l] = 0;
                }
            }
            datTime = 1;
            // begin izm
            eventMassStep = IzmBegin;
            return;
        }
        if (waitFreeSensorsTime==0)
        {   // event timeout sensors no free
            WarningEnabled();
        }
    }
    void WaitFreeSensorsTimer()
    {   // count timeout
        if (waitFreeSensorsTime>0)
        {
            waitFreeSensorsTime--;
        }
    }
    // =========================================================================
    void IzmBeginTimer()
    {
        if (datFlagEvent)
        {   // begin izm
            datErrTmp = (1<<8)-1;
            datErrTmp &= (~datFlagEvent) & nDatMask;
            // wait end izm
            eventMassStep = WaitEndIzm;
        }
    }
    void WaitEndIzmTimer()
    {
        datTime++;
        /*
        if (izmSensorsTime<izmSensorsTimeOut)
        {
            izmSensorsTime--;
        }
        else
        {   // timeout
        }
        */
    }
    // =========================================================================
    // =========================================================================
    // =========================================================================
    // =========================================================================
    // =========================================================================
    // =========================================================================
    // =========================================================================
    void WarningEnabled()
    {
        PortWarningDDR  = 1;
        PortWarningPORT = 0;
    }
    void WarningDisabled()
    {
        PortWarningDDR  = 1;
        PortWarningPORT = 1;
    }
    // =========================================================================
    void init()
    {
        // create object sensors
    //  dat[0] = new tc_ports1<DatTypeData>(port, bit, Porog, Event, tag);
        dat[0] = new tc_ports1<izmLen_DatType>('f', 0, izmLen_DatPorog, Event1, 0);
        dat[1] = new tc_ports1<izmLen_DatType>('f', 1, izmLen_DatPorog, Event1, 1);
        dat[2] = new tc_ports1<izmLen_DatType>('f', 2, izmLen_DatPorog, Event1, 2);
        dat[3] = new tc_ports1<izmLen_DatType>('f', 3, izmLen_DatPorog, Event1, 3);
        dat[4] = new tc_ports1<izmLen_DatType>('f', 4, izmLen_DatPorog, Event1, 4);
        dat[5] = new tc_ports1<izmLen_DatType>('f', 5, izmLen_DatPorog, Event1, 5);
        dat[6] = new tc_ports1<izmLen_DatType>('f', 4, izmLen_DatPorog, Event1, 6);
        dat[7] = new tc_ports1<izmLen_DatType>('f', 5, izmLen_DatPorog, Event1, 7);
    }
    // =========================================================================
}
