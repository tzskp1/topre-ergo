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
  sei();
  twi_init();
  twi_attachSlaveTxEvent(onRequestService); // default callback must exist
  twi_attachSlaveRxEvent(onReceiveService); // default callback must exist
  uint8_t data[100] = "test\n";
  while (1) {
    // twi_transmit(&data, 1);
    PORTB = 0xFF;
    uint8_t ret = twi_writeTo(8, data, 4, 1, 1);
    PORTB = 0x00;
  }
}
