
#include "main.h"

#define PortWarningDDR      DDRE_DDE5
#define PortWarningPORT     PORTE_PORTE5

#define nDat 8
//#define nDatMask ((1<<nDat)-1)

namespace ns_izmlen1
{
    // tmp null function
    bool flNewLen = true;
    unsigned int NewLen;
    unsigned char ErrWorkDat = 0;
    unsigned long lenRender;
    // massive sensors / 0 - sensor show hot tube, 1 - sensor not view /
    tc_ports1<izmLen_DatType> *dat[nDat];
#if (nDat<9)
    // bits flag events from sensors
    unsigned char datFlagEvent;
    // tmp summ err dat 
    unsigned char datErrTmp;
    // temp cur work dat
    unsigned char datWorkTmp;
#elif (nDat<17)
    // bits flag events from sensors
    unsigned int  datFlagEvent;
    // tmp summ err dat 
    unsigned int  datErrTmp;
    // temp cur work dat
    unsigned int  datWorkTmp;
#else
    // bits flag events from sensors
    unsigned long datFlagEvent;
    // tmp summ err dat 
    unsigned long datErrTmp;
    // temp cur work dat
    unsigned long datWorkTmp;
#endif
    // time massive
    unsigned long datTimeMassive[nDat][2];
    unsigned long datTime;
    unsigned char datTimeFlagReg = 0;
      signed char datRegCurMax = -1;
    // =========================================================================
    void Event1(unsigned char level, unsigned char tag);
    void Event2(unsigned char level, unsigned char tag);
    // =========================================================================
    // init wait free sensors
    void InitWaitFreeSensorsMain();
    // =========================================================================
    // wait free sensors
#define waitFreeSensorsTimeOut 180*ftUserTimer
#if (waitFreeSensorsTimeOut<256)
    #define waitFreeSensorsTimeDef unsigned char
#elif (waitFreeSensorsTimeOut<65536)
    #define waitFreeSensorsTimeDef unsigned int
#else
    #define waitFreeSensorsTimeDef unsigned long
#endif
    waitFreeSensorsTimeDef waitFreeSensorsTime;
    void WaitFreeSensorsMain();
    void WaitFreeSensorsTimer();
    // =========================================================================
#define izmSensorsTimeOutMax 15*ftUserTimer
#if (izmSensorsTimeOutMax<256)
    #define izmSensorsTimeOutDef unsigned char
#elif (izmSensorsTimeOutMax<65536)
    #define izmSensorsTimeOutDef unsigned int
#else
    #define izmSensorsTimeOutDef unsigned long
#endif
    izmSensorsTimeOutDef izmSensorsTimeOut = (izmSensorsTimeOutDef)izmSensorsTimeOutMax;
    void IzmBeginTimer();
    void WaitEndIzmTimer();
    void WaitEndIzmMain();
    unsigned char waitEndIzmOk;
    // =========================================================================
    unsigned char eventMassStep;
    void Empty() {}
    void EmptyNext() {eventMassStep++;}
    void WarningEnabled();
    void WarningDisabled();
    unsigned char CheckFreeSensors();
    // =========================================================================
    void IzmRenderMain();
    // =========================================================================
    unsigned int outDacCount;
#define outDacCountMax 5*ftUserTimer
    void InitOutDacMain();
    void WaitEndOutDacTimer();
    // =========================================================================
    // =========================================================================
    signed long speedObr;
    // =========================================================================
#define EvMain      0
#define EvTimer     1
    void (* const flash EventMass[][2])() = 
    {
#define InitOutDac                  0
    // init out dac
         {InitOutDacMain            ,Empty}
#define WaitEndOutDac               1
    // wait end out dac
        ,{Empty                     ,WaitEndOutDacTimer}
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
        ,{WaitEndIzmMain            ,WaitEndIzmTimer}
#define IzmRender                   6
        ,{IzmRenderMain             ,Empty}
        ,{Empty                     ,Empty}
        ,{Empty                     ,Empty}
        ,{Empty                     ,Empty}
        ,{Empty                     ,Empty}
        ,{Empty                     ,Empty}
    };
    // =========================================================================
    void SensorNewDateReset()
    {
        CritSec cs_sens;
        datFlagEvent = 0;
    }
    inline void ScanSensors()
    {
        SensorNewDateReset();
        for (unsigned char n=0; n<nDat; n++)
        {
            dat[n]->for_timer();
        }
    }
    unsigned char SensorNewDate()
    {
        unsigned char st;
        {
            CritSec cs_sens;
            st = datFlagEvent;
        }
        return st;
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
            NewLen = lenRender;
            ErrWorkDat = datErrTmp;
        }
    }
    unsigned char ReadStep()
    {
        unsigned char step;
        {
            CritSec cs_event;
            step = eventMassStep;
        }
        return step;
    }
    unsigned char blockEvent[8] = {3,3,3,3,3,3,3,3};
    // =========================================================================
    void Event1(unsigned char level, unsigned char tag)
    {
        // set event sensor
        datFlagEvent |= 1<<tag;
        //
        blockEvent[tag] = level;
        if (datTimeFlagReg)
        {
            bool ok = false;
            if ( (datTimeMassive[tag][level]==0) && (level==0) )
            {
                ok = true;
            }
            if ( (level==1) && (tag==0) )
            {
                ok = true;
            }
            if ( (level==1) && (tag>0) )
            {
                if (blockEvent[tag-1])
                {
                    ok = true;
                }
            }
            if (ok)
            {
                izmSensorsTimeOut = (izmSensorsTimeOutDef)izmSensorsTimeOutMax;
                // reg time event sensor
                datTimeMassive[tag][level] = datTime;
                // mask working sensors
                datWorkTmp |= (1<<tag);
                // curent max number sensor
                if (datRegCurMax<tag)
                {
                    datRegCurMax = tag;
                }
            }
        }
    }
    void Event2(unsigned char level, unsigned char tag)
    {
    }
    // =========================================================================
    // init wait free sensors
    void InitWaitFreeSensorsMain()
    {
        // set max timeout
        waitFreeSensorsTime = (waitFreeSensorsTimeDef)waitFreeSensorsTimeOut;
        // wait free sensors
        eventMassStep = WaitFreeSensors;
    }
    // =========================================================================
    void WaitFreeSensorsMain()
    {
        if (CheckFreeSensors()!=0)
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
            // count time;
            datTime = 1;
            // reset cur work sensors
            datWorkTmp = 0;
            // unreg cur max number sensor
            datRegCurMax = -1;
            // reg time event sensor On
            datTimeFlagReg = 1;
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
            waitEndIzmOk = 0;
            // reg err sensors
            datErrTmp = ((1<<(datRegCurMax+1))-1) & (~datWorkTmp);
            // wait end izm
            eventMassStep = WaitEndIzm;
        }
    }
    void WaitEndIzmTimer()
    {
        if (izmSensorsTimeOut>0)
        {
            izmSensorsTimeOut--;
            datTime++;
            // wait free sensors
            if (CheckFreeSensors()!=0)
            {   // sensor free
                waitEndIzmOk = 1;
                if ( (datTimeMassive[6][0]!=0) && (datTimeMassive[7][0]!=0) )
                {
                // end !!!!
                eventMassStep = IzmRender;
                // reg time event sensor Off
                datTimeFlagReg = 0;
                }
            }
        }
        else
        {   // timeout
            if (waitEndIzmOk)
            {
                // end !!!!
                eventMassStep = IzmRender;
                // reg time event sensor Off
                datTimeFlagReg = 0;
            }
            else
            {
                // error - reset izmer & 
                datTimeFlagReg = 0;
                WarningEnabled();
                eventMassStep = IzmRender;
            }
        }
    }
    void WaitEndIzmMain()
    {
        if (datFlagEvent)
        {
            datErrTmp = ((1<<(datRegCurMax+1))-1) & (~datWorkTmp);
        }
        else
        {
            datErrTmp = 0;
        }
    }
    signed int Procent(signed int speed1, signed int speed2)
    {
        signed int x;
        if (speed2>speed1)
        {
            x = speed1;
            speed1 = speed2;
            speed2 = x;
        }
        x = ((signed long)1000*speed1/speed2)-1000;
        return x;
    }
    // =========================================================================
    struct st_mss
    {
        unsigned char napr;
        unsigned char modeRender;
        //
        unsigned char fist;
        unsigned char next;
        unsigned char sFist;
        unsigned char sNext;
        unsigned int  dochet;
        unsigned int  Len;
        unsigned char pm;
    };
#define st_mss_lim 100
        st_mss        mss[st_mss_lim];
        unsigned char mss_max = 0;
        // nDat - all sensors (const)
          signed char map[nDat];
        unsigned char maxn = 0;
        //
        class strSpeed
        {
        public:
            unsigned int  sLen;
            unsigned long sTime;
            signed char sb, se, pr;
        };
        void speedSelect(signed char d, signed char u, unsigned char maxn, signed long dochet, unsigned char *sB, unsigned char *sE, unsigned int *sLen, unsigned long *sTime, bool dp)
        {
            signed char sb, se;
            strSpeed mass[6];
            unsigned char massMax = 0;
            // [u]-[u-1]
            se = u;
            sb = u-1;
            if ( (se<maxn) && (sb>=0) && (datTimeMassive[map[se]][0]>0) && (datTimeMassive[map[sb]][0]>0) )
            {
                mass[massMax].sb    = sb;
                mass[massMax].se    = se;
                mass[massMax].sLen  =     vg::rs_Dat[map[se]]   -    vg::rs_Dat[map[sb]];
                mass[massMax].sTime = datTimeMassive[map[se]][0]-datTimeMassive[map[sb]][0];
                mass[massMax].pr    = 1; // check offset < len for speed
                massMax++;
            }
            // [d]-[d-1]
            se = d;
            sb = d-1;
            if ( (se<maxn) && (sb>=0) && (datTimeMassive[map[se]][1]>0) && (datTimeMassive[map[sb]][1]>0) )
            {
                mass[massMax].sb    = sb;
                mass[massMax].se    = se;
                mass[massMax].sLen  =     vg::rs_Dat[map[se]]   -    vg::rs_Dat[map[sb]];
                mass[massMax].sTime = datTimeMassive[map[se]][1]-datTimeMassive[map[sb]][1];
                mass[massMax].pr    = 0; // no check offset < len for speed
                massMax++;
            }
            // [u+1]-[u]
            se = u+1;
            sb = u;
            if ( (se<maxn) && (sb>=0) && (datTimeMassive[map[se]][0]>0) && (datTimeMassive[map[sb]][0]>0) )
            {
                mass[massMax].sb    = sb;
                mass[massMax].se    = se;
                mass[massMax].sLen  =     vg::rs_Dat[map[se]]   -    vg::rs_Dat[map[sb]];
                mass[massMax].sTime = datTimeMassive[map[se]][0]-datTimeMassive[map[sb]][0];
                mass[massMax].pr    = 1; //  check offset < len for speed
                massMax++;
            }
            // [u]-[u-2]
            se = u;
            sb = u-2;
            if ( (se<maxn) && (sb>=0) &&(datTimeMassive[map[se]][0]>0) && (datTimeMassive[map[sb]][0]>0) )
            {
                mass[massMax].sb    = sb;
                mass[massMax].se    = se;
                mass[massMax].sLen  =     vg::rs_Dat[map[se]]   -    vg::rs_Dat[map[sb]];
                mass[massMax].sTime = datTimeMassive[map[se]][0]-datTimeMassive[map[sb]][0];
                mass[massMax].pr    = 0; // no check offset < len for speed
                massMax++;
            }
            // [d+1]-[d]
            se = d+1;
            sb = d;
            if ( (se<maxn) && (sb>=0) && (datTimeMassive[map[se]][1]>0) && (datTimeMassive[map[sb]][1]>0) )
            {
                mass[massMax].sb    = sb;
                mass[massMax].se    = se;
                mass[massMax].sLen  =     vg::rs_Dat[map[se]]   -    vg::rs_Dat[map[sb]];
                mass[massMax].sTime = datTimeMassive[map[se]][1]-datTimeMassive[map[sb]][1];
                mass[massMax].pr    = 1;
                massMax++;
            }
            // find min
            unsigned char n;
            if (dp) n = 1;
            else    n = 0;
            for (unsigned char i=0; i<massMax; i++)
            {
                if ( (mass[i].pr==0) || (dochet<mass[i].sTime) )
                {
                    if ( mass[n].sLen>(mass[i].sLen+30) )
                    {
                        n = i;
                    }
                }
            }
            // save
            *sLen  = mass[n].sLen;
            *sTime = mass[n].sTime;
            *sB    = mass[n].sb;
            *sE    = mass[n].se;
        }            
        unsigned char saveRenderB(signed char d, signed char u, unsigned char maxn)
        {
            if ( mss_max>=st_mss_lim )   return 1;
            if ( (d<0) || (u>=maxn)  )   return 0;
            unsigned char stat = 0;
            unsigned int base;
            unsigned long offSet;
            unsigned char sB,   sE;
            unsigned int  sLen;
            unsigned long sTime;
            base   =     vg::rs_Dat[map[u]]   -    vg::rs_Dat[map[d]];
            offSet = datTimeMassive[map[d]][1]-datTimeMassive[map[u]][0];
            speedSelect(d, u, maxn, offSet, &sB, &sE, &sLen, &sTime, 1);
            offSet = ((unsigned long)offSet*sLen/sTime);
            mss[mss_max].napr   = 1;
            mss[mss_max].fist   = d;
            mss[mss_max].next   = u;
            mss[mss_max].sFist  = sB;
            mss[mss_max].sNext  = sE;
            mss[mss_max].dochet = offSet;
            mss[mss_max].Len    = base+offSet;
            mss[mss_max].pm     = 1; // +
            mss_max++;
            if ( mss_max>=st_mss_lim ) stat = 1;
            return stat;
        }
        unsigned char saveRender1(signed char d, signed char u, unsigned char maxn)
        {
            if ( mss_max>=st_mss_lim )   return 2;
            if ( (d<0) || (u>=maxn)  )   return 0;
            unsigned char stat = 0;
            {
                unsigned int base;
                  signed long offSet;
                unsigned char sB,   sE;
                unsigned int  sLen;
                unsigned long sTime;
                offSet = datTimeMassive[map[d  ]][1]-datTimeMassive[map[u-1]][0];
                if (offSet>0)
                {
                    base   =     vg::rs_Dat[map[u-1]]   -    vg::rs_Dat[map[d  ]];
                    speedSelect(d, u-1, maxn, offSet, &sB, &sE, &sLen, &sTime, 1);
                    offSet = ((unsigned long)offSet*sLen*10/sTime+5)/10;
                    if ( offSet<base )
                    {
                        mss[mss_max].napr   = 3;
                        mss[mss_max].fist   = d;
                        mss[mss_max].next   = u-1;
                        mss[mss_max].sFist  = sB;
                        mss[mss_max].sNext  = sE;
                        mss[mss_max].dochet = offSet;
                        mss[mss_max].Len    = base+offSet;
                        mss[mss_max].pm     = 1; // +
                        mss_max++;
                        if ( mss_max>=st_mss_lim ) stat = 1;
                    }
                }
            }
            return stat;
        }
        unsigned char saveRender2(signed char d, signed char u, unsigned char maxn)
        {
            if ( mss_max>=st_mss_lim )   return 2;
            if ( (d<0) || (u>=maxn)  )   return 0;
            unsigned char stat = 0;
            {
                unsigned int base;
                  signed long offSet;
                unsigned char sB,   sE;
                unsigned int  sLen;
                unsigned long sTime;
                offSet = datTimeMassive[map[u  ]][0]-datTimeMassive[map[d  ]][1];
                if (offSet>0)
                {
                    base   =     vg::rs_Dat[map[u  ]]   -    vg::rs_Dat[map[d  ]];
                    speedSelect(d, u, maxn, offSet, &sB, &sE, &sLen, &sTime, 0);
                    offSet = ((unsigned long)offSet*sLen*10/sTime+5)/10;
                    if ( offSet<base )
                    {
                        mss[mss_max].napr   = 4;
                        mss[mss_max].fist   = d;
                        mss[mss_max].next   = u;
                        mss[mss_max].sFist  = sB;
                        mss[mss_max].sNext  = sE;
                        mss[mss_max].dochet = offSet;
                        mss[mss_max].Len    = base-offSet;
                        mss[mss_max].pm     = 2; // -
                        mss_max++;
                        if ( mss_max>=st_mss_lim ) stat = 1;
                    }
                }
            }
            return stat;
        }
        unsigned char saveRender4(signed char d, signed char u, unsigned char maxn)
        {
            if ( mss_max>=st_mss_lim )   return 2;
            if ( (d<0) || (u>=maxn)  )   return 0;
            unsigned char stat = 0;
            {
                unsigned int base;
                  signed long offSet;
                unsigned char sB,   sE;
                unsigned int  sLen;
                unsigned long sTime;
                offSet = datTimeMassive[map[d+1]][1]-datTimeMassive[map[u  ]][0];
                if (offSet>0)
                {
                    base   =     vg::rs_Dat[map[u  ]]   -    vg::rs_Dat[map[d+1]];
                    speedSelect(d+1, u, maxn, offSet, &sB, &sE, &sLen, &sTime, 1);
                    offSet = ((unsigned long)offSet*sLen*10/sTime+5)*10;
                    if ( offSet<base )
                    {
                        mss[mss_max].napr   = 6;
                        mss[mss_max].fist   = d+1;
                        mss[mss_max].next   = u;
                        mss[mss_max].sFist  = sB;
                        mss[mss_max].sNext  = sE;
                        mss[mss_max].dochet = offSet;
                        mss[mss_max].Len    = base+offSet;
                        mss[mss_max].pm     = 1; // +
                        mss_max++;
                        if ( mss_max>=st_mss_lim ) stat = 1;
                    }
                }
            }
            return stat;
        }
        unsigned char saveRender5(signed char d, signed char u, unsigned char maxn)
        {
            if ( mss_max>=st_mss_lim )   return 2;
            if ( (d<0) || (u>=maxn)  )   return 0;
            unsigned char stat = 0;
            {
                unsigned int base;
                  signed long offSet;
                unsigned char sB,   sE;
                unsigned int  sLen;
                unsigned long sTime;
                offSet = datTimeMassive[map[d  ]][1]-datTimeMassive[map[u  ]][0];
                if (offSet>0)
                {
                    base   =     vg::rs_Dat[map[u  ]]   -    vg::rs_Dat[map[d  ]];
                    speedSelect(d, u, maxn, offSet, &sB, &sE, &sLen, &sTime, 1);
                    offSet = ((unsigned long)offSet*sLen*10/sTime+5)/10;
                    if ( offSet<base )
                    {
                        mss[mss_max].napr   = 7;
                        mss[mss_max].fist   = d;
                        mss[mss_max].next   = u;
                        mss[mss_max].sFist  = sB;
                        mss[mss_max].sNext  = sE;
                        mss[mss_max].dochet = offSet;
                        mss[mss_max].Len    = base+offSet;
                        mss[mss_max].pm     = 1; // +
                        mss_max++;
                        if ( mss_max>=st_mss_lim ) stat = 1;
                    }
                }
            }
            return stat;
        }
        unsigned char saveRender6(signed char d, signed char u, unsigned char maxn)
        {
            if ( mss_max>=st_mss_lim )   return 2;
            if ( ((d-1)<0) || (u>=maxn)  )   return 0;
            unsigned char stat = 0;
            {
                unsigned int base;
                  signed long offSet;
                unsigned char sB,   sE;
                unsigned int  sLen;
                unsigned long sTime;
                offSet = datTimeMassive[map[u-1]][0]-datTimeMassive[map[d-1]][1];
                if (offSet>0)
                {
                    base   =     vg::rs_Dat[map[u-1]]   -    vg::rs_Dat[map[d-1]];
                    speedSelect(d-1, u-1, maxn, offSet, &sB, &sE, &sLen, &sTime, 0);
                    offSet = ((unsigned long)offSet*sLen*10/sTime+5)/10;
                    if ( offSet<base )
                    {
                        mss[mss_max].napr   = 8;
                        mss[mss_max].fist   = d-1;
                        mss[mss_max].next   = u-1;
                        mss[mss_max].sFist  = sB;
                        mss[mss_max].sNext  = sE;
                        mss[mss_max].dochet = offSet;
                        mss[mss_max].Len    = base-offSet;
                        mss[mss_max].pm     = 2; // -
                        mss_max++;
                        if ( mss_max>=st_mss_lim ) stat = 1;
                    }
                }
            }
            return stat;
        }
        unsigned char saveRender7(signed char d, signed char u, unsigned char maxn)
        {
            if ( mss_max>=st_mss_lim )   return 2;
            if ( (d<0) || (u>=maxn)  )   return 0;
            unsigned char stat = 0;
            {
                unsigned int base;
                  signed long offSet;
                unsigned char sB,   sE;
                unsigned int  sLen;
                unsigned long sTime;
                offSet = datTimeMassive[map[d+1]][1]-datTimeMassive[map[u  ]][0];
                if (offSet>0)
                {
                    base   =     vg::rs_Dat[map[u  ]]   -    vg::rs_Dat[map[d+1]];
                    speedSelect(d+1, u, maxn, offSet, &sB, &sE, &sLen, &sTime, 1);
                    offSet = ((unsigned long)offSet*sLen*10/sTime+5)/10;
                    if ( offSet<base )
                    {
                        mss[mss_max].napr   = 9;
                        mss[mss_max].fist   = d+1;
                        mss[mss_max].next   = u;
                        mss[mss_max].sFist  = sB;
                        mss[mss_max].sNext  = sE;
                        mss[mss_max].dochet = offSet;
                        mss[mss_max].Len    = base+offSet;
                        mss[mss_max].pm     = 1; // +
                        mss_max++;
                        if ( mss_max>=st_mss_lim ) stat = 1;
                    }
                }
            }
            return stat;
        }
        unsigned char saveRender8(signed char d, signed char u, unsigned char maxn)
        {
            if ( mss_max>=st_mss_lim )   return 2;
            if ( (d<0) || ((u+1)>=maxn)  )   return 0;
            unsigned char stat = 0;
            {
                unsigned int base;
                  signed long offSet;
                unsigned char sB,   sE;
                unsigned int  sLen;
                unsigned long sTime;
                offSet = datTimeMassive[map[u+1]][0]-datTimeMassive[map[d+1]][1];
                if (offSet>0)
                {
                    base   =     vg::rs_Dat[map[u+1]]   -    vg::rs_Dat[map[d+1]];
                    speedSelect(d+1, u+1, maxn, offSet, &sB, &sE, &sLen, &sTime, 0);
                    offSet = ((unsigned long)offSet*sLen*10/sTime+5)/10;
                    if ( offSet<base )
                    {
                        mss[mss_max].napr   = 10;
                        mss[mss_max].fist   = d+1;
                        mss[mss_max].next   = u+1;
                        mss[mss_max].sFist  = sB;
                        mss[mss_max].sNext  = sE;
                        mss[mss_max].dochet = offSet;
                        mss[mss_max].Len    = base-offSet;
                        mss[mss_max].pm     = 2; // -
                        mss_max++;
                        if ( mss_max>=st_mss_lim ) stat = 1;
                    }
                }
            }
            return stat;
        }
    // ===========================================================================
#define dlinBuf 8
    void IzmRenderMain()
    {
        
        signed char mapD[nDat];
        // reset map
        for(unsigned char i=0; i<nDat; i++)
        {
            mapD[i] = -1;
        }
        // drop no signal sensor
        unsigned char maxnD = 0;
        datErrTmp = 0;
        for(unsigned char i=0;i<nDat; i++)
        {
            if (datTimeMassive[i][0]>0)
            {
                mapD[maxnD] = i;
                maxnD++;
            }
            else
                datErrTmp |= 1<<i;
        }
        if (maxnD>3)
        {
            // ==========================================
            // find speed
            class TspeedF
            {
            public:
                unsigned int speed;
                unsigned char n;
                TspeedF()
                {
                    speed = 0;
                    n = 0;
                }
            };
            TspeedF speedF[dlinBuf];
            for (unsigned char f=0; f<(maxnD-1); f++)
            {
                speedObr = (signed long)ftUserTimer*(vg::rs_Dat[mapD[f+1]]-vg::rs_Dat[mapD[f]])/(datTimeMassive[mapD[f+1]][0]-datTimeMassive[mapD[f]][0]);
                if ( (speedObr<1000) || (speedObr>3000 ) )
                    continue;
                // okrugl
                speedObr = (speedObr+5)/10;
                speedObr = speedObr*10;
                for (unsigned char i=0; i<dlinBuf; i++)
                {
                    if ( (speedF[i].speed==0) || (speedF[i].speed==speedObr) )
                    {
                        speedF[i].speed = speedObr;
                        speedF[i].n++;
                        break;
                    }
                }
            }
            unsigned char speedN = 0;
            speedObr = 0;
            for (unsigned char i=0; i<dlinBuf; i++)
            {
                if (speedF[i].speed==0)
                    break;
                else
                {
                    speedObr = speedObr + (signed long)speedF[i].speed*speedF[i].n;
                    speedN = speedN + speedF[i].n;
                }
            }
            speedObr = speedObr / speedN;
            // ========================================
            // init map
            maxn = 0;
            for(unsigned char i=0; i<nDat; i++)
            {
                map[i] = -1;
            }
            // ==================================
            signed long speed1, speed2;
            unsigned int prc1, prc2;
            unsigned char strt;
            //check sensor 1
            for (unsigned char fist=0; ((fist<maxnD-1) && (maxn==0)); fist++)
            {
                for (unsigned char next=fist+1; ( ((next-fist)<5) && (next<maxnD-1) && (maxn==0)); next++)
                {
                    speed1 = (signed long)ftUserTimer*(vg::rs_Dat[mapD[next]]-vg::rs_Dat[mapD[fist]])/(datTimeMassive[mapD[next]][0]-datTimeMassive[mapD[fist]][0]);
                    prc1 = Procent(speedObr, speed1);
                    if (prc1<vg::prcPorog)
                    {
                        map[maxn] = mapD[fist];
                        maxn++;
                        map[maxn] = mapD[next];
                        maxn++;
                        strt = next+1;
                        break;
                    }
                }
            }
            // check 1,2 speed for 1 sensors
            for (unsigned char i=strt; i<maxnD; i++)
            {
                speed1 = (signed long)ftUserTimer*(vg::rs_Dat[ map[maxn-1]]-vg::rs_Dat[ map[maxn-2]])/(datTimeMassive[ map[maxn-1]][0]-datTimeMassive[ map[maxn-2]][0]);
                speed2 = (signed long)ftUserTimer*(vg::rs_Dat[mapD[i     ]]-vg::rs_Dat[mapD[i-1   ]])/(datTimeMassive[mapD[i     ]][0]-datTimeMassive[mapD[i-1   ]][0]);
                prc1 = Procent(speed1  , speed2);
                prc2 = Procent(speedObr, speed2);
                if ( (prc1<vg::prcPorog) && (prc2<vg::prcPorog) )
                {
                    map[maxn] = mapD[i];
                    maxn++;
                    continue;
                }
            }
            // ==========================
            // mark bad sensor
            prc2 = 0;
            for (unsigned char i=0; i<nDat; i++)
            {
                if (map[prc2]==i)
                    prc2++;
                else
                    datErrTmp |= 1<<i;
            }
            speedN = speedN+1;
        }
        // =========================================
        // ==========================================
        if (maxn<3)
        {   // all bad
            WarningEnabled();
            eventMassStep = InitWaitFreeSensors;
            lenRender=10;
            flNewLen = true;
        }
        else
        {   // find soulions
            mss_max = 0;
            //    write massive adding
            if ( datTimeMassive[map[0]][1]>=datTimeMassive[map[maxn-1]][0] )
            {   // big tube
                saveRenderB(0, maxn-1, maxn);
            }
            else
            {   // small tube
                for (unsigned char sd=0; sd<(maxn-1); sd++)
                {
                    // buff over
                    if ( mss_max>=st_mss_lim ) break;
                    for (unsigned char su=(sd+1); su<maxn; su++)
                    {
                        // buff over
                        if ( mss_max>=st_mss_lim ) break;
                        if ( (datTimeMassive[map[sd]][1]>datTimeMassive[map[su]][0])
                          && (su<(maxn-1))
                           )
                            continue;
                        if ( saveRender1(sd, su, maxn) ) break;
                        if ( saveRender2(sd, su, maxn) ) break;
                        if ( saveRender4(sd, su, maxn) ) break;
                        if ( saveRender5(sd, su, maxn) ) break;
                        if ( saveRender6(sd, su, maxn) ) break;
                        //if ( saveRender7(sd, su, maxn) ) break;
                        //if ( saveRender8(sd, su, maxn) ) break;
                        //break;
                    }
                }
            }
            // sort min offset 
            st_mss tmpSort;
//            unsigned char minN = mss_max-1;
            {
                bool flSort;
                // sort min offset 
                flSort = true;
                while (flSort)
                {
                    flSort = false;
                    for (unsigned char n=1; n<mss_max; n++)
                    {
                        if ( mss[n-1].dochet>mss[n].dochet )
                        {
                            tmpSort = mss[n-1];
                            mss[n-1] = mss[n];
                            mss[n] = tmpSort;
                            flSort = true;
                            break;
                        }
                    }
                }
                // sort min offset & min len for speed
                flSort = true;
//                signed int xxx;
//                xxx = vg::rs_Dat[map[mss[1].sNext]]-vg::rs_Dat[map[mss[1].sFist]];
//                xxx = vg::rs_Dat[map[mss[1].sFist]];
                while (flSort)
                {
                    flSort = false;
                    for (unsigned char n=1; n<mss_max; n++)
                    {
                        if ( (mss[n-1].dochet>(mss[n].dochet-50))
//                          && (mss[n-1].dochet>(mss[n].
                          && ( (vg::rs_Dat[map[mss[n-1].sNext]]-vg::rs_Dat[map[mss[n-1].sFist]])
                              > (vg::rs_Dat[map[mss[n  ].sNext]]-vg::rs_Dat[map[mss[n  ].sFist]]) )
                           )
                        {
                            tmpSort = mss[n-1];
                            mss[n-1] = mss[n];
                            mss[n] = tmpSort;
                            flSort = true;
                            break;
                        }
                    }
                }
                /*
                flSort = false;
                while (flSort)
                {
                    flSort = false;
                    for (unsigned char n=1;n<mss_max;n++)
                    {
                        signed long x = mss[n-1].dochet-mss[n].dochet;
                        if (x<0) x=-x;
                        if ( (x<50)&&(mss[n-1].sLen>mss[n].sLen) )
                        {
                            flSort = true;
                            tmpSort = mss[n-1];
                            mss[n-1] = mss[n];
                            mss[n] = tmpSort;
                            break;
                        }
                    }
                }
                */
                        /*
                        if ( (mss[n-1].dochet==mss[n].dochet)&&(mss[n-1].sLen==mss[n].sLen)&&(mss[n-1].modeRender>mss[n].modeRender) )
                        {
                            flSort = true;
                            tmpSort = mss[n-1];
                            mss[n-1] = mss[n];
                            mss[n] = tmpSort;
                            break;
                        }
                        */
            }
            tmpSort = mss[0];
            // render
            vg::teleMsg[0] = map[tmpSort.fist]  + '0';
            vg::teleMsg[1] = map[tmpSort.next]  + '0';
            vg::teleMsg[2] = '+';
            if ( (tmpSort.napr==4)
              || (tmpSort.napr==5)  
              || (tmpSort.napr==8)  
              || (tmpSort.napr==10)
                  )
            vg::teleMsg[2] = '-';
            vg::teleMsg[3] = map[tmpSort.sFist] + '0';
            vg::teleMsg[4] = map[tmpSort.sNext] + '0';
            lenRender = mss[0].Len;
            flNewLen = true;
            eventMassStep = InitOutDac;
        }
        // =========
        // send to rs debug info
        void SendDebug();
        SendDebug();
        flNewLen = true;
    }
    void SendDebug()
    {
        const unsigned char simLen = 6;
        unsigned char IdxLen;
        unsigned int fTik = ftUserTimer;
        const unsigned char massSendLen = 
              sizeof(unsigned char) // 0xe6
            + sizeof(unsigned char) // code
            + sizeof(unsigned char) // len
            + sizeof(unsigned int)  // 0x55aa
            + sizeof(datErrTmp)
//            + sizeof(lenRender)
            + simLen
            + 7                     // fist, next, napr, sFist, sNext, dochet
            + sizeof(vg::rs_Dat)
            + sizeof(datTimeMassive)
            + sizeof(fTik)          // f tik
            + sizeof(unsigned char) // crc
            ;
            unsigned char massSend[massSendLen];
            unsigned char massSendIdx = 0;
        massSend[massSendIdx++] = 0xe6;
        massSend[massSendIdx++] = 0x0d; // code
        IdxLen = massSendIdx;
        massSend[massSendIdx++] = 0x00; // len
        massSend[massSendIdx++] = 0xaa; // aa
        massSend[massSendIdx++] = 0x55; // 55
        massSend[massSendIdx++] = datErrTmp;
        // lenRender
        /*
        for (unsigned char b=0; b<sizeof(lenRender); b++)
        {
            massSend[massSendIdx++] = ((unsigned char *)&lenRender)[b]; 
        }
        */
        
        {
            unsigned char lIdx = massSendIdx + simLen;
            unsigned int xx = lenRender;
            unsigned char xxtmp;
            unsigned char xxi;
            for (xxi=0; xxi<simLen; xxi++)
            {
                if ( xxi==(simLen-3) )
                {
                    massSend[lIdx-(xxi+1)] = '.';
                }
                else
                {
                    xxtmp = xx % 10;
                    xx = xx / 10;
                    massSend[lIdx-(xxi+1)] = xxtmp + '0';
                }
                massSendIdx++;
            }
        }
        // fist, next, napr, sFist, sNext
        massSend[massSendIdx++] = map[mss[0].fist];
        massSend[massSendIdx++] = map[mss[0].next];
        massSend[massSendIdx++] =     mss[0].napr;
        massSend[massSendIdx++] = map[mss[0].sFist];
        massSend[massSendIdx++] = map[mss[0].sNext];
        // mss[minNum].dochet
        for (unsigned char b=0; b<sizeof(mss[0].dochet); b++)
        {
            massSend[massSendIdx++] = ((unsigned char *)&mss[0].dochet)[b]; 
        }
        // vg::rs_Dat
        for (unsigned char n=0; n<(sizeof(vg::rs_Dat)/sizeof(vg::rs_Dat[0])); n++)
        {
            unsigned int tt = vg::rs_Dat[n];
            for (unsigned char b=0; b<sizeof(tt); b++)
            {
                massSend[massSendIdx++] = ((unsigned char *)&tt)[b];
            }
        }
        // datTimeMassive
        for (unsigned char l=0; l<2; l++)
        {
            for (unsigned char n=0; n<(sizeof(datTimeMassive)/(sizeof(datTimeMassive[0][0])*2)); n++)
            {
                for (unsigned char b=0; b<sizeof(datTimeMassive[0][0]); b++)
                {
                    massSend[massSendIdx++] = ((unsigned char *)&datTimeMassive[n][l])[b];
                }
            }
        }
        // f tik
        for (unsigned char b=0; b<sizeof(fTik); b++)
        {
            massSend[massSendIdx++] = ((unsigned char *)&fTik)[b];
        }
        massSend[IdxLen] = massSendIdx+1;
        // crc
        massSend[massSendIdx] = crc8_buf(massSend, massSendIdx-1);
        massSendIdx++;
        // ============ SEND ================
        if (massSendIdx==massSendLen)
        {
            for (unsigned char i=0;i<massSendIdx; i++)
                PortForDebug::WriteByte(massSend[i]);
        }
        else
        {
            {
                unsigned char sss[] = "Error module send\r\n";
                unsigned char l = sizeof(sss);
                for (unsigned char i=0;i<l; i++)
                    PortForDebug::WriteByte(sss[i]);
            }
            {
                unsigned char sss[8];
                unsigned char *s = sss;
                *s = massSendIdx / 16;
                if ( *s>9 )
                    *s = *s-10+'A';
                else
                    *s = *s+'0';
                s++;
                *s = massSendIdx % 16;
                if ( *s>9 )
                    *s = *s-10+'A';
                else
                    *s = *s+'0';
                s++;
                *s = '!';
                s++;
                *s = '=';
                s++;
                *s = massSendLen / 16;
                if ( *s>9 )
                    *s = *s-10+'A';
                else
                    *s = *s+'0';
                s++;
                *s = massSendLen % 16;
                if ( *s>9 )
                    *s = *s-10+'A';
                else
                    *s = *s+'0';
                s++;
                *s = '\r';
                s++;
                *s = '\n';
                unsigned char l = sizeof(sss);
                for (unsigned char i=0;i<l; i++)
                    PortForDebug::WriteByte(sss[i]);
            }
        }
    }
    // =========================================================================
    void InitOutDacMain()
    {
        // настройка и выдача нуля по токову интерфесу
        ad420_OutDac(lenRender);
        outDacCount = outDacCountMax;
        eventMassStep = WaitEndOutDac;
    }
    void WaitEndOutDacTimer()
    {
        if (outDacCount>0)
        {
            outDacCount--;
        }
        else
        {
            ad420_OutDac(0);
            eventMassStep = InitWaitFreeSensors;
        }
    }
    // =========================================================================
    // ==========================================================================
    // =========================================================================
    // =========================================================================
    unsigned char CheckFreeSensors()
    {
        unsigned char statFree = 1;
        for (unsigned char n=0; n<nDat; n++)
        {   // summ status sensors : 0 - hot, 1 - free
            statFree &= dat[n]->level();
        }
        return statFree;
    }
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
        dat[6] = new tc_ports1<izmLen_DatType>('f', 6, izmLen_DatPorog, Event1, 6);
        dat[7] = new tc_ports1<izmLen_DatType>('f', 7, izmLen_DatPorog, Event1, 7);
        eventMassStep = 0;
        // настройка и выдача нуля по токову интерфесу
        ad420_Init();
        ad420_SwapWord(0);
    }
    // =========================================================================
}
