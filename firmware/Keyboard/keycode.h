#ifndef keycode_h
#define keycode_h
  #include <stdint.h>
  struct code {
    uint8_t number;
    uint8_t mask;
  };
  extern struct code keycode[128];
  extern struct code arraycode[4][7];

  void init_arraycode (void);
#endif
