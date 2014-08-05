
#ifndef OutDebug__h
#define OutDebug__h

void SendOutDebug(unsigned int  lenSlitok,
                  unsigned char workSensor,
#if (izmLenTimeOut<65536)
                  unsigned int  timer_mass[2][6]
#else
                  unsigned long timer_mass[2][6]
#endif
                      );

#endif
