
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
#define izmSensorsTimeOutMax 5000
    unsigned int izmSensorsTimeOut = izmSensorsTimeOutMax;
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
#define outDacCountMax 5000
    void InitOutDacMain();
    void WaitEndOutDacTimer();
    // =========================================================================
    // =========================================================================
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
                izmSensorsTimeOut = izmSensorsTimeOutMax;
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
        waitFreeSensorsTime = waitFreeSensorsTimeOut;
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
                eventMassStep = InitWaitFreeSensors;
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
    // =========================================================================
            struct st_mss
            {
                unsigned char napr;
                unsigned char fist;
                unsigned char next;
                unsigned long dochet;
            };
        st_mss mss[36];
        unsigned char minNum;
    void IzmRenderMain()
    {
        // max
        signed char maxn = 0;
        signed char map[nDat]; // nDat - all sensors (const)
        // ========================
        minNum = 255;
        unsigned char mss_max = 0;
        // ========================
        // reset map
        for(unsigned char i=0; i<nDat; i++)
        {
            map[i] = -1;
        }
        // init map
        datErrTmp = 0;
        for(unsigned char i=0; i<nDat; i++)
        {
            if (datTimeMassive[i][0]>0)
            {
                map[maxn] = i;
                maxn++;
            }
            else
            {
                datErrTmp |= 1<<i;
            }
        }
        // 
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
            // write massive adding
            for (unsigned char sd=0; sd<(maxn-2); sd++)
            {
                for (unsigned char su=(sd+1); su<maxn; su++)
                {
                    if ( datTimeMassive[map[sd]][1]<=datTimeMassive[map[su]][0] )
                    {   // find zone
                        // sd : + : datTimeMassive[map[sd]][1]-datTimeMassive[map[su-1]][0]
                        mss[mss_max].napr = 1;
                        mss[mss_max].fist = sd;
                        mss[mss_max].next = su-1;
                        mss[mss_max].dochet = datTimeMassive[map[sd]][1]-datTimeMassive[map[su-1]][0];
                        mss_max++;
                        // sd : - : datTimeMassive[map[su]][0]-datTimeMassive[map[sd]][1]
                        mss[mss_max].napr = 2;
                        mss[mss_max].fist = sd;
                        mss[mss_max].next = su;
                        mss[mss_max].dochet = datTimeMassive[map[su]][0]-datTimeMassive[map[sd]][1];
                        mss_max++;
                    }
                    else
                    {
                        if ( su==(maxn-1) )
                        {   // end sensor
                            // sd : + : datTimeMassive[map[sd]][1]-datTimeMassive[map[su]][0]
                            mss[mss_max].napr = 0;
                            mss[mss_max].fist = sd;
                            mss[mss_max].next = su;
                            mss[mss_max].dochet = datTimeMassive[map[sd]][1]-datTimeMassive[map[su]][0];
                            mss_max++;
                        }
                        else
                        {
                            continue;
                        }
                    }
                }
            }
            // select min adding
            unsigned long minTime;
            minTime = mss[(mss_max-1)].dochet;
            minNum  = (mss_max-1);
            unsigned long base, speedLen, speedLen1, speedTime;
            for (signed char i=(mss_max-1); i>=0; i--)
            {
                if ( minTime>mss[i].dochet )
                {
                    minTime = mss[i].dochet;
                    minNum = i;
                }
            }
            // render
            base = vg::rs_Dat[map[mss[minNum].next]]-vg::rs_Dat[map[mss[minNum].fist]];
            vg::teleMsg[0] = map[mss[minNum].fist] + '0';
            vg::teleMsg[1] = map[mss[minNum].next] + '0';
//                            mss[mss_max].dochetNext = map[sd];
            if (mss[minNum].napr==0)
            {   // big tube
                speedLen  =     vg::rs_Dat[map[mss[minNum].next]]    -     vg::rs_Dat[map[mss[minNum].next-1]];
                speedLen1 =     vg::rs_Dat[map[mss[minNum].fist+1]]  -     vg::rs_Dat[map[mss[minNum].fist]];
                if ( speedLen>speedLen1)
                {
                    speedLen = speedLen1;
                    speedTime = datTimeMassive[map[mss[minNum].fist+1]][0] - datTimeMassive[map[mss[minNum].fist]][0];
                    vg::teleMsg[3] = map[mss[minNum].fist]   + '0';
                    vg::teleMsg[4] = map[mss[minNum].fist+1] + '0';
                }
                else
                {
                    speedTime = datTimeMassive[map[mss[minNum].next]][0] - datTimeMassive[map[mss[minNum].next-1]][0];
                    vg::teleMsg[3] = map[mss[minNum].next-1] + '0';
                    vg::teleMsg[4] = map[mss[minNum].next]   + '0';
                }
                lenRender = base + speedLen*mss[minNum].dochet/speedTime;
                flNewLen = true;
                vg::teleMsg[2] = '+';
            }
            if (mss[minNum].napr==1)
            {   // normal tube doschit plus
                speedLen  =     vg::rs_Dat[map[mss[minNum].next+1]]  -     vg::rs_Dat[map[mss[minNum].next]];
                speedLen1 =     vg::rs_Dat[map[mss[minNum].fist+1]]  -     vg::rs_Dat[map[mss[minNum].fist]];
                if ( speedLen>speedLen1)
                {
                    speedLen = speedLen1;
                    speedTime = datTimeMassive[map[mss[minNum].fist+1]][0] - datTimeMassive[map[mss[minNum].fist]][0];
                    vg::teleMsg[3] = map[mss[minNum].fist]   + '0';
                    vg::teleMsg[4] = map[mss[minNum].fist+1] + '0';
                }
                else
                {
                    speedTime = datTimeMassive[map[mss[minNum].next+1]][0] - datTimeMassive[map[mss[minNum].next]][0];
                    vg::teleMsg[3] = map[mss[minNum].next]   + '0';
                    vg::teleMsg[4] = map[mss[minNum].next+1] + '0';
                }
                lenRender = base + speedLen*mss[minNum].dochet/speedTime;
                flNewLen = true;
                vg::teleMsg[2] = '+';
            }
            if (mss[minNum].napr==2)
            {   // normal tube doschit minus
                speedLen  =     vg::rs_Dat[map[mss[minNum].next]]    -     vg::rs_Dat[map[mss[minNum].next-1]];
                speedLen1 =     vg::rs_Dat[map[mss[minNum].fist+1]]  -     vg::rs_Dat[map[mss[minNum].fist]];
                if ( speedLen>speedLen1)
                {
                    speedLen = speedLen1;
                    speedTime = datTimeMassive[map[mss[minNum].fist+1]][0] - datTimeMassive[map[mss[minNum].fist]][0];
                    vg::teleMsg[3] = map[mss[minNum].fist]   + '0';
                    vg::teleMsg[4] = map[mss[minNum].fist+1] + '0';
                }
                else
                {
                    speedTime = datTimeMassive[map[mss[minNum].next]][0] - datTimeMassive[map[mss[minNum].next-1]][0];
                    vg::teleMsg[3] = map[mss[minNum].next-1] + '0';
                    vg::teleMsg[4] = map[mss[minNum].next]   + '0';
                }
                lenRender = base - speedLen*mss[minNum].dochet/speedTime;
                flNewLen = true;
                vg::teleMsg[2] = '-';
            }
            // === END RENDER - OUT : lenRender=================================
            vg::teleMsg[5] = 0;
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
        const unsigned char massSendLen = 
              sizeof(unsigned char) // 0xe6
            + sizeof(unsigned char) // code
            + sizeof(unsigned char) // len
            + sizeof(unsigned int)  // 0x55aa
            + sizeof(datErrTmp)
//            + sizeof(lenRender)
            + simLen
            + sizeof(vg::rs_Dat)
            + sizeof(datTimeMassive)
            + sizeof(unsigned int)  // 0xaa55
            + sizeof(minNum)        // cur trio
            + sizeof(st_mss)        // cur trio
            + sizeof(unsigned char) // crc
            ;
            unsigned char massSend[massSendLen];
            unsigned char massSendIdx = 0;
        massSend[massSendIdx++] = 0xe6;
        massSend[massSendIdx++] = 0x0c; // code
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
        massSend[massSendIdx++] = 0x55; // 55
        massSend[massSendIdx++] = 0xaa; // aa
        // minNum
        for (unsigned char b=0; b<sizeof(minNum); b++)
        {
            massSend[massSendIdx++] = ((unsigned char *)&minNum)[b]; 
        }
        // mss[minNum].napr
        for (unsigned char b=0; b<sizeof(mss[minNum].napr); b++)
        {
            massSend[massSendIdx++] = ((unsigned char *)&mss[minNum].napr)[b]; 
        }
        // mss[minNum].fist
        for (unsigned char b=0; b<sizeof(mss[minNum].fist); b++)
        {
            massSend[massSendIdx++] = ((unsigned char *)&mss[minNum].fist)[b]; 
        }
        // mss[minNum].next
        for (unsigned char b=0; b<sizeof(mss[minNum].next); b++)
        {
            massSend[massSendIdx++] = ((unsigned char *)&mss[minNum].next)[b]; 
        }
        // mss[minNum].dochet
        for (unsigned char b=0; b<sizeof(mss[minNum].dochet); b++)
        {
            massSend[massSendIdx++] = ((unsigned char *)&mss[minNum].dochet)[b]; 
        }
        // crc
        massSend[massSendIdx] = crc8_buf(massSend, massSendIdx);
        massSendIdx++;
        massSend[IdxLen] = massSendIdx;
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
        ad420_Init();
        ad420_SwapWord(lenRender*4);
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
            ad420_Init();
            ad420_SwapWord(0);
            eventMassStep = InitWaitFreeSensors;
        }
    }
    // =========================================================================
    // =========================================================================
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
