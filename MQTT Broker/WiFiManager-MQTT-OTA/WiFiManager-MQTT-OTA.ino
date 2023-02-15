 * Define Relay pins as examples to send/receive MQTT messages
 * **/
#define RELAY_PIN_1 12
#define RELAY_PIN_2 14
 
/** 
 * Perform OTAs for ESP8266 / ESP32 Elegantly with password protection! 
 * https://github.com/ayushsharma82/AsyncElegantOTA
 * **/
#include <AsyncElegantOTA.h>
const char *FOTA_USERNAME = "un";
const char *FOTA_PASSWORD = "pw";
 
/** 
 * Add MQTT client here` PubSubClient V2.8 by Nick O'Leary
 * https://github.com/knolleary/pubsubclient 
*/
#include <WiFiClient.h>
#include <PubSubClient.h>
#define MSG_BUFFER_SIZE (1024)
char msg[MSG_BUFFER_SIZE];
 
/** Taiwan No. 1 Free MQTT Server!! **/
const char *mqtt_server = "mqttgo.io";
const int mqtt_port = 1883;
 
/****************************************************************************************************************************
  Async_AutoConnect_ESP32_minimal.ino
  For ESP8266 / ESP32 boards
  Built by Khoi Hoang https://github.com/khoih-prog/ESPAsync_WiFiManager
  Licensed under MIT license
 *****************************************************************************************************************************/
#if !(defined(ESP32))
#error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif
#include <ESPAsync_WiFiManager.h> 
AsyncWebServer webServer(80);
 
//Start DNS server
DNSServer dnsServer;
 
//MQTT - Start WiFi client and connect to MQTT server
WiFiClient espClient;
PubSubClient mqtt_client(espClient);
 
//Define device name
String DEVICE_NAME = "Dual_Relay_Switch";
String home_page_message = "";
 
void setup()
{
    Serial.begin(115200);
    pinMode(RELAY_PIN_1, OUTPUT);
    pinMode(RELAY_PIN_2, OUTPUT);
 
    while (!Serial)
        ;
    delay(200);
    Serial.print("\nAsyncWifimanager started on " + String(ARDUINO_BOARD) + "\n");
 
    //Initialize ESPAsyncWifimanager instance and assign Wi-Fi Client name "AsyncAutoConnect"!
    //This name and IP address can be checked from the router
    ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, &dnsServer, "Dual_Relay_Switch");
 
    //####### RESET SAVED WIFI SETTINGS #############
    //ESPAsync_wifiManager.resetSettings();   
 
    //ESPAsync_wifiManager.setAPStaticIPConfig(IPAddress(192, 168, 132, 1), IPAddress(192, 168, 132, 1), IPAddress(255, 255, 255, 0));
    //ESPAsync_wifiManager.autoConnect("Stonez_ESP32S", "AP-NAME", "AP-PASSWORD");
    
    ESPAsync_wifiManager.autoConnect("Dual_Switch_ESP32S");
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.print(DEVICE_NAME);
        Serial.print(" is on Local IP: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println(ESPAsync_wifiManager.getStatus(WiFi.status()));
    }
 
    //Setup home page access content when visite
    home_page_message = "<!DOCTYPE html><html><head><title>" + DEVICE_NAME 
        + "</title></head><body><p><h2> Hi! This is " + DEVICE_NAME + "</h2>"
        + "To update firmware, <a href='/update'>Click here!!</a><br/><span>Username & Password required!</span></p><body></html>";
 
    webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(200, "text/html", home_page_message); });
 
    //AsyncElegantOTA.begin(&webServer); // Start ElegantOTA WITHOUT username & password
    AsyncElegantOTA.begin(&webServer, FOTA_USERNAME, FOTA_PASSWORD); // Start ElegantOTA with username & password
    webServer.begin();
    Serial.println("AsyncElegantOTA server started @URL/update");
 
    //MQTT starts here!
    mqtt_client.setServer(mqtt_server, mqtt_port);
    mqtt_client.setCallback(callback);
}
 
/**
//When MQTT lost connection, this function will be called to reconnect MQTT//
 * Modified to accept multiple MQTT topics 
 * https://www.baldengineer.com/multiple-mqtt-topics-pubsubclient.html
 */
void callback(char *topic, byte *payload, unsigned int length)
{
    //Print message received
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
 
    //Process multiple topics here//
    payload[length] = '\0';
    String message = (char*)payload;
    
    if (strcmp(topic, "studio/humd_switch") == 0)
    {
      if(message == "true"){
        digitalWrite(RELAY_PIN_1, HIGH);
        Serial.println("humd_switch true");
      }
      if(message == "false"){
        digitalWrite(RELAY_PIN_1, LOW);
        Serial.println("humd_switch false");
      }
    }
    if (strcmp(topic, "studio/humd_switch2") == 0)
    {
      if(message == "true"){
        digitalWrite(RELAY_PIN_2, HIGH);
        Serial.println("humd_switch2 true");
      }
      if(message == "false"){
        digitalWrite(RELAY_PIN_2, LOW);
        Serial.println("humd_switch2 false");
      }
    }
}
 
//============= MQTT ===================
//When MQTT lost connection, this function will be called to reconnect MQTT//
void reconnect(){
    //Loop while MQTT connected
    while (!mqtt_client.connected())
    {
        Serial.print("Attempting MQTT connection... ");
        // Create a random client ID
        String clientId = "Stonez_ESP32Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (mqtt_client.connect(clientId.c_str()))
        {
            Serial.print("connected to ");
            Serial.print(mqtt_server);
            // Once connected, publish an announcement...
            mqtt_client.publish("outTopic", "Hello world");
            // ... and resubscribe
            mqtt_client.subscribe("studio/humd_switch");
            mqtt_client.subscribe("studio/humd_switch2");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}
void loop() {
    //============= MQTT ===================
    //if can't connect to MQTT server, then re-connect
    if (!mqtt_client.connected())
        { reconnect(); }
    //constantly check MQTT to see for messages sending/receiving
    mqtt_client.loop();
 
    /**
     * Write your own code here....
     * 
     */
}