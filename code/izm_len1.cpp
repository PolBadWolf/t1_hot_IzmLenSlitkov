
#include "main.h"

namespace ns_izmlen1
{
    // tmp null function
    unsigned char ReadStep(){return 3;}
    bool SensorNewDate = true;
    bool flNewLen = true;
    unsigned int NewLen = true;
    unsigned char ErrWorkDat = 0;
    // massive sensors
    tc_ports1<izmLen_DatType> *dat[6];
    void init()
    {
    }
    void for_timer()
    {
    }
    void main()
    {
    }
}
