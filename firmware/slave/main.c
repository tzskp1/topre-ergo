#include "Keyboard.h"
#include "../keycode.h"
#include "../matrix.h"

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */

int main(void)
{
  SetupHardware ();
  GlobalInterruptEnable ();
  init_arraycode ();
  init_matrix ();

  for (;;) {
    HID_Task ();
    USB_USBTask ();
  }
}
