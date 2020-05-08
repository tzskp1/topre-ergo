#ifndef matrix_h
#define matrix_h
  #include <avr/io.h>
  #include <avr/interrupt.h>

  #define DRAIN 6 /* C */
  #define INH   7 /* D */
  #define READ  4 /* F */
  #define SELA  2 /* D */
  #define SELB  6 /* E */

  const uint16_t relax_time = 80;

  extern uint16_t state[4][7];

  void init_matrix ();

  uint16_t diff_upto16 (uint16_t a, uint16_t b);

  uint16_t strobe_read (int row, int col);

  void scan_matrix ();
#endif
