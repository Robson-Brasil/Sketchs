//IoT - Automação Residencial
//Dispositivo : ESP32
//Temperatura, Umidade, Luminosidade e Relés
//Autor : Robson Brasil
//Versão : 5
//Última Modificação : 20/07/2021

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>

char auth[] = "WW7eZUxja-yaJ04fANp6KF-z0N1K63Jg";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "RVR 2,4GHz"; // Enter Your WiFi Name
char pass[] = "RodrigoValRobson2021"; // Enter Your Passwword

#define DHTTYPE DHT22
#define DHTPIN  23
#define Rele1   21
#define Rele2   17
#define Rele3   16
#define Rele4    4
#define Rele5    0
#define Rele6    2
#define Rele7   27
#define Rele8   32

#define Virtual1    V1//BOTÃO VIRTUAL V1
#define Virtual2    V2//BOTÃO VIRUTAL V2
#define Virtual3    V3//BOTÃO VIRUTAL V3
#define Virtual4    V4//BOTÃO VIRTUAL V4
#define Virtual5    V5//BOTÃO VIRTUAL V5
#define Virtual6    V6//BOTÃO VIRTUAL V6
#define Virtual7    V7//BOTÃO VIRTUAL V7
#define Virtual8    V8//BOTÃO VIRTUAL V8
#define Virtual9    V9//BOTÃO VIRTUAL V9
#define Virtual10   V10//BOTÃO VIRTUAL V10
#define Virtual11   V11//BOTÃO VIRTUAL V11

int LDR = 36;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  float luminosidade = analogRead(LDR);
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
}

  Blynk.virtualWrite(V9, t);
  Blynk.virtualWrite(V10, h);
  Blynk.virtualWrite(V1, Rele1);
  Blynk.virtualWrite(V2, Rele2);
  Blynk.virtualWrite(V3, Rele3);
  Blynk.virtualWrite(V4, Rele4);
  Blynk.virtualWrite(V5, Rele5);
  Blynk.virtualWrite(V6, Rele6);
  Blynk.virtualWrite(V7, Rele7);
  Blynk.virtualWrite(V8, Rele8);
}

void setup()
{
  pinMode(Rele1, OUTPUT);
  pinMode(Rele2, OUTPUT);
  pinMode(Rele3, OUTPUT);
  pinMode(Rele4, OUTPUT);
  pinMode(Rele5, OUTPUT);
  pinMode(Rele6, OUTPUT);
  pinMode(Rele7, OUTPUT);
  pinMode(Rele8, OUTPUT);
  
  digitalWrite(Rele1, HIGH);
  digitalWrite(Rele2, HIGH);
  digitalWrite(Rele3, HIGH);
  digitalWrite(Rele4, HIGH);
  digitalWrite(Rele5, HIGH);
  digitalWrite(Rele6, HIGH);
  digitalWrite(Rele7, HIGH);
  digitalWrite(Rele8, HIGH);

  pinMode(LDR,INPUT);
  Blynk.begin(auth, ssid, pass);    
  
  Serial.begin(115200);
  dht.begin();
  timer.setInterval(100L, sendSensor);
  Blynk.begin(auth, ssid, pass);
}
BLYNK_CONNECTED(){
  Blynk.syncVirtual(Virtual1);//Relé 1
  Blynk.syncVirtual(Virtual2);//Relé 2
  Blynk.syncVirtual(Virtual3);//Relé 3
  Blynk.syncVirtual(Virtual4);//Relé 4
  Blynk.syncVirtual(Virtual5);//Relé 5
  Blynk.syncVirtual(Virtual6);//Relé 3
  Blynk.syncVirtual(Virtual7);//Relé 4
  Blynk.syncVirtual(Virtual8);//Relé 5  
  Blynk.syncVirtual(Virtual9);//DHT22  - Temperatura
  Blynk.syncVirtual(Virtual10);//DHT22 - Umidade
  Blynk.syncVirtual(Virtual11);//LDR   - Luminosidade
}

void loop()
{
  float luminosidade = analogRead(LDR);  
  float val = analogRead(LDR); 
  Serial.println(val);
  Blynk.virtualWrite(V11,val);
  Blynk.run();
  timer.run();
}
