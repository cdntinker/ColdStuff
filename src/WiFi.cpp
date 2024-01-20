#include "libraries.h"
#include "functions.h"

////////////////////////////////////////////////////////////////
// These exist because the variables are declared elsewhere in
// the project.
extern char *WiFi_ssid;
extern char *WiFi_password;
extern char *WiFi_ClientName;
////////////////////////////////////////////////////////////////

// Initialize the WiFi
//
// If we don't get a connection, we just keep trying forever.
// Probably be an idea to eventually just give up, but most
// ESP8266 devices are kinda useless without the connection...
void WiFi_init()
{

    // We start by connecting to a WiFi network
    Serial.printf("+---------------------------------------+\n");
    Serial.printf("| Connecting to: %-22s |\n", WiFi_ssid);
    Serial.printf("| ");

    WiFi.mode(WIFI_STA); // Connecting as a STATION
    WiFi.begin(WiFi_ssid, WiFi_password);

    int dotcount = 0;
    while (WiFi.status() != WL_CONNECTED) // Give it a bit of time to establish the WiFi connection...
    {
        dotcount++;
        delay(500);
        Serial.printf(".");
        SSD1306_Static("  no WiFi  ", 3);
    }

    for (int i = 0; i < (38 - dotcount); i++)
    {
        Serial.printf(" ");
    }
    Serial.printf("|\n");

    Serial.printf("| WiFi connected on Channel %-2d          |\n", WiFi.channel());
    Serial.printf("| IP address: %-15s           |\n", WiFi.localIP().toString().c_str());

    Serial.printf("| My name is NOT %-22s |\n", wifi_station_get_hostname());
    Serial.printf("| Really... It's %-22s |\n", WiFi_ClientName);

    SSD1306_Static(" WiFi good ", 3);

    // Add "build_flags = -D DEBUG0" to your platformio.ini to get some extra WiFi stats
#ifdef DEBUG0
    WiFi.printDiag(Serial);
    Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
    Serial.print("HostName: ");
    Serial.print(WiFi.hostname());
    Serial.println(" !!!!!!!!!  BULLSHIT !!!!!!!!!");
    // Apparently, the ESP8266WiFi library does not actually support DHCP completely
    // { https://www.reddit.com/r/arduino/comments/d6mvc7/getting_hostname_from_dhcp_with_esp8266/ }
    // { https://github.com/esp8266/Arduino/issues/5695 }
#endif
}

int WiFi_strength()
{
        // Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
return (WiFi.RSSI());
}