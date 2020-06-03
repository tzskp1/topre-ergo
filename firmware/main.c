#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "twi.h"


void onRequestService(void)
{
  return;
}

void onReceiveService(uint8_t* inBytes, int numBytes)
{
    return;
}

int main()
{
  DDRB = 0xFF;
  uint8_t data[100];
  sei();
  twi_init();
  //twi_attachSlaveTxEvent(onRequestService); // default callback must exist
  //twi_attachSlaveRxEvent(onReceiveService); // default callback must exist
  while (1) {
    // twi_transmit(&data, 1);
    twi_readFromHead(8, 1, 10);
    PORTB = 0x00;
    twi_readFromTail(data, 10);
    PORTB = 0xFF;
  }
  return 0;
}
