
#include "rs485.h"
#include "../main.h"

#ifdef RS485
//====================================
#include "rs485.h"
//===
//------------------------------------
// скорость
#define rs_baud 9600
//------------------------------------
// pariry : disable, even, odd
#define rs_parity_disable
//------------------------------------
// количество бит 5, 6, 7, 8
#define rs_bits_8
//------------------------------------
#define RS485_R  PORTD_PORTD1 = 0  // прием
#define RS485_T  PORTD_PORTD1 = 1  // передача
#define RS485_S  PORTD_PORTD1      // статус
#define RS485_D  DDRD_DDD1   = 1  // настройка

// UART Buffer Defines
// 1,2,4,8,16,32,64,128 or 256 bytes
#define UART_RX_BUFFER_SIZE 64
#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1 )
// 1,2,4,8,16,32,64,128 or 256 bytes
#define UART_TX_BUFFER_SIZE 64
#define UART_TX_BUFFER_MASK ( UART_TX_BUFFER_SIZE - 1 )
#if (UART_RX_BUFFER_SIZE & UART_RX_BUFFER_MASK)
#error RS485 RX buffer size is not a power of 2
#endif
#if (UART_TX_BUFFER_SIZE & UART_TX_BUFFER_MASK)
#error RS485 TX buffer size is not a power of 2
#endif

namespace ns_rs485
{
  unsigned char RxBuf[UART_RX_BUFFER_SIZE];
  unsigned char RxHead;
  unsigned char RxTail;
  unsigned char TxBuf[UART_TX_BUFFER_SIZE];
  unsigned char TxHead;
  unsigned char TxTail;
  unsigned char RxLen;       // заполнение буффера
  unsigned char RxOverFlow;  // переполнение при приеме
  unsigned char TxLen;       // заполнение буффера
  unsigned char TxOverFlow;  // переполнение при передаче
  unsigned char fl_485_tx=0;
  //----------
  void init(void)
  {
    //unsigned char x;
    unsigned int baud;
    // For mega128
    baud = (unsigned int)( (unsigned long)C_Fosc/((unsigned long)rs_baud*16) )-1;
    UBRR1H = (unsigned char)(baud>>8);
    UBRR1L = (unsigned char)baud;
    // Parity
    #define parity_error
    UCSR1C = 0;
    #ifdef rs_parity_disable
    UCSR1C_UPM10 = 0;
    UCSR1C_UPM11 = 0;
    #undef parity_error
    #endif
    #ifdef rs_parity_even
    UCSR1C_UPM10 = 0;
    UCSR1C_UPM11 = 1;
    #undef parity_error
    #endif
    #ifdef rs_parity_odd
    UCSR1C_UPM10 = 1;
    UCSR1C_UPM11 = 1;
    #undef parity_error
    #endif
    #ifdef parity_error
    #error RS485 Error define parity : only disable, even or odd
    #endif
    // Bits
    #define bits_error
    #ifdef rs_bits_5
    UCSR1C_UCSZ10 = 0;
    UCSR1C_UCSZ11 = 0;
    UCSR1B_UCSZ12 = 0;
    #undef  bits_error
    #endif
    #ifdef rs_bits_6
    UCSR1C_UCSZ10 = 1;
    UCSR1C_UCSZ11 = 0;
    UCSR1B_UCSZ12 = 0;
    #undef  bits_error
    #endif
    #ifdef rs_bits_7
    UCSR1C_UCSZ10 = 0;
    UCSR1C_UCSZ11 = 1;
    UCSR1B_UCSZ12 = 0;
    #undef  bits_error
    #endif
    #ifdef rs_bits_8
    UCSR1C_UCSZ10 = 1;
    UCSR1C_UCSZ11 = 1;
    UCSR1B_UCSZ12 = 0;
    #undef  bits_error
    #endif
    #ifdef bits_error
    #error RS485 error define bits, only 5, 6, 7, 8
    #endif
    // 1 стоп бит
    UCSR1C_USBS1 = 0;
    RS485_D;
    // начальные значения
    RxHead =  RxTail = TxHead = TxTail = RxLen = RxOverFlow = TxLen = TxOverFlow = 0;
    // включить прием и включить прерывание на прием
    UCSR1B = 0;
    UCSR1B_RXCIE1 = 1;
    UCSR1B_RXEN1  = 1;
    UCSR1B_TXEN1  = 1;
    UCSR1B_TXCIE1 = 1;
  };
  //=======================================
  #pragma vector = USART1_RXC_vect // Прерывание приема от RS485
  __interrupt void UART485_RX_interrupt(void)
  {
    unsigned char data;
    unsigned char tmphead;
    // read the received data
    #ifdef rs_bits_5
    data = UDR1 & 0x1f;
    #endif
    #ifdef rs_bits_6
    data = UDR1 & 0x3f;
    #endif
    #ifdef rs_bits_7
    data = UDR1 & 0x7f;
    #endif
    #ifdef rs_bits_8
    data = UDR1;
    #endif
    // calculate buffer index
    tmphead = (RxHead + 1) & UART_RX_BUFFER_MASK;
    if(tmphead == RxTail)
    {
      // ERROR! Receive buffer overflow
      RxOverFlow = 1;
    }
    else
    {
      RxOverFlow = 0;
      RxHead = tmphead; // store new index
      RxBuf[RxHead] = data; // store received data in buffer
      RxLen++;
    }
  }
  //=======================================
  #pragma vector = USART1_UDRE_vect // Прерывание передачи от RS485
  __interrupt void UART485_TX_interrupt(void)
  {
    unsigned char tmptail;
    // check if all data is transmitted
    if(TxHead != TxTail)
    {
      // calculate buffer index
      tmptail = (TxTail + 1) & UART_TX_BUFFER_MASK;
      TxTail = tmptail; // store new index
      UCSR1B_TXB81 = 1;
      UDR1 = TxBuf[TxTail]; // start transmition
      TxLen--;
    }
    else
    {
      UCSR1B_UDRIE1 = 0; // disable UDRE interrupt
      fl_485_tx = 1;
      TxLen = 0;
    }
  }
  //=======================================
  #pragma vector = USART1_TXC_vect // Прерывание передачи от RS485
  __interrupt void UART2_TX_END(void)
  {
    if(fl_485_tx==1)
    {
      RS485_R;        // переключение на прием
      _delay_us(1);   // 1 мк. сек.
      fl_485_tx=0;
    }
  }
  //=======================================
  unsigned char ReadByte(unsigned char *Byte)
  {
    unsigned char tmptail;
    unsigned char stat;
    unsigned char cs;
    cs = __save_interrupt();
    __disable_interrupt();
    if (RxHead==RxTail)
    {
      RxLen = 0;
      stat = 0;                         // wait for incomming data
    }
    else
    {
      // calculate buffer index
      tmptail = (RxTail + 1) & UART_RX_BUFFER_MASK;
      RxTail = tmptail;           // store new index
      *Byte = RxBuf[tmptail];     // return data
      RxLen--;
      stat = 1;
    }
    __restore_interrupt(cs);
    return stat;
  }
  //=======================================
  unsigned char WriteByte(unsigned char data)
  {
    unsigned char tmphead;              // calculate buffer index
    unsigned char stat;
    unsigned char cs;
    cs = __save_interrupt();
    __disable_interrupt();
    tmphead = (TxHead + 1) & UART_TX_BUFFER_MASK; 
    if (tmphead == TxTail)
    {
      TxOverFlow = 1;
      stat = 0;                       // There is no free place in the buffer
    }
    else
    {
      if ( !RS485_S )
      {
        RS485_T;                // send
        _delay_us(1);           // 1 мк. сек.
      }
      TxBuf[tmphead] = data;    // store data in buffer
      TxHead = tmphead;         // store new index
      UCSR1B_UDRIE1 = 1;        // enable UDRE interrupt
      TxLen++;
      stat = 1;
    }
    __restore_interrupt(cs);
    return stat;
  }
  //=======================================
}
#undef rs_baud
#undef rs_parity
#undef rs_bits
#undef UART_RX_BUFFER_SIZE
#undef UART_RX_BUFFER_MASK
#undef UART_TX_BUFFER_SIZE
#undef UART_TX_BUFFER_MASK
#undef RS485_R
#undef RS485_T
#undef RS485_S
#undef RS485_D
//====================================
#endif
