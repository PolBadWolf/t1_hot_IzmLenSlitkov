
#include "main.h"
#include "OutDebug.h"

void SendOutDebug(unsigned int  lenSlitok,
                  unsigned char workSensor,
#if (izmLenTimeOut<65536)
                  unsigned int  timer_mass[2][6]
#define  timerMassLen  2
#else
                  unsigned long timer_mass[2][6]
#define  timerMassLen  4
#endif
                      )
{
    // len massive
    // lenSlitok        2
    // workSensor       1
    // timer_mass       2*6*timerMassLen
    // lensensor_mass   6*sizeof(unsigned int)
#define lenMassive (2+1+(2*6*timerMassLen)+(6*2))
    unsigned char massiveSend[lenMassive+5];
    unsigned char indx = 0;
    massiveSend[indx++] = 0xe6;
    massiveSend[indx++] = 0x0b;
    massiveSend[indx++] = 0x00; // len
    massiveSend[indx++] = 0x00; // zero
    massiveSend[indx++] = ((unsigned char*)&lenSlitok)[0];
    massiveSend[indx++] = ((unsigned char*)&lenSlitok)[1];
    massiveSend[indx++] = workSensor;
    // time
    for(unsigned char level=0; level<2; level++)
    {
        for(unsigned char sensor=0; sensor<6; sensor++)
        {
            for(unsigned char bb=0; bb<timerMassLen; bb++)
            {
                massiveSend[indx++] = ((unsigned char *)&timer_mass[level][sensor])[bb];
            }
        }
    }
    // len
    for (unsigned char sensor=0; sensor<6; sensor++)
    {
        unsigned int dd = vg::rs_Dat[sensor];
        for(unsigned char bb=0; bb<timerMassLen; bb++)
        {
            massiveSend[indx++] = ((unsigned char *)&dd)[bb];
        }
    }
    massiveSend[2   ] = indx;
    massiveSend[indx] = crc8_buf(massiveSend, indx);
    indx++;
    // send
    for (unsigned char n=0; n<indx; n++)
        PortForDebug::WriteByte(massiveSend[n]);
}
