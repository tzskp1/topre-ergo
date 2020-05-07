#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main()
{
  int c = 0;
  DDRB = 0b00000001;

  for(;;) {
    PINB = 0b00000001;
    _delay_ms(100);
    c += PINB;
    PINB = 0b00000000;
    _delay_ms(100);
    c += PINB;
  }
  return 0;
}
