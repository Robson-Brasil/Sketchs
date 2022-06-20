//#define BLYNK_PRINT Serial            
#include <BlynkSimpleEsp32.h>
#include <AceButton.h>
#include <WiFi.h>
#include <WiFiClient.h>
using namespace ace_button;

// define the GPIO connected with Relays and switches
#define RelayPin1 23  //D23
#define RelayPin2 22  //D22
#define RelayPin3 21  //D21
#define RelayPin4 19  //D19

#define SwitchPin1 13  //D13
#define SwitchPin2 12  //D12
#define SwitchPin3 14  //D14
#define SwitchPin4 27  //D27

#define wifiLed   2   //D2

//Change the virtual pins according the rooms
#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3 
#define VPIN_BUTTON_4    V4

int toggleState_1 = 1; //Define integer to remember the toggle state for relay 1
int toggleState_2 = 1; //Define integer to remember the toggle state for relay 2
int toggleState_3 = 1; //Define integer to remember the toggle state for relay 3
int toggleState_4 = 1; //Define integer to remember the toggle state for relay 4

int wifiFlag = 0;

ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);
ButtonConfig config3;
AceButton button3(&config3);
ButtonConfig config4;
AceButton button4(&config4);

void handleEvent1(AceButton*, uint8_t, uint8_t);
void handleEvent2(AceButton*, uint8_t, uint8_t);
void handleEvent3(AceButton*, uint8_t, uint8_t);
void handleEvent4(AceButton*, uint8_t, uint8_t);

#define AUTH "WW7eZUxja-yaJ04fANp6KF-z0N1K63Jg"                 // You should get Auth Token in the Blynk App.  
#define WIFI_SSID "RVR 2,4GHz"             //Enter Wifi Name
#define WIFI_PASS "RodrigoValRobson2021"         //Enter wifi Password

BlynkTimer timer;

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

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
  }
  if (isconnected == true) {
    wifiFlag = 0;
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
  }
}
void setup()
{
  Serial.begin(115200);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, toggleState_1);
  digitalWrite(RelayPin2, toggleState_2);
  digitalWrite(RelayPin3, toggleState_3);
  digitalWrite(RelayPin4, toggleState_4);

  digitalWrite(wifiLed, HIGH);

  config1.setEventHandler(button1Handler);
  config2.setEventHandler(button2Handler);
  config3.setEventHandler(button3Handler);
  config4.setEventHandler(button4Handler);
  
  button1.init(SwitchPin1);
  button2.init(SwitchPin2);
  button3.init(SwitchPin3);
  button4.init(SwitchPin4);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(1000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  Blynk.config(AUTH);
  delay(100);

  Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
  Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
  Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
  Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
}

void loop()
{  
  if (WiFi.status() != WL_CONNECTED)
  {
    //Serial.println("WiFi Not Connected");
    digitalWrite(wifiLed, LOW);
  }
  else
  {
    //+Serial.println("WiFi Connected");
    digitalWrite(wifiLed, HIGH); //Turn on WiFi LED
    Blynk.run();
  }
  timer.run(); // Initiates SimpleTimer

  button1.check();
  button2.check();
  button3.check();
  button4.check();
}

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      toggleState_1 = 0;
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);   // Update Button Widget
      digitalWrite(RelayPin1, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      toggleState_1 = 1;
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);   // Update Button Widget
      digitalWrite(RelayPin1, HIGH);
      break;
  }
}

void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT2");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      toggleState_2 = 0;
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
      digitalWrite(RelayPin2, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      toggleState_2 = 1;
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
      digitalWrite(RelayPin2, HIGH);
      break;
  }
}

void button3Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT3");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      toggleState_3 = 0;
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
      digitalWrite(RelayPin3, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      toggleState_3 = 1;
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
      digitalWrite(RelayPin3, HIGH);
      break;
  }
}

void button4Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT4");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      toggleState_4 = 0;
      Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
      digitalWrite(RelayPin4, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      toggleState_4 = 1;
      Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
      digitalWrite(RelayPin4, HIGH);
      break;
  }
}
