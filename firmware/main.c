#include <avr/io.h>
#include "matrix.h"

int main()
{
  init_matrix();
  DDRD |= _BV(5);
  while (1) {
    scan_matrix ();
    if (states[2][2].pressed) {
      PORTD |= _BV(5);
    } else {
      PORTD &= ~_BV(5);
    }
  }
}
