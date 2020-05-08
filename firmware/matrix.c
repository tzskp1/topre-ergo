#include "matrix.h"

uint16_t keyhi[4][7] =
  { {500, 500, 500, 500, 500, 500, 500 },
    {500, 500, 500, 500, 500, 500, 500 },
    {500, 500, 500, 500, 500, 500, 500 },
    {500, 500, 500, 500, 500, 500, 500 } };

uint16_t keylo[4][7] =
  { {500, 500, 500, 500, 500, 500, 500 },
    {500, 500, 500, 500, 500, 500, 500 },
    {500, 500, 500, 500, 500, 500, 500 },
    {500, 500, 500, 500, 500, 500, 500 } };

uint16_t state[4][7] = {0};

uint16_t last_time = 0;

void init_matrix () {
  DDRB = 0xFF;
  DDRD |= (1 << INH);
  DDRD |= (1 << SELA);
  DDRE |= (1 << SELB);
  ADCSRA |= _BV(ADEN);  // wake up adc
  ADCSRA &= ~_BV(ADIE); // disallow interrupt
  ADCSRA |= _BV(ADATE); // mode
  ADCSRB &= 0b11110000;
  // ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);  // 104us
  ADCSRA |= _BV(ADPS1);
  ADMUX |= (1 << 6);    // right padding, vcc ref
  ADMUX |= READ;        // READ
  TCCR1B = 0b00000001;  // start counter1
  last_time = TCNT1;
  //ADCSRB &= ~_BV(MUX5);
}

void set_row(int row) {
  switch (row) {
  case 0:
    PORTD &= ~_BV(SELA);
    PORTE &= ~_BV(SELB);
    break;
  case 1:
    PORTD |= _BV(SELA);
    PORTE &= ~_BV(SELB);
    break;
  case 2:
    PORTD &= ~_BV(SELA);
    PORTE |= _BV(SELB);
    break;
  case 3:
    PORTD |= _BV(SELA);
    PORTE |= _BV(SELB);
    break;
  default:
    break;
  }
}

uint16_t diff_upto16 (uint16_t a, uint16_t b) {
  if (a < b) {
    return (0xFFFF - b) + a;
  } else {
    return a - b;
  }
}

void vacuum_drain (){
  DDRC |= _BV(DRAIN);
  PORTC &= ~_BV(DRAIN);
}

void float_drain (){
  DDRC &= ~_BV(DRAIN);
}

uint16_t strobe_read (int row, int col) {
  uint16_t val;
  set_row (row);
  while (diff_upto16 (TCNT1, last_time) < relax_time);
  float_drain ();
  PORTB = _BV(col);
  ADCSRA |= _BV(ADSC);
  while (ADCSRA & _BV(ADSC));
  cli();
  val = ADC;
  sei();
  PORTB = 0;
  vacuum_drain();
  last_time = TCNT1;
  return val;
}

void scan_matrix () {
  int i, j;
  for(i = 0; i < 4; i++) {
    for(j = 0; j < 7; j++) {
      state[i][j] = strobe_read (i, j);
    }
  }
}
