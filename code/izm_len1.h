
#ifndef izm_len1__h
#define izm_len1__h

#define izmLen_TimeOut 80000
#define izmLen_DatPorog 60

#if   (izmLen_DatPorog<256)
#define izmLen_DatType unsigned char
#elif (izmLen_DatPorog<65536)
#define izmLen_DatType unsigned int
#else
#define izmLen_DatType unsigned long
#endif

namespace ns_izmlen1
{
    // tmp null function
    //ok
    unsigned char ReadStep();
    // ok
    unsigned char SensorNewDate();
    void SensorNewDateReset();
    // ok
    extern bool flNewLen;
    // ok
    extern unsigned int NewLen;
    //
    extern unsigned char ErrWorkDat;
    void init();
    void for_timer();
    void main();
    // massive sensors
    extern tc_ports1<unsigned char> *dat[];
    extern unsigned long datTimeMassive[8][2];
}

#define ns_izmlen ns_izmlen1

#endif
