#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include "MFRC522.h"

const int RST_PIN = 22; // Reset pin
const int SS_PIN = 21; // Slave select pin
//==========================
//esp32     mfrc522
//21        SDA
//18        SCK
//23        MOSI
//21        MISO
//22        RST
//GND       GND
//3.3v      3.3V
//==========================
#define BUILTIN_LED 2
// Update these with values suitable for your network.
//const char *ssid = "PTS-2F";
//const char *pass = "PTS6662594";
const char *ssid = "alex9ufo";
const char *pass = "alex9981";

//const char *ssid = "WBR-2200";
//const char *pass = "0226452362";
//const char *ssid = "74170287";
//const char *pass = "24063173";
//const char *ssid =  "yourSSID";     // change according to your Network - cannot be longer than 32 characters!
//const char *pass =  "yourPASSWORD"; // change according to your Network


#define MQTTid              ""                           //id of this mqtt client
#define MQTTip              "broker.mqtt-dashboard.com"  //ip address or hostname of the mqtt broker
#define MQTTport            1883                         //port of the mqtt broker
#define MQTTuser            "alex9ufo"                   //username of this mqtt client
#define MQTTpsw             "alex1234"                   //password of this mqtt client
//#define MQTTuser          "your_username"              //username of this mqtt client
//#define MQTTpsw           "your_password"              //password of this mqtt client
#define MQTTpubQos          2                            //qos of publish (see README)
#define MQTTsubQos          1                            //qos of subscribe

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

//Variables
long lastMsg = 0;
String IDNo_buf="";    //client.publish("alex9ufo/outTopic/RFID/json"
char jsonChar1[100];

String json = "";     //client.publish("alex9ufo/led/led_status",
char jsonChar2[100];

bool Flash = false;  //true
bool Timer = false;  //true
bool Send = false;  //true
int Count= 0;
//=============================================================================
boolean pendingDisconnect = false;
void mqttConnectedCb(); // on connect callback
void mqttDisconnectedCb(); // on disconnect callback
void mqttDataCb(char* topic, byte* payload, unsigned int length); // on new message callback

WiFiClient wclient;
PubSubClient client(MQTTip, MQTTport, mqttDataCb, wclient);

//=============================================================================
String printHex(byte *buffer, byte bufferSize) {
      String id = "";
      for (byte i = 0; i < bufferSize; i++) {
        id += buffer[i] < 0x10 ? "0" : "";
        id += String(buffer[i], HEX);
      }
      return id;
    }
//=============================================================================
void mqttConnectedCb() {
  Serial.println("connected");
  
  // Once connected, publish an announcement...
  client.publish("alex9ufo/outTopic/RFID/json", jsonChar1, MQTTpubQos, true); // true means retain
  // Once connected, publish an announcement...
  client.publish("alex9ufo/led/led_status", jsonChar2, MQTTpubQos, true); // true means retain
  // ... and resubscribe
  client.subscribe("alex9ufo/inTopic/led/led_event", MQTTsubQos);

}
//=============================================================================
void mqttDisconnectedCb() {
  Serial.println("disconnected");
}
//=============================================================================
void mqttDataCb(char* topic, byte* payload, unsigned int length) {

  /*
  you can convert payload to a C string appending a null terminator to it;
  this is possible when the message (including protocol overhead) doesn't
  exceeds the MQTT_MAX_PACKET_SIZE defined in the library header.
  you can consider safe to do so when the length of topic plus the length of
  message doesn't exceeds 115 characters
  */
  char* message = (char *) payload;
  message[length] = 0;

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(message);
  String s = message;

  s.trim();
  // Switch on the LED if an 1 was received as first character
  if (s == "OFF") {
     digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
     // but actually the LED is on; this is because 
     Serial.println("Received OFF , Send LOW TO BuildIn_LED");
     Flash = false;
     Timer = false;
     json ="OFF";
     Send = true ;
    } // if (s == "OFF")
    
   if (s == "ON") {
     digitalWrite(BUILTIN_LED, HIGH);   // Turn the LED off (Note that HIGH is the voltage level
     // but actually the LED is on; this is because
     Serial.println("Received ON , Send HIGH TO BuildIn_LED");
     Flash = false; 
     Timer = false; 
     json ="ON";
     Send = true ;
   } //if (s == "ON")
   
   if (s == "TOGGLE") {
     digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));   // Turn the LED toggle 
     // but actually the LED is on; this is because
     Serial.println("Received TOGGLE , Send Toggle(H->L , L->H) TO BuildIn_LED");
     Flash = false; 
     Timer = false; 
     json ="TOGGLE";
     Send = true ;     
   } //if (s == "TOGGLE") 
   
   if (s == "FLASH") {
     digitalWrite(BUILTIN_LED, HIGH);   // Turn the LED off (Note that HIGH is the voltage level
     // but actually the LED is on; this is because
     Serial.println("Received FLASH , Flashing BuildIn_LED ");
     Flash = true;
     Timer = false;
     json ="FLASH";
     Send = true ;  
    } //if (message[0] == 'FLASH')
   
    if (s == "TIMER") {
     digitalWrite(BUILTIN_LED, HIGH);   // Turn the LED off (Note that HIGH is the voltage level
     // but actually the LED is on; this is because
     Serial.println("Received TIMER ,  BuildIn_LED ON 5 SEC");
     Flash = false;
     Timer = true;
     Count= 10;
     json ="TIMER";
     Send = true ; 
    } //if (message[0] == 'TIMER')
   
}
 
//======================================================
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}
//======================================================
void setup() {
  Serial.begin(115200);
  setup_wifi();
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.println(F("Booting...."));
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
  Serial.println(F("Ready!"));
  Serial.println(F("Control Build LED ON,OFF,FLASH,TOGGLE,TIMER...."));
  Serial.println(F("======================================================")); 
  Serial.println(F("Scan for Card and print UID:"));
}
//======================================================
void process_mqtt() {
  if (WiFi.status() == WL_CONNECTED) {
    if (client.connected()) {
      client.loop();
    } else {
    // client id, client username, client password, last will topic, last will qos, last will retain, last will message
      if (client.connect(MQTTid, MQTTuser, MQTTpsw, MQTTid "/status", 2, true, "0")) {
          pendingDisconnect = false;
          mqttConnectedCb();
      }
    }
  } else {
    if (client.connected())
      client.disconnect();
  }
  if (!client.connected() && !pendingDisconnect) {
    pendingDisconnect = true;
    mqttDisconnectedCb();
  }
}
//======================================================
void loop() {
  process_mqtt();
  long now = millis();
if (Flash)
  {
    digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
    delay(500);
  }
  if (Timer) 
  {
    digitalWrite(BUILTIN_LED, HIGH);
    delay(500);
    Count=Count-1;
    if (Count == 0 ){
       Timer=false;
       digitalWrite(BUILTIN_LED, LOW);
    }
      
  }
  if (Send) {
     // Convert JSON string to character array
     json.toCharArray(jsonChar2, json.length()+1);
     if  (client.connected()) {
              Serial.print("Publish message: ");
              Serial.println(json);
              // Publish JSON character array to MQTT topic
             client.publish("alex9ufo/led/led_status",jsonChar2);
         } 
     Send = false;    
   }
 
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) { // 如果出現新卡片就讀取卡片資料
     delay(100);
     String IDNo = printHex(mfrc522.uid.uidByte, mfrc522.uid.size);
     // Show some details of the PICC (that is: the tag/card)
     if ((IDNo != IDNo_buf) || (now - lastMsg > 5000)) {  //不同卡片 或是 等5秒
         lastMsg = now; 
         Serial.print(F("Card UID:"));
         Serial.println(IDNo);
         //Serial.println(IDNo_buf);
    
         IDNo_buf="";
         IDNo_buf=IDNo;
         // Convert data to JSON string 
         String json1 =
         "{\"data\":{"
         "\"RFID_No\": \"" + IDNo + "\"}"
         "}";
         // Convert JSON string to character array
         json1.toCharArray(jsonChar1, json1.length()+1);
    
         if  (client.connected()) {
              Serial.print("Publish message: ");
              Serial.println(json1);
              // Publish JSON character array to MQTT topic
             client.publish("alex9ufo/outTopic/RFID/json",jsonChar1);
         } 
      } // if ((IDNo != IDNo_buf) || (now - lastMsg > 5000))
  }  // if (mfrc522.PICC_IsNewCardPresent()

}   //Loop
//======================================================
