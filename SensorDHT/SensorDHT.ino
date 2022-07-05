/********************************************************
 * CANAL INTERNET E COISAS
 * Serie Sensores - DHT11/DHT22 Umidade e Temperatura
 * 11/2018 - Andre Michelon
 */

// Biblioteca
#include "DHT.h"

// Instancias DHT
DHT dht22(3, DHT22);
DHT dht11(5, DHT11);

void setup() {
  Serial.begin(115200); 
  Serial.println("Teste DHT");

  // Inicializa sensores
  dht22.begin();
  dht11.begin();
}

void loop() {
  // Executa as leituras de cada sensor
  readDHT(dht22, "DHT22");
  readDHT(dht11, "DHT11");
  Serial.println();

  // Aguarda 3s
  delay(3000);
}

void readDHT(DHT dht, String s) {
  // Efetua a leitura e exibe dados
  float h = dht.readHumidity();         // Umidade
  float c = dht.readTemperature();      // Temperatura em Celsius
  float f = dht.readTemperature(true);  // Temperatura em Fahrenheit

  // Verifica se houve erro na leitura
  if (isnan(h) || isnan(c) || isnan(f)) {
    Serial.println("*** Erro na leitura ***");
    return;
  }

  // Exibicao
  Serial.print(s + " U: ");
  Serial.print(h, 1);
  Serial.print("% T: ");
  Serial.print(c, 1);
  Serial.print("°C ");
  Serial.print(f, 1);
  Serial.print("°F ST: ");
  Serial.print(dht.computeHeatIndex(c, h, false), 1);
  Serial.print("°C  ");
}
