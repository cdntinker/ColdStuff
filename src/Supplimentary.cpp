#include "libraries.h"
#include "functions.h"

////////////////////////////////////////////////////////////////
// Supplimentary Functions
////////////////////////////////////////////////////////////////

// Good chance this is entirely useless because SimpleTimer exists...
bool getTimer(unsigned long &timer, unsigned long interval)
/* Global timer function, used a lot
 *
 * @param unsigned long time - The current timer
 * @param unsigned long interval - Length of time to run timer
 *
 * @return bool True when timer is complete
 * @return bool False when timer is counting
 */
{

  if (timer < 1)
  {
    timer = millis();
  }

  if (millis() - timer >= interval)
  {
    timer = 0;
    return true;
  }

  return false;
}

void blinkLED() // Just a little function to blink the on-board LED for the beacon function.
{
  // On most ESP8266-based modules, GPIO2 is connected to the
  // on-board LED (& it's inverted... so pulling this GPIO low
  // will turn on the on-board LED.

  digitalWrite(LED_BUILTIN, LOW);
  delay(5);
  digitalWrite(LED_BUILTIN, HIGH);
}