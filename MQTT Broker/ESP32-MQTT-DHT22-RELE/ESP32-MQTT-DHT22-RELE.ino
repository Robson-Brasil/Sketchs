/**********************************************************************************
//IoT - Automação Residencial
//Dispositivo : ESP32
//MQTT
//Red-Node
//Nora
//Google Assistant
//Autor : Robson Brasil
//Versão : 69
//Última Modificação : 21/06/2022
 **********************************************************************************/

#include "DHT.h"
#include <WiFi.h>
#include <PubSubClient.h>

extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}
#include <AsyncMqttClient.h>

#define WIFI_SSID "RVR 2,4GHz"
#define WIFI_PASSWORD "RodrigoValRobson2021"

// Raspberry Pi Mosquitto MQTT Broker
#define MQTT_HOST IPAddress(192, 168, 15, 30)
const char*  clientID = "ESP32ClientGoogleAssistant";  // Client ID Obs.: Deve ser único

// For a cloud MQTT broker, type the domain name
//#define MQTT_HOST "example.com"
#define MQTT_PORT 1883

//Tópicos do Subscribe
#define sub1 "ESP32-MinhaCasa/QuartoRobson/LigarInterruptor1"   // Ligados ao Nora
#define sub2 "ESP32-MinhaCasa/QuartoRobson/LigarInterruptor2"   // Ligados ao Nora
#define sub3 "ESP32-MinhaCasa/QuartoRobson/LigarInterruptor3"   // Ligados ao Nora
#define sub4 "ESP32-MinhaCasa/QuartoRobson/LigarInterruptor4"   // Ligados ao Nora
#define sub5 "ESP32-MinhaCasa/QuartoRobson/LigarInterruptor5"   // Ligados ao Nora
#define sub6 "ESP32-MinhaCasa/QuartoRobson/LigarInterruptor6"   // Somente por MQTT
#define sub7 "ESP32-MinhaCasa/QuartoRobson/LigarInterruptor7"   // Somente por MQTT
#define sub8 "ESP32-MinhaCasa/QuartoRobson/LigarInterruptor8"   // Somente por MQTT

// Temperature MQTT Topics
#define pub9 "ESP32-MinhaCasa/QuartoRobson/Temperatura"
#define pub10  "ESP32-MinhaCasa/QuartoRobson/Umidade"

// Digital pin connected to the DHT sensor
#define DHTPIN 16

 // Relays
#define RelayPin1 23  //D23 Ligados ao Nora
#define RelayPin2 22  //D22 Ligados ao Nora
#define RelayPin3 21  //D21 Ligados ao Nora
#define RelayPin4 19  //D19 Ligados ao Nora
#define RelayPin5 18  //D18 Ligados ao Nora
#define RelayPin6  5  //D5  Somente por MQTT
#define RelayPin7 25  //D25 Somente por MQTT
#define RelayPin8 26  //D26 Somente por MQTT
  
// Uncomment whatever DHT sensor type you're using
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)   

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Variables to hold sensor readings
float temp;
float hum;

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 10000;        // Interval at which to publish sensor readings

//  WiFiClient espClient;
//  PubSubClient client(espClient);

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  uint16_t packetIdSub1 = mqttClient.subscribe(sub1, 2);
  uint16_t packetIdSub2 = mqttClient.subscribe(sub2, 2);
  Serial.printf("Subscribing at QoS 2, packetId: ");
  Serial.println(packetIdSub1);
  Serial.println(packetIdSub2);

   // Publish an MQTT message on topic ESP32-MinhaCasa/QuartoRobson/Temperatura
  mqttClient.publish(sub1, 0, true);
  Serial.println("Publishing at QoS 0");
  uint16_t packetIdPub3 = mqttClient.publish(sub1, 0, true);   
  Serial.print("Publishing at QoS 1, packetId: ");
  Serial.println(packetIdPub3);                         

  Serial.print("Subscribing at QoS 2, packetId: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}
void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClient, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total, unsigned int length) {

  Serial.print("Mensagem Chegou [");
  Serial.print(topic);
  Serial.print("] '");
  String data = "";

  if (sub1) {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
      data += (char)payload[i];
  }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
    digitalWrite(RelayPin1, HIGH);  // Turn the LED on (Note that LOW is the voltage level
  } 
    else {
    digitalWrite(RelayPin1, LOW);  // Turn the LED off by making the voltage HIGH
    }
  } 
    else if (sub2) {
    for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
    digitalWrite(RelayPin2, HIGH);  // Turn the LED on (Note that LOW is the voltage level
  } 
    else {
    digitalWrite(RelayPin2, LOW);  // Turn the LED off by making the voltage HIGH
    }
  } 
    else if (strstr(topic, sub3)) {
    for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
    digitalWrite(RelayPin3, HIGH);  // Turn the LED on (Note that LOW is the voltage level
    } 
    else {
    digitalWrite(RelayPin3, LOW);  // Turn the LED off by making the voltage HIGH
    }
  } 
    else if (strstr(topic, sub4)) {
    for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
    digitalWrite(RelayPin4, HIGH);  // Turn the LED on (Note that LOW is the voltage level
  } 
    else {
    digitalWrite(RelayPin4, LOW);  // Turn the LED off by making the voltage HIGH
    }
  } 
    else if (strstr(topic, sub5)) {
    for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
    digitalWrite(RelayPin5, HIGH);  // Turn the LED on (Note that LOW is the voltage level
  } 
    else {
    digitalWrite(RelayPin5, LOW);  // Turn the LED off by making the voltage HIGH
    }
  } 
    else if (strstr(topic, sub6)) {
    for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
    digitalWrite(RelayPin6, HIGH);  // Turn the LED on (Note that LOW is the voltage level
  } 
    else {
    digitalWrite(RelayPin6, LOW);  // Turn the LED off by making the voltage HIGH
    }
  } 
    else if (strstr(topic, sub7)) {
    for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
    digitalWrite(RelayPin7, HIGH);  // Turn the LED on (Note that LOW is the voltage level
  } 
    else {
    digitalWrite(RelayPin7, LOW);  // Turn the LED off by making the voltage HIGH
    }
  } 
    else if (strstr(topic, sub8)) {
    for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(RelayPin8, HIGH);  // Turn the LED on (Note that LOW is the voltage level
  } 
    else {
    digitalWrite(RelayPin8, LOW);  // Turn the LED off by making the voltage HIGH
    }
  } 
    else {
    Serial.println("Não Inscrito no Tópico do MQTT");
    }
}

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  dht.begin();
  
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  // If your broker requires authentication (username and password), set them below
  mqttClient.setCredentials("Robson Brasil", "LoboAlfa");
  connectToWifi();

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(RelayPin5, OUTPUT);
  pinMode(RelayPin6, OUTPUT);
  pinMode(RelayPin7, OUTPUT);
  pinMode(RelayPin8, OUTPUT);

    //Durante a partida, todos os Relés iniciam desligados
  digitalWrite(RelayPin1, HIGH);
  digitalWrite(RelayPin2, HIGH);
  digitalWrite(RelayPin3, HIGH);
  digitalWrite(RelayPin4, HIGH);
  digitalWrite(RelayPin5, HIGH);
  digitalWrite(RelayPin6, HIGH);
  digitalWrite(RelayPin7, HIGH);
  digitalWrite(RelayPin8, HIGH);

}

void loop() {
  unsigned long currentMillis = millis();
  // Every X number of seconds (interval = 10 seconds) 
  // it publishes a new MQTT message
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;
    // New DHT sensor readings
    hum = dht.readHumidity();
    // Read temperature as Celsius (the default)
    temp = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //temp = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(temp) || isnan(hum)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    // Publish an MQTT message on topic ESP32-MinhaCasa/QuartoRobson/Temperatura
    uint16_t packetIdPub1 = mqttClient.publish(pub9, 1, true, String(temp).c_str());                            
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i", pub9, packetIdPub1);
    Serial.printf("Message: %.2f \n", temp);

    // Publish an MQTT message on topic ESP32-MinhaCasa/QuartoRobson/Umidade
    uint16_t packetIdPub2 = mqttClient.publish(pub10, 1, true, String(hum).c_str());                            
    Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", pub10, packetIdPub2);
    Serial.printf("Message: %.2f \n", hum);
  }
}
