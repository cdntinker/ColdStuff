#include "libraries.h"
#include "functions.h"

#include "StickDude.h"

#include "SSD1306.h"

////////////////////////////////////////////////////////////////////////////////
// This code is based on using an
// IZOKEE 0.96" SSD1306 based 128x64 OLED (Yellow/Blue) from Amazon
// { https://www.amazon.ca/gp/product/B076PNP2VD/ }
////////////////////////////////////////////////////////////////////////////////

// Initialize the display
void SSD1306_init()
{
  Serial.printf("+---------------------------------------+\n");
  Wire.begin(SSD1306_SDA, SSD1306_SCL); // Select SDA & SCL pins

  { // debug output (StickDude Stats)
    Serial.printf("StickDude_wave:        %d\n", sizeof(StickDude_wave));
    Serial.printf("StickDude_wave height: %d\n", StickDude_wave_height);
    Serial.printf("StickDude_wave count:  %d\n", StickDude_wave_count);
    Serial.printf("StickDude_wave width:  %d\n", StickDude_wave_width);

    Serial.printf("StickDude_walk: %d\n", sizeof(StickDude_walk));
    Serial.printf("StickDude_walk height: %d\n", StickDude_walk_height);
    Serial.printf("StickDude_walk count:  %d\n", StickDude_walk_count);
    Serial.printf("StickDude_walk width:  %d\n", StickDude_walk_width);
  }

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.printf("| SSD1306 allocation failed             |\n");
    for (;;)
      ;   // Don't proceed, loop forever
          //  Does this ever actually happen?
  }

  display.clearDisplay();

  display.dim(true); // These displays are excessively bright

  // Set up the active (changing) parts of the display
  Dcanvas1.setFont(&FreeMonoBold9pt7b);
  Dcanvas2.setFont(&FreeMonoBold9pt7b);
  // Dcanvas3.setFont(&FreeMonoBold9pt7b);

  // Set up the static (non-changing) parts of the display
  // Scanvas1.setFont(&FreeMonoBold9pt7b);
  // Scanvas2.setFont(&FreeMonoBold9pt7b);
  Scanvas3.setFont(&FreeMonoBold9pt7b);

  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);

  Serial.printf("| SSD1306 initialized                   |\n");
}

// Place static text on the yellow part of the OLED display
// Accepts the string (max 9 characters)
void SSD1306_title(const char *Title)
{ // Max 9 characters...
  display.setFont(&FreeMonoBold12pt7b);
  display.setCursor(0, 14);
  display.print(Title);
  display.display();
}

// Place static text on the blue part of the OLED display
// Accepts the string (max 11 characters) and the row number (1-3)
void SSD1306_Static(const char *Line, int Row)
{ // Max 11 characters
  Scanvas3.fillScreen(BLACK);
  Scanvas3.setCursor(0, 11);
  Scanvas3.print(Line);
  display.drawBitmap(4, row_Y[Row - 1] - 9, Scanvas3.getBuffer(), 120, 15, WHITE, BLACK);

  display.display();
}

// Create an activity indicator
// Just a little dot that crawls across the bottom edge of the display
int DudeStep = 0;
void SSD1306_Dotty()
{
  DottyX++;
  if (DottyX > SCREEN_WIDTH)
  {
    DottyX = 0;
  }
  if (DudeStep >= StickDude_walk_count)
  {
    DudeStep = 0;
  }
  // display.drawPixel(DottyX, SCREEN_HEIGHT - 1, WHITE); // Bottom line
  // display.drawPixel(DottyX - 1, SCREEN_HEIGHT - 1, BLACK);
  DrawStickDude2(DudeStep, 0, 0);
  DudeStep++;
  display.drawBitmap(DottyX, 64 - 12, StickDudePIC.getBuffer(), 8, 12, WHITE, BLACK);
  display.display();
}

// Display the temperatures on the pre-formatted OLED display
void SSD1306_DisplayTemps()
{
  float Beer = DS18B20_getReading(0); // * .3;
  float Food = DS18B20_getReading(1); // * -1;

  char BeerTemp[6];
  char FoodTemp[6];

  Dcanvas1.fillScreen(BLACK);
  if (Beer == -127)
  {
    sprintf(BeerTemp, "-----");
    Beer = 0;
  }
  else
    sprintf(BeerTemp, "%5.1f", Beer);

  Dcanvas2.fillScreen(BLACK);
  if (Food == -127)
  {
    sprintf(FoodTemp, "-----");
    Food = 0;
  }
  else
    sprintf(FoodTemp, "%5.1f", Food);

  Dcanvas1.setCursor(0, 11);
  Dcanvas1.print(BeerTemp);
  display.drawBitmap(64, row_Y[0] - 9, Dcanvas1.getBuffer(), 55, 12, WHITE, BLACK);

  Dcanvas2.setCursor(0, 11);
  Dcanvas2.print(FoodTemp);
  display.drawBitmap(64, row_Y[1] - 9, Dcanvas2.getBuffer(), 55, 12, WHITE, BLACK);

  display.display();

  MQTT_SendData(Beer, Food);

  waveStickDude(0, 0);
  // walkStickDude(0, 0);
  // waveStickDude(0, 0);

#ifdef DEBUG
  Serial.printf("| Sensor 0: %5s - Sensor 1: %5s     |\n", BeerTemp, FoodTemp);
#endif
}

void DrawStickDude(int DudeNum, int TOP, int LEFT)
{
  // display.clearDisplay();
  StickDudePIC.fillScreen(BLACK);

  StickDudePIC.drawBitmap(
      (0),
      (0),
      StickDude_beer[DudeNum], DUDE_WIDTH, DUDE_HEIGHT, 1);

  // display.display();
}

void walkStickDude(int TOP, int LEFT)
{
  // Serial.printf("| Loop through StickDudes Poses...      |\n");
  int FOO;

  for (FOO = 0; FOO < StickDude_walk_count; FOO++)
  {
    {
      DrawStickDude2(FOO, 0, 0);
      display.drawBitmap(DottyX, 64 - 12, StickDudePIC.getBuffer(), 8, 12, WHITE, BLACK);
      display.display();
      delay(30);
    }
  }
  FOO--;
  for (; FOO >= 0; FOO--)
  {
    {
      DrawStickDude2(FOO, 0, 0);
      display.drawBitmap(DottyX, 64 - 12, StickDudePIC.getBuffer(), 8, 12, WHITE, BLACK);
      display.display();
      delay(30);
    }
  }
}

void waveStickDude(int TOP, int LEFT)
{
  // Serial.printf("| Loop through StickDudes Poses...      |\n");
  int FOO;

  for (FOO = 0; FOO < StickDude_beer_count; FOO++)
  {
    {
      DrawStickDude(FOO, 0, 0);
      display.drawBitmap(DottyX, 64 - 12, StickDudePIC.getBuffer(), 8, 12, WHITE, BLACK);
      display.display();
      delay(60);
    }
  }
  FOO--;
  for (; FOO >= 0; FOO--)
  {
    {
      DrawStickDude(FOO, 0, 0);
      display.drawBitmap(DottyX, 64 - 12, StickDudePIC.getBuffer(), 8, 12, WHITE, BLACK);
      display.display();
      delay(30);
    }
  }
}

void DrawStickDude2(int DudeNum, int TOP, int LEFT)
{
  // display.clearDisplay();
  StickDudePIC.fillScreen(BLACK);

  StickDudePIC.drawBitmap(
      (0),
      (0),
      StickDude_walk[DudeNum], DUDE_WIDTH, DUDE_HEIGHT, 1);

  // display.display();
}