#include "libraries.h"
#include "functions.h"

#include "DS18B20.h"

// Start the DS18B20 sensor(s)
// returns number of devices found, if you're interested...
int DS18B20_init()
{
  Serial.printf("+---------------------------------------+\n");
  sensors.begin();
  Serial.printf("|        Sensor(s) initialised          |\n");

  // Grab a count of devices on the wire
  int numberOfDevices = sensors.getDeviceCount();

  Serial.printf("|   Found %2d DS18B20s                   |\n", numberOfDevices);

  return (numberOfDevices);
}

// Read a DS18B20 sensor
// Accepts a device number (in case you have more than one)
// Returns the current reading (Celsius because Farhenheit is for losers)
// (If you're a loser...  Change getTempCByIndex to getTempFByIndex :)
float DS18B20_getReading(int deviceNumber)
{
  sensors.requestTemperatures();
  return (sensors.getTempCByIndex(deviceNumber));
}