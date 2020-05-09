#include "keycode.h"
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
  arraycode[2][2] = keycode['b'];
  arraycode[1][2] = keycode['a'];
}
