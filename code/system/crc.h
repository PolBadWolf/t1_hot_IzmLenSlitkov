
#ifndef crc__h
#define crc__h
#include "Configure.h"
#ifdef CRC8
// ������� �� � ������
unsigned char crc8_buf( unsigned char *buf, int len );
// ������ ������ �����
void crc8_byte( unsigned char *crc, unsigned char data );

#endif
#endif

