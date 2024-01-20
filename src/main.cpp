#include "libraries.h"
#include "functions.h"
#include "TopSecret.h"

SimpleTimer timer;

void setup()
{
  // Enable and turn off the On-board LED
  // (notice that it's inverted on most ESP boards)
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(115200);
  delay(500); // Delay to let the ESP get booted before sending out serial data
  Serial.printf("\n+---------------------------------------+\n");
  Serial.printf("|  ESP With a little OLED & 2 DS18B20s  |\n");
  Serial.printf("|  WiFi Enabled...                      |\n");
  Serial.printf("|  Sends readings out through MQTT...   |\n");
  Serial.printf("|  Able to respond to MQTT...           |\n");
  Serial.printf("|  (Now, with excitable StickDude)      |\n");
  // Serial.printf("+---------------------------------------+\n");

  // SSD1306 ///////////////////////////////////////////////////////////////////
  SSD1306_init();

  // Display static text
  SSD1306_title("ColdStuff");
  SSD1306_Static("Beer:", 1);
  SSD1306_Static("Food:", 2);

  Serial.printf("| Static Text on-screen                 |\n");

  SSD1306_Static("  Booting  ", 3);

  // DS18B20 ///////////////////////////////////////////////////////////////////

  DS18B20_init();

  // Interval Timers ///////////////////////////////////////////////////////////

  Serial.printf("+---------------------------------------+\n");
  timer.setInterval(80, SSD1306_Dotty);
  timer.setInterval(20, DOtheBloodyMQTT);
  // delay(66);
  timer.setInterval(10000, SSD1306_DisplayTemps);

  Serial.printf("| Interval Timers running               |\n");

  WiFi_init();

  MQTT_init();

// SSD1306_StickDude();

  Serial.printf("#=======================================#\n");
  Serial.printf("#             Ready To Run.             #\n");
  Serial.printf("#=======================================#\n");
}

void loop()
{
  // Run the interval timers
  // Running processes here in loop() is a synchronous flow.
  // Running processes via the interval timers is assynchronous.
  // If your actions don't depend directly on each other,
  // synchronous flow makes no sense at all...
  timer.run();
}