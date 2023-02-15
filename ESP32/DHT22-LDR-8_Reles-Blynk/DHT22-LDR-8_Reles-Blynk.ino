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
#define DHTPIN  10
#define Rele1   23
#define Rele2   22
#define Rele3   21
#define Rele4   19
#define Rele5   18
#define Rele6   5
#define Rele7   25
#define Rele8   26
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

  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V4, Rele1);
  Blynk.virtualWrite(V5, Rele2);
  Blynk.virtualWrite(V6, Rele3);
  Blynk.virtualWrite(V7, Rele4);
  Blynk.virtualWrite(V8, Rele5);
  Blynk.virtualWrite(V9, Rele6);
  Blynk.virtualWrite(V10, Rele7);
  Blynk.virtualWrite(V11, Rele8);
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

void loop()
{
  float luminosidade = analogRead(LDR);  
  float val = analogRead(LDR); 
  Blynk.virtualWrite(V3,val);  
  Serial.println(val);
  Blynk.run();
  timer.run();
}
