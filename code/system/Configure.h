
#ifndef configure__h
#define configure__h

// частота чипа
#define  C_Fosc  16000000

// часы
// #define CLOCK
//          работка кнопок
#define KEY4
//          работа LCD индикатора
#define LCD
//          пользовательские функции на таймер диспля ( около 1 кгц )
#define USERTIMERDISPLAY
//          пользовательский таймер
#define USERTIMERUSER
//          работа с меню
#define MENU
//          работа 485 интерфейса
// #define RS485
//          работа 232 интерфейса
#define RS232
//          подсчет к.с.
#define CRC8

// переопределение интерфейса 485 под говорящие имя
#ifdef RS485
#define PcPort ns_rs485
#endif

// переопределение интерфейса 232 под говорящие имя
#ifdef RS232
#define PortForDebug ns_rs232
#endif

#endif

