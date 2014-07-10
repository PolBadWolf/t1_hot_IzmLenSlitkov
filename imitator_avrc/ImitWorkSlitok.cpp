

#include "main.h"
#include "ImitWorkSlitok.h"

unsigned char imws_fl = 0;
  signed int  imws_SlitokB = -1000;
  signed int  imws_SlitokE = -2000;
  signed int  imws_SlitokLen = 2500;
unsigned int  imws_SlitokSpeed = 1000;
unsigned long imws_time = 0;
  signed int  imws_Dat[8] = {0, 2000, 3000, 30000, 30000, 30000, 30000, 30000};

void ImitWorkCycl()
{
    if ( key_status() ) 
    {
      unsigned char key;
      key_read(&key);
      if (key==1) //stop
      {
        imws_fl = 0;
      }
      if (key==2) //в начало
      {
        imws_fl = 0;
        imws_time = 0;
        imws_SlitokB = -1000;
        imws_SlitokLen = vg::rs_slitok_len;
        imws_SlitokSpeed = vg::rs_slitok_speed;
        for (unsigned char i=0;i<8;i++)
          imws_Dat[i] = vg::rs_Dat[i];
      }
      if (key==3) //пуск
      {
        imws_fl = 1;
      }
    }
  scr->dig_uz(c_stolbcov+8, 6, imws_time);
  scr->dig_sz(0, 5, imws_SlitokB);
  scr->dig_sz(6, 5, imws_SlitokE);
}

void ImitWorkTimer()
{
  if (imws_fl>0)
  {
    imws_time++;
    unsigned long temp;
    temp = (unsigned long)imws_time*imws_SlitokSpeed/1000;
    imws_SlitokB = temp - 1000;
    if (imws_SlitokB>25000)
    {
      imws_fl = 0;
    }
  }
  imws_SlitokE = imws_SlitokB - imws_SlitokLen;
  for (unsigned char i=0;i<8;i++)
  {
    if ( (imws_SlitokB>imws_Dat[i]) && (imws_SlitokE<imws_Dat[i]) )
      og::ImitDat[i]->pout(0);
    else
      og::ImitDat[i]->pout(1);
  }
}

