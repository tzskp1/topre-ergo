#ifndef matrix_h
#define matrix_h
  #include <avr/io.h>
  #include <avr/interrupt.h>

  #define DRAIN 6 /* C */
  #define INH   7 /* D */
  #define READ  4 /* F */
  #define SELA  2 /* D */
  #define SELB  6 /* E */

  #define ACTUATION  200
  #define RELEASE    100

  struct state {
    uint8_t depth;
    uint8_t pressed;
  };

  extern struct state states[4][7];

  void init_matrix (void);

  uint16_t diff_upto16 (uint16_t a, uint16_t b);

  uint16_t strobe_read (int row, int col);

  void scan_matrix (void);
#endif
