
#include "../main.h"

#ifdef KEY4
#include "key4.h"

class tc_key4
{
private:
  #define count_filtr_max 20
  unsigned char count_filtr;
  unsigned char fl_push;
  static __flash unsigned char key_tab[16];
  #define key_buffer_dlina 32
  unsigned char key_buffer[key_buffer_dlina];
  unsigned char key_buffer_rd;
  unsigned char key_buffer_wr;
  unsigned char key_buffer_adr(unsigned char adr);
  #define count_delay_max 700
  unsigned int count_delay;
  #define count_repeat_max 50
  unsigned char count_repeat;
  unsigned char cs_write;
  unsigned char cs_read;
  unsigned char cs_status;
  void read_pin(void);
  void registry_key(void);
  void f_write_key(unsigned char key);
public:
  unsigned char key;
  tc_ports *key_pin[4];
  unsigned char pin;
  unsigned char pin_temp;
  tc_key4();
  void for_timer(void);
  unsigned char f_read_key(unsigned char *key);
  unsigned char f_status(void);
};

tc_key4::tc_key4()
{
  key_pin[0] = new tc_ports('g', 0, 0, 0, 3);
  key_pin[1] = new tc_ports('g', 1, 0, 0, 2);
  key_pin[2] = new tc_ports('g', 2, 0, 0, 1);
  key_pin[3] = new tc_ports('g', 3, 0, 0, 0);
  count_filtr = 0;
  fl_push = 0;
  key_buffer_rd = key_buffer_wr = 0;
}
void tc_key4::read_pin(void)
{
  pin_temp = key_pin[0]->pin() |
             key_pin[1]->pin() |
             key_pin[2]->pin() |
             key_pin[3]->pin();
  if ( pin_temp!=pin )
  {
    if (count_filtr<count_filtr_max) count_filtr++;
    if (count_filtr>=count_filtr_max)
    {
      pin = pin_temp;
      count_filtr = 0;
    }
  }
  else
  {
    if (count_filtr>0) count_filtr--;
  }
}
void tc_key4::for_timer(void)
{
  read_pin();
  registry_key();
}
void tc_key4::registry_key(void)
{
  key = key_tab[pin];
  if (fl_push)
  {
    if (pin!=15)
    {
      if (count_delay<count_delay_max) count_delay++;
      if (count_delay>=count_delay_max)
      {
        if (count_repeat<count_repeat_max) count_repeat++;
        else
        {
          count_repeat = 0;
          f_write_key(key);
        }
      }
    }
    else
    {
      count_delay = 0;
      count_repeat = 0;
      fl_push = 0;
    }
  }
  else
  {
    if (pin!=15)
    {
      fl_push = 1;
      f_write_key(key);
      count_delay = 0;
      count_repeat = 0;
    }
  }
}
__flash unsigned char tc_key4::key_tab[] = {
  1,2,1,3,
  1,2,1,4,
  1,2,1,3,
  1,2,1,0 };
unsigned char tc_key4::key_buffer_adr(unsigned char adr)
{
  #if ( (key_buffer_dlina==2) | (key_buffer_dlina==4) | (key_buffer_dlina==8) | (key_buffer_dlina==16) | (key_buffer_dlina==32) | (key_buffer_dlina==64) | (key_buffer_dlina==128) )
  adr = adr & (key_buffer_dlina-1);
  #else
  while(adr>=key_buffer_dlina) adr -= key_buffer_dlina;
  #endif
  return adr;
}
void tc_key4::f_write_key(unsigned char key)
{
  unsigned char adr_temp;
  cs_write = __save_interrupt();
  __disable_interrupt();
  adr_temp = key_buffer_adr(key_buffer_wr+1);
  if ( adr_temp!=key_buffer_rd )
  {
    key_buffer[adr_temp] = key;
    key_buffer_wr = adr_temp;
  }
  __restore_interrupt(cs_write);
}
unsigned char tc_key4::f_read_key(unsigned char *key)
{
  unsigned char st = 0;
  cs_read = __save_interrupt();
  __disable_interrupt();
  if (key_buffer_wr!=key_buffer_rd)
  {
    st = 1;
    key_buffer_rd = key_buffer_adr(key_buffer_rd+1);
    *key = key_buffer[key_buffer_rd];
  }
  __restore_interrupt(cs_read);
  return st;
}
unsigned char tc_key4::f_status(void)
{
  unsigned char n;
  cs_status = __save_interrupt();
  __disable_interrupt();
  if (key_buffer_rd>key_buffer_wr) n = key_buffer_dlina-(key_buffer_rd-key_buffer_wr);
  else n = key_buffer_wr-key_buffer_rd;
  __restore_interrupt(cs_status);
  return n;
}





tc_key4 *o_key4 = new tc_key4;

unsigned char inkey(void)
{
  return (o_key4->key==0)?255:o_key4->key;
}
unsigned char key_status(void)
{
  return o_key4->f_status();
}
unsigned char key_read(unsigned char *key)
{
  return o_key4->f_read_key(key);
}

void key_int(void)
{
  o_key4->for_timer();
}
#endif
