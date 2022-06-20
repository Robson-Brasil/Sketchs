//Viral Science www.viralsciencecreativity.com www.youtube.com/c/viralscience
//ESP32 BLYNK + BLUETOOTH + MANUAL Push Button HOME AUTOMATION SYSTEM

#include <BlynkSimpleEsp32.h>
#include "BluetoothSerial.h" 

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run
`make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

BlynkTimer timer;

#define AUTH "XXXXXXXXXXXXXXXXXXXXXXXXX"                 // Enter Authentication Sent by Blynk  
#define WIFI_SSID "XXXXXXX"             //Enter Your Wifi Name
#define WIFI_PASS "XXXXXXX"         //Enter Your wifi Password

// define the GPIO connected with Relays and switches
#define RelayPin1 23  //D23
#define RelayPin2 22  //D22
#define RelayPin3 21  //D21
#define RelayPin4 19  //D19
#define RelayPin5 18  //D18
#define RelayPin6 5   //D5
#define RelayPin7 25  //D25
#define RelayPin8 26  //D26

#define SwitchPin1 13  //D13
#define SwitchPin2 12  //D12
#define SwitchPin3 14  //D14
#define SwitchPin4 27  //D27
#define SwitchPin5 33  //D33
#define SwitchPin6 32  //D32
#define SwitchPin7 15  //D15
#define SwitchPin8 4   //D4

#define wifiLed    2   //D2

#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3 
#define VPIN_BUTTON_4    V4
#define VPIN_BUTTON_5    V5 
#define VPIN_BUTTON_6    V6
#define VPIN_BUTTON_7    V7 
#define VPIN_BUTTON_8    V8

int toggleState_1 = 0; //Define integer to remember the toggle state for relay 1
int toggleState_2 = 0; //Define integer to remember the toggle state for relay 2
int toggleState_3 = 0; //Define integer to remember the toggle state for relay 3
int toggleState_4 = 0; //Define integer to remember the toggle state for relay 4
int toggleState_5 = 0; //Define integer to remember the toggle state for relay 5
int toggleState_6 = 0; //Define integer to remember the toggle state for relay 6
int toggleState_7 = 0; //Define integer to remember the toggle state for relay 7
int toggleState_8 = 0; //Define integer to remember the toggle state for relay 8

int wifiFlag = 0;
char bt_data; // variable for storing bluetooth data 

// When App button is pushed - switch the state

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, toggleState_2);
}

BLYNK_WRITE(VPIN_BUTTON_3) {
  toggleState_3 = param.asInt();
  digitalWrite(RelayPin3, toggleState_3);
}

BLYNK_WRITE(VPIN_BUTTON_4) {
  toggleState_4 = param.asInt();
  digitalWrite(RelayPin4, toggleState_4);
}

BLYNK_WRITE(VPIN_BUTTON_5) {
  toggleState_5 = param.asInt();
  digitalWrite(RelayPin5, toggleState_5);
}

BLYNK_WRITE(VPIN_BUTTON_6) {
  toggleState_6 = param.asInt();
  digitalWrite(RelayPin6, toggleState_6);
}

BLYNK_WRITE(VPIN_BUTTON_7) {
  toggleState_7 = param.asInt();
  digitalWrite(RelayPin7, toggleState_7);
}

BLYNK_WRITE(VPIN_BUTTON_8) {
  toggleState_8 = param.asInt();
  digitalWrite(RelayPin8, toggleState_8);
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
  Blynk.syncVirtual(VPIN_BUTTON_3);
  Blynk.syncVirtual(VPIN_BUTTON_4);
  Blynk.syncVirtual(VPIN_BUTTON_5);
  Blynk.syncVirtual(VPIN_BUTTON_6);
  Blynk.syncVirtual(VPIN_BUTTON_7);
  Blynk.syncVirtual(VPIN_BUTTON_8);
}

void relayOnOff(int relay){

    switch(relay){
      case 1: 
             if(toggleState_1 == 1){
              digitalWrite(RelayPin1, LOW); // turn on relay 1
              toggleState_1 = 0;
              Serial.println("Device1 ON");
              }
             else{
              digitalWrite(RelayPin1, HIGH); // turn off relay 1
              toggleState_1 = 1;
              Serial.println("Device1 OFF");
              }
             delay(100);
      break;
      case 2: 
             if(toggleState_2 == 1){
              digitalWrite(RelayPin2, LOW); // turn on relay 2
              toggleState_2 = 0;
              Serial.println("Device2 ON");
              }
             else{
              digitalWrite(RelayPin2, HIGH); // turn off relay 2
              toggleState_2 = 1;
              Serial.println("Device2 OFF");
              }
             delay(100);
      break;
      case 3: 
             if(toggleState_3 == 1){
              digitalWrite(RelayPin3, LOW); // turn on relay 3
              toggleState_3 = 0;
              Serial.println("Device3 ON");
              }
             else{
              digitalWrite(RelayPin3, HIGH); // turn off relay 3
              toggleState_3 = 1;
              Serial.println("Device3 OFF");
              }
             delay(100);
      break;
      case 4: 
             if(toggleState_4 == 1){
              digitalWrite(RelayPin4, LOW); // turn on relay 4
              toggleState_4 = 0;
              Serial.println("Device4 ON");
              }
             else{
              digitalWrite(RelayPin4, HIGH); // turn off relay 4
              toggleState_4 = 1;
              Serial.println("Device4 OFF");
              }
             delay(100);
      break;
      case 5: 
             if(toggleState_5 == 1){
              digitalWrite(RelayPin5, LOW); // turn on relay 5
              toggleState_5 = 0;
              Serial.println("Device5 ON");
              }
             else{
              digitalWrite(RelayPin5, HIGH); // turn off relay 5
              toggleState_5 = 1;
              Serial.println("Device5 OFF");
              }
             delay(100);
      break;
      case 6: 
             if(toggleState_6 == 1){
              digitalWrite(RelayPin6, LOW); // turn on relay 6
              toggleState_6 = 0;
              Serial.println("Device6 ON");
              }
             else{
              digitalWrite(RelayPin6, HIGH); // turn off relay 6
              toggleState_6 = 1;
              Serial.println("Device6 OFF");
              }
             delay(100);
      break;
      case 7: 
             if(toggleState_7 == 1){
              digitalWrite(RelayPin7, LOW); // turn on relay 7
              toggleState_7 = 0;
              Serial.println("Device7 ON");
              }
             else{
              digitalWrite(RelayPin7, HIGH); // turn off relay 7
              toggleState_7 = 1;
              Serial.println("Device7 OFF");
              }
             delay(100);
      break;
      case 8: 
             if(toggleState_8 == 1){
              digitalWrite(RelayPin8, LOW); // turn on relay 8
              toggleState_8 = 0;
              Serial.println("Device8 ON");
              }
             else{
              digitalWrite(RelayPin8, HIGH); // turn off relay 8
              toggleState_8 = 1;
              Serial.println("Device8 OFF");
              }
             delay(100);
      break;
      default : break;      
      }  
}

void with_internet(){
    //Manual Switch Control
    if (digitalRead(SwitchPin1) == LOW){
      delay(200);
      relayOnOff(1); 
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);   // Update Button Widget  
    }
    else if (digitalRead(SwitchPin2) == LOW){
      delay(200);
      relayOnOff(2);      
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);   // Update Button Widget
    }
    else if (digitalRead(SwitchPin3) == LOW){
      delay(200);
      relayOnOff(3);
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);   // Update Button Widget
    }
    else if (digitalRead(SwitchPin4) == LOW){
      delay(200);
      relayOnOff(4);
      Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);   // Update Button Widget
    }
    else if (digitalRead(SwitchPin5) == LOW){
      delay(200);
      relayOnOff(5); 
      Blynk.virtualWrite(VPIN_BUTTON_5, toggleState_5);   // Update Button Widget  
    }
    else if (digitalRead(SwitchPin6) == LOW){
      delay(200);
      relayOnOff(6);      
      Blynk.virtualWrite(VPIN_BUTTON_6, toggleState_6);   // Update Button Widget
    }
    else if (digitalRead(SwitchPin7) == LOW){
      delay(200);
      relayOnOff(7);
      Blynk.virtualWrite(VPIN_BUTTON_7, toggleState_7);   // Update Button Widget
    }
    else if (digitalRead(SwitchPin8) == LOW){
      delay(200);
      relayOnOff(8);
      Blynk.virtualWrite(VPIN_BUTTON_8, toggleState_8);   // Update Button Widget
    }
}
void without_internet(){
    //Manual Switch Control
    if (digitalRead(SwitchPin1) == LOW){
      delay(200);
      relayOnOff(1);      
    }
    else if (digitalRead(SwitchPin2) == LOW){
      delay(200);
      relayOnOff(2);
    }
    else if (digitalRead(SwitchPin3) == LOW){
      delay(200);
      relayOnOff(3);
    }
    else if (digitalRead(SwitchPin4) == LOW){
      delay(200);
      relayOnOff(4);
    }
    else if (digitalRead(SwitchPin5) == LOW){
      delay(200);
      relayOnOff(5);      
    }
    else if (digitalRead(SwitchPin6) == LOW){
      delay(200);
      relayOnOff(6);
    }
    else if (digitalRead(SwitchPin7) == LOW){
      delay(200);
      relayOnOff(7);
    }
    else if (digitalRead(SwitchPin8) == LOW){
      delay(200);
      relayOnOff(8);
    }
}

void all_Switch_ON(){
  digitalWrite(RelayPin1, LOW); toggleState_1 = 0; delay(100);
  digitalWrite(RelayPin2, LOW); toggleState_2 = 0; delay(100);
  digitalWrite(RelayPin3, LOW); toggleState_3 = 0; delay(100);
  digitalWrite(RelayPin4, LOW); toggleState_4 = 0; delay(100);
  digitalWrite(RelayPin5, LOW); toggleState_5 = 0; delay(100);
  digitalWrite(RelayPin6, LOW); toggleState_6 = 0; delay(100);
  digitalWrite(RelayPin7, LOW); toggleState_7 = 0; delay(100);
  digitalWrite(RelayPin8, LOW); toggleState_8 = 0; delay(100);
}

void all_Switch_OFF(){
  digitalWrite(RelayPin1, HIGH); toggleState_1 = 1; delay(100);
  digitalWrite(RelayPin2, HIGH); toggleState_2 = 1; delay(100);
  digitalWrite(RelayPin3, HIGH); toggleState_3 = 1; delay(100);
  digitalWrite(RelayPin4, HIGH); toggleState_4 = 1; delay(100);
  digitalWrite(RelayPin5, HIGH); toggleState_5 = 1; delay(100);
  digitalWrite(RelayPin6, HIGH); toggleState_6 = 1; delay(100);
  digitalWrite(RelayPin7, HIGH); toggleState_7 = 1; delay(100);
  digitalWrite(RelayPin8, HIGH); toggleState_8 = 1; delay(100);
}

void Bluetooth_handle()
{
  bt_data = SerialBT.read();
  Serial.println(bt_data);
  delay(20);

  switch(bt_data)
      {
        case 'a': digitalWrite(RelayPin1, LOW);  toggleState_1 = 0; break; // if 'A' received Turn on Relay1
        case 'A': digitalWrite(RelayPin1, HIGH); toggleState_1 = 1; break; // if 'a' received Turn off Relay1
        case 'b': digitalWrite(RelayPin2, LOW);  toggleState_2 = 0; break; // if 'B' received Turn on Relay2
        case 'B': digitalWrite(RelayPin2, HIGH); toggleState_2 = 1; break; // if 'b' received Turn off Relay2
        case 'c': digitalWrite(RelayPin3, LOW);  toggleState_3 = 0; break; // if 'C' received Turn on Relay3
        case 'C': digitalWrite(RelayPin3, HIGH); toggleState_3 = 1; break; // if 'c' received Turn off Relay3
        case 'd': digitalWrite(RelayPin4, LOW);  toggleState_4 = 0; break; // if 'D' received Turn on Relay4
        case 'D': digitalWrite(RelayPin4, HIGH); toggleState_4 = 1; break; // if 'd' received Turn off Relay4
        case 'e': digitalWrite(RelayPin5, LOW);  toggleState_5 = 0; break; // if 'E' received Turn on Relay5
        case 'E': digitalWrite(RelayPin5, HIGH); toggleState_5 = 1; break; // if 'e' received Turn off Relay5
        case 'f': digitalWrite(RelayPin6, LOW);  toggleState_6 = 0; break; // if 'F' received Turn on Relay6
        case 'F': digitalWrite(RelayPin6, HIGH); toggleState_6 = 1; break; // if 'f' received Turn off Relay6
        case 'g': digitalWrite(RelayPin7, LOW);  toggleState_7 = 0; break; // if 'G' received Turn on Relay7
        case 'G': digitalWrite(RelayPin7, HIGH); toggleState_7 = 1; break; // if 'g' received Turn off Relay7
        case 'h': digitalWrite(RelayPin8, LOW);  toggleState_8 = 0; break; // if 'H' received Turn on Relay8
        case 'H': digitalWrite(RelayPin8, HIGH); toggleState_8 = 1; break; // if 'h' received Turn off Relay8
        case 'z': all_Switch_ON(); break;  // if 'Z' received Turn on all Relays
        case 'Z': all_Switch_OFF(); break; // if 'z' received Turn off all Relays
        default : break;
      }
}

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    digitalWrite(wifiLed, LOW); //Turn off WiFi LED
  }
  if (isconnected == true) {
    wifiFlag = 0;
    digitalWrite(wifiLed, HIGH); //Turn on WiFi LED
  }
}

void setup()
{
  Serial.begin(9600);

  btStart();  //Serial.println("Bluetooth On");
  
  SerialBT.begin("ESP32_BT"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  delay(5000);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(RelayPin5, OUTPUT);
  pinMode(RelayPin6, OUTPUT);
  pinMode(RelayPin7, OUTPUT);
  pinMode(RelayPin8, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);
  pinMode(SwitchPin5, INPUT_PULLUP);
  pinMode(SwitchPin6, INPUT_PULLUP);
  pinMode(SwitchPin7, INPUT_PULLUP);
  pinMode(SwitchPin8, INPUT_PULLUP);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, toggleState_1);
  digitalWrite(RelayPin2, toggleState_2);
  digitalWrite(RelayPin3, toggleState_3);
  digitalWrite(RelayPin4, toggleState_4);
  digitalWrite(RelayPin5, toggleState_5);
  digitalWrite(RelayPin6, toggleState_6);
  digitalWrite(RelayPin7, toggleState_7);
  digitalWrite(RelayPin8, toggleState_8);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  Blynk.config(AUTH);
  delay(2000);
}

void loop()
{  
  if (WiFi.status() != WL_CONNECTED)
  {
   // Serial.println("WiFi Not Connected");
   
  }
  else
  {
    //Serial.println("WiFi Connected");
    Blynk.run();
  }

  timer.run(); // Initiates SimpleTimer
  if (wifiFlag == 0){
    with_internet();
    }
  else{
    without_internet();
    if (SerialBT.available()){
     Bluetooth_handle();
   }
  }
  
}
