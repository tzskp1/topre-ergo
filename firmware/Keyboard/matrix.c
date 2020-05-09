#include "matrix.h"
#include "parameter.h"
#define RELAX_TIME 20

struct state states[4][7] = {0};

uint16_t last_time = 0;

void init_matrix () {
  DDRB = 0xFF;
  DDRD |= _BV(INH);
  DDRD |= _BV(SELA);
  DDRE |= _BV(SELB);
  ADCSRA |= _BV(ADEN);  // wake up adc
  ADCSRA &= ~_BV(ADIE); // disallow interrupt
  ADCSRA &= ~_BV(ADATE); // mode
  ADCSRB &= 0b11110000;
  //ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);  // 104us
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

void vacuum_drain (void){
  DDRC |= _BV(DRAIN);
  PORTC &= ~_BV(DRAIN);
}

void float_drain (void){
  DDRC &= ~_BV(DRAIN);
}

uint16_t strobe_read (int row, int col) {
  uint16_t val;
  set_row (row);
  while (diff_upto16 (TCNT1, last_time) < RELAX_TIME);
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

uint8_t normalize (int row, int col, uint16_t value) {
  if (value > keyhi[row][col])
    value = keyhi[row][col];
  if (value < keylo[row][col])
    value = keylo[row][col];
  uint16_t t = 0xFF * (value - keylo[row][col]);
  uint16_t size = keyhi[row][col] - keylo[row][col];
  return (uint8_t) (t / size);
}

void scan_matrix () {
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 7; j++) {
      states[i][j].depth = normalize(i, j, strobe_read (i, j));
      if (states[i][j].pressed == 0 && states[i][j].depth > ACTUATION) {
        states[i][j].pressed = 1;
      } else if (states[i][j].pressed == 1 && states[i][j].depth < RELEASE) {
        states[i][j].pressed = 0;
      }
    }
  }
}
