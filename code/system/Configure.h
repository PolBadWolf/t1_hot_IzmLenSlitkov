
#ifndef configure__h
#define configure__h

// ������� ����
#define  C_Fosc  16000000

// ����
// #define CLOCK
//          ������� ������
#define KEY4
//          ������ LCD ����������
#define LCD
//          ���������������� ������� �� ������ ������ ( ����� 1 ��� )
#define USERTIMERDISPLAY
//          ���������������� ������
#define USERTIMERUSER
//          ������ � ����
#define MENU
//          ������ 485 ����������
// #define RS485
//          ������ 232 ����������
#define RS232
//          ������� �.�.
#define CRC8

// ��������������� ���������� 485 ��� ��������� ���
#ifdef RS485
#define PcPort ns_rs485
#endif

// ��������������� ���������� 232 ��� ��������� ���
#ifdef RS232
#define PortForDebug ns_rs232
#endif

#endif

