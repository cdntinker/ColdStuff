#include "libraries.h"
#include "functions.h"

#include "MQTT.h"

WiFiClient espClient;
PubSubClient MQTT_client(espClient);

void DOtheBloodyMQTT()
{
    if (!MQTT_client.connected())
    {
        MQTT_reconnect();
    }
    MQTT_client.loop();
    MQTT_beacon();

}

////////////////////////////////////////////////////////////////
// MQTT Functions
////////////////////////////////////////////////////////////////

void MQTT_init()
{
    Serial.printf("+---------------------------------------+\n");
    MQTT_client.setServer(MQTT_broker, 1883);
    MQTT_client.setCallback(MQTT_callback);

    //  Build the topic names
    strcpy(MQTT_inTopic, "cmnd/"); //  in - Commands
    strcat(MQTT_inTopic, MQTT_ClientName);
    strcat(MQTT_inTopic, "/#");
    strcpy(MQTT_teleTopic, "tele/"); // out - Telemetry
    strcat(MQTT_teleTopic, MQTT_ClientName);
    strcpy(MQTT_statTopic, "stat/"); // out - Status
    strcat(MQTT_statTopic, MQTT_ClientName);
    strcpy(MQTT_outTopic, "noti/"); // out - Notifications
    strcat(MQTT_outTopic, MQTT_ClientName);

    MQTT_reconnect();
}

void MQTT_callback(char *topic, byte *payload, int length)
{

    char MQTT_msg_in[MQTT_BUFFER_SIZE];
    char *MQTT_command = strrchr(topic, '/');

#ifdef DEBUG
    Serial.printf("|                                       |\n");
    Serial.printf("| Message arrived                       |\n");
    Serial.printf("| Command: %28s |\n", MQTT_command);
#endif

    if (length < MQTT_BUFFER_SIZE)
    {
        for (int i = 0; i < length; i++)
        {
            MQTT_msg_in[i] = (char)payload[i];
            MQTT_msg_in[i + 1] = '\0';
        }

#ifdef DEBUG
        Serial.printf("| Message: %28s |\n", MQTT_msg_in);
#endif
        /////////////////////////////////////////////////////
        // Message handling goes here...
        /////////////////////////////////////////////////////
        if (strcmp(MQTT_command, "/lights") == 0)
        {
            // DoLights(MQTT_msg_in);
        }
        else if (strcmp(MQTT_command, "/respond") == 0)
        {
            // DoAnswer(MQTT_msg_in);
        }
        else if (strcmp(MQTT_command, "/control") == 0)
        {
            // DoControl(MQTT_msg_in);
        }
        else
        {
#ifdef DEBUG
            Serial.printf("| Dunno Whatcha want...                 |\n");
#endif
        }
    }
    else
    {
#ifdef DEBUG
        Serial.printf("| But, it's TOO Bloody Big!             |\n");
#endif
    }
}

void MQTT_reconnect()
{
    // Loop until we're reconnected
    while (!MQTT_client.connected())
    {
        Serial.printf("| Attempting MQTT connection...         |\n");
        // Create a random client ID
        String clientId = MQTT_ClientName;
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (MQTT_client.connect(clientId.c_str()))
        {
            SSD1306_Static(" MQTT good ", 3);
            delay(500);
            SSD1306_Static("           ", 3);
            Serial.printf("| connected to %-24s |\n", MQTT_broker);
            Serial.printf("| My Name:  %-27s |\n", MQTT_ClientName);
            // Once connected, publish an announcement...
            char MQTT_statTopic_Device[100];
            strcpy(MQTT_statTopic_Device, MQTT_statTopic);
            strcat(MQTT_statTopic_Device, "/HELLO");
            // MQTT_client.publish(MQTT_statTopic_Device, "world");
            MQTT_client.publish(MQTT_statTopic_Device, WiFi_ssid);
            // ... and resubscribe
            MQTT_client.subscribe(MQTT_inTopic);
        }
        else
        {

            if (WiFi.status() != WL_CONNECTED)
            {
                SSD1306_Static("  no WiFi  ", 3);
                Serial.printf("| UH OH!!!  No WiFi!!!                  |\n");
            }
            else
            {
                SSD1306_Static("  no MQTT  ", 3);
            }

            Serial.printf("|                                       |\n");
            Serial.printf("| failed, rc=%d                         |\n", MQTT_client.state());
            Serial.printf("| trying again in 5 seconds             |\n");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void MQTT_beacon()
{
    /* Beacon signal published at set interval to indicate the device
   * is still powered up and actively connected to MQTT...
   * A keepalive of sorts
   * also updates state within MQTT so it can be captured for
   * indicator light elsewhere
   */
    char MQTT_teleTopic_Device[100];
    char WiFiSignal[5];
    strcpy(MQTT_teleTopic_Device, MQTT_teleTopic);
    strcat(MQTT_teleTopic_Device, "/beep");
    if (getTimer(beacon_timer, BEACON_INTERVAL))
    {
        sprintf(WiFiSignal, "%d dBm", WiFi_strength());

        // MQTT_client.publish(MQTT_teleTopic_Device, "boop");
        MQTT_client.publish(MQTT_teleTopic_Device, WiFiSignal);
        blinkLED();

#ifdef DEBUG
        Serial.printf("| Beacon sent                           |\n");
#endif
    }
}

void MQTT_Status(char const *Device, char const *Status) // Send status messages
{
    char MQTT_statTopic_Device[100];
    strcpy(MQTT_statTopic_Device, MQTT_statTopic);
    strcat(MQTT_statTopic_Device, "/");
    strcat(MQTT_statTopic_Device, Device);

#ifdef DEBUG
    Serial.printf("}- %16s = %-16s -{\n", Device, Status);
#endif
    MQTT_client.publish(MQTT_statTopic_Device, Status);
}

void MQTT_SendData(float Temp0, float Temp1)
{
    char MQTT_teleTopic_Device[100];
    strcpy(MQTT_teleTopic_Device, MQTT_teleTopic);
    strcat(MQTT_teleTopic_Device, "/");
    strcat(MQTT_teleTopic_Device, "SENSOR");

    char JSONstream[100];
    sprintf(JSONstream,
            "{\"DS18B20_1\":{\"Temperature\":%5.1f},\"DS18B20_2\":{\"Temperature\":%5.1f}}",
            Temp0, Temp1);
    MQTT_client.publish(MQTT_teleTopic_Device, JSONstream);
    
}