
#include "main.h"

#include "dac_ad420.h"

#define ad420DDR_SpiMOSI DDRB_DDB6
#define ad420DDR_SpiMOSO DDRB_DDB6
#define ad420DDR_SpiSCK  DDRB_DDB7
#define ad420DDR_SpiSS   DDRD_DDD4
#define ad420DDR_SpiCLR  DDRD_DDD5

#define ad420_SpiMOSI PORTB_PORTB6
#define ad420_SpiMOSO PORTB_PORTB6
#define ad420_SpiSCK  PORTB_PORTB7
#define ad420_SpiSS   PORTD_PORTD4
#define ad420_SpiCLR  PORTD_PORTD5

void ad420_Init()
{
  ad420_SpiMOSI = 1;
  ad420_SpiMOSO = 1;
  ad420_SpiSCK  = 1;
  ad420_SpiSS   = 1;
  ad420_SpiCLR  = 1;
  ad420DDR_SpiMOSI = 1;
  ad420DDR_SpiMOSO = 1;
  ad420DDR_SpiSCK  = 1;
  ad420DDR_SpiSS   = 1;
  ad420DDR_SpiCLR  = 1;
  _delay_ms(10);
  ad420_SpiCLR  = 0;
}

unsigned int ad420_SwapWord(unsigned int cmd)
{
  unsigned char i;
  unsigned int recv = 0;
  DDRB_DDB6 = 1;
  DDRB_DDB7 = 1;
  DDRD_DDD4 = 1;
  DDRD_DDD5 = 1;
  ad420_SpiSCK = 0;
  _delay_us(2);
  ad420_SpiSS = 0;
  _delay_us(2);
  for (i=0;i<16;i++)
  {
    ad420_SpiMOSI = (cmd&0x8000)?1:0;
    _delay_us(2);
    ad420_SpiSCK = 1;
    _delay_us(2);
    recv <<= 1;
    if (ad420_SpiMOSO) recv |= 1;
    ad420_SpiSCK = 0;
    cmd <<= 1;
    _delay_us(2);
  }
  ad420_SpiSS = 1;
  _delay_us(2);
  return recv;
}

void ad420_OutDac(unsigned int mmetr)
{
    ad420_Init();
    ad420_SwapWord((unsigned long)mmetr*40959/10000);
}

