#include <xc.h>
#pragma config FOSC=HS,WDTE=OFF,PWRTE=OFF,MCLRE=ON,CP=OFF,CPD=OFF,BOREN=OFF,CLKOUTEN=OFF
#pragma config IESO=OFF,FCMEN=OFF,WRT=OFF,VCAPEN=OFF,PLLEN=OFF,STVREN=OFF,LVP=OFF
#define _XTAL_FREQ 8000000

char unitAddress = 0x03; // address 0x01 is master
int doneTx=0;
__bit rNine;
char TXaddress, TXmessage;
char RXaddress, RXmessage;

int myAddress(char);
void UART_Send(char, char);

void processMessage(char);
void sendMessages();

char UART_Init(const long int baudrate) {
	unsigned int x;
	x = (_XTAL_FREQ - baudrate*16)/(baudrate*16);
    // moze biti odredjeno automatski, ali za ovaj primjer ce biti zadano radi jednostavnijeg primjera
  BRGH = 1;
  SPBRG = x;
  SYNC = 0;
  SPEN = 1;
  TX9 = 1;
  RX9 = 1;
  ADDEN = 1; // enable TXaddress detection

  //  ANSELC6 = 0;
  //  ANSELC7 = 0;
  TRISC = 0b10000000;

  CREN = 1;
  TXEN = 1;
  return 1;
}
void init() {
  UART_Init(9600);
  ANSELB = 0x00;

  TRISB = 0b00001111; // 0-3 input 4-7 output
  PORTB = 0x00;
  PORTC = 0x00;
  INTCONbits.GIE=1;	/* Enable Global Interrupt */
  INTCONbits.PEIE=1;
  PIE1bits.RCIE=1;
  PIE1bits.TXIE=0;
}

void main(void) {
  init();
  while(1) if(unitAddress == 0x01) sendMessages();
}

void UART_Send(char add, char msg) {
  while(!RCIDL);
  PORTCbits.RC5 = 1;
  TX9D = 1;
  TXREG = add;
  while(!TXIF);
  TX9D = 0;
  TXREG = msg;
  while(!TRMT);
  PORTCbits.RC5 = 0;
}

int myAddress(char add) {
  return add==unitAddress;
}

void processMessage(char msg) {
  if ((msg>>2)%2) {
    PORTBbits.RB7 = (msg>>1) % 2;
    PORTBbits.RB6 = msg % 2;
  } else {
    PORTBbits.RB5 = (msg>>1) % 2;
    PORTBbits.RB4 = msg % 2;
  }
  if ((msg>>3)%2) sendMessages();
}

void interrupt ISR() {
  // receive
  if (RCIE && RCIF) {
    if (ADDEN) { // Address detected
      RXaddress = RCREG;
      if (myAddress(RXaddress)) ADDEN = 0;
    } else if (!ADDEN) { // Message and unit address previously detected
      RXmessage = RCREG;
      ADDEN = 1;
      processMessage(RXmessage);
    }
  }
}
void waitForResponse() {
  __delay_ms(1); // should not cause more delay, using it to mask start and end bits
  while(!RCIDL); // address
  __delay_ms(1);
  while(!RCIDL); // msg

  __delay_ms(1); // 1ms buffer
}
void sendMessages() {
  switch(unitAddress) {
    case 0x01:
      UART_Send(0x02, (((1 << 2) - 1) & (PORTB >> 0)) + 12); // approx 3ms
      waitForResponse(); // sends two messages, easy way to evade colisions
      waitForResponse();
      UART_Send(0x03, (((1 << 2) - 1) & (PORTB >> 2)) + 12); 
      waitForResponse();
      waitForResponse();
      break;
    case 0x02:
      UART_Send(0x01, (((1 << 2) - 1) & (PORTB >> 0)) + 4);
      UART_Send(0x03, ((1 << 2) - 1) & (PORTB >> 2));
      break;
    case 0x03:
      UART_Send(0x01, ((1 << 2) - 1) & (PORTB >> 0));
      UART_Send(0x02, ((1 << 2) - 1) & (PORTB >> 2));
      break;
  }
}
