#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// WiFi ////////////////////////////////////////////////////////////////////////

void WiFi_init();

int WiFi_strength();

// MQTT ////////////////////////////////////////////////////////////////////////

void MQTT_init();

void MQTT_callback(char *topic, byte *payload, int length);
void MQTT_reconnect();
void MQTT_beacon();

void DOtheBloodyMQTT();

void MQTT_SendData(float Temp0, float Temp1);

// OLED ////////////////////////////////////////////////////////////////////////

void SSD1306_init();
void SSD1306_title(const char *Title);
void SSD1306_Static(const char *Line, int Row);
void SSD1306_DisplayTemps();
void SSD1306_Dotty();
// void SSD1306_StickDude();
void DrawStickDude(int DudeNum, int TOP, int LEFT);
void DrawStickDude2(int DudeNum, int TOP, int LEFT);
void walkStickDude(int TOP, int LEFT);
void waveStickDude(int TOP, int LEFT);

// DS18B20 /////////////////////////////////////////////////////////////////////

int DS18B20_init();
float DS18B20_getReading(int deviceNumber);

// Supplimental Stuff //////////////////////////////////////////////////////////

bool getTimer(unsigned long &timer, unsigned long interval);
void blinkLED();                // Just a little function to blink the LED for the beacon function.

#endif // FUNCTIONS_H