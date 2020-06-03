#include "Keyboard.h"
#include "../keycode.h"
#include "../matrix.h"
#include "../twi.h"

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */

int main(void)
{
  SetupHardware ();
  GlobalInterruptEnable ();
  init_arraycode ();
  init_matrix ();
  twi_init ();

  for (;;) {
    HID_Task ();
    USB_USBTask ();
  }
}
