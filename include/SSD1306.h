#ifndef SSD1306_H
#define SSD1306_H

// I2C pins suitable for the ESP-M3-Everything!
const int SSD1306_SDA = 14; // Tarduino D5
const int SSD1306_SCL = 12; // Tarduino D6

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
// Static Information Canvases
// Full width of the display
// GFXcanvas1 Scanvas1(120, 15);
// GFXcanvas1 Scanvas2(120, 15);
GFXcanvas1 Scanvas3(120, 15);
// Dynamic Information Canvases
// Sized specifically for '%5.1f' temperature readings
GFXcanvas1 Dcanvas1(55, 12);
GFXcanvas1 Dcanvas2(55, 12);
// GFXcanvas1 Dcanvas3(55, 12);

// GFXcanvas1 StickDude(8, 11);
GFXcanvas1 StickDudePIC(8, 12);

int row_Y[] = {27, 42, 57}; // Locations the rows in the main screen part (Blue)

// 
int DottyX = 0; // Initialize the dots position OUTSIDE of the actual loop...

#endif // SSD1306_H