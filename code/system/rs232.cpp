
#include "Configure.h"

#ifdef RS232
#include "../main.h"
//====================================
#include "rs232.h"
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
// UART Buffer Defines
// 1,2,4,8,16,32,64,128 or 256 bytes
#define UART_RX_BUFFER_SIZE 32
#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1 )
// 1,2,4,8,16,32,64,128 or 256 bytes
#define UART_TX_BUFFER_SIZE 32
#define UART_TX_BUFFER_MASK ( UART_TX_BUFFER_SIZE - 1 )
#if (UART_RX_BUFFER_SIZE & UART_RX_BUFFER_MASK)
#error RS232 RX buffer size is not a power of 2
#endif
#if (UART_TX_BUFFER_SIZE & UART_TX_BUFFER_MASK)
#error RS232 TX buffer size is not a power of 2
#endif

namespace ns_rs232
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
  //----------
  void init(void)
  {
    unsigned int baud;
    baud = (unsigned int)( (unsigned long)C_Fosc/((unsigned long)rs_baud*16) );
    UBRR0H = (unsigned char)(baud>>8);
    UBRR0L = (unsigned char)baud;
    // Parity
    #define parity_error
    UCSR0C = 0;
    #ifdef rs_parity_disable
    UCSR0C_UPM00 = 0;
    UCSR0C_UPM01 = 0;
    #undef parity_error
    #endif
    #ifdef rs_parity_even
    UCSR0C_UPM00 = 0;
    UCSR0C_UPM01 = 1;
    #undef parity_error
    #endif
    #ifdef rs_parity_odd
    UCSR0C_UPM00 = 1;
    UCSR0C_UPM01 = 1;
    #undef parity_error
    #endif
    #ifdef parity_error
    #error RS232 Error define parity : only disable, even or odd
    #endif
    // Bits
    #define bits_error
    #ifdef rs_bits_5
    UCSR0C_UCSZ00 = 0;
    UCSR0C_UCSZ01 = 0;
    UCSR0B_UCSZ02 = 0;
    #undef  bits_error
    #endif
    #ifdef rs_bits_6
    UCSR0C_UCSZ00 = 1;
    UCSR0C_UCSZ01 = 0;
    UCSR0B_UCSZ02 = 0;
    #undef  bits_error
    #endif
    #ifdef rs_bits_7
    UCSR0C_UCSZ00 = 0;
    UCSR0C_UCSZ01 = 1;
    UCSR0B_UCSZ02 = 0;
    #undef  bits_error
    #endif
    #ifdef rs_bits_8
    UCSR0C_UCSZ00 = 1;
    UCSR0C_UCSZ01 = 1;
    UCSR0B_UCSZ02 = 0;
    #undef  bits_error
    #endif
    #ifdef bits_error
    #error RS232 error define bits, only 5, 6, 7, 8
    #endif
    // 1 стоп бит
    UCSR0C_USBS0 = 0;
    // начальные значения
    RxHead =  RxTail = TxHead = TxTail = RxLen = RxOverFlow = TxLen = TxOverFlow = 0;
    //enable UART receiver and transmitter,transmitte and receive interrupt
    UCSR0B = 0;
    UCSR0B_RXCIE0 = 1;
    UCSR0B_RXEN0  = 1;
    UCSR0B_TXEN0  = 1;
    UCSR0B_TXCIE0 = 1;
  }
  //===================================
  #pragma vector = USART0_RXC_vect // Прерывание приема от RS232
  __interrupt void UART232_RX_interrupt(void)
  {
    unsigned char data;
    unsigned char tmphead;
    // read the received data
    #ifdef rs_bits_5
    data = UDR0 & 0x1f;
    #endif
    #ifdef rs_bits_6
    data = UDR0 & 0x3f;
    #endif
    #ifdef rs_bits_7
    data = UDR0 & 0x7f;
    #endif
    #ifdef rs_bits_8
    data = UDR0;
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
  //===================================
  #pragma vector = USART0_UDRE_vect /* Прерывание передачи от RS232 */
  __interrupt void UART_TX_interrupt(void)
  {
    unsigned char tmptail;
    // check if all data is transmitted
    if(TxHead != TxTail)
    {
      // calculate buffer index
      tmptail = (TxTail + 1) & UART_TX_BUFFER_MASK;
      TxTail = tmptail;       // store new index
      UCSR0B_TXB80 = 1;
      UDR0 = TxBuf[tmptail];  // start transmition
      TxLen--;
    }
    else
    {
      UCSR0B_UDRIE0 = 0;   // disable UDRE interrupt
      //
      TxLen = 0;
    }
  }
  //===================================
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
      stat = 0; // wait for incomming data
    }
    else
    {
      // calculate buffer index
      tmptail = (RxTail + 1) & UART_RX_BUFFER_MASK;
      RxTail = tmptail;          // store new index
      *Byte = RxBuf[tmptail];    // return data
      RxLen--;
      stat = 1;
    }
    __restore_interrupt(cs);
    return stat;
  }
  //===================================
  unsigned char WriteByte(unsigned char data)
  {
    unsigned char tmphead;
    unsigned char stat;
    unsigned char cs;
    cs = __save_interrupt();
    __disable_interrupt();
    tmphead = (TxHead + 1) & UART_TX_BUFFER_MASK; 
    if (tmphead==TxTail)
    {
      TxOverFlow = 1;
      stat = 0;// There is no free place in the buffer
    }
    else
    {      
      TxBuf[tmphead] = data; // store data in buffer
      TxHead = tmphead; // store new index
      UCSR0B_UDRIE0 = 1; // enable UDRE interrupt
      TxLen++;
      stat = 1;
    }
    __restore_interrupt(cs);
    return stat;
  }
  //===================================
}
#undef rs_baud
#undef rs_parity
#undef rs_bits
#undef UART_RX_BUFFER_SIZE
#undef UART_RX_BUFFER_MASK
#undef UART_TX_BUFFER_SIZE
#undef UART_TX_BUFFER_MASK
//====================================
#endif
