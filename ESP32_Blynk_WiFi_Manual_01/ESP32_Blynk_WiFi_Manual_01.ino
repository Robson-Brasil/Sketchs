//IoT - Automação Residencial
//Dispositivo : ESP32
//Temperatura, Umidade, Luminosidade, Relés e Switchs
//Autor : Robson Brasil
//Versão : 20
//Última Modificação : 29/12/2021

#define AUTH "WW7eZUxja-yaJ04fANp6KF-z0N1K63Jg"                 // You should get Auth Token in the Blynk App.  
#define WIFI_SSID "RVR 2,4GHz"             //Enter Wifi Name
#define WIFI_PASS "RodrigoValRobson2021"         //Enter wifi Password

#include <BlynkSimpleEsp32.h>
#include <DHT.h> 

BlynkTimer timer;

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11     // DHT 11
#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

#define DHTPIN              16 //D16  pin connected with DHT
#define LDR_PIN             34 //D34  pin connected with LDR

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
#define VPIN_BUTTON_C    V9
#define VPIN_TEMPERATURE V10
#define VPIN_HUMIDITY    V11
#define VPIN_LDR         V12

// Relay State
bool toggleState_1 = LOW; //Define integer to remember the toggle state for relay 1
bool toggleState_2 = LOW; //Define integer to remember the toggle state for relay 2
bool toggleState_3 = LOW; //Define integer to remember the toggle state for relay 3
bool toggleState_4 = LOW; //Define integer to remember the toggle state for relay 4
bool toggleState_5 = LOW; //Define integer to remember the toggle state for relay 5
bool toggleState_6 = LOW; //Define integer to remember the toggle state for relay 6
bool toggleState_7 = LOW; //Define integer to remember the toggle state for relay 7
bool toggleState_8 = LOW; //Define integer to remember the toggle state for relay 8

// Switch State
bool SwitchState_1 = LOW;
bool SwitchState_2 = LOW;
bool SwitchState_3 = LOW;
bool SwitchState_4 = LOW;
bool SwitchState_5 = LOW;
bool SwitchState_6 = LOW;
bool SwitchState_7 = LOW;
bool SwitchState_8 = LOW;

int wifiFlag = 0;

float temperature1 = 0;
float humidity1   = 0;
int   ldrVal;

DHT dht(DHTPIN, DHTTYPE);

void readSensor(){
    
  ldrVal = map(analogRead(LDR_PIN), 0, 4095, 10, 0);
  
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else {
    humidity1 = h;
    temperature1 = t;
    Serial.println(temperature1);
    Serial.println(ldrVal);
  }  
}

void sendSensor()
{
  readSensor();
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity1);
  Blynk.virtualWrite(VPIN_TEMPERATURE, temperature1);
  Blynk.virtualWrite(VPIN_LDR, ldrVal);
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

// When App button is pushed - switch the state

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  if(toggleState_1 == 1){
    digitalWrite(RelayPin1, LOW);
  }
  else { 
    digitalWrite(RelayPin1, HIGH);
  }
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  if(toggleState_2 == 1){
    digitalWrite(RelayPin2, LOW);
  }
  else { 
    digitalWrite(RelayPin2, HIGH);
  }
}

BLYNK_WRITE(VPIN_BUTTON_3) {
  toggleState_3 = param.asInt();
  if(toggleState_3 == 1){
    digitalWrite(RelayPin3, LOW);
  }
  else { 
    digitalWrite(RelayPin3, HIGH);
  }
}

BLYNK_WRITE(VPIN_BUTTON_4) {
  toggleState_4 = param.asInt();
  if(toggleState_4 == 1){
    digitalWrite(RelayPin4, LOW);
  }
  else { 
    digitalWrite(RelayPin4, HIGH);
  }
}

BLYNK_WRITE(VPIN_BUTTON_5) {
  toggleState_5 = param.asInt();
  if(toggleState_5 == 1){
    digitalWrite(RelayPin5, LOW);
  }
  else { 
    digitalWrite(RelayPin5, HIGH);
  }
}

BLYNK_WRITE(VPIN_BUTTON_6) {
  toggleState_6 = param.asInt();
  if(toggleState_6 == 1){
    digitalWrite(RelayPin6, LOW);
  }
  else { 
    digitalWrite(RelayPin6, HIGH);
  }
}

BLYNK_WRITE(VPIN_BUTTON_7) {
  toggleState_7 = param.asInt();
  if(toggleState_7 == 1){
    digitalWrite(RelayPin7, LOW);
  }
  else { 
    digitalWrite(RelayPin7, HIGH);
  }
}

BLYNK_WRITE(VPIN_BUTTON_8) {
  toggleState_8 = param.asInt();
  if(toggleState_8 == 1){
    digitalWrite(RelayPin8, LOW);
  }
  else { 
    digitalWrite(RelayPin8, HIGH);
  }
}

BLYNK_WRITE(VPIN_BUTTON_C) {
  all_SwitchOff();
}

void all_SwitchOff(){
  toggleState_1 = 0; digitalWrite(RelayPin1, HIGH); Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1); delay(100);
  toggleState_2 = 0; digitalWrite(RelayPin2, HIGH); Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2); delay(100);
  toggleState_3 = 0; digitalWrite(RelayPin3, HIGH); Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3); delay(100);
  toggleState_4 = 0; digitalWrite(RelayPin4, HIGH); Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4); delay(100);
  toggleState_5 = 0; digitalWrite(RelayPin5, HIGH); Blynk.virtualWrite(VPIN_BUTTON_5, toggleState_5); delay(100);
  toggleState_6 = 0; digitalWrite(RelayPin6, HIGH); Blynk.virtualWrite(VPIN_BUTTON_6, toggleState_6); delay(100);
  toggleState_7 = 0; digitalWrite(RelayPin7, HIGH); Blynk.virtualWrite(VPIN_BUTTON_7, toggleState_7); delay(100);
  toggleState_8 = 0; digitalWrite(RelayPin8, HIGH); Blynk.virtualWrite(VPIN_BUTTON_8, toggleState_8); delay(100);
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity1);
  Blynk.virtualWrite(VPIN_TEMPERATURE, temperature1);
  Blynk.virtualWrite(VPIN_LDR, ldrVal);
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
  Serial.begin(115200);

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
  digitalWrite(RelayPin1, HIGH);
  digitalWrite(RelayPin2, HIGH);
  digitalWrite(RelayPin3, HIGH);
  digitalWrite(RelayPin4, HIGH);
  digitalWrite(RelayPin5, HIGH);
  digitalWrite(RelayPin6, HIGH);
  digitalWrite(RelayPin7, HIGH);
  digitalWrite(RelayPin8, HIGH);

  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  Blynk.config(AUTH);
}

void loop()
{
  Blynk.run();
  timer.run();
}
