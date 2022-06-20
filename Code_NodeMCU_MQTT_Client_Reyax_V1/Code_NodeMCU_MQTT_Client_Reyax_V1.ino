/**********************************************************************************
 *  TITLE: ESP8266(NodeMCU) MQTT(Reyax RYC1001) + Manual(Push Button) control 4-Relays with feedback
 *  Click on the following links to learn more. 
 *  YouTube Video: https://youtu.be/RXRACfHTw6M
 *  Related Blog : https://iotcircuithub.com/esp8266-projects/
 *  by Subhajit (Tech StudyCell)
 *  Add the following 3rd party board manager under: "File -> Preferences -> Additional Boards Manager URLs"
 *  https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
 *  
 *  Download Board ESP8266: https://github.com/esp8266/Arduino
 *  Download the libraries
 *  PubSubClient Library: https://github.com/knolleary/pubsubclient/releases/tag/v2.8
 *  Install the libraries at Arduino IDE -> Sketch - Include Library -> Add Zip Library
 **********************************************************************************/

#include <WiFi.h>
#include <PubSubClient.h>

// Relays
#define RelayPin1 23  //D1
#define RelayPin2 22  //D2
#define RelayPin3 21  //D5
#define RelayPin4 19  //D6

// Switches
#define SwitchPin1 12  //SD3
#define SwitchPin2 13   //D3 
#define SwitchPin3 14  //D7
#define SwitchPin4 15   //RX

//WiFi Status LED
#define wifiLed    0   //D0

int toggleState_1 = 1; //Define integer to remember the toggle state for relay 1
int toggleState_2 = 1; //Define integer to remember the toggle state for relay 2
int toggleState_3 = 1; //Define integer to remember the toggle state for relay 3
int toggleState_4 = 1; //Define integer to remember the toggle state for relay 4

// Update these with values suitable for your network.

const char* ssid = "RVR 2,4GHz"; //WiFI Name
const char* password = "RodrigoValRobson2021"; //WiFi Password
const char* mqttServer = "broker.hivemq.com";
int mqttPort = 1883;
const char* mqttUserName = ""; // MQTT username
const char* mqttPwd = ""; // MQTT password
const char* clientID = "ESP32ClientGoogleAssistant"; // client id

#define sub1 "ESP32-MinhaCasaQuartoRobson/Lampada/LigarLampadaQuartoRobson"
#define sub2 "ESP32-MinhaCasaQuartoKinha/Lampada/LigarLampadaQuartoKinha"
#define sub3 "ESP32-MinhaCasaQuartoVal/Lampada/LigarLampadaQuartoVal"
#define sub4 "ESP32-MinhaCasaCozinha/Lampada/LigarLampadaCozinha"

#define pub1 "ESP32-MinhaCasaQuartoRobson/Lampada/LigarLampadaQuartoRobson"
#define pub2 "ESP32-MinhaCasaQuartoKinha/Lampada/LigarLampadaQuartoKinha"
#define pub3 "ESP32-MinhaCasaQuartoVal/Lampada/LigarLampadaQuartoVal"
#define pub4 "ESP32-MinhaCasaCozinha/Lampada/LigarLampadaCozinha"

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (80)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
 delay(10);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}

void reconnect() {
 while (!client.connected()) {
 if (client.connect(clientID, mqttUserName, mqttPwd)) {
 Serial.println("MQTT connected");
      // ... and resubscribe
      client.subscribe(sub1);
      client.subscribe(sub2);
      client.subscribe(sub3);
      client.subscribe(sub4);
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  if (strstr(topic, sub1))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(RelayPin1, LOW);   // Turn the LED on (Note that LOW is the voltage level
      toggleState_1 = 0;
      client.publish(pub1, "0");
    } else {
      digitalWrite(RelayPin1, HIGH);  // Turn the LED off by making the voltage HIGH
      toggleState_1 = 1;
      client.publish(pub1, "1");
    }    
  }

  else if ( strstr(topic, sub2))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(RelayPin2, LOW);   // Turn the LED on (Note that LOW is the voltage level
      toggleState_2 = 0;
      client.publish(pub2, "0");
    } else {
      digitalWrite(RelayPin2, HIGH);  // Turn the LED off by making the voltage HIGH
      toggleState_2 = 1;
      client.publish(pub2, "1");
    }
  }
  else if ( strstr(topic, sub3))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(RelayPin3, LOW);   // Turn the LED on (Note that LOW is the voltage level
      toggleState_3 = 0;
      client.publish(pub3, "0");
    } else {
      digitalWrite(RelayPin3, HIGH);  // Turn the LED off by making the voltage HIGH
      toggleState_3 = 1;
      client.publish(pub3, "1");
    }
  }
  else if ( strstr(topic, sub4))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(RelayPin4, LOW);   // Turn the LED on (Note that LOW is the voltage level
      toggleState_4 = 0;
      client.publish(pub4, "0");
    } else {
      digitalWrite(RelayPin4, HIGH);  // Turn the LED off by making the voltage HIGH
      toggleState_4 = 1;
      client.publish(pub4, "1");
    }
  }
  else
  {
    Serial.println("unsubscribed topic");
  }
}

void manual_control(){
    //Manual Switch Control
    if (digitalRead(SwitchPin1) == LOW){
      delay(200);
      if(toggleState_1 == 1){
        digitalWrite(RelayPin1, LOW); // turn on relay 1
        toggleState_1 = 0;
        client.publish(pub1, "0");
        Serial.println("Device1 ON");
        }
      else{
        digitalWrite(RelayPin1, HIGH); // turn off relay 1
        toggleState_1 = 1;
        client.publish(pub1, "1");
        Serial.println("Device1 OFF");
        }
     }
    else if (digitalRead(SwitchPin2) == LOW){
      delay(200);
      if(toggleState_2 == 1){
        digitalWrite(RelayPin2, LOW); // turn on relay 2
        toggleState_2 = 0;
        client.publish(pub2, "0");
        Serial.println("Device2 ON");
        }
      else{
        digitalWrite(RelayPin2, HIGH); // turn off relay 2
        toggleState_2 = 1;
        client.publish(pub2, "1");
        Serial.println("Device2 OFF");
        }
    }
    else if (digitalRead(SwitchPin3) == LOW){
      delay(200);
      if(toggleState_3 == 1){
        digitalWrite(RelayPin3, LOW); // turn on relay 3
        toggleState_3 = 0;
        client.publish(pub3, "0");
        Serial.println("Device3 ON");
        }
      else{
        digitalWrite(RelayPin3, HIGH); // turn off relay 3
        toggleState_3 = 1;
        client.publish(pub3, "1");
        Serial.println("Device3 OFF");
        }
    }
    else if (digitalRead(SwitchPin4) == LOW){
      delay(200);
      if(toggleState_4 == 1){
        digitalWrite(RelayPin4, LOW); // turn on relay 4
        toggleState_4 = 0;
        client.publish(pub4, "0");
        Serial.println("Device4 ON");
        }
      else{
        digitalWrite(RelayPin4, HIGH); // turn off relay 4
        toggleState_4 = 1;
        client.publish(pub4, "1");
        Serial.println("Device4 OFF");
        }
    }
    delay(100);
}

void setup() {
  Serial.begin(115200);
    
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  pinMode(wifiLed, OUTPUT);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, HIGH);
  digitalWrite(RelayPin2, HIGH);
  digitalWrite(RelayPin3, HIGH);
  digitalWrite(RelayPin4, HIGH);
  
  //During Starting WiFi LED should TURN OFF
  digitalWrite(wifiLed, HIGH);

  setup_wifi();
   client.setServer(mqttServer, 1883);
   client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    digitalWrite(wifiLed, HIGH);
    reconnect();
  }
  else{
    digitalWrite(wifiLed, LOW);
    manual_control();
  }
  client.loop();
}
