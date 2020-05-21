#include "../keycode.h"

#define NULL { .number = 1, .mask = 0x00 }
#define A    { .number = 1, .mask = 0x10 }
#define B    { .number = 1, .mask = 0x20 }
#define C    { .number = 1, .mask = 0x40 }
#define D    { .number = 1, .mask = 0x80 }
#define E    { .number = 2, .mask = 0x01 }
#define F    { .number = 2, .mask = 0x02 }
#define G    { .number = 2, .mask = 0x04 }
#define H    { .number = 2, .mask = 0x08 }
#define I    { .number = 2, .mask = 0x10 }
#define J    { .number = 2, .mask = 0x20 }
#define K    { .number = 2, .mask = 0x40 }
#define L    { .number = 2, .mask = 0x80 }
#define M    { .number = 3, .mask = 0x01 }
#define N    { .number = 3, .mask = 0x02 }
#define O    { .number = 3, .mask = 0x04 }
#define P    { .number = 3, .mask = 0x08 }
#define Q    { .number = 3, .mask = 0x10 }
#define R    { .number = 3, .mask = 0x20 }
#define S    { .number = 3, .mask = 0x40 }
#define T    { .number = 3, .mask = 0x80 }
#define U    { .number = 4, .mask = 0x01 }
#define V    { .number = 4, .mask = 0x02 }
#define W    { .number = 4, .mask = 0x04 }
#define X    { .number = 4, .mask = 0x08 }
#define Y    { .number = 4, .mask = 0x10 }
#define Z    { .number = 4, .mask = 0x20 }

struct code keycode[128] =
  { NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL };


struct code arraycode[4][7] = {0};

void init_arraycode () {
  arraycode[0][0].number = (41 / 8) + 1;
  arraycode[0][0].mask = 1 << (41 % 8);
  // ESC
  arraycode[0][1].number = (52 / 8) + 1;
  arraycode[0][1].mask = 1 << (52 % 8);
  //'
  arraycode[0][2].number = (54 / 8) + 1;
  arraycode[0][2].mask = 1 << (54 % 8);
  //,
  arraycode[0][3].number = (55 / 8) + 1;
  arraycode[0][3].mask = 1 << (55 % 8);
  //.
  arraycode[0][4] = keycode['p'];
  arraycode[0][5] = keycode['y'];
  // arraycode[0][6] = ?;

  // arraycode[1][0] = ?;
  arraycode[1][1] = keycode['a'];
  arraycode[1][2] = keycode['o'];
  arraycode[1][3] = keycode['e'];
  arraycode[1][4] = keycode['u'];
  arraycode[1][5] = keycode['i'];
  // arraycode[1][6] = ?;

  // arraycode[2][0] = ?;
  arraycode[2][1].number = (51 / 8) + 1;
  arraycode[2][1].mask = 1 << (51 % 8);
  //;
  arraycode[2][2] = keycode['q'];
  arraycode[2][3] = keycode['j'];
  arraycode[2][4] = keycode['k'];
  // arraycode[2][5] = keycode['x'];
  // arraycode[2][6] = ?;
}
