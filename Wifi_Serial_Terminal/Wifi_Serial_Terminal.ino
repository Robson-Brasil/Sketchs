//CODED BY LANZ CIMENI
//THIS CODE WAS FINISHED ON JULY 30 2021
//PLEASE SUBSCRIBE TO MY CHANNEL -> https://www.youtube.com/channel/UCMqbxIEpuyX_OJrA4BPbxXQ

//Watch my 2 videos on BLUETOOTH SERIAL TERMINAL video here!
//BME280 Sensor Reading -> https://www.youtube.com/watch?v=NdaeR-9k6NQ
//RGB Led Control -> https://www.youtube.com/watch?v=pfpJEaCpy-Y

//THANK YOU!

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//No Library? Click this -> https://github.com/ekstrand/ESP8266wifi

const char *ssid = "LANZ"; //ESP Access Point
const char *password = "LANZ12345"; //ESP Password
//Please connect to LANZ
//Please take note that some ESP Wifi are hidden
//Please add/connect the ESP Wifi Manually through your phone
//Please make sure your password are 8 or more characters long

WiFiClient client;
IPAddress ip (192, 168, 19, 2);
IPAddress netmask (255, 255, 255, 0);
const int port = 5210;
WiFiServer server(port);

void setup() {
  // THIS CODE WAS FINISHED ON JULY 30 2021
  Serial.begin(9600);
  delay(1000); //Wait to run ESP properly

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(SSID, PASSWORD);

  Serial.println("Wifi Details:");
  Serial.println(ssid);
  Serial.println(password);
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  if (!client.connected()) {
    Serial.println("Error! Please make sure to connect to LANZ");
    Serial.println("Please manually add LANZ to your phone");
    Serial.println();
    //Please take note that this message will repeatedly show until
    //you add your device to the app (Wifi Serial Terminal)
    //After the device is connected then this message will stop

    delay(2000);
    client = server.available();
    return;
  }
  if (client.available() >= 0) {
    Serial.println("This is a test run");
    Serial.println("Congratulations! Device Connected!");
    Serial.println();
    //Commands with Serial.println show on pc serial monitor

    client.println("This is a test run");
    client.println("Congratulations! Device Connected!");
    //Commands with client.println show in Wifi Serial Terminal App

    delay(2000);
  }
}

//PEACE OUT!
