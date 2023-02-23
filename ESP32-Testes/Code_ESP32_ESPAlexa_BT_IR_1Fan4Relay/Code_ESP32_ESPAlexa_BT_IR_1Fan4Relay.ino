/**********************************************************************************
 *  TITLE: ESPAlexa + Bluetooth + IR + Switch control 1 Fan & 4 Relays using ESP32 (Real time feedback + no WiFi control + EEPROM)
 *  Click on the following links to learn more. 
 *  YouTube Video: https://youtu.be/bVeboJwTPIU
 *  Related Blog : https://iotcircuithub.com/esp32-projects/
 *  
 *  This code is provided free for project purpose and fair use only.
 *  Please do mail us to techstudycell@gmail.com if you want to use it commercially.
 *  Copyrighted © by Tech StudyCell
 *  
 *  Preferences--> Aditional boards Manager URLs : 
 *  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
 *  
 *  Download Board ESP32 (2.0.5) : https://github.com/espressif/arduino-esp32
 *
 *  Download the libraries 
 *  Espalexa Library (2.7.0):  https://github.com/Aircoookie/Espalexa
 *  IRremote Library (3.6.1): https://github.com/Arduino-IRremote/Arduino-IRremote
 *  AceButton Library (1.9.2): https://github.com/bxparks/AceButton
 **********************************************************************************/

// WiFi Credentials
const char* ssid = "";
const char* password = "";

// device names
String Device_1_Name = "Night Lamp";
String Device_2_Name = "Room Light";
String Device_3_Name = "Studio Light";
String Device_4_Name = "Toilet Light";
String Device_5_Name = "Fan";


//Update the HEX code of IR Remote buttons 0x<HEX CODE>
#define IR_Button_1   0x1FEE01F
#define IR_Button_2   0x1FE10EF
#define IR_Button_3   0x1FE906F
#define IR_Button_4   0x1FE50AF
#define IR_Button_5   0x1FED827
#define IR_Fan_Up     0x1FE609F
#define IR_Fan_Down   0x1FEA05F
#define IR_All_On     0x1FE807F
#define IR_All_Off    0x1FE48B7

#include <WiFi.h>
#include <Espalexa.h>
#include <Preferences.h>
#include <IRremote.h>
#include <AceButton.h>
using namespace ace_button;

Preferences pref;
Espalexa espalexa;

// define the GPIO connected with Relays and switches
#define RelayPin1 23  //D23
#define RelayPin2 22  //D22
#define RelayPin3 21  //D21
#define RelayPin4 19  //D19

#define SwitchPin1 13  //D13
#define SwitchPin2 12  //D12
#define SwitchPin3 14  //D14
#define SwitchPin4 27  //D27
#define SwitchPin5 26  //D26

#define FanRelay1 18  //D18
#define FanRelay2 5   //D5
#define FanRelay3 25  //D25

#define FanSwitch1 33  //D33
#define FanSwitch2 32  //D32
#define FanSwitch3 15  //D15
#define FanSwitch4 4   //D4

#define wifiLed   2   //D2
#define IR_RECV_PIN   35 // D35 (IR receiver pin)

#define RX2Pin       16  // RX2
#define TX2Pin       17  // TX2

// Relay State
bool toggleState_1 = LOW; //Define integer to remember the toggle state for relay 1
bool toggleState_2 = LOW; //Define integer to remember the toggle state for relay 2
bool toggleState_3 = LOW; //Define integer to remember the toggle state for relay 3
bool toggleState_4 = LOW; //Define integer to remember the toggle state for relay 4
bool toggleState_5 = LOW; //Define integer to remember the toggle state for FAN
bool fanSpeed_0 = LOW;
bool fanSpeed_1 = LOW; //Define integer to remember the toggle state for relay 5
bool fanSpeed_2 = LOW; //Define integer to remember the toggle state for relay 6
bool fanSpeed_3 = LOW; //Define integer to remember the toggle state for relay 7
bool fanSpeed_4 = LOW; //Define integer to remember the toggle state for relay 8

ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);
ButtonConfig config3;
AceButton button3(&config3);
ButtonConfig config4;
AceButton button4(&config4);
ButtonConfig config5;
AceButton button5(&config5);

void handleEvent1(AceButton*, uint8_t, uint8_t);
void handleEvent2(AceButton*, uint8_t, uint8_t);
void handleEvent3(AceButton*, uint8_t, uint8_t);
void handleEvent4(AceButton*, uint8_t, uint8_t);
void handleEvent5(AceButton*, uint8_t, uint8_t);

String bt_data = ""; // variable for storing bluetooth data
int currSpeed = 0;
bool first_run = true;
int wifiFlag = 0;
IRrecv irrecv(IR_RECV_PIN);
decode_results results;


// prototypes
boolean connectWifi();

//callback functions
void firstLightChanged(uint8_t brightness);
void secondLightChanged(uint8_t brightness);
void thirdLightChanged(uint8_t brightness);
void fourthLightChanged(uint8_t brightness);
void fanChanged(uint8_t brightness);


boolean wifiConnected = false;

//our callback functions
void firstLightChanged(uint8_t brightness)
{
  //Control the device
  if (brightness == 255)
    {
      digitalWrite(RelayPin1, LOW);
      Serial.println("Device1 ON");
      toggleState_1 = 1;
    }
  else
  {
    digitalWrite(RelayPin1, HIGH);
    Serial.println("Device1 OFF");
    toggleState_1 = 0;
  }
  pref.putBool("Relay1", toggleState_1);
}

void secondLightChanged(uint8_t brightness)
{
  //Control the device 
  if (brightness == 255)
    {
      digitalWrite(RelayPin2, LOW);
      Serial.println("Device2 ON");
      toggleState_2 = 1;
    }
  else
  {
    digitalWrite(RelayPin2, HIGH);
    Serial.println("Device2 OFF");
    toggleState_2 = 0;
  }
  pref.putBool("Relay2", toggleState_2);
}

void thirdLightChanged(uint8_t brightness)
{
  //Control the device  
  if (brightness == 255)
    {
      digitalWrite(RelayPin3, LOW);
      Serial.println("Device3 ON");
      toggleState_3 = 1;
    }
  else
  {
    digitalWrite(RelayPin3, HIGH);
    Serial.println("Device3 OFF");
    toggleState_3 = 0;
  }
  pref.putBool("Relay3", toggleState_3);
}

void fourthLightChanged(uint8_t brightness)
{
  //Control the device 
  if (brightness == 255)
    {
      digitalWrite(RelayPin4, LOW);
      Serial.println("Device4 ON");
      toggleState_4 = 1;
    }
  else
  {
    digitalWrite(RelayPin4, HIGH);
    Serial.println("Device4 OFF");
    toggleState_4 = 0;
  }
  pref.putBool("Relay4", toggleState_4);
}

void fanChanged(uint8_t brightness)
{
  currSpeed = map(brightness, 0, 255, 0, 4);

  if(currSpeed == 0){
    toggleState_5 = 0;
    fanSpeedControl(0);
    pref.putBool("Fan_Power", toggleState_5);
  }
  else{
    toggleState_5 = 1;
    fanSpeedControl(currSpeed);
    pref.putBool("Fan_Power", toggleState_5);
    pref.putInt("Fan_Speed", currSpeed);
  }
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi()
{
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state) {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}

void addDevices(){
  // Define your devices here.
  espalexa.addDevice(Device_1_Name, firstLightChanged); //simplest definition, default state off
  espalexa.addDevice(Device_2_Name, secondLightChanged);
  espalexa.addDevice(Device_3_Name, thirdLightChanged);
  espalexa.addDevice(Device_4_Name, fourthLightChanged);
  espalexa.addDevice(Device_5_Name, fanChanged); 

  espalexa.begin();
}

void sendFeedback(int relay, int value){
  EspalexaDevice* d = espalexa.getDevice(relay); //the index is zero-based
  if(relay == 4){
    d->setPercent(map(value, 0, 4, 0, 100)); //set value "brightness" in percent
  }
  else{
    d->setPercent(value);
  }
}

void bluetooth_control()
{
  if(Serial2.available()) {
    bt_data = Serial2.readString();
    //Serial.println(bt_data.substring(bt_data.lastIndexOf(",") + 1));
    
    if (bt_data.substring(bt_data.lastIndexOf(",") + 1) == "A1"){
      digitalWrite(RelayPin1, LOW);  toggleState_1 = 1; // if "A1" received Turn on Relay1
      pref.putBool("Relay1", toggleState_1);
      sendFeedback(0, 100);
    }
    else if(bt_data.substring(bt_data.lastIndexOf(",") + 1) == "A0"){
      digitalWrite(RelayPin1, HIGH);  toggleState_1 = 0; // if "A0" received Turn off Relay1
      pref.putBool("Relay1", toggleState_1);
      sendFeedback(0, 0);
    }
    else if(bt_data.substring(bt_data.lastIndexOf(",") + 1) == "B1"){
      digitalWrite(RelayPin2, LOW);  toggleState_2 = 1; // if "B1" received Turn on Relay2
      pref.putBool("Relay2", toggleState_2);
      sendFeedback(1, 100);
    }
    else if(bt_data.substring(bt_data.lastIndexOf(",") + 1) == "B0"){
      digitalWrite(RelayPin2, HIGH);  toggleState_2 = 0; // if "B0" received Turn off Relay2
      pref.putBool("Relay2", toggleState_2);
      sendFeedback(1, 0);
    }
    else if(bt_data.substring(bt_data.lastIndexOf(",") + 1) == "C1"){
      digitalWrite(RelayPin3, LOW);  toggleState_3 = 1; // if "C1" received Turn on Relay3
      pref.putBool("Relay3", toggleState_3);
      sendFeedback(2, 100);
    }
    else if(bt_data.substring(bt_data.lastIndexOf(",") + 1) == "C0"){
      digitalWrite(RelayPin3, HIGH);  toggleState_3 = 0; // if "C0" received Turn off Relay3
      pref.putBool("Relay3", toggleState_3);
      sendFeedback(2, 0);
    }
    else if(bt_data.substring(bt_data.lastIndexOf(",") + 1) == "D1"){
      digitalWrite(RelayPin4, LOW);  toggleState_4 = 1; // if "D1" received Turn on Relay4
      pref.putBool("Relay4", toggleState_4);
      sendFeedback(3, 100);
    }
    else if(bt_data.substring(bt_data.lastIndexOf(",") + 1) == "D0"){
      digitalWrite(RelayPin4, HIGH);  toggleState_4 = 0; // if "D0" received Turn off Relay4
      pref.putBool("Relay4", toggleState_4);
      sendFeedback(3, 0);
    }
    else if(bt_data.substring(bt_data.lastIndexOf(",") + 1) == "E1"){
      fanSpeedControl(currSpeed); toggleState_5 = 1; // if "E1" received Turn on Fan
      pref.putBool("Fan_Power", toggleState_5);
      sendFeedback(4, currSpeed);
    }
    else if(bt_data.substring(bt_data.lastIndexOf(",") + 1) == "E0"){
      fanSpeedControl(0);  toggleState_5 = 0; // if "E0" received Turn off Fan
      pref.putBool("Fan_Power", toggleState_5);
      sendFeedback(4, 0);
    }
    else if(bt_data.substring(bt_data.lastIndexOf(",") + 1) == "S0"){
      currSpeed = 0;
      if(toggleState_5 == 1){ fanSpeedControl(currSpeed); }//Fan Speed 0
      pref.putInt("Fan_Speed", currSpeed);
      sendFeedback(4, currSpeed);
    }
    else if(bt_data.substring(bt_data.lastIndexOf(",") + 1) == "S1"){
      currSpeed = 1;
      if(toggleState_5 == 1){ fanSpeedControl(currSpeed); }//Fan Speed 1
      pref.putInt("Fan_Speed", currSpeed);
      sendFeedback(4, currSpeed);
    }
    else if(bt_data.substring(bt_data.lastIndexOf(",") + 1) == "S2"){
      currSpeed = 2;
      if(toggleState_5 == 1){ fanSpeedControl(currSpeed); } //Fan Speed 2
      pref.putInt("Fan_Speed", currSpeed);
      sendFeedback(4, currSpeed);
    }
    else if(bt_data.substring(bt_data.lastIndexOf(",") + 1) == "S3"){
      currSpeed = 3;
      if(toggleState_5 == 1){ fanSpeedControl(currSpeed); }//Fan Speed 3
      pref.putInt("Fan_Speed", currSpeed);
      sendFeedback(4, currSpeed);; 
    }
    else if(bt_data.substring(bt_data.lastIndexOf(",") + 1) == "S4"){
      currSpeed = 4;
      if(toggleState_5 == 1){ fanSpeedControl(currSpeed); }//Fan Speed 4
      pref.putInt("Fan_Speed", currSpeed);
      sendFeedback(4, currSpeed);
    }
    else if(bt_data.substring(bt_data.lastIndexOf(",") + 1) == "Z1"){    // if "Z1" received Turn on all Relays
      all_SwitchOn();
    }
    else if(bt_data.substring(bt_data.lastIndexOf(",") + 1) == "Z0"){    // if "Z0" received Turn off all Relays
      all_SwitchOff();
    }
  } 
}

void ir_remote(){
  if (irrecv.decode(&results)) {
    switch(results.value){
        case IR_Button_1:  
          digitalWrite(RelayPin1, toggleState_1);
          toggleState_1 = !toggleState_1;
          pref.putBool("Relay1", toggleState_1);
          (toggleState_1 == false) ? sendFeedback(0, 0) : sendFeedback(0, 100);
          delay(100);            
          break;
        case IR_Button_2:  
          digitalWrite(RelayPin2, toggleState_2);
          toggleState_2 = !toggleState_2;
          pref.putBool("Relay2", toggleState_2);
          (toggleState_2 == false) ? sendFeedback(1, 0) : sendFeedback(1, 100);
          delay(100);            
          break;
        case IR_Button_3:  
          digitalWrite(RelayPin3, toggleState_3);
          toggleState_3 = !toggleState_3;
          pref.putBool("Relay3", toggleState_3);
          (toggleState_3 == false) ? sendFeedback(2, 0) : sendFeedback(2, 100);
          delay(100);            
          break;
        case IR_Button_4:  
          digitalWrite(RelayPin4, toggleState_4);
          toggleState_4 = !toggleState_4;
          pref.putBool("Relay4", toggleState_4);
          (toggleState_4 == false) ? sendFeedback(3, 0) : sendFeedback(3, 100);
          delay(100);            
          break;
        case IR_Button_5:   
          if(toggleState_5 == 0){
            fanSpeedControl(currSpeed); //Turn ON Fan
            sendFeedback(4, currSpeed);
          }
          else {
            fanSpeedControl(0); //Turn OFF Fan
            sendFeedback(4, 0); //set value "brightness" in percent
          }
          toggleState_5 = !toggleState_5;
          pref.putBool("Fan_Power", toggleState_5);
          delay(100);            
          break;
        case IR_Fan_Up: 
          if(currSpeed < 4 && toggleState_5 == 1){
            currSpeed = currSpeed + 1;
            fanSpeedControl(currSpeed);
            pref.putInt("Fan_Speed", currSpeed);
            sendFeedback(4, currSpeed);
            delay(100); 
          }           
          break;
        case IR_Fan_Down: 
          if(currSpeed > 0 && toggleState_5 == 1){
            currSpeed = currSpeed - 1;
            fanSpeedControl(currSpeed);
            pref.putInt("Fan_Speed", currSpeed);
            sendFeedback(4, currSpeed);
            delay(100); 
          }       
          break;
        case IR_All_Off:
          all_SwitchOff();  
          break;
        case IR_All_On:
          all_SwitchOn();  
          break;
        default : break;         
      }   
      //Serial.println(results.value, HEX);    
      irrecv.resume();   
  } 
}

void all_SwitchOn(){
  toggleState_1 = 1; digitalWrite(RelayPin1, LOW); pref.putBool("Relay1", toggleState_1); sendFeedback(0, 100); delay(100);
  toggleState_2 = 1; digitalWrite(RelayPin2, LOW); pref.putBool("Relay2", toggleState_2); sendFeedback(1, 100); delay(100);
  toggleState_3 = 1; digitalWrite(RelayPin3, LOW); pref.putBool("Relay3", toggleState_3); sendFeedback(2, 100); delay(100);
  toggleState_4 = 1; digitalWrite(RelayPin4, LOW); pref.putBool("Relay4", toggleState_4); sendFeedback(3, 100); delay(100);
  toggleState_5 = 1; fanSpeedControl(currSpeed); pref.putBool("Fan_Power", toggleState_5); sendFeedback(4, currSpeed); delay(100);
}

void all_SwitchOff(){
  toggleState_1 = 0; digitalWrite(RelayPin1, HIGH); pref.putBool("Relay1", toggleState_1); sendFeedback(0, 0); delay(100);
  toggleState_2 = 0; digitalWrite(RelayPin2, HIGH); pref.putBool("Relay2", toggleState_2); sendFeedback(1, 0); delay(100);
  toggleState_3 = 0; digitalWrite(RelayPin3, HIGH); pref.putBool("Relay3", toggleState_3); sendFeedback(2, 0); delay(100);
  toggleState_4 = 0; digitalWrite(RelayPin4, HIGH); pref.putBool("Relay4", toggleState_4); sendFeedback(3, 0); delay(100);
  toggleState_5 = 0; fanSpeedControl(0); pref.putBool("Fan_Power", toggleState_5); sendFeedback(4, 0); delay(100);
}



void getRelayState()
{
  //Serial.println("reading data from NVS");
  toggleState_1 = pref.getBool("Relay1", 0);
  digitalWrite(RelayPin1, !toggleState_1); 
  (toggleState_1 == false) ? sendFeedback(0, 0) : sendFeedback(0, 100);
  delay(200);
  toggleState_2 = pref.getBool("Relay2", 0);
  digitalWrite(RelayPin2, !toggleState_2); 
  (toggleState_2 == false) ? sendFeedback(1, 0) : sendFeedback(1, 100);
  delay(200);
  toggleState_3 = pref.getBool("Relay3", 0);
  digitalWrite(RelayPin3, !toggleState_3); 
  (toggleState_3 == false) ? sendFeedback(2, 0) : sendFeedback(2, 100);
  delay(200);
  toggleState_4 = pref.getBool("Relay4", 0);
  digitalWrite(RelayPin4, !toggleState_4); 
  (toggleState_4 == false) ? sendFeedback(3, 0) : sendFeedback(3, 100);
  delay(200);
  currSpeed = pref.getInt("Fan_Speed", 0);
  //Blynk.virtualWrite(VPIN_FAN_SPEED, currSpeed);
  delay(200);
  toggleState_5 = pref.getBool("Fan_Power", 0);
  if(toggleState_5 == 1 && currSpeed > 0){
    fanSpeedControl(currSpeed); 
  }  
  (toggleState_5 == false) ? sendFeedback(4, 0) : sendFeedback(4, currSpeed);
  delay(200);  
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RX2Pin, TX2Pin);
  //Open namespace in read-write mode
  pref.begin("Relay_State", false);
  
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(FanRelay1, OUTPUT);
  pinMode(FanRelay2, OUTPUT);
  pinMode(FanRelay3, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);
  pinMode(SwitchPin5, INPUT_PULLUP);
  pinMode(FanSwitch1, INPUT_PULLUP);
  pinMode(FanSwitch2, INPUT_PULLUP);
  pinMode(FanSwitch3, INPUT_PULLUP);
  pinMode(FanSwitch4, INPUT_PULLUP);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, !toggleState_1);
  digitalWrite(RelayPin2, !toggleState_2);
  digitalWrite(RelayPin3, !toggleState_3);
  digitalWrite(RelayPin4, !toggleState_4);
  digitalWrite(FanRelay1, HIGH);
  digitalWrite(FanRelay2, HIGH);
  digitalWrite(FanRelay3, HIGH);

  digitalWrite(wifiLed, LOW);

  config1.setEventHandler(button1Handler);
  config2.setEventHandler(button2Handler);
  config3.setEventHandler(button3Handler);
  config4.setEventHandler(button4Handler);
  config5.setEventHandler(button5Handler);

  button1.init(SwitchPin1);
  button2.init(SwitchPin2);
  button3.init(SwitchPin3);
  button4.init(SwitchPin4);
  button5.init(SwitchPin5);

  irrecv.enableIRIn(); // Enabling IR sensor

  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected)
  {
    addDevices();
  }
  else
  {
    Serial.println("Cannot connect to WiFi. So in Manual Mode");
  }
  delay(1000);

  getRelayState(); //fetch data from NVS Flash Memory
//  delay(1000);
}

void loop()
{  
  if (WiFi.status() != WL_CONNECTED)
  {
    //Serial.print("WiFi Not Connected ");
    digitalWrite(wifiLed, LOW); //Turn off WiFi LED
  }
  else
  {
    //Serial.print("WiFi Connected  ");
    digitalWrite(wifiLed, HIGH);
    //WiFi Control
    if (wifiConnected){
      espalexa.loop();
      delay(1);
    }
    else {
      wifiConnected = connectWifi(); // Initialise wifi connection
      if(wifiConnected){
      addDevices();
      }
    }
  }
  //Control Switches Manualy
  button1.check();
  button2.check();
  button3.check();
  button4.check();
  button5.check(); 
  
  fanRegularor(); //Control Fan Speed

  ir_remote(); //IR remote Control
  bluetooth_control(); //Bluetooth Control
}

void fanRegularor(){
  if (digitalRead(FanSwitch1) == HIGH && digitalRead(FanSwitch2) == HIGH && digitalRead(FanSwitch3) == HIGH && digitalRead(FanSwitch4) == HIGH  && fanSpeed_0 == LOW)
  {
    if(first_run == false){
      currSpeed = 0;
      if(toggleState_5 == 1){
        fanSpeedControl(currSpeed); 
      }
      pref.putInt("Fan_Speed", currSpeed);
      sendFeedback(4, currSpeed);
    }    
    fanSpeed_1 = LOW;
    fanSpeed_2 = LOW;
    fanSpeed_3 = LOW;
    fanSpeed_4 = LOW;
    fanSpeed_0 = HIGH;
  }
  if (digitalRead(FanSwitch1) == LOW && fanSpeed_1 == LOW)
  {
    if(first_run == false){
      currSpeed = 1;
      if(toggleState_5 == 1){
        fanSpeedControl(currSpeed); 
      }
      pref.putInt("Fan_Speed", currSpeed);
      sendFeedback(4, currSpeed);
    }
    fanSpeed_1 = HIGH;
    fanSpeed_2 = LOW;
    fanSpeed_3 = LOW;
    fanSpeed_4 = LOW;
    fanSpeed_0 = LOW;
  }
  if (digitalRead(FanSwitch2) == LOW && digitalRead(FanSwitch3) == HIGH && fanSpeed_2 == LOW)
  {
    if(first_run == false){
      currSpeed = 2;
      if(toggleState_5 == 1){
        fanSpeedControl(currSpeed); 
      }
      pref.putInt("Fan_Speed", currSpeed);
      sendFeedback(4, currSpeed);
    }
    fanSpeed_1 = LOW;
    fanSpeed_2 = HIGH;
    fanSpeed_3 = LOW;
    fanSpeed_4 = LOW;
    fanSpeed_0 = LOW;
  }
  if (digitalRead(FanSwitch2) == LOW && digitalRead(FanSwitch3) == LOW && fanSpeed_3 == LOW)
  {
    if(first_run == false){
      currSpeed = 3;
      if(toggleState_5 == 1){
        fanSpeedControl(currSpeed); 
      }
      pref.putInt("Fan_Speed", currSpeed);
      sendFeedback(4, currSpeed);
    }
    fanSpeed_1 = LOW;
    fanSpeed_2 = LOW;
    fanSpeed_3 = HIGH;
    fanSpeed_4 = LOW;
    fanSpeed_0 = LOW;
  }
  if (digitalRead(FanSwitch4) == LOW  && fanSpeed_4 == LOW)
  {
    if(first_run == false){
      currSpeed = 4;
      if(toggleState_5 == 1){
        fanSpeedControl(currSpeed); 
      }
      pref.putInt("Fan_Speed", currSpeed);
      sendFeedback(4, currSpeed);
    }
    fanSpeed_1 = LOW;
    fanSpeed_2 = LOW;
    fanSpeed_3 = LOW;
    fanSpeed_4 = HIGH;
    fanSpeed_0 = LOW;
  }
  first_run = false;
}

void fanSpeedControl(int fanSpeed){
  switch(fanSpeed){
      case 0:
        digitalWrite(FanRelay1, HIGH);
        digitalWrite(FanRelay2, HIGH);
        digitalWrite(FanRelay3, HIGH);        
      break;
      case 1:
        digitalWrite(FanRelay1, HIGH);
        digitalWrite(FanRelay2, HIGH);
        digitalWrite(FanRelay3, HIGH);
        delay(500);
        digitalWrite(FanRelay1, LOW);
      break;
      case 2:
        digitalWrite(FanRelay1, HIGH);
        digitalWrite(FanRelay2, HIGH);
        digitalWrite(FanRelay3, HIGH);
        delay(500);
        digitalWrite(FanRelay2, LOW);
      break;
      case 3:
        digitalWrite(FanRelay1, HIGH);
        digitalWrite(FanRelay2, HIGH);
        digitalWrite(FanRelay3, HIGH);
        delay(500);
        digitalWrite(FanRelay1, LOW);
        digitalWrite(FanRelay2, LOW);
      break;
      case 4:
        digitalWrite(FanRelay1, HIGH);
        digitalWrite(FanRelay2, HIGH);
        digitalWrite(FanRelay3, HIGH);
        delay(500);
        digitalWrite(FanRelay3, LOW);
      break;          
      default : break;         
    } 
}

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  switch (eventType) {
    case AceButton::kEventPressed:
      toggleState_1 = 1;
      digitalWrite(RelayPin1, LOW);
      pref.putBool("Relay1", toggleState_1);
      sendFeedback(0, 100);
      break;
    case AceButton::kEventReleased:
      toggleState_1 = 0;
      digitalWrite(RelayPin1, HIGH);
      pref.putBool("Relay1", toggleState_1);
      sendFeedback(0, 0);
      break;
  }
}

void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT2");
  switch (eventType) {
    case AceButton::kEventPressed:
      toggleState_2 = 1;
      digitalWrite(RelayPin2, LOW);
      pref.putBool("Relay2", toggleState_2);
      sendFeedback(1, 100);
      break;
    case AceButton::kEventReleased:
      toggleState_2 = 0;
      digitalWrite(RelayPin2, HIGH);
      pref.putBool("Relay2", toggleState_2);
      sendFeedback(1, 0);
      break;
  }
}

void button3Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT3");
  switch (eventType) {
    case AceButton::kEventPressed:
      toggleState_3 = 1;
      digitalWrite(RelayPin3, LOW);
      pref.putBool("Relay3", toggleState_3);
      sendFeedback(2, 100);
      break;
    case AceButton::kEventReleased:
      toggleState_3 = 0;
      digitalWrite(RelayPin3, HIGH);
      pref.putBool("Relay3", toggleState_3);
      sendFeedback(2, 0);
      break;
  }
}

void button4Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT4");
  switch (eventType) {
    case AceButton::kEventPressed:
      toggleState_4 = 1;
      digitalWrite(RelayPin4, LOW);
      pref.putBool("Relay4", toggleState_4);
      sendFeedback(3, 100);
      break;
    case AceButton::kEventReleased:
      toggleState_4 = 0;
      digitalWrite(RelayPin4, HIGH);
      pref.putBool("Relay4", toggleState_4);
      sendFeedback(3, 0);
      break;
  }
}

void button5Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT5");
  switch (eventType) {
    case AceButton::kEventPressed:
      toggleState_5 = 1;
      fanSpeedControl(currSpeed);
      pref.putBool("Fan_Power", toggleState_5);
      sendFeedback(4, currSpeed);
      break;
    case AceButton::kEventReleased:
      toggleState_5 = 0;
      fanSpeedControl(0);
      pref.putBool("Fan_Power", toggleState_5);
      sendFeedback(4, 0);
      break;
  }
}
