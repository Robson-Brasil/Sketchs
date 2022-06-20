//IoT - Automação Residencial
//Dispositivo : ESP32
//Temperatura, Umidade, Luminosidade, Relés e Switchs
//Autor : Robson Brasil
//Versão : 20
//Última Modificação : 29/12/2021

#include <BlynkSimpleEsp32.h>
BlynkTimer timer;
#include "DHTesp.h"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Define as GPIOs conectads com os Relés e Botões
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
//#define SwitchPin7 15  //D15
#define SwitchPin7 39  //D15
//#define SwitchPin8 4   //D4
#define SwitchPin8 17   //D4

#define wifiLed    2   //D2

// Define os Pinos Virtuais pra serem usados no Blynk
#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3 
#define VPIN_BUTTON_4    V4
#define VPIN_BUTTON_5    V5 
#define VPIN_BUTTON_6    V6
#define VPIN_BUTTON_7    V7 
#define VPIN_BUTTON_8    V8

bool toggleState_1 = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 1
bool toggleState_2 = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 2
bool toggleState_3 = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 3
bool toggleState_4 = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 5
bool toggleState_5 = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 5
bool toggleState_6 = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 6
bool toggleState_7 = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 7
bool toggleState_8 = 0; //Defina o número inteiro para lembrar o estado de alternância para o Relé 8

bool SwitchState_1 = LOW;
bool SwitchState_2 = LOW;
bool SwitchState_3 = LOW;
bool SwitchState_4 = LOW;
bool SwitchState_5 = LOW;
bool SwitchState_6 = LOW;
bool SwitchState_7 = LOW;
bool SwitchState_8 = LOW;

int wifiFlag = 0;

#define AUTH "WW7eZUxja-yaJ04fANp6KF-z0N1K63Jg"  // Blynk Authentication Token
#define WIFI_SSID "RVR 2,4GHz"                  //WiFi Name
#define WIFI_PASS "RodrigoValRobson2021"       //WiFiPassword

DHTesp dht;
int LDR = 34;

float currentTemp;
float currentHumidity;
float currentLuminosidade;

void relayOnOff(int relay){

    switch(relay){
      case 1: 
             if(toggleState_1 == 1){
              digitalWrite(RelayPin1, LOW); // Relé 1 Ligado
              toggleState_1 = 0;
              Serial.println("Device1 LIGADO");
              }
             else{
              digitalWrite(RelayPin1, HIGH); // Relé 1 Desligado
              toggleState_1 = 1;
              Serial.println("Device1 DESLIGADO");
              }
             delay(500);
      break;
      case 2: 
             if(toggleState_2 == 1){
              digitalWrite(RelayPin2, LOW); // Relé 2 Ligado
              toggleState_2 = 0;
              Serial.println("Device2 LIGADO");
              }
             else{
              digitalWrite(RelayPin2, HIGH); // Relé 2 Desligado
              toggleState_2 = 1;
              Serial.println("Device2 DESLIGADO");
              }
             delay(500);
      break;
      case 3: 
             if(toggleState_3 == 1){
              digitalWrite(RelayPin3, LOW); // Relé 3 Ligado
              toggleState_3 = 0;
              Serial.println("Device3 LIGADO");
              }
             else{
              digitalWrite(RelayPin3, HIGH); // Relé 3 Desligado
              toggleState_3 = 1;
              Serial.println("Device3 DESLIGADO");
              }
             delay(500);
      break;
      case 4: 
             if(toggleState_4 == 1){
              digitalWrite(RelayPin4, LOW); // Relé 4 Ligado
              toggleState_4 = 0;
              Serial.println("Device4 LIGADO");
              }
             else{
              digitalWrite(RelayPin4, HIGH); // Relé 4 Desligado
              toggleState_4 = 1;
              Serial.println("Device4 DESLIGADO");
              }
             delay(500);
      break;
      case 5: 
             if(toggleState_5 == 1){
              digitalWrite(RelayPin5, LOW); // Relé 5 Ligado
              toggleState_5 = 0;
              Serial.println("Device5 LIGADO");
              }
             else{
              digitalWrite(RelayPin5, HIGH); // Relé 5 Desligado
              toggleState_5 = 1;
              Serial.println("Device5 DESLIGADO");
              }
             delay(500);
      break;
      case 6: 
             if(toggleState_6 == 1){
              digitalWrite(RelayPin6, LOW); // Relé 6 Ligado
              toggleState_6 = 0;
              Serial.println("Device6 LIGADO");
              }
             else{
              digitalWrite(RelayPin6, HIGH); // Relé 6 Desligado
              toggleState_6 = 1;
              Serial.println("Device6 DESLIGADO");
              }
             delay(500);
      break;
      case 7: 
             if(toggleState_7 == 1){
              digitalWrite(RelayPin7, LOW); // Relé 7 Ligado
              toggleState_7 = 0;
              Serial.println("Device7 LIGADO");
              }
             else{
              digitalWrite(RelayPin7, HIGH); // Relé 7 Desligado
              toggleState_7 = 1;
              Serial.println("Device7 DESLIGADO");
              }
             delay(500);
      break;
      case 8: 
             if(toggleState_8 == 1){
              digitalWrite(RelayPin8, LOW); // Relé 8 Ligado
              toggleState_8 = 0;
              Serial.println("Device8 LIGADO");
              }
             else{
              digitalWrite(RelayPin8, HIGH); // Relé 8 Desligado
              toggleState_8 = 1;
              Serial.println("Device8 DESLIGADO");
              }
             delay(500);
      break;
      default : break;      
      }  
}

void with_internet(){
    //Controle de interruptor manual Com Internet
    if (digitalRead(SwitchPin1) == LOW){
      delay(800);
      relayOnOff(1); 
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);   // Estado do Pino Virtual  
    }
    else if (digitalRead(SwitchPin2) == LOW){
      delay(800);
      relayOnOff(2);      
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);   // Estado do Pino Virtual
    }
    else if (digitalRead(SwitchPin3) == LOW){
      delay(800);
      relayOnOff(3);
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);   // Estado do Pino Virtual
    }
    else if (digitalRead(SwitchPin4) == LOW){
      delay(800);
      relayOnOff(4);
      Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);   // Estado do Pino Virtual
    }
    else if (digitalRead(SwitchPin5) == LOW){
      delay(800);
      relayOnOff(5); 
      Blynk.virtualWrite(VPIN_BUTTON_5, toggleState_5);   // Estado do Pino Virtual  
    }
    else if (digitalRead(SwitchPin6) == LOW){
      delay(800);
      relayOnOff(6);      
      Blynk.virtualWrite(VPIN_BUTTON_6, toggleState_6);   // Estado do Pino Virtual
    }
    else if (digitalRead(SwitchPin7) == LOW){
      delay(800);
      relayOnOff(7);
      Blynk.virtualWrite(VPIN_BUTTON_7, toggleState_7);   // Estado do Pino Virtual
    }
    else if (digitalRead(SwitchPin8) == LOW){
      delay(800);
      relayOnOff(8);
      Blynk.virtualWrite(VPIN_BUTTON_8, toggleState_8);   // Estado do Pino Virtual
    }
}
void without_internet(){
    //Controle de interruptor manual Sem Internet
    if (digitalRead(SwitchPin1) == LOW){
      delay(1);
      relayOnOff(1);      
    }
    else if (digitalRead(SwitchPin2) == LOW){
      delay(1);
      relayOnOff(2);
    }
    else if (digitalRead(SwitchPin3) == LOW){
      delay(1);
      relayOnOff(3);
    }
    else if (digitalRead(SwitchPin4) == LOW){
      delay(1);
      relayOnOff(4);
    }
   else if (digitalRead(SwitchPin5) == LOW){
      delay(1);
      relayOnOff(5);      
    }
    else if (digitalRead(SwitchPin6) == LOW){
      delay(1);
      relayOnOff(6);
   }
    else if (digitalRead(SwitchPin7) == LOW){
      delay(1);
      relayOnOff(7);
    }
    else if (digitalRead(SwitchPin8) == LOW){
      delay(1);
      relayOnOff(8);
    }
}

BLYNK_CONNECTED() {
  // Solicitação do Estado Mais Recente no Servidor
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
  Blynk.syncVirtual(VPIN_BUTTON_3);
  Blynk.syncVirtual(VPIN_BUTTON_4);
  Blynk.syncVirtual(VPIN_BUTTON_5);
  Blynk.syncVirtual(VPIN_BUTTON_6);
  Blynk.syncVirtual(VPIN_BUTTON_7);
  Blynk.syncVirtual(VPIN_BUTTON_8);
}

//  Muda o Estado do botão do Aplicativo Quando é pressionado
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

void checkBlynkStatus() { // Chamado a cada 3 segundos por SimpleTimer SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
  wifiFlag = 1;
  digitalWrite(wifiLed, LOW); //WiFi LED Desligado
}
 if (isconnected == true) {
 wifiFlag = 0;
 digitalWrite(wifiLed, HIGH); //WiFi LED Ligado

}
 }

void setup()
  {
  Serial.begin(115200);

  dht.setup(16, DHTesp::DHT22);
   
  currentTemp = dht.getTemperature();
  currentHumidity = dht.getHumidity();
  currentLuminosidade = analogRead(LDR);

  pinMode(LDR,INPUT);
  
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(RelayPin5, OUTPUT);
  pinMode(RelayPin6, OUTPUT);
  pinMode(RelayPin7, OUTPUT);
  pinMode(RelayPin8, OUTPUT);

//  pinMode(wifiLed, OUTPUT);

  digitalWrite(SwitchPin1, LOW);
  digitalWrite(SwitchPin2, LOW);
  digitalWrite(SwitchPin3, LOW);
  digitalWrite(SwitchPin4, LOW);
  digitalWrite(SwitchPin5, LOW);
  digitalWrite(SwitchPin6, LOW);
  digitalWrite(SwitchPin7, LOW);
  digitalWrite(SwitchPin8, LOW);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);
  pinMode(SwitchPin5, INPUT_PULLUP);
  pinMode(SwitchPin6, INPUT_PULLUP);
  pinMode(SwitchPin7, INPUT_PULLUP);
  pinMode(SwitchPin8, INPUT_PULLUP);

  //Durante a partida, todos os relés devem Iniciar DESLIGADOS
  digitalWrite(RelayPin1, HIGH);
  digitalWrite(RelayPin2, HIGH);
  digitalWrite(RelayPin3, HIGH);
  digitalWrite(RelayPin4, HIGH);
  digitalWrite(RelayPin5, HIGH);
  digitalWrite(RelayPin6, HIGH);
  digitalWrite(RelayPin7, HIGH);
  digitalWrite(RelayPin8, HIGH);    

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(30000L, checkBlynkStatus); // Verifique se o servidor Blynk está conectado a cada 2 segundos
  Blynk.config(AUTH);
  
}

void loop()
{  
  if (WiFi.status() != WL_CONNECTED)
{
    Serial.println("WiFi NÃO ESTÁ CONECTADO");
  }
  else
{
    Serial.println("WiFi CONECTADO");
    Blynk.run();
  }
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
  float luminosidade = analogRead(LDR);
  
    if (isnan(humidity) || isnan(temperature))
{
    Serial.println("Falha ao ler os dados dos sensores!");
    return;
   }
    else
{
     Serial.print(temperature);
     Serial.println("ºC");
     Serial.println("----------------------------------");
     Serial.print(humidity);
     Serial.println("----------------------------------");
     Serial.println("%");
   }

  Blynk.virtualWrite(V9, temperature); // Pino Virtual V9 para Tempratura
  Blynk.virtualWrite(V10, humidity); // Pino Virtual V10 para Humidade    

//  LDR = map(analogRead(LDR), 0, 4095, 10, 0);

  float val = analogRead(LDR); 
  Serial.println(val);
  Blynk.virtualWrite(V11,val);
//delay(1000);
  
    {
  timer.run(); // Inicialização do SimpleTimer
  if (wifiFlag == 0)
    with_internet();
  else
    without_internet();
}
  }
