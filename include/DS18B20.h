#ifndef DS18B20_H
#define DS18B20_H

const int oneWireBus = 2;            // Where the sensor(s) connect (GPIO4 == Tarduino D2)

OneWire oneWire(oneWireBus);         // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature sensor

#endif // DS18B20_H